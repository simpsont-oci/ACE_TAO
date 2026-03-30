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
    // Hold a reference to the transport to prevent its destruction, because that would
    // also destruct this idle timer object
    this->transport_->add_reference ();

    int const retval = this->transport_->handle_idle_timeout (current_time, act);

    this->transport_->remove_reference ();

    return retval;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
