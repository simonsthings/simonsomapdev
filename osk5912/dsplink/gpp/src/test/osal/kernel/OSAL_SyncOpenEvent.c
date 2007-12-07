/** ============================================================================
 *  @file   OSAL_SyncOpenEvent.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of SYNC_OpenEvent API test case.
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
#include <OSAL_SyncOpenEvent.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   OSAL_SyncOpenEvent
 *
 *  @desc   Tests the SYNC_OpenEvent API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_SyncOpenEvent (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS     statusOfSetup    = DSP_SOK   ;
    DSP_STATUS     status           = DSP_SOK   ;
    SyncEvObject * evObj            = NULL      ;
    SyncAttrs      syncAttr                     ;

    DBC_Require (argc == 2) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 2) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_SyncOpenEvent. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = SYNC_Initialize () ;
    }

    /*  --------------------------------------------------------------------
     *  Tests SYNC_OpenEvent.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        status = SYNC_OpenEvent (&evObj, &syncAttr) ;
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    SYNC_CloseEvent (evObj) ;
    SYNC_Finalize () ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
