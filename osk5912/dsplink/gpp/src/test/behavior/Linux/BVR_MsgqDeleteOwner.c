/** ============================================================================
 *  @file   BVR_MsgqDeleteOwner.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior\Linux
 *
 *  @desc   Implemetation of BVR_MsgqDeleteOwner test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

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
#include <BVR_PostProcess.h>
#include <BVR_MsgqDeleteOwner.h>
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
#define NUM_ARGS 4

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
 *  @const  OWNER_MSGQ_ID
 *
 *  @desc   Owner id of msgq.
 *  ============================================================================
 */
#define OWNER_MSGQ_ID 0

/** ============================================================================
 *  @const  NUM_MSG_PER_POOL
 *
 *  @desc   Number of messages in each buffer pool.
 *  ============================================================================
 */
#define NUM_MSG_PER_POOL 5

/** ============================================================================
 *  @const  DSP_MSGQ_ID
 *
 *  @desc   ID of the MSGQ to be created on the DSP.
 *  ============================================================================
 */
#define DSP_MSGQ_ID 0

/** ============================================================================
 *  @const  GPP_MSGQ_ID
 *
 *  @desc   ID of the MSGQ to be created on the GPP.
 *  ============================================================================
 */
#define GPP_MSGQ_ID 0

/** ============================================================================
 *  @const  NUM_MSG
 *
 *  @desc   Number of messages to be exchanged.
 *  ============================================================================
 */
#define NUM_MSG 0

/** ============================================================================
 *  @name   MAX_THREADS
 *
 *  @desc   Maximum no of threads which can connect to the processor.
 *  ============================================================================
 */
#define MAX_THREADS    16


/** ----------------------------------------------------------------------------
 *  @func   MsgqDeleteFxn
 *
 *  @desc   Function that is spawned as a thread. It attempts to delete a
 *          queue created by another thread.
 *
 *  @arg    arguments
 *              All the required arguments packed into the ArgsToThread struct.
 *
 *  @ret    None
 *
 *  @enter  arguments should not be NULL
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
MsgqDeleteFxn (Void * arguments) ;


/** ============================================================================
 *  @func   BVR_MsgqDeleteOwner
 *
 *  @desc   The BVR_MsgqDeleteOwner test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MsgqDeleteOwner (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup   = DSP_SOK ;
    DSP_STATUS      statusOfCleanup = DSP_SOK ;
    DSP_STATUS      status          = DSP_SOK ;
    ProcessorId     procId          = 0       ;
    Uint16          numMsg          = 0       ;
    Uint16          numThreads      = 1       ;
    Char8 **        dspArgs         = NULL    ;

    ArgsToThread    arguments [MAX_THREADS]        ;
    pthread_t       threadIdentifier [MAX_THREADS] ;
    Uint16          i                              ;
    Uint16          maxMsgSize                     ;

    DBC_Require (argc == NUM_ARGS) ;
    DBC_Require (argv != NULL) ;

    if ((argc != NUM_ARGS) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */
        procId = TST_StringToInt (argv [2]) ;

        maxMsgSize = ALIGN (sizeof (TestMsg)) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        dspArgs = BVR_CreateDspArgumentList (NUM_DSP_ARGS,
                                             NUM_MSG_PER_POOL,
                                             maxMsgSize,
                                             MAX_GPP_MSGQS,
                                             DSP_MSGQ_ID,
                                             GPP_MSGQ_ID,
                                             NUM_MSG) ;

        statusOfSetup = BVR_SetupMessaging (procId,
                                            NUM_MSG_PER_POOL,
                                            maxMsgSize,
                                            MAX_GPP_MSGQS,
                                            argv [3],
                                            NUM_DSP_ARGS,
                                            dspArgs) ;


        BVR_FreeDspArgumentList (NUM_DSP_ARGS, dspArgs) ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = MSGQ_Create (OWNER_MSGQ_ID, NULL) ;
            if (DSP_FAILED (statusOfSetup)) {
                TST_PrnError ("MSGQ_Create failed. Status = [0x%x]",
                              statusOfSetup) ;
            }
        }

        /*  --------------------------------------------------------------------
         *  Test if only owner can delete the queue.
         *  --------------------------------------------------------------------
         */
        for (i = 0 ; i < numThreads ; i++) {
            arguments [i].procId = procId ;
            arguments [i].msgqId = OWNER_MSGQ_ID ;
            arguments [i].numMsg = numMsg ;
            pthread_create(&(threadIdentifier [i]), NULL,
                           (Void *) MsgqDeleteFxn, (Void *) &(arguments[i])) ;
        }

        /* Now check for all statuses */
        for (i = 0 ; DSP_SUCCEEDED (status) && (i < numThreads) ; i++) {
            pthread_join (threadIdentifier [i], NULL) ;
            if (arguments [i].status != DSP_EACCESSDENIED) {
                TST_PrnError ("Unexpected MSGQ_Delete status for non-owner "
                              "queue. Status = [0x%x]", status) ;
                status = DSP_EFAIL ;
            }
        }

        status = MSGQ_Delete (OWNER_MSGQ_ID) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("MSGQ_Delete failed. Status = [0x%x]", status) ;
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        statusOfCleanup = BVR_CleanupMessaging (procId) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (statusOfCleanup)) {
            status = DSP_EFAIL ;
        }
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MsgqDeleteFxn
 *
 *  @desc   Function that is spawned as a thread. It attempts to delete a
 *          queue created by another thread.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
Void
MsgqDeleteFxn (Void * arguments)
{
    DSP_STATUS      status    = DSP_SOK ;
    ProcessorId     procId              ;
    ArgsToThread *  args                ;

    args   = (ArgsToThread *) arguments ;
    procId = args->procId ;
    /*  --------------------------------------------------------------------
     *  Try deleting the queue.
     *  --------------------------------------------------------------------
     */
    status = MSGQ_Delete (OWNER_MSGQ_ID) ;
    args->status = status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
