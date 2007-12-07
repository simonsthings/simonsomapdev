/** ============================================================================
 *  @file   BVR_MsgqSend.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_MsgqSend test case.
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
#include <TST_Sleep.h>
#include <TST_PrintFuncs.h>
#include <BVR_PostProcess.h>
#include <BVR_MsgqSend.h>
#include <BVR_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   DATA_VALIDATION_REQUIRED/DATA_VALIDATION_NOTREQUIRED
 *
 *  @desc   Message Id to be set to indicate if data validation is required.
 *  ============================================================================
 */
#define DATA_VALIDATION_REQUIRED        1
#define DATA_VALIDATION_NOTREQUIRED     0

/** ============================================================================
 *  @name   DATA_VALIDATION_SUCCEEDED/DATA_VALIDATION_FAILED
 *
 *  @desc   Values returned by the DSP for the data validation report.
 *  ============================================================================
 */
#define DATA_VALIDATION_SUCCEEDED  1
#define DATA_VALIDATION_FAILED     0

/** ============================================================================
 *  @name   NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 10

/** ============================================================================
 *  @name   NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 6


/** ============================================================================
 *  @func   BVR_MsgqSend
 *
 *  @desc   This test tests the GPP to DSP message transfer.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqSend (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup   = DSP_SOK                  ;
    DSP_STATUS      statusOfCleanup = DSP_SOK                  ;
    DSP_STATUS      status          = DSP_SOK                  ;
    DSP_STATUS      tmpStatus       = DSP_SOK                  ;

    ProcessorId     procId          = 0                        ;
    Uint16          numMsgPerPool   = 0                        ;
    Uint16          maxMsgSize      = 0                        ;
    MsgQueueId      msgqOutId       = 0                        ;
    Uint16          msgId           = 0                        ;
    Uint16          testVariableMsg = 0                        ;
    Uint16          numMsg          = 0                        ;
    Char8 *         dspExecutable   = NULL                     ;

    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER}           ;
    MsgqMsg         msg             = NULL                     ;
    TestMsg *       testMsg         = NULL                     ;
    Uint16          maxLocalQueues  = MAX_MSGQS                ;
    Uint16          msgAllocSize    = 0                        ;
    Uint16          msgBufferSize   = TST_MSQ_DATA_SIZE        ;
    Uint16          i               = 0                        ;
    Uint16          j               = 0                        ;
    Char8 **        dspArgs         = NULL                     ;

    Uint32          timeout         = 100                      ;
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
        procId          = TST_StringToInt (argv [2]) ;
        numMsgPerPool   = TST_StringToInt (argv [3]) ;
        maxMsgSize      = TST_StringToInt (argv [4]) ;
        msgqOutId       = TST_StringToInt (argv [5]) ;
        msgId           = TST_StringToInt (argv [6]) ;
        testVariableMsg = TST_StringToInt (argv [7]) ;
        numMsg          = TST_StringToInt (argv [8]) ;
        dspExecutable   = argv [9]                   ;

        maxMsgSize    =   maxMsgSize > sizeof (TestMsg)
                        ? maxMsgSize : sizeof (TestMsg) ;
        maxMsgSize    = ALIGN (maxMsgSize) ;
        msgAllocSize  = ALIGN (sizeof (TestMsg)) ;
        msgBufferSize += (msgAllocSize - sizeof (TestMsg)) / 2 ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = BVR_CreateDspArgumentList (NUM_DSP_ARGS,
                                             numMsgPerPool,
                                             maxMsgSize,
                                             maxLocalQueues,
                                             msgqOutId,
                                             MSGQ_INVALID_ID,
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
            statusOfSetup = DSP_ENOTFOUND ;
            while (statusOfSetup == DSP_ENOTFOUND) {
            statusOfSetup = MSGQ_Locate (procId,
                                         msgqOutId,
                                         &msgqLocateAttrs) ;
                if (DSP_FAILED (statusOfSetup) && statusOfSetup != DSP_ENOTFOUND) {
                TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
                else if (statusOfSetup == DSP_ENOTFOUND) {
                    TST_Sleep (timeout) ;
                    timeout *= 2 ;
                }
            }
        }
        else {
            TST_PrnError ("Failed to setup MSGQ. Status = [0x%x]",
                          statusOfSetup) ;
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

        status = statusOfSetup ;

        /*  --------------------------------------------------------------------
         *  Tests transfer from GPP to DSP.
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; ((i < numMsg) && DSP_SUCCEEDED (status)) ; i++) {
            status = MSGQ_Alloc (TST_MQA_ID, msgAllocSize, &msg) ;

            if (DSP_SUCCEEDED (status)) {
                testMsg = (TestMsg *) msg ;
                for (j = 0 ; j < msgBufferSize ; j++) {
                    testMsg->buffer[j] = j ;
                }

                status = MSGQ_Put (procId, msgqOutId, msg,
                                   msgId, MSGQ_INVALID_ID) ;
                tmpStatus = status ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("Failed to put a message. Status = [0x%x]",
                                  status) ;
                }

                /*
                 *  Issue an empty buffer to read data and then reclaim it
                 */
                if (   DSP_SUCCEEDED (status)
                    && (msgId == DATA_VALIDATION_REQUIRED)) {
                    status = CHNL_Issue (procId, readChnlId, &io_info) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("CHNL_Issue failed. Status = [0x%x]",
                                      status) ;
                    }
                }

                if (   DSP_SUCCEEDED (status)
                    && (msgId == DATA_VALIDATION_REQUIRED)) {
                    status = CHNL_Reclaim (procId,
                                           readChnlId,
                                           WAIT_FOREVER,
                                           &io_info) ;
                    if (DSP_FAILED (status)) {
                        TST_PrnError ("CHNL_Reclaim failed. Status = [0x%x]",
                                      status) ;
                    }
                    else if (   *((Uint16 *)(io_info.buffer))
                             != DATA_VALIDATION_SUCCEEDED) {
                        status = DSP_EFAIL ;
                        TST_PrnError ("Failed data verification. "
                                      "Status = [0x%x]", status) ;
                    }
                }
            }
            else {
                TST_PrnError ("Failed to allocate a message. Status = [0x%x]",
                              status) ;
            }

            /*
             *  If variable sized message test enabled, change the
             *  size for the next message.
             */
            if (testVariableMsg != 0) {
                msgAllocSize  =   (msgAllocSize == maxMsgSize)
                                ? ALIGN (sizeof (TestMsg))
                                : maxMsgSize ;
                msgBufferSize =   TST_MSQ_DATA_SIZE ;
                msgBufferSize += (msgAllocSize - sizeof (TestMsg)) / 2 ;
            }
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        statusOfCleanup = CHNL_FreeBuffer (procId, readChnlId,
                                           &(io_info.buffer), 1) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on CHNL_FreeBuffer. Status = [0x%x]",
                          statusOfCleanup) ;
            status = statusOfCleanup ;
        }

        statusOfCleanup = CHNL_Delete (procId, readChnlId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on CHNL_Delete. Status = [0x%x]",
                          statusOfCleanup) ;
            status = statusOfCleanup ;
        }

        if (DSP_FAILED (tmpStatus)) { /* Need to free only if last put failed */
            statusOfCleanup = MSGQ_Free (msg) ;
            if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
                TST_PrnError ("Failed on MSGQ_Free. Status = [0x%x]",
                              statusOfCleanup) ;
                status = statusOfCleanup ;
            }
        }

        statusOfCleanup = MSGQ_Release (procId, msgqOutId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on MSGQ_Release. Status = [0x%x]",
                          statusOfCleanup) ;
            status = statusOfCleanup ;
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
