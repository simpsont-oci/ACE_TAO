// -*- C++ -*-

//=============================================================================
/**
 *  @file    Uring_Asynch_IO.h
 *
 *  The implementation classes for the Linux io_uring asynchronous
 *  operations are defined here in this file.
 */
//=============================================================================

#ifndef ACE_URING_ASYNCH_IO_H
#define ACE_URING_ASYNCH_IO_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)

#include "ace/Asynch_IO_Impl.h"
#include "ace/Atomic_Op.h"
#include "ace/Guard_T.h"
#include "ace/Time_Value.h"
#include "ace/Uring_Proactor.h"

#include <set>
#include <sys/socket.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Uring_Asynch_Operation;

/**
 * @class ACE_Uring_Asynch_Result
 *
 * @brief Base class for io_uring completion tokens.
 *
 * Concrete asynchronous result types derive from this class and adapt
 * an io_uring completion back into the corresponding ACE result
 * callback.
 */
class ACE_Export ACE_Uring_Asynch_Result
  : public virtual ACE_Asynch_Result_Impl
{
public:
  ACE_Uring_Asynch_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                           const void *act,
                           ACE_HANDLE handle,
                           u_long offset,
                           u_long offset_high,
                           ACE_Proactor *proactor);

  virtual ~ACE_Uring_Asynch_Result (void);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0) = 0;

  virtual size_t bytes_transferred (void) const;
  virtual void set_bytes_transferred (size_t n);
  virtual u_long error (void) const;
  virtual void set_error (u_long err);
  virtual const void *act (void) const;
  virtual int success (void) const;
  virtual const void *completion_key (void) const;
  virtual ACE_HANDLE event (void) const;
  virtual u_long offset (void) const;
  virtual u_long offset_high (void) const;
  virtual int priority (void) const;
  virtual int signal_number (void) const;
  virtual int post_completion (ACE_Proactor_Impl *proactor);

  ACE_Handler *handler (void) const;
  ACE_Handler *dispatch_handler (void) const;

  void owner (ACE_Uring_Asynch_Operation *operation);
  ACE_Uring_Asynch_Operation *owner (void) const;

protected:
  ACE_Handler *handler_;
  ACE_Handler::Proxy_Ptr handler_proxy_;
  const void *act_;
  ACE_HANDLE handle_;
  u_long offset_;
  u_long offset_high_;
  ACE_Proactor *proactor_;
  size_t bytes_transferred_;
  u_long error_;
  ACE_Atomic_Op<ACE_Thread_Mutex, ACE_Uring_Asynch_Operation *> owner_;
};

/**
 * @class ACE_Uring_Asynch_Timer
 *
 * @brief Result object used for timer completions posted into the ring.
 */
class ACE_Export ACE_Uring_Asynch_Timer : public ACE_Uring_Asynch_Result
{
public:
  ACE_Uring_Asynch_Timer (const ACE_Handler::Proxy_Ptr &handler_proxy,
                          const void *act,
                          const ACE_Time_Value &tv,
                          ACE_Proactor *proactor);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

private:
  ACE_Time_Value time_;
};

/**
 * @class ACE_Uring_Asynch_Operation
 *
 * @brief Base class for io_uring-backed asynchronous operations.
 */
class ACE_Export ACE_Uring_Asynch_Operation
  : public virtual ACE_Asynch_Operation_Impl
{
public:
  virtual int open (const ACE_Handler::Proxy_Ptr &handler_proxy,
                    ACE_HANDLE handle,
                    const void *completion_key,
                    ACE_Proactor *proactor);

  virtual int cancel (void);
  virtual ACE_Proactor *proactor (void) const;
  ACE_Handler *handler (void);

  int submit_result (ACE_Uring_Asynch_Result *result);
  void register_result (ACE_Uring_Asynch_Result *result);
  void unregister_result (ACE_Uring_Asynch_Result *result);

protected:
  ACE_Uring_Asynch_Operation (ACE_Uring_Proactor *proactor);
  virtual ~ACE_Uring_Asynch_Operation (void);

  ACE_Uring_Proactor *uring_proactor_;
  ACE_Proactor *proactor_;
  ACE_Handler::Proxy_Ptr handler_proxy_;
  ACE_HANDLE handle_;
  ACE_Thread_Mutex pending_results_lock_;
  std::set<ACE_Uring_Asynch_Result *> pending_results_;
};

