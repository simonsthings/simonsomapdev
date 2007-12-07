/** ============================================================================
 *  @file   ldrv_chnl.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Link driver's channel module implementation.
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

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug                 */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- OSAL Header                   */
#include <mem.h>
#include <cfg.h>
#include <sync.h>
#include <print.h>

/*  ----------------------------------- Generic Function              */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Link Driver                   */
#include <ldrv.h>
#include <ldrv_chnl.h>
#include <ldrv_io.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component ID of this module.
 *  ============================================================================
 */
#define COMPONENT_ID        ID_LDRV_CHNL

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_CHNL, __LINE__)

/** ============================================================================
 *  @macro  IS_CHNL_READY
 *
 *  @desc   Is the channel ready.
 *  ============================================================================
 */
#define IS_CHNL_READY(chnlObj)         \
       ((chnlObj->chnlState & ChannelState_Ready) == ChannelState_Ready)

/** ============================================================================
 *  @macro  IS_CHNL_EOS
 *
 *  @desc   Has the channel reached EOS.
 *  ============================================================================
 */
#define IS_CHNL_EOS(chnlObj)         \
       ((chnlObj->chnlState & ChannelState_EOS) == ChannelState_EOS)

/** ============================================================================
 *  @macro  IS_CHNL_IDLE
 *
 *  @desc   Is channel in idle state.
 *  ============================================================================
 */
#define IS_CHNL_IDLE(chnlObj)         \
       ((chnlObj->chnlState & ChannelState_Idled) == ChannelState_Idled)

/** ============================================================================
 *  @macro  IS_INPUT_CHNL
 *
 *  @desc   Is this channel a input channel.
 *  ============================================================================
 */
#define IS_INPUT_CHNL(chnlObj) ((chnlObj->attrs).mode == ChannelMode_Input)

/** ============================================================================
 *  @macro  IS_OUTPUT_CHNL
 *
 *  @desc   Is this channel a output channel.
 *  ============================================================================
 */
#define IS_OUTPUT_CHNL(chnlObj) ((chnlObj->attrs).mode == ChannelMode_Output)


/** ============================================================================
 *  @macro  LDRV_Obj
 *
 *  @desc   Global LDRV object.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;

/** ----------------------------------------------------------------------------
 *  @name   LDRV_CHNL_Object
 *
 *  @desc   Contains information related to channels.
 *  ----------------------------------------------------------------------------
 */
STATIC LDRVChnlObject *
        LDRV_CHNL_Object [MAX_PROCESSORS][MAX_CHANNELS + NUM_MSGQ_CHANNELS] ;


/** ----------------------------------------------------------------------------
 *  @func   CreateChirpList
 *
 *  @desc   Allocates a chirp list.
 *
 *  @arg    numIRPs
 *              Number of chirps to be allocated in the list.
 *
 *  @ret    NULL
 *              Error allocating list.
 *          non-NULL
 *              Pointer to allocated list.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
List *
CreateChirpList (IN Uint32 numIRPs) ;


/** ----------------------------------------------------------------------------
 *  @func   FreeChirpList
 *
 *  @desc   Deallocates a chirp list.
 *
 *  @arg    chirpList
 *              List to be freed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Error occurred while freeing the memory.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
FreeChirpList (IN List * chirpList) ;


/** ----------------------------------------------------------------------------
 *  @func   AllocateChirp
 *
 *  @desc   Allocates a single chirp.
 *
 *  @arg    None
 *
 *  @ret    NULL
 *              Error allocating chirp.
 *          Non-NULL
 *              Pointer to allocated chirp.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
LDRVChnlIRP *
AllocateChirp () ;


#if defined (DDSP_DEBUG)
/** ----------------------------------------------------------------------------
 *  @func   PrintChirp
 *
 *  @desc   Prints a single CHIRP (Channel IO Request Packet).
 *
 *  @arg    chirp
 *              CHIRP to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
PrintChirp (IN LDRVChnlIRP * chirp) ;


/** ----------------------------------------------------------------------------
 *  @func   PrintChirpList
 *
 *  @desc   Prints a CHIRP (Channel IO Request Packet) List.
 *
 *  @arg    chirpList
 *              CHIRP List to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
PrintChirpList (IN List * chirpList) ;
#endif /* defined (DDSP_DEBUG) */


