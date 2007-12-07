/** ============================================================================
 *  @file   pmgr_msgq.c
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr
 *
 *  @desc   This file implements the functions of PMGR_MSGQ subcomponent.
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
#include <errbase.h>
#include <safe.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug                 */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Header                   */
#include <mem.h>
#include <sync.h>
#include <prcs.h>
#include <print.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- OSAL Header                   */
#include <cfg.h>

/*  ----------------------------------- Generic Function              */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Processor Manager             */
#include <pmgr_proc.h>
#include <pmgr_msgq.h>

/*  ----------------------------------- Link Driver                   */
#include <ldrv_mqt.h>
#include <ldrv_mqa.h>
#include <ldrv_msgq.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   COMPONENT_ID
 *
 *  @desc   Component ID of this module. That will be used by TRC macros.
 *  ============================================================================
 */
#define COMPONENT_ID        ID_PMGR_MSGQ

/** ============================================================================
 *  @name   SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason (defined in GEN).
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_PMGR_MSGQ, __LINE__)

/** ============================================================================
 *  @name   PMGR_MSGQ_OwnerArray
 *
 *  @desc   Array of MSGQ owners.
 *  ============================================================================
 */
STATIC PrcsObject * PMGR_MSGQ_OwnerArray [MAX_MSGQS] ;

/** ============================================================================
 *  @name   PMGR_MqtOwnerArray
 *
 *  @desc   Array of transport owners.
 *  ============================================================================
 */
STATIC PrcsObject ** PMGR_MqtOwnerArray ;

/** ============================================================================
 *  @name   PMGR_MqaOwnerArray
 *
 *  @desc   Array of allocator owners.
 *  ============================================================================
 */
STATIC PrcsObject ** PMGR_MqaOwnerArray ;

/** ============================================================================
 *  @name   PMGR_MSGQ_IsInitialized
 *
 *  @desc   Flag to keep track of initialization of this subcomponent.
 *  ============================================================================
 */
STATIC Bool PMGR_MSGQ_IsInitialized = FALSE ;