// ---------------------------------------------------------------------------
// Read stream and file
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Read_Stream_Result
 *
 * @brief io_uring implementation of the read stream/file result.
 */
class ACE_Export ACE_Uring_Asynch_Read_Stream_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Read_File_Result_Impl
{
public:
  ACE_Uring_Asynch_Read_Stream_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                       ACE_HANDLE handle,
                                       ACE_Message_Block *message_block,
                                       size_t bytes_to_read,
                                       const void *act,
                                       ACE_Proactor *proactor,
                                       u_long offset = 0,
                                       u_long offset_high = 0);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  virtual size_t bytes_to_read (void) const;
  virtual ACE_Message_Block &message_block (void) const;
  virtual ACE_HANDLE handle (void) const;

protected:
  ACE_Message_Block *message_block_;
  size_t bytes_to_read_;
};

class ACE_Export ACE_Uring_Asynch_Read_Stream
  : public ACE_Uring_Asynch_Operation,
    public virtual ACE_Asynch_Read_Stream_Impl
{
public:
  ACE_Uring_Asynch_Read_Stream (ACE_Uring_Proactor *proactor);

  virtual int read (ACE_Message_Block &message_block,
                    size_t num_bytes_to_read,
                    const void *act,
                    int priority,
                    int signal_number);
};

class ACE_Export ACE_Uring_Asynch_Read_File
  : public ACE_Uring_Asynch_Read_Stream,
    public ACE_Asynch_Read_File_Impl
{
public:
  ACE_Uring_Asynch_Read_File (ACE_Uring_Proactor *proactor);

  virtual int read (ACE_Message_Block &message_block,
                    size_t num_bytes_to_read,
                    u_long offset,
                    u_long offset_high,
                    const void *act,
                    int priority,
                    int signal_number);

  virtual int read (ACE_Message_Block &message_block,
                    size_t num_bytes_to_read,
                    const void *act,
                    int priority,
                    int signal_number);
};

class ACE_Export ACE_Uring_Asynch_Read_File_Result
  : public ACE_Uring_Asynch_Read_Stream_Result
{
public:
  ACE_Uring_Asynch_Read_File_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                     ACE_HANDLE handle,
                                     ACE_Message_Block *message_block,
                                     size_t bytes_to_read,
                                     const void *act,
                                     ACE_Proactor *proactor,
                                     u_long offset = 0,
                                     u_long offset_high = 0);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);
};

// ---------------------------------------------------------------------------
// Write stream and file
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Write_Stream_Result
 *
 * @brief io_uring implementation of the write stream/file result.
 */
class ACE_Export ACE_Uring_Asynch_Write_Stream_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Write_File_Result_Impl
{
public:
  ACE_Uring_Asynch_Write_Stream_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_write,
     const void *act,
     ACE_Proactor *proactor,
     u_long offset = 0,
     u_long offset_high = 0);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  virtual size_t bytes_to_write (void) const;
  virtual ACE_Message_Block &message_block (void) const;
  virtual ACE_HANDLE handle (void) const;

protected:
  ACE_Message_Block *message_block_;
  size_t bytes_to_write_;
};

class ACE_Export ACE_Uring_Asynch_Write_Stream
  : public ACE_Uring_Asynch_Operation,
    public virtual ACE_Asynch_Write_Stream_Impl
{
public:
  ACE_Uring_Asynch_Write_Stream (ACE_Uring_Proactor *proactor);

  virtual int write (ACE_Message_Block &message_block,
                     size_t bytes_to_write,
                     const void *act,
                     int priority,
                     int signal_number);
};

