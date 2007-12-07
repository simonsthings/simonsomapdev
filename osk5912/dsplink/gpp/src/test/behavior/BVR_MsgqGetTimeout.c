/** ============================================================================
 *  @file   BVR_MsgqGetTimeout.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_MsgqGetTimeout test case.
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
#include <BVR_PostProcess.h>
#include <BVR_MsgqGetTimeout.h>
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
#define NUM_ARGS 11


/** ============================================================================
 *  @name   NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 6

/** ============================================================================
 *  @func   BVR_MsgqGetTimeout
 *
 *  @desc   Function to test the behavior of MSGQ_Get () with finite
 *          timeout.
 *
 *          This test tests the behavior of MSGQ_Get with finite timeout.
 *          The GPP side sends a control message with a delay value to
 *          the DSP. The DSP then sleeps for the delay period and sends
 *          the message back to the GPP. The GPP in the mean time tries
 *          to get the message twice with finite timeout values specified
 *          in the data file.
 *
 *                     GPP                                       DSP
 *                                          | |
 *          MSGQ_Put (msg with delay val)   | |
 *                                          | |
 *                                          | | MSGQ_Get (infinite wait)
 *                                          | |
 *                                          | | Sleep for 'Timeout' specified
 *                                          | | by GPP side.
 *          MSGQ_Get (timeout1)             | |
 *                                          | |
 *                                          | |
 *                                          | |
 *                                          | |
 *                                          | |
 *                                          | |
 *          Timeout 1 expires               | |
 *          MSGQ_Get (timeout2)             | |
 *                                          | | Wakes up from sleep here.
 *                                          | | MSGQ_Put
 *                                          | |
 *                                          | |
 *          Timeout 2 expires               | |
 *                                          | |
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqGetTimeout (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup   = DSP_SOK          ;
    DSP_STATUS      statusOfCleanup = DSP_SOK          ;
    DSP_STATUS      status1         = DSP_SOK          ;
    DSP_STATUS      status2         = DSP_SOK          ;
    DSP_STATUS      tempStatus      = DSP_SOK          ;
    DSP_STATUS      expStatusGet1   = DSP_SOK          ;
    DSP_STATUS      expStatusGet2   = DSP_SOK          ;

    ProcessorId     procId          = 0                ;
    Uint16          numMsgPerPool   = 0                ;
    MsgQueueId      msgqOutId       = 0                ;
    MsgQueueId      msgqInId        = 1                ;
    Uint16          timeout1        = 0                ;
    Uint16          timeout2        = 0                ;
    Uint16          dspDelay        = 0                ;
    Char8 *         dspExecutable   = NULL             ;

    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER}   ;
    TestMsg *       msg             = NULL             ;
    Uint16          maxMsgSize      = 0                ;
    Uint16          msgId           = 0                ;
    Uint16          numMsg          = 2                ;
    Uint16          maxLocalQueues  = 1                ;
    Char8 **        dspArgs         = NULL             ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status1 = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]",
                      status1) ;
    }
    else {

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId        = TST_StringToInt (argv [3])  ;
        numMsgPerPool = TST_StringToInt (argv [4])  ;
        msgqOutId     = TST_StringToInt (argv [5])  ;
        msgqInId      = TST_StringToInt (argv [6])  ;
        timeout1      = TST_StringToInt (argv [7])  ;
        timeout2      = TST_StringToInt (argv [8])  ;
        dspDelay      = TST_StringToInt (argv [9])  ;
        dspExecutable = argv [10]                   ;

        maxMsgSize    = ALIGN (sizeof (TestMsg))   ;
        maxLocalQueues = msgqInId + 1 ;


        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = BVR_CreateDspArgumentList (NUM_DSP_ARGS,
                                             numMsgPerPool,
                                             maxMsgSize,
                                             maxLocalQueues,
                                             msgqOutId,
                                             msgqInId,
                                             numMsg) ;

        statusOfSetup = BVR_SetupMessaging (procId,
                                            numMsgPerPool,
                                            maxMsgSize,
                                            maxLocalQueues,
                                            dspExecutable,
                                            NUM_DSP_ARGS,
                                            dspArgs) ;

        BVR_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Create (msgqInId, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Create failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = DSP_ENOTFOUND ;
            while (statusOfSetup == DSP_ENOTFOUND) {
                statusOfSetup = MSGQ_Locate (procId,
                                             msgqOutId,
                                             &msgqLocateAttrs) ;
                if (DSP_FAILED (statusOfSetup)) {
                    TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                                  statusOfSetup) ;
                }
            }
        }
        else {
            TST_PrnError ("Failed to setup MSGQ. Status = [0x%x]",
                          statusOfSetup) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            /* Wait for the DSP to become ready for the test */
            statusOfSetup = MSGQ_Get (msgqInId, WAIT_FOREVER, (MsgqMsg *) &msg) ;

            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Get failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_FAILED (statusOfSetup)) {
            status1 = statusOfSetup ;
        }

        /*  --------------------------------------------------------------------
         *  Test MSGQ_Get with finite timeout.
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status1)) {
            msg->info = dspDelay ;
            status1 = MSGQ_Put (procId, msgqOutId,
                                (MsgqMsg) msg,
                                msgId, msgqInId) ;
        }

        if (DSP_FAILED (status1)) {
            TST_PrnError ("Failed to put a message. Status = [0x%x]",
                          status1) ;
            MSGQ_Free ((MsgqMsg) msg) ;
            msg = NULL ;
            status1 = status2 = DSP_EFAIL ;
        }
        else {
            status1 = MSGQ_Get (msgqInId, timeout1, (MsgqMsg *) &msg) ;
            tempStatus = TST_StrToStatus (argv [1],
                                          &expStatusGet1) ;

            if (DSP_SUCCEEDED (tempStatus)) {
                if (status1 != expStatusGet1) {
                    TST_PrnError ("First MSGQ_Get ended with unexpected "
                                  "status. Status = [0x%x]",
                                   status1) ;
                }
            }
            else {
                TST_PrnError ("TST_StrToStatus Failed. Status = [0x%x]",
                              tempStatus) ;
            }

            if (DSP_SUCCEEDED (status1)) {
                MSGQ_Free ((MsgqMsg) msg) ;
                msg = NULL ;
            }

            status2 = MSGQ_Get (msgqInId, timeout2, (MsgqMsg *) &msg) ;
            tempStatus = TST_StrToStatus (argv [2],
                                          &expStatusGet2) ;

            if (DSP_SUCCEEDED (tempStatus)) {
                if (status2 != expStatusGet2) {
                    TST_PrnError ("Second MSGQ_Get ended with unexpected "
                                  "status. Status = [0x%x]",
                                   status2) ;
                }
            }
            else {
                TST_PrnError ("TST_StrToStatus Failed. Status = [0x%x]",
                              tempStatus) ;
            }

            if (DSP_SUCCEEDED (status2)) {
                MSGQ_Free ((MsgqMsg) msg) ;
                msg = NULL ;
            }
        }

        status1 = BVR_PostProcess (status1, argv [1], statusOfSetup, argv [0]) ;
        status2 = BVR_PostProcess (status2, argv [2], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup
         *  --------------------------------------------------------------------
         */
        if (msg != NULL) {
            statusOfCleanup = MSGQ_Free ((MsgqMsg) msg) ;
        }
        if (DSP_SUCCEEDED (status1) && DSP_FAILED (statusOfCleanup)) {
            status1 = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Free. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = MSGQ_Delete (msgqInId) ;
        if (DSP_SUCCEEDED (status1) && DSP_FAILED (statusOfCleanup)) {
            status1 = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Delete. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = MSGQ_Release (procId, msgqOutId) ;
        if (DSP_SUCCEEDED (status1) && DSP_FAILED (statusOfCleanup)) {
            status1 = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Release. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = BVR_CleanupMessaging (procId) ;
        if (DSP_SUCCEEDED (status1) && DSP_FAILED (statusOfCleanup)) {
            status1 = statusOfCleanup ;
        }
    }

    if (DSP_FAILED (status1) || DSP_FAILED (status2)) {
        status1 = DSP_EFAIL ;
    }

    return (status1) ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
