/** ============================================================================
 *  @file   mem.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implementation of sub-component MEM.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <asm/io.h>

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
#include <mem_os.h>
#include <mem.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Identifier for sub-component.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_OSAL_MEM

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_OSAL_MEM, __LINE__)


/** ============================================================================
 *  @name   MEM_IsInitialized
 *
 *  @desc   Flag to track initialization of the sub-component.
 *  ============================================================================
 */
STATIC Bool    MEM_IsInitialized  = FALSE   ;


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @name   MEM_DefaultAlloc
 *
 *  @desc   Number of allocation with default type done so far.
 *  ============================================================================
 */
STATIC Uint32 MEM_DefaultAlloc = 0 ;

/** ============================================================================
 *  @name   MEM_DefaultFree
 *
 *  @desc   Number of deallocation with default type done so far.
 *  ============================================================================
 */
STATIC Uint32 MEM_DefaultFree = 0 ;

/** ============================================================================
 *  @name   MEM_SpecialAlloc
 *
 *  @desc   Number of allocation with non-default type done so far.
 *  ============================================================================
 */
STATIC Uint32 MEM_SpecialAlloc = 0 ;

/** ============================================================================
 *  @name   MEM_SpecialFree
 *
 *  @desc   Number of deallocation with non-default type done so far.
 *  ============================================================================
 */
STATIC Uint32 MEM_SpecialFree = 0 ;
#endif


/** ============================================================================
 *  @func   MEM_Initialize
 *
 *  @desc   Initializes the MEM sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Initialize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("MEM_Initialize") ;

    DBC_Require (MEM_IsInitialized == FALSE) ;

    MEM_IsInitialized = TRUE ;

    TRC_1LEAVE ("MEM_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MEM_Finalize
 *
 *  @desc   Releases resources used by this sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Finalize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("MEM_Finalize") ;

    DBC_Require (MEM_IsInitialized == TRUE) ;

    MEM_IsInitialized = FALSE ;

    TRC_1LEAVE ("MEM_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MEM_Alloc
 *
 *  @desc   Allocates the specified number of bytes.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Alloc (OUT Void ** ptr, IN Uint32 cBytes, IN OUT Pvoid arg)
{
    DSP_STATUS       status   = DSP_SOK ;
    MemAllocAttrs *  allocArg = NULL    ;

    TRC_3ENTER ("MEM_Alloc", ptr, cBytes, arg) ;

    DBC_Require (ptr != NULL) ;
    DBC_Require (MEM_IsInitialized == TRUE) ;
    DBC_Require (cBytes != 0) ;

    /*  ------------------------------------------------------------------------
     *  Validate arguments
     *  ------------------------------------------------------------------------
     */
    if (ptr == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (cBytes == 0) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;

        *ptr = NULL ;
    }
    else {
        if (arg == MEM_DEFAULT) {
            /*  ----------------------------------------------------------------
             *  Allocations from 'default' memory area
             *  ----------------------------------------------------------------
             */
            *ptr = vmalloc (cBytes) ;
            if (*ptr == NULL) {
                status = DSP_EMEMORY ;
                SET_FAILURE_REASON ;
            }
#if defined (DDSP_DEBUG)
            else {
                MEM_DefaultAlloc++ ;
            }
#endif    /* if defined (DDSP_DEBUG) */
        }
        else {
            /*  ----------------------------------------------------------------
             *  OS dependent allocation from 'special' memory area(s).
             *  ----------------------------------------------------------------
             */
            allocArg = (MemAllocAttrs *) arg ;

            *ptr = consistent_alloc (
                            GFP_KERNEL,
                            cBytes,
                            (dma_addr_t *) &(allocArg->physicalAddress)) ;

            if (*ptr == NULL) {
                status = DSP_EMEMORY ;
                SET_FAILURE_REASON ;
            }
#if defined (DDSP_DEBUG)
            else {
                MEM_SpecialAlloc++ ;
            }
#endif    /* if defined (DDSP_DEBUG) */
        }
    }

    DBC_Ensure (   ((ptr == NULL) && DSP_FAILED (status))
                || ((ptr != NULL) && (*ptr != NULL) && DSP_SUCCEEDED (status))
                || ((ptr != NULL) && (*ptr == NULL) && DSP_FAILED (status))) ;

    TRC_1LEAVE ("MEM_Alloc", status) ;

    return status ;
}

