/** ============================================================================
 *  @file   STS_MsgqLocate.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress\Linux
 *
 *  @desc   Implementation of STS_MsgqLocate test case.
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
#include <platform.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- Link API                      */
#include <proc.h>
#include <chnl.h>
#include <msgqdefs.h>
#include <msgq.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <TST_Sleep.h>
#include <STS_MsgqLocate.h>
#include <STS_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 4

/** ============================================================================
 *  @macro  NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 6

/** ============================================================================
 *  @name   MAX_THREADS
 *
 *  @desc   Maximum no of threads which can connect to the processor.
 *  ============================================================================
 */
#define MAX_THREADS    16

/** ============================================================================
 *  @name   NUM_LOCATES_PER_THREAD
 *
 *  @desc   Number of locates to be performed by each thread.
 *  ============================================================================
 */
#define NUM_LOCATES_PER_THREAD    8


/** ----------------------------------------------------------------------------
 *  @func   DspLocateFxn
 *
 *  @desc   Function that is spawned as a thread. Each of the thread tries
 *          to locate a queue on the DSP.
 *
 *  @arg    arguments
 *              All the required arguments packed into the ArgsToThread struct.
 *
 *  @ret    None
 *
 *  @enter  arguments should not be NULL
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
DspLocateFxn (Void * arguments) ;


/** ============================================================================
 *  @func   STS_MsgqLocate
 *
 *  @desc   The STS_MsgqLocate test forks multiple threads, each thread
 *          tries to locate the same queue on the DSP side. This test
 *          tries to check reentrancy and leaks in the locate mechanism.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_MsgqLocate (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status          = DSP_SOK          ;
    DSP_STATUS      expectedStatus  = DSP_SOK          ;

    ProcessorId     procId          = 0                ;
    Uint16          numThreads      = 1                ;
    MsgQueueId      msgqDspId       = 0                ;
    Uint16          numMsg          = 1                ;
    Uint16          maxMsgSize      = 0              ;
    Uint16          numMsgPerPool   = 0              ;
    Uint16          maxLocalQueues  = 1              ;
    MsgqMsg         msg             = NULL             ;
    Uint16          msgId           = 0                ;
    Char8 **        dspArgs         = NULL             ;
    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER}   ;

    ArgsToThread arguments [MAX_THREADS]        ;
    pthread_t    threadIdentifier [MAX_THREADS] ;
    Uint16       i                              ;


    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }
    else {
        TST_PrnInfo ("This test needs visual inspection also.") ;
        TST_PrnInfo ("Please enable trace prints for ID_OSAL_MEM and set "
                     "severity level to 4\n") ;

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        status      = TST_StrToStatus (argv [0], &expectedStatus) ;
        procId      = TST_StringToInt (argv [1]) ;
        numThreads  = TST_StringToInt (argv [2]) ;
        numThreads  = (numThreads > MAX_THREADS) ? MAX_THREADS : numThreads ;
        maxMsgSize    = RMQT_CTRLMSG_SIZE ;
        numMsgPerPool = numThreads * 4 ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = STS_CreateDspArgumentList (NUM_DSP_ARGS,
                                             numMsgPerPool,
                                             maxMsgSize,
                                             maxLocalQueues,
                                             msgqDspId,
                                             MSGQ_INVALID_ID,
                                             numMsg) ;

        status = STS_SetupMessaging (procId,
                                     numMsgPerPool,
                                     maxMsgSize,
                                     maxLocalQueues,
                                     argv [3],
                                     NUM_DSP_ARGS,
                                     dspArgs) ;

        STS_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to setup MSGQ. Status = [0x%x]",
                          status) ;
        }

        /*  --------------------------------------------------------------------
         *  Test multiple locates
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; (i < numThreads) && DSP_SUCCEEDED (status); i++) {
            arguments [i].procId = procId    ;
            arguments [i].msgqId = msgqDspId ;
            arguments [i].numMsg = numMsg    ;
            arguments [i].status = DSP_SOK   ;
            pthread_create(&(threadIdentifier [i]), NULL,
                           (Void *) DspLocateFxn, (Void *) &(arguments[i])) ;
        }

        /* Now check for all statuses */
        for (i = 0 ; (i < numThreads) ; i++) {
            pthread_join (threadIdentifier [i], NULL) ;
            if (DSP_FAILED (arguments [i].status) && DSP_SUCCEEDED (status)) {
                status = arguments [i].status ;
                TST_PrnError ("One of the threads Failed with status = [0x%x]",
                              status) ;
            }
        }

        /*
         *  Shutdown the DSP side by sending a message.
         */
        status = MSGQ_Locate (procId, msgqDspId, &msgqLocateAttrs) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                          status) ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Alloc (TST_MQA_ID, maxMsgSize, &msg) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Put (procId, msgqDspId, msg, msgId, MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Release (procId, msgqDspId) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Release failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = STS_CleanupMessaging (procId) ;
        }

        if (status != expectedStatus) {
            status = DSP_EFAIL ;
        }
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   DspLocateFxn
 *
 *  @desc   Function that is spawned as a thread. Each of the thread tries
 *          to locate a queue on the DSP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
DspLocateFxn (Void * arguments)
{
    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER} ;
    DSP_STATUS      tmpStatus       = DSP_SOK        ;

    ProcessorId     procId ;
    MsgQueueId      msgqId ;
    ArgsToThread *  args   ;
    Uint16          i      ;
    Uint32          timeOut ;

    args = (ArgsToThread *) arguments ;
    procId = args->procId ;
    msgqId = args->msgqId ;

    /*  --------------------------------------------------------------------
     *  Locate.
     *  --------------------------------------------------------------------
     */
    for (i = 0 ;
         (i < NUM_LOCATES_PER_THREAD) && DSP_SUCCEEDED (args->status) ;
         i++) {
        timeOut = 100 ;
        args->status = DSP_ENOTFOUND ;
        while (args->status == DSP_ENOTFOUND) {
        args->status = MSGQ_Locate (procId, msgqId, &msgqLocateAttrs) ;
            TST_Sleep (timeOut) ;
            timeOut *= 2 ;
        }
        if (DSP_FAILED (args->status)) {
            TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                          args->status) ;
        }
        else {
            tmpStatus = MSGQ_Release (procId, msgqId) ;
            if (DSP_FAILED (tmpStatus)) {
                TST_PrnInfo ("MSGQ_Locate failed. Status = [0x%x]",
                              tmpStatus) ;
    }
        }
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
