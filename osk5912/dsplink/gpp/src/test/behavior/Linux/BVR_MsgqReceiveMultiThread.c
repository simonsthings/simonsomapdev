/** ============================================================================
 *  @file   BVR_MsgqReceiveMultiThread.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior\Linux
 *
 *  @desc   Implemetation of BVR_MsgqReceiveMultiThread test case.
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

/*  ----------------------------------- Processor Manager             */
#include <proc.h>
#include <chnl.h>

/*  ----------------------------------- MSGQ Component                */
#include <msgqdefs.h>
#include <msgq.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <BVR_PostProcess.h>
#include <BVR_MsgqReceiveMultiThread.h>
#include <BVR_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 6

/** ============================================================================
 *  @macro  NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 6

/** ============================================================================
 *  @const  DSP_MSGQ_ID
 *
 *  @desc   ID of the MSGQ to be created on the DSP.
 *  ============================================================================
 */
#define DSP_MSGQ_ID 0

/** ============================================================================
 *  @const  GPP_MSGQ_ID
 *
 *  @desc   ID of the MSGQ to be created on the GPP.
 *  ============================================================================
 */
#define GPP_MSGQ_ID 0

/** ============================================================================
 *  @name   MAX_THREADS
 *
 *  @desc   Maximum no of threads which can connect to the processor.
 *  ============================================================================
 */
#define MAX_THREADS    15


/** ----------------------------------------------------------------------------
 *  @func   DspReceiveFxn
 *
 *  @desc   Function that is spawned as a thread. Each thread creates a queue
 *          and recieves messages from the DSP.
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
DspReceiveFxn (Void * arguments) ;


/** ============================================================================
 *  @func   BVR_MsgqReceiveMultiThread
 *
 *  @desc   The BVR_MsgqReceiveMultiThread test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqReceiveMultiThread (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status          = DSP_SOK     ;
    DSP_STATUS      statusOfSetup   = DSP_SOK     ;
    DSP_STATUS      statusOfCleanup = DSP_SOK     ;
    ProcessorId     procId          = 0           ;
    MsgQueueId      msgqId          = GPP_MSGQ_ID ;
    Uint16          numMsg          = 1           ;
    Uint16          totalNumMsg     = 1           ;
    Uint16          numThreads      = 1           ;
    Char8 **        dspArgs         = NULL        ;

    ArgsToThread arguments [MAX_THREADS]        ;
    pthread_t    threadIdentifier [MAX_THREADS] ;
    Uint16       i                              ;
    Uint16       maxMsgSize                     ;


    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId      = TST_StringToInt (argv [2]) ;
        numThreads  = TST_StringToInt (argv [3]) ;
        numMsg      = TST_StringToInt (argv [4]) ;
        numThreads  = (numThreads > MAX_THREADS) ? MAX_THREADS : numThreads ;
        totalNumMsg = numMsg * numThreads ;
        maxMsgSize  = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = BVR_CreateDspArgumentList (NUM_DSP_ARGS,
                                             totalNumMsg,
                                             maxMsgSize,
                                             numThreads,
                                             DSP_MSGQ_ID,
                                             GPP_MSGQ_ID,
                                             numMsg) ;

        statusOfSetup = BVR_SetupMessaging (procId,
                                            totalNumMsg,
                                            maxMsgSize,
                                            numThreads,
                                            argv [5],
                                            NUM_DSP_ARGS,
                                            dspArgs) ;

        BVR_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        /*  --------------------------------------------------------------------
         *  Test multi thread transfer from DSP to GPP.
         *  (GPP multi thread recieve)
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; i < numThreads ; i++) {
            arguments [i].procId = procId ;
            arguments [i].msgqId = msgqId ;
            arguments [i].numMsg = numMsg ;
            pthread_create(&(threadIdentifier [i]), NULL,
                           (Void *) DspReceiveFxn, (Void *) &(arguments[i])) ;
            msgqId++ ;
        }

        /* Now check for all statuses */
        for (i = 0 ; DSP_SUCCEEDED (status) && (i < numThreads) ; i++) {
            pthread_join (threadIdentifier [i], NULL) ;
            if (DSP_FAILED (arguments [i].status)) {
                status = arguments [i].status ;
            }
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        statusOfCleanup = BVR_CleanupMessaging (procId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = DSP_EFAIL ;
        }
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   DspReceiveFxn
 *
 *  @desc   Function that is spawned as a thread. Each thread creates a queue
 *          and recieves messages from the DSP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
DspReceiveFxn (Void * arguments)
{
    DSP_STATUS      status          = DSP_SOK ;
    DSP_STATUS      statusOfCleanup = DSP_SOK ;
    TestMsg *       testMsg         = NULL    ;
    MsgqMsg         msg             = NULL    ;
    ProcessorId     procId                    ;
    MsgQueueId      msgqId                    ;
    Uint16          numMsg                    ;
    ArgsToThread *  args                      ;
    Uint16          i                         ;
    Uint16          j                         ;

    args   = (ArgsToThread *) arguments ;
    procId = args->procId ;
    msgqId = args->msgqId ;
    numMsg = args->numMsg ;

        status = MSGQ_Create (msgqId, NULL) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("MSGQ_Create failed. Status = [0x%x]", status) ;
        }

    for (i = 0 ; ((i < numMsg) && DSP_SUCCEEDED (status)) ; i++) {
        status = MSGQ_Get (msgqId, WAIT_FOREVER, &msg) ;

        if (DSP_SUCCEEDED (status)) {
            testMsg = (TestMsg *) msg ;
            for (j = 0 ;
                 (j < TST_MSQ_DATA_SIZE) && DSP_SUCCEEDED (status) ;
                 j++) {
                if (testMsg->buffer[j] != (char) j) {
                    status = DSP_EFAIL ;
                    TST_PrnError ("Failed data verification. Status = [0x%x]",
                                  status) ;
                }
            }

            statusOfCleanup = MSGQ_Free (msg) ;
            if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
                status = statusOfCleanup ;
                TST_PrnError ("MSGQ_Free failed. Status = [0x%x]", status) ;
            }
        }
        else {
            TST_PrnError ("MSGQ_Get failed. Status = [0x%x]", status) ;
        }
    }

    statusOfCleanup = MSGQ_Delete (msgqId) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
        status = statusOfCleanup ;
        TST_PrnError ("MSGQ_Delete failed. Status = [0x%x]", status) ;
    }

    args->status = status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
