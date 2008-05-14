/** ============================================================================
 *  @file   msgqdefs.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines data types and structures used by DSP/BIOS(tm) Link for
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


#if !defined (MSGQDEFS_H)
#define MSGQDEFS_H


USES (gpptypes.h)
USES (dsplink.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @name   MsgqMqtError
 *
 *  @desc   This type is used for identifying types of MQT async error messages.
 *  ============================================================================
 */
typedef Int16    MsgqMqtError ;

/** ============================================================================
 *  @name   MsgqId
 *
 *  @desc   This type is used for identifying a message queue on a specific
 *          processor.
 *  ============================================================================
 */
typedef Uint16   MsgqId ;

/** ============================================================================
 *  @name   MsgqQueue
 *
 *  @desc   This type is used for identifying a message queue across processors.
 *  ============================================================================
 */
typedef Uint32   MsgqQueue ;


/** ============================================================================
 *  @const  MSGQ_INVALIDMSGQ
 *
 *  @desc   This constant denotes an invalid message queue.
 *  ============================================================================
 */
#define MSGQ_INVALIDMSGQ      (Uint16) 0xFFFF

/** ============================================================================
 *  @const  MSGQ_INVALIDPROCID
 *
 *  @desc   This constant denotes an invalid processor ID.
 *  ============================================================================
 */
#define MSGQ_INVALIDPROCID    (Uint16) 0xFFFF

/** ============================================================================
 *  @const  MSGQ_INTERNAL_ID_START
 *
 *  @desc   This constant defines the start of internal MSGQ message ID range.
 *  ============================================================================
 */
#define MSGQ_INTERNALIDSSTART (Uint16) 0xFF00

/** ============================================================================
 *  @const  MSGQ_ASYNCLOCATEMSGID
 *
 *  @desc   This constant defines the asynchronous locate message ID.
 *  ============================================================================
 */
#define MSGQ_ASYNCLOCATEMSGID (Uint16) 0xFF00

/** ============================================================================
 *  @const  MSGQ_ASYNCERRORMSGID
 *
 *  @desc   This constant defines the asynchronous error message ID.
 *  ============================================================================
 */
#define MSGQ_ASYNCERRORMSGID (Uint16) 0xFF01

/** ============================================================================
 *  @const  MSGQ_INTERNALIDSEND
 *
 *  @desc   This constant defines the end of internal MSGQ message ID range.
 *  ============================================================================
 */
#define MSGQ_INTERNALIDSEND (Uint16) 0xFF7f

/** ============================================================================
 *  @const  MSGQ_MQTMSGIDSSTART
 *
 *  @desc   This constant defines the start of transport message ID range.
 *  ============================================================================
 */
#define MSGQ_MQTMSGIDSSTART (Uint16) 0xFF80

/** ============================================================================
 *  @const  MSGQ_MQTMSGIDSEND
 *
 *  @desc   This constant defines the end of transport message ID range.
 *  ============================================================================
 */
#define MSGQ_MQTMSGIDSEND (Uint16) 0xFFFE

/** ============================================================================
 *  @const  MSGQ_INVALIDMSGID
 *
 *  @desc   This constant is used to denote no message ID value.
 *  ============================================================================
 */
#define MSGQ_INVALIDMSGID (Uint16) 0xFFFF

/** ============================================================================
 *  @const  MSGQ_MQTERROREXIT
 *
 *  @desc   In an asynchronous error message, this value as the error type
 *          indicates that remote MQT has called exit.
 *  ============================================================================
 */
#define MSGQ_MQTERROREXIT  (MsgqMqtError) -1

/** ============================================================================
 *  @const  MSGQ_MQTFAILEDPUT
 *
 *  @desc   In an asynchronous error message, this value as the error type
 *          indicates that the transport failed to send a message to the remote
 *          processor.
 *  ============================================================================
 */
#define MSGQ_MQTFAILEDPUT  (MsgqMqtError) -2

/** ============================================================================
 *  @const  MSG_HEADER_RESERVED_SIZE
 *
 *  @desc   This macro defines the size of the reserved field of message header.
 *  ============================================================================
 */
#define MSG_HEADER_RESERVED_SIZE    2

/** ============================================================================
 *  @macro  IS_VALID_MSGQ
 *
 *  @desc   Is the MSGQ valid.
 *  ============================================================================
 */
#define IS_VALID_MSGQ(msgq)        (msgq != MSGQ_INVALIDMSGQ)


/** ============================================================================
 *  @name   MsgqPend
 *
 *  @desc   Defines the types for the pend function.
 *
 *  @arg    notifyHandle
 *              Pointer to the notification object for the message queue.
 *  @arg    timeout
 *              Timeout Value.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          SYNC_E_FAIL
 *              General error from GPP-OS.
 *          DSP_ETIMEOUT
 *              Timeout occured while performing operation.
 *          DSP_EPOINTER
 *              Invalid pointer passed.
 *  ============================================================================
 */
typedef DSP_STATUS (*MsgqPend) (Pvoid notifyHandle, Uint32 timeout) ;


/** ============================================================================
 *  @name   MsgqPost
 *
 *  @desc   Defines the types for the post function.
 *
 *  @arg    notifyHandle
 *              Pointer to the notification object for the message queue.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          SYNC_E_FAIL
 *              General error from GPP-OS.
 *          DSP_EPOINTER
 *              Invalid pointer passed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *  ============================================================================
 */
typedef DSP_STATUS (*MsgqPost) (Pvoid notifyHandle) ;


/** ============================================================================
 *  @name   MsgqAttrs
 *
 *  @desc   This structure defines the attributes required during opening of
 *          the MSGQ.
 *
 *  @field  notifyHandle
 *              Pointer to the notification object for the message queue.
 *  @field  pend
 *              Function to be used to wait to receive a message.
 *  @field  post
 *              Function to be used to indicate arrival of a message.
 *  ============================================================================
 */
