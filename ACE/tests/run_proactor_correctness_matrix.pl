#!/usr/bin/env perl

use strict;
use warnings;

use Cwd qw(abs_path);
use File::Basename qw(dirname);
use File::Copy qw(copy);
use File::Path qw(make_path);
use Getopt::Long qw(GetOptions);
use POSIX qw(WNOHANG strftime);
use Text::ParseWords qw(shellwords);
use Time::HiRes qw(sleep time);

my $script_dir = abs_path(dirname($0));
my $ace_root = abs_path("$script_dir/..");
my $log_dir = "$script_dir/log";
my $matrix_root = "$log_dir/proactor_matrix";
my $run_id = $ENV{RUN_ID} || strftime('%Y%m%d-%H%M%S', localtime);
my $run_dir = "$matrix_root/$run_id";

$ENV{ACE_ROOT} = $ace_root;
$ENV{LD_LIBRARY_PATH} = join(
  ':',
  grep { defined $_ && $_ ne '' }
    ("$ace_root/lib", $script_dir, $ENV{LD_LIBRARY_PATH})
);

my $timeout_secs = $ENV{TIMEOUT_SECS} // 180;
my $base_port = $ENV{BASE_PORT} // 20000;
my $include_default = $ENV{INCLUDE_DEFAULT} // 0;
my $fail_fast = $ENV{FAIL_FAST} // 0;
my $run_network_udp = $ENV{RUN_NETWORK_UDP} // 1;
my $expected_fail_backends = $ENV{EXPECTED_FAIL_BACKENDS} // '';

my @requested_tests;
my @requested_backends;
my $list_only = 0;
my $help = 0;

sub usage {
  print <<'EOF';
Usage: ./run_proactor_correctness_matrix.pl [options]

Options:
  --test <name>       Run only the named test case. May be repeated.
  --backend <name>    Run only the named backend. May be repeated.
  --list              Print the resolved test/backend matrix and exit.
  --help              Show this help.

Environment:
  BASE_PORT=20000         Starting port for tests that accept -p.
  TIMEOUT_SECS=180        Per-test timeout.
  RUN_ID=<label>          Output directory label under log/proactor_matrix.
  INCLUDE_DEFAULT=0|1     Include the shared "default" backend selection.
  RUN_NETWORK_UDP=0|1     Include the UDP correctness variant of
                          Proactor_Network_Performance_Test.
  FAIL_FAST=0|1           Stop on the first failing matrix entry.
  EXPECTED_FAIL_BACKENDS  Space-separated backends whose nonzero exits
                          should be recorded as expected failures.
EOF
}

sub require_file {
  my ($path, $message) = @_;
  if (!-e $path) {
    print STDERR "error: $message\n";
    exit 1;
  }
}

sub contains_value {
  my ($needle, @values) = @_;
  for my $value (@values) {
    return 1 if $value eq $needle;
  }
  return 0;
}

sub shell_quote {
  my ($value) = @_;
  return "''" if !defined $value || $value eq '';
  return $value if $value =~ /\A[-+A-Za-z0-9_.,\/:=]+\z/;
  $value =~ s/'/'\\''/g;
  return "'$value'";
}

sub backend_is_expected_fail {
  my ($backend) = @_;
  return $expected_fail_backends =~ /(?:^|\s)\Q$backend\E(?:\s|$)/ ? 1 : 0;
}

sub test_needs_port {
  my ($test_name) = @_;
  return ($test_name eq 'Proactor_Network_Performance_Test'
       || $test_name eq 'Proactor_Test'
       || $test_name eq 'Proactor_Test_IPV6'
       || $test_name eq 'Proactor_UDP_Test'
       || $test_name eq 'Proactor_Scatter_Gather_Test') ? 1 : 0;
}

sub interpret_wait_status {
  my ($status) = @_;
  return 255 if !defined $status;
  return 128 + ($status & 127) if ($status & 127);
  return $status >> 8;
}

sub run_command_with_timeout {
  my ($cmd_ref, $stdout_log, $timeout) = @_;

  my $pid = fork();
  if (!defined $pid) {
    die "fork failed: $!";
  }

  if ($pid == 0) {
    open STDOUT, '>', $stdout_log or do {
      print STDERR "failed to open $stdout_log: $!\n";
      exit 127;
    };
    open STDERR, '>&', \*STDOUT or exit 127;
    exec { $cmd_ref->[0] } @$cmd_ref or do {
      print STDERR "exec failed for $cmd_ref->[0]: $!\n";
      exit 127;
    };
  }

  my $deadline = time() + $timeout;
  while (1) {
    my $wait_pid = waitpid($pid, WNOHANG);
    if ($wait_pid == $pid) {
      return interpret_wait_status($?);
    }
    if ($wait_pid == -1) {
      return 255;
    }

    if (time() >= $deadline) {
      kill 'TERM', $pid;
      my $grace_deadline = time() + 2;
      while (time() < $grace_deadline) {
        my $grace_pid = waitpid($pid, WNOHANG);
        return interpret_wait_status($?) if $grace_pid == $pid;
        last if $grace_pid == -1;
        sleep 0.1;
      }
      kill 'KILL', $pid;
      waitpid($pid, 0);
      return 124;
    }

    sleep 0.1;
  }
}

Getopt::Long::Configure('no_auto_abbrev');
my $ok = GetOptions(
  'test=s@' => \@requested_tests,
  'backend=s@' => \@requested_backends,
  'list' => \$list_only,
  'help' => \$help,
);

if (!$ok || $help) {
  usage();
  exit($ok ? 0 : 2);
}

my @backends = qw(aiocb sig cb uring);
@backends = ('default', @backends) if $include_default;

