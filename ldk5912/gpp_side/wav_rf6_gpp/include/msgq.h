/** ============================================================================
 *  @file   msgq.h
 *
 *  @path   $(DSPLINK)/gpp/src/api/
 *
 *  @desc   Defines the interfaces and data structures for the API sub-component
 *          MSGQ.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (MSGQ_H)
#define  MSGQ_H


USES (gpptypes.h)
USES (errbase.h)
USES (msgqdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  MSGQ_GetMsgId
 *
 *  @desc   This macro returns the message ID of the specified message.
 *  ============================================================================
 */
#define MSGQ_GetMsgId(msg) (((MsgqMsg) (msg))->msgId)

/** ============================================================================
 *  @macro  MSGQ_GetMsgSize
 *
 *  @desc   This macro returns the size of the specified message.
 *  ============================================================================
 */
#define MSGQ_GetMsgSize(msg) (((MsgqMsg) (msg))->size)

/** ============================================================================
 *  @macro  MSGQ_SetMsgId
 *
 *  @desc   This macro sets the message ID in the specified message.
 *  ============================================================================
 */
#define MSGQ_SetMsgId(msg, id) ((MsgqMsg) (msg))->msgId = id

/** ============================================================================
 *  @macro  MSGQ_GetDstQueue
 *
 *  @desc   This macro returns the MsgqQueue handle of the destination message
 *          queue for the specified message.
 *  ============================================================================
 */
#define MSGQ_GetDstQueue(msg) ((Uint32) ID_GPP << 16) | ((MsgqMsg) (msg))->dstId

/** ============================================================================
 *  @macro  MSGQ_SetSrcQueue
 *
 *  @desc   This macro sets the source message queue in the specified message.
 *  ============================================================================
 */
#define MSGQ_SetSrcQueue(msg, msgq) ((MsgqMsg) (msg))->srcId = (MsgqId) msgq ; \
                                    ((MsgqMsg) (msg))->srcProcId = msgq >> 16

/** ============================================================================
 *  @macro  MSGQ_IsLocalQueue
 *
 *  @desc   This macro checks whether the specified queue is a local queue.
 *  ============================================================================
 */
#define MSGQ_IsLocalQueue(msgq) ((msgq >> 16) == ID_GPP)