typedef struct MsgqAttrs_tag {
    Pvoid    notifyHandle ;
    MsgqPend pend ;
    MsgqPost post ;
} MsgqAttrs ;

/** ============================================================================
 *  @name   PcpyMqtAttrs
 *
 *  @desc   This structure defines the attributes for initialization of the
 *          PCPY MQT.
 *
 *  @field  poolId
 *              Pool ID used for allocating control messages.
 *              This pool is also used in case the ID within the message
 *              received from the DSP is invalid.
 *  ============================================================================
 */
typedef struct PcpyMqtAttrs_tag {
    PoolId    poolId ;
} PcpyMqtAttrs ;

#if !defined (STATIC_CONFIG)
/** ============================================================================
 *  @name   ZcpyMqtAttrs
 *
 *  @desc   This structure defines the attributes for initialization of the
 *          ZCPY MQT.
 *
 *  @field  poolId
 *              Pool ID used for allocating control messages.
 *  ============================================================================
 */
typedef struct ZcpyMqtAttrs_tag {
    PoolId    poolId ;
} ZcpyMqtAttrs ;
#endif /* if !defined (STATIC_CONFIG) */

/** ============================================================================
 *  @name   MsgqLocateAttrs
 *
 *  @desc   This structure defines the attributes required during synchronous
 *          location of a MSGQ.
 *
 *  @field  timeout
 *              Timeout value in milliseconds for the locate calls.
 *  ============================================================================
 */
typedef struct MsgqLocateAttrs_tag {
    Uint32     timeout ;
} MsgqLocateAttrs ;

/** ============================================================================
 *  @name   MsgqLocateAsyncAttrs
 *
 *  @desc   This structure defines the attributes required during asynchronous
 *          location of a MSGQ.
 *
 *  @field  poolId
 *              ID of the pool to be used for allocating async locate messages.
 *  @field  arg
 *              User-defined argument returned with an async locate message.
 *  ============================================================================
 */
typedef struct MsgqLocateAsyncAttrs_tag {
    PoolId     poolId ;
    Pvoid      arg ;
} MsgqLocateAsyncAttrs ;

/** ============================================================================
 *  @name   MsgqMsgHeader
 *
 *  @desc   This structure defines the format of the message header that must be
 *          the first field of any message.
 *
 *  @field  reserved
 *              Reserved for use by the MQT. The MQT typically uses them as a
 *              link for queuing the messages.
 *  @field  srcProcId
 *              Processor Id for the source message queue
 *  @field  poolId
 *              ID of the Pool used for allocating this message.
 *  @field  size
 *              Size of the message including the header.
 *  @field  dstId
 *              ID of the destination message queue.
 *  @field  srcId
 *              ID of the source message queue for reply.
 *  @field  msgId
 *              User-specified message ID.
 *  ============================================================================
 */
typedef struct MsgqMsgHeader_tag {
    Uint32    reserved [MSG_HEADER_RESERVED_SIZE] ;
    Uint16    srcProcId    ;
    Uint16    poolId       ;
    Uint16    size         ;
    Uint16    dstId        ;
    Uint16    srcId        ;
    Uint16    msgId        ;
} MsgqMsgHeader ;

/** ============================================================================
 *  @name   MsgqAsyncLocateMsg
 *
 *  @desc   This structure defines the asynchronous locate message format.
 *
 *  @field  header
 *              Fixed message header required for all messages.
 *  @field  msgqQueue
 *              Reply message queue specified during MSGQ_LocateAsync ().
 *  @field  arg
 *              User-defined argument specified as part of the
 *              MsgqLocateAsyncAttrs
 *  ============================================================================
 */
typedef struct MsgqAsyncLocateMsg_tag {
    MsgqMsgHeader  header ;
    MsgqQueue      msgqQueue ;
    Pvoid          arg ;
} MsgqAsyncLocateMsg ;

/** ============================================================================
 *  @name   MsgqAsyncErrorMsg
 *
 *  @desc   This structure defines the asynchronous error message format.
 *
 *  @field  header
 *              Fixed message header required for all messages.
 *  @field  errorType
 *              Type of error.
 *  @field  arg1
 *              First argument dependent on the error type.
 *              MSGQ_MQTERROREXIT : Processor ID of the transport.
 *              MSGQ_MQTFAILEDPUT : Handle of the destination message queue on
 *                                  which the put failed.
 *  @field  arg2
 *              Second argument dependent on the error type.
 *              MSGQ_MQTERROREXIT : Not used.
 *              MSGQ_MQTFAILEDPUT : Status of the MSGQ_Put () call that failed.
 *  ============================================================================
 */
typedef struct MsgqAsyncErrorMsg_tag {
    MsgqMsgHeader  header ;
    MsgqMqtError   errorType ;
    Pvoid          arg1 ;
    Pvoid          arg2 ;
} MsgqAsyncErrorMsg ;

/** ============================================================================
 *  @name   MsgqMsg
 *
 *  @desc   This is the type of any buffer used for messaging.
 *  ============================================================================
 */
typedef MsgqMsgHeader * MsgqMsg ;

/** ============================================================================
 *  @name   MqtInterface
 *
 *  @desc   Forward declaration of MQT interface table.
 *  ============================================================================
 */
typedef struct MqtInterface_tag MqtInterface ;

/** ============================================================================
 *  @name   MqtObject
 *
 *  @desc   Forward declaration of MqtObject.
 *  ============================================================================
 */
typedef struct MqtObject_tag MqtObject ;

#if defined (__cplusplus)
}
#endif


#endif /* !defined (MSGQDEFS_H) */
