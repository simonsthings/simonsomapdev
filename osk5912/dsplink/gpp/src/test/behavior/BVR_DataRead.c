/** ============================================================================
 *  @file   BVR_DataRead.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implementation of behavior test for reading data from DSP.
 *          Host receives buffer that is the size of the entire shared
 *          memory. The data received will be tested against full range
 *          of data values possible from 0x0000 to 0xffff. The DSP sends
 *          buffers with these values in subsequent iterations.
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
#include <chnl.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <BVR_DataRead.h>
#include <BVR_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   BUFSIZE
 *
 *  @desc   Buffer size of data transfer
 *  ============================================================================
 */
#define BUFSIZE           100

/** ============================================================================
 *  @name   TEST_CHANNEL_TIMEOUT
 *
 *  @desc   Timeout for reclaim timeout.
 *  ============================================================================
 */
#define TEST_CHANNEL_TIMEOUT   WAIT_FOREVER


/** ============================================================================
 *  @func   BVR_DataRead
 *
 *  @desc   Function that reads data written by DSP. It assumes that the DSP
 *          keeps sending data on channel 0. The data buffers are assumed to
 *          be filled with a particular value - depending on the order in which
 *          they are send/received.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_DataRead (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status        = DSP_SOK ;
    DSP_STATUS    statusOfSetup = DSP_SOK ;
    DSP_STATUS    tempStatus    = DSP_SOK ;
    Pstr          fileName      = NULL    ;
    ChannelId     readChnlId    = 1       ;
    ProcessorId   dspId         = 0       ;
    Uint16 *      ptr16         = NULL    ;
    Uint8 *       ptr8          = NULL    ;
    Char8 *       buffer [1]              ;
    ChannelAttrs  chnlAttrs               ;
    ChannelIOInfo io_info                 ;
    Uint16        i                       ;
    Uint8         indexBuf                ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 4) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 4) && (argv != NULL)) {
        statusOfSetup = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_DataRead. Status = [0x%x]",
                      statusOfSetup) ;
    }
    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId    = TST_StringToInt (argv [2]) ;
        fileName = argv [3]                   ;

        statusOfSetup = PROC_Setup () ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Attach (dspId, NULL) ;
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to attach to DSP. Status = [0x%x]",
                          statusOfSetup) ;
        }
        else {
            statusOfSetup = PROC_Load (dspId, fileName, 0, NULL) ;
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to Load on DSP. Status = [0x%x]",
                          statusOfSetup) ;
        }
        else {
            statusOfSetup = PROC_Start (dspId) ;
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to start DSP. Status = [0x%x]",
                          statusOfSetup) ;
        }
        else {
            chnlAttrs.mode      = ChannelMode_Input ;
            chnlAttrs.endianism = Endianism_Default ;
            chnlAttrs.size      = ChannelDataSize_16bits ;
            statusOfSetup = CHNL_Create (dspId, readChnlId, &chnlAttrs) ;
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to create channel. Status = [0x%x]",
                          statusOfSetup) ;
        }
        else {
            statusOfSetup = CHNL_AllocateBuffer (dspId,
                                                 readChnlId,
                                                 (Char8 **) buffer,
                                                 BUFSIZE,
                                                 1) ;   /* Number of buffers */
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on read channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
    }
    /*  --------------------------------------------------------------------
     *  Test BVR_DataRead.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        /* Iterate for all possible byte values that can be received from DSP */
        for (indexBuf = 0 ;
             (DSP_SUCCEEDED (status) && (indexBuf < 0xF)) ;
             indexBuf++) {

            /* Issue an empty buffer to read data and then reclaim it */
            io_info.buffer = (Char8 *)buffer [0]      ;
            io_info.size   = BUFSIZE ;
            io_info.arg    = 0       ;
            status = CHNL_Issue (dspId, readChnlId, &io_info) ;
            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (dspId,
                                       readChnlId,
                                       TEST_CHANNEL_TIMEOUT,
                                       &io_info) ;
            }
            else {
                TST_PrnError ("Failed to issue on channel. Status = [0x%x]",
                              status) ;
            }

            /* Verify data received on buffer */
            ptr8 = (Char8 *) (buffer [0]) ;
            ptr16 = (Uint16 *) (buffer [0]) ;
            if (DSP_SUCCEEDED (status)) {
                for (i = 0 ;
                     DSP_SUCCEEDED (status) && (i < BUFSIZE / DSP_MAUSIZE) ;
                     i++) {
                    if (DSP_MAUSIZE == 1) {
                        if (*ptr8 != i) {
                            status = DSP_EFAIL ;
                            TST_PrnError ("Received incorrect value. Status = [0x%x]",
                                          status) ;
                            TST_PrnInfo ("Incorrect value %d at %d", *ptr8, i) ;
                        }
                        ptr8++ ;
                    }
                    else if (DSP_MAUSIZE == 2) {
                        if (*ptr16 != i) {
                            status = DSP_EFAIL ;
                            TST_PrnError ("Received incorrect value. Status = [0x%x]",
                                          status) ;
                            TST_PrnInfo ("Incorrect value %d at %d",
                                         *ptr16, i) ;
                        }
                        ptr16++ ;
                    }
                }
            }
            else {
                TST_PrnError ("Failed to reclaim buffer on channel. "
                              "Status = [0x%x]", status) ;
            }
        }
    }

    tempStatus = CHNL_FreeBuffer (dspId, readChnlId, (Char8 **) buffer, 1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
            TST_PrnError ("CHNL_FreeBuffer failed. Status = [0x%x]", status) ;
        }

    tempStatus = CHNL_Delete (dspId, readChnlId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("CHNL_Delete failed. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Stop (dspId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("PROC_Stop failed. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Detach (dspId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("PROC_Detach failed. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Destroy () ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("PROC_Destroy failed. Status = [0x%x]", status) ;
    }

    status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
