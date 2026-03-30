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

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* IDLE_TRANSPORT_TIMEOUT_ECHO_I_H */