class ACE_Export ACE_Uring_Asynch_Write_File
  : public ACE_Uring_Asynch_Write_Stream,
    public ACE_Asynch_Write_File_Impl
{
public:
  ACE_Uring_Asynch_Write_File (ACE_Uring_Proactor *proactor);

  virtual int write (ACE_Message_Block &message_block,
                     size_t bytes_to_write,
                     u_long offset,
                     u_long offset_high,
                     const void *act,
                     int priority,
                     int signal_number);

  virtual int write (ACE_Message_Block &message_block,
                     size_t bytes_to_write,
                     const void *act,
                     int priority,
                     int signal_number);
};

class ACE_Export ACE_Uring_Asynch_Write_File_Result
  : public ACE_Uring_Asynch_Write_Stream_Result
{
public:
  ACE_Uring_Asynch_Write_File_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_write,
     const void *act,
     ACE_Proactor *proactor,
     u_long offset = 0,
     u_long offset_high = 0);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);
};

// ---------------------------------------------------------------------------
// Accept and connect
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Accept_Result
 *
 * @brief io_uring implementation of the accept result.
 */
class ACE_Export ACE_Uring_Asynch_Accept_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Accept_Result_Impl
{
public:
  ACE_Uring_Asynch_Accept_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                  ACE_HANDLE listen_handle,
                                  ACE_HANDLE accept_handle,
                                  ACE_Message_Block *message_block,
                                  size_t bytes_to_read,
                                  const void *act,
                                  ACE_Proactor *proactor);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  virtual ACE_HANDLE accept_handle (void) const;
  virtual ACE_Message_Block &message_block (void) const;
  virtual ACE_HANDLE listen_handle (void) const;
  virtual size_t bytes_to_read (void) const;

  struct sockaddr *addr (void);
  socklen_t *addrlen (void);

private:
  ACE_HANDLE accept_handle_;
  ACE_Message_Block *message_block_;
  size_t bytes_to_read_;
  struct sockaddr_storage client_addr_;
  socklen_t addr_len_;
};

class ACE_Export ACE_Uring_Asynch_Accept
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Accept_Impl
{
public:
  ACE_Uring_Asynch_Accept (ACE_Uring_Proactor *proactor);

  virtual int accept (ACE_Message_Block &message_block,
                      size_t bytes_to_read,
                      ACE_HANDLE accept_handle,
                      const void *act,
                      int priority,
                      int signal_number,
                      int addr_family);
};

class ACE_Export ACE_Uring_Asynch_Connect_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Connect_Result_Impl
{
public:
  ACE_Uring_Asynch_Connect_Result (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                   ACE_HANDLE connect_handle,
                                   const void *act,
                                   ACE_Proactor *proactor);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  virtual ACE_HANDLE connect_handle (void) const;
  void connect_handle (ACE_HANDLE handle);

private:
  ACE_HANDLE connect_handle_;
};

class ACE_Export ACE_Uring_Asynch_Connect
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Connect_Impl
{
public:
  ACE_Uring_Asynch_Connect (ACE_Uring_Proactor *proactor);

  virtual int open (const ACE_Handler::Proxy_Ptr &handler_proxy,
                    ACE_HANDLE handle,
                    const void *completion_key,
                    ACE_Proactor *proactor);

  virtual int connect (ACE_HANDLE connect_handle,
                       const ACE_Addr &remote_sap,
                       const ACE_Addr &local_sap,
                       int reuse_addr,
                       const void *act,
                       int priority,
                       int signal_number);
};

// ---------------------------------------------------------------------------
// Datagram (UDP)
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Read_Dgram_Result
 *
 * @brief io_uring implementation of the datagram read result.
 */
class ACE_Export ACE_Uring_Asynch_Read_Dgram_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Read_Dgram_Result_Impl
{
public:
  ACE_Uring_Asynch_Read_Dgram_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_read,
     int flags,
     int protocol_family,
     const void *act,
     ACE_Proactor *proactor);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  virtual ACE_Message_Block *message_block (void) const;
  virtual int remote_address (ACE_Addr &addr) const;
  virtual int flags (void) const;
  virtual ACE_HANDLE handle (void) const;
  virtual size_t bytes_to_read (void) const;

  struct msghdr *msg (void);

