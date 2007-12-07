/** ============================================================================
 *  @file   API_ChnlIdle.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of Chnl_Idle API test case.
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
#include <API_PostProcess.h>
#include <API_ChnlIdle.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


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
 *  @func   API_ChnlIdle
 *
 *  @desc   This test case tests the Chnl_Idle  API
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_ChnlIdle (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS     statusOfSetup         = DSP_SOK   ;
    DSP_STATUS     status                = DSP_SOK   ;
    Uint32         bufSize               = 0         ;
    Uint32         bufNum                = 0         ;
    Uint32         timeOut               = 0         ;
    Uint32         processorId           = 0         ;
    Uint32         channelId             = 0         ;
    Uint32         i                     = 0         ;
    Char8 *        fileName                          ;
    ChannelAttrs   chnlAttr                          ;
    ChannelIOInfo  ioReq                             ;
    Char8       *  bufArray [MAX_BUFSIZE]            ;

    DBC_Require (argc == 13) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 13) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_ChnlIdle. Status = [0x%x]",
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
        chnlAttr.endianism      = (Endianism) (TST_StringToInt (argv [6])) ;
        chnlAttr.size           = (ChannelDataSize)
                                  (TST_StringToInt (argv [7])) ;
        bufSize                 = TST_StringToInt (argv [8])  ;
        bufNum                  = TST_StringToInt (argv [9])  ;
        ioReq.size              = TST_StringToInt (argv [10]) ;
        ioReq.arg               = TST_StringToInt (argv [11]) ;
        timeOut                 = TST_StringToInt (argv [12]) ;

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
            for (i = 0 ; i < bufNum ; i++) {
                bufArray [i] = NULL ;
            }
            statusOfSetup = CHNL_AllocateBuffer (processorId,
                                                 channelId,
                                                 bufArray,
                                                 bufSize,
                                                 bufNum) ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            ioReq.buffer = bufArray [0] ;
            statusOfSetup = CHNL_Issue (processorId, channelId, &ioReq) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests Chnl_Cancel.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        status = CHNL_Idle (processorId, channelId) ;
    }

    status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    CHNL_Reclaim (processorId, channelId, timeOut, &ioReq) ;
    CHNL_FreeBuffer (processorId, channelId, bufArray, bufNum) ;
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
 *  @modif  None
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
