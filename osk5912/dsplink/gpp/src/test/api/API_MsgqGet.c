/** ============================================================================
 *  @file   API_MsgqGet.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_Get API test case.
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

/*  ----------------------------------- MSGQ Component                */
#include <msgqdefs.h>
#include <msgq.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <API_PostProcess.h>
#include <API_MsgqUtils.h>
#include <API_MsgqGet.h>


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
 *  @const  MAX_GPP_MSGQS
 *
 *  @desc   Maximum number of MSGQs created on the GPP.
 *  ============================================================================
 */
#define MAX_GPP_MSGQS 1

/** ============================================================================
 *  @const  NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages in each buffer pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL 5

/** ============================================================================
 *  @const  NUM_MSG
 *
 *  @desc   Number of messages to be exchanged.
 *  ============================================================================
 */
#define NUM_MSG 1

/** ============================================================================
 *  @name   MSG_HDR_NULL_PTR_PTR/MSG_HDR_VALID_PTR_PTR
 *
 *  @desc   Type of message header pointer to pass.
 *  ============================================================================
 */
#define MSG_HDR_NULL_PTR_PTR   0
#define MSG_HDR_VALID_PTR_PTR  1


/** ============================================================================
 *  @func   API_MsgqGet
 *
 *  @desc   Tests the MSGQ_Get API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqGet (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  statusOfSetup   = DSP_SOK ;
    DSP_STATUS  statusOfCleanup = DSP_SOK ;
    DSP_STATUS  status          = DSP_SOK ;
    DSP_STATUS  tmpStatus       = DSP_SOK ;
    ProcessorId procId          = 0       ;
    MsgQueueId  msgqCreateId    = 0       ;
    MsgQueueId  msgqGetId       = 0       ;
    Uint8       msgType         = 0       ;
    MsgqMsg     msg             = NULL    ;
    Char8 **    dspArgs         = NULL    ;
    Uint16      maxMsgSize      = 0       ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqGet. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId        = TST_StringToInt (argv [2]) ;
        msgqCreateId  = TST_StringToInt (argv [3]) ;
        msgqGetId     = TST_StringToInt (argv [4]) ;
        msgType       = TST_StringToInt (argv [5]) ;
        maxMsgSize    = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;
        if (DSP_FAILED (statusOfSetup)) {
           TST_PrnError ("Failed on PROC_Setup. Status = [0x%x]",
                         statusOfSetup) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenAllocator (TST_MQA_ID,
                                               NUM_MSG_PER_POOL,
                                               maxMsgSize) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenTransport (TST_LMQT_ID,
                                               TST_MQA_ID,
                                               maxMsgSize,
                                               MAX_GPP_MSGQS) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Attach (procId, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("Failed on PROC_Attach. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            dspArgs = API_CreateDspArgumentList (NUM_DSP_ARGS,
                                                 NUM_MSG_PER_POOL,
                                                 maxMsgSize,
                                                 MAX_GPP_MSGQS,
                                                 0,
                                                 msgqCreateId,
                                                 NUM_MSG) ;

            statusOfSetup = PROC_Load (procId, argv [6],
                                       NUM_DSP_ARGS, dspArgs) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("Failed on PROC_Load. Status = [0x%x]",
                              statusOfSetup) ;
            }

            API_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Start (procId) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("Failed on PROC_Start. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenTransport (TST_RMQT_ID,
                                               TST_MQA_ID,
                                               maxMsgSize,
                                               MAX_GPP_MSGQS) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Create (msgqCreateId, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("Failed on MSGQ_Create. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_Get.
         *  --------------------------------------------------------------------
         */
        if (msgType == MSG_HDR_NULL_PTR_PTR) {
            status = MSGQ_Get (msgqGetId, WAIT_FOREVER, NULL) ;
        }
        else {
            status = MSGQ_Get (msgqGetId, WAIT_FOREVER, &msg) ;
        }
        tmpStatus = status ;

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        if ((msg != NULL) && (DSP_SUCCEEDED (tmpStatus))) {
        /* Get allocs buffer on success. Free the buffer only if get passed. */
            statusOfCleanup = MSGQ_Free (msg) ;
            if (DSP_FAILED (statusOfCleanup)) {
                TST_PrnError ("Failed on MSGQ_Free. Status = [0x%x]",
                              statusOfCleanup) ;
            }
        }
        statusOfCleanup = MSGQ_Delete (msgqCreateId) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on MSGQ_Delete. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = API_CloseTransport (TST_RMQT_ID) ;

        statusOfCleanup = PROC_Stop (procId) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on PROC_Stop. Status = [0x%x]",
                          statusOfCleanup) ;
        }
        statusOfCleanup = PROC_Detach (procId) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on PROC_Detach. Status = [0x%x]",
                          statusOfCleanup) ;
        }

        statusOfCleanup = API_CloseTransport (TST_LMQT_ID) ;
        statusOfCleanup = API_CloseAllocator (TST_MQA_ID) ;

        statusOfCleanup = PROC_Destroy () ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on PROC_Destroy. Status = [0x%x]",
                          statusOfCleanup) ;
        }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
