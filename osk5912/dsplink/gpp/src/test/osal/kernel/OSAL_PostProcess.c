/** ============================================================================
 *  @file   OSAL_PostProcess.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of OSAL_PostProcess function.
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

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   OSAL_PostProcess
 *
 *  @desc   This function compares the status of setup with expected status of
 *          setup and status of test with expected status of test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_PostProcess (IN DSP_STATUS statusOfTest,
                  IN Char8 * expStatusTest,
                  IN DSP_STATUS statusOfSetup,
                  IN Char8 * expStatusSetup)
{
    DSP_STATUS expStatusOfSetup  = DSP_SOK ;
    DSP_STATUS expStatusOfTest   = DSP_SOK ;
    DSP_STATUS tempStatus        = DSP_SOK ;
    DSP_STATUS status            = DSP_SOK ;

    DBC_Require (expStatusTest != NULL) ;
    DBC_Require (expStatusSetup != NULL) ;

    tempStatus = TST_StrToStatus (expStatusTest, &expStatusOfTest) ;

    if (DSP_SUCCEEDED (tempStatus)) {
        tempStatus = TST_StrToStatus (expStatusSetup, &expStatusOfSetup) ;
    }
    if (DSP_SUCCEEDED (tempStatus)) {
        if (   (statusOfTest == expStatusOfTest)
            && (statusOfSetup == expStatusOfSetup)) {
            status = DSP_SOK ;
        }
        else {
            status = DSP_EFAIL ;
        }
    }

    if (DSP_FAILED (tempStatus)) {
        status = DSP_EFAIL ;
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
