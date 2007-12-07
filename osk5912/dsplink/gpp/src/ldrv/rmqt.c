/** ============================================================================
 *  @file   rmqt.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implements the remote MQT interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <dsplink.h>
#include <intobject.h>
#include <platform.h>
#include <errbase.h>
#include <safe.h>

#include <dspdefs.h>
#include <linkdefs.h>
#include <cfgdefs.h>
#include <msgqdefs.h>

/*  ----------------------------------- Trace & Debug                 */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- OSAL Header                   */
#include <mem.h>
#include <sync.h>
#include <print.h>

/*  ----------------------------------- Generic Function              */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Link Driver                   */
#include <ldrv_mqt.h>
#include <ldrv_mqa.h>
#include <ldrv_msgq.h>
#include <ldrv.h>
#include <ldrv_chnl.h>
#include <rmqt.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent ID.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_RMQT


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_RMQT, __LINE__)


/** ============================================================================
 *  @macro  SHUTDOWN_NOTIFY_TIMEOUT
 *
 *  @desc   Timeout for the notification of shutdown notification.
 *  ============================================================================
 */
#define SHUTDOWN_NOTIFY_TIMEOUT 1000


/** ============================================================================
 *  @name   LDRV_Obj
 *
 *  @desc   Global defined in ldrv.c.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;


/** ============================================================================
 *  @name   RMQT_Interface
 *
 *  @desc   The MQT APIs are exposed to MSGQ through the function table.
 *  ============================================================================
 */
MqtInterface RMQT_Interface = {
    &RMQT_Initialize,
    &RMQT_Finalize,
    &RMQT_Open,
    &RMQT_Close,
    (FnMqtCreate) &LDRV_MSGQ_NotImpl,
    &RMQT_Locate,
    (FnMqtDelete) &LDRV_MSGQ_NotImpl,
    &RMQT_Release,
    (FnMqtGet) &LDRV_MSGQ_NotImpl,
    &RMQT_Put,
    &RMQT_GetReplyId,
    (FnMqtGetById) &LDRV_MSGQ_NotImpl,
#if defined (DDSP_PROFILE)
    &RMQT_Instrument,
#endif /* defined (DDSP_PROFILE) */
#if defined (DDSP_DEBUG)
    &RMQT_Debug,
#endif /* defined (DDSP_DEBUG) */
} ;


/** ----------------------------------------------------------------------------
 *  @func   CreateRemoteMsgq
 *
 *  @desc   Create and return the remote MSGQ object for specified MSGQ ID.
 *
 *  @arg    mqtInfo
 *              Handle to the MQT state object.
 *  @arg    msgqId
 *              ID of the message queue for which the MSGQ object is to be
 *              created.
 *  @arg    msgqHandle
 *              Location to receive the handle to the created MSGQ object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtInfo must be valid.
 *          msgqId must be valid.
 *          msgqHandle must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
CreateRemoteMsgq (IN  RmqtState *          mqtInfo,
                  IN  MsgQueueId           msgqId,
                  OUT LdrvMsgqHandle *     msgqHandle) ;


/** ----------------------------------------------------------------------------
 *  @func   DeleteRemoteMsgq
 *
 *  @desc   Delete the remote MSGQ object for specified MSGQ ID.
 *
 *  @arg    mqtInfo
 *              Handle to the MQT state object.
 *  @arg    msgqId
 *              ID of the message queue for which the MSGQ object is to be
 *              created.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqtInfo must be valid.
 *          msgqId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
DeleteRemoteMsgq (IN  RmqtState * mqtInfo, IN  MsgQueueId  msgqId) ;


/** ----------------------------------------------------------------------------
 *  @func   ControlMsgProcessing
 *
 *  @desc   This function implements the callback for control messages, which
 *          runs when the message has been received from the DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    msgHeader
 *              Pointer to the message buffer.
 *  @arg    size
 *              Size of the message buffer.
 *  @arg    mqtHandle
 *              Handle to the transport object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          buffer must be valid.
 *          mqtInfo must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
ControlMsgProcessing (IN  ProcessorId             procId,
                      IN  MsgqMsg                 msgHeader,
                      IN  Uint32                  size,
                      IN  LdrvMsgqTransportHandle mqtHandle) ;


/** ----------------------------------------------------------------------------
 *  @func   SendMessage
 *
 *  @desc   This function sends a message to the DSP.
 *
 *  @arg    mqtHandle
 *              Handle to the transport object.
 *  @arg    msgqId
 *              ID of the destination MSGQ.
 *  @arg    msg
 *              Pointer to the message to be sent to the destination MSGQ.
 *  @arg    dpcContext
 *              Indicates whether this function was called from DPC context.
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
 *  @see    RMQT_Put (), RMQT_Locate (), ControlMsgProcessing ()
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
SendMessage (IN  LdrvMsgqTransportHandle mqtHandle,
             IN  MsgQueueId              msgqId,
             IN  MsgqMsg                 msg,
             IN  Bool                    dpcContext) ;


/** ============================================================================
 *  @func   RMQT_Initialize
 *
 *  @desc   This function performs global initialization of the remote MQT.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
RMQT_Initialize ()
{
    TRC_0ENTER ("RMQT_Initialize") ;

    /* No initialization code is required */

    TRC_0LEAVE ("RMQT_Initialize") ;
}


/** ============================================================================
 *  @func   RMQT_Finalize
 *
 *  @desc   This function performs global finalization of the remote MQT.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
RMQT_Finalize ()
{
    TRC_0ENTER ("RMQT_Finalize") ;

    /* No initialization code is required */

    TRC_0LEAVE ("RMQT_Finalize") ;
}


