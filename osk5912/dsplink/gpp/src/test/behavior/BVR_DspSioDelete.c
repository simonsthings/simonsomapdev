/** ============================================================================
 *  @file   BVR_DspSioDelete.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implementation of behavior test for BVR_DspSioDelete.
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
#include <TST_Sleep.h>
#include <TST_PrintFuncs.h>
#include <BVR_DspSioDelete.h>
#include <BVR_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MESG_BUF_SIZE
 *
 *  @desc   Size of the message buffer to be transferred to DSP.
 *  ============================================================================
 */
#define MESG_BUF_SIZE     16

/** ============================================================================
 *  @name   INPUT_MODE
 *
 *  @desc   Input mode for the channel on the DSP side.
 *  ============================================================================
 */
#define INPUT_MODE     0

/** ============================================================================
 *  @name   OUTPUT_MODE
 *
 *  @desc   Output mode for the channel on the DSP side.
 *  ============================================================================
 */
#define OUTPUT_MODE     1

/** ============================================================================
 *  @name   INPUT_CONTROL_CHNL_ID
 *
 *  @desc   Channel id for input control channel.
 *  ============================================================================
 */
#define INPUT_CONTROL_CHNL_ID      13

/** ============================================================================
 *  @name   OUTPUT_CONTROL_CHNL_ID
 *
 *  @desc   Channel id for output control channel.
 *  ============================================================================
 */
#define OUTPUT_CONTROL_CHNL_ID     14

/** ============================================================================
 *  @name   MAXBUFS
 *
 *  @desc   Maximum number of buffers that can be allocated.
 *  ============================================================================
 */
#define MAXBUFS            16

/** ============================================================================
 *  @name   TEST_CHANNEL_TIMEOUT
 *
 *  @desc   Timeout for control channel reclaim.
 *  ============================================================================
 */
#define TEST_CHANNEL_TIMEOUT   WAIT_FOREVER

/** ============================================================================
 *  @name   MAX_ARGUMENTS
 *
 *  @desc   The maximum no of arguments which are being passed to DSP.
 *  ============================================================================
 */
#define MAX_ARGUMENTS      10


