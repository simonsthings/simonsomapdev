/** ============================================================================
 *  @file   msgq.c
 *
 *  @path   $(DSPLINK)\gpp\src\api
 *
 *  @desc   This file implements the functions of MSGQ API subcomponent.
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
#include <dbc.h>
#include <signature.h>
#include <trc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- OSAL Header                   */
#include <drv_pmgr.h>
#include <drv_api.h>
#include <print.h>

/*  ----------------------------------- User API                      */
#include <msgq.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   COMPONENT_ID
 *
 *  @desc   Component ID of this module. That will be used by TRC macros.
 *  ============================================================================
 */
#define COMPONENT_ID        ID_API_MSGQ

/** ============================================================================
 *  @name   SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  \
            TRC_3PRINT (TRC_LEVEL7, \
                        "\nFailure: Status:[0x%x] File:[0x%x] Line:[%d]\n", \
                        status, FID_C_API_MSGQ, __LINE__)

/** ============================================================================
 *  @name   DRV_Handle
 *
 *  @desc   Handle to global driver object.
 *  ============================================================================
 */
EXTERN DRV_Object * DRV_Handle ;


/** ============================================================================
 *  @func   MSGQ_AllocatorOpen
 *
 *  @desc   This function initializes an allocator.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_AllocatorOpen (IN  AllocatorId  mqaId, IN  Pvoid mqaAttrs)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_2ENTER ("MSGQ_AllocatorOpen", mqaId, mqaAttrs) ;

    DBC_Require (mqaAttrs != NULL) ;
    DBC_Require (mqaId < MAX_ALLOCATORS) ;

    if ((mqaAttrs == NULL) || (mqaId >= MAX_ALLOCATORS)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqAllocatorOpenArgs.mqaId = mqaId ;
        args.apiArgs.msgqAllocatorOpenArgs.mqaAttrs = mqaAttrs ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_ALLOCATOROPEN, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_AllocatorOpen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_AllocatorClose
 *
 *  @desc   This function finalizes the allocator component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_AllocatorClose (IN  AllocatorId mqaId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_1ENTER ("MSGQ_AllocatorClose", mqaId) ;

    DBC_Require (mqaId < MAX_ALLOCATORS) ;

    args.apiArgs.msgqAllocatorCloseArgs.mqaId = mqaId ;

    if (mqaId >= MAX_ALLOCATORS) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_ALLOCATORCLOSE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_AllocatorClose", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_TransportOpen
 *
 *  @desc   This function initializes an MQT component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_TransportOpen (IN  TransportId mqtId, IN  Pvoid mqtAttrs)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_2ENTER ("MSGQ_TransportOpen", mqtId, mqtAttrs) ;

    DBC_Require (mqtAttrs != NULL) ;

    if (mqtAttrs == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqTransportOpenArgs.mqtId = mqtId ;
        args.apiArgs.msgqTransportOpenArgs.mqtAttrs = mqtAttrs ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_TRANSPORTOPEN, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_TransportOpen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_TransportClose
 *
 *  @desc   This function finalizes the MQT component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_TransportClose (IN  TransportId mqtId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_1ENTER ("MSGQ_TransportClose", mqtId) ;

    args.apiArgs.msgqTransportCloseArgs.mqtId = mqtId ;

    status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_TRANSPORTCLOSE, &args) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("MSGQ_TransportClose", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Create
 *
 *  @desc   This function creates the message queue to be used for receiving
 *          messages, identified through the specified MSGQ ID.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Create (IN  MsgQueueId msgqId, IN OPT MsgqAttrs * msgqAttrs)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_2ENTER ("MSGQ_Create", msgqId, msgqAttrs) ;

    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    if (IS_VALID_MSGQID (msgqId)) {
        args.apiArgs.msgqCreateArgs.msgqId = msgqId ;
        args.apiArgs.msgqCreateArgs.msgqAttrs = msgqAttrs ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_CREATE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("MSGQ_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Delete
 *
 *  @desc   This function deletes the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Delete (IN  MsgQueueId msgqId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_1ENTER ("MSGQ_Delete", msgqId) ;

    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    if (IS_VALID_MSGQID (msgqId)) {
        args.apiArgs.msgqDeleteArgs.msgqId = msgqId ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_DELETE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("MSGQ_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID, on the specified processor.
 *          The MSGQ ID is translated to a unique name on the other processor.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Locate (IN  ProcessorId       procId,
             IN  MsgQueueId        msgqId,
             IN  MsgqLocateAttrs * attrs)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_3ENTER ("MSGQ_Locate", procId, msgqId, attrs) ;

    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require (attrs != NULL) ;

    if (   (   (!IS_VALID_PROCID (procId))
            && (!IS_LOCAL_PROCID (procId)))
        || (!IS_VALID_MSGQID (msgqId))
        || (attrs == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqLocateArgs.procId = procId ;
        args.apiArgs.msgqLocateArgs.msgqId = msgqId ;
        args.apiArgs.msgqLocateArgs.attrs  = attrs  ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_LOCATE, &args) ;
        if ((DSP_FAILED (status)) && (status != DSP_ENOTFOUND)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Locate", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier
 *          MSGQ_Locate () call.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Release (IN  ProcessorId procId, IN  MsgQueueId msgqId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_2ENTER ("MSGQ_Release", procId, msgqId) ;

    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    if (   (   (!IS_VALID_PROCID (procId))
            && (!IS_LOCAL_PROCID (procId)))
        || (!IS_VALID_MSGQID (msgqId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqReleaseArgs.procId = procId ;
        args.apiArgs.msgqReleaseArgs.msgqId = msgqId ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_RELEASE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Release", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Alloc
 *
 *  @desc   This function allocates a message, and returns the pointer to the
 *          user.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Alloc (IN  AllocatorId mqaId, IN  Uint16 size, OUT MsgqMsg * msg)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_3ENTER ("MSGQ_Alloc", mqaId, size, msg) ;

    DBC_Require (mqaId <  MAX_ALLOCATORS) ;
    DBC_Require (msg   != NULL) ;
    DBC_Require (size  >= sizeof (MsgqMsgHeader)) ;

    if (    (msg == NULL)
        ||  (size < sizeof (MsgqMsgHeader))
        ||  (mqaId >= MAX_ALLOCATORS)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqAllocArgs.mqaId = mqaId ;
        args.apiArgs.msgqAllocArgs.size  = size ;
        args.apiArgs.msgqAllocArgs.msg   = msg ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_ALLOC, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Alloc", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Free
 *
 *  @desc   This function frees a message.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Free (IN  MsgqMsg msg)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_1ENTER ("MSGQ_Free", msg) ;

    DBC_Require (msg != NULL) ;
    DBC_Require ((msg != NULL) && (msg->mqaId < MAX_ALLOCATORS)) ;

    if ((msg == NULL) || (msg->mqaId >= MAX_ALLOCATORS)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqFreeArgs.msg = msg ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_FREE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Free", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Put
 *
 *  @desc   This function sends a message to the specified MSGQ on the specified
 *          processor.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Put (IN     ProcessorId procId,
          IN     MsgQueueId  destMsgqId,
          IN     MsgqMsg     msg,
          IN OPT Uint16      msgId,
          IN OPT MsgQueueId  srcMsgqId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_5ENTER ("MSGQ_Put", procId, destMsgqId, msg, msgId, srcMsgqId) ;

    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (IS_VALID_MSGQID (destMsgqId)) ;

    if (   (   (!IS_VALID_PROCID (procId))
            && (!IS_LOCAL_PROCID (procId)))
        || (msg == NULL)
        || (!IS_VALID_MSGQID (destMsgqId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqPutArgs.procId     = procId ;
        args.apiArgs.msgqPutArgs.destMsgqId = destMsgqId ;
        args.apiArgs.msgqPutArgs.msg        = msg ;
        args.apiArgs.msgqPutArgs.srcMsgqId  = srcMsgqId ;
        args.apiArgs.msgqPutArgs.msgId      = msgId ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_PUT, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Put", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_Get
 *
 *  @desc   This function receives a message on the specified MSGQ.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Get (IN  MsgQueueId msgqId, IN  Uint32 timeout, OUT MsgqMsg * msg)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_3ENTER ("MSGQ_Get", msgqId, timeout, msg) ;

    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require (msg != NULL) ;

    if ((!IS_VALID_MSGQID (msgqId)) || (msg == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqGetArgs.msgqId  = msgqId ;
        args.apiArgs.msgqGetArgs.timeout = timeout ;
        args.apiArgs.msgqGetArgs.msg     = msg ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_GET, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Get", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID and processor ID to be used for
 *          replying to a received message.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_GetReplyId (IN  MsgqMsg       msg,
                 OUT ProcessorId * procId,
                 OUT MsgQueueId *  msgqId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_3ENTER ("MSGQ_GetReplyId", msg, procId, msgqId) ;

    DBC_Require (msg != NULL) ;
    DBC_Require (procId != NULL) ;
    DBC_Require (msgqId != NULL) ;

    if ((msg == NULL) || (procId == NULL) || (msgqId == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqGetReplyIdArgs.msg    = msg ;
        args.apiArgs.msgqGetReplyIdArgs.procId = procId ;
        args.apiArgs.msgqGetReplyIdArgs.msgqId = msgqId ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_GETREPLYID, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_GetReplyId", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MSGQ_SetErrorHandler
 *
 *  @desc   This API allows the user to designate a MSGQ as an error-handler
 *          MSGQ to receive asynchronous error messages from the transports.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_SetErrorHandler (IN  MsgQueueId msgqId, IN  Uint16 mqaId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_2ENTER ("MSGQ_SetErrorHandler", msgqId, mqaId) ;

    DBC_Require (IS_VALID_MSGQID (msgqId) || (msgqId == MSGQ_INVALID_ID)) ;

    if (!IS_VALID_MSGQID (msgqId) && (msgqId != MSGQ_INVALID_ID)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqSetErrorHandlerArgs.msgqId = msgqId ;
        args.apiArgs.msgqSetErrorHandlerArgs.mqaId  = mqaId ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_SETERRORHANDLER, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_SetErrorHandler", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   MSGQ_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MSGQ_Instrument (IN  ProcessorId      procId,
                 IN  MsgQueueId       msgqId,
                 OUT MsgqInstrument * retVal)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_3ENTER ("MSGQ_Instrument", procId, msgqId, retVal) ;

    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require (retVal != NULL) ;

    if (   (   (!IS_VALID_PROCID (procId))
            && (!IS_LOCAL_PROCID (procId)))
        || (retVal == NULL)
        || (!IS_VALID_MSGQID (msgqId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqInstrumentArgs.procId = procId ;
        args.apiArgs.msgqInstrumentArgs.msgqId = msgqId ;
        args.apiArgs.msgqInstrumentArgs.retVal = retVal ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_INSTRUMENT, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Instrument", status) ;

    return status ;
}
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   MSGQ_Debug
 *
 *  @desc   This function prints the current status of the MSGQ subcomponent.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
MSGQ_Debug (IN  ProcessorId procId, IN  MsgQueueId msgqId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_2ENTER ("MSGQ_Debug", procId, msgqId) ;

    DBC_Require (   (IS_VALID_PROCID (procId))
                 || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    if (   (   (!IS_VALID_PROCID (procId))
            && (!IS_LOCAL_PROCID (procId)))
        || (!IS_VALID_MSGQID (msgqId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.msgqDebugArgs.procId = procId ;
        args.apiArgs.msgqDebugArgs.msgqId = msgqId ;

        status = DRV_INVOKE (DRV_Handle, CMD_MSGQ_DEBUG, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MSGQ_Debug", status) ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
