/** ============================================================================
 *  @file   BVR_MsgqReceive.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_MsgqReceive test case.
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
#include <BVR_MsgqReceive.h>
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
#define NUM_ARGS 7

/** ============================================================================
 *  @name   NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 6

/** ============================================================================
 *  @name   MAX_LOCAL_QUEUES
 *
 *  @desc   The number of queues to be created on the GPP.
 *  ============================================================================
 */
#define MAX_LOCAL_QUEUES 1


/** ============================================================================
 *  @func   BVR_MsgqReceive
 *
 *  @desc   This test tests the DSP to GPP message transfer.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqReceive (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  statusOfSetup   = DSP_SOK ;
    DSP_STATUS  statusOfCleanup = DSP_SOK ;
    DSP_STATUS  status          = DSP_SOK ;
    DSP_STATUS  tmpStatus       = DSP_SOK ;

    ProcessorId procId          = 0       ;
    ProcessorId outProcId       = 0       ;
    Uint16      numMsgPerPool   = 0       ;
    MsgQueueId  msgqInId        = 0       ;
    MsgQueueId  msgqOutId       = 0       ;
    Uint16      numMsg          = 1       ;
    Char8 *     dspExecutable   = NULL    ;

    MsgqMsg     msg             = NULL    ;
    TestMsg *   testMsg         = NULL    ;
    Uint16      maxMsgSize      = 0       ;
    Uint16      i               = 0       ;
    Uint16      j               = 0       ;
    Char8 **    dspArgs         = NULL    ;


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
        msgqInId      = TST_StringToInt (argv [4]) ;
        numMsg        = TST_StringToInt (argv [5]) ;
        dspExecutable = argv [6]                   ;

        maxMsgSize    =   ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = BVR_CreateDspArgumentList (NUM_DSP_ARGS,
                                             numMsgPerPool,
                                             maxMsgSize,
                                             MAX_LOCAL_QUEUES,
                                             msgqOutId,
                                             msgqInId,
                                             numMsg) ;

        statusOfSetup = BVR_SetupMessaging (procId,
                                            numMsgPerPool,
                                            maxMsgSize,
                                            MAX_LOCAL_QUEUES + msgqInId,
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
        else {
            TST_PrnError ("Failed to setup MSGQ. Status = [0x%x]",
                          statusOfSetup) ;
        }

        status = statusOfSetup ;

        /*  --------------------------------------------------------------------
         *  Tests transfer from DSP to GPP.
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; ((i < numMsg) && DSP_SUCCEEDED (status)) ; i++) {
            status = MSGQ_Get (msgqInId, WAIT_FOREVER, &msg) ;

            if (DSP_SUCCEEDED (status)) {
                testMsg = (TestMsg *) msg ;
                for (j = 0 ;
                     (j < TST_MSQ_DATA_SIZE) && DSP_SUCCEEDED (status) ;
                     j++) {
                    if (testMsg->buffer[j] != j) {
                        status = DSP_EFAIL ;
                        TST_PrnError ("Failed data verification. "
                                      "Status = [0x%x]", status) ;
                    }
                }
                tmpStatus = MSGQ_GetReplyId (msg, &outProcId, &msgqOutId) ;
                if (DSP_FAILED (tmpStatus)) {
                    TST_PrnError ("MSGQ_GetReplyId failed. Status = [0x%x]",
                                  tmpStatus) ;
                }
                tmpStatus = MSGQ_Free (msg) ;
                if (DSP_FAILED (tmpStatus)) {
                    TST_PrnError ("MSGQ_Free failed. Status = [0x%x]",
                                  tmpStatus) ;
                }
            }
            else {
                TST_PrnError ("Failed to get a message. Status = [0x%x]",
                              status) ;
            }
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup
         *  --------------------------------------------------------------------
         */

        statusOfCleanup = MSGQ_Alloc (TST_MQA_ID, maxMsgSize, &msg) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Alloc. Status = [0x%x]", status) ;
        }
        statusOfCleanup = MSGQ_Put (procId, msgqOutId, msg, 0, MSGQ_INVALID_ID) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Put. Status = [0x%x]", status) ;
        }
        statusOfCleanup = MSGQ_Delete (msgqInId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = statusOfCleanup ;
            TST_PrnError ("Failed on MSGQ_Delete. Status = [0x%x]", status) ;
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
