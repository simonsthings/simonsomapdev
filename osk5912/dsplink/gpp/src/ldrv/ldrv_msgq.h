/** ============================================================================
 *  @file   ldrv_msgq.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the interface and structures of LDRV MSGQ driver.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LDRV_MSGQ_H)
#define LDRV_MSGQ_H


USES (gpptypes.h)
USES (errbase.h)
USES (dsplink.h)
USES (msgqdefs.h)
USES (ldrv_mqa.h)
USES (ldrv_mqt.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  ID_MSGCHNL_TO_DSP
 *
 *  @desc   This macro defines the ID of the messaging channel to the DSP.
 *  ============================================================================
 */
#define ID_MSGCHNL_TO_DSP MAX_CHANNELS

/** ============================================================================
 *  @const  ID_MSGCHNL_FM_DSP
 *
 *  @desc   This macro defines the ID of the messaging channel from the DSP.
 *  ============================================================================
 */
#define ID_MSGCHNL_FM_DSP (MAX_CHANNELS + 1)

/** ============================================================================
 *  @const  DSPLINK_DSPMSGQ_NAME
 *
 *  @desc   This macro defines the prefix to the names of all MSGQs created on
 *          the DSP for communication with the GPP.
 *  ============================================================================
 */
#define DSPLINK_DSPMSGQ_NAME    "DSPLINK_DSP00MSGQ"

/** ============================================================================
 *  @const  DSPLINK_GPPMSGQ_NAME
 *
 *  @desc   This macro defines the prefix to the names of all MSGQs created on
 *          the GPP for communication with the DSP.
 *  ============================================================================
 */
#define DSPLINK_GPPMSGQ_NAME    "DSPLINK_GPPMSGQ"

/** ============================================================================
 *  @name   LdrvMsgqStatus
 *
 *  @desc   This enumeration defines the possible states of the MSGQ object.
 *
 *  @field  LdrvMsgqStatus_Empty
 *              The message queue is empty.
 *  @field  LdrvMsgqStatus_Inuse
 *              The message queue is in use.
 *  @field  LdrvMsgqStatus_LocatePending
 *              The message queue is waiting for the completion of locate.
 *  ============================================================================
 */
typedef enum {
    LdrvMsgqStatus_Empty         = 0,
    LdrvMsgqStatus_Inuse         = 1,
    LdrvMsgqStatus_LocatePending = 2
} LdrvMsgqStatus ;


/** ============================================================================
 *  @name   LdrvMsgqObject
 *
 *  @desc   Forward declaration of LDRV MSGQ object .
 *  ============================================================================
 */
typedef struct LdrvMsgqObject_tag LdrvMsgqObject ;

/** ============================================================================
 *  @name   LdrvMsgqHandle
 *
 *  @desc   Forward declaration of the handle to LDRV MSGQ object .
 *  ============================================================================
 */
typedef LdrvMsgqObject * LdrvMsgqHandle ;


/** ============================================================================
 *  @name   LdrvMsgqObject
 *
 *  @desc   This structure defines the MSGQ object. It includes all information
 *          specific to a particular MSGQ.
 *
 *  @field  msgqId
 *              ID of the MSGQ.
 *  @field  getSem
 *              Pointer to the semaphore to be used for waiting for messages on
 *              this MSGQ.
 *  @field  mqtRepository
 *              Handle to the MQT instance for this MSGQ. This object is
 *              specific to each MQT, and contains all information needed by it
 *              for interaction with this MSGQ. There is one instance of this
 *              object for every MSGQ.
 *  @field  mqtGet
 *              Pointer to the mqtGet () function of the transport. This pointer
 *              is replicated within this structure for faster access in time
 *              critical MSGQ_Get () function.
 *  @field  mqtPut
 *              Pointer to the mqtPut () function of the transport. This pointer
 *              is replicated within this structure for faster access in time
 *              critical MSGQ_Put () function.
 *  @field  msgqStatus
 *              State of the MSGQ.
 *  ============================================================================
 */
