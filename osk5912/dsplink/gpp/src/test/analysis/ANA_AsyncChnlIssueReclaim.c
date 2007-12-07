/** ============================================================================
 *  @file   ANA_AsyncChnLIssueReclaim.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\analysis
 *
 *  @desc   Implemetation of test case which tests the Channel issue
 *          reclaim performance asynchronously.
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
#include <TST_Analysis.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <ANA_AsyncChnlIssueReclaim.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   ANA_AsyncChnlIssueReclaim
 *
 *  @desc   This function tests the Channel issue  reclaim performance
 *          asynchronously
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ANA_AsyncChnlIssueReclaim (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    status                = DSP_SOK ;
    Uint32        timeOut               = 0       ;
    Uint32        bufSize               = 0       ;
    Uint32        count                 = 0       ;
    Uint32        processorId           = 0       ;
    Uint32        channelId             = 0       ;
    Uint32        stopTime              = 0       ;
    Uint32        noOfIssueReclaim      = 0       ;
    Uint32        iterations            = 0       ;
    Uint32        i                     = 0       ;
    ChannelAttrs  chnlAttr                        ;
    Char8       * bufInput [MAX_BUFFERS]          ;
    ChannelIOInfo ioReqInput                      ;
    Uint32        noOfArguments                   ;
    Char8       * arrayOfArguments [10]           ;

    DBC_Require (argc != 7) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 7) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Wrong no of arguments passed. Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        processorId = TST_StringToInt (argv [0]) ;
        channelId   = TST_StringToInt (argv [1]) ;
        bufSize     = TST_StringToInt (argv [3]) ;
        noOfIssueReclaim  = TST_StringToInt (argv [4]) ;
        iterations  = TST_StringToInt (argv [5]) ;

        status = PROC_Setup () ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (processorId, NULL) ;
    }

    if (DSP_SUCCEEDED (status)) {
        noOfArguments = 2 ;
        arrayOfArguments [0] = argv [3] ;
        arrayOfArguments [1] = argv [6] ;
        status = PROC_Load (processorId,
                            argv [2],
                            noOfArguments,
                            arrayOfArguments) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (processorId) ;
    }

    if (DSP_SUCCEEDED (status)) {
        if (channelId == 0) {
            chnlAttr.mode = ChannelMode_Output ;
        }
        else if (channelId == 1) {
            chnlAttr.mode = ChannelMode_Input ;
        }
        else {
            status = DSP_EINVALIDARG ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        chnlAttr.endianism = Endianism_Default ;
        chnlAttr.size = ChannelDataSize_16bits ;
        status = CHNL_Create (processorId, channelId, &chnlAttr) ;
    }

    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; i < noOfIssueReclaim ; i++) {
            bufInput [i] = NULL ;
        }
        status = CHNL_AllocateBuffer (processorId,
                                      channelId,
                                      bufInput,
                                      bufSize ,
                                      noOfIssueReclaim) ;
        ioReqInput.size = bufSize ;
        timeOut =  WAIT_FOREVER ;
    }
    if (DSP_SUCCEEDED (status)) {
        TST_ZeroTime () ;
    }

    for (i = 0 ; i < iterations ; i++) {
        for (count = 0 ;   (count < noOfIssueReclaim)
                       && (DSP_SUCCEEDED (status)); count++) {
            ioReqInput.buffer = bufInput [count] ;
            status = CHNL_Issue (processorId, channelId, &ioReqInput) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("ISSUE Failed. Status = [0x%x]", status) ;
            }
        }

        for (count= 0 ;   (count < noOfIssueReclaim)
                       && (DSP_SUCCEEDED (status)); count++) {
            ioReqInput.buffer = bufInput [count] ;
            status = CHNL_Reclaim (processorId,
                                   channelId,
                                   timeOut,
                                   &ioReqInput) ;
            if (DSP_FAILED (status)) {
                TST_PrnError ("Reclaim Failed. Status = [0x%x]", status) ;
            }
        }
    }

    if (DSP_SUCCEEDED (status)) {
        stopTime = TST_GetTime () ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = TST_DoAnalysis (0,
                                 stopTime,
                                 (noOfIssueReclaim * iterations),
                                 bufSize) ;
    }

    CHNL_FreeBuffer (processorId, channelId, bufInput, noOfIssueReclaim) ;
    CHNL_Delete (processorId, channelId) ;
    PROC_Stop (processorId) ;
    PROC_Detach (processorId) ;
    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
