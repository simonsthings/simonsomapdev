/** ============================================================================
 *  @file   BVR_MultiThreadOpen.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior\Linux
 *
 *  @desc   Implementation of behavior test to open DSP from multiple threads.
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
#include <pthread.h>
#include <stdlib.h>

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
#include <BVR_MultiThreadOpen.h>
#include <BVR_PostProcess.h>
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MAX_THREADS
 *
 *  @desc   Maximum no of threads which can connect to the processor.
 *  ============================================================================
 */
#define MAX_THREADS    16


/** ============================================================================
 *  @name   ArrayStatus
 *
 *  @desc   Pointer of the status which is to be updated by the threads to
 *          be created.
 *  ============================================================================
 */
DSP_STATUS  ArrayStatus [MAX_THREADS] ;

/** ----------------------------------------------------------------------------
 *  @func   DspAttachFxn
 *
 *  @desc   Function that is spawned as a thread.
 *
 *  @modif  ArrayStatus
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
DspAttachFxn (Void * arguments) ;


/** ============================================================================
 *  @name   ArgsToThread
 *
 *  @desc   The information is passed to each thread created.
 *
 *  @field  dspId
 *              Id of the processor which is to be attached.
 *  @field  statusToBeUpdated
 *              Pointer of the status which is to be updated by the threads to
 *              be created.
 *  ============================================================================
 */
typedef struct ArgsToThread_tag {
    Uint32       dspId               ;
    Uint32       identifierOfThread  ;
} ArgsToThread ;


/** ============================================================================
 *  @func   BVR_MultiThreadOpen
 *
 *  @desc   Function to test the behavior when the DSP is attached
 *          from multiple threads.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MultiThreadOpen (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS   status        = DSP_SOK        ;
    DSP_STATUS   statusOfSetup = DSP_SOK        ;
    Uint32       numThreads      = 0              ;
    ProcessorId  dspId         = 0              ;
    Uint32       i                              ;
    ArgsToThread arguments [MAX_THREADS]        ;
    pthread_t    threadIdentifier [MAX_THREADS] ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 4) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 4) && (argv != NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }

    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId    = TST_StringToInt (argv [2]) ;
        numThreads = TST_StringToInt (argv [3]) ;
    }

    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = PROC_Setup () ;
    }

    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = PROC_Attach (dspId, NULL) ;
    }

    /*  --------------------------------------------------------------------
     *  Test BVR_MultiThreadOpen.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        for (i = 0 ; i < numThreads ; i++) {
            arguments [i].dspId = dspId          ;
            arguments [i].identifierOfThread = i ;
            pthread_create(&(threadIdentifier [i]), NULL,
                           (Void *) DspAttachFxn, (Void *) &(arguments[i]));
       }
        /* Now check for all statuses */
        for (i = 0 ; DSP_SUCCEEDED (status) && (i < numThreads) ; i++) {
            pthread_join (threadIdentifier [i], NULL) ;
            if (ArrayStatus [i] != DSP_EALREADYCONNECTED) {
                status = DSP_EFAIL ;
            }
        }
    }

    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    PROC_Detach (dspId) ;
    PROC_Destroy ()     ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   DspAttachFxn
 *
 *  @desc   Function that is spawned as a thread.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
DspAttachFxn (Void * arguments)
{
    DSP_STATUS  status = DSP_SOK ;
    ProcessorId dspId            ;

    /* Expect the dspId as argv[0] */
    dspId = ((ArgsToThread *) arguments)->dspId ;

    status = PROC_Attach (dspId, NULL) ;
    if (DSP_FAILED (status)) {
        TST_PrnInfo ("Attach Failed") ;
    }
    else {
        status = PROC_Detach (dspId) ;
        if (DSP_FAILED (status)) {
            TST_PrnInfo ("Detach Failed") ;
        }
    }

    /* Update the status in global area */
    ArrayStatus [((ArgsToThread *) arguments)->identifierOfThread]= status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
