/** ============================================================================
 *  @file   API_MsgqDelete.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_Delete API test case.
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
#include <API_MsgqDelete.h>


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
#define NUM_DSP_ARGS 6

/** ============================================================================
 *  @const  NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages in each buffer pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL 5

/** ============================================================================
 *  @const  MAX_GPP_MSGQS
 *
 *  @desc   Maximum number of MSGQs created on the GPP.
 *  ============================================================================
 */
#define MAX_GPP_MSGQS 2


/** ============================================================================
 *  @func   API_MsgqDelete
 *
 *  @desc   Tests the MSGQ_Delete API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqDelete (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  statusOfSetup   = DSP_SOK     ;
    DSP_STATUS  status          = DSP_SOK     ;
    DSP_STATUS  statusOfCleanup = DSP_SOK     ;
    ProcessorId procId          = 0           ;
    AllocatorId mqaId           = 0           ;
    TransportId mqtId           = TST_LMQT_ID ;
    MsgQueueId  msgqCreateId    = 0           ;
    MsgQueueId  msgqDeleteId    = 0           ;
    Uint16      maxMsgSize      = 0           ;
    Uint32      temp                          ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqDelete failed. Status =[0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId = TST_StringToInt (argv [2]) ;

        statusOfSetup = TST_MacroToIntValue (&temp,
                                             MAX_MSGQS,
                                             argv [3],
                                             "MAX_MSGQS") ;
        if (DSP_SUCCEEDED (statusOfSetup)) {
            msgqCreateId = (MsgQueueId) temp ;
        }
        else if (statusOfSetup == DSP_ENOTFOUND) { /* Not a macro value */
            msgqCreateId  = TST_StringToInt (argv [3]) ;
            statusOfSetup = DSP_SOK ;
        }

        statusOfSetup = TST_MacroToIntValue (&temp,
                                             MAX_MSGQS,
                                             argv [4],
                                             "MAX_MSGQS") ;
        if (DSP_SUCCEEDED (statusOfSetup)) {
            msgqDeleteId = (MsgQueueId) temp ;
        }
        else if (statusOfSetup == DSP_ENOTFOUND) { /* Not a macro value */
            msgqDeleteId  = TST_StringToInt (argv [4]) ;
            statusOfSetup = DSP_SOK ;
        }

        maxMsgSize = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("PROC_Setup failed. Status =[0x%x]",
                          statusOfSetup) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenAllocator (mqaId,
                                               NUM_MSG_PER_POOL,
                                               maxMsgSize) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Attach (procId, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("PROC_Attach failed. Status =[0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenTransport (mqtId,
                                               mqaId,
                                               maxMsgSize,
                                               MAX_GPP_MSGQS) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Create (msgqCreateId, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Create failed. Status =[0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_Delete.
         *  --------------------------------------------------------------------
         */
        status = MSGQ_Delete (msgqDeleteId) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("MSGQ_Delete failed. Status =[0x%x]",
                          status) ;
        }

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        if (msgqCreateId != msgqDeleteId) {
            statusOfCleanup = MSGQ_Delete (msgqCreateId) ;
            if (DSP_FAILED (statusOfCleanup)) {
                TST_PrnError ("MSGQ_Delete failed. Status =[0x%x]",
                              statusOfCleanup) ;
            }
        }
        statusOfCleanup = API_CloseTransport (mqtId) ;

        statusOfCleanup = PROC_Detach (procId) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("PROC_Detach failed. Status =[0x%x]",
                          statusOfCleanup) ;
        }

        statusOfCleanup = API_CloseAllocator (mqaId) ;

        statusOfCleanup = PROC_Destroy () ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("PROC_Destroy failed. Status =[0x%x]",
                          statusOfCleanup) ;
        }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
