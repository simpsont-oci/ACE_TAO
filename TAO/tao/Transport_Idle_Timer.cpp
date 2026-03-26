#include "tao/Transport_Idle_Timer.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Idle_Timer::Transport_Idle_Timer (TAO_Transport *transport)
    : transport_ (transport)
  {
  }

  int
  Transport_Idle_Timer::handle_timeout (const ACE_Time_Value &current_time, const void* act)
  {
    return this->transport_->handle_idle_timeout (current_time, act);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
