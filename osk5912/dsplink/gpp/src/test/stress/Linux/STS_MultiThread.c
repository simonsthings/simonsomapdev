/** ============================================================================
 *  @file   STS_MultiThread.c
 *
 *  @path   $(DSPLINK)\gpp\src\stress\Linux
 *
 *  @desc   Implementation of Multi threaded stress test.
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
#include <unistd.h>

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

/*  ----------------------------------- User API                      */
#include <chnl.h>
#include <proc.h>

/*  ----------------------------------- Testing Header                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_Sleep.h>
#include <TST_PrintFuncs.h>
#include <TST_Print.h>
#include <TST_RunTest.h>
#include <STS_MultiThread.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   STS_MAX_ALLOC_BUFFERS
 *
 *  @desc   Maximum buffer number of buffers that can be allocated for a
 *          channel
 *  ============================================================================
 */
#define STS_MAX_ALLOC_BUFFERS      16

/** ============================================================================
 *  @name   MAX_ARGS
 *
 *  @desc   The maximum no of arguments which can be passed to DSP.
 *  ============================================================================
 */
#define MAX_ARGS 10


/** ============================================================================
 *  @name   TransferInfo
 *
 *  @desc   Information needed for data transfer.
 *
 *  @field  procId
 *              Processer Id
 *  @field  channelId
 *              Channel Id
 *  @field  channelMode
 *              Input or Output Mode
 *  @field  numChirps
 *              Number of chirps
 *  @field  bufferLength
 *              Length of buffers to be transferred
 *  @field  delay
 *              Delay needed between consecutive issues/reclaim
 *  @field  xferCycles
 *              Number of times transfer to be repeated
 *  ============================================================================
 */
typedef struct TransferInfo_tag {
    ChannelId   channelId         ;
    ChannelMode channelMode       ;
    Uint32      numChirps         ;
    Uint32      bufferLength      ;
    Uint32      delay             ;
    Uint32      xferCycles        ;
    DSP_STATUS  returnStatus      ;
} TransferInfo ;


/** ============================================================================
 *  @name   TaskInfo
 *
 *  @desc   Information passed to task as second argument.
 *
 *  @field  argument
 *              argument that task can interpret
 *  ============================================================================
 */
typedef struct TaskInfo_tag {
    ProcessorId   procId       ;
    TransferInfo  xferInfo     ;
    DSP_STATUS  * returnStatus ;
} TaskInfo ;


/** ============================================================================
 *  @name   runContext
 *
 *  @desc   Context information passed to the thread for execution.
 *  ============================================================================
 */
STATIC TaskInfo runContext [MAX_PROCESSORS][MAX_CHANNELS]  ;


/** ============================================================================
 *  @name   returnStatus
 *
 *  @desc   Status returned by various threads.
 *  ============================================================================
 */
DSP_STATUS returnStatus [MAX_PROCESSORS][MAX_CHANNELS] ;


