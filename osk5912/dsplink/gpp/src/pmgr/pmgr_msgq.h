/** ============================================================================
 *  @file   pmgr_msgq.h
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr
 *
 *  @desc   Defines the interfaces and data structures for the sub-component
 *          PMGR_MSGQ.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PMGR_MSGQ_H)
#define  PMGR_MSGQ_H


USES (gpptypes.h)
USES (errbase.h)
USES (linkdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   PMGR_MSGQ_Setup
 *
 *  @desc   This function initializes the MSGQ component.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  PMGR MSGQ component must not be initialized before calling this
 *          function.
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_Setup
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Setup () ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Destroy
 *
 *  @desc   This function finalizes the MSGQ component.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_Destroys
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Destroy () ;


/** ============================================================================
 *  @func   PMGR_MSGQ_AllocatorOpen
 *
 *  @desc   This function initializes the MQA component.
 *
 *  @arg    mqaId
 *              ID of the MQA to be opened.
 *  @arg    mqaAttrs
 *              Attributes for initialization of the MQA component.
 *              The structure of the expected attributes is specific to every
 *              MQA.
 *  @arg    mqaInfo
 *              Location to receive the handle to the initialized MQA state
 *              object.
 *
 *  @ret    DSP_SOK
 *              The MQA component has been successfully opened.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *          mqaAttrs must be valid.
 *          mqaInfo must be valid.
 *
 *  @leave  None
 *
 *  @see    MSGQ_AllocatorOpen, LDRV_MSGQ_AllocatorOpen
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_AllocatorOpen (IN  AllocatorId mqaId,
                         IN  Pvoid       mqaAttrs,
                         OUT Pvoid *     mqaInfo) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_AllocatorClose
 *
 *  @desc   This function finalizes the allocator component.
 *
 *  @arg    mqaId
 *              ID of the MQA to be closed.
 *
 *  @ret    DSP_SOK
 *              The MQA component has been successfully closed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *
 *  @leave  None
 *
 *  @see    MSGQ_AllocatorClose, LDRV_MSGQ_AllocatorClose
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_AllocatorClose (IN  AllocatorId mqaId) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_TransportOpen
 *
 *  @desc   This function initializes the MQT component.
 *
 *  @arg    mqtId
 *              ID of the MQT to be opened.
 *  @arg    mqtAttrs
 *              Attributes for initialization of the MQT component.
 *              The structure of the expected attributes is specific to every
 *              MQT.
 *
 *  @ret    DSP_SOK
 *              The MQT component has been successfully opened.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtAttrs must be valid.
 *          PMGR MSGQ component must be initialized before calling this
 *          function.
 *
 *  @leave  None
 *
 *  @see    PMGR_MSGQ_TransportClose
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_TransportOpen (IN  TransportId mqtId, IN  Pvoid mqtAttrs) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_TransportClose
 *
 *  @desc   This function finalizes the MQT component.
 *
 *  @arg    mqtId
 *              ID of the MQT to be closed.
 *
 *  @ret    DSP_SOK
 *              The MQT component has been successfully closed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *
 *  @leave  None
 *
 *  @see    PMGR_MSGQ_TransportOpen
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_TransportClose (IN  TransportId mqtId) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Create
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
 *              The message queue has been successfully created.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_MSGQ_Delete, PMGR_MSGQ_Locate
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Create (IN  MsgQueueId msgqId, IN OPT MsgqAttrs * msgqAttrs) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Delete
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
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msgqId must be valid.
 *          Client should be the owner of msgqId MSGQ.
 *
 *  @leave  None
 *
 *  @see    PMGR_MSGQ_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Delete (IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID, on the specified processor.
 *
 *  @arg    procId
 *              ID of the processor on which the MSGQ is to be located.
 *  @arg    msgqId
 *              ID of the message queue to be located.
 *  @arg    attrs
 *              Attributes for location of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully located.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_ETIMEOUT
 *              Timeout occurred while locating the MSGQ.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ENOTFOUND
 *              The message queue does not exist on the specified processor.
 *
 *  @enter  procId must be valid.
 *          PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msgqId must be valid.
 *          attrs must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgQueueId, PMGR_MSGQ_Put, PMGR_MSGQ_Release
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Locate (IN  ProcessorId       procId,
                  IN  MsgQueueId        msgqId,
                  IN  MsgqLocateAttrs * attrs) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Release
 *
 *  @desc   This function releases the MSGQ located earlier.
 *
 *  @arg    procId
 *              Processor identifier.
 *  @arg    msgqId
 *              ID of the message queue to be released.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully released.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ENOTFOUND
 *              The message queue does not exist on the specified processor.
 *              This error occurs when the MSGQ_Locate was not called / was
 *              unsuccessful for the msgqId on the processor specified.
 *
 *  @enter  procId must be valid.
 *          PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgQueueId, PMGR_MSGQ_Locate
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Release (IN  ProcessorId procId, IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Alloc
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
 *              The message has been successfully allocated.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, PMGR_MSGQ_Put
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Alloc (IN  AllocatorId mqaId, IN  Uint16 size, OUT MsgqMsg  * msg) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Free
 *
 *  @desc   This function frees a message.
 *
 *  @arg    msg
 *              Pointer to the message to be freed.
 *
 *  @ret    DSP_SOK
 *              The message has been successfully freed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, PMGR_MSGQ_Get
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Free (IN  MsgqMsg   msg) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Put
 *
 *  @desc   This function sends a message to the specified MSGQ on a particular
 *          processor.
 *
 *  @arg    procId
 *              ID of the processor on which the MSGQ to which the message is to
 *              be sent, exists.
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
 *              The message has been successfully sent.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ENOTFOUND
 *              The message queue does not exist. This implies that the MSGQ has
 *              not been located before this call was made.
 *
 *  @enter  procId must be valid.
 *          PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msg must be valid.
 *          destMsgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, PMGR_MSGQ_Get
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Put (IN     ProcessorId procId,
               IN     MsgQueueId  destMsgqId,
               IN     MsgqMsg     msg,
               IN OPT Uint16      msgId,
               IN OPT MsgQueueId  srcMsgqId) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Get
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
 *              The message has been successfully received.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_ETIMEOUT
 *              Timeout occurred while receiving the message.
 *
 *  @enter  PMGR MSGQ component must be initialized before calling this
 *          function.
 *          msg must be valid.
 *          msgqId must be valid.
 *          Client should be the owner of msgqId MSGQ.
 *
 *  @leave  None
 *
 *  @see    MsgqMsgHeader, PMGR_MSGQ_Put
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Get (IN  MsgQueueId msgqId, IN  Uint32 timeout, OUT MsgqMsg * msg) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_GetReplyId
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
 *              The reply information has been successfully retrieved.
 *          DSP_ENOTFOUND
 *              Reply information has not been provided by the sender.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msg must be valid.
 *          procId must be valid.
 *          msgqId must be valid.
 *          PMGR MSGQ component must be initialized before calling this
 *          function.
 *
 *  @leave  None
 *
 *  @see    MSGQ_GetReplyId, LDRV_MSGQ_GetReplyId
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_GetReplyId (IN  MsgqMsg       msg,
                      OUT ProcessorId * procId,
                      OUT MsgQueueId *  msgqId) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_SetErrorHandler
 *
 *  @desc   This function allows the user to designate a MSGQ as an
 *          error-handler MSGQ to receive asynchronous error messages from the
 *          transports.
 *
 *  @arg    msgqId
 *              Message queue to receive the error messages.
 *  @arg    mqaId
 *              ID indicating the allocator to be used for allocating the error
 *              messages.
 *
 *  @ret    DSP_SOK
 *              The error handler has been successfully set.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    MSGQ_SetErrorHandler, LDRV_MSGQ_SetErrorHandler
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_SetErrorHandler (IN  MsgQueueId msgqId, OUT Uint16 mqaId) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PMGR_MSGQ_Instrument
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
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          msgqId must be valid.
 *          PMGR MSGQ component must be initialized before calling this
 *          function.
 *          retVal must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqInstrument
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Instrument (IN  ProcessorId      procId,
                      IN  MsgQueueId       msgqId,
                      OUT MsgqInstrument * retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PMGR_MSGQ_Debug
 *
 *  @desc   This function prints the current status of the MSGQ subcomponent.
 *
 *  @arg    procId
 *              ID of the MSGQ on which the message is to be received.
 *  @arg    msgqId
 *              Timeout value to wait for the message (in milliseconds).
 *
 *  @ret    None
 *
 *  @enter  procId must be valid.
 *          msgqId must be valid.
 *          PMGR MSGQ component must be initialized before calling this
 *          function.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
PMGR_MSGQ_Debug (IN  ProcessorId procId, IN  MsgQueueId msgqId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (PMGR_MSGQ_H) */
