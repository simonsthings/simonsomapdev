/** ============================================================================
 *  @file   BVR_DoubleLoadImage.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implementation of behavior test to test loading two base
 *          images on DSP.
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
#include <dsplink.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- Processor Manager             */
#include <proc.h>
#include <chnl.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <BVR_DoubleLoadImage.h>
#include <BVR_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */

/** ============================================================================
 *  @func   BVR_DoubleLoadImage
 *
 *  @desc   Function to test behavior when two executable images are loaded
 *          on DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
BVR_DoubleLoadImage (IN  Uint32 argc, IN Pstr * argv)
{
    DSP_STATUS  status        = DSP_SOK ;
    DSP_STATUS  statusOfSetup = DSP_SOK ;
    ProcessorId dspId         = 0       ;
    Pstr        baseImage1    = NULL    ;
    Pstr        baseImage2    = NULL    ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 5) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 5) && (argv != NULL)) {
        statusOfSetup = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_DoubleLoadImage. "
                      "Status = [0x%x]", statusOfSetup) ;
    }
    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId      = TST_StringToInt (argv [2]) ;
        baseImage1 = argv [3]                   ;
        baseImage2 = argv [4]                   ;

        status = PROC_Setup () ;

        if (DSP_SUCCEEDED (status)) {
            status = PROC_Attach (dspId, NULL) ;
        }

        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to attach. Status = [0x%x]", status) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Test BVR_DoubleLoadImage.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        status = PROC_Load (dspId, baseImage1, 0, NULL) ;

        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to load first base image. Status = [0x%x]",
                          status) ;
        }
        else {
            status = PROC_Load (dspId, baseImage2, 0, NULL) ;
        }

        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to load second base image. Status = [0x%x]",
                          status) ;
        }
        else {
            status = PROC_Detach (dspId) ;
        }

        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to detach from DSP. Status = [0x%x]",
                          status) ;
        }
    }

    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;


    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
