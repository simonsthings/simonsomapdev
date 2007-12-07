/** ============================================================================
 *  @file   OSAL_DpcInitialize.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of DPC_Initialize API test case.
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
#include <OSAL_DpcInitialize.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   OSAL_DpcInitialize
 *
 *  @desc   Tests the DPC_Initialize API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_DpcInitialize (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status           = DSP_SOK   ;
    DSP_STATUS    statusOfSetup    = DSP_SOK   ;


    DBC_Require (argc == 2) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 2) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_DpcInitialize. Status = [0x%x]",
                      status) ;
    }

    /*  --------------------------------------------------------------------
     *  Tests DPC_Initialize.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        status = DPC_Initialize () ;
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    DPC_Finalize () ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
