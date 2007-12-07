/** ============================================================================
 *  @file   STS_SingleThread.c
 *
 *  @path   $(DSPLINK)\gpp\src\stress\Linux
 *
 *  @desc   Implementation of single threaded stress test.
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
#include <STS_SingleThread.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   TASK_ID
 *
 *  @desc   The identifier of the thread to be created
 *  ============================================================================
 */
#define TASK_ID 1

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
 *  ============================================================================
 */
typedef struct TransferInfo_tag {
    ChannelId   channelId     ;
    ChannelMode channelMode   ;
    Uint32      numChirps     ;
    Uint32      bufferLength  ;
    Uint32      delay         ;
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
    ProcessorId  procId                  ;
    TransferInfo xferInfo [MAX_CHANNELS] ;
    DSP_STATUS   returnStatus            ;
} TaskInfo ;


/** ============================================================================
 *  @name   runContext
 *
 *  @desc   TransferInfo variables for various tests
 *  ============================================================================
 */
STATIC
TaskInfo
runContext [MAX_PROCESSORS] ;


/** ============================================================================
 *  @name   returnStatus
 *
 *  @desc   Return status for the data transfer done on each processor.
 *  ============================================================================
 */
STATIC
DSP_STATUS
returnStatus [MAX_PROCESSORS] ;


/** ============================================================================
 *  @name   numChannels
 *
 *  @desc   Number of channels on which data transfer is done.
 *  ============================================================================
 */
STATIC
Uint32
numChannels = 0 ;


/** ============================================================================
 *  @name   xferCycles
 *
 *  @desc   The number of times the transfers will be done on all channels.
 *  ============================================================================
 */
STATIC
Uint32
xferCycles = 0 ;


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
 *              Operation Successfully Completed.
 *          DSP_EFAIL
 *              Failure occured.
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
 *              Operation Successfully Completed.
 *          DSP_EFAIL
 *              occured
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
STRESS_TEST_DoDataTransferTest (IN ProcessorId     procId,
                                IN ChannelId       channelId,
                                IN ChannelMode     channelMode,
                                IN Uint32          numChirps,
                                IN Uint32          bufferLength,
                                IN Uint32          delay) ;


