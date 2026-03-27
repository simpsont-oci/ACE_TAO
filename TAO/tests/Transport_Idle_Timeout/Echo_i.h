// -*- C++ -*-
// TAO/tests/Idle_Transport_Timeout/Echo_i.h

#ifndef IDLE_TRANSPORT_TIMEOUT_ECHO_I_H
#define IDLE_TRANSPORT_TIMEOUT_ECHO_I_H

#include "testS.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager_T.h"

/**
 * @class Echo_i
 *
 * @brief Servant implementation for Test::Echo.
 *
 * Provides ping/cache_size/shutdown operations used by the
 * Idle_Transport_Timeout regression test.
 */
class Echo_i : public virtual POA_Test::Echo
{
public:
  explicit Echo_i (CORBA::ORB_ptr orb);

  // Test::Echo operations
  char *ping (const char *msg) override;

  /// Return the number of transport-cache entries that are currently
  /// in the ENTRY_IDLE_AND_PURGABLE state.  The test uses this to
  /// confirm that a transport has been created (cache_size > 0) and
  /// later that it has been closed by the idle timer (cache_size == 0).
  CORBA::Long cache_size () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* IDLE_TRANSPORT_TIMEOUT_ECHO_I_H */
