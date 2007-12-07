/** ============================================================================
 *  @file   lmqt.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the local MQT interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LMQT_H)
#define LMQT_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   LmqtObj
 *
 *  @desc   This structure defines the transport object, which has an instance
 *          for every MSGQ created on the processor.
 *
 *  @field  msgQueue
 *              Message repository to queue pending messages.
 *  ============================================================================
 */
typedef struct LmqtObj_tag {
    List *    msgQueue ;
} LmqtObj ;

/** ============================================================================
 *  @name   LmqtState
 *
 *  @desc   This structure defines the transport state object, which exists as
 *          a single instance for the local MQT.
 *
 *  @field  maxNumMsgq
 *              Maximum number of MSGQs that can be created on the local
 *              processor.
 *  @field  msgqHandles
 *              Array of handles to the MSGQ objects for the local MSGQs.
 *  ============================================================================
 */
typedef struct LmqtState_tag {
    Uint16            maxNumMsgq  ;
    LdrvMsgqHandle *  msgqHandles ;
} LmqtState ;


/** ============================================================================
 *  @func   LMQT_Initialize
 *
 *  @desc   This function performs global initialization of the local MQT.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_Setup, LMQT_Finalize
 *  ============================================================================
 */
NORMAL_API
Void
LMQT_Initialize () ;


/** ============================================================================
 *  @func   LMQT_Finalize
 *
 *  @desc   This function performs global finalization of the local MQT.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_Destroy, LMQT_Initialize
 *  ============================================================================
 */
NORMAL_API
Void
LMQT_Finalize () ;


/** ============================================================================
 *  @func   LMQT_Open
 *
 *  @desc   This function opens the local MQT and configures it according to the
 *          user attributes.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    mqtAttrs
 *              Attributes required for initialization of the MQT component.
 *
 *  @ret    DSP_SOK
 *              The local MQT has been successfully initialized.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          mqtAttrs must be valid.
 *
 *  @leave  None
 *
 *  @see    LmqtState, LmqtAttrs, LMQT_Close
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Open (IN  LdrvMsgqTransportHandle mqtHandle, IN  Pvoid mqtAttrs) ;


/** ============================================================================
 *  @func   LMQT_Close
 *
 *  @desc   This function closes the local MQT, and cleans up its state object.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *
 *  @ret    DSP_SOK
 *              This component has been successfully closed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *
 *  @leave  None
 *
 *  @see    LmqtState, LMQT_Open
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Close (IN  LdrvMsgqTransportHandle mqtHandle) ;


/** ============================================================================
 *  @func   LMQT_Create
 *
 *  @desc   This function creates the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              ID of the message queue to be created.
 *  @arg    attrs
 *              Optional attributes for creation of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully created.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    LmqtObj, LMQT_Delete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Create (IN     LdrvMsgqTransportHandle mqtHandle,
             IN     MsgQueueId              msgqId,
             IN OPT MsgqAttrs *             attrs) ;


/** ============================================================================
 *  @func   LMQT_Delete
 *
 *  @desc   This function deletes the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              ID of the message queue to be deleted.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    LmqtObj, LMQT_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Delete (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   LMQT_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              ID of the message queue to be located.
 *  @arg    attrs
 *              Attributes for location of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully located.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              The message queue does not exist among the MSQs managed by this
 *              MQT.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *          attrs must be valid.
 *
 *  @leave  None
 *
 *  @see    LMQT_Release
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Locate (IN  LdrvMsgqTransportHandle mqtHandle,
             IN  MsgQueueId              msgqId,
             IN  MsgqLocateAttrs *       attrs) ;


/** ============================================================================
 *  @func   LMQT_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier
 *          LMQT_Locate () or LMQT_GetReplyId () call.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              ID of the message queue to be released.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully released.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              The message queue has not been previously located.
 *          DSP_EFAIL
 *              General failure
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    LMQT_Locate
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Release (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   LMQT_Get
 *
 *  @desc   This function receives a message on the specified MSGQ.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              ID of the MSGQ on which the message is to be received.
 *  @arg    timeout
 *              Timeout value to wait for the message (in milliseconds).
 *  @arg    msg
 *              Location to receive the message.
 *
 *  @ret    DSP_SOK
 *              The message has been successfully received.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ETIMEOUT
 *              Timeout occurred while receiving the message.
 *          DSP_ENOTCOMPLETE
 *               Operation not complete when SYNC_NOWAIT was specified as
 *               timeout.
 *
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Get (IN  LdrvMsgqTransportHandle mqtHandle,
          IN  MsgQueueId              msgqId,
          IN  Uint32                  timeout,
          OUT MsgqMsg *               msg) ;


/** ============================================================================
 *  @func   LMQT_Put
 *
 *  @desc   This function sends a message to the specified local MSGQ.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              ID of the destination MSGQ.
 *  @arg    msg
 *              Pointer to the message to be sent to the destination MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              The message queue does not exist.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Put (IN  LdrvMsgqTransportHandle mqtHandle,
          IN  MsgQueueId              msgqId,
          IN  MsgqMsg                 msg) ;


/** ============================================================================
 *  @func   LMQT_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID to be used for replying to a
 *          received message.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msg
 *              Message, whose reply MSGQ ID is to be extracted.
 *  @arg    msgqId
 *              Location to receive the ID of the reply MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msg must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_GetReplyId (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgqMsg                 msg,
                 IN  MsgQueueId *            msgqId) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LMQT_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              Message queue identifier.
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
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *          retVal must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Instrument (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgQueueId              msgqId,
                 OUT MsgqInstrument *        retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LMQT_Debug
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MSGQ transport object.
 *  @arg    msgqId
 *              Message queue identifier.
 *
 *  @ret    None.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
LMQT_Debug (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LMQT_H) */
