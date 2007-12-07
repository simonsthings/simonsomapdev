/** ============================================================================
 *  @file   BVR_ReclaimTimeout.c
 *
 *  @path   $ (DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implementation of behavior test for BVR_ReclaimTimeout.
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
#include <BVR_ReclaimTimeout.h>
#include <BVR_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


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
 *  @desc   Timeout for control channel reclaim.
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
#define MAX_ARGUMENTS      5


/** ============================================================================
 *  @func   BVR_ReclaimTimeout
 *
 *  @desc   Function to test the behavior of CHNL_Reclaim () with finite
 *          timeout.
 *
 *          This test tests the behavior of CHNL_Reclaim with finite timeout.
 *          The GPP side sends a control message and dsp side waits for
 *          that message. The GPP side then issues a buffer on the data channel.
 *          DSP side sleeps for a timeout value specified by GPP. GPP side
 *          reclaims the buffer twice, with the finite timeout values specified
 *          by the user. DSP side after waking up from sleep issues and reclaims
 *          the buffer on data channel.
 *
 *                     GPP                                       DSP
 *                                          | |
 *          CHNL_Issue (Control Channel)    +-+
 *                                          | |
 *                                          | + SIO_issue (Control Channel)
 *                                          | |
 *          CHNL_Reclaim (Control Channel)  + |
 *                                          | + SIO_reclaim (Control Channel)
 *                                          | |
 *                                          | + Sleep for 'Timeout' specified
 *                                          | | by GPP side.
 *          CHNL_Issue (Data Channel)       + |
 *                                          | |
 *          CHNL_Reclaim (Data Channel)     + |
 *          (Finite Timeout Specified       | |
 *           by the User)                   | |
 *                                          | |
 *                                          |-+ Wakes up from sleep here.
 *                                          | |
 *                                          | + SIO_issue   (Data Channel)
 *                                          | |
 *                                          | |
 *                                          | + SIO_reclaim (Data Channel)
 *                                          | |
 *          CHNL_Reclaim (Data Channel)     + |
 *          (Finite Timeout Specified       | |
 *           by the User)                   | |
 *                                          | |
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_ReclaimTimeout (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status                           = DSP_SOK ;
    DSP_STATUS    statusOfSetup                    = DSP_SOK ;
    DSP_STATUS    statusOfFirstReclaim             = DSP_SOK ;
    DSP_STATUS    statusOfSecondReclaim            = DSP_SOK ;
    DSP_STATUS    expStatusOfFirstReclaim          = DSP_SOK ;
    DSP_STATUS    expStatusOfSecondReclaim         = DSP_SOK ;
    DSP_STATUS    tempStatus                       = DSP_SOK ;
    Pstr          fileName                         = NULL    ;
    ChannelId     dataChnlId                       = 0       ;
    ProcessorId   dspId                            = 0       ;
    Uint32        noOfArguments                    = 0       ;
    Uint32        timeOut1ForReclaim               = 0       ;
    Uint32        timeOut2ForReclaim               = 0       ;
    Uint32        bufSize                          = 0       ;
    Char8   *     arrayOfArguments [MAX_ARGUMENTS]           ;
    Char8   *     dataBuffers [MAXBUFS]                      ;
    Char8   *     outputControlBuffers [MAXBUFS]             ;
    ChannelAttrs  dataChnlAttrs                              ;
    ChannelAttrs  outputControlChnlAttrs                     ;
    ChannelIOInfo dataIoReq                                  ;
    ChannelIOInfo outputControlIoReq                         ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 11) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 11) || (argv == NULL)) {
        statusOfSetup = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_ReclaimTimeout. Status = [0x%x]",
                      statusOfSetup) ;
    }
    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId              = TST_StringToInt (argv [4])  ;
        fileName           = argv [5]                    ;
        dataChnlId         = TST_StringToInt (argv [6])  ;
        timeOut1ForReclaim = TST_StringToInt (argv [7])  ;
        timeOut2ForReclaim = TST_StringToInt (argv [8])  ;
        bufSize            = TST_StringToInt (argv [10]) ;
        statusOfSetup      = PROC_Setup ()               ;
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
        arrayOfArguments [0] = argv [6] ;
        arrayOfArguments [1] = argv [9] ;
        arrayOfArguments [2] = argv [10] ;
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
        dataChnlAttrs.mode    = ((dataChnlId % 2) == 0)
                                  ? ChannelMode_Output
                                  : ChannelMode_Input ;

        dataChnlAttrs.endianism = Endianism_Default ;
        dataChnlAttrs.size      = ChannelDataSize_16bits ;

        statusOfSetup = CHNL_Create (dspId, dataChnlId, &dataChnlAttrs) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to create data channel. Status = [0x%x]",
                          statusOfSetup) ;
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
                                             1) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on data channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
    }
    if (DSP_SUCCEEDED (statusOfSetup)) {
        statusOfSetup = CHNL_AllocateBuffer (dspId,
                                             OUTPUT_CONTROL_CHNL_ID,
                                             outputControlBuffers,
                                             bufSize,
                                             1) ;
        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on output control"
                          " channel. Status = [0x%x]", statusOfSetup) ;
        }
    }
    /*  ------------------------------------------------------------------------
     *  Test CHNL_Reclaim with finite timeout.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        outputControlIoReq.buffer = (Char8 *) outputControlBuffers [0] ;
        outputControlIoReq.size   = bufSize ;
        outputControlIoReq.arg    = 0 ;
        status = CHNL_Issue (dspId,
                             OUTPUT_CONTROL_CHNL_ID,
                             &outputControlIoReq) ;

        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (dspId,
                                   OUTPUT_CONTROL_CHNL_ID,
                                   TEST_CHANNEL_TIMEOUT,
                                   &outputControlIoReq) ;
        }

        if (DSP_SUCCEEDED (status)) {
            dataIoReq.buffer = (Char8 *) dataBuffers [0] ;
            dataIoReq.size   = bufSize ;
            dataIoReq.arg    = 0 ;
            status = CHNL_Issue (dspId, dataChnlId, &dataIoReq) ;
        }

        if (DSP_SUCCEEDED (status)) {
            statusOfFirstReclaim = CHNL_Reclaim (dspId,
                                   dataChnlId,
                                   timeOut1ForReclaim,
                                   &dataIoReq) ;
            tempStatus = TST_StrToStatus (argv [2],
                                          &expStatusOfFirstReclaim) ;

            if (DSP_SUCCEEDED (tempStatus)) {
                if (statusOfFirstReclaim != expStatusOfFirstReclaim) {
                    status = DSP_EFAIL ;
                    TST_PrnError ("First Reclaim ended with unexpected "
                                  "status. Status = [0x%x]",
                                   statusOfFirstReclaim) ;
                }
            }
            else {
                status = DSP_EFAIL ;
                TST_PrnError ("TST_StrToStatus Failed. Status = [0x%x]",
                              status) ;
            }
        }

        if (DSP_SUCCEEDED (status)) {
            statusOfSecondReclaim = CHNL_Reclaim (dspId,
                                    dataChnlId,
                                    timeOut2ForReclaim,
                                    &dataIoReq) ;
            tempStatus = TST_StrToStatus (argv [3],
                                          &expStatusOfSecondReclaim) ;

            if (DSP_SUCCEEDED (tempStatus)) {
                if (statusOfSecondReclaim != expStatusOfSecondReclaim) {
                    status = DSP_EFAIL ;
                    TST_PrnError ("Second Reclaim ended with unexpected "
                                  "status. Status = [0x%x]",
                                  statusOfSecondReclaim) ;
                }
            }
            else {
                status = DSP_EFAIL ;
                TST_PrnError ("TST_StrToStatus Failed. Status = [0x%x]",
                              status) ;
            }
        }
    }

    tempStatus = CHNL_FreeBuffer (dspId,
                                  dataChnlId,
                                  (Char8 **) dataBuffers,
                                  1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't free data buffers. Status = [0x%x]",
                      status) ;
    }

    tempStatus = CHNL_FreeBuffer (dspId,
                                  OUTPUT_CONTROL_CHNL_ID,
                                  (Char8 **) outputControlBuffers,
                                  1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't free output control buffers. Status = [0x%x]",
                      status) ;
    }

    tempStatus = CHNL_Delete (dspId, dataChnlId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete data channel. Status = [0x%x]",
                      status) ;
    }

    tempStatus = CHNL_Delete (dspId, OUTPUT_CONTROL_CHNL_ID) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete output control channel. Status = [0x%x]",
                      status) ;
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
