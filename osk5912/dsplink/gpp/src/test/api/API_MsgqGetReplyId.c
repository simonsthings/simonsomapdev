/** ============================================================================
 *  @file   API_MsgqGetReplyId.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_GetReplyId API test case.
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
#include <API_MsgqGetReplyId.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 7

/** ============================================================================
 *  @macro  NUM_DSP_ARGS
 *
 *  @desc   The number of arguments expected by DSP.
 *  ============================================================================
 */
#define NUM_DSP_ARGS 6

/** ============================================================================
 *  @macro  PTR_TYP_NULL/PTR_TYP_VALID
 *
 *  @desc   Type of pointer to pass.
 *  ============================================================================
 */
#define PTR_TYP_NULL   0
#define PTR_TYP_VALID  1

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
#define NUM_MSG_PER_POOL 1

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
 *  @const  NUM_MSG
 *
 *  @desc   Number of messages to be exchanged.
 *  ============================================================================
 */
#define NUM_MSG 1


/** ============================================================================
 *  @func   API_MsgqGetReplyId
 *
 *  @desc   Tests the MSGQ_GetReplyId API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqGetReplyId (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    statusOfSetup   = DSP_SOK ;
    DSP_STATUS    statusOfCleanup = DSP_SOK ;
    DSP_STATUS    status          = DSP_SOK ;
    ProcessorId   procId          = 0       ;
    MsgqMsg       msg             = NULL    ;
    MsgQueueId    msgqReplyId     = 0       ;
    ProcessorId   procReplyId     = 0       ;
    Uint8         msgType         = 0       ;
    Uint8         msgqIdPtrType   = 0       ;
    Uint8         procIdPtrType   = 0       ;
    MsgqMsg *     msgPtr          = NULL    ;
    MsgQueueId *  msgqReplyIdPtr  = NULL    ;
    ProcessorId * procReplyIdPtr  = NULL    ;
    Char8 **      dspArgs         = NULL    ;
    Uint16        maxMsgSize                ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqGetReplyId. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId         = TST_StringToInt (argv [2]) ;
        msgType        = TST_StringToInt (argv [3]) ;
        msgqIdPtrType  = TST_StringToInt (argv [4]) ;
        procIdPtrType  = TST_StringToInt (argv [5]) ;
        msgPtr         = &msg                       ;

        msgqReplyIdPtr = (msgqIdPtrType == PTR_TYP_NULL) ? NULL : &msgqReplyId ;
        procReplyIdPtr = (procIdPtrType == PTR_TYP_NULL) ? NULL : &procReplyId ;

        maxMsgSize = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenAllocator (TST_MQA_ID,
                                               NUM_MSG_PER_POOL,
                                               maxMsgSize) ;
        }
        else {
            TST_PrnError ("PROC_Setup failed. Status = [0x%x]", statusOfSetup) ;
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
                TST_PrnError ("PROC_Attach failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            dspArgs = API_CreateDspArgumentList (NUM_DSP_ARGS,
                                                 NUM_MSG_PER_POOL,
                                                 maxMsgSize,
                                                 MAX_GPP_MSGQS,
                                                 DSP_MSGQ_ID,
                                                 GPP_MSGQ_ID,
                                                 NUM_MSG) ;

            statusOfSetup = PROC_Load (procId, argv [6],
                                       NUM_DSP_ARGS, dspArgs) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("PROC_Load failed. Status = [0x%x]",
                              statusOfSetup) ;
            }

            API_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Start (procId) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("PROC_Start failed. Status = [0x%x]",
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
            statusOfSetup = MSGQ_Create (GPP_MSGQ_ID, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Create failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup) && msgType != PTR_TYP_NULL) {
            statusOfSetup = MSGQ_Get (GPP_MSGQ_ID, WAIT_FOREVER, &msg) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Get failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_GetReplyId.
         *  --------------------------------------------------------------------
         */
        status = MSGQ_GetReplyId (*msgPtr, procReplyIdPtr, msgqReplyIdPtr) ;

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        if (msg != NULL) {
            statusOfCleanup = MSGQ_Free (msg) ;
            if (DSP_FAILED (statusOfCleanup)) {
                TST_PrnError ("Failed on MSGQ_Free. Status = [0x%x]",
                              statusOfCleanup) ;
            }
        }
        statusOfCleanup = MSGQ_Delete (GPP_MSGQ_ID) ;
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
