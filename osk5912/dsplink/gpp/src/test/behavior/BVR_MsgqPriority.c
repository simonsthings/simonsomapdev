/** ============================================================================
 *  @file   BVR_MsgqPriority.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_MsgqPriority test case.
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
#include <BVR_MsgqPriority.h>
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
#define NUM_ARGS 9

/** ============================================================================
 *  @name   NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 6

/** ============================================================================
 *  @name   SHUTDOWN_DSP
 *
 *  @desc   The info code to be sent to DSP to signal shutdown.
 *  ============================================================================
 */
#define SHUTDOWN_DSP 0


/** ============================================================================
 *  @func   BVR_MsgqPriority
 *
 *  @desc   This test tests the priority of messages over data channel.
 *          It requests the DSP side application to flood the GPP with
 *          data buffers over the data channel and send a message
 *          subsequently. The test passes when the message is received
 *          before the expected number of data buffers.
 *          The failure of the test does not mean a faulty functionality.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqPriority (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup   = DSP_SOK                  ;
    DSP_STATUS      statusOfCleanup = DSP_SOK                  ;
    DSP_STATUS      status          = DSP_SOK                  ;

    ProcessorId     procId          = 0                        ;
    Uint16          numMsgPerPool   = 0                        ;
    Uint16          maxMsgSize      = 0                        ;
    MsgQueueId      msgqOutId       = 0                        ;
    MsgQueueId      msgqInId        = 1                        ;
    Uint16          maxPackets      = 1                        ;
    Char8 *         dspExecutable   = NULL                     ;

    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER}           ;
    TestMsg *       msg             = NULL                     ;
    Uint16          msgId           = 0                        ;
    Uint16          maxLocalQueues  = 1                        ;
    Uint16          timeOut         = 100                      ;
    Uint16          i               = 0                        ;
    Uint16          numPackets      = 1                        ;
    Bool            msgReceived     = FALSE                    ;
    Char8 **        dspArgs         = NULL                     ;

    ChannelId       readChnlId      = 1                        ;
    ChannelAttrs    chnlAttrs       = {Endianism_Default,
                                       ChannelMode_Input,
                                       ChannelDataSize_16bits} ;
    ChannelIOInfo   io_info         = {NULL,
                                       TST_REPORT_SIZE,
                                       0}                      ;

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
        procId        = TST_StringToInt (argv [2]) ;
        numMsgPerPool = TST_StringToInt (argv [3]) ;
        maxMsgSize    = TST_StringToInt (argv [4]) ;
        msgqOutId     = TST_StringToInt (argv [5]) ;
        msgqInId      = TST_StringToInt (argv [6]) ;
        maxPackets    = TST_StringToInt (argv [7]) ;
        dspExecutable = argv [8]                   ;

        maxMsgSize    =   maxMsgSize > sizeof (TestMsg)
                        ? maxMsgSize : sizeof (TestMsg)  ;
        maxMsgSize     = ALIGN (maxMsgSize) ;
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
                                             maxPackets) ;

        statusOfSetup = BVR_SetupMessaging (procId,
                                            numMsgPerPool,
                                            maxMsgSize,
                                            maxLocalQueues,
                                            dspExecutable,
                                            NUM_DSP_ARGS,
                                            dspArgs) ;

        BVR_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = DSP_ENOTFOUND ;
            while (statusOfSetup == DSP_ENOTFOUND) {
            statusOfSetup = MSGQ_Locate (procId,
                                         msgqOutId,
                                         &msgqLocateAttrs) ;
                if (   (DSP_FAILED (statusOfSetup))
                    && (statusOfSetup != DSP_ENOTFOUND)) {
                TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
                else if (statusOfSetup == DSP_ENOTFOUND) {
                    TST_Sleep (timeOut) ;
                    timeOut *= 2 ;
                }
            }
        }
        else {
            TST_PrnError ("Failed to setup MSGQ. Status = [0x%x]",
                          statusOfSetup) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Create (msgqInId, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Create failed. Status = [0x%x]",
                              statusOfSetup) ;
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

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Alloc (TST_MQA_ID, maxMsgSize,
                                        (MsgqMsg *) &msg) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_FAILED (statusOfSetup)) {
            status = statusOfSetup ;
        }

        /*  --------------------------------------------------------------------
         *  Tests priority of messages.
         *  --------------------------------------------------------------------
         */
        while (   (numPackets <= (2 * maxPackets))
               && DSP_SUCCEEDED (status)
               && (msgReceived == FALSE)) {
            if (numPackets > maxPackets) {
                numPackets = maxPackets ;
            }

            msg->info = numPackets ;
            status = MSGQ_Put (procId, msgqOutId, (MsgqMsg) msg,
                               msgId, msgqInId) ;
            if (DSP_SUCCEEDED (status)) {
                /*
                 *  Attempt to get a message buffer before a data buffer until
                 *  numPackets are reclaimed after issuing.
                 *  If a message is received before this, the test is considered
                 *  successful - else proceed to the next iteration with
                 *  increased value of numPackets.
                 */
                i = 0 ;
                while (   (i < numPackets)
                       && DSP_SUCCEEDED (status)
                       && (msgReceived == FALSE)) {
                    /* Issue an empty buffer to read data and then reclaim it */
                    status = CHNL_Issue (procId, readChnlId, &io_info) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("Failed to issue on channel. "
                                      "Status = [0x%x]", status) ;
                    }
                    else {
                        status = CHNL_E_NOIOC ;
                        while (status == CHNL_E_NOIOC) {
                            status = MSGQ_Get (msgqInId, WAIT_NONE,
                                               (MsgqMsg *) &msg) ;
                            if (DSP_SUCCEEDED (status)) {
                                msgReceived = TRUE ;
                            }
                            else {
                                status = CHNL_Reclaim (procId, readChnlId,
                                                       WAIT_NONE, &io_info) ;
                                if (DSP_SUCCEEDED (status)) {
                                    i++ ;
                                }
                                else {
                                    TST_Sleep (1000) ;
                                }
                            }
                        }
                    }
                }

                if (msgReceived == TRUE) {
                    /*
                     *  Message was received before a data buffer.
                     *  Indicates that the test passed.
                     *  The buffer that was issued before the call to
                     *  MSGQ_Get () was not reclaimed - reclaim it.
                     */
                    status = CHNL_Reclaim (procId, readChnlId,
                                           WAIT_FOREVER, &io_info) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("Failed on CHNL_Reclaim."
                                      " Status = [0x%x]",
                                      status) ;
                    }
                }
                else {
                    /*
                     *  This indicates that the test failed for the current
                     *  number of data buffers.
                     *  Try to test the scenario with an increased number
                     *  of data buffers after getting the message buffer
                     *  that DSP would have put earlier.
                     */
                    status = MSGQ_Get (msgqInId, WAIT_FOREVER,
                                       (MsgqMsg *) &msg) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("Failed on MSGQ_Get. Status = [0x%x]",
                                      status) ;
                    }
                    numPackets = numPackets * 2 ;
                }
            }
                else {
                TST_PrnError ("Failed to put a message. Status = [0x%x]",
                                      status) ;
                    }
        }

        if (msgReceived == FALSE) {
            status = DSP_EFAIL ;
        }
        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup
         *  --------------------------------------------------------------------
         */

        /* Shut down DSP side */
        msg->info = SHUTDOWN_DSP ;
        statusOfCleanup = MSGQ_Put (procId, msgqOutId, (MsgqMsg) msg,
                                    msgId, msgqInId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Put. Status = [0x%x]",
                          statusOfCleanup) ;
        }

        statusOfCleanup = CHNL_FreeBuffer (procId, readChnlId,
                                           &(io_info.buffer), 1) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on CHNL_FreeBuffer. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = CHNL_Delete (procId, readChnlId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on CHNL_Delete. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = MSGQ_Delete (msgqInId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Delete. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = MSGQ_Release (procId, msgqOutId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Release. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = BVR_CleanupMessaging (procId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
        }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
