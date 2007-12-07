/** ============================================================================
 *  @file   BVR_MultiProcessOpen.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior\Linux
 *
 *  @desc   Implementation of behavior test to open DSP from multiple processes.
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
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>
#include <dsplink.h>

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

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <TST_Print.h>
#include <BVR_MultiProcessOpen.h>
#include <BVR_PostProcess.h>
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MAX_PROCESSES
 *
 *  @desc   Maximum no of processes which can connect to the processor.
 *  ============================================================================
 */
#define MAX_PROCESSES    16

/** ============================================================================
 *  @name   MAX_ARGS
 *
 *  @desc   The maximum no of arguments which can be passed to DSP.
 *  ============================================================================
 */
#define MAX_ARGS 10


/** ============================================================================
 *  @name   xferCycles
 *
 *  @desc   This is the number of times the whole test will be run.
 *  ============================================================================
 */
Uint32 xferCycles = 0 ;


/** ============================================================================
 *  @func   DataXferFunction
 *
 *  @desc   Function that is spawned as a process.
 *
 *  @arg    dspId
 *              Count of the no. of arguments passed.
 *  @arg    dspId
 *              Processor Id.
 *  @arg    channelId
 *              Channel Id.
 *  @arg    bufferLength
 *              The length of the buffer to be transferred.
 *  @arg    numChirps
 *              Queuing length specified by the user.
 *
 *  @ret    0
 *              Operation Successfully completed.
 *          1
 *              Error in execution of test.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
STATIC
int
DataXferFunction (ProcessorId  dspId,
                  ChannelId    channelId,
                  Uint32       bufferLength,
                  Uint32       numChirps) ;


/** ============================================================================
 *  @func   BVR_MultiProcessOpen
 *
 *  @desc   Function to test the behavior when the DSP is attached
 *          from multiple processes.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MultiProcessOpen (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS   status        = DSP_SOK           ;
    DSP_STATUS   statusOfSetup = DSP_SOK           ;
    Uint32       numProcesses  = 0                 ;
    ProcessorId  dspId         = 0                 ;
    Uint32       i             = 0                 ;
    Uint32       bufferLength  = 0                 ;
    Uint32       numChirps     = 0                 ;
    int          isChild       = 0                 ;
    int          ret           = 0                 ;
    int          childId  [MAX_PROCESSES]          ;
    int          processIdentifier [MAX_PROCESSES] ;
    Uint32       noOfArguments                     ;
    Char8  *     arrayOfArguments [MAX_ARGS]       ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 8) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 8) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of this test. Status = [0x%x]", status) ;
    }

    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        xferCycles   = TST_StringToInt (argv [2]) ;
        dspId        = TST_StringToInt (argv [3]) ;
        numProcesses = TST_StringToInt (argv [4]) ;
        bufferLength = TST_StringToInt (argv [6]) ;
        numChirps    = TST_StringToInt (argv [7]) ;
        statusOfSetup = PROC_Setup () ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Call to PROC_Setup Failed. Status = [0x%x]",
                          statusOfSetup) ;
        }
    }

    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = PROC_Attach (dspId, NULL) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to attach to DSP. Status = [0x%x]",
                          statusOfSetup) ;
        }
    }

    if (DSP_SUCCEEDED (statusOfSetup)) {
        noOfArguments = 4 ;
        arrayOfArguments [0] = argv [2] ;
        arrayOfArguments [1] = argv [6] ;
        arrayOfArguments [2] = argv [7] ;
        arrayOfArguments [3] = argv [4] ;
        status = PROC_Load (i,
                            argv [5],
                            noOfArguments,
                            arrayOfArguments) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to Load on DSP. Status = [0x%x]",
                          statusOfSetup) ;
        }
    }

    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = PROC_Start (dspId) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to start DSP. Status = [0x%x]",
                          statusOfSetup) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Test BVR_MultiProcessOpen.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        for (i = 0 ;(i < numProcesses) && (isChild == 0) ; i++) {
            childId [i] = fork() ;

            if (childId [i] == 0) {
                isChild = 1 ;
            }
            if (childId [i] < 0) {
                status = DSP_EFAIL ;
                TST_PrnError ("Call to fork failed. Status = [0x%x]", status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            if (isChild == 0) {
                /* Now check for all status */
                for (i = 0 ; i < numProcesses ; i++) {
                    ret = waitpid (childId [i],
                                   &(processIdentifier[i]),
                                   WUNTRACED) ;
                    if (ret == -1) {
                        status = DSP_EFAIL ;
                        TST_PrnError ("Error exit from wait. Status = [0x%x]",
                                      status) ;
                    }
                    if (WEXITSTATUS (processIdentifier[i]) == 0) {
                        status = DSP_EFAIL ;
                        TST_PrnError ("Wait for child process failed. "
                                      "Status = [0x%x]", status) ;
                    }
                }
            }
            else  {
                processIdentifier [i-1] = DataXferFunction (dspId,
                                                            (i-1),
                                                            bufferLength,
                                                            numChirps) ;
                exit (processIdentifier [i-1]) ;
            }
        }
    }

    TST_Print ("\n") ;
    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    PROC_Stop (dspId) ;
    PROC_Detach (dspId) ;
    PROC_Destroy ()     ;

    return status ;

}