struct LdrvMsgqObject_tag {
    Uint16           msgqId ;
    SyncSemObject *  getSem ;
    Pvoid            mqtRepository ;
    FnMqtGet         mqtGet ;
    FnMqtPut         mqtPut ;
    LdrvMsgqStatus   msgqStatus ;
} ;

/** ============================================================================
 *  @name   LdrvMsgqState
 *
 *  @desc   This structure defines the MSGQ state object. It includes all global
 *          information required by the MSGQ component.
 *
 *  @field  allocators
 *              Array of allocator objects.
 *  @field  transports
 *              Array of transport objects, one for every processor in the
 *              system. This includes the local processor, as well as any other
 *              processors to which the local processor is connected.
 *  @field  numAllocators
 *              Number of allocators configured in the system.
 *  @field  numTransports
 *              Number of transports configured in the system.
 *  @field  localTransportId
 *              ID of the local transport. The local MQT ID is used to index
 *              into the MQT objects table whenever the local MQT needs to be
 *              accessed.
 *  @field  mqtMap
 *              Mapping of the processor ID to the MQT ID. This information is
 *              obtained through the CFG, and used for converting the processor
 *              ID into the MQT ID during MSGQ API calls.
 *  @field  errorHandlerMsgq
 *              ID of the MSGQ registered by the user as an error handler. If no
 *              error handler MSGQ has been registered by the user, the value of
 *              this field is MSGQ_INVALID_ID.
 *  @field  errorMqaId
 *              ID of the allocator to be used for allocating the asynchronous
 *              error messages, if the user has registered an error handler
 *              MSGQ. If no error handler MSGQ has been registered by the user,
 *              the value of this field is MSGQ_INVALID_ID.
 *  ============================================================================
 */
typedef struct LdrvMsgqState_tag {
    LdrvMsgqAllocatorObj * allocators ;
    LdrvMsgqTransportObj * transports ;
    Uint16                 numAllocators ;
    Uint16                 numTransports ;
    Uint16                 localTransportId ;
    Uint16                 mqtMap [MAX_PROCESSORS] ;
    MsgQueueId             errorHandlerMsgq ;
    AllocatorId            errorMqaId ;
} LdrvMsgqState ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Setup
 *
 *  @desc   This function initializes the MSGQ component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must not be initialized.
 *
 *  @leave  Component must be initialized upon successful completion otherwise
 *          it must be uninitialized.
 *
 *  @see    LDRV_MSGQ_Destroy
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Setup () ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Destroy
 *
 *  @desc   This function finalizes the MSGQ component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *
 *  @leave  Component must be uninitialized.
 *
 *  @see    LDRV_MSGQ_Setup
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Destroy () ;


