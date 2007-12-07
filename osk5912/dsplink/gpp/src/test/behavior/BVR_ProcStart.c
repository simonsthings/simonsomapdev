/** ============================================================================
 *  @file   BVR_ProcStart.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_ProcStart test case.
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

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_Sleep.h>
#include <TST_PrintFuncs.h>
#include <BVR_PostProcess.h>
#include <BVR_ProcStart.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   TEST_SLEEP_TIME
 *
 *  @desc   Sleep time for the test.
 *  ============================================================================
 */
#define TEST_SLEEP_TIME 10


/** ============================================================================
 *  @func   BVR_ProcStart
 *
 *  @desc   This test loads the DSP binary image and starts and then sleeps.
 *          The dsp then tries to send data to the GPP. It tests if the gpp
 *          side is stable enough to handel unwanted interrupts from DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_ProcStart (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS statusOfSetup    = DSP_SOK   ;
    DSP_STATUS status           = DSP_SOK   ;
    Uint32     processorId      = 0         ;
    Uint32     dummyArg                     ;

    DBC_Require (argc == 5)    ;
    DBC_Require (argv != NULL) ;

    if ((argc != 5) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_ProcStart. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        processorId = TST_StringToInt (argv [2]) ;

        /* Argument passed to Proc Load */

        dummyArg = TST_StringToInt (argv [4]) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Attach (processorId, NULL) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Load (processorId, argv [3], dummyArg, NULL) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests PROC_Start.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (processorId) ;
    }

    TST_Sleep (TEST_SLEEP_TIME) ;

    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    PROC_Stop (processorId) ;
    PROC_Detach (processorId) ;
    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
