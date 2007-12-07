/** ============================================================================
 *  @file   STS_MsgqUtils.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress\Linux
 *
 *  @desc   Defines utility functions for STS Msgq tests.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <stdarg.h>

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

/*  ----------------------------------- Link API                      */
#include <proc.h>
#include <chnl.h>
#include <msgqdefs.h>
#include <msgq.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_Mem.h>
#include <TST_PrintFuncs.h>
#include <STS_MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_POOLS
 *
 *  @desc   Number of pools to be configured for the allocator.
 *  ============================================================================
 */
#define NUM_POOLS 3

/** ============================================================================
 *  @macro  TST_MAX_MSGQS
 *
 *  @desc   The maximum number of message queues on the DSP.
 *  ============================================================================
 */
#define TST_MAX_MSGQS MAX_MSGQS


/** ============================================================================
 *  @func   STS_OpenAllocator
 *
 *  @desc   Opens an allocator.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_OpenAllocator (IN AllocatorId mqaId,
                   IN Uint16      numMsgPerPool,
                   IN Uint16      maxMsgSize)
{
    DSP_STATUS  status      = DSP_SOK                     ;
    Uint16      msgSize [NUM_POOLS] = {RMQT_CTRLMSG_SIZE,
                                       ALIGN (sizeof (TestMsg))}   ;
    Uint16      numMsg  [NUM_POOLS]                       ;
    MqaBufAttrs mqaAttrs    = {NUM_POOLS,
                               msgSize,
                               numMsg}                    ;

    numMsg [0]  = numMsgPerPool   ;
    numMsg [1]  = numMsgPerPool   ;
    numMsg [2]  = numMsgPerPool   ;
    msgSize [2] = maxMsgSize      ;

    status = MSGQ_AllocatorOpen (mqaId, (Pvoid) &mqaAttrs) ;

    return status ;
}


/** ============================================================================
 *  @func   STS_CloseAllocator
 *
 *  @desc   Closes an allocator.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_CloseAllocator (IN AllocatorId mqaId)
{
    DSP_STATUS status = DSP_SOK ;

    status = MSGQ_AllocatorClose (mqaId) ;

    return status ;
}


/** ============================================================================
 *  @func   STS_OpenTransport
 *
 *  @desc   Opens transport. Assumes the id TST_LMQT_ID on GPP is local transport.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_OpenTransport (IN TransportId mqtId,
                   IN AllocatorId mqaId,
                   IN Uint16      maxMsgSize,
                   IN Uint16      numMsgqs)
{
    DSP_STATUS status    = DSP_SOK         ;
    LmqtAttrs  lmqtAttrs = {numMsgqs}      ;
    RmqtAttrs  rmqtAttrs              ;

    if (mqtId == TST_LMQT_ID) {
        status = MSGQ_TransportOpen (mqtId, (Pvoid) &lmqtAttrs) ;
    }
    else {
        if (maxMsgSize < RMQT_CTRLMSG_SIZE) {
            maxMsgSize = RMQT_CTRLMSG_SIZE ;
        }

        if (maxMsgSize < ALIGN (sizeof (TestMsg))) {
            maxMsgSize = ALIGN (sizeof (TestMsg)) ;
        }

        rmqtAttrs.maxNumMsgq   = TST_MAX_MSGQS ;
        rmqtAttrs.maxMsgSize   = maxMsgSize    ;
        rmqtAttrs.defaultMqaId = mqaId ;
        status = MSGQ_TransportOpen (mqtId, (Pvoid) &rmqtAttrs) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   STS_CloseTransport
 *
 *  @desc   Closes transport.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_CloseTransport (IN TransportId mqtId)
{
    DSP_STATUS status = DSP_SOK ;

    status = MSGQ_TransportClose (mqtId) ;

    return status ;
}


/** ============================================================================
 *  @func   STS_SetupMessaging
 *
 *  @desc   Setup the proc and messaging components. This function calls
 *          PROC_Setup, MSGQ_AllocatorOpen, PROC_Attach, PROC_Load, PROC_Start
 *          and MSGQ_TransportOpen.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_SetupMessaging (IN ProcessorId procId,
                    IN Uint16      numMsgPerPool,
                    IN Uint16      maxMsgSize,
                    IN Uint16      numMsgqs,
                    IN Char8 *     imagePath,
                    IN Uint32      argc,
                    IN Char8 **    argv)
{
    DSP_STATUS status = DSP_SOK ;

    status = PROC_Setup () ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed on PROC_Setup. Status = [0x%x]", status) ;
    }
    else {
        status = STS_OpenAllocator (TST_MQA_ID,
                                           numMsgPerPool,
                                           maxMsgSize) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed on STS_OpenAllocator. Status = [0x%x]",
                          status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        status = STS_OpenTransport (TST_LMQT_ID,
                                    TST_MQA_ID,
                                    maxMsgSize,
                                    numMsgqs) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed on STS_OpenTransport LMQT. Status = [0x%x]",
                          status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (procId, NULL) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed on PROC_Attach. Status = [0x%x]", status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Load (procId, imagePath, argc, argv) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed on PROC_Load. Status = [0x%x]", status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (procId) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed on PROC_Start. Status = [0x%x]", status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        status = STS_OpenTransport (TST_RMQT_ID,
                                    TST_MQA_ID,
                                    maxMsgSize,
                                    numMsgqs) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Failed on STS_OpenTransport RMQT. Status = [0x%x]",
                          status) ;
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   STS_CleanupMessaging
 *
 *  @desc   Close and cleanup the initializations done by STS_SetupMessaging
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_CleanupMessaging (IN ProcessorId  procId)
{
    DSP_STATUS status = DSP_SOK ;

    status = STS_CloseTransport (TST_RMQT_ID) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed on STS_CloseTransport. Status = [0x%x]", status) ;
    }

    status = PROC_Stop (procId) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed on PROC_Stop. Status = [0x%x]", status) ;
    }

    status = PROC_Detach (procId) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed on PROC_Detach. Status = [0x%x]", status) ;
    }

    status = STS_CloseTransport (TST_LMQT_ID) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed on STS_CloseTransport. Status = [0x%x]", status) ;
    }

    status = STS_CloseAllocator (TST_MQA_ID) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed on STS_CloseAllocator. Status = [0x%x]", status) ;
    }

    status = PROC_Destroy () ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Failed on PROC_Destroy. Status = [0x%x]", status) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   STS_CreateDspArgumentList
 *
 *  @desc   Create a list of arguments to be passed to the DSP from
 *          the list of Uints.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Char8 **
STS_CreateDspArgumentList (IN Uint16 numArgs, ...)
{
    Uint16   i       ;
    Uint16   arg     ;
    Char8 ** argList ;
    va_list  va      ;

    DBC_Require (numArgs > 0) ;

    va_start (va, numArgs) ;
    TST_Alloc ((Pvoid *) &(argList), numArgs * sizeof (Char8 *)) ;

    for (i = 0 ; i < numArgs ; i++) {
        arg = va_arg (va, int) ;
        TST_Alloc ((Pvoid *) &(argList [i]), TST_MAX_SRTLEN * sizeof (Char8)) ;
        TST_IntToString (arg, argList [i]) ;
    }

    va_end (va) ;

    return argList ;
}


/** ============================================================================
 *  @func   STS_FreeDspArgumentList
 *
 *  @desc   Delete the list created by STS_CreateDspArgumentList
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
STS_FreeDspArgumentList (IN Uint16 numArgs, IN Char8 ** argList)
{
    Uint16   i ;

    for (i = 0 ; i < numArgs ; i++) {
        TST_Free ((Pvoid *) &(argList [i])) ;
    }
    TST_Free ((Pvoid *) &argList) ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