/** ============================================================================
 *  @func   MEM_Calloc
 *
 *  @desc   Allocates the specified number of bytes and memory is set to zero.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Calloc (OUT Void ** ptr, IN Uint32 cBytes, IN OUT Pvoid arg)
{
    DSP_STATUS status = DSP_SOK ;
    Uint32       i                ;

    TRC_3ENTER ("MEM_Calloc", ptr, cBytes, arg) ;

    DBC_Require (ptr != NULL) ;
    DBC_Require (MEM_IsInitialized == TRUE) ;
    DBC_Require (cBytes != 0) ;

    status = MEM_Alloc (ptr, cBytes, arg) ;

    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; i < cBytes ; i++) {
            (*(Uint8 **) ptr)[i] = 0 ;
        }
    }

    DBC_Ensure (   ((ptr == NULL) && DSP_FAILED (status))
                || ((ptr != NULL) && (*ptr != NULL) && DSP_SUCCEEDED (status))
                || ((ptr != NULL) && (*ptr == NULL) && DSP_FAILED (status))) ;

    TRC_1LEAVE ("MEM_Calloc", status) ;

    return  status ;
}


/** ============================================================================
 *  @func   MEM_Free
 *
 *  @desc   Frees up the specified chunk of memory.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Free (IN Pvoid ptr, IN Pvoid arg)
{
    DSP_STATUS       status   = DSP_SOK ;
    MemFreeAttrs *    freeArg  = NULL    ;

    TRC_2ENTER ("MEM_Free", ptr, arg) ;

    DBC_Require (ptr != NULL) ;
    DBC_Require (MEM_IsInitialized == TRUE) ;

    /*  ------------------------------------------------------------------------
     *  Validate arguments
     *  ------------------------------------------------------------------------
     */
    if (ptr == NULL) {
        status = DSP_EPOINTER ;
    }
    else {
        if (arg == MEM_DEFAULT) {
            /*  ----------------------------------------------------------------
             *  Free allocations from 'default' memory area
             *  ----------------------------------------------------------------
             */
            vfree (ptr) ;

#if defined (DDSP_DEBUG)
            MEM_DefaultFree++ ;
#endif    /* if defined (DDSP_DEBUG) */
        }
        else {
            /*  ----------------------------------------------------------------
             *  Free OS dependent allocation from 'special' memory area(s).
             *  ----------------------------------------------------------------
             */
            freeArg = (MemFreeAttrs *) arg ;

            consistent_free (ptr,
                             freeArg->size,
                             (dma_addr_t) freeArg->physicalAddress) ;

#if defined (DDSP_DEBUG)
            MEM_SpecialFree++ ;
#endif    /* if defined (DDSP_DEBUG) */
        }
    }

    TRC_1LEAVE ("MEM_Free", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MEM_Map
 *
 *  @desc   Maps a memory area into virtual space.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Map (IN OUT MemMapInfo * mapInfo)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("MEM_Map", mapInfo) ;

    DBC_Require (mapInfo != NULL) ;

    mapInfo->dst = 0 ;
    if (mapInfo != NULL) {
        mapInfo->dst = (Uint32) ioremap_nocache ((dma_addr_t) (mapInfo->src),
                                                  mapInfo->size) ;

        if (mapInfo->dst == 0) {
            status = DSP_EMEMORY ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (  (DSP_SUCCEEDED (status) && (mapInfo->dst != 0))
               || (DSP_FAILED (status)    && (mapInfo->dst == 0))) ;

    TRC_1LEAVE ("MEM_Map", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MEM_Unmap
 *
 *  @desc   Unmaps a virtual memory area.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Unmap (IN MemUnmapInfo * unmapInfo)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("MEM_Unmap", unmapInfo) ;

    DBC_Require (unmapInfo != NULL) ;

    if (unmapInfo != NULL) {
        DBC_Assert (unmapInfo->addr != 0) ;
        iounmap ((unsigned int *) unmapInfo->addr) ;
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("MEM_Unmap", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MEM_Copy
 *
 *  @desc   Copies the data between memory areas.
 *          (The endianism is currently not used.)
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
MEM_Copy (IN Uint8 * dst, OUT Uint8 * src, IN Uint32 len, IN Endianism end)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_4ENTER ("MEM_Copy", dst, src, len, end) ;

    memcpy (dst, src, len) ;

    TRC_1LEAVE ("MEM_Copy", status) ;

    return status ;
}


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   MEM_Debug
 *
 *  @desc   Prints debug information for MEM.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
MEM_Debug ()
{
    TRC_0ENTER ("MEM_Debug") ;

    TRC_4PRINT (TRC_LEVEL4,
                "    MEM_DefaultAlloc [%d]\n"
                "    MEM_DefaultFree  [%d]\n"
                "    MEM_SpecialAlloc [%d]\n"
                "    MEM_SpecialFree  [%d]\n",
                 MEM_DefaultAlloc, MEM_DefaultFree,
                 MEM_SpecialAlloc, MEM_SpecialFree) ;

    TRC_0LEAVE ("MEM_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
