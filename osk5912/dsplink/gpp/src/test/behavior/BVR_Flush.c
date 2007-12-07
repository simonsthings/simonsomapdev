/** ============================================================================
 *  @file   BVR_Flush.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implementation of behavior test for behavior of CHNL_Flush.
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
#include <TST_PrintFuncs.h>
#include <BVR_Flush.h>
#include <BVR_PostProcess.h>
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


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
 *  @name   TEST_CHANNEL_TIMEOUT
 *
 *  @desc   Timeout for reclaim timeout.
 *  ============================================================================
 */
#define TEST_CHANNEL_TIMEOUT   WAIT_FOREVER

/** ============================================================================
 *  @name   MAXBUFS
 *
 *  @desc   Maximum number of buffers that can be allocated.
 *  ============================================================================
 */
#define MAXBUFS            16

/** ============================================================================
 *  @name   MAX_ARGUMENTS
 *
 *  @desc   The maximum no of arguments which are being passed to DSP.
 *  ============================================================================
 */
#define MAX_ARGUMENTS            5


/** ============================================================================
 *  @func   BVR_Flush
 *
 *  @desc   Function to test the behavior of CHNL_Flush ()
 *
 *          This test issues one (or multiple) buffer(s) on a data channel
 *          and calls CHNL_Flush to flush all the buffers.
 *          Then it sends a message on output control channel to dsp to indicate
 *          that it has called CHNL_Flush. The dsp side now calls an issue and
 *          then reclaim with a finite timeout which should fail (with timeout).
 *          The dsp side then sends a message on input control channel with the
 *          number of bytes it had received. The test passes if the number of
 *          bytes received was 0. The flow diagram of the test is as follows.
 *
 *                    GPP                                       DSP
 *                                         | |
 *         CHNL_Issue (Data Channel)       +-+
 *                                         | |
 *         CHNL_Flush (Data Channel)       + |  SIO_issue (Control Channel)
 *                                         | |
 *         CHNL_Reclaim (Data Channel)     + |
 *         (With 0 timeout value)          | |
 *                                         | |
 *         CHNL_Issue (Control Channel)    + |
 *                                         | |
 *         CHNL_Reclaim (Control Channel)  + |
 *                                         | |
 *                                         | |  SIO_reclaim (Control Channel)
 *                                         | |
 *                                         |-+  Unblocks here.
 *                                         | |
 *                                         +-|
 *                                         | + SIO_issue   (Data Channel)
 *                                         | |
 *         CHNL_Issue (Control Channel)    | + SIO_reclaim (Data Channel)
 *                                         | | (Fails with 'TIMEOUT'.)
 *                                         | |
 *                                         +-+ SIO_issue   (Control Channel)
 *                                         | |
 *         CHNL_Reclaim(Control Channel)   | + SIO_reclaim (Control Channel)
 *                                         | | (Sends how many bytes received
 *         Unblocks here.                  +-| on data channel.)
 *                                         | |
 *                                         | |
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_Flush (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status                 = DSP_SOK ;
    DSP_STATUS    statusOfSetup          = DSP_SOK ;
    DSP_STATUS    tempStatus             = DSP_SOK ;
    ChannelId     dataChnlId             = 0       ;
    ProcessorId   dspId                  = 0       ;
    Uint32        numToBeQueued          = 0       ;
    Uint32        noOfArguments          = 0       ;
    Uint32        bufSize                = 0       ;
    Uint32        modeOfDataChnl         = 0       ;
    Pstr          fileName               = NULL    ;
    Uint16  *     ptr                    = NULL    ;
    Char8   *     arrayOfArguments [MAX_ARGUMENTS] ;
    Char8   *     dataBuffers [MAXBUFS]            ;
    Char8   *     inputControlBuffers [MAXBUFS]    ;
    Char8   *     outputControlBuffers [MAXBUFS]   ;
    ChannelAttrs  dataChnlAttrs                    ;
    ChannelAttrs  inputControlChnlAttrs            ;
    ChannelAttrs  outputControlChnlAttrs           ;
    ChannelIOInfo dataIoReq                        ;
    ChannelIOInfo inputControlIoReq                ;
    ChannelIOInfo outputControlIoReq               ;
    Uint32        i                                ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 8)    ;
    DBC_Require (argv != NULL) ;

    if ((argc != 8) && (argv == NULL)) {
        statusOfSetup = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_Flush. Status = [0x%x]",
                      statusOfSetup) ;
    }
    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId          = TST_StringToInt (argv [2]) ;
        fileName       = argv [3]                   ;
        dataChnlId     = TST_StringToInt (argv [4]) ;
        modeOfDataChnl = TST_StringToInt (argv [5]) ;
        numToBeQueued  = TST_StringToInt (argv [6]) ;
        bufSize        = TST_StringToInt (argv [7]) ;
        statusOfSetup  = PROC_Setup ()              ;
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
        noOfArguments = 3 ;
        arrayOfArguments [0] = argv [4] ;
        arrayOfArguments [1] = argv [5] ;
        arrayOfArguments [2] = argv [7] ;
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
        if (numToBeQueued != 0) {
            statusOfSetup = CHNL_AllocateBuffer (dspId,
                                                 dataChnlId,
                                                 dataBuffers,
                                                 bufSize,
                                                 numToBeQueued) ;
        }
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on data channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = CHNL_AllocateBuffer (dspId,
                                             INPUT_CONTROL_CHNL_ID,
                                             inputControlBuffers,
                                             bufSize,
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
                                             bufSize,
                                             1) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on output control "
                          "channel. Status = [0x%x]", statusOfSetup) ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Test CHNL_Flush.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {

        /* Queue the buffers in the requested queue */
        for (i = 0 ; (i < numToBeQueued) && (DSP_SUCCEEDED (status)) ; i++) {
            dataIoReq.buffer = (Char8 *) dataBuffers [i] ;
            dataIoReq.size   = bufSize ;
            dataIoReq.arg    = 0 ;
            status = CHNL_Issue (dspId, dataChnlId, &dataIoReq) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Issue Failed. Status = [0x%x]",
                              status) ;
            }
        }

        /* Flush all the Queued buffers in the requested queue */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Flush (dspId, dataChnlId) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Flush Failed. Status = [0x%x]",
                              status) ;
            }
        }

        if ((DSP_SUCCEEDED (status)) && (numToBeQueued > 0)) {
            status = CHNL_Reclaim (dspId,
                                   dataChnlId,
                                   0,
                                   &dataIoReq) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Call to CHNL_Reclaim Failed. Status = [0x%x]",
                              status) ;
            }
        }
        /* Send a buffer on the control channel to unblock the DSP */
        if (DSP_SUCCEEDED (status)) {
            outputControlIoReq.buffer = (Char8 *) outputControlBuffers [0] ;
            outputControlIoReq.size   = bufSize ;
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

        /* Receive a buffer from DSP containing the number of bytes
         * DSP has received.
         */
        if (DSP_SUCCEEDED (status)) {
            inputControlIoReq.buffer = (Char8 *) inputControlBuffers [0] ;
            inputControlIoReq.size   = bufSize ;
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
            if (*ptr != 0) {
                if (DSP_FAILED (status)) {
                    TST_PrnError ("DSP received buffers inspite of "
                                  "CHNL_Flush. Status = [0x%x]", status) ;
                }
                status = DSP_EFAIL ;
            }
        }
    }

    if (numToBeQueued != 0) {
        tempStatus = CHNL_FreeBuffer (dspId,
                                      dataChnlId,
                                      (Char8 **) dataBuffers,
                                      numToBeQueued) ;
    }

    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't free data buffers. Status = [0x%x]",
                      status) ;
    }

    tempStatus = CHNL_FreeBuffer (dspId,
                                  INPUT_CONTROL_CHNL_ID,
                                  (Char8 **) inputControlBuffers,
                                  1) ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Couldn't free input control channel buffers. "
                      "Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_FreeBuffer (dspId,
                                  OUTPUT_CONTROL_CHNL_ID,
                                  (Char8 **) outputControlBuffers,
                                  1) ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Couldn't free output control channel buffers. "
                      "Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_Delete (dspId, dataChnlId) ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Couldn't delete data channel. Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_Delete (dspId, INPUT_CONTROL_CHNL_ID) ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Couldn't delete input control channel. Status = [0x%x]",
                      status) ;
    }

    tempStatus = CHNL_Delete (dspId, OUTPUT_CONTROL_CHNL_ID) ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Couldn't delete output control channel. "
                      "Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Stop (dspId) ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Call to PROC_Stop Failed. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Detach (dspId) ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Call to PROC_Detach Failed. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Destroy () ;
    if (   (DSP_SUCCEEDED (status))
        && (DSP_SUCCEEDED (statusOfSetup))
        && (DSP_FAILED (tempStatus))) {

        status = tempStatus ;
        TST_PrnError ("Call to PROC_Destroy Failed. Status = [0x%x]", status) ;
    }

    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
