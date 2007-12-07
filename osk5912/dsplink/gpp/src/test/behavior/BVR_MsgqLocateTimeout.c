/** ============================================================================
 *  @file   BVR_MsgqLocateTimeout.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_MsgqLocateTimeout test case.
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
#include <BVR_PostProcess.h>
#include <BVR_MsgqLocateTimeout.h>
#include <BVR_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 5

/** ============================================================================
 *  @name   NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 5

/** ============================================================================
 *  @name   TST_LOCATE_TIMEOUT
 *
 *  @desc   Timeout for the locate requests.
 *  ============================================================================
 */
#define TST_LOCATE_TIMEOUT 100

/** ============================================================================
 *  @name   GPP_CONTROL_MSGQ_ID
 *
 *  @desc   Msgq Id on GPP
 *  ============================================================================
 */
#define GPP_CONTROL_MSGQ_ID             0

/** ============================================================================
 *  @name   DSP_CONTROL_MSGQ_ID
 *
 *  @desc   Msgq Id on DSP
 *  ============================================================================
 */
#define DSP_CONTROL_MSGQ_ID             0

/** ============================================================================
 *  @name   NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages per pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL      5


/** ============================================================================
 *  @func   BVR_MsgqLocateTimeout
 *
 *  @desc   Function to test the behavior of MSGQ_Locate () with finite
 *          timeout.
 *
 *          This test tests the behavior of MSGQ_Locate with finite timeout.
 *          The GPP side sends a control message with a delay value to
 *          the DSP. The DSP then sleeps for the delay period and sends
 *          the message back to the GPP. The GPP in the mean time tries
 *          to get the message twice with finite timeout values specified
 *          in the data file.
 *
 *                     GPP                                       DSP
 *
 *          MSGQ_Create (queue1)            | |
 *                                          | |
 *          MSGQ_Get (queue1 )              | | MSGQ_Create (queue2)
 *          (infinite timeout)              | |
 *          Get the delay value from        | | MSGQ_Locate(queue1)
 *            the message and sleep.        | |   (infinite timeout)
 *                                          | |   Locate is successfull here.
 *                                          | |
 *                                          | | MSGQ_Put (queue2)(infinite
 *                                          | |                   timeout)
 *                                          | |
 *           Wakes up from sleep here.      | | MSGQ_Get (queue1) (infinite
 *                                          | |                    timeout)
 *                                          | | Get the delay value from
 *                                          | | the message and sleep.
 *                                          | |
 *          MSGQ_Locate (infinite timeout)  | |
 *              (queue2)                    | |
 *            Locate is successfull here.   | |
 *                                          | |
 *          MSGQ_Locate (infinite timeout)  | |
 *              (testqueue)                 | |
 *           Locate fails here.             | |
 *                                          | |
 *           MSGQ_Put (queue2 )             | |
 *                                          | | Wakes up from sleep here.
 *                                          | |
 *                                          | | MSGQ_Create (testqueue)
 *           MSGQ_Get (queue1 )             | |
 *           Get the delay value from       | |
 *           the message and sleep.         | |
 *                                          | | MSGQ_Put (queue1)
 *                                          | |
 *           Wakes up from sleep here.      | |
 *                                          | |
 *           MSGQ_Locate (infinite timeout) | |
 *              (testqueue)                 | | MSGQ_Get (queue2)
 *           Locate is successfull here.    | | Get the delay value from
 *                                          | | the message and sleep.
 *           MSGQ_Put (queue2 )             | |
 *                                          | |
 *           MSGQ_Get (queue1 )             | | Wakes up from sleep here.
 *           Get the delay value from       | |
 *                                          | |
 *                                          | | MSGQ_Delete (testqueue)
 *                                          | |
 *                                          | | MSGQ_Put (queue1)
 *                                          | |
 *           Wakes up from sleep here.      | |
 *                                          | | MSGQ_Get (queue2)
 *           MSGQ_Locate (infinite timeout) | | Get the delay value from
 *                (testqueue)               | | the message and sleep.
 *             Locate fails here.           | |
 *                                          | |
 *           MSGQ_Put (queue2)              | |
 *                                          | | Wakes up from sleep here.
 *           MSGQ_Delete (queue 1)          | |
 *                                          | | MSGQ_Delete (queue2)
 *                                          | |
 *                                          | |
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqLocateTimeout (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup   = DSP_SOK        ;
    DSP_STATUS      statusOfCleanup = DSP_SOK        ;
    DSP_STATUS      status          = DSP_SOK ;

    ProcessorId     procId          = 0              ;
    MsgQueueId      testMsgqId      = 0    ;
    Char8 *         dspExecutable   = NULL           ;

    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER}   ;
    TestMsg *       msg             = NULL             ;
    Uint16          maxMsgSize      = 0              ;
    Uint16          msgId           = 0                ;
    Uint16          maxLocalQueues  = MAX_MSGQS      ;
    Char8 **        dspArgs         = NULL             ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]",
                      status) ;
    }
    else {
        procId        = TST_StringToInt (argv [2])  ;
        testMsgqId    = TST_StringToInt (argv [3])  ;
        dspExecutable = argv [4]                    ;

        maxMsgSize     = ALIGN (sizeof (TestMsg))    ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        dspArgs = BVR_CreateDspArgumentList (NUM_DSP_ARGS,
                                             NUM_MSG_PER_POOL,
                                             maxMsgSize,
                                             maxLocalQueues,
                                             testMsgqId,
                                             DSP_CONTROL_MSGQ_ID) ;

        statusOfSetup = BVR_SetupMessaging (procId,
                                            NUM_MSG_PER_POOL,
                                            maxMsgSize,
                                            maxLocalQueues,
                                            dspExecutable,
                                            NUM_DSP_ARGS,
                                            dspArgs) ;
        BVR_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Create (GPP_CONTROL_MSGQ_ID, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Create failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }
        else {
            TST_PrnError ("Failed to setup MSGQ. Status = [0x%x]",
                          statusOfSetup) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Get (GPP_CONTROL_MSGQ_ID,
                                      WAIT_FOREVER,
                                        (MsgqMsg *) &msg) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Get failed - 1. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Locate (procId,
                                         DSP_CONTROL_MSGQ_ID,
                                         &msgqLocateAttrs) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            status = MSGQ_Locate (procId, testMsgqId, &msgqLocateAttrs) ;
            if (status == DSP_ENOTFOUND) {
                status = DSP_SOK ;
            }
            else if (DSP_FAILED (status)) {
                TST_PrnInfo ("First MSGQ_Locate failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Put (procId, DSP_CONTROL_MSGQ_ID, (MsgqMsg) msg,
                                msgId, MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed - 1. Status = [0x%x]",
                              status) ;
                MSGQ_Free ((MsgqMsg) msg) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Get (GPP_CONTROL_MSGQ_ID,
                               WAIT_FOREVER,
                               (MsgqMsg *) &msg) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Get failed - 2. Status = [0x%x]",
                              status) ;
            MSGQ_Free ((MsgqMsg) msg) ;
        }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Locate (procId, testMsgqId, &msgqLocateAttrs) ;
            if (DSP_FAILED (status)) {
                TST_PrnInfo ("Second MSGQ_Locate failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Put (procId, DSP_CONTROL_MSGQ_ID, (MsgqMsg) msg,
                                      msgId, MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed - 2. Status = [0x%x]",
                              status) ;
                MSGQ_Free ((MsgqMsg) msg) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Get (GPP_CONTROL_MSGQ_ID,
                                      WAIT_FOREVER,
                                      (MsgqMsg *) &msg) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Get failed - 3. Status = [0x%x]",
                              status) ;
                MSGQ_Free ((MsgqMsg) msg) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Locate (procId, testMsgqId, &msgqLocateAttrs) ;
            if (status == DSP_ENOTFOUND) {
                status = DSP_SOK ;
            }
            else if (DSP_FAILED (status)) {
                TST_PrnInfo ("Third MSGQ_Locate failed. Status = [0x%x]",
                              status) ;
            }
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Put (procId, DSP_CONTROL_MSGQ_ID, (MsgqMsg) msg,
                                    msgId, MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed - 3. Status = [0x%x]",
                              status) ;
            MSGQ_Free ((MsgqMsg) msg) ;
        }
        }

        statusOfCleanup = MSGQ_Delete (GPP_CONTROL_MSGQ_ID) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on MSGQ_Delete. Status = [0x%x]",
                          statusOfCleanup) ;
        }

        statusOfCleanup = BVR_CleanupMessaging (procId) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed in BVR_CleanupMessaging. Status = [0x%x]",
                          statusOfCleanup) ;
    }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
