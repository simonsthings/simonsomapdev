/** ============================================================================
 *  @file   chnl.c
 *
 *  @path   $(DSPLINK)\gpp\src\api
 *
 *  @desc   This file implements the functions of CHNL API subcomponent.
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
#include <chnl.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   COMPONENT_ID
 *
 *  @desc   Component ID of this module. That will be used by TRC macros.
 *  ============================================================================
 */
#define COMPONENT_ID        ID_API_CHNL

/** ============================================================================
 *  @name   SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  \
            TRC_3PRINT (TRC_LEVEL7, \
                        "\nFailure: Status:[0x%x] File:[0x%x] Line:[%d]\n", \
                        status, FID_C_API_CHNL, __LINE__)

/** ============================================================================
 *  @macro  IS_VALID_CHNL_ATTRS
 *
 *  @desc   Check for validity of channel attributes.
 *  ============================================================================
 */
#define IS_VALID_CHNL_ATTRS(attrs)                                  \
             (   (attrs == NULL)                                    \
              || (   (   (attrs->endianism == Endianism_Default)    \
                      || (attrs->endianism == Endianism_Big)        \
                      || (attrs->endianism == Endianism_Little))    \
                  && (   (attrs->mode == ChannelMode_Input)         \
                      || (attrs->mode == ChannelMode_Output))       \
                  && (   (attrs->size == ChannelDataSize_16bits)    \
                      || (attrs->size == ChannelDataSize_32bits))))


/** ============================================================================
 *  @name   DRV_Handle
 *
 *  @desc   Handle to global driver object.
 *  ============================================================================
 */
EXTERN DRV_Object  * DRV_Handle ;


/** ============================================================================
 *  @func   CHNL_Create
 *
 *  @desc   Creates resources used for transferring data between GPP and DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Create (IN ProcessorId     procId,
             IN ChannelId       chnlId,
             IN ChannelAttrs *  attrs)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_3ENTER ("CHNL_Create", procId, chnlId, attrs) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (IS_VALID_CHNL_ATTRS (attrs)) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))
        || (!IS_VALID_CHNL_ATTRS (attrs))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlCreateArgs.procId = procId ;
        args.apiArgs.chnlCreateArgs.chnlId = chnlId ;
        args.apiArgs.chnlCreateArgs.attrs  = attrs ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_CREATE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_Delete
 *
 *  @desc   Releases channel resources used for transferring data between GPP
 *          and DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Delete (IN ProcessorId  procId,
             IN ChannelId    chnlId)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_2ENTER ("CHNL_Delete", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlDeleteArgs.procId = procId ;
        args.apiArgs.chnlDeleteArgs.chnlId = chnlId ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_DELETE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_AllocateBuffer
 *
 *  @desc   Allocates an array of buffers of specified size and returns them
 *          to the client.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_AllocateBuffer (IN  ProcessorId procId,
                     IN  ChannelId   chnlId,
                     OUT Char8 **    bufArray,
                     IN  Uint32      size,
                     IN  Uint32      numBufs)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_5ENTER ("CHNL_AllocateBuffer",
                procId,
                chnlId,
                size,
                bufArray,
                numBufs) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (bufArray != NULL) ;
    DBC_Require (numBufs <= MAX_ALLOC_BUFFERS) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))
        || (bufArray == NULL)
        || (numBufs > MAX_ALLOC_BUFFERS)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlAllocateBufferArgs.procId   = procId   ;
        args.apiArgs.chnlAllocateBufferArgs.chnlId   = chnlId   ;
        args.apiArgs.chnlAllocateBufferArgs.bufArray = bufArray ;
        args.apiArgs.chnlAllocateBufferArgs.size     = size     ;
        args.apiArgs.chnlAllocateBufferArgs.numBufs  = numBufs  ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_ALLOCATEBUFFER, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_AllocateBuffer", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_FreeBuffer
 *
 *  @desc   Frees buffer(s) allocated by CHNL_AllocateBuffer.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_FreeBuffer (IN ProcessorId procId,
                 IN ChannelId   chnlId,
                 IN Char8 **    bufArray,
                 IN Uint32      numBufs)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_4ENTER ("CHNL_FreeBuffer", procId, chnlId, bufArray, numBufs) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (bufArray != NULL) ;
    DBC_Require (numBufs <= MAX_ALLOC_BUFFERS) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))
        || (bufArray == NULL)
        || (numBufs > MAX_ALLOC_BUFFERS)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlFreeBufferArgs.procId   = procId   ;
        args.apiArgs.chnlFreeBufferArgs.chnlId   = chnlId   ;
        args.apiArgs.chnlFreeBufferArgs.bufArray = bufArray ;
        args.apiArgs.chnlFreeBufferArgs.numBufs  = numBufs  ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_FREEBUFFER, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_FreeBuffer", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_Issue
 *
 *  @desc   Issues an input or output request on a specified channel.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Issue (IN ProcessorId     procId,
            IN ChannelId       chnlId,
            IN ChannelIOInfo * ioReq)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_3ENTER ("CHNL_Issue", procId, chnlId, ioReq) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (ioReq != NULL) ;
    DBC_Require ((ioReq != NULL) && (ioReq->buffer != NULL)) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))
        || (ioReq == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (ioReq->buffer == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlIssueArgs.procId = procId ;
        args.apiArgs.chnlIssueArgs.chnlId = chnlId ;
        args.apiArgs.chnlIssueArgs.ioReq  = ioReq  ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_ISSUE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Issue", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_Reclaim
 *
 *  @desc   Gets the buffer back that has been issued to this channel.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Reclaim (IN     ProcessorId      procId,
              IN     ChannelId        chnlId,
              IN     Uint32           timeout,
              IN OUT ChannelIOInfo *  ioReq)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_4ENTER ("CHNL_Reclaim", procId, chnlId, timeout, ioReq) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (ioReq != NULL) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))
        || (ioReq == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlReclaimArgs.procId = procId   ;
        args.apiArgs.chnlReclaimArgs.chnlId = chnlId   ;
        args.apiArgs.chnlReclaimArgs.timeout = timeout ;
        args.apiArgs.chnlReclaimArgs.ioReq  = ioReq    ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_RECLAIM, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Reclaim", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_Idle
 *
 *  @desc   If the channel is an input stream this function resets the channel
 *          and causes any currently buffered input data to be discarded.  If
 *          the channel is an output channel, this function causes any
 *          currently queued buffers to be transferred through the channel. It
 *          causes the client to wait for as long as it takes for the data to
 *          be transferred through the channel.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Idle (IN ProcessorId          procId,
           IN ChannelId            chnlId)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_2ENTER ("CHNL_Idle", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlIdleArgs.procId = procId   ;
        args.apiArgs.chnlIdleArgs.chnlId = chnlId   ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_IDLE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Idle", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_Flush
 *
 *  @desc   Discards all the requested buffers that are pending for transfer
 *          both in case of input mode channel as well as output mode channel.
 *          One must still have to call the CHNL_Reclaim to get back the
 *          discarded buffers.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Flush (IN ProcessorId procId,
            IN ChannelId   chnlId)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_2ENTER ("CHNL_Flush", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlFlushArgs.procId = procId   ;
        args.apiArgs.chnlFlushArgs.chnlId = chnlId   ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_FLUSH, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Flush", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CHNL_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations
 *          on channels.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Control (IN  ProcessorId  procId,
              IN  ChannelId    chnlId,
              IN  Int32        cmd,
              OPT Pvoid        arg)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_4ENTER ("CHNL_Control", procId, chnlId, cmd, arg) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlControlArgs.procId = procId ;
        args.apiArgs.chnlControlArgs.chnlId = chnlId ;
        args.apiArgs.chnlControlArgs.cmd    = cmd    ;
        args.apiArgs.chnlControlArgs.arg    = arg    ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_CONTROL, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Control", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   CHNL_Instrument
 *
 *  @desc   Gets the instrumentation information related to PROC's
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Instrument (IN  ProcessorId      procId,
                 IN  ChannelId        chnlId,
                 OUT ChnlInstrument * retVal)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_3ENTER ("CHNL_Instrument", procId, chnlId, retVal) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (retVal != NULL) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))
        || (retVal == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlInstrumentArgs.procId    = procId ;
        args.apiArgs.chnlInstrumentArgs.chnlId    = chnlId ;
        args.apiArgs.chnlInstrumentArgs.chnlStats = retVal ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_INSTRUMENT, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("CHNL_Instrument", status) ;

    return status ;
}
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   CHNL_Debug
 *
 *  @desc   Prints the current status of CHNL component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
CHNL_Debug (IN ProcessorId procId,
            IN ChannelId   chnlId)
{
    DSP_STATUS  status = DSP_SOK ;
    CMD_Args    args             ;

    TRC_0ENTER ("CHNL_Debug") ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (   (!IS_VALID_PROCID (procId))
        || (!IS_VALID_CHNLID (procId, chnlId))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.chnlDebugArgs.procId = procId ;
        args.apiArgs.chnlDebugArgs.chnlId = chnlId ;

        status = DRV_INVOKE (DRV_Handle, CMD_CHNL_DEBUG, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_0LEAVE ("CHNL_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
