// -*- C++ -*-

//=============================================================================
/**
 *  @file    Uring_Proactor.h
 *
 *  The implementation classes for the Linux io_uring Proactor are
 *  defined here in this file.
 */
//=============================================================================

#ifndef ACE_URING_PROACTOR_H
#define ACE_URING_PROACTOR_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)

#include "ace/OS_NS_Thread.h"
#include "ace/Proactor_Impl.h"
#include "ace/Thread_Mutex.h"

#include <liburing.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Uring_Asynch_Result;

/**
 * @class ACE_Uring_Proactor
 *
 * @brief Linux io_uring implementation of the Proactor.
 *
 * This class provides the factory methods and completion dispatch
 * support needed by the ACE asynchronous operation classes when the
 * backend is implemented on top of Linux io_uring.
 */
class ACE_Export ACE_Uring_Proactor : public ACE_Proactor_Impl
{
public:
  ACE_Uring_Proactor (size_t entries = 256);
  virtual ~ACE_Uring_Proactor (void);

  virtual int close (void);

  virtual int register_handle (ACE_HANDLE handle,
                               const void *completion_key);

  virtual int handle_events (ACE_Time_Value &wait_time);
  virtual int handle_events (void);

  virtual int wake_up_dispatch_threads (void);
  virtual int close_dispatch_threads (int wait);
  virtual size_t number_of_threads (void) const;
  virtual void number_of_threads (size_t threads);
  virtual ACE_HANDLE get_handle (void) const;

  // Methods used to create Asynch I/O factory and result objects.

  virtual ACE_Asynch_Read_Stream_Impl *create_asynch_read_stream (void);
  virtual ACE_Asynch_Write_Stream_Impl *create_asynch_write_stream (void);
  virtual ACE_Asynch_Read_File_Impl *create_asynch_read_file (void);
  virtual ACE_Asynch_Write_File_Impl *create_asynch_write_file (void);
  virtual ACE_Asynch_Accept_Impl *create_asynch_accept (void);
  virtual ACE_Asynch_Connect_Impl *create_asynch_connect (void);
  virtual ACE_Asynch_Transmit_File_Impl *create_asynch_transmit_file (void);
  virtual ACE_Asynch_Read_Dgram_Impl *create_asynch_read_dgram (void);
  virtual ACE_Asynch_Write_Dgram_Impl *create_asynch_write_dgram (void);

  virtual ACE_Asynch_Read_Stream_Result_Impl *
    create_asynch_read_stream_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                      ACE_HANDLE handle,
                                      ACE_Message_Block &message_block,
                                      size_t bytes_to_read,
                                      const void *act,
                                      ACE_HANDLE event = ACE_INVALID_HANDLE,
                                      int priority = 0,
                                      int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Write_Stream_Result_Impl *
    create_asynch_write_stream_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                       ACE_HANDLE handle,
                                       ACE_Message_Block &message_block,
                                       size_t bytes_to_write,
                                       const void *act,
                                       ACE_HANDLE event = ACE_INVALID_HANDLE,
                                       int priority = 0,
                                       int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Read_File_Result_Impl *
    create_asynch_read_file_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                    ACE_HANDLE handle,
                                    ACE_Message_Block &message_block,
                                    size_t bytes_to_read,
                                    const void *act,
                                    u_long offset,
                                    u_long offset_high,
                                    ACE_HANDLE event = ACE_INVALID_HANDLE,
                                    int priority = 0,
                                    int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Write_File_Result_Impl *
    create_asynch_write_file_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                     ACE_HANDLE handle,
                                     ACE_Message_Block &message_block,
                                     size_t bytes_to_write,
                                     const void *act,
                                     u_long offset,
                                     u_long offset_high,
                                     ACE_HANDLE event = ACE_INVALID_HANDLE,
                                     int priority = 0,
                                     int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Read_Dgram_Result_Impl *
    create_asynch_read_dgram_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                     ACE_HANDLE handle,
                                     ACE_Message_Block *message_block,
                                     size_t bytes_to_read,
                                     int flags,
                                     int protocol_family,
                                     const void *act,
                                     ACE_HANDLE event = ACE_INVALID_HANDLE,
                                     int priority = 0,
                                     int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Write_Dgram_Result_Impl *
    create_asynch_write_dgram_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                      ACE_HANDLE handle,
                                      ACE_Message_Block *message_block,
                                      size_t bytes_to_write,
                                      int flags,
                                      const void *act,
                                      ACE_HANDLE event = ACE_INVALID_HANDLE,
                                      int priority = 0,
                                      int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Accept_Result_Impl *
    create_asynch_accept_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                 ACE_HANDLE listen_handle,
                                 ACE_HANDLE accept_handle,
                                 ACE_Message_Block &message_block,
                                 size_t bytes_to_read,
                                 const void *act,
                                 ACE_HANDLE event = ACE_INVALID_HANDLE,
                                 int priority = 0,
                                 int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Connect_Result_Impl *
    create_asynch_connect_result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                  ACE_HANDLE connect_handle,
                                  const void *act,
                                  ACE_HANDLE event = ACE_INVALID_HANDLE,
                                  int priority = 0,
                                  int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Transmit_File_Result_Impl *
    create_asynch_transmit_file_result
      (const ACE_Handler::Proxy_Ptr &handler_proxy,
       ACE_HANDLE socket,
       ACE_HANDLE file,
       ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
       size_t bytes_to_write,
       u_long offset,
       u_long offset_high,
       size_t bytes_per_send,
       u_long flags,
       const void *act,
       ACE_HANDLE event = ACE_INVALID_HANDLE,
       int priority = 0,
       int signal_number = ACE_SIGRTMIN);

  virtual ACE_Asynch_Result_Impl *
    create_asynch_timer (const ACE_Handler::Proxy_Ptr &handler_proxy,
                         const void *act,
                         const ACE_Time_Value &tv,
                         ACE_HANDLE event = ACE_INVALID_HANDLE,
                         int priority = 0,
                         int signal_number = 0);

  virtual int post_wakeup_completions (int how_many);

  /// Serialize SQE preparation and submission.
  ACE_Thread_Mutex &sq_mutex (void);
  bool is_initialized (void) const;

  /// Access to the underlying ring for operation implementations.
  struct io_uring_sqe *get_sqe (void);
  int submit_sqe (void);
  int submit_sqe_if_necessary (void);
  int submit_pending_sqe (void);
  int signal_submitter (void);

protected:
  int process_cqes (int max_to_process,
                    const ACE_Time_Value *wait_time = 0);

private:
  enum
  {
    DEFAULT_CQE_BATCH_SIZE = 256,
    DEFAULT_SUBMIT_BATCH_SIZE = 8
  };

  int arm_submit_wakeup_locked (void);
  void drain_submit_wakeup_locked (void);
  bool on_dispatch_thread (void) const;

  struct io_uring ring_;
  bool is_initialized_;
  bool submit_signal_pending_;
  ACE_HANDLE submit_wakeup_handle_;
  ACE_thread_t dispatch_thread_id_;
  mutable ACE_Thread_Mutex dispatch_mutex_;
  mutable ACE_Thread_Mutex sq_mutex_;
  mutable ACE_Thread_Mutex cq_mutex_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */

#include /**/ "ace/post.h"
#endif /* ACE_URING_PROACTOR_H */
