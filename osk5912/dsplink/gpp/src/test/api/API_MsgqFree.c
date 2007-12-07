/** ============================================================================
 *  @file   API_MsgqFree.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_Free API test case.
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
#include <API_MsgqFree.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_ARGS
 *
 *  @desc   The number of arguments expected by this test.
 *  ============================================================================
 */
#define NUM_ARGS 3

/** ============================================================================
 *  @const  NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages in each buffer pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL 1

/** ============================================================================
 *  @name   BUFFER_PTR_TYPE_NULL
 *          BUFFER_PTR_TYPE_VALID
 *
 *  @desc   Argument types for bufferPtrType
 *  ============================================================================
 */
#define BUFFER_PTR_TYPE_NULL        0
#define BUFFER_PTR_TYPE_VALID       1


/** ============================================================================
 *  @func   API_MsgqFree
 *
 *  @desc   Tests the MSGQ_Free API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqFree (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  statusOfSetup    = DSP_SOK              ;
    DSP_STATUS  statusOfCleanup  = DSP_SOK              ;
    DSP_STATUS  status           = DSP_SOK              ;
    AllocatorId mqaId            = 0                    ;
    Uint8       bufferPtrType    = BUFFER_PTR_TYPE_NULL ;
    MsgqMsg     msgPtr           = NULL                 ;
    Uint16      maxMsgSize       = 0                    ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqFree. Status = [0x%x]",
                      status) ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        bufferPtrType = TST_StringToInt (argv [2]) ;
        maxMsgSize = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed on PROC_Setup. Status = [0x%x]",
                          statusOfSetup) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = API_OpenAllocator (mqaId,
                                               NUM_MSG_PER_POOL,
                                               maxMsgSize) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Alloc (mqaId, maxMsgSize, &msgPtr) ;
            if (DSP_FAILED (statusOfSetup)){
                TST_PrnError ("Failed on MSGQ_Alloc. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_Free.
         *  --------------------------------------------------------------------
         */

        if (bufferPtrType == BUFFER_PTR_TYPE_NULL) {
            status = MSGQ_Free (NULL) ;
        }
        else {
            status = MSGQ_Free (msgPtr) ;
        }

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        if (bufferPtrType == BUFFER_PTR_TYPE_NULL) {
            statusOfCleanup = MSGQ_Free (msgPtr) ;
            if (DSP_FAILED (statusOfCleanup)) {
                TST_PrnError ("Failed on MSGQ_Free. Status = [0x%x]",
                              statusOfCleanup) ;
            }
        }

        statusOfCleanup = API_CloseAllocator (mqaId) ;

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
