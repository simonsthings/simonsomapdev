/** ============================================================================
 *  @file   BVR_MsgqSendMultiThread.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior\Linux
 *
 *  @desc   Implemetation of BVR_MsgqSendMultiThread test case.
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
#include <TST_Sleep.h>
#include <BVR_PostProcess.h>
#include <BVR_MsgqSendMultiThread.h>
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
 *  @const  MAX_GPP_MSGQS
 *
 *  @desc   Maximum number of MSGQs created on the GPP.
 *  ============================================================================
 */
#define MAX_GPP_MSGQS 1

/** ============================================================================
 *  @const  DSP_MSGQ_ID
 *
 *  @desc   ID of the MSGQ to be created on the DSP.
 *  ============================================================================
 */
#define DSP_MSGQ_ID 0

/** ============================================================================
 *  @name   MAX_THREADS
 *
 *  @desc   Maximum no of threads which can connect to the processor.
 *  ============================================================================
 */
#define MAX_THREADS    16

/** ============================================================================
 *  @name   DATA_VALIDATION_SUCCEEDED/DATA_VALIDATION_FAILED
 *
 *  @desc   Values returned by the DSP for the data validation report.
 *  ============================================================================
 */
#define VALIDATION 1

/** ============================================================================
 *  @name   DATA_VALIDATION_SUCCEEDED/DATA_VALIDATION_FAILED
 *
 *  @desc   Values returned by the DSP for the data validation report.
 *  ============================================================================
 */
#define DATA_VALIDATION_SUCCEEDED  1
#define DATA_VALIDATION_FAILED     0


/** ----------------------------------------------------------------------------
 *  @func   DspSendFxn
 *
 *  @desc   Function that is spawned as a thread. Each thread sends
 *          messages to the DSP.
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
DspSendFxn (Void * arguments) ;


/** ============================================================================
 *  @func   BVR_MsgqSendMultiThread
 *
 *  @desc   The BVR_MsgqSendMultiThread test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqSendMultiThread (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status          = DSP_SOK                  ;
    DSP_STATUS      statusOfSetup   = DSP_SOK                  ;
    DSP_STATUS      statusOfCleanup = DSP_SOK                  ;
    ProcessorId     procId          = 0                        ;
    MsgQueueId      msgqId          = 0                        ;
    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER}           ;
    ChannelId       readChnlId      = 1                        ;
    ChannelAttrs    chnlAttrs       = {Endianism_Default,
                                       ChannelMode_Input,
                                       ChannelDataSize_16bits} ;
    ChannelIOInfo   io_info         = {NULL,
                                       TST_REPORT_SIZE,
                                       0}                      ;
    Uint16          numMsg          = 1                        ;
    Uint32          timeOut         = 100                      ;
    Uint16          totalNumMsg     = 1                        ;
    Uint16          numThreads      = 1                        ;
    Char8 **        dspArgs         = NULL                     ;

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
                                             MAX_GPP_MSGQS,
                                             DSP_MSGQ_ID,
                                             0,    /* For this test, DSP doesn't
                                                    * expect a valid value at
                                                    * this position.
                                                    */
                                             totalNumMsg) ;

        statusOfSetup = BVR_SetupMessaging (procId,
                                            totalNumMsg,
                                            maxMsgSize,
                                            MAX_GPP_MSGQS,
                                            argv [5],
                                            NUM_DSP_ARGS,
                                            dspArgs) ;

        BVR_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_SUCCEEDED (status)) {
            status = DSP_ENOTFOUND ;
            while (status == DSP_ENOTFOUND) {
                status = MSGQ_Locate (procId,
                                         msgqId,
                                         &msgqLocateAttrs) ;
                if (DSP_FAILED (status) && status != DSP_ENOTFOUND) {
                TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                                  status) ;
                }
                else if (status == DSP_ENOTFOUND) {
                    TST_Sleep (timeOut) ;
                    timeOut *= 2 ;
                }
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = CHNL_Create (procId, readChnlId, &chnlAttrs) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("CHNL_Create failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = CHNL_AllocateBuffer (procId,
                                                 readChnlId,
                                                 &(io_info.buffer),
                                                 TST_REPORT_SIZE,
                                                 1) ;   /* Number of buffers */
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("CHNL_AllocateBuffer failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Test multi thread transfer from GPP to DSP.
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; i < numThreads ; i++) {
            arguments [i].procId = procId ;
            arguments [i].msgqId = msgqId ;
            arguments [i].numMsg = numMsg ;
            pthread_create(&(threadIdentifier [i]), NULL,
                           (Void *) DspSendFxn, (Void *) &(arguments[i])) ;
        }

        for (i = 0 ;
             DSP_SUCCEEDED (status) && (i < totalNumMsg) && VALIDATION ;
             i++ ) {
            /* Issue an empty buffer to read data and then reclaim it */
            status = CHNL_Issue (procId, readChnlId, &io_info) ;

            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (procId, readChnlId,
                                       WAIT_FOREVER, &io_info) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("CHNL_Reclaim failed. Status = [0x%x]",
                                  statusOfSetup) ;
                }
            }
            else {
                TST_PrnError ("CHNL_Issue Failed. Status = [0x%x]", status) ;
            }

            if (    DSP_SUCCEEDED (status)
                &&  (   *((Uint16 *) (io_info.buffer))
                     != DATA_VALIDATION_SUCCEEDED)) {
                status = DSP_EFAIL ;
                TST_PrnError ("Failed data verification. Status = [0x%x]",
                              status) ;
            }
        }

        /* Now check for all statuses */
        for (i = 0 ; i < numThreads ; i++) {
            pthread_join (threadIdentifier [i], NULL) ;
            if (DSP_FAILED (arguments [i].status) && DSP_SUCCEEDED (status)) {
                status = arguments [i].status ;
            }
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        statusOfCleanup = CHNL_FreeBuffer (procId, readChnlId,
                                           &(io_info.buffer), 1) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("CHNL_FreeBuffer Failed. Status = [0x%x]",
                          statusOfCleanup) ;
        }

        statusOfCleanup = CHNL_Delete (procId, readChnlId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("CHNL_Delete Failed. Status = [0x%x]",
                          statusOfCleanup) ;
        }

        statusOfCleanup = MSGQ_Release (procId, msgqId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("MSGQ_Release Failed. Status = [0x%x]",
                          statusOfCleanup) ;
        }

        statusOfCleanup = BVR_CleanupMessaging (procId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
        }
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   DspSendFxn
 *
 *  @desc   Function that is spawned as a thread. Each thread sends
 *          messages to the DSP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
