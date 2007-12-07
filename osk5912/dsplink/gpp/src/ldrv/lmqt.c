/** ============================================================================
 *  @file   lmqt.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implements the local MQT subcomponent.
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
#include <lmqt.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent ID.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_LMQT


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_LMQT, __LINE__)


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @name   LDRV_Obj
 *
 *  @desc   Global defined in ldrv.c.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;
#endif /* if defined (DDSP_PROFILE) */


/** ============================================================================
 *  @name   LMQT_Interface
 *
 *  @desc   The MQT APIs are exposed to MSGQ through the function table.
 *  ============================================================================
 */
MqtInterface LMQT_Interface = {
    &LMQT_Initialize,
    &LMQT_Finalize,
    &LMQT_Open,
    &LMQT_Close,
    &LMQT_Create,
    &LMQT_Locate,
    &LMQT_Delete,
    &LMQT_Release,
    &LMQT_Get,
    &LMQT_Put,
    &LMQT_GetReplyId,
    (FnMqtGetById) &LDRV_MSGQ_NotImpl,
#if defined (DDSP_PROFILE)
    &LMQT_Instrument,
#endif /* defined (DDSP_PROFILE) */
#if defined (DDSP_DEBUG)
    &LMQT_Debug,
#endif /* defined (DDSP_DEBUG) */
} ;


/** ============================================================================
 *  @func   LMQT_Initialize
 *
 *  @desc   This function performs global initialization of the local MQT.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LMQT_Initialize ()
{
    TRC_0ENTER ("LMQT_Initialize") ;

    /*  No initialization code is required.  */

    TRC_0LEAVE ("LMQT_Initialize") ;
}


/** ============================================================================
 *  @func   LMQT_Finalize
 *
 *  @desc   This function performs global finalization of the local MQT.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LMQT_Finalize ()
{
    TRC_0ENTER ("LMQT_Finalize") ;

    /*  No finalization code is required.  */

    TRC_0LEAVE ("LMQT_Finalize") ;
}


