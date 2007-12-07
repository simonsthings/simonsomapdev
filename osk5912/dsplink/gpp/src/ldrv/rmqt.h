/** ============================================================================
 *  @file   rmqt.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the remote MQT interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (RMQT_H)
#define RMQT_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   ID_RMQT_CTRL
 *
 *  @desc   This macro defines the internal ID used to identify control
 *          messages.
 *  ============================================================================
 */
#define ID_RMQT_CTRL (Uint16) 0xFF00


/** ============================================================================
 *  @name   RmqtCtrlCmd
 *
 *  @desc   This enumeration defines the types of control commands that are sent
 *          between the MQTs on different processors.
 *
 *  @field  RmqtCtrlCmd_Locate
 *              Locate command.
 *  @field  RmqtCtrlCmd_LocateAck
 *              Locate acknowledgement command.
 *  @field  RmqtCtrlCmd_Exit
 *              Exit command.
 *  ============================================================================
 */
typedef enum {
    RmqtCtrlCmd_Locate     = 0,
    RmqtCtrlCmd_LocateAck  = 1,
    RmqtCtrlCmd_Exit       = 2
} RmqtCtrlCmd ;


/** ============================================================================
 *  @name   RmqtObj
 *
 *  @desc   This structure defines the transport object, which has an instance
 *          for every MSGQ created on the processor.
 *
 *  @field  locateSem
 *              Semaphore used during mqtLocate.
 *  ============================================================================
 */
typedef struct RmqtObj_tag {
    SyncSemObject *    locateSem ;
} RmqtObj ;

/** ============================================================================
 *  @name   RmqtCtrlMsg
 *
 *  @desc   This structure defines the format of the control messages that are
 *          sent between the MQTs on different processors. This structure is
 *          common between the GPP and the DSP, and must be maintained as the
 *          same on both. To ensure this, padding must be added as required
 *          within the structure.
 *
 *  @field  msgHeader
 *              Fixed message header required for all messages.
 *  @field  ctrlMsg
 *              Defines the format of the different control messages.
 *
 *              locateMsg:
 *                  msgqId      -> ID of the MSGQ to be located on
 *                                 the remote processor.
 *                  mqaId       -> MQA ID to allocate async response messages
 *                  timeout     -> Timeout value for sync locate
 *                  replyHandle -> Reply MSGQ handle for async locate
 *                  arg         -> User-defined value passed to locate
 *                  semHandle   -> Semaphore handle for sync locate
 *
 *              locateAckMsg:
 *                  msgqId      -> ID of the MSGQ located on the
 *                                 remote processor.
 *                  mqaId       -> MQA ID to allocate async response
 *                                 messages
 *                  timeout     -> Timeout value for sync locate
 *                  replyHandle -> Reply MSGQ handle for async
 *                                 locate
 *                  arg         -> User-defined value passed to
 *                                 locate
 *                  semHandle   -> Semaphore handle for sync locate
 *                  msgqFound   -> Requested MSGQ was found on the
 *                                 remote processor?
 *                                      0 -> Not found,
 *                                      1 -> Found
 *                  padding     -> Padding for alignment.
 *
 *              No control message is required when the command indicates exit
 *              of the remote MQT.
 *  ============================================================================
 */
typedef struct RmqtCtrlMsg_tag {
    MsgqMsgHeader msgHeader ;
    union {
        struct {
            Uint16      msgqId      ;
            Uint16      mqaId       ;
            Uint32      timeout     ;
            Uint32      replyHandle ;
            Uint32      arg         ;
            Uint32      semHandle   ;
        } locateMsg ;

        struct {
            Uint16      msgqId      ;
            Uint16      mqaId       ;
            Uint32      timeout     ;
            Uint32      replyHandle ;
            Uint32      arg         ;
            Uint32      semHandle   ;
            Uint16      msgqFound   ;
            Uint16      padding     ;
        } locateAckMsg ;
    } ctrlMsg ;
} RmqtCtrlMsg ;

/** ============================================================================
 *  @name   RmqtState
 *
 *  @desc   This structure defines the transport state object, which exists as
 *          a single instance for the remote MQT.
 *
 *  @field  mqtHandle
 *              Handle to the LDRV MSGQ Transport object.
 *  @field  maxNumMsgq
 *              Maximum number of MSGQs that can be created on the remote
 *              processor.
 *  @field  maxMsgSize
 *              Maximum message size (in bytes) supported by the MQT.
 *  @field  msgqHandles
 *              Array of handles to the MSGQ objects for the remote MSGQs.
 *  @field  getBuffer
 *              The buffer to be used for priming the input channel.
 *  @field  msgQueue
 *              Message repository to queue pending messages.
 *  @field  defaultMqaId
 *              The default MQA to be used by the remote MQT.
 *  @field  procId
 *              Processor Id associated with this MQT.
 *  ============================================================================
 */
typedef struct RmqtState_tag {
    LdrvMsgqTransportHandle mqtHandle    ;
    Uint16                  maxNumMsgq   ;
    Uint16                  maxMsgSize   ;
    LdrvMsgqHandle *        msgqHandles  ;
    MsgqMsg                 getBuffer    ;
    List *                  msgQueue     ;
    Uint16                  defaultMqaId ;
    ProcessorId             procId       ;
} RmqtState ;


/** ============================================================================
 *  @func   RMQT_Initialize
 *
 *  @desc   This function performs global initialization of the remote MQT.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_Setup, RMQT_Finalize
 *  ============================================================================
 */
NORMAL_API
Void
RMQT_Initialize () ;


