/** ============================================================================
 *  @file   OSAL_DpcFinalize.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of DPC_Finalize API test case.
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
#include <dpc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <OSAL_PostProcess.h>
#include <OSAL_DpcFinalize.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   OSAL_DpcFinalize
 *
 *  @desc   Tests the DPC_Finalize API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_DpcFinalize  (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    statusOfSetup    = DSP_SOK   ;
    DSP_STATUS    status           = DSP_SOK   ;

    DBC_Require (argc == 2) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 2) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_DpcFinalize. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = DPC_Initialize () ;
    }

    /*  --------------------------------------------------------------------
     *  Tests DPC_Finalize.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        status = DPC_Finalize () ;
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