/** ============================================================================
 *  @func   LMQT_Open
 *
 *  @desc   This function opens the local MQT and configures it according to the
 *          user attributes.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Open (IN  LdrvMsgqTransportHandle mqtHandle, IN  Pvoid mqtAttrs)
{
    DSP_STATUS      status      = DSP_SOK ;
    LmqtState *     mqtState    = NULL    ;
    LmqtAttrs *     lmqtAttrs   = NULL    ;
    Uint16          maxNumMsgq  = 0       ;

    TRC_2ENTER ("LMQT_Open", mqtHandle, mqtAttrs) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (mqtAttrs != NULL) ;

    if ((mqtHandle == NULL) || (mqtAttrs == NULL)) {
        status = DSP_EINVALIDARG ;
        if (mqtHandle != NULL) {
            /*  Initialize the mqtInfo to NULL.  */
            mqtHandle->mqtInfo = NULL ;
        }
        SET_FAILURE_REASON ;
    }
    else {
        /*  Initialize the mqtInfo to NULL.  */
        mqtHandle->mqtInfo = NULL ;

        lmqtAttrs = (LmqtAttrs *) mqtAttrs ;

        /* Check if the maximum number of MSGQs is valid. */
        if (lmqtAttrs->maxNumMsgq <= MAX_MSGQS) {
            maxNumMsgq = lmqtAttrs->maxNumMsgq ;

            /*  Allocate memory for the state object.  */
            status = MEM_Alloc ((Void **) &mqtState,
                                (sizeof (LmqtState)),
                                MEM_DEFAULT) ;
            if (DSP_SUCCEEDED (status)) {
                mqtState->maxNumMsgq = maxNumMsgq ;

                if (maxNumMsgq > 0) {
                    /*  Allocate memory for the msgq handles.  */
                    status = MEM_Calloc ((Void **) &(mqtState->msgqHandles),
                                        (maxNumMsgq * sizeof (LdrvMsgqHandle)),
                                        MEM_DEFAULT) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                        FREE_PTR (mqtState) ;
                    }
                }
                else {
                    mqtState->msgqHandles = NULL ;
                }

                if (DSP_SUCCEEDED (status)) {
                    mqtHandle->mqtInfo = mqtState ;
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
    }

    TRC_1LEAVE ("LMQT_Open", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_Close
 *
 *  @desc   This function closes the local MQT, and cleans up its state object.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Close (IN  LdrvMsgqTransportHandle mqtHandle)
{
    DSP_STATUS      status      = DSP_SOK ;
    DSP_STATUS      tmpStatus   = DSP_SOK ;
    LmqtState *     mqtState    = NULL    ;
    Uint16          i           = 0       ;

    TRC_1ENTER ("LMQT_Close", mqtHandle) ;

    DBC_Require (mqtHandle != NULL) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;
        mqtHandle->mqtInfo = NULL ;

        if (mqtState != NULL) {
            /*  ----------------------------------------------------------------
             *  Delete all the local queues.
             *  ----------------------------------------------------------------
             */
            for (i = 0 ; i < mqtState->maxNumMsgq ; i++) {
                if (mqtState->msgqHandles [i] != NULL) {
                    tmpStatus = LMQT_Delete (mqtHandle, i) ;
                    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
            }

            /*  ----------------------------------------------------------------
             *  Free the msgqHandles memory.
             *  ----------------------------------------------------------------
             */
            if (mqtState->msgqHandles != NULL) {
                tmpStatus = FREE_PTR (mqtState->msgqHandles) ;
                if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
            }

            /*  ----------------------------------------------------------------
             *  Free the mqt state object and set to NULL the mqtInfo field in
             *  LDRV MSGQ transport state object.
             *  ----------------------------------------------------------------
             */
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

    TRC_1LEAVE ("LMQT_Close", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_Create
 *
 *  @desc   This function creates the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Create (IN     LdrvMsgqTransportHandle mqtHandle,
             IN     MsgQueueId              msgqId,
             IN OPT MsgqAttrs *             attrs)
{
    DSP_STATUS      status      = DSP_SOK ;
    LmqtState *     mqtState    = NULL    ;
    LmqtObj *       ptrMsgq     = NULL    ;
    SyncSemObject * getSem      = NULL    ;
    LdrvMsgqHandle  msgqHandle  = NULL    ;
    SyncAttrs       attr                  ;

    TRC_3ENTER ("LMQT_Create", mqtHandle, msgqId, attrs) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Assert ((mqtHandle != NULL) && (mqtHandle->mqtInfo != NULL)) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else if (msgqId >= mqtState->maxNumMsgq){
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        /*  Check if the queue already exists */
        if (mqtState->msgqHandles [msgqId] != NULL) {
            /*  ----------------------------------------------------------------
             *  This queue already exists.
             *  ----------------------------------------------------------------
             */
            status = DSP_EALREADYEXISTS ;
            SET_FAILURE_REASON ;
        }
    }

    SYNC_ProtectionStart () ;
    if (DSP_SUCCEEDED (status)) {
        status = MEM_Alloc ((Void **) &(mqtState->msgqHandles [msgqId]),
                            (sizeof (LdrvMsgqObject)),
                            MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            /*  Allocate memory for the LmqtObj object.  */
            status = MEM_Alloc ((Void **) &ptrMsgq,
                                (sizeof (LmqtObj)),
                                MEM_DEFAULT) ;
            if (DSP_SUCCEEDED (status)) {
                mqtState->msgqHandles [msgqId]->mqtRepository = ptrMsgq ;
                /*  Create the list for the local queue.  */
                status = LIST_Create (&(ptrMsgq->msgQueue)) ;
                if (DSP_SUCCEEDED (status)) {
                    /*  --------------------------------------------------------
                     *  Create the semaphore object and set the fields
                     *  in msgqHandle.
                     *  --------------------------------------------------------
                     */
                    attr.flag = SyncSemType_Binary ;
                    status = SYNC_CreateSEM (&getSem, &attr) ;
                    if (DSP_SUCCEEDED (status)) {
                        msgqHandle = mqtState->msgqHandles [msgqId] ;

                        msgqHandle->getSem     = getSem ;
                        msgqHandle->mqtGet     = LMQT_Interface.mqtGet ;
                        msgqHandle->mqtPut     = LMQT_Interface.mqtPut ;
                        msgqHandle->msgqId     = msgqId ;
                        msgqHandle->msgqStatus = LdrvMsgqStatus_Inuse ;
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
            SET_FAILURE_REASON ;
        }
        /*  --------------------------------------------------------------------
         *  If status is invalid do the cleanup of the queue.
         *  --------------------------------------------------------------------
         */
        if (DSP_FAILED (status)) {
            LMQT_Delete (mqtHandle, msgqId) ;
        }
    }

    SYNC_ProtectionEnd () ;

    TRC_1LEAVE ("LMQT_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_Delete
 *
 *  @desc   This function deletes the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Delete (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId)
{
    DSP_STATUS     status      = DSP_SOK ;
    DSP_STATUS     tmpStatus   = DSP_SOK ;
    LmqtState *    mqtState    = NULL    ;
    LmqtObj *      ptrMsgq     = NULL    ;
    LdrvMsgqHandle msgqHandle  = NULL    ;
    MsgqMsg        msg         = NULL    ;

    TRC_2ENTER ("LMQT_Delete", mqtHandle, msgqId) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Assert ((mqtHandle != NULL) && (mqtHandle->mqtInfo != NULL)) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else if (msgqId >= mqtState->maxNumMsgq){
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        DBC_Assert (mqtState->msgqHandles != NULL) ;
        msgqHandle = mqtState->msgqHandles [msgqId] ;
        mqtState->msgqHandles [msgqId] = NULL ;
        if (msgqHandle != NULL) {
            /*  ----------------------------------------------------------------
             *  If getSem is not NULL, delete the semaphore and set it to
             *  NULL.
             *  ----------------------------------------------------------------
             */
            if (msgqHandle->getSem != NULL) {
                status = SYNC_DeleteSEM (msgqHandle->getSem) ;
                if (DSP_SUCCEEDED (status)) {
                    SET_FAILURE_REASON ;
                }
                msgqHandle->getSem = NULL ;
            }

            ptrMsgq = msgqHandle->mqtRepository ;
            /*  ----------------------------------------------------------------
             *  If ptrMsgq is not NULL delete the list object and free the
             *  ptrMsgq.
             *  ----------------------------------------------------------------
             */
            if (ptrMsgq != NULL) {
                if (ptrMsgq->msgQueue != NULL) {
                    SYNC_ProtectionStart () ;
                    while (   (LIST_IsEmpty (ptrMsgq->msgQueue) != TRUE)
                           && (DSP_SUCCEEDED (tmpStatus))) {
                        tmpStatus = LIST_GetHead (ptrMsgq->msgQueue,
                                                  (ListElement **) &msg) ;
                        if ((DSP_SUCCEEDED (tmpStatus)) && (msg != NULL)) {
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
                    tmpStatus = LIST_Delete (ptrMsgq->msgQueue) ;
                    if (   (DSP_SUCCEEDED (status))
                        && (DSP_FAILED (tmpStatus))) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                    SYNC_ProtectionEnd () ;
                }

                tmpStatus = FREE_PTR (ptrMsgq) ;
                if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
            }

            tmpStatus = FREE_PTR (msgqHandle) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }
    }

    TRC_1LEAVE ("LMQT_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Locate (IN  LdrvMsgqTransportHandle mqtHandle,
             IN  MsgQueueId              msgqId,
             IN  MsgqLocateAttrs *       attrs)
{
    DSP_STATUS      status      = DSP_SOK   ;
    LmqtState *     mqtState    = NULL      ;

    TRC_3ENTER ("LMQT_Locate", mqtHandle, msgqId, attrs) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Assert ((mqtHandle != NULL) && (mqtHandle->mqtInfo != NULL)) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;
    DBC_Require (attrs != NULL) ;

    if ((mqtHandle == NULL) || (attrs == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else if (msgqId >= mqtState->maxNumMsgq) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
        else if (mqtState->msgqHandles [msgqId] == NULL) {
            status = DSP_ENOTFOUND ;
        }
    }

    TRC_1LEAVE ("LMQT_Locate", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier
 *          LMQT_Locate () call.
 *
 *          NOTE : Since the MQT is local nothing needs to be done.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Release (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId)
{
    DSP_STATUS  status   = DSP_SOK ;
    LmqtState * mqtState = NULL   ;

    TRC_2ENTER ("LMQT_Release", mqtHandle, msgqId) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Assert ((mqtHandle != NULL) && (mqtHandle->mqtInfo != NULL)) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else if (msgqId >= mqtState->maxNumMsgq) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
        else if (mqtState->msgqHandles [msgqId] == NULL) {
            status = DSP_ENOTFOUND ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LMQT_Release", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_Get
 *
 *  @desc   This function receives a message on the specified MSGQ.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Get (IN  LdrvMsgqTransportHandle mqtHandle,
          IN  MsgQueueId              msgqId,
          IN  Uint32                  timeout,
          OUT MsgqMsg *               msg)
{
    DSP_STATUS  status   = DSP_SOK ;
    LmqtState * mqtState = NULL    ;
    LmqtObj *   ptrMsgq  = NULL    ;

    TRC_4ENTER ("LMQT_Get", mqtHandle, msgqId, timeout, msg) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Assert ((mqtHandle != NULL) && (mqtHandle->mqtInfo != NULL)) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;
    DBC_Require (msg != NULL) ;

    /*  Since Get is a time critical operation parameter validation is not being
     *  done here.
     */
    mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
    DBC_Assert (mqtState->msgqHandles != NULL) ;
    DBC_Assert (mqtState->msgqHandles [msgqId] != NULL) ;
    DBC_Assert (mqtState->msgqHandles [msgqId]->getSem != NULL) ;

    ptrMsgq = (LmqtObj *)(mqtState->msgqHandles [msgqId]->mqtRepository) ;
    DBC_Assert (ptrMsgq != NULL) ;

    *msg = NULL ;
    while ((DSP_SUCCEEDED (status)) && (*msg == NULL)) {
        /*  --------------------------------------------------------------------
         *  Get the first element of the queue in msg. If the queue is empty,
         *  NULL will be returned which is not an error condition.
         *  --------------------------------------------------------------------
         */
        SYNC_ProtectionStart () ;
        status = LIST_GetHead (ptrMsgq->msgQueue, (ListElement **) msg) ;
        SYNC_ProtectionEnd () ;

        /*  --------------------------------------------------------------------
         *  Wait on the semaphore till the time a message comes in this local
         *  queue. This semaphore will be posted by LMQT_Put.
         *  If the semaphore was already posted, below call returns immediately.
         *  However, the list could still be empty, because the message might
         *  have been taken away by a previous call. In this case, try to get
         *  the message once more (while loop), and wait once more if a message
         *  was not available.
         *  --------------------------------------------------------------------
         */
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        else if (*msg == NULL) {
            status = SYNC_WaitSEM (mqtState->msgqHandles [msgqId]->getSem,
                                   timeout) ;
            if ((timeout == SYNC_NOWAIT) && (status == SYNC_E_FAIL)) {
                status = DSP_ENOTCOMPLETE ;
                SET_FAILURE_REASON ;
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
    }

    TRC_1LEAVE ("LMQT_Get", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_Put
 *
 *  @desc   This function sends a message to the specified local MSGQ.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Put (IN  LdrvMsgqTransportHandle mqtHandle,
          IN  MsgQueueId              msgqId,
          IN  MsgqMsg                 msg)
{
    DSP_STATUS  status   = DSP_SOK ;
    LmqtState * mqtState = NULL    ;
    LmqtObj *   ptrMsgq  = NULL    ;

    TRC_3ENTER ("LMQT_Put", mqtHandle, msgqId, msg) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Assert  ((mqtHandle != NULL) && (mqtHandle->mqtInfo != NULL)) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;
    DBC_Require (msg != NULL) ;

    /*  Since Put is a time critical operation parameter validation is not being
     *  done here.
     */
    mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
    if (mqtState == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        DBC_Assert (mqtState->msgqHandles [msgqId] != NULL) ;
        /*  --------------------------------------------------------------------
         *  Put the message in the requested queue if it exists. Also post the
         *  semaphore on which the client will wait / is already waiting in get
         *  operation.
         *  --------------------------------------------------------------------
         */
        if (mqtState->msgqHandles [msgqId] != NULL) {
            ptrMsgq = (LmqtObj * )
                               (mqtState->msgqHandles [msgqId]->mqtRepository) ;

            /* Currently no protection is provided for the queue operation,
             * since it is assumed to be called from DPC context.
             * Local processor messaging is not supported.
             */
            status = LIST_PutTail (ptrMsgq->msgQueue, (ListElement *) msg) ;
            if (DSP_SUCCEEDED (status)) {
                DBC_Assert (mqtState->msgqHandles [msgqId]->getSem != NULL) ;
                status = SYNC_SignalSEM (
                                       mqtState->msgqHandles [msgqId]->getSem) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_ENOTFOUND ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LMQT_Put", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LMQT_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID to be used for replying to a
 *          received message.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_GetReplyId (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgqMsg                 msg,
                 IN  MsgQueueId *            msgqId)
{
    DSP_STATUS  status  = DSP_SOK ;
    LmqtState * mqtState= NULL    ;

    TRC_2ENTER ("LMQT_GetReplyId", msg, msgqId) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (msgqId != NULL) ;

    if ((mqtHandle == NULL) || (msgqId == NULL) || (msg == NULL)) {
        if (msgqId != NULL) {
            *msgqId = MSGQ_INVALID_ID ;
        }
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;
        if (mqtState == NULL) {
            *msgqId = MSGQ_INVALID_ID ;
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Extract the srcId from the message and put it in location pointed to
         *  by msgqId.
         *  --------------------------------------------------------------------
         */
        *msgqId = msg->srcId ;
    }

    TRC_1LEAVE ("LMQT_GetReplyId", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LMQT_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LMQT_Instrument (IN  LdrvMsgqTransportHandle mqtHandle,
                 IN  MsgQueueId              msgqId,
                 OUT MsgqInstrument *        retVal)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("LMQT_Instrument", mqtHandle, msgqId, retVal) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;
    DBC_Require (retVal != NULL) ;

    if (   (mqtHandle == NULL)
        || (retVal == NULL)
        || (msgqId >= ((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        *retVal = LDRV_Obj.msgqStats.localMsgqData [msgqId] ;
    }

    TRC_1LEAVE ("LMQT_Instrument", status) ;

    return status ;
}
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LMQT_Debug
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LMQT_Debug (IN  LdrvMsgqTransportHandle mqtHandle, IN  MsgQueueId msgqId)
{
    DSP_STATUS       status      = DSP_SOK ;
    LmqtState *      mqtState    = NULL    ;
    LdrvMsgqHandle   msgqHandle  = NULL    ;

    TRC_2ENTER ("LMQT_Debug", mqtHandle, msgqId) ;

    DBC_Require (mqtHandle != NULL) ;
    DBC_Require (   (mqtHandle != NULL)
                 && (mqtHandle->mqtInfo != NULL)
                 && (  msgqId
                     < (((LmqtState *) (mqtHandle->mqtInfo))->maxNumMsgq))) ;

    if (mqtHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
        TRC_0PRINT (TRC_LEVEL4, "Wrong parameters passed to LMQT_Debug\n") ;
    }
    else {
        mqtState = (LmqtState *) (mqtHandle->mqtInfo) ;
        DBC_Assert (mqtState != NULL) ;
        if (mqtState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
            TRC_0PRINT (TRC_LEVEL4, "The component is not opened \n") ;
        }
        else if (msgqId >= mqtState->maxNumMsgq){
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
            TRC_0PRINT (TRC_LEVEL4, "Wrong parameters passed to LMQT_Debug\n") ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        msgqHandle = mqtState->msgqHandles [msgqId] ;
        if (msgqHandle != NULL) {
            TRC_0PRINT (TRC_LEVEL4, "This is a Local message queue\n") ;
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
                        "The MSGQ [0x%x] is not created \n",
                        msgqId) ;
        }
    }

    TRC_1LEAVE ("LMQT_Debug", status) ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
