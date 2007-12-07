/** ============================================================================
 *  @file   OSAL_SyncWaitOnEvent.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of SYNC_WaitOnEvent API test case.
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
#include <OSAL_SyncWaitOnEvent.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   OSAL_SyncWaitOnEvent
 *
 *  @desc   Tests the SYNC_WaitOnEvent API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_SyncWaitOnEvent (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS     statusOfSetup    = DSP_SOK   ;
    DSP_STATUS     status           = DSP_SOK   ;
    Int32          nullTest         = 0         ;
    Uint32         timeOut          = 0         ;
    SyncEvObject * evObj            = NULL      ;
    SyncAttrs      syncAttr                     ;

    DBC_Require (argc == 4) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 4) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_SyncWaitOnEvent. "
                      "Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        timeOut = TST_StringToInt (argv [2]) ;
        nullTest = TST_StringToInt (argv [3]) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = SYNC_Initialize () ;

        if (DSP_SUCCEEDED (status)) {
            statusOfSetup = SYNC_OpenEvent (&evObj, &syncAttr) ;
        }

        if (DSP_SUCCEEDED (status)) {
            statusOfSetup = SYNC_SetEvent (evObj) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests SYNC_WaitOnEvent.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        if (nullTest == 1) {
            status = SYNC_WaitOnEvent (NULL, timeOut) ;
        }
        else {
            status = SYNC_WaitOnEvent (evObj, timeOut) ;
        }
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    SYNC_CloseEvent (evObj) ;
    SYNC_Finalize () ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
