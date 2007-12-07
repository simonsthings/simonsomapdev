/** ============================================================================
 *  @file   msgq.h
 *
 *  @path   $(DSPLINK)\gpp\src\api
 *
 *  @desc   Defines the interfaces and data structures for the API sub-component
 *          MSGQ.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (MSGQ_H)
#define  MSGQ_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   MSGQ_AllocatorOpen
 *
 *  @desc   This function initializes an allocator. The allocator id
 *          specified in the call should be configured in the CFG. This function
 *          should be called only once as part of system initialization after
 *          the client has called PROC_Attach the first time. Subsequent
 *          tasks / threads should not call this function again as it would
 *          result in unpredictable behavior.
 *
 *  @arg    mqaId
 *              ID of the MQA to be opened.
 *  @arg    mqaAttrs
 *              Attributes for initialization of the MQA component. The
 *              structure of the expected attributes is specific to an MQA.
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
 *              Allocator already open.
 *
 *  @enter  mqaAttrs must be valid.
 *
 *  @leave  None
 *
 *  @see    MSGQ_AllocatorClose
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_AllocatorOpen (IN  AllocatorId  mqaId, IN  Pvoid mqaAttrs) ;


/** ============================================================================
 *  @func   MSGQ_AllocatorClose
 *
 *  @desc   This function finalizes the allocator component.
 *
 *  @arg    mqaId
 *              ID of the MQA to be closed.
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
 *  @see    MSGQ_AllocatorOpen
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_AllocatorClose (IN  AllocatorId mqaId) ;


/** ============================================================================
 *  @func   MSGQ_TransportOpen
 *
 *  @desc   This function initializes an MQT component. The transport id
 *          specified in the call should be configured in the CFG. This function
 *          should be called only once as part of system initialization after
 *          the client has called PROC_Attach the first time. Subsequent
 *          tasks / threads should not call this function again as it would
 *          result in unpredictable behavior.
 *
 *  @arg    mqtId
 *              ID of the MQT to be opened.
 *  @arg    mqtAttrs
 *              Attributes for initialization of the MQT component.
 *              The structure of the expected attributes is specific to an MQT.
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
 *  @enter  mqtAttrs must be valid.
 *
 *  @leave  None
 *
 *  @see    MSGQ_TransportClose
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_TransportOpen (IN  TransportId mqtId, IN  Pvoid mqtAttrs) ;


/** ============================================================================
 *  @func   MSGQ_TransportClose
 *
 *  @desc   This function finalizes the MQT component.
 *
 *  @arg    mqtId
 *              ID of the MQT to be closed.
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
MSGQ_TransportClose (IN  TransportId mqtId) ;


/** ============================================================================
 *  @func   MSGQ_Create
 *
 *  @desc   This function creates the message queue to be used for receiving
 *          messages, identified through the specified MSGQ ID.
 *
 *  @arg    msgqId
 *              ID of the message queue to be created.
 *  @arg    msgqAttrs
 *              Optional attributes for creation of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  msgqId must be valid.

 *
 *  @leave  None
 *
 *  @see    MSGQ_Delete, MSGQ_Locate
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Create (IN  MsgQueueId msgqId, IN OPT MsgqAttrs * msgqAttrs) ;


/** ============================================================================
 *  @func   MSGQ_Delete
 *
 *  @desc   This function deletes the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @arg    msgqId
 *              ID of the message queue to be deleted.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MSGQ_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Delete (IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   MSGQ_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID, on the specified processor.
 *
 *  @arg    procId
 *              Processor identifier.
 *  @arg    msgqId
 *              ID of the MQT to be opened.
 *  @arg    attrs
 *              Optional attributes for location of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ETIMEOUT
 *              Timeout occurred while locating the message.
 *          DSP_ENOTFOUND
 *              The message queue does not exist on the specified processor.
 *          DSP_ENOTCOMPLETE
 *               Operation not complete when SYNC_NOWAIT was specified as
 *               timeout.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  procId must be valid.
 *          msgqId must be valid.
 *          attrs  must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgQueueId, MSGQ_Put, MSGQ_Release
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Locate (IN  ProcessorId       procId,
             IN  MsgQueueId        msgqId,
             IN  MsgqLocateAttrs * attrs) ;


/** ============================================================================
 *  @func   MSGQ_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier
 *          MSGQ_Locate () or MSGQ_GetReplyId () call.
 *
 *  @arg    procId
 *              Processor identifier.
 *  @arg    msgqId
 *              ID of the MQT to be opened.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ENOTFOUND
 *              The message queue does not exist on the specified processor.
 *              This error occurs when the MSGQ_Locate was not called / was
 *              unsuccessful for the msgqId on the processor specified.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  procId must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgQueueId, MSGQ_Locate
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Release (IN  ProcessorId procId, IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   MSGQ_Alloc
 *
 *  @desc   This function allocates a message, and returns the pointer to the
 *          user.
 *
 *  @arg    mqaId
 *              ID of the MQA to be used for allocating this message.
 *  @arg    size
 *              Size of the message to be allocated.
 *  @arg    msg
 *              Location to receive the allocated message.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  msg must be valid.
 *          size must be greater than size of MsgqMsgHeader.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, MSGQ_Put
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Alloc (IN  AllocatorId mqaId, IN  Uint16 size, OUT MsgqMsg * msg) ;


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
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  msg must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, MSGQ_Get
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Free (IN  MsgqMsg msg) ;


/** ============================================================================
 *  @func   MSGQ_Put
 *
 *  @desc   This function sends a message to the specified MSGQ on the specified
 *          processor.
 *
 *  @arg    procId
 *              Processor identifier on which the MSGQ exists.
 *  @arg    destMsgqId
 *              ID of the destination MSGQ.
 *  @arg    msg
 *              Pointer to the message to be sent to the destination MSGQ.
 *  @arg    msgId
 *              Optional message ID to be associated with the message.
 *  @arg    srcMsgqId
 *              Optional ID of the source MSGQ to receive reply messages.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ENOTFOUND
 *              The message queue does not exist on the specified processor.
 *
 *  @enter  procId must be valid.
 *          msg must be valid.
 *          destMsgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, MSGQ_Get
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Put (IN     ProcessorId procId,
          IN     MsgQueueId  destMsgqId,
          IN     MsgqMsg     msg,
          IN OPT Uint16      msgId,
          IN OPT MsgQueueId  srcMsgqId) ;


/** ============================================================================
 *  @func   MSGQ_Get
 *
 *  @desc   This function receives a message on the specified MSGQ.
 *
 *  @arg    msgqId
 *              ID of the MSGQ on which the message is to be received.
 *  @arg    timeout
 *              Timeout value to wait for the message (in milliseconds).
 *  @arg    msg
 *              Location to receive the message.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ETIMEOUT
 *              Timeout occurred while receiving the message.
 *          DSP_ENOTCOMPLETE
 *               Operation not complete when SYNC_NOWAIT was specified as
 *               timeout.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  msg must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, MSGQ_Put
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Get (IN  MsgQueueId msgqId, IN  Uint32 timeout, OUT MsgqMsg * msg) ;


/** ============================================================================
 *  @func   MSGQ_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID and processor ID to be used for
 *          replying to a received message.
 *
 *  @arg    msg
 *              Message, whose reply MSGQ ID is to be extracted.
 *  @arg    procId
 *              Location to retrieve the ID of the processor where the reply
 *              MSGQ resides.
 *  @arg    msgqId
 *              Location to retrieve the ID of the reply MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  msg must be valid.
 *          procId must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_GetReplyId (IN  MsgqMsg       msg,
                 OUT ProcessorId * procId,
                 OUT MsgQueueId *  msgqId) ;


/** ============================================================================
 *  @func   MSGQ_SetErrorHandler
 *
 *  @desc   This API allows the user to designate a MSGQ as an error-handler
 *          MSGQ to receive asynchronous error messages from the transports.
 *
 *  @arg    msgqId
 *              Message queue to receive the error messages.
 *  @arg    mqaId
 *              ID indicating the allocator to be used for allocating the error
 *              messages.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqAsyncErrorMsg
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_SetErrorHandler (IN  MsgQueueId msgqId, IN  Uint16 mqaId) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   MSGQ_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @arg    procId
 *              Processor identifier.
 *  @arg    msgqId
 *              Message queue identifier.
 *  @arg    retVal
 *              Location to retrieve the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  procId must be valid.
 *          msgqId must be valid.
 *          retVal must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqInstrument
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Instrument (IN  ProcessorId      procId,
                 IN  MsgQueueId       msgqId,
                 OUT MsgqInstrument * retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   MSGQ_Debug
 *
 *  @desc   This function prints the current status of the MSGQ subcomponent.
 *
 *  @arg    procId
 *              Processor identifier.
 *  @arg    msgqId
 *              Message queue identifier.
 *
 *  @ret    None
 *
 *  @enter  procId must be valid.
 *  @enter  msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
MSGQ_Debug (IN  ProcessorId procId, IN  MsgQueueId msgqId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (MSGQ_H) */
