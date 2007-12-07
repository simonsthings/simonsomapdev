/** ============================================================================
 *  @file   BVR_DataWrite.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implementation of behavior test for writing data to DSP.
 *          Host writes the DSP and then receives the buffer back. The buffers
 *          are the size of the entire shared memory.
 *          The data received is  tested against full range of data
 *          values possible from 0x0000 to 0xffff, the values that were written
 *          to the buffers before sending them to DSP.
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
#include <BVR_DataWrite.h>
#include <BVR_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   TEST_CHANNEL_TIMEOUT
 *
 *  @desc   Timeout for reclaim timeout.
 *  ============================================================================
 */
#define TEST_CHANNEL_TIMEOUT   WAIT_FOREVER

/** ============================================================================
 *  @name   MAX_ARGS
 *
 *  @desc   The maximum no of arguments which can be passed to DSP.
 *  ============================================================================
 */
#define MAX_ARGS 10


/** ----------------------------------------------------------------------------
 *  @func   FillBuffer
 *
 *  @desc   This function fill the specified memory buffer with the specified
 *          value.
 *
 *  @arg    buffer
 *              Buffer to be filled.
 *  @arg    size
 *              size of the buffer.
 *  @arg    value
 *              Uint8 value with which to fill the buffer.
 *
 *  @ret    None
 *
 *  @enter  buffer must be valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
FillBuffer (Char8 * buffer, Uint32 size, Uint8 value) ;


/** ============================================================================
 *  @func   BVR_DataWrite
 *
 *  @desc   Function to test the behavior of data write test.
 *          This is essentially a loopback test. It sends data and verifies
 *          data transfer by checking for the buffer received. The DSP side
 *          simply received the buffer and sends it back.
 *          All data values of form 0xNNNN are tested where 'N' is the number
 *          of buffer being sent.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_DataWrite (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status        = DSP_SOK ;
    DSP_STATUS    statusOfSetup = DSP_SOK ;
    DSP_STATUS    tempStatus    = DSP_SOK ;
    Uint32        bufSize       = 0       ;
    ChannelId     readChnlId    = 1       ;
    ChannelId     writeChnlId   = 0       ;
    Pstr          fileName      = NULL    ;
    ProcessorId   dspId         = 0       ;
    Char8 *       readBuffer  [1]         ;
    Char8 *       writeBuffer [1]         ;
    ChannelAttrs  chnlAttrs               ;
    ChannelIOInfo write_io_info           ;
    ChannelIOInfo read_io_info            ;
    Uint32        i                       ;
    Uint8         indexBuf                ;
    Char8         verifyVal               ;
    Uint32        noOfArguments           ;


    Char8  *      arrayOfArguments [MAX_ARGS] ;


    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 6) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 6) && (argv != NULL)) {
        statusOfSetup = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_DataWrite. Status = [0x%x]",
                      statusOfSetup) ;
    }
    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (statusOfSetup)) {
        dspId    = TST_StringToInt (argv [2]) ;
        fileName = argv [3] ;
        bufSize  = TST_StringToInt (argv [4]) ;

        statusOfSetup = PROC_Setup () ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Attach (dspId, NULL) ;
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to attach to DSP. Status = [0x%x]",
                      statusOfSetup) ;
        }
        else {
            noOfArguments = 2 ;
            arrayOfArguments [0] = argv [4] ;
            arrayOfArguments [1] = argv [5] ;

            statusOfSetup = PROC_Load (dspId,
                                       fileName,
                                       noOfArguments,
                                       arrayOfArguments) ;
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
            chnlAttrs.mode      = ChannelMode_Output ;
            chnlAttrs.endianism = Endianism_Default  ;
            chnlAttrs.size      = ChannelDataSize_16bits ;

            statusOfSetup = CHNL_Create (dspId, writeChnlId, &chnlAttrs) ;
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to create write channel. Status = [0x%x]",
                      statusOfSetup) ;
        }
        else {
            statusOfSetup = CHNL_AllocateBuffer (dspId,
                                                 writeChnlId,
                                                 writeBuffer,
                                                 bufSize,
                                                 1) ;   /* Number of buffers */
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on write channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
        else {
            chnlAttrs.mode      = ChannelMode_Input ;
            chnlAttrs.endianism = Endianism_Default  ;
            chnlAttrs.size      = ChannelDataSize_16bits ;

            statusOfSetup = CHNL_Create (dspId, readChnlId, &chnlAttrs) ;
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to create read channel. Status = [0x%x]",
                      statusOfSetup) ;
        }
        else {
            statusOfSetup = CHNL_AllocateBuffer (dspId,
                                                 readChnlId,
                                                 readBuffer,
                                                 bufSize,
                                                 1) ;   /* Number of buffers */
        }

        if (DSP_FAILED (statusOfSetup)) {
            TST_PrnError ("Failed to allocate buffer on read channel. "
                          "Status = [0x%x]", statusOfSetup) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Test BVR_DataWrite.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        for (indexBuf = 0 ;
             (DSP_SUCCEEDED (status) && (indexBuf < 0xF)) ;
             indexBuf++) {

            verifyVal = (Uint8)((indexBuf << 4) | indexBuf) ;

            /* Fill the buffer with value signifying the iteration number*/
            FillBuffer (writeBuffer[0], bufSize, verifyVal) ;

            /* Send the written buffer ot DSP */
            write_io_info.buffer = writeBuffer[0]  ;
            write_io_info.size   = bufSize ;
            write_io_info.arg    = 0            ;
            status = CHNL_Issue (dspId, writeChnlId, &write_io_info) ;

            /* Reclaim the written buffer */
            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (dspId,
                                       writeChnlId,
                                       TEST_CHANNEL_TIMEOUT,
                                       &write_io_info) ;
            }
            else {
                TST_PrnError ("Couldn't issue buffer to write channel. "
                              "Status = [0x%x]", status) ;
            }

            /* Now issue an empty buffer on read channel */
            if (DSP_SUCCEEDED (status)) {
                read_io_info.buffer = readBuffer[0]  ;
                read_io_info.size   = bufSize ;
                read_io_info.arg    = 0            ;
                status = CHNL_Issue (dspId, readChnlId, &read_io_info) ;
            }
            else {
                TST_PrnError ("Couldn't reclaim buffer on write channel. "
                              "Status = [0x%x]", status) ;
            }

            /* Now reclaim the buffer on read channel - expecting it to
             * be filled by DSP
             */
            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (dspId,
                                       readChnlId,
                                       TEST_CHANNEL_TIMEOUT,
                                       &read_io_info) ;
            }
            else {
                TST_PrnError ("Couldn't issue buffer on read channel. "
                              "Status = [0x%x]", status) ;
            }

            /* Now verify the buffer content */
            if (DSP_SUCCEEDED (status)) {
                for (i = 0 ;
                     DSP_SUCCEEDED (status) && (i < (bufSize / DSP_MAUSIZE)) ;
                     i++) {
                    if (read_io_info.buffer [i] != verifyVal) {
                        status = DSP_EFAIL ;
                        TST_PrnError ("Incorrect value received during read. "
                                      "Status = [0x%x]", status) ;
                    }
                }
            }
        }
    }

    tempStatus = CHNL_FreeBuffer (dspId, readChnlId, readBuffer, 1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("CHNL_FreeBuffer failed. Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_Delete (dspId, readChnlId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("CHNL_Delete failed. Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_FreeBuffer (dspId, writeChnlId, writeBuffer, 1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("CHNL_FreeBuffer failed. Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_Delete (dspId, writeChnlId) ;
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


/** ----------------------------------------------------------------------------
 *  @func   FillBuffer
 *
 *  @desc   This function fill the specified memory buffer with the specified
 *          value.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
FillBuffer (Char8 * buffer, Uint32 size, Uint8 value)
{
    Uint32 i ;

    DBC_Require (buffer != NULL) ;

    for (i = 0 ; i < size ; i++) {
        buffer [i] = value ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