/** ----------------------------------------------------------------------------
 *  @func   STRESS_TEST_TaskEntryDoMultipleChnlTransfer
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
STRESS_TEST_TaskEntryDoMultipleChnlTransfer (Void *   param) ;


/** ============================================================================
 *  @func   STS_SingleThread
 *
 *  @desc   Entry point to single threaded stress test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_SingleThread (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  status             = DSP_SOK ;
    DSP_STATUS  tempStatus         = DSP_SOK ;
    DSP_STATUS  expectedStatus     = DSP_SOK ;
    Uint32      numChirps          = 0       ;
    Uint32      bufferLength       = 100     ;
    Uint32      delay              = 0       ;
    ChannelMode channelMode                  ;
    Uint32      i                            ;
    Uint32      j                            ;
    Uint32      noOfArguments                ;
    Char8  *    arrayOfArguments [MAX_ARGS]  ;
    ProcAttr    attrs                        ;
    pthread_t   threadIdentifier             ;

    DBC_Require (argc == 7) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 7) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Wrong number of arguments. Status = [0x%x]", status) ;
    }
    if (DSP_SUCCEEDED (status)) {
        status        = TST_StrToStatus (argv [0], &expectedStatus) ;
        xferCycles    = TST_StringToInt (argv [1])                  ;
        bufferLength  = TST_StringToInt (argv [2])                  ;
        numChirps     = TST_StringToInt (argv [3])                  ;
        delay         = TST_StringToInt (argv [4])                  ;
        numChannels    = TST_StringToInt (argv [6])                 ;
        if (numChannels > MAX_CHANNELS) {
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
                    TST_PrnError ("PROC_Attach failed. Status = [0x%x]",
                                  status) ;
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
                    TST_PrnError ("PROC_Start failed. Status = [0x%x]",
                                  status) ;
                }
            }
            for (j = 0 ; (j < numChannels) && DSP_SUCCEEDED (status) ; j++) {
                if ((j % 2) == 0) {
                    channelMode = ChannelMode_Output ;
                }
                else {
                    channelMode = ChannelMode_Input ;
                }
                runContext [i].procId                    = i            ;
                runContext [i].xferInfo [j].channelId    = j            ;
                runContext [i].xferInfo [j].channelMode  = channelMode  ;
                runContext [i].xferInfo [j].numChirps    = numChirps    ;
                runContext [i].xferInfo [j].bufferLength = bufferLength ;
                runContext [i].xferInfo [j].delay        = delay        ;
            }
        }

        /* Create a thread which transfers data between GPP and DSP using
         * the specified context information
         */
        threadIdentifier = TASK_ID ;
        pthread_create (&threadIdentifier,
                        NULL,
                        (void*) STRESS_TEST_TaskEntryDoMultipleChnlTransfer,
                        (void*) runContext) ;

        pthread_join (threadIdentifier, NULL) ;

        TST_Print ("\n") ;
        /*
         * Delete Phase
         */
        for (i = 0 ; (i < MAX_PROCESSORS) ; i++) {
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

        for (i = 0 ; (i < MAX_PROCESSORS) && DSP_SUCCEEDED (status) ; i++) {
            if (returnStatus [i] != expectedStatus) {
                status = DSP_EFAIL ;
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
                                IN Uint32          delay)
{
    DSP_STATUS       status                    = DSP_SOK ;
    DSP_STATUS       tempStatus                = DSP_SOK ;
    Bool             doDeallocation            = FALSE   ;
    ChannelAttrs     attrs                               ;
    Uint32           j                                   ;
    ChannelIOInfo    ioInfo                              ;
    Char8 *          bufferArray [STS_MAX_ALLOC_BUFFERS] ;

    attrs.mode      = channelMode            ;
    attrs.size      = ChannelDataSize_16bits ;
    attrs.endianism = Endianism_Default      ;

    /* Progress print */
    TST_Print (".") ;
    status = CHNL_Create (procId, channelId, &attrs) ;
    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (procId, channelId, bufferArray,
                                      bufferLength, numChirps ) ;
        if (DSP_SUCCEEDED (status)) {
            doDeallocation = TRUE ;
        }
    }
    else {
        TST_PrnError ("CHNL_Create failed. Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /* Fill the buffers with sample data in case of output channel */
        if (channelMode == ChannelMode_Output) {
            status = FillSampleData (bufferArray, bufferLength, numChirps) ;
        }

        /* Issue all the buffers */
        for (j = 0 ; (j < numChirps) && DSP_SUCCEEDED (status) ;
             j++) {
            TST_Sleep (delay) ;
            ioInfo.buffer = bufferArray [j] ;
            ioInfo.size = bufferLength ;
            status = CHNL_Issue (procId, channelId, &ioInfo) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("CHNL_Issue failed. Status = [0x%x]", status) ;
            }
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
                TST_PrnError ("CHNL_Reclaim failed. Status = [0x%x]", status) ;
            }
        }

        /* Verify the data received in case of input channel */
        if ((channelMode == ChannelMode_Input) && DSP_SUCCEEDED (status)) {
            status = VerifyDataIntegrity (bufferArray,
                                          bufferLength, numChirps) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Data Integrity check failed. Status = [0x%x]",
                              status) ;
            }
        }
    }

    if (doDeallocation) {
        tempStatus = CHNL_FreeBuffer (procId,
                                      channelId,
                                      bufferArray,
                                      numChirps ) ;
        if (DSP_SUCCEEDED (status) && DSP_FAILED (tempStatus)) {
            status = tempStatus ;
            TST_PrnError ("CHNL_FreeBuffer failed. Status = [0x%x]", status) ;
        }
    }

    tempStatus = CHNL_Delete (procId, channelId) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tempStatus)) {
        status = tempStatus ;
        TST_PrnError ("CHNL_Delete failed. Status = [0x%x]", status) ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   STRESS_TEST_TaskEntryDoMultipleChnlTransfer
 *
 *  @desc   Stress test task entry
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
EXPORT_API
Void
STRESS_TEST_TaskEntryDoMultipleChnlTransfer (Void * param)
{
    DSP_STATUS     status       = DSP_SOK            ;
    TransferInfo * xferInfo     = NULL               ;
    TaskInfo *     runContext   = (TaskInfo *)param  ;
    Uint32         i                                 ;
    Uint32         j                                 ;
    Uint32         k                                 ;

    for (i = 0 ; i < MAX_PROCESSORS ; i++) {
        xferInfo = runContext [i].xferInfo ;
        /* Transfer the buffers to DSP for xferCycles times */
        for (k = 0 ;    ((k < xferCycles)|| (xferCycles == 0))
                    &&  (DSP_SUCCEEDED (status)) ; k++) {
            for (j = 0 ; (j < numChannels) && DSP_SUCCEEDED (status) ; j++) {

                status = STRESS_TEST_DoDataTransferTest (
                                                   i,
                                                   xferInfo [j].channelId,
                                                   xferInfo [j].channelMode,
                                                   xferInfo [j].numChirps,
                                                   xferInfo [j].bufferLength,
                                                   xferInfo [j].delay) ;
                returnStatus [i] = status ;
            }
        }
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
