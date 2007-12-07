/** ============================================================================
 *  @file   msgqdefs.h
 *
 *  @path   $(DSPLINK)\gpp\inc\Linux
 *
 *  @desc   Defines data types and structures used by DSP/BIOS(tm) Link for
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


#if !defined (MSGQDEFS_H)
#define MSGQDEFS_H


USES (gpptypes.h)
USES (dsplink.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @const  MAX_ALLOCATORS
 *
 *  @desc   Maximum number of allocators supported by DSP/BIOS Link.
 *  ============================================================================
 */
#define MAX_ALLOCATORS    16

/** ============================================================================
 *  @const  MAX_MSGQS
 *
 *  @desc   Maximum number of message queues that can be created on the GPP.
 *  ============================================================================
 */
#define MAX_MSGQS    32

/** ============================================================================
 *  @macro  IS_LOCAL_PROCID
 *
 *  @desc   Is the local processor id valid.
 *  ============================================================================
 */
#define IS_LOCAL_PROCID(id)        (id == ID_LOCAL_PROCESSOR)

/** ============================================================================
 *  @macro  IS_VALID_MSGQID
 *
 *  @desc   Is the MSGQ id valid.
 *  ============================================================================
 */
#define IS_VALID_MSGQID(id)        (id < MAX_MSGQS)

/** ============================================================================
 *  @const  ID_LOCAL_PROCESSOR
 *
 *  @desc   This constant defines the ID of the local processor (GPP).
 *  ============================================================================
 */
#define ID_LOCAL_PROCESSOR    (ProcessorId) 0xFFFF

/** ============================================================================
 *  @const  MSGQ_INTERNAL_ID_START
 *
 *  @desc   This constant defines the start of internal MSGQ message id range.
 *  ============================================================================
 */
#define MSGQ_INTERNAL_ID_START (Uint16) 0xFE00

/** ============================================================================
 *  @const  MSGQ_ASYNC_ERROR_MSGID
 *
 *  @desc   This constant defines the asynchronous error message id.
 *  ============================================================================
 */
#define MSGQ_ASYNC_ERROR_MSGID (Uint16) 0xFE01

/** ============================================================================
 *  @const  MSGQ_INTERNAL_ID_END
 *
 *  @desc   This constant defines the end of internal MSGQ message id range.
 *  ============================================================================
 */
#define MSGQ_INTERNAL_ID_END (Uint16) 0xFEFF

/** ============================================================================
 *  @const  MSGQ_MQT_MSGID_START
 *
 *  @desc   This constant defines the start of transport message id range.
 *  ============================================================================
 */
#define MSGQ_MQT_MSGID_START (Uint16) 0xFF00

/** ============================================================================
 *  @const  MSGQ_MQT_MSGID_END
 *
 *  @desc   This constant defines the end of transport message id range.
 *  ============================================================================
 */
#define MSGQ_MQT_MSGID_END (Uint16) 0xFFFE

/** ============================================================================
 *  @const  MSGQ_INVALID_ID
 *
 *  @desc   This constant defines the invalid ID for MSGQ, MQT, MQA and
 *          messages.
 *  ============================================================================
 */
#define MSGQ_INVALID_ID   (Uint16) 0xFFFF

/** ============================================================================
 *  @const  MSG_HEADER_RESERVED_SIZE
 *
 *  @desc   This macro defines the size of the reserved field of message header.
 *  ============================================================================
 */
#define MSG_HEADER_RESERVED_SIZE    2

/** ============================================================================
 *  @const  MSGQ_GetMsgId
 *
 *  @desc   This macro returns the message ID of the specified message.
 *  ============================================================================
 */
#define MSGQ_GetMsgId(msg) (((MsgqMsg) (msg))->msgId)

/** ============================================================================
 *  @const  MSGQ_GetMsgSize
 *
 *  @desc   This macro returns the size of the specified message.
 *  ============================================================================
 */
#define MSGQ_GetMsgSize(msg) (((MsgqMsg) (msg))->size)

/** ============================================================================
 *  @name   MsgqErrorType
 *
 *  @desc   This enumeration defines the possible types of asynchronous error
 *          messages.
 *
 *  @field  MsgqErrorType_MqtExit
 *              Indicates that remote MQT has called exit.
 *              The arg1 and arg2 field of MsgqAsyncErrorMsg structure shall be
 *              as follows in this error type :
 *                  arg1 : Mqt Id of the remote MSGQ.
 *                  arg2 : Not used.
 *                  arg3 : Not used.
 *
 *  @field  MsgqErrorType_PutFailed
 *              Indicates that MSGQ_Put failed.
 *              The arg1 and arg2 field of MsgqAsyncErrorMsg structure shall be
 *              as follows in this error type :
 *                  arg1 : ID of the processor on which the destination message
 *                         queue exists.
 *                  arg2 : ID of the destination message queue on which the put
 *                         failed.
 *                  arg3 : Status of the MSGQ_Put call that failed.
 *
 *  @see    MsgqAsyncErrorMsg
 *  ============================================================================
 */
typedef enum {
    MsgqErrorType_MqtExit   = 0,
    MsgqErrorType_PutFailed = 1
} MsgqErrorType ;


