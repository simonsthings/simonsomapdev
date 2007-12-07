/** ============================================================================
 *  @file   strmcopy.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\Application
 *
 *  @desc   Implements the strmcopy test which sends the data across
 *          the LINK with data validation.
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

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- Processor Manager             */
#include <proc.h>
#include <chnl.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Analysis.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <strmcopy.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   StrmCopy
 *
 *  @desc   This test the strmcopy test which sends the data across
 *          the LINK with data validation
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
StrmCopy (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status         = DSP_SOK ;
    DSP_STATUS    tempStatus     = DSP_SOK ;
    Uint32        bufSize        = 0       ;
    Uint32        processorId    = 0       ;
    Uint32        channelInput   = 0       ;
    Uint32        channelOutput  = 1       ;
    Uint32        iterations     = 0       ;
    Uint32        timeOut                  ;
    ChannelAttrs  chnlAttrInput            ;
    ChannelAttrs  chnlAttrOutput           ;
    Char8       * bufInput  [1]            ;
    Char8       * bufOutput [1]            ;
    ChannelIOInfo ioReqInput               ;
    ChannelIOInfo ioReqOutput              ;
    Uint32        count                    ;
    Uint32        i                        ;

    DBC_Require (argc == 6) ;
    DBC_Require (argv != NULL) ;
    if ((argc != 6) || (argv == NULL)) {
        TST_PrnError ("Wrong no of arguments passed. Status = [0x%x]", status) ;
        status = DSP_EINVALIDARG ;
    }

    if (DSP_SUCCEEDED (status)) {
        processorId   = TST_StringToInt (argv [0]) ;
        channelInput  = TST_StringToInt (argv [1]) ;
        channelOutput = TST_StringToInt (argv [2]) ;
        bufSize       = TST_StringToInt (argv [4]) ;
        iterations    = TST_StringToInt (argv [5]) ;
        status        = PROC_Setup ()              ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (processorId, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Load (processorId, argv [3], 0, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (processorId) ;
    }

    if (DSP_SUCCEEDED (status)) {
        chnlAttrInput.mode = ChannelMode_Input      ;
        chnlAttrInput.endianism = Endianism_Default ;
        chnlAttrInput.size = ChannelDataSize_16bits ;
        status = CHNL_Create (processorId, channelInput,
                                    &chnlAttrInput) ;
    }

    if (DSP_SUCCEEDED (status)) {
        chnlAttrOutput.mode = ChannelMode_Output ;
        chnlAttrOutput.endianism = Endianism_Default ;
        chnlAttrOutput.size = ChannelDataSize_16bits ;
        status = CHNL_Create (processorId, channelOutput,
                                    &chnlAttrOutput) ;
    }

    if (DSP_SUCCEEDED (status)) {
        bufInput [1] = NULL ;
        status = CHNL_AllocateBuffer
                    (processorId, channelInput, (Char8 **) &bufInput,
                     bufSize ,1) ;
        status = CHNL_AllocateBuffer
                    (processorId, channelOutput, (Char8 **) &bufOutput,
                     bufSize ,1) ;
    }

    if (DSP_SUCCEEDED (status)) {
        ioReqInput.size  = bufSize ;
        ioReqOutput.size = bufSize ;
        timeOut =  WAIT_FOREVER ;
        for (count = 0 ;  (count < iterations)
                       && (DSP_SUCCEEDED (status)) ; count++) {
            for (i = 0 ; i < bufSize ; i++) {
                bufOutput [0][i] = 'a' ;
            }

            if (DSP_SUCCEEDED (status)) {
                ioReqOutput.buffer = bufOutput [0] ;
                status = CHNL_Issue
                            (processorId, channelOutput, &ioReqOutput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("ISSUE Failed. Status = [0x%x]", status) ;
            }
            if (DSP_SUCCEEDED (status)) {
                ioReqInput.buffer = bufInput [0] ;
                status = CHNL_Issue
                            (processorId, channelInput, &ioReqInput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("ISSUE Failed. Status = [0x%x]", status) ;
            }

            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim
                                (processorId, channelOutput,
                                 timeOut, &ioReqOutput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("Reclaim Failed. Status = [0x%x]", status) ;
            }
            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim
                                (processorId, channelInput,
                                 timeOut, &ioReqInput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("Reclaim Failed. Status = [0x%x]", status) ;
            }

            if (DSP_SUCCEEDED (status)) {
                for (i = 0 ;   (i < bufSize)
                            && (DSP_SUCCEEDED (status)) ; i++) {
                    if (bufOutput [0][i] != 'a') {
                        status = DSP_EFAIL ;
                    }
                }
            }

            if ((i % 1000) == 0) {
                TST_PrnInfo ("[%d]", i) ;
            }
        }
    }

    tempStatus = CHNL_FreeBuffer (processorId,
                     channelInput,
                     (Char8 **) &bufInput,
                     1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
            TST_PrnError ("Couldn't free buffers. Status = [0x%x]", status) ;
        }

    tempStatus = CHNL_Delete (processorId, channelInput) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete channel. Status = [0x%x]", status) ;
    }

    tempStatus = CHNL_FreeBuffer (processorId,
                     channelOutput,
                     (Char8 **) &bufOutput,
                     1) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
            TST_PrnError ("Couldn't free buffers. Status = [0x%x]", status) ;
        }

    tempStatus = CHNL_Delete (processorId, channelOutput) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't delete channel. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Detach (processorId) ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't call destroy. Status = [0x%x]", status) ;
    }

    tempStatus = PROC_Destroy () ;
    if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tempStatus))) {
        status = tempStatus ;
        TST_PrnError ("Couldn't call destroy. Status = [0x%x]", status) ;
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