/** ----------------------------------------------------------------------------
 *  @func   FillSampleData
 *
 *  @desc   Fills up the data in the passed buffers.
 *
 *  @arg    bufferArray
 *              Array of buffer
 *          bufferLength
 *              Length of each buffer in the array
 *          numBuffers
 *              Number of buffers in the array
 *
 *  @ret    DSP_SOK
 *              Success
 *          DSP_EFAIL
 *              Failure
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
FillSampleData (Char8 ** bufferArray,
                Uint32   bufferLength,
                Uint32   numBuffers) ;


/** ----------------------------------------------------------------------------
 *  @func   VerifyDataIntegrity
 *
 *  @desc   Verifies the data integrity of received data
 *
 *  @arg    bufferArray
 *              Array of buffer
 *          bufferLength
 *              Length of each buffer in the array
 *          numBuffers
 *              Number of buffers in the array
 *
 *  @ret    DSP_SOK
 *              Success
 *          DSP_EFAIL
 *              Failure
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
VerifyDataIntegrity (Char8 ** bufferArray,
                     Uint32   bufferLength,
                     Uint32   numBuffers) ;


/** ----------------------------------------------------------------------------
 *  @func   STRESS_TEST_DoDataTransferTest
 *
 *  @desc   Exchange the data between the processors (MPU/DSP).
 *
 *  @arg    procId
 *              Processor Id
 *          channelId
 *              Channel Id
 *          channelMode
 *              Input or output mode
 *          numChirps
 *              Number of chirps to allocate for this transfer
 *          bufferLength
 *              Size of buffer to be used for transfer
 *          delay
 *              delay needed between consecutive issues/reclaims
 *          xferCycles
 *              Number of times to repeat transfer of buffers
 *
 *  @ret    DSP_SOK
 *              Success
 *          DSP_EFAIL
 *              Failure
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
STRESS_TEST_DoDataTransferTest (IN ProcessorId procId,
                                IN ChannelId   channelId,
                                IN ChannelMode channelMode,
                                IN Uint32      numChirps,
                                IN Uint32      bufferLength,
                                IN Uint32      delay,
                                IN Uint32      xferCycles) ;


/** ----------------------------------------------------------------------------
 *  @func   TaskDoTransfer
 *
 *  @desc   Multiple channel transfer stress test task entry function
 *
 *  @arg    expectedStatus
 *              Status expected
 *          param
 *              This gives array containing information structure regarding
 *              the transfer that has to be done.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
EXPORT_API
Void
TaskDoTransfer (Void *   param) ;


/** ============================================================================
 *  @func   STS_MultiThread
 *
 *  @desc   Multi threaded stress test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_MultiThread (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  status                                         = DSP_SOK ;
    DSP_STATUS  tempStatus                                     = DSP_SOK ;
    DSP_STATUS  expectedStatus                                 = DSP_SOK ;
    Uint32      numChirps                                      = 0       ;
    Uint32      bufferLength                                   = 100     ;
    Uint32      xferCycles                                     = 100     ;
    Uint32      delay                                          = 0       ;
    Uint32      numThreads                                     = 0       ;
    ChannelMode channelMode                                              ;
    Uint32      i                                                        ;
    Uint32      j                                                        ;
    ProcAttr    attrs                                                    ;
    Uint32      noOfArguments                                            ;
    Char8  *    arrayOfArguments [MAX_ARGS]                              ;
    pthread_t   threadIdentifier[MAX_PROCESSORS][MAX_CHANNELS]           ;

    DBC_Require (argc == 7) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 7) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Wrong number of arguments. Status = [0x%x]", status) ;
    }
    if (DSP_SUCCEEDED (status)) {
        status       = TST_StrToStatus (argv [0], &expectedStatus) ;
        xferCycles   = TST_StringToInt (argv [1])                  ;
        bufferLength = TST_StringToInt (argv [2])                  ;
        numChirps    = TST_StringToInt (argv [3])                  ;
        delay        = TST_StringToInt (argv [4])                  ;
        numThreads    = TST_StringToInt (argv [6])                 ;
        if (numThreads > MAX_CHANNELS) {
            status = DSP_EINVALIDARG ;
            TST_PrnError ("The number of threads to be created cannot be "
                          "more than supported number of channels. "
                          "Status = [0x%x]", status) ;
        }
    }
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Setup () ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("PROC_Setup failed. Status = [0x%x]", status) ;
        }
    }
    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; (i < MAX_PROCESSORS) ; i++) {
            if (DSP_SUCCEEDED (status)) {
                status = PROC_Attach (i, &attrs) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("PROC_Attach failed. Status = [0x%x]", status) ;
                }
            }
            if (DSP_SUCCEEDED (status)) {
                noOfArguments = 4 ;
                arrayOfArguments [0] = argv [1] ;
                arrayOfArguments [1] = argv [2] ;
                arrayOfArguments [2] = argv [3] ;
                arrayOfArguments [3] = argv [6] ;
                status = PROC_Load (i,
                                    argv [5],
                                    noOfArguments,
                                    arrayOfArguments) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("PROC_Load failed. Status = [0x%x]", status) ;
                }
            }
            if (DSP_SUCCEEDED (status)) {
                status = PROC_Start (i) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("PROC_Start failed. Status = [0x%x]", status) ;
                }
            }
            for (j = 0 ; (j < numThreads) && (DSP_SUCCEEDED (status)) ; j++) {
                if ((j % 2) == 0) {
                    channelMode = ChannelMode_Output ;
                }
                else {
                    channelMode = ChannelMode_Input ;
                }
                runContext[i][j].procId                = i                   ;
                runContext[i][j].xferInfo.channelId    = j                   ;
                runContext[i][j].xferInfo.channelMode  = channelMode         ;
                runContext[i][j].xferInfo.numChirps    = numChirps           ;
                runContext[i][j].xferInfo.bufferLength = bufferLength        ;
                runContext[i][j].xferInfo.delay        = delay               ;
                runContext[i][j].xferInfo.xferCycles   = xferCycles          ;
                runContext[i][j].returnStatus          = &returnStatus[i][j] ;

                /* Create a thread which transfers data between GPP and DSP
                 * using the specified context information
                 */
                pthread_create (&threadIdentifier[i][j],
                                NULL,
                                (void*) TaskDoTransfer,
                                (void*) &runContext[i][j]) ;
            }
        }

        for (j = 0 ; (j < numThreads) && DSP_SUCCEEDED (status) ; j++) {
            pthread_join (threadIdentifier [0][j], NULL) ;
            if (returnStatus[0][j] != expectedStatus) {
                status = DSP_EFAIL ;
                TST_PrnError ("Thread execution failed. Status = [0x%x]",
                              status) ;
                TST_PrnInfo ("Status: Expected 0x%x, Actual 0x%x",
                             expectedStatus, returnStatus [0][j]) ;
            }
        }

        /*
         * Delete Phase
         */
        for (i = 0 ; (i < MAX_PROCESSORS) ; i++) {
            TST_Print ("\n") ;
            tempStatus = PROC_Stop (i) ;
            if (DSP_SUCCEEDED (status) && DSP_FAILED (tempStatus)) {
                status = tempStatus ;
                TST_PrnError ("PROC_Stop failed. Status = [0x%x]", status) ;
            }
            tempStatus = PROC_Detach (i) ;
            if (DSP_SUCCEEDED (status) && DSP_FAILED (tempStatus)) {
                status = tempStatus ;
                TST_PrnError ("PROC_Detach failed. Status = [0x%x]", status) ;
            }
        }
    }

    tempStatus = PROC_Destroy () ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tempStatus)) {
        status = tempStatus ;
        TST_PrnError ("PROC_Destroy failed. Status = [0x%x]", status) ;
    }
    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   STRESS_TEST_DoDataTransferTest
 *
 *  @desc   Exchange the data between the processors (MPU/DSP).
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
STRESS_TEST_DoDataTransferTest (IN ProcessorId     procId,
                                IN ChannelId       channelId,
                                IN ChannelMode     channelMode,
                                IN Uint32          numChirps,
                                IN Uint32          bufferLength,
                                IN Uint32          delay,
                                IN Uint32          xferCycles)
{
    DSP_STATUS       status                     = DSP_SOK ;
    Bool             doDeallocation             = FALSE   ;
    ChannelAttrs     attrs                                ;
    Uint32           i                                    ;
    Uint32           j                                    ;
    ChannelIOInfo    ioInfo                               ;
    Char8 *          bufferArray [STS_MAX_ALLOC_BUFFERS]  ;

    attrs.mode      = channelMode            ;
    attrs.size      = ChannelDataSize_16bits ;
    attrs.endianism = Endianism_Default      ;

    status = CHNL_Create (procId, channelId, &attrs) ;
    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (procId, channelId, bufferArray,
                                      bufferLength, numChirps ) ;
        if (DSP_SUCCEEDED (status)) {
            doDeallocation = TRUE ;
        }
    }

    if (DSP_SUCCEEDED (status)) {

        /* Fill the buffers with sample data in case of output channel */
        if (channelMode == ChannelMode_Output) {
            status = FillSampleData (bufferArray, bufferLength, numChirps) ;
        }

        /* Transfer the buffers to DSP for xferCycles times */
        for (i = 0 ; ((i < xferCycles) || (xferCycles == 0))
                     && DSP_SUCCEEDED (status) ; i++) {
            /* Issue all the buffers */
            for (j = 0 ; (j < numChirps) && DSP_SUCCEEDED (status) ;
                 j++) {
                TST_Sleep (delay) ;
                ioInfo.buffer = bufferArray [j] ;
                ioInfo.size = bufferLength ;
                status = CHNL_Issue (procId, channelId, &ioInfo) ;
            }

            /* Reclaim all the buffers */
            for (j = 0 ; (j < numChirps) && DSP_SUCCEEDED (status) ;
                 j++) {
                TST_Sleep (delay) ;
                ioInfo.buffer = NULL ;
                status = CHNL_Reclaim (procId,
                                       channelId,
                                       WAIT_FOREVER,
                                       &ioInfo) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("CHNL_Reclaim failed. Status = [0x%x]",
                                  status) ;
                }
            }

            /* Verify the data received in case of input channel */
            if ((channelMode == ChannelMode_Input) && DSP_SUCCEEDED (status)) {
                status = VerifyDataIntegrity (bufferArray,
                                              bufferLength, numChirps) ;
            }

            /* Show progress every 10 cycle */
            if ((i % 10) == 0) {
                TST_Print (".") ;
            }
        }
    }

    if (doDeallocation) {
        CHNL_FreeBuffer (procId, channelId, bufferArray, numChirps ) ;
    }

    CHNL_Delete (procId, channelId) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   TaskDoTransfer
 *
 *  @desc   Stress test task entry
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
EXPORT_API
Void
TaskDoTransfer (Void * param)
{
    DSP_STATUS     status       = DSP_SOK            ;
    TaskInfo *     runContext   = (TaskInfo *)param  ;
    ProcessorId    procId                            ;
    TransferInfo   xferInfo                          ;

    procId   = runContext->procId   ;
    xferInfo = runContext->xferInfo ;

    if (DSP_SUCCEEDED (status)) {
        status = STRESS_TEST_DoDataTransferTest (procId,
                                                 xferInfo.channelId,
                                                 xferInfo.channelMode,
                                                 xferInfo.numChirps,
                                                 xferInfo.bufferLength,
                                                 xferInfo.delay,
                                                 xferInfo.xferCycles) ;
    *(runContext->returnStatus) = status ;
    }
}


/** ----------------------------------------------------------------------------
 *  @func   FillSampleData
 *
 *  @desc   Fills up the data in the passed buffers.
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
FillSampleData (Char8 * bufferArray [],
                Uint32  bufferLength,
                Uint32  numBuffers)
{
    /* Perform operation based on DSP side sources. */
    return DSP_SOK ;
}


/** ----------------------------------------------------------------------------
 *  @func   VerifyDataIntegrity
 *
 *  @desc   Verifies the data integrity of received data
 *
 *  @modif    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
VerifyDataIntegrity (Char8 * bufferArray [],
                     Uint32  bufferLength,
                     Uint32  numBuffers)
{
    /* Perform operation based on DSP side sources. */
    return DSP_SOK ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
