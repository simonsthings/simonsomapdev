/** ============================================================================
 *  @file   STS_MsgqMemory.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress
 *
 *  @desc   Implementation of STS_MsgqMemory test case.
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
#include <STS_MsgqMemory.h>
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
 *  @macro  MAX_NUM_MSG
 *
 *  @desc   The maximum number of messages in any pool of the allocator.
 *  ============================================================================
 */
#define MAX_NUM_MSG 128

/** ============================================================================
 *  @macro  NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages to be configured for each pool of the allocator.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL     10


/** ============================================================================
 *  @func   STS_MsgqMemory
 *
 *  @desc   The STS_MsgqMemory test checks the proper freeing of the memory
 *          during put. It also tests the allocator functionality by stressing
 *          it to run out of memory.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_MsgqMemory (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status          = DSP_SOK          ;
    DSP_STATUS      expectedStatus  = DSP_SOK          ;

    ProcessorId     procId          = 0                ;
    MsgQueueId      msgqDspId       = 0                ;
    Uint16          numMsg          = 1                ;
    Uint16          maxMsgSize      = sizeof (TestMsg) ;
    Uint16          maxLocalQueues  = 1                ;
    Uint16          msgId           = 0                ;
    Char8 **        dspArgs         = NULL             ;
    MsgqLocateAttrs msgqLocateAttrs = {WAIT_FOREVER}   ;
    Uint32          timeout         = 100              ;

    MsgqMsg         msg [MAX_NUM_MSG] ;
    Int16  i ;
    Uint16 j ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }
    else {
        TST_PrnInfo ("This test needs visual inspection also.") ;
        TST_PrnInfo ("Please enable trace prints for ID_OSAL_MEM and set "
                     "severity level to 4\n") ;

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        status      = TST_StrToStatus (argv [0], &expectedStatus) ;
        procId      = TST_StringToInt (argv [1]) ;
        numMsg      = TST_StringToInt (argv [2]) ;
        maxMsgSize  = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = STS_CreateDspArgumentList (NUM_DSP_ARGS,
                                             NUM_MSG_PER_POOL,
                                             maxMsgSize,
                                             maxLocalQueues,
                                             msgqDspId,
                                             MSGQ_INVALID_ID,
                                             numMsg) ;

        status = STS_SetupMessaging (procId,
                                     NUM_MSG_PER_POOL,
                                     maxMsgSize,
                                     maxLocalQueues,
                                     argv [3],
                                     NUM_DSP_ARGS,
                                     dspArgs) ;

        STS_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

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
        else {
            TST_PrnError ("Failed to setup MSGQ. Status = [0x%x]",
                          status) ;
        }

        /*  --------------------------------------------------------------------
         *  Test proper allocation and freeing of messages
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; ((i < MAX_NUM_MSG) && DSP_SUCCEEDED (status)) ; i++) {
            status = MSGQ_Alloc (TST_MQA_ID, maxMsgSize, & msg [i]) ;
        }

        status = DSP_FAILED (status) ? DSP_SOK : DSP_EFAIL ;

        for (i -= 2, j = 0 ;
             ((j < numMsg) && DSP_SUCCEEDED (status)) ;
             i--, j++) {
            status = MSGQ_Put (procId,
                               msgqDspId,
                               msg [i],
                               msgId,
                               MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Put failed. Status = [0x%x]",
                              status) ;
            }
        }

        /* Sleep half a second to let the messages go. */
        TST_Sleep (500000) ;

        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Alloc (TST_MQA_ID, maxMsgSize, &(msg [++i])) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Alloc failed. Status = [0x%x]",
                              status) ;
            }
        }

        /*
         *  Free the buffers that were allocated earlier and not freed
         *  by the MSGQ_Put () call.
         */
        for ( ; ((i >= 0) && DSP_SUCCEEDED (status)) ; i--) {
            status = MSGQ_Free (msg [i]) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("MSGQ_Free failed. Status = [0x%x]",
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