/** ----------------------------------------------------------------------------
 *  @func   PMGR_MSGQ_IsOwner
 *
 *  @desc   Checks if the calling client is the owner.
 *
 *  @arg    prcsInfo
 *              Pointer to the PRCS object with which the PRCS information of
 *              the current client is to be compared.
 *
 *  @ret    TRUE
 *              If the calling client has created the MSGQ.
 *          FALSE
 *              Otherwise.
 *
 *  @enter  msgqId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Bool
PMGR_MSGQ_IsOwner (PrcsObject * prcsInfo) ;


/** ============================================================================
 *  @func   PMGR_MSGQ_Setup
 *
 *  @desc   This function initializes the MSGQ component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Setup ()
{
    DSP_STATUS  status  = DSP_SOK ;
    Uint16      i       = 0       ;
    Uint32      numMqas           ;
    Uint32      numMqts           ;

    TRC_0ENTER ("PMGR_MSGQ_Setup") ;

    DBC_Require (PMGR_MSGQ_IsInitialized == FALSE) ;

    status = LDRV_MSGQ_Setup () ;
    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; i < MAX_MSGQS ; i++) {
            PMGR_MSGQ_OwnerArray [i] = NULL ;
        }
        status = CFG_GetNumValue (CFG_DRIVER_NUMMQTS, 0, &numMqts) ;
        if (DSP_SUCCEEDED (status)) {
            status = MEM_Calloc ((Void **) &(PMGR_MqtOwnerArray),
                                (numMqts * sizeof (PrcsObject *)),
                                MEM_DEFAULT) ;
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

    if (DSP_SUCCEEDED (status)) {
        status = CFG_GetNumValue (CFG_DRIVER_NUMMQAS, 0, &numMqas) ;
        if (DSP_SUCCEEDED (status)) {
            status = MEM_Calloc ((Void **) &(PMGR_MqaOwnerArray),
                                (numMqas * sizeof (PrcsObject *)),
                                MEM_DEFAULT) ;
            if (DSP_FAILED (status)) {
                FREE_PTR (PMGR_MqtOwnerArray) ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            FREE_PTR (PMGR_MqtOwnerArray) ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        PMGR_MSGQ_IsInitialized = TRUE ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Setup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Destroy
 *
 *  @desc   This function finalizes the MSGQ component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Destroy ()
{
    DSP_STATUS  status    = DSP_SOK ;
    DSP_STATUS  tmpStatus = DSP_SOK ;
    Uint16      i         = 0       ;

    TRC_0ENTER ("PMGR_MSGQ_Destroy") ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;

    if (PMGR_MSGQ_IsInitialized) {
        PMGR_MSGQ_IsInitialized = FALSE ;
        for (i = 0 ; i < MAX_MSGQS ; i++) {
            PMGR_MSGQ_OwnerArray [i] = NULL ;
        }

        status = FREE_PTR (PMGR_MqaOwnerArray) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        tmpStatus = FREE_PTR (PMGR_MqtOwnerArray) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
        tmpStatus = LDRV_MSGQ_Destroy () ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Destroy", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_AllocatorOpen
 *
 *  @desc   This function initializes the allocator component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_AllocatorOpen (IN  AllocatorId mqaId,
                         IN  Pvoid       mqaAttrs,
                         OUT Pvoid *     mqaInfo)
{
    DSP_STATUS   status   = DSP_SOK ;
    PrcsObject * prcsInfo = NULL    ;
    Uint32       numMqas            ;

    TRC_3ENTER ("PMGR_MSGQ_AllocatorOpen", mqaId, mqaAttrs, mqaInfo) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (mqaAttrs != NULL) ;
    DBC_Require (mqaInfo != NULL) ;

    if (mqaInfo == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        status = CFG_GetNumValue (CFG_DRIVER_NUMMQAS, 0, &numMqas) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        else if (mqaId >= numMqas) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        status = PRCS_Create (&prcsInfo) ;
        if (DSP_SUCCEEDED (status)) {
            status = LDRV_MSGQ_AllocatorOpen (mqaId, mqaAttrs, mqaInfo) ;
            if (DSP_FAILED (status)) {
                PRCS_Delete (prcsInfo) ;
                SET_FAILURE_REASON ;
            }
            else {
                PMGR_MqaOwnerArray [mqaId] = prcsInfo ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_MSGQ_AllocatorOpen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_AllocatorClose
 *
 *  @desc   This function finalizes the allocator component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_AllocatorClose (IN  AllocatorId mqaId)
{
    DSP_STATUS  status    = DSP_SOK ;
    DSP_STATUS  tmpStatus = DSP_SOK ;
    Uint32      numMqas             ;

    TRC_1ENTER ("PMGR_MSGQ_AllocatorClose", mqaId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;

    status = CFG_GetNumValue (CFG_DRIVER_NUMMQAS, 0, &numMqas) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    else if (mqaId >= numMqas) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        if (PMGR_MSGQ_IsOwner (PMGR_MqaOwnerArray [mqaId])) {
            status = LDRV_MSGQ_AllocatorClose (mqaId) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
            tmpStatus = PRCS_Delete (PMGR_MqaOwnerArray [mqaId]) ;
            PMGR_MqaOwnerArray [mqaId] = NULL ;
            if ((DSP_FAILED (tmpStatus)) && (DSP_SUCCEEDED (status))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_EACCESSDENIED ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_MSGQ_AllocatorClose", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_TransportOpen
 *
 *  @desc   This function initializes the MQT component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_TransportOpen (IN  TransportId mqtId, IN  Pvoid mqtAttrs)
{
    DSP_STATUS   status   = DSP_SOK ;
    PrcsObject * prcsInfo = NULL    ;
    Uint32       numMqts            ;

    TRC_2ENTER ("PMGR_MSGQ_TransportOpen", mqtId, mqtAttrs) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (mqtAttrs != NULL) ;

    status = CFG_GetNumValue (CFG_DRIVER_NUMMQTS, 0, &numMqts) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    else if (mqtId >= numMqts) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        if (PMGR_MqtOwnerArray [mqtId] == NULL) {
            status = PRCS_Create (&prcsInfo) ;
            if (DSP_SUCCEEDED (status)) {
                status = LDRV_MSGQ_TransportOpen (mqtId, mqtAttrs) ;
                if (DSP_FAILED (status)) {
                    PRCS_Delete (prcsInfo) ;
                    SET_FAILURE_REASON ;
                }
                else {
                    PMGR_MqtOwnerArray [mqtId] = prcsInfo ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            /* Transport already opened */
            status = DSP_EACCESSDENIED ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_MSGQ_TransportOpen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_TransportClose
 *
 *  @desc   This function finalizes the MQT component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_TransportClose (IN  TransportId mqtId)
{
    DSP_STATUS  status    = DSP_SOK ;
    DSP_STATUS  tmpStatus = DSP_SOK ;
    Uint32      numMqts             ;

    TRC_1ENTER ("PMGR_MSGQ_TransportClose", mqtId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;

    status = CFG_GetNumValue (CFG_DRIVER_NUMMQTS, 0, &numMqts) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    else if (mqtId >= numMqts) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        if (PMGR_MqtOwnerArray [mqtId] != NULL) {
            if (PMGR_MSGQ_IsOwner (PMGR_MqtOwnerArray [mqtId])) {
                status = LDRV_MSGQ_TransportClose (mqtId) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
                tmpStatus = PRCS_Delete (PMGR_MqtOwnerArray [mqtId]) ;
                PMGR_MqtOwnerArray [mqtId] = NULL ;
                if ((DSP_FAILED (tmpStatus)) && (DSP_SUCCEEDED (status))) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                status = DSP_EACCESSDENIED ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            /* Transport not opened */
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_MSGQ_TransportClose", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Create
 *
 *  @desc   This function creates the message queue to be used for receiving
 *          messages, identified through the specified MSGQ ID.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Create (IN  MsgQueueId msgqId, IN OPT MsgqAttrs * msgqAttrs)
{
    DSP_STATUS   status     = DSP_SOK ;
    PrcsObject * prcsInfo   = NULL    ;

    TRC_2ENTER ("PMGR_MSGQ_Create", msgqId, msgqAttrs) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    if (PMGR_MSGQ_OwnerArray [msgqId] == NULL) {
        status = PRCS_Create (&prcsInfo) ;
        if (DSP_SUCCEEDED (status)) {
            PMGR_MSGQ_OwnerArray [msgqId] = prcsInfo ;
            status = LDRV_MSGQ_Create (msgqId, msgqAttrs) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
                PMGR_MSGQ_OwnerArray [msgqId] = NULL ;
                PRCS_Delete (prcsInfo) ;
                prcsInfo = NULL ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EALREADYEXISTS ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Delete
 *
 *  @desc   This function deletes the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Delete (IN  MsgQueueId msgqId)
{
    DSP_STATUS   status    = DSP_SOK ;
    DSP_STATUS   tmpStatus = DSP_SOK ;

    TRC_1ENTER ("PMGR_MSGQ_Delete", msgqId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require (PMGR_MSGQ_IsOwner (PMGR_MSGQ_OwnerArray [msgqId])) ;

    if (PMGR_MSGQ_OwnerArray [msgqId] != NULL) {
        if (PMGR_MSGQ_IsOwner (PMGR_MSGQ_OwnerArray [msgqId])) {
            status = LDRV_MSGQ_Delete (msgqId) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
            if (PMGR_MSGQ_OwnerArray [msgqId] != NULL) {
                tmpStatus = PRCS_Delete (PMGR_MSGQ_OwnerArray [msgqId]) ;
                PMGR_MSGQ_OwnerArray [msgqId] = NULL ;
                if ((DSP_FAILED (tmpStatus)) && (DSP_SUCCEEDED (status))) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
            }
        }
        else {
            status = DSP_EACCESSDENIED ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        /* Message queue not created */
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID, on the specified processor.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Locate (IN     ProcessorId       procId,
                  IN     MsgQueueId        msgqId,
                  IN OPT MsgqLocateAttrs * attrs)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_3ENTER ("PMGR_MSGQ_Locate", procId, msgqId, attrs) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require (attrs != NULL) ;

    status = LDRV_MSGQ_Locate (procId, msgqId, attrs) ;
    if ((DSP_FAILED (status)) && (status != DSP_ENOTFOUND)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Locate", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier
 *          PMGR_MSGQ_Locate () call.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Release (IN  ProcessorId procId, IN  MsgQueueId msgqId)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_2ENTER ("PMGR_MSGQ_Release", procId, msgqId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    status = LDRV_MSGQ_Release (procId, msgqId) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Release", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Alloc
 *
 *  @desc   This function allocates a message, and returns the pointer to the
 *          user.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Alloc (IN  AllocatorId mqaId, IN  Uint16 size, OUT MsgqMsg  * msg)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_3ENTER ("PMGR_MSGQ_Alloc", mqaId, size, msg) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (msg != NULL) ;

    status = LDRV_MSGQ_Alloc (mqaId, size, msg) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Alloc", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Free
 *
 *  @desc   This function frees a message.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Free (IN  MsgqMsg   msg)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_1ENTER ("PMGR_MSGQ_Free", msg) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (msg != NULL) ;

    status = LDRV_MSGQ_Free (msg) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Free", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Put
 *
 *  @desc   This function sends a message to the specified MSGQ on the specified
 *          processor.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Put (IN     ProcessorId procId,
               IN     MsgQueueId  destMsgqId,
               IN     MsgqMsg     msg,
               IN OPT Uint16      msgId,
               IN OPT MsgQueueId  srcMsgqId)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_5ENTER ("PMGR_MSGQ_Put", procId, destMsgqId, msg, srcMsgqId, msgId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (IS_VALID_MSGQID (destMsgqId)) ;

    status = LDRV_MSGQ_Put (procId,
                            destMsgqId,
                            msg,
                            msgId,
                            srcMsgqId) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Put", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_Get
 *
 *  @desc   This function receives a message on the specified MSGQ.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Get (IN  MsgQueueId msgqId, IN  Uint32 timeout, OUT MsgqMsg * msg)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_3ENTER ("PMGR_MSGQ_Get", msgqId, timeout, msg) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require (PMGR_MSGQ_IsOwner (PMGR_MSGQ_OwnerArray [msgqId])) ;

    /* Ownership validation is not being done here for efficiency */
    status = LDRV_MSGQ_Get (msgqId, timeout, msg) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Get", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID and processor ID to be used for
 *          replying to a received message.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_GetReplyId (IN  MsgqMsg       msg,
                      OUT ProcessorId * procId,
                      OUT MsgQueueId *  msgqId)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_3ENTER ("PMGR_MSGQ_GetReplyId", msg, procId, msgqId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (procId != NULL) ;
    DBC_Require (msgqId != NULL) ;

    status = LDRV_MSGQ_GetReplyId (msg, procId, msgqId) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_GetReplyId", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_MSGQ_SetErrorHandler
 *
 *  @desc   This function allows the user to designate a MSGQ as an
 *          error-handler MSGQ to receive asynchronous error messages from the
 *          transports.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_SetErrorHandler (IN  MsgQueueId msgqId, OUT Uint16 mqaId)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_2ENTER ("PMGR_MSGQ_SetErrorHandler", msgqId, mqaId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (IS_VALID_MSGQID (msgqId) || (msgqId == MSGQ_INVALID_ID)) ;

    status = LDRV_MSGQ_SetErrorHandler (msgqId, mqaId) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_SetErrorHandler", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PMGR_MSGQ_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PMGR_MSGQ_Instrument (IN  ProcessorId      procId,
                      IN  MsgQueueId       msgqId,
                      OUT MsgqInstrument * retVal)
{
    DSP_STATUS  status = DSP_SOK ;


    TRC_3ENTER ("PMGR_MSGQ_Instrument", procId, msgqId, retVal) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (retVal != NULL) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    status = LDRV_MSGQ_Instrument (procId, msgqId, retVal) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_Instrument", status) ;

    return status ;
}
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PMGR_MSGQ_Debug
 *
 *  @desc   This function prints the current status of the specified message
 *          queue.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
PMGR_MSGQ_Debug (IN  ProcessorId    procId, IN  MsgQueueId msgqId)
{
    TRC_2ENTER ("PMGR_MSGQ_Debug", procId, msgqId) ;

    DBC_Require (PMGR_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    if (PMGR_MSGQ_IsInitialized == TRUE) {
        if (PMGR_MSGQ_OwnerArray [msgqId] != NULL) {
            TRC_2PRINT (TRC_LEVEL4, "MSGQ = [0x%x] owned by [0x%x] \n", msgqId,
                                    PMGR_MSGQ_OwnerArray [msgqId]) ;
        }
        else {
            TRC_1PRINT (TRC_LEVEL4, "MSGQ = [0x%x] not created \n", msgqId) ;
        }
    }
    else {
        TRC_0PRINT (TRC_LEVEL4, "MSGQ component not yet initialized \n") ;
    }

    LDRV_MSGQ_Debug (procId, msgqId) ;

    TRC_0LEAVE ("PMGR_MSGQ_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


/*  ----------------------------------------------------------------------------
 *  @func   PMGR_MSGQ_IsOwner
 *
 *  @desc   Checks if the calling client is the owner.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Bool
PMGR_MSGQ_IsOwner (PrcsObject * prcsInfo)
{
    DSP_STATUS   status      = DSP_SOK ;
    DSP_STATUS   tmpStatus   = DSP_SOK ;
    PrcsObject * curPrcsInfo = NULL    ;
    Bool         isOwner     = FALSE   ;

    TRC_1ENTER ("PMGR_MSGQ_IsOwner", prcsInfo) ;

    DBC_Require (prcsInfo != NULL) ;

    if (prcsInfo != NULL) {
        status = PRCS_Create (&curPrcsInfo) ;

        if (DSP_SUCCEEDED (status)) {
            status = PRCS_IsEqual (curPrcsInfo,
                                   prcsInfo,
                                   &isOwner) ;

            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }

            tmpStatus = PRCS_Delete (curPrcsInfo) ;
            curPrcsInfo = NULL ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_MSGQ_IsOwner", isOwner) ;

    return isOwner ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