/** ============================================================================
 *  @func   LDRV_MSGQ_AllocatorOpen
 *
 *  @desc   This function opens the MQA component.
 *
 *  @arg    mqaId
 *              ID of the MQA to be opened.
 *  @arg    mqaAttrs
 *              Attributes for initialization of the MQA component.
 *  @arg    mqaInfo
 *              Location to receive the handle to the initialized MQA state
 *              object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          mqaAttrs must be valid.
 *
 *  @leave  None
 *
 *  @see    LDRV_MsgqAllocatorAttrs
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_AllocatorOpen (IN  AllocatorId mqaId,
                         IN  Pvoid       mqaAttrs,
                         OUT Pvoid *     mqaInfo) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_AllocatorClose
 *
 *  @desc   This function closes the MQA component.
 *
 *  @arg    mqaId
 *              ID of the MQA to be closed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_AllocatorOpen
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_AllocatorClose (IN  AllocatorId mqaId) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_TransportOpen
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
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          mqtAttrs must be valid.
 *
 *  @leave  None
 *
 *  @see    LDRV_MsgqTransportAttrs
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_TransportOpen (IN  TransportId mqtId, IN  Pvoid mqtAttrs) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_TransportClose
 *
 *  @desc   This function finalizes the MQT component.
 *
 *  @arg    mqtId
 *              ID of the MQT to be closed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *
 *  @leave  None
 *
 *  @see    LDRV_MSGQ_AllocatorOpen
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_TransportClose (IN  TransportId mqtId) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Create
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
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Create (IN MsgQueueId  msgqId, IN OPT MsgqAttrs * msgqAttrs) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Delete
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
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Delete (IN  MsgQueueId msgqId) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Locate
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
 *              Operation successfully completed.
 *          DSP_ENOTFOUND
 *              The message queue does not exist on the specified processor.
 *          DSP_ETIMEOUT
 *              Timeout occurred while locating the MSGQ.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          procId must be valid.
 *          attrs must not be NULL.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Locate (IN  ProcessorId       procId,
                  IN  MsgQueueId        msgqId,
                  IN  MsgqLocateAttrs * attrs) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier call.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    msgqId
 *              ID of the message queue to be released.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          procId must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Release (IN  ProcessorId procId, IN  MsgQueueId  msgqId) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Alloc
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
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Alloc (IN  AllocatorId mqaId, IN  Uint16 size, OUT MsgqMsg * msg) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Free
 *
 *  @desc   This function frees a message.
 *
 *  @arg    msg
 *              Pointer to the message to be freed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Free (IN  MsgqMsg msg) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Put
 *
 *  @desc   This function sends a message to the specified MSGQ on the specified
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
 *              ID of the source MSGQ to receive reply messages (if any).
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_ENOTFOUND
 *              The message queue does not exist. This implies that the MSGQ has
 *              not been located before this call was made.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Put (IN     ProcessorId procId,
               IN     MsgQueueId  destMsgqId,
               IN     MsgqMsg     msg,
               IN OPT Uint16      msgId,
               IN OPT MsgQueueId  srcMsgqId) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_Get
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
 *          DSP_ETIMEOUT
 *              Timeout occurred while receiving the message.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          msg must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Get (IN  MsgQueueId msgqId, IN  Uint32 timeout, OUT MsgqMsg * msg) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_GetReplyId
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
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          msg must be valid.
 *          procId must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_GetReplyId (IN  MsgqMsg       msg,
                      OUT ProcessorId * procId,
                      OUT MsgQueueId *  msgqId) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_SetErrorHandler
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
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          msgqId must be valid.
 *          mqaId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_SetErrorHandler (IN  MsgQueueId msgqId, IN  Uint16 mqaId) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_SendErrorMsg
 *
 *  @desc   This function sends an asynchronous error message of a particular
 *          type to the user-defined error handler MSGQ.
 *
 *  @arg    errorType
 *              Type of the error.
 *  @arg    arg1
 *              First argument dependent on the error type.
 *  @arg    arg2
 *              Second argument dependent on the error type.
 *  @arg    arg3
 *              Third argument dependent on the error type.
 *
 *  @ret    DSP_SOK
 *              The error message has been successfully sent.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_SendErrorMsg (IN  MsgqErrorType errorType,
                        IN  Pvoid         arg1,
                        IN  Pvoid         arg2,
                        IN  Pvoid         arg3) ;


/** ============================================================================
 *  @func   LDRV_MSGQ_NotImpl
 *
 *  @desc   This function should be used in interface tables where some
 *          functions are not being implemented.
 *
 *  @arg    None
 *
 *  @ret    DSP_ENOTIMPL
 *              This function is not implemented.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_NotImpl () ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LDRV_MSGQ_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    msgqId
 *              Message queue identifier.
 *  @arg    retVal
 *              Location to retrieve the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  The component must be initialized.
 *          retVal must be valid.
 *
 *  @leave  None
 *
 *  @see    MsgqInstrument
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Instrument (IN  ProcessorId      procId,
                      IN  MsgQueueId       msgqId,
                      OUT MsgqInstrument * retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_MSGQ_Debug
 *
 *  @desc   This function prints the current status of the MSGQ subcomponent.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    msgqId
 *              Message queue identifier.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
LDRV_MSGQ_Debug (IN  ProcessorId procId, IN  MsgQueueId msgqId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LDRV_MSGQ_H) */
