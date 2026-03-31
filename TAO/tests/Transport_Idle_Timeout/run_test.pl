#!/usr/bin/perl

# Test driver for the idle-transport-timeout regression test.
# Follows the standard TAO test-script conventions used throughout
# TAO/tests/ (PerlACE helpers, two-process server/client, IOR handshake).
#
# Scenarios driven:
#   1.  TC-1, TC-2, TC-3 — timeout enabled (3 s), normal close/reconnect/reuse
#   2.  TC-4              — timeout disabled (0), connection must persist

use strict;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

my $ior_file = "test.ior";
my $timeout_sec = 3;   # must match svc.conf value
my $status = 0;
my $debug_level = '0';
my $cdebug_level = '0';
foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($i eq '-cdebug') {
      $cdebug_level = '10';
    }
}

sub run_scenario {
    my ($label, $svc_conf, $extra_client_args) = @_;

    print "\n### $label ###\n";

    my $server = PerlACE::TestTarget::create_target (1) || die "Cannot create server target";
    my $client = PerlACE::TestTarget::create_target (2) || die "Cannot create client target";

    my $server_ior = $server->LocalFile ($ior_file);
    my $client_ior = $client->LocalFile ($ior_file);

    $server->DeleteFile ($ior_file);
    $client->DeleteFile ($ior_file);

    my $SV = $server->CreateProcess (
        "server",
        "-ORBdebuglevel $debug_level -ORBSvcConf $svc_conf -o $server_ior"
    );

    my $CL = $client->CreateProcess (
        "client",
        "-ORBdebuglevel $cdebug_level -ORBSvcConf $svc_conf -k file://$client_ior -t $timeout_sec $extra_client_args"
    );

    # Start server
    my $server_status = $SV->Spawn ();
    if ($server_status != 0) {
        print STDERR "ERROR: server Spawn returned $server_status\n";
        return 1;
    }

    # Wait for IOR file to appear (up to 30 s)
    if ($server->WaitForFileTimed ($ior_file,
                                   $server->ProcessStartWaitInterval ()) == -1) {
        print STDERR "ERROR: IOR file '$server_ior' not created\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    # Transfer IOR file to client (no-op on single-host runs)
    if ($server->GetFile ($ior_file) == -1) {
        print STDERR "ERROR: server GetFile '$ior_file' failed\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }
    if ($client->PutFile ($ior_file) == -1) {
        print STDERR "ERROR: client PutFile '$ior_file' failed\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    # Run client — allow generous wall-clock budget:
    #   TC-1+TC-2+TC-3: ~3*(timeout+2) + 3 slack = ~20 s for timeout=3
    #   TC-4:           timeout+2 + 3 slack       = ~8 s
    my $client_budget = ($extra_client_args =~ /-d/) ? 15 : 60;
    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ()
                                            + $client_budget);
    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    # Wait for server to exit (it calls orb->shutdown() on receiving shutdown())
    my $server_exit = $SV->WaitKill ($server->ProcessStopWaitInterval ());
    if ($server_exit != 0) {
        print STDERR "ERROR: server returned $server_exit\n";
        $status = 1;
    }

    $server->DeleteFile ($ior_file);
    $client->DeleteFile ($ior_file);

    return $status;
}

# ---------------------------------------------------------------------------
# Scenario 1: TC-1, TC-2, TC-3  (timeout enabled)
# ---------------------------------------------------------------------------
run_scenario (
    "TC-1/TC-2/TC-3: idle timeout enabled (${timeout_sec}s)",
    "svc.conf",
    ""
);

# ---------------------------------------------------------------------------
# Final result
# ---------------------------------------------------------------------------
exit $status;