/** ============================================================================
 *  @func   LDRV_CHNL_Initialize
 *
 *  @desc   This function sets up the underlying communication mechanism by
 *          calling LDRV_IO_Initialize (). It also initializes the internal
 *          data-structures of the LDRV CHNL subcomponent.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Initialize (IN ProcessorId  procId)
{
    DSP_STATUS   status = DSP_SOK ;
    Uint32       i                ;

    TRC_1ENTER ("LDRV_CHNL_Initialize", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    for (i = 0 ; i < MAX_CHANNELS ; i++) {
        LDRV_CHNL_Object [procId][i] = NULL ;
    }

    status = LDRV_IO_Initialize (procId) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CHNL_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_Finalize
 *
 *  @desc   This function closes all open channels if any. It then calls
 *          LDRV_IO_Finalize () function to finalize the underlying
 *          communication mechanism.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Finalize (IN ProcessorId procId)
{
    DSP_STATUS   status    = DSP_SOK ;
    DSP_STATUS   tmpStatus = DSP_SOK ;
    Uint32       i                   ;

    TRC_1ENTER ("LDRV_CHNL_Finalize", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    /* Close the channels if not already closed */
    for (i = 0 ; i < MAX_CHANNELS ; i++) {
        if (LDRV_CHNL_Object [procId][i] != NULL) {
            tmpStatus = LDRV_CHNL_Close (procId, i) ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                status = tmpStatus ;
            }
        }
    }

    tmpStatus = LDRV_IO_Finalize (procId) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CHNL_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_Open
 *
 *  @desc   Opens and prepares a channel for use. The queues to manage data
 *          transfer requests are created in this function.
 *
 *  @modif  LDRV_CHNL_Object [procId][chnlId]
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Open (IN ProcessorId      procId,
                IN ChannelId        chnlId,
                IN ChannelAttrs *   attrs)
{
    DSP_STATUS     status       = DSP_SOK ;
    SyncAttrs      syncAttrs    = {0}     ;
    Bool           closeEvent1  = FALSE   ;
    Bool           closeEvent2  = FALSE   ;
    Uint32         numChirps              ;

    TRC_3ENTER ("LDRV_CHNL_Open", procId, chnlId, attrs) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (attrs != NULL) ;

    DBC_Assert (LDRV_CHNL_Object [procId][chnlId] == NULL) ;

    status = MEM_Calloc ((Void **) &LDRV_CHNL_Object [procId][chnlId],
                         sizeof(LDRVChnlObject),
                         MEM_DEFAULT) ;

    if (DSP_SUCCEEDED (status)) {
        status = CFG_GetNumValue (CFG_DRIVER_QUEUE, CFG_ID_NONE, &numChirps) ;
        if (DSP_SUCCEEDED (status)) {
            LDRV_CHNL_Object [procId][chnlId]->signature = SIGN_LDRV_CHNL  ;
            LDRV_CHNL_Object [procId][chnlId]->chnlState = ChannelState_Idled  ;
            LDRV_CHNL_Object [procId][chnlId]->attrs    = *attrs ;
            LDRV_CHNL_Object [procId][chnlId]->freeList =
                                                  CreateChirpList (numChirps) ;

            if (LDRV_CHNL_Object [procId][chnlId]->freeList == NULL) {
                status = DSP_EMEMORY ;
                SET_FAILURE_REASON ;
            }
            else {
                LDRV_CHNL_Object [procId][chnlId]->requestList =
                                                  CreateChirpList (0) ;

                if (LDRV_CHNL_Object [procId][chnlId]->requestList == NULL) {
                    status = DSP_EMEMORY ;
                    SET_FAILURE_REASON ;
                }
                else {
                    LDRV_CHNL_Object [procId][chnlId]->completedList =
                                                  CreateChirpList (0) ;
                    if (LDRV_CHNL_Object [procId][chnlId]->completedList
                                                                    == NULL) {
                        status = DSP_EMEMORY ;
                        SET_FAILURE_REASON ;
                    }
                }
            }

            if (DSP_FAILED (status)) {
                FreeChirpList (
                    LDRV_CHNL_Object [procId][chnlId]->freeList) ;
                FreeChirpList (
                    LDRV_CHNL_Object [procId][chnlId]->requestList) ;
                FreeChirpList (
                    LDRV_CHNL_Object [procId][chnlId]->completedList) ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = SYNC_OpenEvent (
                            &(LDRV_CHNL_Object [procId][chnlId]->syncEvent),
                            &syncAttrs ) ;
            closeEvent1 = TRUE ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = SYNC_OpenEvent (
                            &(LDRV_CHNL_Object [procId][chnlId]->chnlIdleSync),
                            &syncAttrs ) ;
            closeEvent2 = TRUE ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        /* Set the state of a new channel to Idle */
        if (DSP_SUCCEEDED (status)) {
            status = SYNC_SetEvent (
                            LDRV_CHNL_Object [procId][chnlId]->chnlIdleSync) ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        SYNC_ProtectionStart () ;
        if (DSP_SUCCEEDED (status)) {
            status = LDRV_IO_OpenChannel (procId, chnlId) ;
        }
        SYNC_ProtectionEnd () ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
            LDRV_CHNL_Object [procId][chnlId]->signature = SIGN_NULL ;
            if (closeEvent1) {
                SYNC_CloseEvent (
                            LDRV_CHNL_Object [procId][chnlId]->syncEvent) ;
            }
            if (closeEvent2) {
                SYNC_CloseEvent (
                            LDRV_CHNL_Object [procId][chnlId]->chnlIdleSync) ;
            }
            FREE_PTR (LDRV_CHNL_Object [procId][chnlId]) ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   DSP_FAILED (status)
                || (   DSP_SUCCEEDED (status)
                    && (   LDRV_CHNL_Object [procId][chnlId]->chnlState
                        == ChannelState_Idled)
                   )) ;

    TRC_1LEAVE ("LDRV_CHNL_Open", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_Close
 *
 *  @desc   Closes the channel.
 *
 *  @modif  LDRV_CHNL_Object [procId][chnlId]
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Close (IN ProcessorId procId,
                 IN ChannelId   chnlId)
{
    DSP_STATUS       status    = DSP_SOK ;
    DSP_STATUS       tmpStatus = DSP_SOK ;
    LDRVChnlObject * chnlObj   = NULL ;

    TRC_2ENTER ("LDRV_CHNL_Close", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    chnlObj = LDRV_CHNL_Object [procId][chnlId] ;
    if (chnlObj == NULL) {
        status = DSP_EFAIL ;             /* channel not open */
        SET_FAILURE_REASON ;
    }
    else {
        /* Cancel all pending IO */
        tmpStatus = LDRV_CHNL_Idle (procId, chnlId, FALSE) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        SYNC_ProtectionStart () ;
        tmpStatus = LDRV_IO_CloseChannel (procId, chnlId) ;
        SYNC_ProtectionEnd () ;

        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
        LDRV_CHNL_Object [procId][chnlId] = NULL ;

        tmpStatus = SYNC_CloseEvent (chnlObj->syncEvent) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = SYNC_CloseEvent (chnlObj->chnlIdleSync) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        SYNC_ProtectionStart () ;

        tmpStatus = FreeChirpList (chnlObj->freeList) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = FreeChirpList (chnlObj->requestList) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = FreeChirpList (chnlObj->completedList) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        SYNC_ProtectionEnd () ;

        chnlObj->signature = SIGN_NULL ;

        tmpStatus = FREE_PTR (chnlObj) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LDRV_CHNL_Close", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_AddIORequest
 *
 *  @desc   Adds an IO request to a channel. An IO request may be a request
 *          for transferring a buffer from GPP side to DSP side or vice-versa.
 *          The direction of data transfer is decided by the mode of channel.
 *
 *  @modif  LDRV_CHNL_Object [procId][chnlId]
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_AddIORequest (IN ProcessorId      procId,
                        IN ChannelId        chnlId,
                        IN LDRVChnlIOInfo * ioInfo)
{
    DSP_STATUS        status      = DSP_SOK  ;
    LDRVChnlIRP *     chirp       = NULL     ;
    LDRVChnlObject *  chnlObj     = NULL     ;
    Bool              isListEmpty = FALSE    ;
    Bool              isEOS                  ;
#if defined (DDSP_PROFILE_DETAILED)
    ChnlInstrument *  chnlInst ;
    Uint32            i        ;
#endif

    TRC_3ENTER ("LDRV_CHNL_AddIORequest", procId, chnlId, ioInfo) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (ioInfo != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Start the protection in non-DPC context to ensure that DPC does not
     *  preempt the execution below.
     *  ------------------------------------------------------------------------
     */
    if (ioInfo->dpcContext == FALSE) {
        SYNC_ProtectionStart () ;
    }

    isEOS = (Bool) ((ioInfo->size == 0) ? TRUE : FALSE) ;

    DBC_Assert (LDRV_CHNL_Object [procId][chnlId] != NULL) ;

    if (IS_CHNL_EOS (LDRV_CHNL_Object [procId][chnlId])) {
        status = CHNL_E_EOS ;
        SET_FAILURE_REASON ;
    }
    else if (LIST_IsEmpty (LDRV_CHNL_Object [procId][chnlId]->freeList)) {
        status = CHNL_E_NOIORPS ;
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        chnlObj = LDRV_CHNL_Object [procId][chnlId] ;
        if (IS_CHNL_IDLE (LDRV_CHNL_Object [procId][chnlId])) {
            /* Reset the underlying communication hardware here
             * and make channel ready again
             */
            chnlObj->chnlState = ChannelState_Ready ;
        }

        status = LIST_GetHead (chnlObj->freeList, (ListElement **) &chirp) ;
        if (DSP_SUCCEEDED (status)) {
            isListEmpty = LIST_IsEmpty (chnlObj->requestList) ;
            if (isEOS) {
                chnlObj->chnlState = ChannelState_EOS ;
            }

            chirp->buffer    = ioInfo->buffer   ;
            chirp->size      = ioInfo->size     ;
            chirp->arg       = ioInfo->arg      ; /*  This field is used as
                                                   *  an argument for callback
                                                   *  function.
                                                   */
            chirp->callback  = ioInfo->callback ;
            chirp->iocStatus = LDRV_CHNL_IOCSTATE_CANCELED ;

            status = LIST_PutTail (chnlObj->requestList,
                                   (ListElement *) chirp) ;

#if defined (DDSP_PROFILE)
            (LDRV_Obj.chnlStats.chnlData [procId][chnlId].numBufsQueued)++ ;
#endif /* defined (DDSP_PROFILE) */

#if defined (DDSP_PROFILE_DETAILED)
            /* Store the first few bytes of buffer if output channel */
            if (chnlObj->attrs.mode == ChannelMode_Output) {
                chnlInst = &(LDRV_Obj.chnlStats.chnlData [procId][chnlId]) ;
                for (i = 0 ; i < DATA_LENGTH ; i++) {
                    chnlInst->archive [chnlInst->archIndex][i]
                                                       = chirp->buffer [i] ;
                }
                chnlInst->archIndex++ ;
                if (chnlInst->archIndex == HIST_LENGTH) {
                    chnlInst->archIndex = 0 ;
                }
            }
#endif

            if (DSP_SUCCEEDED (status)) {
                status = SYNC_ResetEvent (chnlObj->chnlIdleSync) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
                else {
                    status = LDRV_IO_Request (procId, chnlId) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
            }
            else {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_ERESOURCE ;
            SET_FAILURE_REASON ;
            chirp = NULL ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  End the protection from DPC.
     *  ------------------------------------------------------------------------
     */
    if (ioInfo->dpcContext == FALSE) {
        SYNC_ProtectionEnd () ;
    }

    /*  ------------------------------------------------------------------------
     *  Schedule the DPC only if the requested list is empty.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        if (isListEmpty == TRUE) {
            status = LDRV_IO_ScheduleDPC (procId, chnlId) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
    }

    TRC_1LEAVE ("LDRV_CHNL_AddIORequest", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_GetIOCompletion
 *
 *  @desc   Waits for a specified amount of time for an I/O completion event
 *          on a channel.
 *          Upon successful completion, a buffer is returned as part of ioInfo
 *          structure. A filled buffer is returned in case of an 'Input' channel
 *          and an empty buffer is returned in case of an 'Output' channel.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_GetIOCompletion (IN  ProcessorId      procId,
                           IN  ChannelId        chnlId,
                           IN  Uint32           timeout,
                           OUT LDRVChnlIOInfo * ioInfo)
{
    DSP_STATUS        status     = DSP_SOK ;
    DSP_STATUS        tmpStatus  = DSP_SOK ;
    LDRVChnlIRP *     chirp      = NULL    ;
    LDRVChnlObject *  chnlObj    = NULL    ;

#if defined (DDSP_PROFILE_DETAILED)
    ChnlInstrument *  chnlInst ;
    Uint32            i        ;
#endif

    TRC_4ENTER ("LDRV_CHNL_GetIOCompletion", procId, chnlId, timeout, ioInfo) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (ioInfo != NULL) ;

    if (LDRV_CHNL_Object [procId][chnlId] == NULL) {
        status = DSP_EFAIL ;      /* channel not opened */
        SET_FAILURE_REASON ;
    }
    else {
        chnlObj = LDRV_CHNL_Object [procId][chnlId] ;

        status = SYNC_WaitOnEvent (chnlObj->syncEvent, timeout) ;

        /*  --------------------------------------------------------------------
         *  Start the protection in non-DPC context to ensure that DPC does not
         *  preempt the execution below.
         *  Even if status of SYNC_WaitOnEvent is a failure (viz. on timeout)
         *  the list operations still need protection.
         *  --------------------------------------------------------------------
         */
        if (ioInfo->dpcContext == FALSE) {
            SYNC_ProtectionStart () ;
        }

        if (DSP_SUCCEEDED (status)) {
            DBC_Assert (!LIST_IsEmpty (chnlObj->completedList)) ;

            status = LIST_GetHead (chnlObj->completedList,
                                   (ListElement **) &chirp) ;
            if (DSP_SUCCEEDED (status)) {
                ioInfo->buffer      = chirp->buffer ;
                ioInfo->size        = chirp->size   ;
                ioInfo->arg         = chirp->arg    ;
                ioInfo->callback    = chirp->callback ;
#if defined (DDSP_PROFILE_DETAILED)
                /* Store the first few bytes of buffer if input channel */
                if (chnlObj->attrs.mode == ChannelMode_Input) {
                    chnlInst = &(LDRV_Obj.chnlStats.chnlData [procId][chnlId]) ;
                    for (i = 0 ; i < DATA_LENGTH ; i++) {
                        chnlInst->archive [chnlInst->archIndex][i]
                                                           = chirp->buffer [i] ;
                    }
                    chnlInst->archIndex++ ;
                    if (chnlInst->archIndex == HIST_LENGTH) {
                        chnlInst->archIndex = 0 ;
                    }
                }
#endif
                if (chirp->iocStatus != LDRV_CHNL_IOCSTATE_COMPLETE) {
                    ioInfo->completionStatus = IOState_NotCompleted ;
                }
                else {
                    ioInfo->completionStatus = IOState_Completed ;
                }

                status = LIST_PutTail (chnlObj->freeList,
                                       (ListElement*) chirp) ;

                if (DSP_FAILED (status)) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            if ((timeout == SYNC_NOWAIT) && (status == SYNC_E_FAIL)) {
                status = CHNL_E_NOIOC ;
                SET_FAILURE_REASON ;
            }
            else {
                SET_FAILURE_REASON ;
            }
        }

        if (LIST_IsEmpty (chnlObj->completedList)) {
            tmpStatus = SYNC_ResetEvent (chnlObj->syncEvent) ;
            if (DSP_FAILED (tmpStatus)) {
                if (DSP_SUCCEEDED (status)) {
                    status =  tmpStatus ;
                    SET_FAILURE_REASON ;
                }
            }
        }
        /*  --------------------------------------------------------------------
         *  End the protection from DPC.
         *  --------------------------------------------------------------------
         */
        if (ioInfo->dpcContext == FALSE) {
            SYNC_ProtectionEnd () ;
        }
    }

    if (DSP_FAILED (status)) {
        ioInfo->buffer = NULL ;
        ioInfo->arg    = 0    ;
        ioInfo->size   = 0    ;
    }

    DBC_Ensure (   DSP_FAILED (status)
                || (   DSP_SUCCEEDED (status)
                    && (ioInfo->buffer != NULL))) ;

    DBC_Ensure (   DSP_SUCCEEDED (status)
                || (   DSP_FAILED (status)
                    && (ioInfo->buffer == NULL))) ;

    TRC_1LEAVE ("LDRV_CHNL_GetIOCompletion", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_AddIOCompletion
 *
 *  @desc   Notification for the completion of IO.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_AddIOCompletion (IN ProcessorId   procId,
                           IN ChannelId     chnlId,
                           IN LDRVChnlIRP * chirp)
{
    DSP_STATUS        status    = DSP_SOK ;
    DSP_STATUS        tmpStatus = DSP_SOK ;
    LDRVChnlObject *  chnlObj   = NULL    ;

    TRC_3ENTER ("LDRV_CHNL_AddIOCompletion", procId, chnlId, chirp) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (LDRV_CHNL_Object [procId][chnlId] != NULL) ;
    DBC_Require (chirp != NULL) ;

    chnlObj = LDRV_CHNL_Object [procId][chnlId] ;

    status = LIST_PutTail (chnlObj->completedList, (ListElement*) chirp) ;
    if (DSP_SUCCEEDED (status)) {
#if defined (DDSP_PROFILE)
        (LDRV_Obj.chnlStats.chnlData [procId][chnlId].numBufsQueued)-- ;
#endif /* defined (DDSP_PROFILE) */

        status = SYNC_SetEvent (chnlObj->syncEvent) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        if (LIST_IsEmpty (chnlObj->requestList)) {
            tmpStatus = SYNC_SetEvent (chnlObj->chnlIdleSync) ;
            if (DSP_FAILED (tmpStatus) && (DSP_SUCCEEDED (status))) {
                status =  tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CHNL_AddIOCompletion", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_Idle
 *
 *  @desc   In case of input mode channel this function discards all pending
 *          input requests from the channel. In case of output mode channel,
 *          action of this function depends upon the flush parameter and is as
 *          follows:
 *          If flush is TRUE this function will block till all output buffers
 *          are transferred to the DSP.
 *          If flush is FALSE this function will discard all the output
 *          requests pending on this channel without blocking.
 *
 *  @modif  LDRV_CHNL_Object [procId][chnlId]
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Idle (IN ProcessorId procId,
                IN ChannelId   chnlId,
                IN Bool        flush)
{
    DSP_STATUS        status  = DSP_SOK ;
    LDRVChnlIRP *     chirp   = NULL    ;
    Bool              done    = FALSE   ;
    LDRVChnlObject *  chnlObj = NULL    ;

    TRC_3ENTER ("LDRV_CHNL_Idle", procId, chnlId, flush) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require ((flush == TRUE) || (flush == FALSE))

    DBC_Assert (LDRV_CHNL_Object [procId][chnlId] != NULL) ;

    chnlObj = LDRV_CHNL_Object [procId][chnlId] ;

    if (chnlObj == NULL) {
        status = DSP_EFAIL ;      /* channel not opened */
        SET_FAILURE_REASON ;
    }
    else {
        if (   IS_INPUT_CHNL (chnlObj)
            || (   IS_OUTPUT_CHNL (chnlObj)
                && (flush == FALSE))) {
            /*  ----------------------------------------------------------------
             *  Start the protection from DPC.
             *  ----------------------------------------------------------------
             */
            SYNC_ProtectionStart () ;

            /*  ----------------------------------------------------------------
             *  Discard all pending IO.
             *  ----------------------------------------------------------------
             */
            while ((!LIST_IsEmpty (chnlObj->requestList)) && (!done)) {
                status = LIST_GetHead (chnlObj->requestList,
                                       (ListElement **) &chirp) ;
                if (DSP_SUCCEEDED (status)) {
                    /* Call the callback function if it is specified */
                    DBC_Assert (chirp != NULL) ;
                    if (chirp->callback != NULL) {
                        chirp->callback (procId,
                                         CHNL_E_CANCELLED,
                                         chirp->buffer,
                                         chirp->size,
                                         (Pvoid) chirp->arg) ;
                        /* The status is ignored here */
                    }

                    chirp->size = 0 ;
                    chirp->iocStatus |= LDRV_CHNL_IOCSTATE_CANCELED ;

                    status = LIST_PutTail (chnlObj->completedList,
                                          (ListElement *) chirp) ;

                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                    else {
                        status = SYNC_SetEvent (chnlObj->syncEvent) ;
                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                            done = TRUE ;
                        }
                    }
                }
                else {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                    done = TRUE ;
                }
            }
            if (LIST_IsEmpty (chnlObj->requestList) && DSP_SUCCEEDED (status)) {
                status = SYNC_SetEvent (chnlObj->chnlIdleSync) ;
                if (DSP_SUCCEEDED (status)) {
                    status = LDRV_IO_Cancel (procId, chnlId) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }

            if (DSP_SUCCEEDED (status)) {
                chnlObj->chnlState = ChannelState_Idled ;
            }

            /*  ----------------------------------------------------------------
             *  End the protection from DPC.
             *  ----------------------------------------------------------------
             */
            SYNC_ProtectionEnd () ;
        }
        else {
            status = SYNC_WaitOnEvent (chnlObj->chnlIdleSync,
                                       SYNC_WAITFOREVER) ;
            if (DSP_SUCCEEDED (status)) {
                chnlObj->chnlState = ChannelState_Idled ;
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
    }

    DBC_Ensure (   DSP_FAILED(status)
                || (   DSP_SUCCEEDED (status)
                    && LIST_IsEmpty (
                             LDRV_CHNL_Object [procId][chnlId]->requestList)
                    && (   LDRV_CHNL_Object [procId][chnlId]->chnlState
                        == ChannelState_Idled))) ;

    TRC_1LEAVE ("LDRV_CHNL_Idle", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations
 *          on channels.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Control (IN  ProcessorId  procId,
                   IN  ChannelId    chnlId,
                   IN  Int32        cmd,
                   OPT Pvoid        arg)
{
    DSP_STATUS   status   = DSP_SOK ;

    TRC_4ENTER ("LDRV_CHNL_Control", procId, chnlId, cmd, arg) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    status = DSP_ENOTIMPL ;

    TRC_1LEAVE ("LDRV_CHNL_Control", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_GetChannelMode
 *
 *  @desc   Gets the channel mode.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
ChannelMode
LDRV_CHNL_GetChannelMode (IN ProcessorId   procId,
                          IN ChannelId     chnlId)
{
    TRC_2ENTER ("LDRV_CHNL_GetChannelMode", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    TRC_1LEAVE ("LDRV_CHNL_GetChannelMode",
                LDRV_CHNL_Object [procId][chnlId]->attrs.mode) ;

    return LDRV_CHNL_Object [procId][chnlId]->attrs.mode ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_GetChannelState
 *
 *  @desc   Sets last output channel.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
ChannelState
LDRV_CHNL_GetChannelState (IN ProcessorId   procId,
                           IN ChannelId     chnlId)
{
    ChannelState chnlState = ChannelState_Closed ;

    TRC_2ENTER ("LDRV_CHNL_GetChannelState", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    if (LDRV_CHNL_Object [procId][chnlId] != NULL) {
        chnlState = LDRV_CHNL_Object [procId][chnlId]->chnlState ;
    }

    TRC_1LEAVE ("LDRV_CHNL_GetChannelState", chnlState) ;

    return chnlState ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_SetChannelState
 *
 *  @desc   Sets the channel state.
 *
 *  @modif  LDRV_CHNL_Object [procId][chnlId]
 *  ============================================================================
 */
NORMAL_API
Void
LDRV_CHNL_SetChannelState (IN ProcessorId    procId,
                           IN ChannelId      chnlId,
                           IN ChannelState   state)
{
    TRC_3ENTER ("LDRV_CHNL_SetChannelState", procId, chnlId, state) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    LDRV_CHNL_Object [procId][chnlId]->chnlState = state ;

    TRC_0LEAVE ("LDRV_CHNL_SetChannelState") ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_GetChannelEndianism
 *
 *  @desc   Gets channel endianism information.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Endianism
LDRV_CHNL_GetChannelEndianism (IN ProcessorId   procId,
                               IN ChannelId     chnlId)
{
    LDRVChnlObject *   chnlObj       = NULL    ;
    Endianism          endianismInfo           ;

    TRC_2ENTER ("LDRV_CHNL_GetChannelEndianism", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (LDRV_CHNL_Object [procId][chnlId] != NULL) ;

    chnlObj = LDRV_CHNL_Object [procId][chnlId] ;
    endianismInfo = chnlObj->attrs.endianism ;

    TRC_1LEAVE ("LDRV_CHNL_GetChannelEndianism", endianismInfo) ;

    return  endianismInfo ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_ChannelHasMoreChirps
 *
 *  @desc   Returns TRUE if the channel has more chirps in the IO request queue.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Bool
LDRV_CHNL_ChannelHasMoreChirps  (IN ProcessorId   procId,
                                 IN ChannelId     chnlId)

{
    Bool hasMoreChirps ;

    TRC_2ENTER ("LDRV_CHNL_ChannelHasMoreChirps", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    if (LIST_IsEmpty (LDRV_CHNL_Object [procId][chnlId]->requestList)) {
        hasMoreChirps = FALSE ;
    }
    else {
        hasMoreChirps = TRUE ;
    }

    TRC_1LEAVE ("LDRV_CHNL_ChannelHasMoreChirps", hasMoreChirps) ;

    return hasMoreChirps ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_GetRequestChirp
 *
 *  @desc   Gets the chirp from request queue of the specified channel.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
LDRVChnlIRP *
LDRV_CHNL_GetRequestChirp (IN ProcessorId   procId,
                           IN ChannelId     chnlId)
{
    DSP_STATUS        status  = DSP_SOK ;
    LDRVChnlIRP *     chirp   = NULL    ;
    LDRVChnlObject *  chnlObj = NULL    ;

    TRC_2ENTER ("LDRV_CHNL_GetRequestChirp", procId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (LDRV_CHNL_Object [procId][chnlId] != NULL) ;

    chnlObj = LDRV_CHNL_Object [procId][chnlId] ;

    status = LIST_GetHead (chnlObj->requestList, (ListElement **) &chirp) ;
    if (DSP_FAILED (status)) {
        chirp = NULL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CHNL_GetRequestChirp", chirp) ;
    return chirp ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_HandshakeSetup
 *
 *  @desc   It does initializations related to handshake procedure that are
 *          required before starting of DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_HandshakeSetup (IN ProcessorId procId)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("LDRV_CHNL_HandshakeSetup", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    status = LDRV_IO_HandshakeSetup (procId) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CHNL_HandshakeSetup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_CHNL_Handshake
 *
 *  @desc   Does the necessary handshake (if required) between for the channels
 *          across GPP & DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Handshake (IN ProcessorId procId)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("LDRV_CHNL_Handshake", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    status = LDRV_IO_Handshake (procId) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CHNL_Handshake", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LDRV_CHNL_Instrument
 *
 *  @desc   Gets the instrumentation information related to the
 *          specified channel
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Instrument (IN  ProcessorId       procId,
                      IN  ChannelId         chnlId,
                      OUT ChnlInstrument *  retVal)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("LDRV_CHNL_Instrument", procId, chnlId, retVal) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (   (IS_VALID_CHNLID (procId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (retVal != NULL) ;

    if (retVal == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        *retVal = LDRV_Obj.chnlStats.chnlData [procId][chnlId] ;
    }

    TRC_1LEAVE ("LDRV_CHNL_Instrument", status) ;

    return status ;
}
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_CHNL_Debug
 *
 *  @desc   Prints out debug information of CHNL module. It will print all
 *          the important data structures and variable of this module.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LDRV_CHNL_Debug (ProcessorId procId, ChannelId chnlId)
{
    TRC_2ENTER ("LDRV_CHNL_Debug", procId, chnlId) ;

    TRC_3PRINT (TRC_LEVEL4, "ChannelObject"
                            " [procId=%d][chnlId =%d] = 0x%x\n",
                            procId, chnlId,
                            LDRV_CHNL_Object [procId][chnlId]) ;

    if (LDRV_CHNL_Object [procId][chnlId] != NULL) {
        TRC_1PRINT (TRC_LEVEL4, "Channel State = [0x%x]\n",
                              LDRV_CHNL_Object [procId][chnlId]->chnlState) ;

        TRC_0PRINT (TRC_LEVEL4, "Free List Info:\n") ;

        PrintChirpList (LDRV_CHNL_Object [procId][chnlId]->freeList) ;

        TRC_0PRINT (TRC_LEVEL4, "Free List Info Ends\n") ;

        TRC_0PRINT (TRC_LEVEL4, "Request List Info:\n") ;

        PrintChirpList (LDRV_CHNL_Object [procId][chnlId]->requestList) ;

        TRC_0PRINT (TRC_LEVEL4, "Request List Info Ends\n") ;

        TRC_0PRINT (TRC_LEVEL4, "Completed List Info:\n") ;

        PrintChirpList (LDRV_CHNL_Object [procId][chnlId]->completedList) ;

        TRC_0PRINT (TRC_LEVEL4, "Completed List Info Ends\n") ;

        TRC_1PRINT (TRC_LEVEL4, "Channel SyncEvent = [0x%x]\n",
                    LDRV_CHNL_Object [procId][chnlId]->syncEvent) ;
    }

    TRC_0PRINT (TRC_LEVEL1, "DEBUG INFO END\n") ;

    TRC_0LEAVE ("LDRV_CHNL_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


/** ----------------------------------------------------------------------------
 *  @func   AllocateChirp
 *
 *  @desc   Allocates a single chirp.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
LDRVChnlIRP *
AllocateChirp ()
{
    DSP_STATUS     status = DSP_SOK ;
    LDRVChnlIRP *  chirp  = NULL    ;

    TRC_0ENTER ("AllocateChirp") ;

    status = MEM_Calloc ((Void **) &chirp, sizeof(LDRVChnlIRP), MEM_DEFAULT) ;
    if (DSP_SUCCEEDED (status)) {
        status = LIST_InitializeElement ((ListElement *) &(chirp->link)) ;
        if (DSP_FAILED (status)) {
            FREE_PTR (chirp) ;
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("AllocateChirp", chirp) ;

    return chirp ;
}


/** ----------------------------------------------------------------------------
 *  @func   FreeChirpList
 *
 *  @desc   deallocates a chirp list.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
FreeChirpList (IN List * chirpList)
{
    DSP_STATUS     status     = DSP_SOK ;
    DSP_STATUS     freeStatus = DSP_SOK ;
    LDRVChnlIRP *  listHead             ;

    TRC_1ENTER ("FreeChirpList", chirpList) ;

    if (chirpList != NULL) {
        while (!LIST_IsEmpty (chirpList)) {
            status = LIST_GetHead (chirpList,
                                   (ListElement **) &listHead) ;

            if (DSP_FAILED (status)) {
                freeStatus = status ;
                SET_FAILURE_REASON ;
            }
            else {
                status = FREE_PTR (listHead) ;
                if (DSP_FAILED (status)) {
                    freeStatus = status ;
                    SET_FAILURE_REASON ;
                }
            }
        }

        status = LIST_Delete (chirpList) ;
        if (DSP_SUCCEEDED (status)) {
            status = freeStatus ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("FreeChirpList", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   CreateChirpList
 *
 *  @desc   Allocates a chirp list.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
List *
CreateChirpList (IN Uint32 numIRPs)
{
    DSP_STATUS     status    = DSP_SOK ;
    List *         chirpList = NULL    ;
    LDRVChnlIRP *  chirp     = NULL    ;
    Uint32         i                   ;

    TRC_1ENTER ("CreateChirpList", numIRPs) ;

    status = LIST_Create (&chirpList) ;
    if (DSP_SUCCEEDED (status)) {

        /* Make N chirps and place on queue. */
        for (i = 0 ; (i < numIRPs) && (DSP_SUCCEEDED (status)) ; i++) {
            chirp = AllocateChirp () ;
            if (chirp != NULL) {
                status = LIST_PutTail (chirpList, (ListElement *) chirp) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                    FREE_PTR (chirp) ;
                }
            }
            else {
                status = DSP_EMEMORY ;
                SET_FAILURE_REASON ;
            }
        }

        /* If we couldn't allocate all chirps, free those allocated: */
        if ((i != numIRPs) || DSP_FAILED (status)) {
            FreeChirpList(chirpList) ;  /* No care for status */
            chirpList = NULL ;
        }
    }
    else {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("CreateChirpList", chirpList) ;

    return chirpList ;
}


#if defined (DDSP_DEBUG)
/** ----------------------------------------------------------------------------
 *  @func   PrintChirp
 *
 *  @desc   Prints a single CHIRP (Channel IO Request Packet).
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
PrintChirp (IN LDRVChnlIRP * chirp)
{
    if (chirp == NULL) {
        TRC_0PRINT (TRC_LEVEL4, "\nChirp is NULL\n") ;
    }
    else {
        TRC_5PRINT (TRC_LEVEL4,
                    "CHIRP [0x%x]:\n"
                    "    buffer    = 0x%x\n"
                    "    arg       = 0x%x\n"
                    "    size      = 0x%x\n"
                    "    iocStatus = 0x%x\n",
                    chirp, chirp->buffer, chirp->arg,
                    chirp->size, chirp->iocStatus) ;
    }
}


/** ----------------------------------------------------------------------------
 *  @func   PrintChirpList
 *
 *  @desc   Prints a CHIRP (Channel IO Request Packet) List.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
PrintChirpList (IN List * chirpList)
{
    DSP_STATUS     status     = DSP_SOK ;
    LDRVChnlIRP *  curElem    = NULL    ;
    LDRVChnlIRP *  nextElem   = NULL    ;

    if (chirpList == NULL) {
        TRC_0PRINT (TRC_LEVEL4, "  List is NULL\n") ;
    }
    else if (LIST_IsEmpty (chirpList)) {
        TRC_0PRINT (TRC_LEVEL4, "  List is Empty\n") ;
    }
    else {
        TRC_0PRINT (TRC_LEVEL4, "  List is Non-Empty\n") ;
        TRC_0PRINT (TRC_LEVEL4, ">>>>ChirpList Elements: Start<<<<<\n") ;
        LIST_First (chirpList, (ListElement **) &curElem) ;
        PrintChirp (curElem) ;
        status = LIST_Next (chirpList, (ListElement *) curElem,
                            (ListElement **) &nextElem) ;
        while (DSP_SUCCEEDED (status)  && (nextElem != NULL)) {
            PrintChirp (nextElem) ;
            curElem = nextElem ;
            status = LIST_Next (chirpList, (ListElement *) curElem,
                                (ListElement **) &nextElem) ;
        }
        TRC_0PRINT (TRC_LEVEL4, ">>>>ChirpList Elements: End<<<<<\n") ;
    }
}
#endif  /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
