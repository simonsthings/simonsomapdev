/** ============================================================================
 *  @file   OSAL_SyncLeaveCS.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of SYNC_LeaveCS API test case.
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
#include <errbase.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- OSAL Headers                  */
#include <sync.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <OSAL_PostProcess.h>
#include <OSAL_SyncLeaveCS.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   OSAL_SyncLeaveCS
 *
 *  @desc   Tests the SYNC_LeaveCS API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_SyncLeaveCS (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS     statusOfSetup    = DSP_SOK   ;
    DSP_STATUS     status           = DSP_SOK   ;
    Int32          nullTest         = 0         ;
    SyncCsObject * csObj            = NULL      ;

    DBC_Require (argc == 3) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 3) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_SyncLeaveCS. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        nullTest = TST_StringToInt (argv [2]) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = SYNC_Initialize () ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = SYNC_CreateCS (&csObj) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = SYNC_EnterCS (csObj) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests SYNC_LeaveCS.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        if (nullTest == 1) {
            status = SYNC_LeaveCS (NULL) ;
        }
        else {
            status = SYNC_LeaveCS (csObj) ;
        }
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    SYNC_DeleteCS (csObj) ;
    SYNC_Finalize () ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
