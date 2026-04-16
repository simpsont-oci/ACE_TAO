// -*- C++ -*-

//=============================================================================
/**
 *  @file    POSIX_CB_Proactor.h
 *
 *  @author Alexander Libman <alibman@ihug.com.au>
 */
//=============================================================================

#ifndef ACE_POSIX_CB_PROACTOR_H
#define ACE_POSIX_CB_PROACTOR_H

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS)

#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Semaphore.h"
#include "ace/Thread_Mutex.h"
#include "ace/Null_Semaphore.h"

#include "ace/POSIX_Proactor.h"


ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_POSIX_CB_Proactor
 *
 * @brief Implementation of Callback-based Proactor
 * };
 */
class ACE_Export ACE_POSIX_CB_Proactor : public ACE_POSIX_AIOCB_Proactor
{
  class Notification_State
  {
  public:
    explicit Notification_State (ACE_SYNCH_SEMAPHORE &sema);

    void add_pending (void);
    void complete_one (void);
    size_t pending (void) const;

    void add_ref (void);
    void remove_ref (void);

    void detach (void);

  private:
    ACE_Thread_Mutex mutex_;
    ACE_SYNCH_SEMAPHORE *sema_;
    ACE_Atomic_Op<ACE_Thread_Mutex, size_t> pending_callbacks_;
    ACE_Atomic_Op<ACE_Thread_Mutex, size_t> ref_count_;
  };

public:
  virtual Proactor_Type get_impl_type (void);

  /// Destructor.
  virtual ~ACE_POSIX_CB_Proactor (void);

  /// Constructor defines max number asynchronous operations that can
  /// be started at the same time.
  ACE_POSIX_CB_Proactor (size_t max_aio_operations = ACE_AIO_DEFAULT_SIZE);

  /// Close down the Proactor.
  virtual int close (void);

  // This only public so the "extern C" completion function can see it
  // when needed.
  static void aio_completion_func (sigval cb_data);

protected:

  /**
   * Dispatch a single set of events.  If @a wait_time elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (ACE_Time_Value &wait_time);

  /**
   * Block indefinitely until at least one event is dispatched.
   * Dispatch a single set of events.  If @a wait_time elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (void);

  /// Find free slot to store result and aiocb pointer
  virtual ssize_t allocate_aio_slot (ACE_POSIX_Asynch_Result *result);

  /// Initiate an aio operation and track callback lifetime.
  virtual int start_aio (ACE_POSIX_Asynch_Result *result,
                         ACE_POSIX_Proactor::Opcode op);

  /// Notify queue of "post_completed" ACE_POSIX_Asynch_Results
  /// called from post_completion method
  virtual int notify_completion (int sig_num);

  /// Post a result without instantiating the AIOCB notify pipe.
  virtual int post_completion (ACE_POSIX_Asynch_Result *result);

  /**
   * Dispatch a single set of events.  If @a milli_seconds elapses
   * before any events occur, return 0. Return 1 if a completion is
   * dispatched. Return -1 on errors.
   */
  int handle_events_i (u_long milli_seconds);

  /// Semaphore variable to notify
  /// used to wait the first AIO start
  ACE_SYNCH_SEMAPHORE sema_;

  Notification_State *notification_state_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS */
#endif /* ACE_POSIX_CB_PROACTOR_H*/
