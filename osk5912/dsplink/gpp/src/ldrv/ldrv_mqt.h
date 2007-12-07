/** ============================================================================
 *  @file   ldrv_mqt.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the interface and structures of LDRV MQT.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LDRV_MQT_H)
#define LDRV_MQT_H


USES (gpptypes.h)
USES (errbase.h)
USES (dsplink.h)
USES (msgqdefs.h)
USES (ldrv_msgq.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   LdrvMsgqTransportObj
 *
 *  @desc   Forward declaration of LDRV MQT transport object.
 *  ============================================================================
 */
typedef struct LdrvMsgqTransportObj_tag LdrvMsgqTransportObj ;

/** ============================================================================
 *  @name   LdrvMsgqTransportHandle
 *
 *  @desc   Forward declaration of the handle to LDRV MQT transport object.
 *  ============================================================================
 */
typedef LdrvMsgqTransportObj * LdrvMsgqTransportHandle ;


/** ============================================================================
 *  @name   FnMqtInitialize
 *
 *  @desc   Signature of the function that performs MQT initialization.
 *
 *  @arg    None
 *
 *  @ret    None
 *  ============================================================================
 */
typedef Void (*FnMqtInitialize) () ;

/** ============================================================================
 *  @name   FnMqtFinalize
 *
 *  @desc   Signature of the function that performs MQT finalization.
 *
 *  @arg    None
 *
 *  @ret    None
 *  ============================================================================
 */
typedef Void (*FnMqtFinalize) () ;

/** ============================================================================
 *  @name   FnMqtOpen
 *
 *  @desc   Signature of the function that opens the MQT and configures it
 *          according to the user attributes.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    mqtAttrs
 *              Attributes for initialization of the MQT component.
 *
 *  @ret    DSP_SOK
 *              This component has been successfully opened.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtOpen) (LdrvMsgqTransportHandle mqtHandle,
                                 Pvoid                   mqtAttrs) ;

/** ============================================================================
 *  @name   FnMqtClose
 *
 *  @desc   Signature of the function that closes the MQT, and cleans up its
 *          state object.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *
 *  @ret    DSP_SOK
 *              This component has been successfully closed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtClose) (LdrvMsgqTransportHandle mqtHandle) ;

/** ============================================================================
 *  @name   FnMqtCreate
 *
 *  @desc   Signature of the function that creates the message queue identified
 *          by the specified MSGQ ID.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    msgqId
 *              ID of the message queue to be created.
 *  @arg    attrs
 *              Optional attributes for creation of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtCreate) (LdrvMsgqTransportHandle mqtHandle,
                                   MsgQueueId              msgqId,
                                   MsgqAttrs *             attrs) ;

/** ============================================================================
 *  @name   FnMqtLocate
 *
 *  @desc   Signature of the function that verifies the existence and status of
 *          the message queue identified by the specified MSGQ ID.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    msgqId
 *              ID of the message queue to be located.
 *  @arg    attrs
 *              Attributes for location of the MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_ENOTFOUND
 *              The message queue does not exist among the MSQs managed by this
 *              MQT.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtLocate) (LdrvMsgqTransportHandle mqtHandle,
                                   MsgQueueId              msgqId,
                                   MsgqLocateAttrs *       attrs) ;

/** ============================================================================
 *  @name   FnMqtDelete
 *
 *  @desc   Signature of the function that deletes the message queue identified
 *          by the specified MSGQ ID.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    msgqId
 *              ID of the message queue to be deleted.
 *
 *  @ret    DSP_SOK
 *              The message queue has been successfully deleted.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtDelete) (LdrvMsgqTransportHandle mqtHandle,
                                   MsgQueueId              msgqId) ;

/** ============================================================================
 *  @name   FnMqtRelease
 *
 *  @desc   Signature of the function that releases the MSGQ located through an
 *          earlier locate / getreplyid call.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    msgqId
 *              ID of the message queue to be released.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtRelease) (LdrvMsgqTransportHandle mqtHandle,
                                    MsgQueueId              msgqId) ;

/** ============================================================================
 *  @name   FnMqtGet
 *
 *  @desc   Signature of the function that receives a message on the specified
 *          MSGQ.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
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
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtGet) (LdrvMsgqTransportHandle mqtHandle,
                                MsgQueueId              msgqId,
                                Uint32                  timeout,
                                MsgqMsg *               msg) ;

/** ============================================================================
 *  @name   FnMqtPut
 *
 *  @desc   Signature of the function that sends a message on the specified
 *          MSGQ ID.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    msgqId
 *              ID of the destination MSGQ.
 *  @arg    msg
 *              Pointer to the message to be sent to the destination MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_ENOTFOUND
 *              The message queue does not exist. This implies that the MSGQ has
 *              not been located before this call was made.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtPut) (LdrvMsgqTransportHandle mqtHandle,
                                MsgQueueId              msgqId,
                                MsgqMsg                 msg) ;

/** ============================================================================
 *  @name   FnMqtGetReplyId
 *
 *  @desc   Signature of the function that gets the reply MSGQ ID for a
 *          particular message.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    msg
 *              Pointer to the message.
 *  @arg    msgqId
 *              ID of the destination MSGQ.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtGetReplyId) (LdrvMsgqTransportHandle mqtHandle,
                                       MsgqMsg                 msg,
                                       MsgQueueId *            msgqId) ;


/** ============================================================================
 *  @name   FnMqtGetById
 *
 *  @desc   Signature of the MQT function that receives a message having a
 *          particular MSG ID.
 *
 *  @arg    msgqId
 *              Message queue identifier.
 *  @arg    msgIds
 *              Array containing message queue identifiers.
 *  @arg    numIds
 *              Number of message queue identifiers in the array.
 *  @arg    timeout
 *              Timeout value to wait for the message (in milliseconds).
 *  @arg    msg
 *              Location to receive the allocated message.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_ETIMEOUT
 *              Timeout occurred while receiving the message.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtGetById) (MsgQueueId  msgqId,
                                    Uint16 *    msgIds,
                                    Uint16      numIds,
                                    Uint32      timeout,
                                    MsgqMsg *   msg) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @name   FnMqtInstrument
 *
 *  @desc   Signature of the MQT instrumentation function.
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
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqtInstrument) (LdrvMsgqTransportHandle mqtHandle,
                                       MsgQueueId              msgqId,
                                       MsgqInstrument *        retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @name   FnMqtDebug
 *
 *  @desc   Signature of the MQT debug function.
 *
 *  @arg    mqtHandle
 *              This is the handle to LDRV MQT transport object.
 *  @arg    msgqId
 *              Message queue identifier.
 *
 *  @ret    None.
 *  ============================================================================
 */
