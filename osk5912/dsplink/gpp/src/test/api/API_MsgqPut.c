/** ============================================================================
 *  @file   API_MsgqPut.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_Put API test case.
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
#include <TST_Sleep.h>
#include <API_PostProcess.h>
#include <API_MsgqUtils.h>
#include <API_MsgqPut.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 9

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
#define MAX_GPP_MSGQS MAX_MSGQS

/** ============================================================================
 *  @const  NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages in each buffer pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL 2

/** ============================================================================
 *  @const  NUM_MSG
 *
 *  @desc   Number of messages to be exchanged.
 *  ============================================================================
 */
#define NUM_MSG 1

/** ============================================================================
 *  @macro  LOCATE_TIMEOUT
 *
 *  @desc   Timeout for locate to be declared as failed.
 *  ============================================================================
 */
#define LOCATE_TIMEOUT 10000

/** ============================================================================
 *  @macro  DSP_QUEUE_ID
 *
 *  @desc   Queue to be created on the DSP.
 *  ============================================================================
 */
#define DSP_QUEUE_ID 0

/** ============================================================================
 *  @macro  MSG_HDR_NULL/MSG_HDR_VALID
 *
 *  @desc   Type of message header pointer to pass.
 *  ============================================================================
 */
#define MSG_HDR_NULL   0
#define MSG_HDR_VALID  1


/** ============================================================================
 *  @func   API_MsgqPut
 *
 *  @desc   Tests the MSGQ_Put API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqPut (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup          = DSP_SOK          ;
    DSP_STATUS      statusOfCleanup        = DSP_SOK          ;
    DSP_STATUS      expStatusOfSetup       = DSP_SOK          ;
    DSP_STATUS      status                 = DSP_SOK          ;
    DSP_STATUS      tmpStatus              = DSP_SOK          ;
    ProcessorId     procId                 = 0                ;
    ProcessorId     putProcId              = 0                ;
    MsgQueueId      msgqLocateId           = 0                ;
    MsgQueueId      msgqPutId              = 0                ;
    Uint8           msgType                = 0                ;
    Uint16          msgId                  = 0                ;
    MsgqMsg         msg                    = NULL             ;
    MsgqLocateAttrs msgqLocateAttrs        = {LOCATE_TIMEOUT} ;
    Char8 **        dspArgs                = NULL             ;
    Bool            locateFlag             = FALSE            ;
    Uint16          maxMsgSize                                ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqPut. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId       = TST_StringToInt (argv [2]) ;
        msgqLocateId = TST_StringToInt (argv [3]) ;
        msgqPutId    = TST_StringToInt (argv [4]) ;
        putProcId    = TST_StringToInt (argv [5]) ;
        msgType      = TST_StringToInt (argv [6]) ;
        msgId        = TST_StringToInt (argv [7]) ;

        maxMsgSize   = ALIGN (sizeof (TestMsg)) ;

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
                                                 DSP_QUEUE_ID,
                                                 0, /* dummy argument */
                                                 NUM_MSG) ;

            statusOfSetup = PROC_Load (procId, argv [8],
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
            TST_StrToStatus (argv [0], &expStatusOfSetup) ;
            statusOfSetup = DSP_ENOTFOUND ;
            while ((statusOfSetup == DSP_ENOTFOUND) && (locateFlag == FALSE)) {
            statusOfSetup = MSGQ_Locate (putProcId,
                                         msgqLocateId,
                                         &msgqLocateAttrs) ;
                if (statusOfSetup == DSP_ENOTFOUND) {
                    if (expStatusOfSetup != DSP_ENOTFOUND) {
                        TST_Sleep (1000) ;
                    }
                    else {
                        locateFlag = TRUE ;
                    }
                }
                else if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }
        }

        if (DSP_SUCCEEDED (statusOfSetup) && (msgType == MSG_HDR_VALID)) {
            statusOfSetup = MSGQ_Alloc (TST_MQA_ID,
                                        maxMsgSize,
                                        &msg) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_Put.
         *  --------------------------------------------------------------------
         */
        status = MSGQ_Put (putProcId, msgqPutId, msg, msgId, MSGQ_INVALID_ID) ;
        tmpStatus = status ;

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        if ((msg != NULL) && (DSP_FAILED (tmpStatus))) {
            /* Free the buffer if not already free'd by _Put */
            statusOfCleanup = MSGQ_Free (msg) ;
            if (DSP_FAILED (statusOfCleanup)) {
                TST_PrnError ("Failed on MSGQ_Free. Status = [0x%x]",
                              statusOfCleanup) ;
            }
        }

        statusOfCleanup = MSGQ_Release (putProcId, msgqLocateId) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on MSGQ_Release. Status = [0x%x]",
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
