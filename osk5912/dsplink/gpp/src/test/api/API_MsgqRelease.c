/** ============================================================================
 *  @file   API_MsgqRelease.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_Release API test case.
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
#include <API_MsgqRelease.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 8

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
 *  @const  NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages in each buffer pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL 1

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
#define LOCATE_TIMEOUT 1000


/** ============================================================================
 *  @func   API_MsgqRelease
 *
 *  @desc   Tests the MSGQ_Release API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqRelease (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup   = DSP_SOK          ;
    DSP_STATUS      statusOfCleanup = DSP_SOK          ;
    DSP_STATUS      status          = DSP_SOK          ;
    ProcessorId     procId          = 0                ;
    ProcessorId     locateProcId    = 0                ;
    TransportId     mqtId           = 1                ;
    MsgQueueId      msgqLocateId    = 0                ;
    MsgQueueId      msgqReleaseId   = 0                ;
    MsgqLocateAttrs msgqLocateAttrs = {LOCATE_TIMEOUT} ;
    Char8 **        dspArgs         = NULL             ;
    Uint16          maxMsgSize                         ;
    MsgQueueId      dspMsgqId                          ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqRelease. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId        = TST_StringToInt (argv [2]) ;
        msgqLocateId  = TST_StringToInt (argv [3]) ;
        msgqReleaseId = TST_StringToInt (argv [4]) ;
        dspMsgqId     = TST_StringToInt (argv [5]) ;
        locateProcId  = TST_StringToInt (argv [6]) ;

        maxMsgSize    = ALIGN (sizeof (TestMsg)) ;

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
                                                 dspMsgqId,
                                                 0, /* dummy argument */
                                                 NUM_MSG) ;

            statusOfSetup = PROC_Load (procId, argv [7],
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
            statusOfSetup = API_OpenTransport (mqtId,
                                               TST_MQA_ID,
                                               maxMsgSize,
                                               MAX_GPP_MSGQS) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Locate (locateProcId,
                                         msgqLocateId,
                                         &msgqLocateAttrs) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_Release.
         *  --------------------------------------------------------------------
         */
        status = MSGQ_Release (locateProcId, msgqReleaseId) ;

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        if (msgqReleaseId != msgqLocateId) {
            statusOfCleanup = MSGQ_Release (locateProcId, msgqLocateId) ;
            if (DSP_FAILED (statusOfCleanup)) {
                TST_PrnError ("Failed on MSGQ_Release. Status = [0x%x]",
                              statusOfCleanup) ;
            }
        }
        statusOfCleanup = API_CloseTransport (mqtId) ;
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
