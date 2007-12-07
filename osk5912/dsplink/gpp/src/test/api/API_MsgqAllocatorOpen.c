/** ============================================================================
 *  @file   API_MsgqAllocatorOpen.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_AllocatorOpen API test case.
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
#include <API_MsgqAllocatorOpen.h>


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
 *  @name   MAX_MSG_SIZE
 *
 *  @desc   The maximum size of the message configured for this test.
 *  ============================================================================
 */
#define MAX_MSG_SIZE                ALIGN (0x100)

/** ============================================================================
 *  @name   MQA_ATTRS_NULL_PTR
 *          MQA_ATTRS_CORRECT_STRUCTURE
 *
 *  @desc   Argument types for mqaAttrsPresent
 *  ============================================================================
 */
#define MQA_ATTRS_NULL_PTR          0
#define MQA_ATTRS_CORRECT_STRUCTURE 1


/** ============================================================================
 *  @func   API_MsgqAllocatorOpen
 *
 *  @desc   Tests the MSGQ_AllocatorOpen API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqAllocatorOpen (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  statusOfSetup   = DSP_SOK             ;
    DSP_STATUS  statusOfCleanup = DSP_SOK             ;
    DSP_STATUS  status          = DSP_SOK             ;
    AllocatorId mqaId           = 0                   ;
    Pvoid       mqaAttrsPtr     = NULL                ;
    Uint8       mqaAttrsPresent = MQA_ATTRS_NULL_PTR  ;
    Uint16      msgSize []      = {RMQT_CTRLMSG_SIZE,
                                   ALIGN (sizeof (TestMsg)),
                                   MAX_MSG_SIZE}      ;
    Uint16      numMsg  []      = {5, 5, 5}           ;
    MqaBufAttrs mqaAttrs        = {3,
                                   msgSize,
                                   numMsg}            ;
    Uint32      temp                                  ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqAllocatorOpen. "
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
            mqaId = (AllocatorId) temp ;
        }
        else if (statusOfSetup == DSP_ENOTFOUND) { /* Not a macro value */
            mqaId         = TST_StringToInt (argv [2]) ;
            statusOfSetup = DSP_SOK ;
        }

        mqaAttrsPresent = TST_StringToInt (argv [3]) ;
        if (mqaAttrsPresent != MQA_ATTRS_NULL_PTR) {
            mqaAttrsPtr      = (Pvoid) &mqaAttrs      ;
        }


        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed on PROC_Setup. Status = [0x%x]",
                          statusOfSetup) ;
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_AllocatorOpen.
         *  --------------------------------------------------------------------
         */

        status = MSGQ_AllocatorOpen (mqaId, mqaAttrsPtr) ;

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
        statusOfCleanup = MSGQ_AllocatorClose (mqaId) ;
        if (DSP_FAILED (statusOfCleanup)) {
            TST_PrnError ("Failed on MSGQ_AllocatorClose. Status = [0x%x]",
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
