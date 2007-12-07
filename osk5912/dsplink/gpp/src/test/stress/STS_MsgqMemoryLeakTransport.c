/** ============================================================================
 *  @file   STS_MsgqMemoryLeakTransport.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress
 *
 *  @desc   Implementation of STS_MsgqMemoryLeakTransport test case.
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

/*  ----------------------------------- Link API                      */
#include <proc.h>
#include <chnl.h>
#include <msgqdefs.h>
#include <msgq.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <TST_Sleep.h>
#include <STS_MsgqMemoryLeakTransport.h>
#include <STS_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 4

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
 *  @func   STS_MsgqMemoryLeakTransport
 *
 *  @desc   Tests memory leaks by continuously opening and closing the transport.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_MsgqMemoryLeakTransport (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status          = DSP_SOK        ;
    DSP_STATUS      expectedStatus  = DSP_SOK        ;
    ProcessorId     procId          = 0              ;
    Uint16          maxMsgSize      = 0              ;
    Uint16          maxLocalQueues  = 1              ;
    MsgQueueId      msgqDspId       = 0              ;
    MsgQueueId      msgqGppId       = 1              ;
    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER} ;
    Uint16          msgId           = 0              ;
    Uint16          numIters        = 1              ;
    Uint16          numMsg          = 1              ;
    Uint32          timeout         = 100            ;
    MsgqMsg         msg             = NULL           ;
    Char8 **        dspArgs         = NULL           ;

    Uint16 i ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }
    else {
        TST_PrnInfo ("This is a visual test.") ;
        TST_PrnInfo ("Please enable trace prints for ID_OSAL_MEM and set "
                     "severity level to 4\n") ;

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        status     = TST_StrToStatus (argv [0], &expectedStatus) ;
        procId     = TST_StringToInt (argv [1]) ;
        numIters   = TST_StringToInt (argv [2]) ;
        maxMsgSize = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = STS_CreateDspArgumentList (NUM_DSP_ARGS,
                                             NUM_MSG_PER_POOL,
                                             maxMsgSize,
                                             maxLocalQueues,
                                             msgqDspId,
                                             msgqGppId,
                                             numMsg) ;

        status = STS_SetupMessaging (procId,
                                     NUM_MSG_PER_POOL,
                                     maxMsgSize,
                                     maxLocalQueues,
                                     argv [3],
                                     NUM_DSP_ARGS,
                                     dspArgs) ;

        STS_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to setup messaging. Status = [0x%x]",
                          status) ;
        }

        /*  --------------------------------------------------------------------
         *  Test consecutive open and close of transport.
         *
         *  The transports need to be closed first as they
         *  are opened in STS_SetupMessaging.
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; ((i < numIters) && DSP_SUCCEEDED (status)) ; i++) {
            status = STS_CloseTransport (TST_RMQT_ID) ;
            if (DSP_SUCCEEDED (status)) {
                status = STS_CloseTransport (TST_LMQT_ID) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("STS_CloseTransport LMQT failed. "
                                  "Status = [0x%x]", status) ;
                }
            }
            else {
                TST_PrnError ("STS_CloseTransport RMQT failed. "
                              "Status = [0x%x]", status) ;
            }
            if (DSP_SUCCEEDED (status)) {
                status = STS_OpenTransport (TST_LMQT_ID,
                                            TST_MQA_ID,
                                            maxMsgSize,
                                            maxLocalQueues) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("STS_OpenTransport LMQT failed. "
                                  "Status = [0x%x]", status) ;
                }
            }
            if (DSP_SUCCEEDED (status)) {
                status = STS_OpenTransport (TST_RMQT_ID,
                                            TST_MQA_ID,
                                            maxMsgSize,
                                            maxLocalQueues) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("STS_OpenTransport RMQT failed. "
                                  "Status = [0x%x]", status) ;
                }
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = DSP_ENOTFOUND ;
            while (status == DSP_ENOTFOUND) {
                status = MSGQ_Locate (procId,
                                      msgqDspId,
                                      &msgqLocateAttrs) ;
                if (DSP_FAILED (status) && status != DSP_ENOTFOUND) {
                TST_PrnError ("MSGQ_Locate failed. Status = [0x%x]",
                              status) ;
            }
                else if (status == DSP_ENOTFOUND) {
                    TST_Sleep (timeout) ;
                    timeout *= 2 ;
                }
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Alloc (TST_MQA_ID, maxMsgSize, &msg) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Put (procId, msgqDspId, msg, msgId, MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Release (procId, msgqDspId) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Release failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = STS_CleanupMessaging (procId) ;
        }

        if (status != expectedStatus) {
            status = DSP_EFAIL ;
        }
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
