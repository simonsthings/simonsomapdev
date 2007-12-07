/** ============================================================================
 *  @file   BVR_MultiProcessAttach.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior\Linux
 *
 *  @desc   Implementation of behavior test to open DSP from multiple processes.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
#include <BVR_MultiProcessAttach.h>
#include <BVR_PostProcess.h>
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MAX_PROCESSES
 *
 *  @desc   Maximum no of processes which can connect to the processor.
 *  ============================================================================
 */
#define MAX_PROCESSES    16


/** ============================================================================
 *  @func   DspAttachFxn
 *
 *  @desc   Function that is spawned as a process.
 *
 *  @arg    dspId
 *              Count of the no. of arguments passed.
 *  @arg    argv
 *              List of arguments.
 *
 *  @ret    0
 *              Operation Successfully completed.
 *          1
 *              Error in execution of test.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
STATIC
int
DspAttachFxn (ProcessorId  dspId) ;


/** ============================================================================
 *  @func   BVR_MultiProcessAttach
 *
 *  @desc   Function to test the behavior when the DSP is attached
 *          from multiple processes.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MultiProcessAttach (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS   status        = DSP_SOK           ;
    DSP_STATUS   statusOfSetup = DSP_SOK           ;
    Uint32       numProcesses  = 0                 ;
    ProcessorId  dspId         = 0                 ;
    int          childId       = 1                 ;
    Uint32       i             = 0                 ;
    int          processIdentifier [MAX_PROCESSES] ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 4) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 4) && (argv != NULL)) {
        statusOfSetup = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_MultiProcessAttach. "
                      "Status = [0x%x]", statusOfSetup) ;
    }

    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId    = TST_StringToInt (argv [2]) ;
        numProcesses = TST_StringToInt (argv [3]) ;
    }

    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = PROC_Setup () ;
    }

    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = PROC_Attach (dspId, NULL) ;
    }

    /*  --------------------------------------------------------------------
     *  Test BVR_MultiProcessAttach.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        for (i = 0 ; (i < numProcesses) && (childId > 0) ; i++) {
            childId = fork() ;
        }
        if (childId < 0) {
            status = DSP_EFAIL ;
        }
    }

    if ((DSP_SUCCEEDED (status)) && (childId > 0)) {
            /* Now check for all status */
        for (i = 0 ; DSP_SUCCEEDED (status) && (i < numProcesses) ; i++) {
            wait (&(processIdentifier[i])) ;
            if (WEXITSTATUS (processIdentifier[i]) == 0) {
                status = DSP_EFAIL ;
            }
        }

    }
    else if (DSP_SUCCEEDED (status)) {
        processIdentifier[i] = DspAttachFxn (dspId) ;
        exit (processIdentifier[i]) ;
    }
    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    PROC_Detach (dspId) ;
    PROC_Destroy ()     ;

    return status ;

}


/** ----------------------------------------------------------------------------
 *  @func   DspAttachFxn
 *
 *  @desc   Function that is called from the created process.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
int
DspAttachFxn (ProcessorId  dspId)
{
    DSP_STATUS  status = DSP_SOK ;
    int retStatus      = 1       ;

    /* Expect the dspId as argv[0] */

    status = PROC_Attach (dspId, NULL) ;

    if (DSP_FAILED (status)) {
        TST_PrnError ("Attach Failed. Status = [0x%x]", status) ;
        retStatus = 0 ;
    }
    else {
        status = PROC_Detach (dspId) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Detach Failed. Status = [0x%x]", status) ;
            retStatus = 0 ;
        }
    }
    return retStatus ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
