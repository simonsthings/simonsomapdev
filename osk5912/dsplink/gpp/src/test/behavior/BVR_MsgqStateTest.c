/** ============================================================================
 *  @file   BVR_MsgqStateTest.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_MsgqStateTest test case.
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
#include <BVR_MsgqStateTest.h>
#include <BVR_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 6

/** ============================================================================
 *  @macro  NUM_DSP_ARGS
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
 *  @func   BVR_MsgqStateTest
 *
 *  @desc   The BVR_MsgqStateTest test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqStateTest (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup   = DSP_SOK        ;
    DSP_STATUS      statusOfCleanup = DSP_SOK        ;
    DSP_STATUS      status          = DSP_SOK        ;
    DSP_STATUS      expStatus       = DSP_SOK        ;
    DSP_STATUS      tmpStatus       = DSP_SOK        ;
    ProcessorId     procId          = 0              ;
    MsgQueueId      msgqInId        = 1              ;
    MsgQueueId      msgqOutId       = 0              ;
    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER} ;
    Uint16          numMsg          = 0              ;
    Uint32          timeOut         = 100            ;
    Uint16          msgId           = 0              ;
    MsgqMsg         msg             = NULL           ;
    Pstr            stateString     = NULL           ;
    Pstr            fileName        = NULL           ;
    Char8 **        dspArgs         = NULL           ;
    Uint16          position        = 0              ;
    Bool            ignoreStatus    = FALSE          ;
    Uint16          maxMsgSize      = 0              ;
    Char8           transition                       ;

    DBC_Require (argc >= NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc < NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        TST_StrToStatus (argv [1], &expStatus) ;
        procId      = TST_StringToInt (argv [2]) ;
        fileName    = argv [3] ;
        stateString = argv [4] ;
        numMsg      = TST_StringToInt (argv [5]) ;
        maxMsgSize  = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("PROC_Setup failed. Status = [0x%x]", statusOfSetup) ;
        }

        dspArgs = BVR_CreateDspArgumentList (NUM_DSP_ARGS,
                                             NUM_MSG_PER_POOL,
                                             maxMsgSize,
                                             MAX_MSGQS,
                                             msgqOutId,
                                             msgqInId,
                                             numMsg) ;

        /*  --------------------------------------------------------------------
         *  The state test.
         *  --------------------------------------------------------------------
         */
        while ( (stateString [position] != '\0') && DSP_SUCCEEDED (status)) {
            transition = stateString [position++] ;

            /* valid calls: 0 - 9, a - z, A - Z and escape character '\'
             * On presence of other characters - error is flagged. '\' is used
             * to skip testing of return value for the call following it.
             */
            if (   ((transition >= '0') && (transition <= '9'))
                || ((transition >= 'a') && (transition <= 'z'))
                || ((transition >= 'A') && (transition <= 'Z'))
                || (transition == '\\') ) {
                switch (transition)
                {
                case '1':
                    tmpStatus = PROC_Attach (procId, NULL) ;
                    break ;
                case '2':
                    tmpStatus = PROC_Detach (procId) ;
                    break ;
                case '3':
                    tmpStatus = PROC_Load (procId, fileName,
                                           NUM_DSP_ARGS, dspArgs) ;
                    break ;
                case '4':
                    tmpStatus = PROC_Start (procId) ;
                    break ;
                case '5':
                    tmpStatus = PROC_Stop (procId) ;
                    break ;
                case '6':
                    tmpStatus = BVR_OpenAllocator (TST_MQA_ID,
                                                   NUM_MSG_PER_POOL,
                                                   maxMsgSize) ;
                    break ;
                case '7':
                    tmpStatus = BVR_CloseAllocator (TST_MQA_ID) ;
                    break ;
                case '8':
                    tmpStatus = BVR_OpenTransport (TST_LMQT_ID,
                                                   TST_MQA_ID,
                                                   maxMsgSize,
                                                   MAX_MSGQS) ;
                    break ;
                case '9':
                    tmpStatus = BVR_CloseTransport (TST_LMQT_ID) ;
                    break ;
                case 'A':
                    tmpStatus = BVR_OpenTransport (TST_RMQT_ID,
                                                   TST_MQA_ID,
                                                   maxMsgSize,
                                                   MAX_MSGQS) ;
                    break ;
                case 'B':
                    tmpStatus = BVR_CloseTransport (TST_RMQT_ID) ;
                    break ;
                case 'C':
                    do {
                    tmpStatus = MSGQ_Locate (procId,
                                             msgqOutId,
                                             &msgqLocateAttrs) ;
                        TST_Sleep (timeOut) ;
                        timeOut *= 2 ;
                    } while (   (tmpStatus == DSP_ENOTFOUND)
                             && (ignoreStatus != TRUE)
                             && (expStatus != DSP_ENOTFOUND)) ;
                    break ;
                case 'D':
                    tmpStatus = MSGQ_Release (procId, msgqOutId) ;
                    break ;
                case 'E':
                    tmpStatus = MSGQ_Create (msgqInId, NULL) ;
                    break ;
                case 'F':
                    tmpStatus = MSGQ_Delete (msgqInId) ;
                    break ;
                case 'G':
                    tmpStatus = MSGQ_Alloc (TST_MQA_ID, maxMsgSize, &msg) ;
                    break ;
                case 'H':
                    tmpStatus = MSGQ_Free (msg) ;
                    if (DSP_SUCCEEDED (tmpStatus)) {
                        msg = NULL ;
                    }
                    break ;
                case 'I':
                    tmpStatus = MSGQ_Put (procId, msgqOutId, msg,
                                          msgId, msgqInId) ;
                    if (DSP_SUCCEEDED (tmpStatus)) {
                        msg = NULL ;
                    }
                    break ;
                case 'J':
                    tmpStatus = MSGQ_Get (msgqInId, WAIT_FOREVER, &msg) ;
                    break ;
                case 'K':
                    tmpStatus = MSGQ_GetReplyId (msg,
                                                 &procId,
                                                 &msgqOutId) ;
                    break ;
                case '\\':
                    ignoreStatus = TRUE ;
                    break ;
                default:
                    tmpStatus = DSP_ENOTIMPL ;
                    TST_PrnError ("Unimplemented transition in transition"
                                  "string. Status = [0x%x]", tmpStatus) ;
                    break ;
                }
                if (transition != '\\') {
                    if (ignoreStatus == TRUE) {
                        ignoreStatus = FALSE ;
                    }
                    else {
                        status = tmpStatus ;
                    }
                }
            }
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        BVR_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;
        statusOfCleanup = PROC_Destroy () ;
        if (DSP_FAILED (statusOfCleanup) && DSP_SUCCEEDED (status)) {
            TST_PrnError ("Failed on PROC_Destroy. Status = [0x%x]",
                          statusOfCleanup) ;
            status = DSP_EFAIL ;
        }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