typedef Void (*FnMqtDebug) (LdrvMsgqTransportHandle mqtHandle,
                            MsgQueueId              msgqId) ;
#endif /* defined (DDSP_DEBUG) */


/** ============================================================================
 *  @name   MqtInterface
 *
 *  @desc   This structure defines the function pointer table that must be
 *          implemented for every MQT in the system.
 *
 *  @field  mqtInitialize
 *              Pointer to MQT initialization function.
 *  @field  mqtFinalize
 *              Pointer to MQT finalization function.
 *  @field  mqtOpen
 *              Pointer to MQT open function.
 *  @field  mqtClose
 *              Pointer to MQT close function.
 *  @field  mqtCreate
 *              Pointer to MQT function for creating a MSGQ.
 *  @field  mqtLocate
 *              Pointer to MQT function for locating a MSGQ.
 *  @field  mqtDelete
 *              Pointer to MQT function for deleting a MSGQ.
 *  @field  mqtRelease
 *              Pointer to MQT function for releasing a MSGQ.
 *  @field  mqtGet
 *              Pointer to MQT function for receiving a message.
 *  @field  mqtPut
 *              Pointer to MQT function for sending a message.
 *  @field  mqtGetReplyId
 *              Pointer to MQT function for getting the reply MSGQ ID for a
 *              particular message.
 *  @field  FnMqtGetById
 *              Pointer to MQT function for receiving a message having a
 *              particular MSG ID.
 *  @field  mqtInstrument
 *              Pointer to MQT Instrumentation function.
 *  @field  mqtDebug
 *              Pointer to MQT debug function.
 *  ============================================================================
 */
struct MqtInterface_tag {
    FnMqtInitialize   mqtInitialize ;
    FnMqtFinalize     mqtFinalize   ;
    FnMqtOpen         mqtOpen       ;
    FnMqtClose        mqtClose      ;
    FnMqtCreate       mqtCreate     ;
    FnMqtLocate       mqtLocate     ;
    FnMqtDelete       mqtDelete     ;
    FnMqtRelease      mqtRelease    ;
    FnMqtGet          mqtGet        ;
    FnMqtPut          mqtPut        ;
    FnMqtGetReplyId   mqtGetReplyId ;
    FnMqtGetById      mqtGetById    ;
#if defined (DDSP_PROFILE)
    FnMqtInstrument   mqtInstrument ;
#endif /* defined (DDSP_PROFILE) */
#if defined (DDSP_DEBUG)
    FnMqtDebug        mqtDebug      ;
#endif /* defined (DDSP_DEBUG) */
} ;


/** ============================================================================
 *  @name   LdrvMsgqTransportObj_tag
 *
 *  @desc   This structure defines the common attributes of the transport
 *          object. There is one instance of the transport object per MQT
 *          in the system.
 *
 *  @field  mqtName
 *              Name of the MQT. Used for debugging purposes only.
 *  @field  mqtInterface
 *              Pointer to the function table of the MQT represented by the
 *              transport object.
 *  @field  mqtInfo
 *              State information needed by the transport. The contents of this
 *              are transport-specific.
 *  @field  mqtId
 *              ID of the MQT represented by the transport object.
 *  @field  procId
 *              Processor Id associated with this MQT.
 *  ============================================================================
 */
struct LdrvMsgqTransportObj_tag {
#if defined (DDSP_DEBUG)
    Char8                  mqtName [DSP_MAX_STRLEN] ;
#endif /* if defined (DDSP_DEBUG) */
    MqtInterface *         mqtInterface ;
    Pvoid                  mqtInfo ;
    Uint16                 mqtId ;
    ProcessorId            procId ;
} ;


/** ============================================================================
 *  @name   MqtObject
 *
 *  @desc   This structure defines the MQT object stored in the LDRV object.
 *
 *  @field  mqtName
 *              Name of the MQT. For debugging purposes only.
 *  @field  interface
 *              Function pointer interface to access the functions for this MQT.
 *  @field  linkId
 *              ID of the link used by this MQT.
 *  ============================================================================
 */
struct MqtObject_tag {
    Char8           mqtName    [DSP_MAX_STRLEN] ;
    MqtInterface *  interface                   ;
    Uint32          linkId                      ;
} ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LDRV_MQT_H) */