/** ============================================================================
 *  @func   RMQT_Open
 *
 *  @desc   This function opens the remote MQT and configures it according to
 *          the user attributes.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Open (IN  LdrvMsgqTransportHandle mqtHandle, IN  Pvoid mqtAttrs)
{
    DSP_STATUS       status            = DSP_SOK   ;
    RmqtState *      mqtState          = NULL      ;
    RmqtAttrs *      rmqtAttrs         = NULL      ;
    Uint16           maxNumMsgq        = 0         ;
    Uint16           i                 = 0         ;
    Uint16           j                 = 0         ;
    Bool             isStateAlloc      = FALSE     ;
    Bool             isHandlesAlloc    = FALSE     ;
    Bool             isGetBufAlloc     = FALSE     ;
    ChannelAttrs     inpChnlAttrs                  ;
    ChannelAttrs     outChnlAttrs                  ;
    LDRVChnlIOInfo   ioInfo                        ;

    TRC_2ENTER ("RMQT_Open", mqtHandle, mqtAttrs) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (mqtAttrs != NULL) ;

    if ((mqtHandle == NULL) || (mqtAttrs == NULL)) {
        status = DSP_EINVALIDARG ;
        if (mqtHandle != NULL) {
            /* Initialize the mqtInfo to NULL */
            mqtHandle->mqtInfo = NULL ;
        }
        SET_FAILURE_REASON ;
    }
    else {
        /* Initialize the mqtInfo to NULL */
        mqtHandle->mqtInfo = NULL ;

        rmqtAttrs = (RmqtAttrs *) mqtAttrs ;

        /* Check if the maximum number of MSGQs is valid. */
        if (rmqtAttrs->maxNumMsgq <= MAX_MSGQS) {
            maxNumMsgq = rmqtAttrs->maxNumMsgq ;
            /* Allocate memory for the state object */
            status = MEM_Calloc ((Void **) &mqtState,
                                 (sizeof (RmqtState)),
                                  MEM_DEFAULT) ;
            if (DSP_SUCCEEDED (status)) {
                isStateAlloc = TRUE ;
                mqtState->maxNumMsgq = maxNumMsgq ;
                mqtState->maxMsgSize = rmqtAttrs->maxMsgSize ;
                mqtState->defaultMqaId = rmqtAttrs->defaultMqaId ;
                mqtState->procId   = mqtHandle->procId ;

                if (maxNumMsgq > 0) {
                    /* Allocate memory for the MSGQ handles */
                    status = MEM_Calloc ((Void **) &(mqtState->msgqHandles),
                                         (maxNumMsgq * sizeof (LdrvMsgqHandle)),
                                         MEM_DEFAULT) ;
                    if (DSP_SUCCEEDED (status)) {
                        isHandlesAlloc = TRUE ;
                        for (i= 0 ;
                             (i < maxNumMsgq) && (DSP_SUCCEEDED (status)) ;
                             i++) {
                            status = CreateRemoteMsgq (
                                        mqtState,
                                        i,
                                        &(mqtState->msgqHandles [i])) ;
                        }
                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                            for (j = 0 ; i < j ; j++) {
                                DeleteRemoteMsgq (mqtState,i) ;
                            }
                        }
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    mqtState->msgqHandles = NULL ;
                }

                if (DSP_SUCCEEDED (status)) {
                    status = MEM_Calloc ((Void **) &(mqtState->getBuffer),
                                        (rmqtAttrs->maxMsgSize),
                                        MEM_DEFAULT) ;
                    if (DSP_SUCCEEDED (status)) {
                        isGetBufAlloc = TRUE ;
                        status = LIST_Create (&(mqtState->msgQueue)) ;
                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }

        /* Create the channels for messaging */
        if (DSP_SUCCEEDED (status)) {
            inpChnlAttrs.mode = ChannelMode_Input ;
            inpChnlAttrs.endianism = Endianism_Default ;
            inpChnlAttrs.size = ChannelDataSize_16bits ;

            outChnlAttrs.mode = ChannelMode_Output ;
            outChnlAttrs.endianism = Endianism_Default ;
            outChnlAttrs.size = ChannelDataSize_16bits ;

            status = LDRV_CHNL_Open (mqtHandle->procId,
                                     ID_MSGCHNL_FM_DSP,
                                     &inpChnlAttrs) ;
            if (DSP_SUCCEEDED (status)) {
                status = LDRV_CHNL_Open (mqtHandle->procId,
                                         ID_MSGCHNL_TO_DSP,
                                         &outChnlAttrs) ;
                if (DSP_SUCCEEDED (status)) {
                    ioInfo.buffer     = mqtState->getBuffer ;
                    ioInfo.size       = rmqtAttrs->maxMsgSize ;
                    ioInfo.arg        = (Uint32) mqtHandle ;
                    ioInfo.callback   = RMQT_GetCallback ;
                    ioInfo.dpcContext = FALSE ;

                    /* Set the mqtState here, since the callback will be
                     * invoked if the DSP is already ready. The callback
                     * expects the state to be valid at this point.
                     */
                    mqtHandle->mqtInfo = mqtState ;
                    status = LDRV_CHNL_AddIORequest (mqtHandle->procId,
                                                     ID_MSGCHNL_FM_DSP,
                                                     &ioInfo) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                        LDRV_CHNL_Close (mqtHandle->procId, ID_MSGCHNL_FM_DSP) ;
                        LDRV_CHNL_Close (mqtHandle->procId, ID_MSGCHNL_TO_DSP) ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                    LDRV_CHNL_Close (mqtHandle->procId, ID_MSGCHNL_FM_DSP) ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }

        if ((DSP_FAILED (status)) && (mqtState != NULL)) {
            /* Do cleanup of the allocated objects in case of failure */
            if (mqtState->msgQueue != NULL) {
                LIST_Delete (mqtState->msgQueue) ;
            }

            if (isGetBufAlloc == TRUE) {
                FREE_PTR (mqtState->getBuffer) ;
            }

            for (j = 0 ; i < maxNumMsgq ; j++) {
                DeleteRemoteMsgq (mqtState, i) ;
            }

            if (isHandlesAlloc == TRUE) {
                FREE_PTR (mqtState->msgqHandles) ;
            }

            if (isStateAlloc == TRUE) {
                FREE_PTR (mqtState) ;
                mqtHandle->mqtInfo = NULL ;
            }
        }
    }

    TRC_1LEAVE ("RMQT_Open", status) ;

    return status ;
}


/** ============================================================================
 *  @func   RMQT_Close
 *
 *  @desc   This function closes the remote MQT, and cleans up its state object.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Close (IN  LdrvMsgqTransportHandle mqtHandle)
{
    DSP_STATUS      status      = DSP_SOK ;
    DSP_STATUS      tmpStatus   = DSP_SOK ;
    RmqtState *     mqtState    = NULL    ;
    Uint16          i           = 0       ;
    MsgqMsg         msg         = NULL    ;
    LDRVChnlIOInfo  ioInfo                ;

    TRC_1ENTER ("RMQT_Close", mqtHandle) ;

    DBC_Require (mqtHandle != NULL) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;

        if (mqtState != NULL) {
            tmpStatus = LDRV_CHNL_Close (mqtState->procId, ID_MSGCHNL_FM_DSP) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }

            if (mqtState->getBuffer != NULL) {
                /* Send a synchronous MQT shutdown message to the GPP */
                /* Reuse the getBuffer */
                mqtState->getBuffer->dstId = ID_RMQT_CTRL ;
                mqtState->getBuffer->msgId = RmqtCtrlCmd_Exit ;
                ioInfo.buffer     = mqtState->getBuffer ;
                ioInfo.size       = mqtState->maxMsgSize ;
                ioInfo.arg        = (Uint32) NULL ;
                ioInfo.callback   = NULL ;
                ioInfo.dpcContext = FALSE ;
                tmpStatus = LDRV_CHNL_AddIORequest (mqtState->procId,
                                                    ID_MSGCHNL_TO_DSP,
                                                    &ioInfo) ;
                if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
                if (DSP_SUCCEEDED (tmpStatus)) {
                    ioInfo.dpcContext = FALSE ;
                    tmpStatus = LDRV_CHNL_GetIOCompletion (
                                                mqtState->procId,
                                                ID_MSGCHNL_TO_DSP,
                                                SHUTDOWN_NOTIFY_TIMEOUT,
                                                &ioInfo) ;

                    if (tmpStatus == DSP_ETIMEOUT) {
                        tmpStatus = DSP_SOK ;
                    }
                    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
            }

            tmpStatus = LDRV_CHNL_Close (mqtState->procId, ID_MSGCHNL_TO_DSP) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
            mqtHandle->mqtInfo = NULL ;

            tmpStatus = FREE_PTR (mqtState->getBuffer) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }

            if (mqtState->msgqHandles != NULL) {
                for (i = 0 ; i < mqtState->maxNumMsgq ; i++) {
                    tmpStatus = DeleteRemoteMsgq
                                  ((RmqtState *) mqtState, i) ;
                    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
            }

            if (mqtState->msgQueue != NULL) {
                SYNC_ProtectionStart () ;
                while (   (LIST_IsEmpty (mqtState->msgQueue) != TRUE)
                       && (DSP_SUCCEEDED (tmpStatus))) {
                    tmpStatus = LIST_GetHead (mqtState->msgQueue,
                                              (ListElement **) &msg) ;
                    if ((DSP_SUCCEEDED (tmpStatus)) && (msg != NULL)) {
                        msg->size *=
                               LDRV_Obj.dspObjects [mqtState->procId].wordSize ;
                        tmpStatus = LDRV_MSGQ_Free (msg) ;
                        if (   (DSP_SUCCEEDED (status))
                            && (DSP_FAILED (tmpStatus))) {
                            status = tmpStatus ;
                            SET_FAILURE_REASON ;
                        }
                    }
                    else if (   (DSP_SUCCEEDED (status))
                             && (DSP_FAILED (tmpStatus))) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
                tmpStatus = LIST_Delete (mqtState->msgQueue) ;
                if (   (DSP_SUCCEEDED (status))
                    && (DSP_FAILED (tmpStatus))) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
                SYNC_ProtectionEnd () ;
            }

            tmpStatus = FREE_PTR (mqtState->msgqHandles) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }

            tmpStatus = FREE_PTR (mqtState) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("RMQT_Close", status) ;

    return status ;
}


/** ============================================================================
 *  @func   RMQT_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Locate (IN  LdrvMsgqTransportHandle mqtHandle,
             IN  MsgQueueId              msgqId,
             IN  MsgqLocateAttrs *       attrs)
{
    DSP_STATUS     status       = DSP_SOK ;
    RmqtState *    mqtState     = NULL    ;
    MsgqMsg        locateMsg    = NULL    ;
    LdrvMsgqHandle locateMsgq   = NULL    ;
    RmqtObj *      locateHandle = NULL    ;

    TRC_3ENTER ("RMQT_Locate", mqtHandle, msgqId, attrs) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((RmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;
    DBC_Require (attrs != NULL) ;

    if ((mqtHandle == NULL)|| (attrs == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else if (msgqId >= mqtState->maxNumMsgq) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        locateMsgq = mqtState->msgqHandles [msgqId] ;

        status = LDRV_MSGQ_Alloc (mqtState->defaultMqaId,
                                  RMQT_CTRLMSG_SIZE,
                                  &locateMsg) ;
        if (DSP_SUCCEEDED (status)) {
            locateMsg->dstId = ID_RMQT_CTRL ;
            locateMsg->msgId = RmqtCtrlCmd_Locate ;
            ((RmqtCtrlMsg *) locateMsg)->ctrlMsg.locateMsg.msgqId =
                                                                msgqId ;
            status = SendMessage (mqtHandle,
                                  ID_RMQT_CTRL,
                                  locateMsg,
                                  FALSE) ; /* Not called from DPC context */
            if (DSP_SUCCEEDED (status)) {
                locateHandle = (RmqtObj *) locateMsgq->mqtRepository ;
                status = SYNC_WaitSEM (locateHandle->locateSem,
                                       attrs->timeout) ;
                if (DSP_SUCCEEDED (status)) {
                    if (locateMsgq->msgqStatus == LdrvMsgqStatus_Empty) {
                        /* Not doing SET_FAILURE_REASON because its not
                         * a failure actually
                         */
                        status = DSP_ENOTFOUND ;
                    }
                }
                else {
                    locateMsg->size *=
                           LDRV_Obj.dspObjects [mqtState->procId].wordSize ;
                    LDRV_MSGQ_Free (locateMsg) ;
                    if (   (attrs->timeout == SYNC_NOWAIT)
                        && (status == SYNC_E_FAIL)) {
                        status = DSP_ENOTCOMPLETE ;
                        SET_FAILURE_REASON ;
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }
                }
            }
            else {
                locateMsg->size *=
                           LDRV_Obj.dspObjects [mqtState->procId].wordSize ;
                LDRV_MSGQ_Free (locateMsg) ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("RMQT_Locate", status) ;

    return status ;
}


/** ============================================================================
 *  @func   RMQT_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier
 *          RMQT_Locate () call.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Release (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId)
{
    DSP_STATUS  status   = DSP_SOK ;
    RmqtState * mqtState = NULL    ;

    TRC_2ENTER ("RMQT_Release", mqtHandle, msgqId) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((RmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else if (msgqId >= mqtState->maxNumMsgq) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
        else if (mqtState->msgqHandles [msgqId] == NULL) {
            status = DSP_ENOTFOUND ; /* MSGQ was not located */
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        mqtState->msgqHandles [msgqId]->msgqStatus = LdrvMsgqStatus_Empty ;
    }

    TRC_1LEAVE ("RMQT_Release", status) ;

    return status ;
}


/** ============================================================================
 *  @func   RMQT_Put
 *
 *  @desc   This function sends a message to the specified remote MSGQ.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Put (IN  LdrvMsgqTransportHandle mqtHandle,
          IN  MsgQueueId              msgqId,
          IN  MsgqMsg                 msg)
{
    DSP_STATUS  status   = DSP_SOK   ;
    RmqtState * mqtState = NULL      ;

    TRC_3ENTER ("RMQT_Put", mqtHandle, msgqId, msg) ;

    /*  Since Put is a time critical operation parameter validation is not being
     *  done here.
     */

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle->mqtInfo != NULL)
                 && (   msgqId
                      < (((RmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;
    DBC_Require (msg != NULL) ;

    mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
    DBC_Assert (mqtState != NULL) ;

    /*  Check if the destination MSGQ has been located before this call */
    if (mqtState->msgqHandles [msgqId]->msgqStatus != LdrvMsgqStatus_Inuse) {
        status = DSP_ENOTFOUND ;
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = SendMessage (mqtHandle, msgqId, msg, FALSE) ; /*  Not called
                                                                *  from DPC
                                                                *  context
                                                                */
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("RMQT_Put", status) ;

    return status ;
}


/** ============================================================================
 *  @func   RMQT_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID to be used for replying to a
 *          received message.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_GetReplyId (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgqMsg                 msg,
                 IN  MsgQueueId *            msgqId)
{
    DSP_STATUS      status       = DSP_SOK ;
    LdrvMsgqHandle  locateHandle = NULL    ;
    RmqtState *     mqtState     = NULL    ;

    TRC_2ENTER ("RMQT_GetReplyId", msg, msgqId) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (msgqId != NULL) ;
    DBC_Require (msg != NULL) ;

    if ((mqtHandle == NULL) || (msgqId == NULL) || (msg == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;

        if (msgqId != NULL) {
            *msgqId = MSGQ_INVALID_ID ;
        }
    }
    else {
        *msgqId = MSGQ_INVALID_ID ;
        mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;
        if (mqtState != NULL) {
            if (msg->srcId != MSGQ_INVALID_ID) {
                *msgqId = msg->srcId ;
                locateHandle = mqtState->msgqHandles [*msgqId] ;

                /* If the remote MSGQ was not previously located, create a new
                 * remote MSGQ object.
                 */
                if (msg->srcId < mqtState->maxNumMsgq) {
                    locateHandle->msgqStatus = LdrvMsgqStatus_Inuse ;
                }
            }
            else {
                /*  Not doing SET_FAILURE_REASON because its not a
                 *  failure actually.
                 */
                status = DSP_ENOTFOUND ;
            }
        }
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("RMQT_GetReplyId", status) ;

    return status ;
}


/** ============================================================================
 *  @func   RMQT_PutCallback
 *
 *  @desc   This function implements the callback that runs when the message to
 *          be sent to a remote MSGQ has been transferred across the physical
 *          link.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_PutCallback (IN  ProcessorId   procId,
                  IN  DSP_STATUS    statusOfIo,
                  IN  Uint8 *       buffer,
                  IN  Uint32        size,
                  IN  Pvoid         mqtHandle)
{
    DSP_STATUS          status       = DSP_SOK ;
    MsgqMsg             msgHeader    = NULL    ;
    RmqtState *         mqtState     = NULL    ;
    ListElement *       msg          = NULL    ;
    LDRVChnlIOInfo      ioInfo                 ;
    Uint32              mqtId                  ;
    Uint32              wordSize               ;

#if defined (DDSP_PROFILE)
    MsgQueueId  msgqId = 0 ;
#endif /* defined (DDSP_PROFILE) */

    TRC_5ENTER ("RMQT_PutCallback", procId,
                                   statusOfIo,
                                   buffer,
                                   size,
                                   mqtHandle) ;

    DBC_Require (buffer != NULL) ;
    DBC_Require (mqtHandle != NULL) ;

    /*  Since RMQT_PutCallback is a time critical operation parameter validation
     *  is not being done here.
     */

    if (DSP_SUCCEEDED (statusOfIo)) {
        mqtState = (RmqtState * )
                    (((LdrvMsgqTransportHandle)mqtHandle)->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;

        status = LIST_GetHead (mqtState->msgQueue, (ListElement **) &msg) ;

        if (DSP_SUCCEEDED (status)) {
            DBC_Assert (msg == (ListElement *) buffer) ;
#if defined (DDSP_PROFILE)
            msgqId = ((MsgqMsg) msg)->dstId ;
            if (msgqId != ID_RMQT_CTRL) {
                LDRV_Obj.msgqStats.msgqData [procId][msgqId].transferred++ ;
            }
#endif /* defined (DDSP_PROFILE) */

            /*  Set the dpcContext field in the ioInfo structure to TRUE to
             *  indicate that the call is being made from a DPC context.
             */
            ioInfo.dpcContext = TRUE ;
            status = LDRV_CHNL_GetIOCompletion (mqtState->procId,
                                                ID_MSGCHNL_TO_DSP,
                                                SYNC_NOWAIT,
                                                &ioInfo) ;
            if (DSP_SUCCEEDED (status)) {
                wordSize = LDRV_Obj.dspObjects [mqtState->procId].wordSize ;
                ((MsgqMsg) buffer)->size *= wordSize ;

                status = LDRV_MSGQ_Free ((MsgqMsg) buffer) ;
                if (DSP_SUCCEEDED (status)) {
                    status = LIST_First (mqtState->msgQueue, &msg) ;

                    /*  --------------------------------------------------------
                     *  Get the first element of the queue in msg. If the queue
                     *  is empty, NULL will be returned which is not an error
                     *  condition.
                     *  --------------------------------------------------------
                     */
                    if ((DSP_SUCCEEDED (status)) && (msg != NULL)) {
                        msgHeader = (MsgqMsg) msg ;
                        ioInfo.buffer     = msgHeader ;
                        ioInfo.size       = msgHeader->size * wordSize ;
                        ioInfo.arg        = (Uint32) mqtHandle ;
                        ioInfo.callback   = RMQT_PutCallback ;
                        ioInfo.dpcContext = TRUE ;
                        status = LDRV_CHNL_AddIORequest (mqtState->procId,
                                                         ID_MSGCHNL_TO_DSP,
                                                         &ioInfo) ;
                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }
                    else if (DSP_FAILED (status)){
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
    else {
        msgHeader = (MsgqMsg) buffer ;
        if (msgHeader->dstId != ID_RMQT_CTRL) {
            mqtId = (Uint32) (((LdrvMsgqTransportHandle) mqtHandle)->mqtId) ;
            status = LDRV_MSGQ_SendErrorMsg (MsgqErrorType_PutFailed,
                                             (Pvoid) (procId),
                                             (Pvoid) (mqtId),
                                             (Pvoid) (statusOfIo)) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            /* Control message put failed */
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("RMQT_PutCallback", status) ;

    return status ;
}


/** ============================================================================
 *  @func   RMQT_GetCallback
 *
 *  @desc   This function implements the callback that runs when the message has
 *          been received from the DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_GetCallback (IN  ProcessorId   procId,
                  IN  DSP_STATUS    statusOfIo,
                  IN  Uint8 *       buffer,
                  IN  Uint32        size,
                  IN  Pvoid         mqtHandle)
{
    DSP_STATUS     status       = DSP_SOK ;
    MsgqMsg        msgHeader    = NULL    ;
    MsgqMsg        newMsgHeader = NULL    ;
    RmqtState *    mqtState     = NULL    ;
    LDRVChnlIOInfo ioInfo                 ;
    AllocatorId    mqaId                  ;

    TRC_5ENTER ("RMQT_GetCallback", procId,
                                    statusOfIo,
                                    buffer,
                                    size,
                                    mqtHandle) ;

    DBC_Require (buffer != NULL) ;
    DBC_Require (mqtHandle != NULL) ;

    /*  Since RMQT_GetCallback is a time critical operation parameter validation
     *  is not being done here.
     */
    mqtState = (RmqtState * )(((LdrvMsgqTransportHandle) mqtHandle)->mqtInfo) ;
    DBC_Assert (mqtState != NULL) ;

    /*  Do not perform any callback activities if the I/O has aborted */
    if (DSP_SUCCEEDED (statusOfIo)) {
        msgHeader = (MsgqMsg) buffer ;
        if (msgHeader->dstId != ID_RMQT_CTRL) {
            msgHeader->size =  (msgHeader->size)
                             * LDRV_Obj.dspObjects [mqtState->procId].wordSize ;
            mqaId = msgHeader->mqaId ;
            status = LDRV_MSGQ_Alloc (mqaId, msgHeader->size, &newMsgHeader) ;
            if (DSP_FAILED (status)) {
                /*  If the allocator id given by the remote side was invalid.
                 */
                status = LDRV_MSGQ_Alloc (mqtState->defaultMqaId,
                                          msgHeader->size,
                                          &newMsgHeader) ;
            }

            DBC_Assert (newMsgHeader != NULL) ;

            if (DSP_SUCCEEDED (status)) {
                status = MEM_Copy ((Uint8 *) newMsgHeader,
                                   (Uint8 *) buffer,
                                   msgHeader->size,
                                   Endianism_Default) ;
                if (DSP_SUCCEEDED (status)) {
                    newMsgHeader->mqtId =
                                ((LdrvMsgqTransportHandle) mqtHandle)->mqtId ;
                    status = LDRV_MSGQ_Put (ID_LOCAL_PROCESSOR,
                                            newMsgHeader->dstId,
                                            newMsgHeader,
                                            newMsgHeader->msgId,
                                            newMsgHeader->srcId) ;
                    if (DSP_FAILED (status)) {
                        LDRV_MSGQ_Free (newMsgHeader) ;
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    LDRV_MSGQ_Free (newMsgHeader) ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = ControlMsgProcessing (procId,
                                           msgHeader,
                                           size,
                                          (LdrvMsgqTransportHandle) mqtHandle) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Reissue the getBuffer on the input channel
         *  --------------------------------------------------------------------
         */

        /*  Set the dpcContext field in the ioInfo structure to TRUE to
         *  indicate that the call is being made from a DPC context.
         */
        ioInfo.dpcContext = TRUE ;
        status = LDRV_CHNL_GetIOCompletion (mqtState->procId,
                                            ID_MSGCHNL_FM_DSP,
                                            SYNC_NOWAIT,
                                            &ioInfo) ;
        if (DSP_SUCCEEDED (status)) {
            ioInfo.buffer     = mqtState->getBuffer ;
            ioInfo.size       = mqtState->maxMsgSize ;
            ioInfo.arg        = (Uint32) mqtHandle ;
            ioInfo.callback   = RMQT_GetCallback ;
            ioInfo.dpcContext = TRUE ;
            status = LDRV_CHNL_AddIORequest (procId,
                                             ID_MSGCHNL_FM_DSP,
                                             &ioInfo) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("RMQT_GetCallback", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   RMQT_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
RMQT_Instrument (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgQueueId              msgqId,
                 OUT MsgqInstrument *        retVal)
{
    DSP_STATUS  status = DSP_SOK ;
    ProcessorId procId = 0       ;

    TRC_3ENTER ("RMQT_Instrument", mqtHandle, msgqId, retVal) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((RmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;
    DBC_Require (retVal != NULL) ;

    if (   (mqtHandle == NULL)
        || (retVal == NULL)
        || (msgqId >= ((RmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        procId = mqtHandle->procId ;

        *retVal =  LDRV_Obj.msgqStats.msgqData [procId][msgqId] ;
    }

    TRC_1LEAVE ("RMQT_Instrument", status) ;

    return status ;
}
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   RMQT_Debug
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
RMQT_Debug (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId)
{
    DSP_STATUS       status      = DSP_SOK ;
    RmqtState *      mqtState    = NULL    ;
    LdrvMsgqHandle   msgqHandle  = NULL    ;

    TRC_2ENTER ("RMQT_Debug", mqtHandle, msgqId) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((RmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
        TRC_0PRINT (TRC_LEVEL4, "Wrong parameters passed to RMQT_Debug\n") ;
    }
    else {
        mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
            TRC_0PRINT (TRC_LEVEL4, "The component is not opened \n") ;
        }
        else if (msgqId >= mqtState->maxNumMsgq) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
            TRC_0PRINT (TRC_LEVEL4, "Wrong parameters passed to RMQT_Debug\n") ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        msgqHandle = mqtState->msgqHandles [msgqId] ;
        if (msgqHandle != NULL) {
            TRC_0PRINT (TRC_LEVEL4, "This is a Remote message queue\n") ;
            TRC_1PRINT (TRC_LEVEL4,
                        "maxNumMsgq for this transport =  [0x%x]\n",
                        mqtState->maxNumMsgq) ;
            TRC_1PRINT (TRC_LEVEL4,
                        "msgqId for the message queue =  [0x%x]\n",
                        msgqHandle->msgqId) ;
            TRC_1PRINT (TRC_LEVEL4,
                        "getSem for the message queue =  [0x%x]\n",
                        msgqHandle->getSem) ;
            TRC_1PRINT (TRC_LEVEL4,
                        "mqtRepository for the message queue =  [0x%x]\n",
                        msgqHandle->mqtRepository) ;
            TRC_1PRINT (TRC_LEVEL4,
                        "mqtGet for the message queue =  [0x%x]\n",
                        msgqHandle->mqtGet) ;
            TRC_1PRINT (TRC_LEVEL4,
                        "mqtPut for the message queue =  [0x%x]\n",
                        msgqHandle->mqtPut) ;
            TRC_1PRINT (TRC_LEVEL4,
                        "msgqStatus for the message queue =  [0x%x]\n",
                        msgqHandle->msgqStatus) ;
        }
        else {
            TRC_1PRINT (TRC_LEVEL4,
                        "The MSGQ [0x%x] is not created \n", msgqId) ;
        }
    }

    TRC_1LEAVE ("RMQT_Debug", status) ;
}
#endif /* defined (DDSP_DEBUG) */


/** ----------------------------------------------------------------------------
 *  @func   CreateRemoteMsgq
 *
 *  @desc   Create and return the remote MSGQ object for specified MSGQ ID.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
CreateRemoteMsgq (IN  RmqtState *      mqtInfo,
                  IN  MsgQueueId       msgqId,
                  OUT LdrvMsgqHandle * msgqHandle)
{
    DSP_STATUS  status       = DSP_SOK ;
    RmqtObj *   rmqtObj      = NULL    ;
    SyncAttrs   attr                   ;

    TRC_3ENTER ("CreateRemoteMsgq", mqtInfo, msgqId, msgqHandle) ;

    DBC_Require (mqtInfo != NULL) ;
    DBC_Require (msgqId < mqtInfo->maxNumMsgq) ;
    DBC_Require (msgqHandle != NULL) ;

    /* Create and initialize a remote MSGQ object */
    status = MEM_Calloc ((Void **) msgqHandle,
                         (sizeof (LdrvMsgqObject)),
                         MEM_DEFAULT) ;
    if (DSP_SUCCEEDED (status)) {
        /* Initialize the non-NULL fields of the MSGQ object */
        (*msgqHandle)->mqtGet = RMQT_Interface.mqtGet ;
        (*msgqHandle)->mqtPut = RMQT_Interface.mqtPut ;
        (*msgqHandle)->msgqId = msgqId ;
        status = MEM_Calloc ((Void **) &((*msgqHandle)->mqtRepository),
                             (sizeof (RmqtObj)),
                             MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            rmqtObj = (*msgqHandle)->mqtRepository ;
            (*msgqHandle)->msgqStatus  = LdrvMsgqStatus_Empty ;
            attr.flag = SyncSemType_Counting ;
            status = SYNC_CreateSEM (&(rmqtObj->locateSem), &attr) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
                FREE_PTR ((*msgqHandle)->mqtRepository) ;
                FREE_PTR (*msgqHandle) ;
            }
        }
        else {
            SET_FAILURE_REASON ;
            FREE_PTR (*msgqHandle) ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("CreateRemoteMsgq", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   DeleteRemoteMsgq
 *
 *  @desc   Delete the remote MSGQ object for specified MSGQ ID.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
DeleteRemoteMsgq (IN  RmqtState * mqtInfo, IN  MsgQueueId  msgqId)
{
    DSP_STATUS   status       = DSP_SOK ;
    DSP_STATUS   tmpStatus    = DSP_SOK ;
    RmqtObj *    locateHandle = NULL    ;

    TRC_2ENTER ("DeleteRemoteMsgq", mqtInfo, msgqId) ;

    DBC_Require (mqtInfo != NULL) ;
    DBC_Require (msgqId < mqtInfo->maxNumMsgq) ;

    if (mqtInfo->msgqHandles [msgqId] != NULL) {
        locateHandle = (RmqtObj *)
                            (mqtInfo->msgqHandles [msgqId]->mqtRepository) ;

        DBC_Assert (locateHandle != NULL) ;
        if (locateHandle->locateSem != NULL) {
            status = SYNC_DeleteSEM (locateHandle->locateSem) ;
            locateHandle->locateSem = NULL ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }

        tmpStatus = FREE_PTR (mqtInfo->msgqHandles [msgqId]->mqtRepository) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = FREE_PTR (mqtInfo->msgqHandles [msgqId]) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("DeleteRemoteMsgq", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   ControlMsgProcessing
 *
 *  @desc   This function implements the callback for control messages, which
 *          runs when the message has been received from the DSP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
ControlMsgProcessing (IN  ProcessorId             procId,
                      IN  MsgqMsg                 msgHeader,
                      IN  Uint32                  size,
                      IN  LdrvMsgqTransportHandle mqtHandle)
{
    DSP_STATUS      status       = DSP_SOK ;
    DSP_STATUS      tmpStatus    = DSP_SOK ;
    RmqtCtrlMsg *   ctrlMsg      = NULL    ;
    RmqtCtrlMsg *   locateMsg    = NULL    ;
    LdrvMsgqHandle  locateMsgq   = NULL    ;
    RmqtObj *       locateHandle = NULL    ;
    MsgQueueId      msgqId       = 0       ;
    RmqtState *     mqtState     = NULL    ;
    MsgqLocateAttrs attrs                  ;

    TRC_4ENTER ("ControlMsgProcessing", procId, msgHeader, size, mqtHandle) ;

    DBC_Require (msgHeader != NULL) ;
    DBC_Require (mqtHandle != NULL) ;

    mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
    ctrlMsg = (RmqtCtrlMsg *) msgHeader ;
    if (msgHeader->msgId == RmqtCtrlCmd_Locate) {
        msgqId  = ctrlMsg->ctrlMsg.locateMsg.msgqId ;
        status = LDRV_MSGQ_Locate (ID_LOCAL_PROCESSOR,
                                   msgqId,
                                   &attrs) ;
        if ((DSP_SUCCEEDED (status)) || (status == DSP_ENOTFOUND)) {
            tmpStatus = status ;
            status = LDRV_MSGQ_Alloc (mqtState->defaultMqaId,
                                      RMQT_CTRLMSG_SIZE,
                                      (MsgqMsg *) &locateMsg) ;
            if (DSP_SUCCEEDED (status)) {
                /*  Set fields in the locateAckMsg as expected by the DSP */
                locateMsg->ctrlMsg.locateAckMsg.msgqId
                                      = ctrlMsg->ctrlMsg.locateMsg.msgqId ;
                locateMsg->ctrlMsg.locateAckMsg.mqaId
                                      = ctrlMsg->ctrlMsg.locateMsg.mqaId ;
                locateMsg->ctrlMsg.locateAckMsg.timeout
                                      = ctrlMsg->ctrlMsg.locateMsg.timeout ;
                locateMsg->ctrlMsg.locateAckMsg.replyHandle
                                      = ctrlMsg->ctrlMsg.locateMsg.replyHandle ;
                locateMsg->ctrlMsg.locateAckMsg.arg
                                      = ctrlMsg->ctrlMsg.locateMsg.arg ;
                locateMsg->ctrlMsg.locateAckMsg.semHandle
                                      = ctrlMsg->ctrlMsg.locateMsg.semHandle ;

                ((MsgqMsg) locateMsg)->msgId = RmqtCtrlCmd_LocateAck ;
                ((MsgqMsg) locateMsg)->dstId = ID_RMQT_CTRL ;
                ((MsgqMsg) locateMsg)->mqtId = mqtHandle->mqtId ;
                if (DSP_SUCCEEDED (tmpStatus)) {
                    locateMsg->ctrlMsg.locateAckMsg.msgqFound = 1 ;
                }
                else {
                    locateMsg->ctrlMsg.locateAckMsg.msgqFound = 0 ;
                }

                status = SendMessage (mqtHandle,
                                      ID_RMQT_CTRL,
                                      (MsgqMsg) locateMsg,
                                      TRUE) ; /* Called from DPC context */
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
    else if (msgHeader->msgId == RmqtCtrlCmd_LocateAck) {
        /*  Locate acknowledgement control message  */
        msgqId     = ctrlMsg->ctrlMsg.locateAckMsg.msgqId ;
        locateMsgq = mqtState->msgqHandles [msgqId] ;

        if (ctrlMsg->ctrlMsg.locateAckMsg.msgqFound == 1) {
            locateMsgq->msgqStatus  = LdrvMsgqStatus_Inuse ;
        }
        else {
            locateMsgq->msgqStatus  = LdrvMsgqStatus_Empty ;
        }
        locateHandle = (RmqtObj *) locateMsgq->mqtRepository ;
        DBC_Assert (locateHandle->locateSem != NULL) ;
        status = SYNC_SignalSEM (locateHandle->locateSem) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else if (msgHeader->msgId == RmqtCtrlCmd_Exit) {
        /* MQT exit notification control message */
        status = LDRV_MSGQ_SendErrorMsg (MsgqErrorType_MqtExit,
                                         (Pvoid) ((Uint32)(mqtHandle->mqtId)),
                                         NULL,
                                         NULL) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("ControlMsgProcessing", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   SendMessage
 *
 *  @desc   This function sends a message to the DSP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
SendMessage (IN  LdrvMsgqTransportHandle mqtHandle,
             IN  MsgQueueId              msgqId,
             IN  MsgqMsg                 msg,
             IN  Bool                    dpcContext)
{
    DSP_STATUS          status       = DSP_SOK   ;
    RmqtState *         mqtState     = NULL      ;
    Bool                listEmpty    = FALSE     ;
    LDRVChnlIOInfo      ioInfo                   ;
    Uint32              wordSize                 ;
    Uint32              msgSize                  ;

    TRC_3ENTER ("SendMessage", mqtHandle, msgqId, msg) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle->mqtInfo != NULL)
                 && (   (msgqId
                         < (((RmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))
                     || (msgqId == ID_RMQT_CTRL))) ;
    DBC_Require (msg != NULL) ;

    mqtState = (RmqtState *) (mqtHandle->mqtInfo) ;
    DBC_Assert (mqtState != NULL) ;

    wordSize = LDRV_Obj.dspObjects [mqtState->procId].wordSize ;
    /*  MSG size must be multiple of wordSize  */
    DBC_Assert ((msg->size % wordSize) == 0) ;
    msgSize = msg->size ;
    msg->size  /=  wordSize ;

    if (dpcContext == FALSE) {
        SYNC_ProtectionStart () ;
    }

    if (LIST_IsEmpty (mqtState->msgQueue)) {
        listEmpty = TRUE ;
    }
    /* Add the message to the end of the message queue */
    status = LIST_PutTail (mqtState->msgQueue, (ListElement *) msg) ;
    if (dpcContext == FALSE) {
        SYNC_ProtectionEnd () ;
    }

    if (DSP_SUCCEEDED (status)) {
        if (listEmpty == TRUE) {
            ioInfo.buffer     = msg ;
            ioInfo.size       = msgSize ;
            ioInfo.arg        = (Uint32) mqtHandle ;
            ioInfo.callback   = RMQT_PutCallback ;
            ioInfo.dpcContext = dpcContext ;
            status = LDRV_CHNL_AddIORequest (mqtState->procId,
                                             ID_MSGCHNL_TO_DSP,
                                             &ioInfo) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SendMessage", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
