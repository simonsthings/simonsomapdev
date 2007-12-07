/** ============================================================================
 *  @file   ANA_Loop.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\analysis
 *
 *  @desc  Implements a test case which tests the Channel issue
 *         reclaim performance synchronously.
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

/*  ----------------------------------- Link Driver                   */
#include <chnl.h>
#include <proc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Analysis.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <ANA_Loop.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   ANA_Loop
 *
 *  @desc   Tests the Channel issue  reclaim performance
 *          synchronously.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ANA_Loop (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status         = DSP_SOK ;
    Uint32        timeOut        = 0       ;
    Uint32        bufSize        = 0       ;
    Uint32        count          = 0       ;
    Uint32        stopTime       = 0       ;
    Uint32        processorId    = 0       ;
    Uint32        channelInput   = 0       ;
    Uint32        channelOutput  = 1       ;
    Uint32        iterations     = 0       ;
    ChannelAttrs  chnlAttrInput            ;
    ChannelAttrs  chnlAttrOutput           ;
    Char8       * bufInput  [1]            ;
    Char8       * bufOutput [1]            ;
    ChannelIOInfo ioReqInput               ;
    ChannelIOInfo ioReqOutput              ;
    Uint32        noOfArguments            ;
    Char8       * arrayOfArguments [10]    ;

    DBC_Require (argc == 7) ;
    DBC_Require (argv != NULL) ;
    if ((argc != 7) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Wrong no of arguments passed. Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        processorId   = TST_StringToInt (argv [0]) ;
        channelInput  = TST_StringToInt (argv [1]) ;
        channelOutput = TST_StringToInt (argv [2]) ;
        bufSize       = TST_StringToInt (argv [4]) ;
        iterations    = TST_StringToInt (argv [5]) ;
        status = PROC_Setup () ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (processorId, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        noOfArguments = 2 ;
        arrayOfArguments [0] = argv [4] ;
        arrayOfArguments [1] = argv [6] ;
        status = PROC_Load (processorId,
                            argv [3],
                            noOfArguments,
                            arrayOfArguments) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (processorId) ;
    }

    if (DSP_SUCCEEDED (status)) {
        chnlAttrInput.mode = ChannelMode_Input ;
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
        status = CHNL_AllocateBuffer
                    (processorId, channelInput, (Char8 **) &bufInput,
                     bufSize ,1) ;
        status = CHNL_AllocateBuffer
                    (processorId, channelOutput, (Char8 **) &bufOutput,
                     bufSize ,1) ;
    }

    if (DSP_SUCCEEDED (status)) {
        ioReqInput.size = bufSize ;
        ioReqOutput.size = bufSize ;
        timeOut =  WAIT_FOREVER ;

        TST_ZeroTime () ;

        for (count = 0 ;   (count < iterations)
                       && (DSP_SUCCEEDED (status)) ; count++) {

            if (DSP_SUCCEEDED (status)) {
                ioReqOutput.buffer = bufOutput [0] ;
                status = CHNL_Issue (processorId,
                                     channelOutput,
                                     &ioReqOutput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("ISSUE Failed. Status = [0x%x]", status) ;
            }
            if (DSP_SUCCEEDED (status)) {
                ioReqInput.buffer = bufInput [0] ;
                status = CHNL_Issue (processorId,
                                     channelInput,
                                     &ioReqInput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("ISSUE Failed. Status = [0x%x]", status) ;
            }

            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim(processorId,
                                      channelOutput,
                                      timeOut,
                                      &ioReqOutput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("Reclaim Failed. Status = [0x%x]", status) ;
            }
            if (DSP_SUCCEEDED (status)) {
                status = CHNL_Reclaim (processorId,
                                       channelInput,
                                       timeOut,
                                       &ioReqInput) ;
            }
            if (DSP_FAILED (status)) {
                TST_PrnError ("Reclaim Failed. Status = [0x%x]", status) ;
            }
        }
        stopTime = TST_GetTime () ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = TST_DoAnalysis (0, stopTime, (iterations * 2), bufSize) ;
    }

    CHNL_FreeBuffer (processorId,
                     channelInput,
                     (Char8 **) &bufInput,
                     1) ;

    CHNL_FreeBuffer (processorId,
                     channelOutput,
                     (Char8 **) &bufOutput,
                     1) ;

    CHNL_Delete (processorId, channelInput) ;
    CHNL_Delete (processorId, channelOutput) ;
    PROC_Stop (processorId) ;
    PROC_Detach (processorId) ;
    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
