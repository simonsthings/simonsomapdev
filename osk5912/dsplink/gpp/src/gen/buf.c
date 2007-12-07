/** ============================================================================
 *  @file   buf.c
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Implementation of platform independent BUF subcomponent.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <print.h>
#include <mem.h>
#include <sync.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <buf.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   COMPONENT_ID
 *
 *  @desc   Component ID of this module. That will be used by TRC macros.
 *  ============================================================================
 */
#define COMPONENT_ID        ID_GEN_BUF

/** ============================================================================
 *  @name   SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_GEN_BUF, __LINE__)


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @name   BUF_FREESTAMP
 *
 *  @desc   Value to indicate that the buffer was never allocated.
 *  ============================================================================
 */
#define BUF_FREESTAMP        0xBEEF

/** ============================================================================
 *  @name   BUF_ALLOCSTAMP
 *
 *  @desc   Value to indicate that the buffer was allocated.
 *  ============================================================================
 */
#define BUF_ALLOCSTAMP        0xCAFE
#endif /* if defined (DDSP_PROFILE) */


/** ============================================================================
 *  @func   BUF_Initialize
 *
 *  @desc   This function initializes the buffer component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Initialize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("BUF_Initialize") ;

    TRC_1LEAVE ("BUF_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   BUF_Finalize
 *
 *  @desc   This function finalizes the buffer component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Finalize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("BUF_Finalize") ;

    TRC_1LEAVE ("BUF_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   BUF_Create
 *
 *  @desc   This function creates and initializes a fixed buffer pool and
 *          returns a handle to the corresponding buffer pool object.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Create (IN     Uint16       numBufs,
            IN     Uint16       size,
            OUT    BufHandle  * bufHandle,
            IN OPT Uint32       bufAddress)
{
    DSP_STATUS  status       = DSP_SOK ;
    Uint32      bufPtr       = 0       ;
    Uint32      i            = 0       ;

    TRC_4ENTER ("BUF_Create", numBufs, size, bufHandle, bufAddress) ;

    DBC_Require (bufHandle != NULL) ;
    DBC_Require (size >= MIN_BUF_SIZE) ;
    DBC_Require (numBufs > 0) ;

    if (   (bufHandle == NULL)
        || (size < MIN_BUF_SIZE)
        || (numBufs <= 0)) {
        /* Set buffer handle to NULL if its valid */
        if (bufHandle != NULL) {
            *bufHandle = NULL ;
        }
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        status = MEM_Alloc ((Void **) bufHandle,
                            (sizeof (BufObj)),
                            MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            if ((Pvoid) bufAddress == NULL) {
                /* Allocate memory for BUF pool */
                status = MEM_Alloc ((Void **) &((*bufHandle)->nextFree),
                                    (size * numBufs),
                                    MEM_DEFAULT) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                    FREE_PTR (*bufHandle) ;
                }
                else {
                    (*bufHandle)->freePool  = TRUE ;
                }
            }
            else {
                /*  Client has allocated memory and given the pointer to this
                 *  function
                 */
                (*bufHandle)->freePool  = FALSE ;
                (*bufHandle)->nextFree  = bufAddress ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            (*bufHandle)->size          = size    ;
            (*bufHandle)->totalBuffers  = numBufs ;
            (*bufHandle)->freeBuffers   = numBufs ;
            (*bufHandle)->startAddress  = (*bufHandle)->nextFree ;

            bufPtr = (*bufHandle)->nextFree ;
            /* Construct list of free buffers */
            for (i = 1 ; i < numBufs ; i++) {
                /* Stitch the current buffer with next free buffer */
                ((BufHeader *) bufPtr)->next = (BufHeader *) (bufPtr + size) ;

#if defined (DDSP_PROFILE)
                /*  usedOnce field set to indicate that buffer is
                 *  not yet allocated
                 */
                ((BufHeader *) bufPtr)->usedOnce = BUF_FREESTAMP ;
#endif /* if defined (DDSP_PROFILE) */

                bufPtr = bufPtr + size ;
            }
            /* The next field of last buffer is filled with zero */
            ((BufHeader *) bufPtr)->next = (BufHeader *) NULL ;

#if defined (DDSP_PROFILE)
            /* Set the usedOnce field of last buffer */
            ((BufHeader *) bufPtr)->usedOnce = BUF_FREESTAMP ;
#endif /* if defined (DDSP_PROFILE) */
        }
    }

    DBC_Ensure (   ((bufHandle == NULL) && DSP_FAILED (status))
                || (   (bufHandle != NULL)
                     && (*bufHandle == NULL)
                     && DSP_FAILED (status))
                || (DSP_SUCCEEDED (status))) ;

    DBC_Ensure ((   (bufHandle != NULL)
                    && (*bufHandle != NULL)
                    && DSP_SUCCEEDED (status))
                 || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("BUF_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   BUF_Delete
 *
 *  @desc   This function deletes the buffer pool specified by the user.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Delete (IN  BufHandle  bufHandle)
{
    DSP_STATUS  status       = DSP_SOK ;
    DSP_STATUS  tmpStatus    = DSP_SOK ;

    TRC_1ENTER ("BUF_Delete", bufHandle) ;

    DBC_Require (bufHandle != NULL) ;

    if (bufHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        DBC_Assert (bufHandle->totalBuffers == bufHandle->freeBuffers) ;
        if (bufHandle->freePool == TRUE) {
            status = MEM_Free ((Pvoid)(bufHandle->startAddress), MEM_DEFAULT) ;
            bufHandle->startAddress = 0x0 ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        tmpStatus = FREE_PTR (bufHandle) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("BUF_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   BUF_Alloc
 *
 *  @desc   This function allocates a free buffer from the specified buffer pool
 *          and returns it to the user.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Alloc (IN  BufHandle  bufHandle, OUT Pvoid * buffer)
{
    DSP_STATUS   status   = DSP_SOK ;
    BufHeader  * bufPtr   = NULL    ;
    Uint32       irqFlags           ;

    TRC_2ENTER ("BUF_Alloc", bufHandle, buffer) ;

    DBC_Require (bufHandle != NULL) ;
    DBC_Require (buffer != NULL) ;

    if ((bufHandle == NULL) || (buffer == NULL)) {
        if (buffer != NULL) {
            *buffer = NULL ;
        }
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        irqFlags = SYNC_SpinLockStart () ;
        bufPtr = (BufHeader *)(bufHandle->nextFree) ;
        if (bufPtr != NULL) {

            /* Assign next free buffer to next pointer */
            bufHandle->nextFree = (Uint32)(bufPtr->next) ;

#if defined (DDSP_PROFILE)
            /*  usedOnce field set to indicate that buffer is
             *  not yet allocated
             */
            bufPtr->usedOnce = BUF_ALLOCSTAMP ;
#endif /* if defined (DDSP_PROFILE) */

            /* Decrement the number of free buffers */
            bufHandle->freeBuffers-- ;
            SYNC_SpinLockEnd (irqFlags) ;
        }
        else {
            status = DSP_EMEMORY ;
            SET_FAILURE_REASON ;
        }
        *buffer = bufPtr ;
    }

    DBC_Ensure (   ((buffer == NULL) && DSP_FAILED (status))
                || (   (buffer != NULL)
                     && (*buffer == NULL)
                     && DSP_FAILED (status))
                || (DSP_SUCCEEDED (status))) ;

    DBC_Ensure ((   (buffer != NULL)
                    && (*buffer != NULL)
                    && DSP_SUCCEEDED (status))
                 || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("BUF_Alloc", status) ;

    return status ;
}


/** ============================================================================
 *  @func   BUF_Free
 *
 *  @desc   This function frees the buffer specified by the user, and returns it
 *          to the buffer pool.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Free (IN  BufHandle  bufHandle, IN  Pvoid  buffer)
{
    DSP_STATUS   status     = DSP_SOK ;
    Uint32       endAddr    = 0       ;
    Uint32       bufOffset  = 0       ;
    Uint32       irqFlags             ;

    TRC_2ENTER ("BUF_Free", bufHandle, buffer) ;

    DBC_Require (bufHandle != NULL) ;
    DBC_Require (buffer != NULL) ;

    if ((bufHandle == NULL) || (buffer == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        irqFlags = SYNC_SpinLockStart () ;
        endAddr = bufHandle->startAddress
                 + ((bufHandle->size) * (bufHandle->totalBuffers)) ;
        if (   ((Uint32) buffer < bufHandle->startAddress)
            || ((Uint32) buffer >= endAddr)) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
        else {
            bufOffset = (Uint32) buffer - bufHandle->startAddress ;
            if (((bufOffset) % (bufHandle->size) == 0)) {
                /* Stitch the freed buffer at top of pool */
                ((BufHeader *)buffer)->next = (BufHeader *)(bufHandle->nextFree) ;

                /* Make the freed buffer as next free buffer */
                bufHandle->nextFree = (Uint32)buffer ;

                /* Increment the number of free buffers */
                bufHandle->freeBuffers++ ;
            }
            else {
                /* invalid buffer being freed */
                status = DSP_EMEMORY ;
                SET_FAILURE_REASON ;
            }
        }
        SYNC_SpinLockEnd (irqFlags) ;
    }

    TRC_1LEAVE ("BUF_Free", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   BUF_GetStats
 *
 *  @desc   This function gets instrumentation information about the specified
 *          buffer pool.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_GetStats (IN  BufHandle  bufHandle, OUT BufStats * bufStats)
{
    DSP_STATUS   status     = DSP_SOK ;
    BufHeader  * bufHeadPtr = NULL    ;
    Uint16       count      = 0       ;

    TRC_2ENTER ("BUF_GetStats", bufHandle, bufStats) ;

    DBC_Require (bufHandle != NULL) ;
    DBC_Require (bufStats != NULL) ;

    if ((bufHandle == NULL) || (bufStats == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        bufStats->size = bufHandle->size ;
        bufStats->totalBuffers =  bufHandle->totalBuffers ;
        bufStats->freeBuffers = bufHandle->freeBuffers ;

        bufHeadPtr = (BufHeader *) bufHandle->startAddress ;
        /*  Traverse through the buffer pool till BUF_FREESTAMP is found
         *  on usedOnce field
         */
        for (count = 0 ; (   (bufHeadPtr->usedOnce != BUF_FREESTAMP)
                          && (count < bufHandle->totalBuffers)) ; count++) {

            /* Assign the next buffer address to bufheadptr */
            bufHeadPtr = (BufHeader *) (  (Char8 *) bufHeadPtr
                                        + bufHandle->size) ;
        }
        bufStats->maxUsed = count ;
    }

    TRC_1LEAVE ("BUF_GetStats", status) ;

    return status ;
}
#endif /* if defined (DDSP_PROFILE) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
