/** ============================================================================
 *  @file   STS_MsgqMemoryLeakAllocator.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress
 *
 *  @desc   Implementation of STS_MsgqMemoryLeakAllocator test case.
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
#include <STS_MsgqMemoryLeakAllocator.h>
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
 *  @func   STS_MsgqMemoryLeakAllocator
 *
 *  @desc   This test tests for memory leak in the allocator.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_MsgqMemoryLeakAllocator (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status          = DSP_SOK ;
    DSP_STATUS      tmpStatus       = DSP_SOK ;
    DSP_STATUS      expectedStatus  = DSP_SOK ;
    Uint16          numIters        = 1       ;
    Uint16          numMsgPerPool   = 0       ;
    Uint16          maxMsgSize      = 0       ;

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
        status        = TST_StrToStatus (argv [0], &expectedStatus) ;
        numIters      = TST_StringToInt (argv [1]) ;
        numMsgPerPool = TST_StringToInt (argv [2]) ;
        maxMsgSize    = TST_StringToInt (argv [3]) ;
        maxMsgSize    =   maxMsgSize > RMQT_CTRLMSG_SIZE
                        ? maxMsgSize : RMQT_CTRLMSG_SIZE ;
        maxMsgSize    =   maxMsgSize > sizeof (TestMsg)
                        ? maxMsgSize : sizeof (TestMsg) ;
        maxMsgSize    = ALIGN (maxMsgSize) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        status = PROC_Setup () ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("PROC_Setup failed. Status = [0x%x]", status) ;
        }

        /*  --------------------------------------------------------------------
         *  Tests successive allocator open and close
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; ((i < numIters) && DSP_SUCCEEDED (status)) ; i++) {
            status = STS_OpenAllocator (TST_MQA_ID, numMsgPerPool, maxMsgSize) ;
            if (DSP_SUCCEEDED (status)) {
                status = STS_CloseAllocator (TST_MQA_ID) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("STS_CloseAllocator failed. Status = [0x%x]",
                                  status) ;
                }
            }
            else {
                TST_PrnError ("STS_OpenAllocator failed. Status = [0x%x]",
                              status) ;
            }
        }

        tmpStatus = PROC_Destroy () ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
            TST_PrnError ("PROC_Destroy failed. Status = [0x%x]",
                          status) ;
            status = DSP_EFAIL ;
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