/** ----------------------------------------------------------------------------
 *  @func   DataXferFunction
 *
 *  @desc   Function that is called from the created process.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
NORMAL_API
STATIC
int
DataXferFunction (ProcessorId  dspId,
                  ChannelId    channelId,
                  Uint32       bufferLength,
                  Uint32       numChirps)
{
    DSP_STATUS    status          = DSP_SOK ;
    DSP_STATUS    tempStatus      = DSP_SOK ;
    int           retStatus       = 1       ;
    Uint32        i               = 0       ;
    Uint32        numXfr          = 0       ;
    ChannelMode   channelMode               ;
    ChannelAttrs  attrs                     ;
    Char8 *       bufferArray [1]           ;
    ChannelIOInfo ioInfo                    ;

    status = PROC_Attach (dspId, NULL) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed to attach to DSP. Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        if ((channelId % 2) == 0) {
            channelMode = ChannelMode_Output ;
        }
        else {
            channelMode = ChannelMode_Input ;
        }
        attrs.mode      = channelMode            ;
        attrs.size      = ChannelDataSize_16bits ;
        attrs.endianism = Endianism_Default      ;
        status = CHNL_Create (dspId, channelId, &attrs) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to create channel. Status = [0x%x]", status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (dspId,
                                      channelId,
                                      bufferArray,
                                      bufferLength,
                                      1) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed to allocate buffer on data channel. "
                          "Status = [0x%x]", status) ;
        }
    }

    /* Repeat the Cycles for 'xferCycles' number of times */
    for (numXfr = 0 ; (numXfr < xferCycles) && (DSP_SUCCEEDED (status)) ; numXfr++) {
        /* Transfer the buffers to DSP for numChirps times */
        for (i = 0 ; (i < numChirps) && (DSP_SUCCEEDED (status)) ; i++) {
            ioInfo.buffer = bufferArray [0] ;
            ioInfo.size = bufferLength ;
            status = CHNL_Issue (dspId, channelId, &ioInfo) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Issue Failed. Status = [0x%x]",
                              status) ;
            }

            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (dspId,
                                       channelId,
                                       WAIT_FOREVER,
                                       &ioInfo) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("Call to CHNL_Reclaim Failed. "
                                  "Status = [0x%x]", status) ;
                }
            }
        }
        /* Progress print every 10 cycle */
        if (numXfr % 10 == 0) {
            TST_Print (".") ;
        }
    }

    tempStatus = CHNL_FreeBuffer (dspId,
                                  channelId,
                                  (Char8 **) bufferArray,
                                  1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't free buffers. Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_Delete (dspId, channelId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete channel. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Detach (dspId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't call PROC_Detach. Status = [0x%x]", status) ;
    }

    if (DSP_FAILED (status)) {
        retStatus = 0 ;
    }

    return retStatus ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
