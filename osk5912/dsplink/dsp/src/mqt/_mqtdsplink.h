/** ============================================================================
 *  @file   _mqtdsplink.h
 *
 *  @path   $(DSPLINK)\dsp\src\mqt
 *
 *  @desc   Internal declarations for the remote MQT for GPP-DSP communication.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef _MQTDSPLINK_
#define _MQTDSPLINK_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <que.h>
#include <sem.h>
#include <gio.h>
#include <msgq.h>


/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <dsplink.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  ID_MQTDSPLINK_CTRL
 *
 *  @desc   Internal ID used to identify control messages.
 *  ============================================================================
 */
#define ID_MQTDSPLINK_CTRL  (Uint16) 0xFF00


/** ============================================================================
 *  @name   MQTDSPLINK_CtrlCmd
 *
 *  @desc   Defines the types of control commands that are sent between the MQTs
 *          on different processors.
 *
 *  @field  MqtCmdLocate
 *              Indicates a locate command message.
 *  @field  MqtCmdLocateAck
 *              Indicates a locate acknowledgement message.
 *  @field  MqtCmdExit
 *              Indicates a remote transport exit notification message.
 *  ============================================================================
 */
typedef enum {
    MqtCmdLocate     = 0,
    MqtCmdLocateAck  = 1,
    MqtCmdExit       = 2
} MQTDSPLINK_CtrlCmd ;


/** ============================================================================
 *  @name   MQTDSPLINK_State
 *
 *  @desc   Transport state object, existing as a single instance for the
 *          DSPLINK remote MQT.
 *
 *  @field  mqtId
 *              ID of this MQT.
 *  @field  numRemoteMsgq
 *              Maximum number of MSGQs that can be created on the remote
 *              processor.
 *  @field  remoteMsgqs
 *              Array of remote MSGQ objects.
 *  @field  maxMsgSize
 *              Maximum message size (in MADUs) supported by the MQT.
 *  @field  numLocalMsgq
 *              Number of local MSGQs.
 *  @field  localMsgqs
 *              Array of local MSGQ objects.
 *  @field  getBuffer
 *              The buffer to be used for priming the input channel.
 *  @field  inpChan
 *              Handle to the input channel used for receiving messages from the
 *              GPP.
 *  @field  outChan
 *              Handle to the output channel used for sending messages to the
 *              GPP.
 *  @field  getCallback
 *              GIO callback object for submissions on the input channel.
 *  @field  putCallback
 *              GIO callback object for submissions on the output channel.
 *  @field  msgQueue
 *              Message repository to queue pending messages.
 *  @field  defaultMqaId
 *              The default MQA to be used by the remote MQT.
 *  ============================================================================
 */
typedef struct MQTDSPLINK_State_tag {
    Uint16            mqtId ;
    Uint16            numRemoteMsgq ;
    MSGQ_Handle       remoteMsgqs ;
    Uint16            maxMsgSize ;
    Uint16            numLocalMsgq ;
    MSGQ_Handle       localMsgqs ;
    Ptr               getBuffer ;
    GIO_Handle        inpChan ;
    GIO_Handle        outChan ;
    GIO_AppCallback   getCallback ;
    GIO_AppCallback   putCallback ;
    QUE_Obj           msgQueue ;
    Uint16            defaultMqaId ;
} MQTDSPLINK_State ;

/** ============================================================================
 *  @name   MQTDSPLINK_Obj
 *
 *  @desc   Transport object, which has an instance for every MSGQ created on
 *          the processor.
 *
 *  @field  locateSem
 *              Semaphore used during mqtLocate ()
 *  ============================================================================
 */
typedef struct MQTDSPLINK_Obj_tag {
    SEM_Obj locateSem ;
} MQTDSPLINK_Obj, *MQTDSPLINK_Handle ;

/** ============================================================================
 *  @name   MQTDSPLINK_CtrlMsg
 *
 *  @desc   Defines the format of the control messages that are sent between the
 *          MQTs on different processors.
 *          This structure is common between the GPP and the DSP, and must be
 *          maintained as the same on both. To ensure this, padding must be
 *          added as required within the structure.
 *
 *  @field  msgHeader
 *              Fixed message header required for all messages.
 *  @field  ctrlMsg
 *              Defines the format of the different control messages.
 *              locateMsg:    msgqId      -> ID of the MSGQ to be located on the
 *                                           remote processor
 *                            mqaId       -> MQA ID to alloc async response msg
 *                            timeout     -> Timeout value for sync locate
 *                            replyHandle -> Reply MSGQ handle for async locate
 *                            arg         -> User-defined value passed to locate
 *                            semHandle   -> Semaphore handle for sync locate
 *              locateAckMsg: msgqId      -> ID of the MSGQ located on the
 *                                           remote processor.
 *                            mqaId       -> MQA ID to alloc async response msg
 *                            timeout     -> Timeout value for sync locate
 *                            replyHandle -> Reply MSGQ handle for async locate
 *                            arg         -> User-defined value passed to locate
 *                            semHandle   -> Semaphore handle for sync locate
 *                            msgqFound   -> Requested MSGQ was found on the
 *                                           remote processor? 0 -> Not found,
 *                                                             1 -> Found
 *                            padding     -> Padding for alignment.
 *              No control message is required when the command indicates exit
 *              of the remote MQT.
 *  ============================================================================
 */
typedef struct MQTDSPLINK_CtrlMsg_tag {
    MSGQ_MsgHeader msgHeader ;
    union {
        struct {
            Uint16      msgqId ;
            Uint16      mqaId;
            Uint32      timeout;
            Uint32      replyHandle;
            Uint32      arg;
            Uint32      semHandle ;
        } locateMsg ;

        struct {
            Uint16      msgqId ;
            Uint16      mqaId;
            Uint32      timeout;
            Uint32      replyHandle;
            Uint32      arg;
            Uint32      semHandle ;
            Uint16      msgqFound ;
            Uint16      padding ;
        } locateAckMsg ;
    } ctrlMsg ;
} MQTDSPLINK_CtrlMsg ;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* _MQTDSPLINK_ */
