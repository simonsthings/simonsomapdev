/** ============================================================================
 *  @file   API_MsgqTransportOpen.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of MSGQ_TransportOpen API test case.
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
#include <API_MsgqTransportOpen.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  MQT_ATTRS_NULL_PTR
 *          MQT_ATTRS_CORRECT_STRUCTURE
 *          MQT_ATTRS_INVALID_MAX_QUEUE
 *
 *  @desc   Argument types for mqtAttrsType
 *  ============================================================================
 */
#define MQT_ATTRS_NULL_PTR          0
#define MQT_ATTRS_CORRECT_STRUCTURE 1
#define MQT_ATTRS_INVALID_MAX_QUEUE 2

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
 *  @const  DSP_MSGQ_ID
 *
 *  @desc   ID of the MSGQ to be created on the DSP.
 *  ============================================================================
 */
#define DSP_MSGQ_ID 0

/** ============================================================================
 *  @const  NUM_MSG
 *
 *  @desc   Number of messages to be exchanged.
 *  ============================================================================
 */
#define NUM_MSG 1

/** ============================================================================
 *  @const  ID_PROCESSOR
 *
 *  @desc   The processor id of the processor being used.
 *  ============================================================================
 */
#define ID_PROCESSOR 0


/** ============================================================================
 *  @func   API_MsgqTransportOpen
 *
 *  @desc   Tests the MSGQ_TransportOpen API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_MsgqTransportOpen (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  statusOfSetup   = DSP_SOK            ;
    DSP_STATUS  statusOfCleanup = DSP_SOK            ;
    DSP_STATUS  status          = DSP_SOK            ;
    ProcessorId procId          = 0                  ;
    TransportId mqtId           = 0                  ;
    Uint8       mqtAttrsType    = MQT_ATTRS_NULL_PTR ;
    Pvoid       mqtAttrsPtr     = NULL               ;
    LmqtAttrs   lmqtAttrs       = {MAX_MSGQS}        ;
    RmqtAttrs   rmqtAttrs       = {MAX_MSGQS,
                                   0,
                                   0}                ;
    Char8 **    dspArgs         = NULL               ;
    Uint16      maxMsgSize                           ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_MsgqTransportOpen. "
                      "Status = [0x%x]", status) ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        procId       = TST_StringToInt (argv [2]) ;
        mqtId        = TST_StringToInt (argv [3]) ;
        mqtAttrsType = TST_StringToInt (argv [4]) ;

        if (mqtAttrsType != MQT_ATTRS_NULL_PTR) {
            mqtAttrsPtr = (mqtId == 0)
                        ? (Pvoid) &lmqtAttrs
                        : (Pvoid) &rmqtAttrs ;
        }
        if (mqtAttrsType == MQT_ATTRS_INVALID_MAX_QUEUE) {
            lmqtAttrs.maxNumMsgq += 2 ;
            rmqtAttrs.maxNumMsgq += 2 ;
        }
        maxMsgSize = ALIGN (sizeof (TestMsg)) ;

        rmqtAttrs.defaultMqaId = TST_MQA_ID ;
        rmqtAttrs.maxMsgSize = maxMsgSize ;

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
                                                 DSP_MSGQ_ID,
                                                 0, /* dummy argument */
                                                 NUM_MSG) ;

            statusOfSetup = PROC_Load (ID_PROCESSOR, argv [5],
                                       NUM_DSP_ARGS, dspArgs) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("PROC_Load failed. Status = [0x%x]",
                              statusOfSetup) ;
            }

            API_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Start (ID_PROCESSOR) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("PROC_Start failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Tests MSGQ_TransportOpen.
         *  --------------------------------------------------------------------
         */
        status = MSGQ_TransportOpen (mqtId, mqtAttrsPtr) ;

        status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        /*  --------------------------------------------------------------------
         *  Cleanup.
         *  --------------------------------------------------------------------
         */
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