if (@requested_backends) {
  my @filtered_backends;
  for my $backend (@requested_backends) {
    if (!contains_value($backend, @backends)) {
      print STDERR "error: unsupported backend selection: $backend\n";
      exit 2;
    }
    push @filtered_backends, $backend;
  }
  @backends = @filtered_backends;
}

my @all_cases = (
  { test_name => 'Proactor_Contract_Test', variant => '', args => '' },
  { test_name => 'Proactor_File_Test', variant => '', args => '' },
  {
    test_name => 'Proactor_Network_Performance_Test',
    variant => 'tcp',
    args => '-n 8 -m 32 -b 512 -w 4 -T 2',
  },
  { test_name => 'Proactor_Scatter_Gather_Test', variant => '', args => '' },
  { test_name => 'Proactor_Stress_Test', variant => '', args => '' },
  { test_name => 'Proactor_Test', variant => '', args => '-n 2 -c 4 -x 16384' },
  { test_name => 'Proactor_Test_IPV6', variant => '', args => '-n 2 -c 4 -x 16384' },
  { test_name => 'Proactor_Timer_Test', variant => '', args => '' },
  { test_name => 'Proactor_UDP_Test', variant => '', args => '-n 2 -c 4 -x 16384' },
);

if ($run_network_udp) {
  push @all_cases, {
    test_name => 'Proactor_Network_Performance_Test',
    variant => 'udp',
    args => '-u -n 8 -m 32 -b 512 -w 4 -T 2',
  };
}

my @cases;
if (!@requested_tests) {
  @cases = @all_cases;
} else {
  for my $case (@all_cases) {
    my $case_label = $case->{test_name};
    $case_label .= ':' . $case->{variant} if $case->{variant} ne '';
    if (contains_value($case->{test_name}, @requested_tests)
        || contains_value($case_label, @requested_tests)) {
      push @cases, $case;
    }
  }
}

if (!@cases) {
  print STDERR "error: no test cases selected\n";
  exit 2;
}

require_file(
  "$ace_root/ace/config.h",
  "missing $ace_root/ace/config.h; configure ACE before running the matrix",
);
require_file(
  "$ace_root/include/makeinclude/platform_macros.GNU",
  "missing $ace_root/include/makeinclude/platform_macros.GNU; configure ACE before running the matrix",
);

make_path($log_dir, $run_dir);

if ($list_only) {
  print "run_dir=$run_dir\n";
  print 'backends=' . join(' ', @backends) . "\n";
  for my $case (@cases) {
    my $case_label = $case->{test_name};
    $case_label .= ':' . $case->{variant} if $case->{variant} ne '';
    my $args = $case->{args} ne '' ? $case->{args} : '<none>';
    print "case $case_label args=$args\n";
    for my $backend (@backends) {
      print "  backend $backend\n";
    }
  }
  exit 0;
}

my $pass_count = 0;
my $fail_count = 0;
my $skip_count = 0;
my $xfail_count = 0;
my $port_offset = 0;

sub run_case {
  my ($case, $backend) = @_;

  my $test_name = $case->{test_name};
  my $variant = $case->{variant};
  my $args_string = $case->{args};
  my $case_label = $test_name;
  $case_label .= ':' . $variant if $variant ne '';

  my $binary = "$script_dir/$test_name";
  my $native_log = "$log_dir/$test_name.log";
  my $archive_base = "$run_dir/$test_name";
  $archive_base .= ".$variant" if $variant ne '';
  $archive_base .= ".$backend";
  my $stdout_log = "$archive_base.stdout-stderr.log";
  my $archived_native_log = "$archive_base.log";

  require_file($binary, "missing $binary; build the Proactor tests before running the matrix");

  my @extra_args = $args_string ne '' ? shellwords($args_string) : ();
  if (test_needs_port($test_name)) {
    push @extra_args, ('-p', $base_port + $port_offset);
    ++$port_offset;
  }

  my @cmd = ($binary, '-t', $backend, @extra_args);

  unlink $native_log, $stdout_log, $archived_native_log;

  print "[RUN ] $case_label backend=$backend\n";
  print '       command:' . join('', map { ' ' . shell_quote($_) } @cmd) . "\n";

  my $rc = run_command_with_timeout(\@cmd, $stdout_log, $timeout_secs);

  if (-f $native_log) {
    copy($native_log, $archived_native_log)
      or die "copy $native_log -> $archived_native_log failed: $!";
  }

  if ($rc == 0) {
    ++$pass_count;
    print "[PASS] $case_label backend=$backend\n";
    return 1;
  }

  if (backend_is_expected_fail($backend)) {
    ++$xfail_count;
    print "[XFAIL] $case_label backend=$backend rc=$rc\n";
    print "        stdout/stderr: $stdout_log\n";
    print "        test log: $archived_native_log\n" if -f $archived_native_log;
    return 1;
  }

  ++$fail_count;
  print STDERR "[FAIL] $case_label backend=$backend rc=$rc\n";
  print STDERR "       stdout/stderr: $stdout_log\n";
  print STDERR "       test log: $archived_native_log\n" if -f $archived_native_log;
  return 0;
}

for my $case (@cases) {
  my $case_label = $case->{test_name};
  $case_label .= ':' . $case->{variant} if $case->{variant} ne '';

  for my $backend (@backends) {
    if (!run_case($case, $backend) && $fail_fast) {
      print STDERR "\nSummary: pass=$pass_count fail=$fail_count xfail=$xfail_count skip=$skip_count run_dir=$run_dir\n";
      exit 1;
    }
  }
}

print "\nSummary: pass=$pass_count fail=$fail_count xfail=$xfail_count skip=$skip_count run_dir=$run_dir\n";
exit($fail_count == 0 ? 0 : 1);