DspSendFxn (Void * arguments)
{
    DSP_STATUS     status          = DSP_SOK ;
    DSP_STATUS     statusOfCleanup = DSP_SOK ;
    DSP_STATUS     tmpStatus       = DSP_SOK ;
    TestMsg *      testMsg         = NULL    ;
    MsgqMsg        msg             = NULL    ;
    ProcessorId    procId                    ;
    MsgQueueId     msgqId                    ;
    Uint16         numMsg                    ;
    ArgsToThread * args                      ;
    Uint16         i                         ;
    Uint16         j                         ;
    Uint16         msgId           = VALIDATION ;
    Uint16         msgBufferSize   = 0          ;

    args   = (ArgsToThread *) arguments ;
    procId = args->procId ;
    msgqId = args->msgqId ;
    numMsg = args->numMsg ;

    /*  ------------------------------------------------------------------------
     *  Tests transfer from GPP to DSP.
     *  ------------------------------------------------------------------------
     */
    msgBufferSize = TST_MSQ_DATA_SIZE ;
    msgBufferSize += ((ALIGN (sizeof (TestMsg))) - sizeof (TestMsg)) / 2 ;
    for (i = 0 ; ((i < numMsg) && DSP_SUCCEEDED (status)) ; i++) {
        status = MSGQ_Alloc (TST_MQA_ID, ALIGN (sizeof (TestMsg)), &msg) ;

        if (DSP_SUCCEEDED (status)) {
            testMsg = (TestMsg *) msg ;
            for (j = 0 ; j < msgBufferSize ; j++) {
                testMsg->buffer[j] = j ;
            }
        }
        else {
            TST_PrnError ("MSGQ_Alloc Failed. Status = [0x%x]", status) ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Put (procId, msgqId, msg, msgId, MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed. Status = [0x%x]", status) ;
            }
            tmpStatus = status ;
        }
    }

    if (DSP_FAILED (tmpStatus)) { /* Need to free only if last put failed */
        statusOfCleanup = MSGQ_Free (msg) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("MSGQ_Free Failed. Status = [0x%x]",
                          statusOfCleanup) ;
        }
    }

    args->status = status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
