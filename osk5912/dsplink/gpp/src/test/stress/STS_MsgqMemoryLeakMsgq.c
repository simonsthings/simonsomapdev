/** ============================================================================
 *  @file   STS_MsgqMemoryLeakMsgq.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress
 *
 *  @desc   Implementation of STS_MsgqMemoryLeakMsgq test case.
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
#include <STS_MsgqMemoryLeakMsgq.h>
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
 *  @func   STS_MsgqMemoryLeakMsgq
 *
 *  @desc   Tests memory leaks in consecutive create and delete
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_MsgqMemoryLeakMsgq (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status          = DSP_SOK        ;
    DSP_STATUS      expectedStatus  = DSP_SOK        ;
    ProcessorId     procId          = 0              ;
    Uint16          maxMsgSize      = 0              ;
    Uint16          maxLocalQueues  = 1              ;
    MsgQueueId      msgqOutId       = 1              ;
    MsgQueueId      msgqInId        = 0              ;
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
                                             msgqOutId,
                                             msgqInId,
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
         *  Test consecutive create and delete.
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; ((i < numIters) && DSP_SUCCEEDED (status)) ; i++) {
            status = MSGQ_Create (msgqInId, NULL) ;
            if (DSP_SUCCEEDED (status)) {
                status = MSGQ_Delete (msgqInId) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("MSGQ_Delete failed. Status = [0x%x]",
                                  status) ;
                }
            }
            else {
                TST_PrnError ("MSGQ_Create failed. Status = [0x%x]",
                              status) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Cleanup
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            status = DSP_ENOTFOUND ;
            while (status == DSP_ENOTFOUND) {
                status = MSGQ_Locate (procId,
                                      msgqOutId,
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
            status = MSGQ_Put (procId, msgqOutId, msg, msgId, MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Release (procId, msgqOutId) ;
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