/** ============================================================================
 *  @func   BVR_DspSioDelete
 *
 *  @desc   Function to test the behavior DSP SIO_delete when flush flag is TRUE
 *          and FALSE. In this function the GPP side first sends a message on
 *          the control channel to synchronize with DSP. It then, depending upon
 *          the channel mode and flush flag issues and reclaims the buffer(s)
 *          to the DSP side. The DSP image executes and then it sends the status
 *          of its execution on input control channel. If the DSP side
 *          successfully passes then the test passes.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_DspSioDelete (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status                           = DSP_SOK ;
    DSP_STATUS    statusOfSetup                    = DSP_SOK ;
    DSP_STATUS    tempStatus                       = DSP_SOK ;
    ProcessorId   dspId                            = 0       ;
    ChannelId     dataChnlId                       = 0       ;
    Uint32        noOfArguments                    = 0       ;
    Uint32        numToBeQueued                    = 0       ;
    Uint32        bufSize                          = 0       ;
    Uint32        isFlush                          = 0       ;
    Uint32        reclaimTimeout                   = 0       ;
    Uint32        modeOfDataChnl                   = 0       ;
    Uint32        modeOfDspDataChnl                = 0       ;
    Uint32        i                                = 0       ;
    Pstr          fileName                         = NULL    ;
    Uint16  *     ptr                              = NULL    ;
    Uint32        sleepDelay                       = 0       ;
    Char8   *     arrayOfArguments [MAX_ARGUMENTS]           ;
    Char8   *     inputControlBuffers [1]                    ;
    Char8   *     outputControlBuffers [1]                   ;
    Char8   *     dataBuffers [MAXBUFS]                      ;
    ChannelAttrs  dataChnlAttrs                              ;
    ChannelIOInfo dataIoReq                                  ;
    ChannelAttrs  inputControlChnlAttrs                      ;
    ChannelAttrs  outputControlChnlAttrs                     ;
    ChannelIOInfo inputControlIoReq                          ;
    ChannelIOInfo outputControlIoReq                         ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 10)   ;
    DBC_Require (argv != NULL) ;

    if ((argc != 10) || (argv == NULL)) {
        statusOfSetup = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_DspSioDelete. Status = [0x%x]",
                      statusOfSetup) ;
    }
    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId              = TST_StringToInt (argv [2]) ;
        fileName           = argv [3]                   ;
        numToBeQueued      = TST_StringToInt (argv [4]) ;
        dataChnlId         = TST_StringToInt (argv [5]) ;
        modeOfDataChnl     = TST_StringToInt (argv [6]) ;
        bufSize            = TST_StringToInt (argv [7]) ;
        isFlush            = TST_StringToInt (argv [8]) ;
        reclaimTimeout     = TST_StringToInt (argv [9]) ;
        modeOfDspDataChnl  = (modeOfDataChnl == 0) ? INPUT_MODE : OUTPUT_MODE ;

        statusOfSetup      = PROC_Setup ()              ;
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
        noOfArguments = 5 ;
        arrayOfArguments [0] = argv [4] ;
        arrayOfArguments [1] = argv [5] ;
        arrayOfArguments [2] = argv [6] ;
        arrayOfArguments [3] = argv [7] ;
        arrayOfArguments [4] = argv [8] ;
        statusOfSetup = PROC_Load (dspId,
                                   fileName,
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
    if (DSP_SUCCEEDED (statusOfSetup)) {
        if (modeOfDataChnl == 0) {
            dataChnlAttrs.mode    = ChannelMode_Output ;
        }
        else if (modeOfDataChnl == 1) {
            dataChnlAttrs.mode    = ChannelMode_Input ;
        }
        else {
            statusOfSetup = DSP_EFAIL ;
            TST_PrnError ("Incorrect mode for channel passed. Status = [0x%x]",
                          statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dataChnlAttrs.endianism = Endianism_Default ;
        dataChnlAttrs.size      = ChannelDataSize_16bits ;

        statusOfSetup = CHNL_Create (dspId, dataChnlId, &dataChnlAttrs) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to create data channel. Status = [0x%x]",
                          statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        inputControlChnlAttrs.mode      = ChannelMode_Input ;
        inputControlChnlAttrs.endianism = Endianism_Default ;
        inputControlChnlAttrs.size      = ChannelDataSize_16bits ;

        statusOfSetup = CHNL_Create (dspId,
                                     INPUT_CONTROL_CHNL_ID,
                                     &inputControlChnlAttrs) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to create input control channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        outputControlChnlAttrs.mode      = ChannelMode_Output ;
        outputControlChnlAttrs.endianism = Endianism_Default ;
        outputControlChnlAttrs.size      = ChannelDataSize_16bits ;

        statusOfSetup = CHNL_Create (dspId,
                                     OUTPUT_CONTROL_CHNL_ID,
                                     &outputControlChnlAttrs) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to create output control channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = CHNL_AllocateBuffer (dspId,
                                             dataChnlId,
                                             dataBuffers,
                                             bufSize,
                                             numToBeQueued) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on data channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = CHNL_AllocateBuffer (dspId,
                                             INPUT_CONTROL_CHNL_ID,
                                             inputControlBuffers,
                                             MESG_BUF_SIZE,
                                             1) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on input control "
                          "channel. Status = [0x%x]", statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = CHNL_AllocateBuffer (dspId,
                                             OUTPUT_CONTROL_CHNL_ID,
                                             outputControlBuffers,
                                             MESG_BUF_SIZE,
                                             1) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on output control "
                          "channel. Status = [0x%x]", statusOfSetup) ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Test behavior of DSP executable.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        /* Send a buffer on the control channel to unblock the DSP */
        if (DSP_SUCCEEDED (status)) {
            outputControlIoReq.buffer = (Char8 *) outputControlBuffers [0] ;
            outputControlIoReq.size   = MESG_BUF_SIZE ;
            outputControlIoReq.arg    = 0 ;
            status = CHNL_Issue (dspId,
                                 OUTPUT_CONTROL_CHNL_ID,
                                 &outputControlIoReq) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Issue Failed. Status = [0x%x]",
                              status) ;
            }
        }
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (dspId,
                                   OUTPUT_CONTROL_CHNL_ID,
                                   TEST_CHANNEL_TIMEOUT,
                                   &outputControlIoReq) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Reclaim Failed. Status = [0x%x]",
                              status) ;
            }
        }

        /* Behavior of SIO_delete () is:
         * For input mode: Always discards buffers
         * For output mode: If flush == TRUE, discards buffers
         *                  If flush == FALSE, wait for completion.
         */
        if ( (DSP_SUCCEEDED (status))
            &&  (   (modeOfDspDataChnl  == INPUT_MODE)
                 || (    (modeOfDspDataChnl == OUTPUT_MODE)
                      && (isFlush == TRUE)))) {
            TST_Sleep (sleepDelay) ;
            dataIoReq.buffer = (Char8 *) dataBuffers [0] ;
            dataIoReq.size   = bufSize ;
            dataIoReq.arg    = 0 ;
            status = CHNL_Issue (dspId, dataChnlId, &dataIoReq) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Issue Failed. Status = [0x%x]",
                              status) ;
            }

            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (dspId,
                                       dataChnlId,
                                       reclaimTimeout,
                                       &dataIoReq) ;
                /* This is expected behavior */
                if (status == DSP_ETIMEOUT) {
                    status = DSP_SOK ;
                }
                else {
                    status = DSP_EFAIL ;
                    TST_PrnError ("Call to CHNL_Reclaim should have failed. "
                                  "Status = [0x%x]", status) ;
                }
            }
        }
        else if (DSP_SUCCEEDED (status)) {
            /* Sleeps to allow DSP side to call SIO_delete */
            TST_Sleep (sleepDelay) ;

            /* Queue the buffers in the requested queue */
            for (i = 0 ;   (i < numToBeQueued)
                        && (DSP_SUCCEEDED (status)) ; i++) {
                dataIoReq.buffer = (Char8 *) dataBuffers [i] ;
                dataIoReq.size   = bufSize ;
                dataIoReq.arg    = 0 ;
                status = CHNL_Issue (dspId, dataChnlId, &dataIoReq) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("Call to CHNL_Issue Failed. Status = [0x%x]",
                                  status) ;
                }
            }
            for (i = 0 ;   (i < numToBeQueued)
                        && (DSP_SUCCEEDED (status)) ; i++) {
                status = CHNL_Reclaim (dspId,
                                       dataChnlId,
                                       TEST_CHANNEL_TIMEOUT,
                                       &dataIoReq) ;
                if (DSP_FAILED (status)) {
                    TST_PrnError ("Call to CHNL_Reclaim Failed. Status = [0x%x]",
                                  status) ;
                }
                if (dataIoReq.size != bufSize) {
                    status = DSP_EFAIL ;
                    TST_PrnError ("The size of buffers received is "
                                  "inconsistent. Status = [0x%x]", status) ;
                }
            }
        }

        /* Receive a buffer from DSP containing the status of DSP execution */
        if (DSP_SUCCEEDED (status)) {
            inputControlIoReq.buffer = (Char8 *) inputControlBuffers [0] ;
            inputControlIoReq.size   = MESG_BUF_SIZE ;
            inputControlIoReq.arg    = 0 ;
            status = CHNL_Issue (dspId,
                                 INPUT_CONTROL_CHNL_ID,
                                 &inputControlIoReq) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Issue Failed. Status = [0x%x]",
                              status) ;
            }
        }
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (dspId,
                                   INPUT_CONTROL_CHNL_ID,
                                   TEST_CHANNEL_TIMEOUT,
                                   &inputControlIoReq) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Reclaim Failed. Status = [0x%x]",
                              status) ;
            }
        }
        if (DSP_SUCCEEDED (status)) {
            ptr = (Uint16 *) (inputControlBuffers [0]) ;
            if (*ptr != TRUE) {
                if (DSP_FAILED (status)) {
                    TST_PrnError ("DSP execution failed. Status = [0x%x]",
                                  status) ;
                }
                status = DSP_EFAIL ;
            }
        }
    }

    tempStatus = CHNL_FreeBuffer (dspId,
                                  dataChnlId,
                                  (Char8 **) dataBuffers,
                                  numToBeQueued) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't free data buffers. Status = [0x%x]", status) ;
    }
    tempStatus = CHNL_FreeBuffer (dspId,
                                  INPUT_CONTROL_CHNL_ID,
                                  (Char8 **) inputControlBuffers,
                                  1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't free input control channel buffers. "
                      "Status = [0x%x]", status) ;
    }
    tempStatus = CHNL_FreeBuffer (dspId,
                                  OUTPUT_CONTROL_CHNL_ID,
                                  (Char8 **) outputControlBuffers,
                                  1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't free output control channel buffers. "
                      "Status = [0x%x]", status) ;
    }
    tempStatus = CHNL_Delete (dspId, dataChnlId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete data channel. Status = [0x%x]", status) ;
    }
    tempStatus = CHNL_Delete (dspId, INPUT_CONTROL_CHNL_ID) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete input control channel. "
                      "Status = [0x%x]", status) ;
    }
    tempStatus = CHNL_Delete (dspId, OUTPUT_CONTROL_CHNL_ID) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete output control channel. "
                      "Status = [0x%x]", status) ;
    }
    tempStatus = PROC_Stop (dspId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Call to PROC_Stop Failed. Status = [0x%x]", status) ;
    }
    tempStatus = PROC_Detach (dspId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Call to PROC_Detach Failed. Status = [0x%x]", status) ;
    }
    tempStatus = PROC_Destroy () ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Call to PROC_Destroy Failed. Status = [0x%x]", status) ;
    }

    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