/** ============================================================================
 *  @func   MSGQ_TransportOpen
 *
 *  @desc   This function initializes the transport associated with the
 *          specified processor. The transport corresponding to the processor ID
 *          specified in the call should be configured in the CFG. This function
 *          should be called only once as part of system initialization after
 *          the client has called PROC_Attach the first time. Subsequent
 *          tasks / threads should not call this function again as it would
 *          result in unpredictable behavior.
 *
 *  @arg    procId
 *              ID of the Processor for which the transport is to be opened.
 *  @arg    attrs
 *              Attributes for initialization of the transport.
 *              The structure of the expected attributes is specific to a
 *              transport.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *          DSP_EACCESSDENIED
 *              Transport already open.
 *
 *  @enter  attrs must be valid.
 *
 *  @leave  None
 *
 *  @see    MSGQ_TransportClose
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_TransportOpen (IN  ProcessorId procId, IN  Pvoid attrs) ;


/** ============================================================================
 *  @func   MSGQ_TransportClose
 *
 *  @desc   This function finalizes the transport associated with the
 *          specified processor.
 *
 *  @arg    procId
 *              ID of the Processor for which the transport is to be closed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *          DSP_EACCESSDENIED
 *              Not the owner.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MSGQ_TransportOpen
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_TransportClose (IN ProcessorId procId) ;


/** ============================================================================
 *  @func   MSGQ_Open
 *
 *  @desc   This function opens the message queue to be used for receiving
 *          messages, identified through the specified message queue name.
 *
 *  @arg    queueName
 *              Name of the message queue to be opened.
 *  @arg    msgqQueue
 *              Location to store the handle to the message queue.
 *  @arg    attrs
 *              Optional attributes for creation of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              Attempt to open more than number of message queues configured.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  queueName must be valid.
 *          msgqQueue must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MsgqAttrs, MSGQ_Close (), MSGQ_Locate ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Open (IN     Pstr        queueName,
           OUT    MsgqQueue * msgqQueue,
           IN OPT MsgqAttrs * attrs) ;


/** ============================================================================
 *  @func   MSGQ_Close
 *
 *  @desc   This function closes the message queue identified by the specified
 *          MSGQ handle
 *
 *  @arg    msgqQueue
 *              Handle to the message queue to be closed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgqQueue must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MSGQ_Open ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Close (IN MsgqQueue msgqQueue) ;


/** ============================================================================
 *  @func   MSGQ_Locate
 *
 *  @desc   This function synchronously locates the message queue identified by
 *          the specified MSGQ name and returns a handle to the located message
 *          queue.
 *
 *  @arg    queueName
 *              Name of the message queue to be located.
 *  @arg    msgqQueue
 *              Location to store the handle to the located message queue.
 *  @arg    attrs
 *              Optional attributes for location of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              The specified message queue could not be located.
 *          DSP_ETIMEOUT
 *              Timeout occurred while locating the message.
 *          DSP_ENOTCOMPLETE
 *               Operation not complete when WAIT_NONE was specified as timeout.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  queueName must be valid.
 *          msgqQueue must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MsgqLocateAttrs, MSGQ_Put (), MSGQ_Release ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Locate (IN     Pstr              queueName,
             OUT    MsgqQueue *       msgqQueue,
             IN OPT MsgqLocateAttrs * attrs) ;


/** ============================================================================
 *  @func   MSGQ_LocateAsync
 *
 *  @desc   This function asynchronously locates the message queue identified by
 *          the specified MSGQ name. An attempt is made to asynchronously
 *          locate the message queue. If found, an MsgqAsyncLocateMsg message is
 *          sent to the specified reply message queue.
 *
 *  @arg    queueName
 *              Name of the message queue to be located.
 *  @arg    replyQueue
 *              Message queue to be used to receive the response message for
 *              asynchronous location.
 *  @arg    attrs
 *              Attributes for asynchronous location of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  queueName must be valid.
 *          replyQueue must be valid.
 *          attrs must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MsgqLocateAsyncAttrs, MSGQ_Put (), MSGQ_Release ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_LocateAsync (IN Pstr                    queueName,
                  IN MsgqQueue               replyQueue,
                  IN MsgqLocateAsyncAttrs *  attrs) ;


/** ============================================================================
 *  @func   MSGQ_Release
 *
 *  @desc   This function releases the message queue identified by the MSGQ
 *          handle that was located earlier.
 *
 *  @arg    msgqQueue
 *              Handle to the message queue to be released.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              The message queue has not been previously located.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgqQueue must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MSGQ_Locate ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Release (IN MsgqQueue msgqQueue) ;


/** ============================================================================
 *  @func   MSGQ_Alloc
 *
 *  @desc   This function allocates a message, and returns the pointer to the
 *          user.
 *
 *  @arg    poolId
 *              ID of the Pool to be used for allocating this message.
 *  @arg    size
 *              Size of the message to be allocated.
 *  @arg    msg
 *              Location to receive the allocated message.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msg must be a valid pointer.
 *          size must be greater than size of MsgqMsgHeader.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, MSGQ_Put ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Alloc (IN PoolId poolId, IN Uint16 size, OUT MsgqMsg * msg) ;


/** ============================================================================
 *  @func   MSGQ_Free
 *
 *  @desc   This function frees a message.
 *
 *  @arg    msg
 *              Pointer to the message to be freed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msg must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, MSGQ_Get ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Free (IN MsgqMsg msg) ;


/** ============================================================================
 *  @func   MSGQ_Put
 *
 *  @desc   This function sends a message to the specified MSGQ.
 *
 *  @arg    msgqQueue
 *              Handle to the destination MSGQ.
 *  @arg    msg
 *              Pointer to the message to be sent to the destination MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgqQueue must be valid.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MsgqMsgHeader, MSGQ_Get ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Put (IN MsgqQueue msgqQueue, IN MsgqMsg msg) ;


/** ============================================================================
 *  @func   MSGQ_Get
 *
 *  @desc   This function receives a message on the specified MSGQ.
 *
 *  @arg    msgqQueue
 *              Handle to the MSGQ on which the message is to be received.
 *  @arg    timeout
 *              Timeout value to wait for the message (in milliseconds).
 *  @arg    msg
 *              Location to receive the message.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *          DSP_ETIMEOUT
 *              Timeout occurred while receiving the message.
 *          DSP_ENOTCOMPLETE
 *               Operation not complete when WAIT_NONE was specified as timeout.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgQueue must be valid.
 *          msgqQueue must be a local queue.
 *          msg must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MsgqMsgHeader, MSGQ_Put ()
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Get (IN MsgqQueue msgqQueue, IN Uint32 timeout, OUT MsgqMsg * msg) ;


/** ============================================================================
 *  @func   MSGQ_GetSrcQueue
 *
 *  @desc   This function returns a handle to the source message queue of a
 *          message to be used for replying to the message.
 *
 *  @arg    msg
 *              Message, whose source MSGQ handle is to be returned.
 *  @arg    msgqQueue
 *              Location to retrieve the handle to the source MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *          DSP_ENOTFOUND
 *              Source information has not been provided by the sender.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msg must be valid.
 *          msgqQueue must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue, MsgqMsgHeader
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_GetSrcQueue (IN MsgqMsg msg, OUT MsgqQueue * msgqQueue) ;


/** ============================================================================
 *  @func   MSGQ_Count
 *
 *  @desc   This API returns the count of the number of messages in a local
 *          message queue.
 *
 *  @arg    msgqQueue
 *              Handle to the MSGQ for which the count is to be retrieved.
 *  @arg    count
 *              Location to receive the message count.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgqQueue must be valid.
 *          msgqQueue must be a local queue.
 *          count must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    MsgqQueue
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Count (IN MsgqQueue msgqQueue, OUT Uint16 * count) ;


/** ============================================================================
 *  @func   MSGQ_SetErrorHandler
 *
 *  @desc   This API allows the user to designate a MSGQ as an error-handler
 *          MSGQ to receive asynchronous error messages from the transports.
 *
 *  @arg    errorQueue
 *              Handle to the message queue to receive the error messages.
 *  @arg    poolId
 *              ID indicating the pool to be used for allocating the error
 *              messages.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MsgqAsyncErrorMsg
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_SetErrorHandler (IN MsgqQueue errorQueue, IN PoolId poolId) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   MSGQ_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @arg    msgqQueue
 *              Handle to the message queue.
 *  @arg    retVal
 *              Location to retrieve the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgqQueue must be valid.
 *          retVal must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    MsgqInstrument
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Instrument (IN MsgqQueue msgqQueue, OUT MsgqInstrument * retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   MSGQ_Debug
 *
 *  @desc   This function prints the current status of the MSGQ subcomponent.
 *
 *  @arg    msgqQueue
 *              Handle to the message queue.
 *
 *  @ret    None
 *
 *  @enter  msgqQueue must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
MSGQ_Debug (IN MsgqQueue msgqQueue) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (MSGQ_H) */