/** ============================================================================
 *  @name   MsgqMsgHeader
 *
 *  @desc   This structure defines the format of the message header that must be
 *          the first field of any message.
 *
 *  @field  reserved
 *              Reserved for use by the MQT. The MQT typically uses them as a
 *              link for queuing the messages.
 *  @field  mqtId
 *              ID of the MQT used for transporting this message.
 *  @field  mqaId
 *              ID of the MQA used for allocating this message.
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
    Uint16    mqtId        ;
    Uint16    mqaId        ;
    Uint16    size         ;
    Uint16    dstId        ;
    Uint16    srcId        ;
    Uint16    msgId        ;
} MsgqMsgHeader ;

/** ============================================================================
 *  @name   MsgqMsg
 *
 *  @desc   This is the type of any buffer used for messaging.
 *  ============================================================================
 */
typedef MsgqMsgHeader * MsgqMsg ;

/** ============================================================================
 *  @name   MsgqAttrs
 *
 *  @desc   This structure defines the attributes required during creation of
 *          the MSGQ.
 *
 *  @field  dummy
 *              Dummy placeholder field.
 *  ============================================================================
 */
typedef struct MsgqAttrs_tag {
    Uint16     dummy ;
} MsgqAttrs ;

/** ============================================================================
 *  @name   MqaBufAttrs
 *
 *  @desc   This structure defines the attributes required for initialization of
 *          the buffer MQA.
 *
 *  @field  numBufPools
 *              Number of buffer pools to be configured in the MQA.
 *  @field  msgSize
 *              Array of sizes of the messages in the buffer pools. This array
 *              is of size numBufPools.
 *  @field  numMsg
 *              Array of number of messages in all buffer pools. This array is
 *              of size numBufPools.
 *  ============================================================================
 */
typedef struct MqaBufAttrs_tag {
    Uint16     numBufPools ;
    Uint16 *   msgSize ;
    Uint16 *   numMsg ;
} MqaBufAttrs ;

/** ============================================================================
 *  @name   LmqtAttrs
 *
 *  @desc   This structure defines the attributes for initialization of the
 *          local MQT.
 *
 *  @field  maxNumMsgq
 *              Maximum number of MSGQs that can be created on the local
 *              processor.
 *  ============================================================================
 */
typedef struct LmqtAttrs_tag {
    Uint16              maxNumMsgq ;
} LmqtAttrs ;

/** ============================================================================
 *  @name   RmqtAttrs
 *
 *  @desc   This structure defines the attributes for initialization of the
 *          remote MQT.
 *
 *  @field  maxNumMsgq
 *              Maximum number of MSGQs that can be created on the remote
 *              processor.
 *  @field  maxMsgSize
 *              Maximum message size (in bytes) supported by the MQT.
 *  @field  defaultMqaId
 *              The default MQA to be used by the remote MQT, in case the MQA ID
 *              within the message received from the DSP is invalid. This case
 *              can occur in case of a mismatch between allocators configured on
 *              the GPP and the DSP. This allocator is also used for allocating
 *              control messages.
 *  ============================================================================
 */
typedef struct RmqtAttrs_tag {
    Uint16       maxNumMsgq ;
    Uint16       maxMsgSize ;
    AllocatorId  defaultMqaId ;
} RmqtAttrs ;

/** ============================================================================
 *  @name   MsgqAsyncErrorMsg
 *
 *  @desc   This structure defines the asynchronous error message format.
 *
 *  @field  msgHeader
 *              Fixed message header required for all messages.
 *  @field  errorType
 *              Type of error.
 *  @field  arg1
 *              First argument dependent on the error type.
 *  @field  arg2
 *              Second argument dependent on the error type.
 *  @field  arg3
 *              Third argument dependent on the error type.
 *  ============================================================================
 */
typedef struct MsgqAsyncErrorMsg_tag {
    MsgqMsgHeader  msgHeader ;
    Uint16         errorType ;
    Pvoid          arg1 ;
    Pvoid          arg2 ;
    Pvoid          arg3 ;
} MsgqAsyncErrorMsg ;

/** ============================================================================
 *  @name   MsgqLocateAttrs
 *
 *  @desc   This structure defines the attributes required during location of a
 *          MSGQ.
 *
 *  @field  timeout
 *              Timeout value in milliseconds for the locate calls.
 *  ============================================================================
 */
typedef struct MsgqLocateAttrs_tag {
    Uint32     timeout ;
} MsgqLocateAttrs ;

/** ============================================================================
 *  @name   MqtInterface
 *
 *  @desc   Forward declaration of LDRV MQT interface table.
 *  ============================================================================
 */
typedef struct MqtInterface_tag MqtInterface ;

/** ============================================================================
 *  @name   MqaInterface
 *
 *  @desc   Forward declaration of LDRV MQT interface table.
 *  ============================================================================
 */
typedef struct MqaInterface_tag MqaInterface ;

/** ============================================================================
 *  @name   MqaObject
 *
 *  @desc   Forward declaration of MqaObject.
 *  ============================================================================
 */
typedef struct MqaObject_tag MqaObject ;

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
