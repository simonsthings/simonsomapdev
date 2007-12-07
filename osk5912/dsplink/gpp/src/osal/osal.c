/** ============================================================================
 *  @file   osal.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal
 *
 *  @desc   Implementation of OS Adaptation layer initialization and
 *          finalization functions.
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
#include <intobject.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <osal.h>
#include <cfg.h>
#include <mem.h>
#include <dpc.h>
#include <isr.h>
#include <prcs.h>
#include <sync.h>
#include <kfile.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_OSAL_OSAL

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  GEN_SetReason (status, FID_C_OSAL_OSAL, __LINE__)


/** ============================================================================
 *  @name   OSAL_InitializeCount
 *
 *  @desc   Number of times OSAL has been initialized.
 *  ============================================================================
 */
STATIC Uint32  OSAL_InitializeCount = 0   ;


/** ============================================================================
 *  @func   OSAL_Initialize
 *
 *  @desc   Initializes the OS Adaptation layer.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_Initialize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("OSAL_Initialize") ;

    if (OSAL_InitializeCount == 0) {
        status = MEM_Initialize   () ;
        if (DSP_SUCCEEDED (status)) {
            status = SYNC_Initialize  () ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = KFILE_Initialize () ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = ISR_Initialize   () ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = DPC_Initialize   () ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = PRCS_Initialize  () ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = PRINT_Initialize  () ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = CFG_Initialize  () ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            OSAL_InitializeCount++ ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
    else {
        OSAL_InitializeCount++ ;
    }

    TRC_1LEAVE ("OSAL_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   OSAL_Finalize
 *
 *  @desc   Releases OS adaptation layer resources indicating that they would
 *          no longer be used.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_Finalize ()
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;

    TRC_0ENTER ("OSAL_Finalize") ;

    if (OSAL_InitializeCount == 1) {

        status = PRINT_Finalize  () ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }

        tmpStatus = CFG_Finalize   () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = PRCS_Finalize   () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = DPC_Finalize   () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = ISR_Finalize   () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = KFILE_Finalize () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = SYNC_Finalize  () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = MEM_Finalize   () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    if (OSAL_InitializeCount > 0) {
        if (DSP_SUCCEEDED (status)) {
            OSAL_InitializeCount-- ;
        }
    }
    else {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("OSAL_Finalize", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif
