/** ============================================================================
 *  @file   BVR_EndOfStream.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of BVR_EndOfStream test case.
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
#include <BVR_PostProcess.h>
#include <BVR_EndOfStream.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   TEST_SLEEP_TIME
 *
 *  @desc   Sleep time for the test.
 *  ============================================================================
 */
#define TEST_SLEEP_TIME 10


/** ----------------------------------------------------------------------------
 *  @func   PROCInitialize
 *
 *  @desc   Initializes the DSP.
 *
 *  @arg    argc
 *              Count of the no. of arguments passed.
 *  @arg    argv
 *              List of arguments.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EMEMORY
 *              Memory error
 *          DSP_EFAIL
 *              General failure
 *          DSP_SALREADYATTACHED
 *              Operation Successfully completed. Also, indicates
 *              that another client had already attached to DSP.
 *          DSP_EINVALIDARG
 *              Parameter ProcId is invalid.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFILE
 *              Invalid base image.
 *          DSP_SALREATESTARTED
 *              DSP is already in running state.
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
PROCInitialize (Uint32 processorId, Char8 * fileName) ;


/** ============================================================================
 *  @func   BVR_EndOfStream
 *
 *  @desc   This test loads the DSP binary image and starts it and then does 1
 *          data transfer and then idles the channel and then sleeps.
 *          The dsp then tries to send data to the GPP. It tests if the gpp
 *          side is stable enough to handel unwanted interrupts from DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_EndOfStream (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS     statusOfSetup         = DSP_SOK   ;
    DSP_STATUS     status                = DSP_SOK   ;
    Uint32         bufSize               = 0         ;
    Uint32         timeOut               = 0         ;
    Uint32         processorId           = 0         ;
    Uint32         channelId             = 0         ;
    Char8 *        fileName                          ;
    ChannelAttrs   chnlAttr                          ;
    ChannelIOInfo  ioReq                             ;
    Char8       *  bufArray [1]                      ;

    DBC_Require (argc == 12) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 12) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_EndOfStream. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        processorId             = TST_StringToInt (argv [2])  ;
        channelId               = TST_StringToInt (argv [3])  ;
        fileName                = argv [4]                    ;
        chnlAttr.mode           = (ChannelMode) (TST_StringToInt (argv [5])) ;
        chnlAttr.endianism      = (Endianism) (TST_StringToInt (argv [6]))   ;
        chnlAttr.size           = (ChannelDataSize)
                                  (TST_StringToInt (argv [7])) ;
        bufSize                 = TST_StringToInt (argv [8])  ;
        ioReq.size              = TST_StringToInt (argv [9]) ;
        ioReq.arg               = TST_StringToInt (argv [10]) ;
        timeOut                 = TST_StringToInt (argv [11]) ;

        if (timeOut == 1) {
            timeOut = WAIT_FOREVER ;
        }
        else if (timeOut == 0) {
            timeOut = WAIT_NONE ;
        }

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = PROCInitialize (processorId ,fileName) ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = CHNL_Create (processorId, channelId, &chnlAttr) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            bufArray [0] = NULL ;
            statusOfSetup = CHNL_AllocateBuffer (processorId,
                                                 channelId,
                                                 bufArray,
                                                 bufSize,
                                                 1) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            ioReq.buffer = bufArray [0] ;
            statusOfSetup = CHNL_Issue (processorId, channelId, &ioReq) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = CHNL_Reclaim (processorId,
                                          channelId,
                                          timeOut,
                                          &ioReq) ;
        }

        /*
         * Receive the END OF STREAM
         */

        if (DSP_SUCCEEDED (statusOfSetup)) {
            ioReq.buffer = bufArray [0] ;
            statusOfSetup = CHNL_Issue (processorId, channelId, &ioReq) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = CHNL_Reclaim (processorId,
                                          channelId,
                                          timeOut,
                                          &ioReq) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        ioReq.buffer = bufArray [0] ;
        status = CHNL_Issue (processorId, channelId, &ioReq) ;
    }

    TST_Sleep (TEST_SLEEP_TIME) ;

    if (DSP_SUCCEEDED (status)) {
        status = CHNL_Reclaim (processorId,
                               channelId,
                               timeOut,
                               &ioReq) ;
    }

    /*
     * Sleep for some time to give dsp the time to run and send unwanted data.
     */
    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    CHNL_FreeBuffer (processorId, channelId, bufArray, 1) ;
    CHNL_Delete (processorId, channelId) ;
    PROC_Stop (processorId) ;
    PROC_Detach (processorId) ;
    PROC_Destroy () ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   PROCInitialize
 *
 *  @desc   This function does PROC_Setup, PROC_Attach, PROC_Load and
 *          PROC_Start.
 *
 *  @modif  none
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
PROCInitialize (Uint32 processorId ,Char8 * fileName)
{
    DSP_STATUS status = DSP_SOK ;

    status = PROC_Setup () ;

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (processorId, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Load (processorId, fileName, 0, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (processorId) ;
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */