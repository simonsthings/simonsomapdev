/** ============================================================================
 *  @file   pmgr_chnl.c
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr
 *
 *  @desc   This file implements the functions of PMGR_CHNL subcomponent.
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

/*  ----------------------------------- Generic Function              */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Processor Manager             */
#include <pmgr_proc.h>
#include <pmgr_chnl.h>

/*  ----------------------------------- Link Driver                   */
#include <ldrv_chnl.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   COMPONENT_ID
 *
 *  @desc   Component ID of this module. That will be used by TRC macros.
 *  ============================================================================
 */
#define COMPONENT_ID        ID_PMGR_CHNL

/** ============================================================================
 *  @name   SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason (defined in GEN).
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_PMGR_CHNL, __LINE__)

/** ============================================================================
 *  @name   PMGR_CHNL_OwnerArray
 *
 *  @desc   Array of channel owners.
 *  ============================================================================
 */
STATIC PrcsObject * PMGR_CHNL_OwnerArray [MAX_PROCESSORS][MAX_CHANNELS] ;

/** ============================================================================
 *  @name   PMGR_CHNL_IsInitialized
 *
 *  @desc   Flag that checks if channels for a processor is initialized or not.
 *  ============================================================================
 */
STATIC Bool PMGR_CHNL_IsInitialized [MAX_PROCESSORS] ;


/** ----------------------------------------------------------------------------
 *  @func   PMGR_CHNL_IsOwner
 *
 *  @desc   Checks that the calling client is the owner of the specified
 *          channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Id to open.
 *
 *  @ret    TRUE
 *              If the calling client has opened the channel.
 *          FALSE
 *              Otherwise.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Bool
PMGR_CHNL_IsOwner (IN  ProcessorId procId,
                   IN  ChannelId   chnlId) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Initialize
 *
 *  @desc   Sets up all channel objects in Link Driver.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Initialize (IN ProcessorId procId)
{
    DSP_STATUS   status = DSP_SOK ;
    Uint32       i                ;

    TRC_1ENTER ("PMGR_CHNL_Initialize", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    status = LDRV_CHNL_Initialize (procId) ;

    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; i < MAX_CHANNELS ; i++) {
            PMGR_CHNL_OwnerArray [procId][i] = NULL ;
        }
        PMGR_CHNL_IsInitialized [procId] = TRUE ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Finalize
 *
 *  @desc   Releases all channel objects setup in Link Driver.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Finalize (IN ProcessorId   procId)
{
    DSP_STATUS status = DSP_SOK ;
    Uint32     i      = 0       ;

    TRC_1ENTER ("PMGR_CHNL_Finalize", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;

    if (PMGR_CHNL_IsInitialized [procId]) {
        status = LDRV_CHNL_Finalize (procId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }

        for (i = 0 ; i < MAX_CHANNELS ; i++) {
            PMGR_CHNL_OwnerArray [procId][i] = NULL ;
        }

        PMGR_CHNL_IsInitialized [procId] = FALSE ;
    }
    else {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Create
 *
 *  @desc   Creates resources used for transferring data between GPP and DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Create (IN     ProcessorId     procId,
                  IN     ChannelId       chnlId,
                  OPT IN ChannelAttrs *  attrs)
{
    DSP_STATUS   status     = DSP_SOK ;
    Bool         isAttached = FALSE   ;
    PrcsObject * prcsInfo   = NULL    ;
    ChannelAttrs ldrvAttrs            ;

    TRC_3ENTER ("PMGR_CHNL_Create", procId, chnlId, attrs) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (attrs != NULL) ;

    if (PMGR_CHNL_OwnerArray [procId][chnlId] != NULL) {
        status = DSP_EALREADYCONNECTED ;
        SET_FAILURE_REASON ;
    }
    else {
        status = PRCS_Create (&prcsInfo) ;

        if (DSP_SUCCEEDED (status)) {
            status = PMGR_PROC_IsAttached (procId, prcsInfo, &isAttached) ;
            if (DSP_SUCCEEDED (status)) {
                if (isAttached == TRUE) {
                    PMGR_CHNL_OwnerArray [procId][chnlId] = prcsInfo ;
                    ldrvAttrs.endianism = attrs->endianism ;
                    ldrvAttrs.mode      = attrs->mode      ;
                    ldrvAttrs.size      = attrs->size      ;
                    status = LDRV_CHNL_Open (procId, chnlId, &ldrvAttrs) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    status = DSP_EATTACHED ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }

            if (DSP_FAILED (status)) {
                PMGR_CHNL_OwnerArray [procId][chnlId] = NULL ;
                PRCS_Delete (prcsInfo) ;
                prcsInfo = NULL ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_CHNL_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Delete
 *
 *  @desc   Releases channel resources used for transferring data between GPP
 *          and DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Delete (IN ProcessorId  procId,
                  IN ChannelId    chnlId)
{
    DSP_STATUS status  = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;

    TRC_2ENTER ("PMGR_CHNL_Delete", procId, chnlId) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (PMGR_CHNL_IsOwner (procId, chnlId)) {
        status = LDRV_CHNL_Close (procId, chnlId) ;
        tmpStatus = PRCS_Delete (PMGR_CHNL_OwnerArray [procId][chnlId]) ;
        PMGR_CHNL_OwnerArray [procId][chnlId] = NULL ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_AllocateBuffer
 *
 *  @desc   Allocates an array of buffers of specified size and returns them
 *          to the client.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_AllocateBuffer (IN  ProcessorId procId,
                          IN  ChannelId   chnlId,
                          OUT Char8 **    bufArray,
                          IN  Uint32      size,
                          IN  Uint32      numBufs)
{
    DSP_STATUS status  = DSP_SOK ;
    Uint32     i                 ;
    Uint32     j                 ;

    TRC_5ENTER ("PMGR_CHNL_AllocateBuffer", procId, chnlId,
                                            bufArray, size, numBufs) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (bufArray != NULL) ;
    DBC_Require (numBufs <= MAX_ALLOC_BUFFERS) ;

    if (PMGR_CHNL_IsOwner (procId, chnlId)) {
        for (i = 0 ; (i < numBufs) && (DSP_SUCCEEDED (status)) ; i++) {
            status = MEM_Alloc ((Void **)(bufArray + i), size, MEM_DEFAULT) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }

        /* In case of failure during allocation, deallocate the buffers that
         * have already been allocated
         */
        if (DSP_FAILED (status)) {
            for (j = 0 ; j < (i - 1) ; j++) {
                FREE_PTR (*(bufArray + j)) ;
            }
        }
    }
    else {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_AllocateBuffer", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_FreeBuffer
 *
 *  @desc   Frees buffer(s) allocated by PMGR_CHNL_AllocateBuffer.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_FreeBuffer (IN ProcessorId procId,
                      IN ChannelId   chnlId,
                      IN Char8 **    bufArray,
                      IN Uint32      numBufs)
{
    DSP_STATUS status  = DSP_SOK ;
    Uint32     i                 ;

    TRC_4ENTER ("PMGR_CHNL_FreeBuffer", procId, chnlId, bufArray, numBufs) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (bufArray != NULL) ;
    DBC_Require (numBufs <= MAX_ALLOC_BUFFERS) ;

    if (PMGR_CHNL_IsOwner (procId, chnlId)) {
        for (i = 0 ; i < numBufs ; i++) {
            status = FREE_PTR (*(bufArray + i)) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_FreeBuffer", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Issue
 *
 *  @desc   Issues an input or output request on a specified channel.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Issue (IN ProcessorId     procId,
                 IN ChannelId       chnlId,
                 IN ChannelIOInfo * ioReq)
{
    DSP_STATUS     status  = DSP_SOK ;
    LDRVChnlIOInfo ioInfo            ;

    TRC_3ENTER ("PMGR_CHNL_Issue", procId, chnlId, ioReq) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (ioReq != NULL) ;
    DBC_Require (PMGR_CHNL_IsOwner (procId, chnlId)) ;

    ioInfo.buffer      = ioReq->buffer ;
    ioInfo.size        = ioReq->size   ;
    ioInfo.arg         = ioReq->arg    ;
    ioInfo.callback    = NULL          ;
    ioInfo.dpcContext  = FALSE         ;

    status =  LDRV_CHNL_AddIORequest (procId, chnlId, &ioInfo) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Issue", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Reclaim
 *
 *  @desc   Gets the buffer back that has been issued to this channel.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Reclaim (IN     ProcessorId      procId,
                   IN     ChannelId        chnlId,
                   IN     Uint32           timeout,
                   IN OUT ChannelIOInfo *  ioReq)
{
    DSP_STATUS     status  = DSP_SOK ;
    LDRVChnlIOInfo ioInfo            ;

    TRC_4ENTER ("PMGR_CHNL_Reclaim", procId, chnlId, timeout, ioReq) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (ioReq != NULL) ;
    DBC_Require (PMGR_CHNL_IsOwner (procId, chnlId)) ;

    /*  Set the dpcContext field in the ioInfo structure to FALSE to indicate
     *  that the call is not being made from a DPC context.
     */
    ioInfo.dpcContext = FALSE ;

    status = LDRV_CHNL_GetIOCompletion (procId, chnlId,
                                        timeout, &ioInfo) ;

    if (DSP_SUCCEEDED (status)) {
        ioReq->buffer = ioInfo.buffer ;
        ioReq->size   = ioInfo.size   ;
        ioReq->arg    = ioInfo.arg    ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Reclaim", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Idle
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
NORMAL_API
DSP_STATUS
PMGR_CHNL_Idle (IN ProcessorId          procId,
                IN ChannelId            chnlId)
{
    DSP_STATUS status  = DSP_SOK ;

    TRC_2ENTER ("PMGR_CHNL_Idle", procId, chnlId) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (PMGR_CHNL_IsOwner (procId, chnlId)) {
        status = LDRV_CHNL_Idle (procId, chnlId, TRUE) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Idle", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Flush
 *
 *  @desc   Discards all the requested buffers that are pending for transfer
 *          both in case of input mode channel as well as output mode channel.
 *          One must still have to call the PMGR_CHNL_Reclaim to get back the
 *          discarded buffers.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Flush (IN ProcessorId          procId,
                 IN ChannelId            chnlId)
{
    DSP_STATUS status  = DSP_SOK ;

    TRC_2ENTER ("PMGR_CHNL_Flush", procId, chnlId) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (PMGR_CHNL_IsOwner (procId, chnlId)) {
        status = LDRV_CHNL_Idle (procId, chnlId, FALSE) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Flush", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_CHNL_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations
 *          on channels.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Control (IN  ProcessorId  procId,
                   IN  ChannelId    chnlId,
                   IN  Int32        cmd,
                   OPT Pvoid        arg)
{
    DSP_STATUS   status   = DSP_SOK ;

    TRC_4ENTER ("PMGR_CHNL_Control", procId, chnlId, cmd, arg) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    status = DSP_ENOTIMPL ;

    TRC_1LEAVE ("PMGR_CHNL_Control", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PMGR_CHNL_Instrument
 *
 *  @desc   Gets the instrumentation information related to the specified
 *          channel's
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Instrument (IN  ProcessorId       procId,
                      IN  ChannelId         chnlId,
                      OUT ChnlInstrument *  retVal)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("PMGR_CHNL_Instrument", procId, chnlId, retVal) ;

    DBC_Require (PMGR_CHNL_IsInitialized [procId]) ;
    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;
    DBC_Require (retVal != NULL) ;

    status = LDRV_CHNL_Instrument (procId, chnlId, retVal) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_CHNL_Instrument", status) ;

    return status ;
}
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PMGR_CHNL_Debug
 *
 *  @desc   Prints the current status of CHNL sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
PMGR_CHNL_Debug (ProcessorId procId, ChannelId chnlId)
{
    TRC_0ENTER ("PMGR_CHNL_Debug") ;

    LDRV_CHNL_Debug (procId, chnlId) ;

    TRC_0LEAVE ("PMGR_CHNL_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


/*  ----------------------------------------------------------------------------
 *  @func   PMGR_CHNL_IsOwner
 *
 *  @desc   Checks that the calling client is the owner of the specified
 *          channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Bool
PMGR_CHNL_IsOwner (IN  ProcessorId procId,
                   IN  ChannelId   chnlId)
{
    DSP_STATUS   status    = DSP_SOK ;
    PrcsObject * prcsInfo  = NULL    ;
    Bool         isOwner   = FALSE   ;

    TRC_2ENTER ("PMGR_CHNL_IsOwner", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (IS_VALID_CHNLID (procId, chnlId)) ;

    if (PMGR_CHNL_OwnerArray [procId][chnlId] != NULL) {
        status = PRCS_Create (&prcsInfo) ;

        if (DSP_SUCCEEDED (status)) {
            status = PRCS_IsEqual (prcsInfo,
                                   PMGR_CHNL_OwnerArray [procId][chnlId],
                                   &isOwner) ;

            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }

            status   = PRCS_Delete (prcsInfo) ;
            prcsInfo = NULL ;
            if (DSP_FAILED (status)) {
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

    TRC_1LEAVE ("PMGR_CHNL_IsOwner", isOwner) ;

    return isOwner ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

