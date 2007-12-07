/** ============================================================================
 *  @file   API_ProcStop.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of PROC_Stop API test case.
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
#include <TST_PrintFuncs.h>
#include <API_PostProcess.h>
#include <API_ProcStop.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   API_ProcStop
 *
 *  @desc   Tests the PROC_Stop API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_ProcStop (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS statusOfSetup    = DSP_SOK   ;
    DSP_STATUS status           = DSP_SOK   ;
    Uint32     processorId      = 0         ;
    Uint32     stopAgain        = 0         ;
    Uint32     dummyArg                     ;

    DBC_Require (argc == 6) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 6) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_ProcStop. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        processorId = TST_StringToInt (argv [2]) ;
        stopAgain   = TST_StringToInt (argv [5]) ;

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

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Start (processorId) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests PROC_Stop.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        if (stopAgain == 0) {
            status = PROC_Stop (processorId) ;
            if (DSP_SUCCEEDED (status)) {
                status = PROC_Stop (processorId) ;
            }
        }
        else {
            status = PROC_Stop (processorId) ;
        }
    }

    status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    PROC_Detach (processorId) ;
    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
