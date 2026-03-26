/**
 *  @file    Transport_Idle_Timer.h
 *
 *  Reactor timer that fires when a transport has been idle for the
 * configured transport_idl_timeout period and triggers auto-close.
 *
 *  @author  Johnny Willemsen
 */

#ifndef TAO_TRANSPORT_IDLE_TIMER_H
#define TAO_TRANSPORT_IDLE_TIMER_H

#include /**/ "ace/pre.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;

namespace TAO
{
  /**
  * @class Transport_Idle_Timer
  *
  * @brief One-shot reactor timer that closes an idle transport.
  *
  * Created by TAO_Transport::schedule_idle_timer() when a transport
  * enters the ENTRY_IDLE_AND_PURGABLE state.  Cancelled if the
  * transport is reacquired for a new request before the timer fires.
  */
  class Transport_Idle_Timer : public ACE_Event_Handler
  {
  public:
    explicit Transport_Idle_Timer (TAO_Transport *transport);
    ~Transport_Idle_Timer () override = default;

    /// Reactor callback — close the transport if still idle.
    int handle_timeout (const ACE_Time_Value &current_time,
                        const void *act = nullptr) override;

  private:
    // todo, var
    TAO_Transport *transport_;  ///< Non-owning pointer
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_IDLE_TRANSPORT_TIMER_H */
