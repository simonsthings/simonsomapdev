/** ============================================================================
 *  @file   ANA_MsgqPutGet.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of testcase that measures the throughput of the
 *          message transfers to / from DSP depending upon the mode of
 *          operation.
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
#include <ANA_MsgqPutGet.h>
#include <TST_Analysis.h>
#include <ANA_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 7

/** ============================================================================
 *  @name   BATCH_SIZE
 *
 *  @desc   The number of of messages to be transferred in one batch.
 *  ============================================================================
 */
#define BATCH_SIZE 25

/** ============================================================================
 *  @name   NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 7

/** ============================================================================
 *  @func   ANA_MsgqPutGet
 *
 *  @desc   Function to test the performance of Messaging using MGSQ_Put,
 *          MSGQ_Get (). The mode of operation of this test comes from data
 *          file. This test synchronizes itself with dsp side test after
 *          MSGQ_Locate, during message transfers (after transfer of one batch
 *          of buffers) and before cleanup.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ANA_MsgqPutGet (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS       status          = DSP_SOK          ;
    DSP_STATUS       tmpStatus       = DSP_SOK          ;
    ProcessorId      procId          = 0                ;
    MsgQueueId       msgqInId        = 1                ;
    MsgQueueId       msgqOutId       = 1                ;
    Uint32           stopTime        = 0                ;
    Char8 *          dspExecutable   = NULL             ;
    Uint32           msgSize         = 0                ;
    Uint32           count           = 0                ;
    Uint32           i               = 0                ;
    Uint32           timeOut         = 100              ;
    Uint32           iterations      = 0                ;
    Uint32           modeOfOperation = 0                ;
    Uint16           maxLocalQueues  = 0                ;
    Uint16           numMsgPerPool   = 0                ;
    MsgqLocateAttrs  msgqLocateAttrs = {WAIT_FOREVER}   ;
    MsgqMsgHeader *  msg             = NULL             ;
    Uint16           maxMsgSize      = sizeof (TestMsg) ;
    Uint16           msgId           = 0                ;
    Char8 **         dspArgs         = NULL             ;

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
        procId           = TST_StringToInt (argv [0])  ;
        msgqInId         = TST_StringToInt (argv [1])  ;
        msgqOutId        = TST_StringToInt (argv [2])  ;
        dspExecutable    = argv [3]                    ;
        msgSize          = ALIGN (TST_StringToInt (argv [4]))  ;
        iterations       = TST_StringToInt (argv [5])  ;
        modeOfOperation  = TST_StringToInt (argv [6])  ;

        /*  --------------------------------------------------------------------
         *  The assumption here is that msgSize will be greater than
         *  RMQT_CTRLMSG_SIZE.
         *  --------------------------------------------------------------------
         */
        maxMsgSize =  msgSize ;

        maxLocalQueues = msgqInId + 1 ;
        numMsgPerPool =  BATCH_SIZE + 1 ;

        /*  --------------------------------------------------------------------
         *  Setup phase of the application.
         *  --------------------------------------------------------------------
         */
        dspArgs = ANA_CreateDspArgumentList (NUM_DSP_ARGS,
                                             numMsgPerPool,
                                             maxMsgSize,
                                             maxLocalQueues,
                                             msgqOutId,
                                             msgqInId,
                                             iterations,
                                             modeOfOperation) ;

        status = ANA_SetupMessaging (procId,
                                     numMsgPerPool,
                                     maxMsgSize,
                                     maxLocalQueues,
                                     dspExecutable,
                                     NUM_DSP_ARGS,
                                     dspArgs) ;

        ANA_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Create (msgqInId, NULL) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Create failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = DSP_ENOTFOUND ;
            while (status == DSP_ENOTFOUND) {
                status = MSGQ_Locate (procId, msgqOutId, &msgqLocateAttrs) ;
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

        /*  --------------------------------------------------------------------
         *  Waiting for the dsp to complete the locate and then sending the
         *  message to indicate that this side has completed the locate.
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Get (msgqInId, WAIT_FOREVER, &msg) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Get failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Put (procId, msgqOutId, msg, msgId, msgqInId) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                              status) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Execute phase of the application.
         *
         *  If modeOfOperation == 1 then continuously receive messages from DSP
         *  side.
         *
         *  If modeOfOperation == 2 then continuously send messages to DSP
         *  side.
         *
         *  If modeOfOperation == 3 then continuously send and receive messages
         *  to DSP side.
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            TST_ZeroTime () ;
        }

        /*  --------------------------------------------------------------------
         *  Send / receive messages to DSP depending upon the mode of operation.
         *  --------------------------------------------------------------------
         */
        for (count = 0 ; (DSP_SUCCEEDED (status)) && (count < iterations) ;
             count += BATCH_SIZE) {
            for (i = 0 ; (DSP_SUCCEEDED (status)) && (i < BATCH_SIZE) ; i++) {
                if (modeOfOperation == 2 || modeOfOperation == 3) {
                    status = MSGQ_Alloc (TST_MQA_ID, msgSize, &msg) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                                      status) ;
                    }
                    else {
                        status = MSGQ_Put (procId,
                                           msgqOutId,
                                           msg,
                                           msgId,
                                           msgqInId) ;
                        if (DSP_FAILED (status)) {
                            TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                                          status) ;
                        }
                    }
                }

                if (   (DSP_SUCCEEDED (status))
                    && (   modeOfOperation == 1
                        || modeOfOperation == 3)) {
                    status = MSGQ_Get (msgqInId, WAIT_FOREVER, &msg) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("MSGQ_Get failed. Status = [0x%x]",
                                      status) ;
                    }
                    else {
                        status = MSGQ_Free (msg) ;
                        if (DSP_FAILED (status)) {
                            TST_PrnError ("MSGQ_Free failed. Status = [0x%x]",
                                          status) ;
                        }
                    }
                }
            }
            /*  ----------------------------------------------------------------
             *  Synchronize with the DSP side.
             *  ----------------------------------------------------------------
             */
            if (DSP_SUCCEEDED (status)) {
                if (modeOfOperation == 1) {
                    status = MSGQ_Alloc (TST_MQA_ID, msgSize, &msg) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                                      status) ;
                    }
                    else {
                        status = MSGQ_Put (procId,
                                           msgqOutId,
                                           msg,
                                           msgId,
                                           msgqInId) ;
                        if (DSP_FAILED (status)) {
                            TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                                          status) ;
                        }
                    }
                }
                else if (modeOfOperation == 2) {
                    status = MSGQ_Get (msgqInId, WAIT_FOREVER, &msg) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("MSGQ_Get failed. Status = [0x%x]",
                                      status) ;
                    }
                    else {
                        status = MSGQ_Free (msg) ;
                        if (DSP_FAILED (status)) {
                            TST_PrnError ("MSGQ_Free failed. Status = [0x%x]",
                                          status) ;
                        }
                    }
                }
            }
        }

        if (DSP_SUCCEEDED (status)) {
            stopTime = TST_GetTime () ;
        }

        /*  --------------------------------------------------------------------
         *  Do the analysis of the test.
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            if (modeOfOperation == 3) {
                iterations *= 2 ;
            }
            status = TST_DoAnalysis (0, stopTime, iterations , msgSize) ;
        }

        /*  --------------------------------------------------------------------
         *  Synchronizing with the DSP side for cleanup.
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Alloc (TST_MQA_ID, msgSize, &msg) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                              status) ;
            }
            else {
                status = MSGQ_Put (procId, msgqOutId, msg, msgId, msgqInId) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                                  status) ;
                }
            }
        }

        /*  --------------------------------------------------------------------
         *  Cleanup phase of the application.
         *  --------------------------------------------------------------------
         */
        tmpStatus = MSGQ_Delete (msgqInId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
            status = tmpStatus ;
            TST_PrnError ("Failed on MSGQ_Delete. Status = [0x%x]",
                          status) ;
        }

        tmpStatus = MSGQ_Release (procId, msgqOutId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
            status = tmpStatus ;
            TST_PrnError ("Failed on MSGQ_Release. Status = [0x%x]",
                          status) ;
        }

        tmpStatus = ANA_CleanupMessaging (procId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
            status = tmpStatus ;
        }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