private:
  ACE_Message_Block *message_block_;
  size_t bytes_to_read_;
  int flags_;
  struct msghdr msg_;
  struct iovec iov_;
  struct sockaddr_storage remote_addr_;
};

class ACE_Export ACE_Uring_Asynch_Read_Dgram
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Read_Dgram_Impl
{
public:
  ACE_Uring_Asynch_Read_Dgram (ACE_Uring_Proactor *proactor);

  virtual ssize_t recv (ACE_Message_Block *message_block,
                        size_t &number_of_bytes_recvd,
                        int flags,
                        int protocol_family,
                        const void *act,
                        int priority,
                        int signal_number);
};

class ACE_Export ACE_Uring_Asynch_Write_Dgram_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Write_Dgram_Result_Impl
{
public:
  ACE_Uring_Asynch_Write_Dgram_Result
    (const ACE_Handler::Proxy_Ptr &handler_proxy,
     ACE_HANDLE handle,
     ACE_Message_Block *message_block,
     size_t bytes_to_write,
     int flags,
     const void *act,
     ACE_Proactor *proactor);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  virtual ACE_Message_Block *message_block (void) const;
  virtual int flags (void) const;
  virtual ACE_HANDLE handle (void) const;
  virtual size_t bytes_to_write (void) const;

  struct msghdr *msg (void);

private:
  ACE_Message_Block *message_block_;
  size_t bytes_to_write_;
  int flags_;
  struct msghdr msg_;
  struct iovec iov_;
};

class ACE_Export ACE_Uring_Asynch_Write_Dgram
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Write_Dgram_Impl
{
public:
  ACE_Uring_Asynch_Write_Dgram (ACE_Uring_Proactor *proactor);

  virtual ssize_t send (ACE_Message_Block *message_block,
                        size_t &number_of_bytes_sent,
                        int flags,
                        const ACE_Addr &remote_addr,
                        const void *act,
                        int priority,
                        int signal_number);
};

// ---------------------------------------------------------------------------
// Transmit file
// ---------------------------------------------------------------------------

/**
 * @class ACE_Uring_Asynch_Transmit_File_Result
 *
 * @brief io_uring implementation of the transmit file result.
 */
class ACE_Export ACE_Uring_Asynch_Transmit_File_Result
  : public ACE_Uring_Asynch_Result,
    public ACE_Asynch_Transmit_File_Result_Impl
{
public:
  ACE_Uring_Asynch_Transmit_File_Result
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
     ACE_Proactor *proactor);

  virtual void complete (size_t bytes_transferred,
                         int success,
                         const void *completion_key,
                         u_long error = 0);

  virtual ACE_HANDLE socket (void) const;
  virtual ACE_HANDLE file (void) const;
  virtual ACE_Asynch_Transmit_File::Header_And_Trailer *
    header_and_trailer (void) const;
  virtual size_t bytes_to_write (void) const;
  virtual size_t bytes_per_send (void) const;
  virtual u_long flags (void) const;

private:
  ACE_HANDLE file_;
  ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer_;
  size_t bytes_to_write_;
  size_t bytes_per_send_;
  u_long flags_;
};

class ACE_Export ACE_Uring_Asynch_Transmit_File
  : public ACE_Uring_Asynch_Operation,
    public ACE_Asynch_Transmit_File_Impl
{
public:
  ACE_Uring_Asynch_Transmit_File (ACE_Uring_Proactor *proactor);

  virtual int transmit_file
    (ACE_HANDLE file,
     ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
     size_t bytes_to_write,
     u_long offset,
     u_long offset_high,
     size_t bytes_per_send,
     u_long flags,
     const void *act,
     int priority,
     int signal_number);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */

#include /**/ "ace/post.h"
#endif /* ACE_URING_ASYNCH_IO_H */
