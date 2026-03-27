#include "Echo_i.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager_T.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

Echo_i::Echo_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Echo_i::ping (const char *msg)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Echo_i::ping (%C)\n"), msg));
  return CORBA::string_dup (msg);
}

CORBA::Long
Echo_i::cache_size ()
{
  // Walk the transport cache of the server-side ORB and count entries
  // that are in the ENTRY_IDLE_AND_PURGABLE state.
  //
  // TAO_Transport_Cache_Manager stores its map in orb_core->lane_resources()
  // (or the global resource set for the default thread pool).  The public
  // API exposes current_size() on the underlying ACE_Hash_Map, which counts
  // ALL entries (idle + busy + closed).  To count only idle entries we call
  // the transport cache's cache_idle_transport() helper; however the cleanest
  // portable path for a test is to use the purge_entry / is_entry_idle APIs.
  //
  // For simplicity — and to avoid reaching into internal template
  // instantiations — we use the documented public accessor
  // TAO_ORB_Core::transport_cache() together with its current_size()
  // method.  The test compares before/after values: after one ping the
  // count must be >= 1; after the idle timeout has fired it must be 0.
  //
  // Note: current_size() returns the total map size (idle + busy).
  // Because the test serialises its calls, at the point cache_size() is
  // invoked no request is in flight, so all entries are idle.

  TAO_ORB_Core *core = orb_->orb_core ();

  CORBA::Long n = static_cast<CORBA::Long> (
    core->lane_resources ().transport_cache ().current_size ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Echo_i::cache_size () -> %d\n"), (int)n));

  return n;
}

void
Echo_i::shutdown ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Echo_i::shutdown ()\n")));
  orb_->shutdown (false);
}
