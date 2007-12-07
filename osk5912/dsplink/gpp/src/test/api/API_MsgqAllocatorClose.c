/** ============================================================================
 *  @file   API_MsgqAllocatorClose.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_AllocatorClose API test case.
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
#include <API_MsgqAllocatorClose.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 4

/** ============================================================================
 *  @name   NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages in each buffer pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL 5

/** ============================================================================
 *  @const  ID_PROCESSOR
 *
 *  @desc   The processor id of the processor being used.
 *  ============================================================================
 */
#define ID_PROCESSOR 0


/** ============================================================================
 *  @func   API_MsgqAllocatorClose
 *
 *  @desc   Tests the MSGQ_AllocatorClose API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqAllocatorClose (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  statusOfSetup   = DSP_SOK ;
    DSP_STATUS  statusOfCleanup = DSP_SOK ;
    DSP_STATUS  status          = DSP_SOK ;
    AllocatorId mqaOpenId       = 0       ;
    AllocatorId mqaCloseId      = 0       ;
    Uint32      temp                      ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqAllocatorClose. "
                      "Status = [0x%x]", status) ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = TST_MacroToIntValue (&temp,
                                             MAX_ALLOCATORS,
                                             argv [2],
                                             "MAX_ALLOCATORS") ;
        if (DSP_SUCCEEDED (statusOfSetup)) {
            mqaOpenId = (AllocatorId) temp ;
        }
        else if (statusOfSetup == DSP_ENOTFOUND) { /* Not a macro value */
            mqaOpenId     = TST_StringToInt (argv [2]) ;
            statusOfSetup = DSP_SOK ;
        }

        statusOfSetup = TST_MacroToIntValue (&temp,
                                             MAX_ALLOCATORS,
                                             argv [3],
                                             "MAX_ALLOCATORS") ;
        if (DSP_SUCCEEDED (statusOfSetup)) {
            mqaCloseId = (AllocatorId) temp ;
        }
        else if (statusOfSetup == DSP_ENOTFOUND) { /* Not a macro value */
            mqaCloseId    = TST_StringToInt (argv [3]) ;
            statusOfSetup = DSP_SOK ;
        }

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("PROC_Setup failed. Status = [0x%x]",
                          statusOfSetup) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Attach (ID_PROCESSOR, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("PROC_Attach failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenAllocator (mqaOpenId,
                                               NUM_MSG_PER_POOL,
                                               RMQT_CTRLMSG_SIZE) ;
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_AllocatorClose.
         *  --------------------------------------------------------------------
         */

        status = MSGQ_AllocatorClose (mqaCloseId) ;

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        if ((mqaOpenId != mqaCloseId) && DSP_SUCCEEDED (statusOfSetup)) {
            statusOfCleanup = API_CloseAllocator (mqaOpenId) ;
        }

        statusOfCleanup = PROC_Detach (ID_PROCESSOR) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("PROC_Detach failed. Status = [0x%x]",
                          statusOfCleanup) ;
        }

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