/** ============================================================================
 *  @func   RMQT_Finalize
 *
 *  @desc   This function performs global finalization of the remote MQT.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_Destroy, RMQT_Initialize
 *  ============================================================================
 */
NORMAL_API
Void
RMQT_Finalize () ;


/** ============================================================================
 *  @func   RMQT_Open
 *
 *  @desc   This function opens the remote MQT and configures it according to
 *          the user attributes.
 *
 *  @arg    mqtHandle
 *              Handle to the transport object.
 *  @arg    mqtAttrs
 *              Attributes required for initialization of the MQT component.
 *
 *  @ret    DSP_SOK
 *              The remote MQT has been successfully opened.
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
 *  @see    RmqtState, RmqtAttrs, RMQT_Close
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Open (IN  LdrvMsgqTransportHandle mqtHandle, IN  Pvoid mqtAttrs) ;


/** ============================================================================
 *  @func   RMQT_Close
 *
 *  @desc   This function closes the remote MQT, and cleans up its state object.
 *
 *  @arg    mqtHandle
 *              Handle to the transport object.
 *
 *  @ret    DSP_SOK
 *              The remote MQT has been successfully closed.
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
 *  @see    RmqtState, RMQT_Open
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Close (IN  LdrvMsgqTransportHandle mqtHandle) ;


/** ============================================================================
 *  @func   RMQT_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID.
 *
 *  @arg    mqtHandle
 *              Handle to the transport object.
 *  @arg    msgqId
 *              ID of the message queue to be located.
 *  @arg    attrs
 *              Attributes for location of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully located.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ETIMEOUT
 *              Timeout occurred while locating the MSGQ.
 *          DSP_ENOTFOUND
 *              The message queue does not exist among the MSGQs managed by this
 *              MQT.
 *          DSP_ENOTCOMPLETE
 *               Operation not complete when SYNC_NOWAIT was specified as
 *               timeout.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *          attrs must be valid.
 *
 *  @leave  None
 *
 *  @see    ID_LOCAL_PROCESSOR, RMQT_Release
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Locate (IN  LdrvMsgqTransportHandle mqtHandle,
             IN  MsgQueueId              msgqId,
             IN  MsgqLocateAttrs *       attrs) ;


/** ============================================================================
 *  @func   RMQT_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier
 *          RMQT_Locate () or RMQT_GetReplyId () call.
 *
 *  @arg    mqtHandle
 *              Handle to the transport object.
 *  @arg    msgqId
 *              ID of the message queue to be released.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully released.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              The message queue was not previously located.
 *          DSP_EFAIL
 *              General failure
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    ID_LOCAL_PROCESSOR, RMQT_Locate
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Release (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   RMQT_Put
 *
 *  @desc   This function sends a message to the specified remote MSGQ.
 *
 *  @arg    mqtHandle
 *              Handle to the transport object.
 *  @arg    msgqId
 *              ID of the destination MSGQ.
 *  @arg    msg
 *              Pointer to the message to be sent to the destination MSGQ.
 *
 *  @ret    DSP_SOK
 *              The message has been successfully sent.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_ENOTFOUND
 *              The message queue does not exist. This implies that the MSGQ has
 *              not been located before this call was made.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtHandle must be valid.
 *          msgqId must be valid.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    RMQT_PutCallback
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Put (IN  LdrvMsgqTransportHandle mqtHandle,
          IN  MsgQueueId              msgqId,
          IN  MsgqMsg                 msg) ;


/** ============================================================================
 *  @func   RMQT_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID to be used for replying to a
 *          received message.
 *
 *  @arg    mqtHandle
 *              Handle to the transport object.
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
RMQT_GetReplyId (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgqMsg                 msg,
                 OUT MsgQueueId *            msgqId) ;


/** ============================================================================
 *  @func   RMQT_PutCallback
 *
 *  @desc   This function implements the callback that runs when the message to
 *          be sent to a remote MSGQ has been transferred across the physical
 *          link.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    statusOfIo
 *              Status of the IO requested.
 *  @arg    buffer
 *              Pointer to the message buffer.
 *  @arg    size
 *              Size of the message buffer.
 *  @arg    mqtHandle
 *              Argument associated with the IO request.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          buffer must be valid.
 *          mqtHandle must be valid.
 *
 *  @leave  None
 *
 *  @see    RMQT_Put
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_PutCallback (IN  ProcessorId   procId,
                  IN  DSP_STATUS    statusOfIo,
                  IN  Uint8 *       buffer,
                  IN  Uint32        size,
                  IN  Pvoid         mqtHandle) ;


/** ============================================================================
 *  @func   RMQT_GetCallback
 *
 *  @desc   This function implements the callback that runs when the message has
 *          been received from the DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    statusOfIo
 *              Status of the IO requested.
 *  @arg    buffer
 *              Pointer to the message buffer.
 *  @arg    size
 *              Size of the message buffer.
 *  @arg    mqtHandle
 *              Argument associated with the IO request.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          buffer must be valid.
 *          arg must be valid.
 *
 *  @leave  None
 *
 *  @see    RMQT_Open
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_GetCallback (IN  ProcessorId   procId,
                  IN  DSP_STATUS    statusOfIo,
                  IN  Uint8 *       buffer,
                  IN  Uint32        size,
                  IN  Pvoid         mqtHandle) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   RMQT_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
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
RMQT_Instrument (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgQueueId              msgqId,
                 OUT MsgqInstrument *        retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   RMQT_Debug
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
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
RMQT_Debug (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (RMQT_H) */
