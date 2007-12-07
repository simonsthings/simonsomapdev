/** ============================================================================
 *  @file   API_MsgqUtils.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api\Linux
 *
 *  @desc   Defines utility functions for API Msgq tests.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers           */
#include <stdarg.h>
#include <stdio.h>

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

/*  ----------------------------------- MSGQ Component                */
#include <msgqdefs.h>
#include <msgq.h>

/*  ----------------------------------- Test framework                */
#include <TST_Mem.h>
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <API_MsgqUtils.h>
#include <TST_PrintFuncs.h>


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
 *  @func   API_OpenAllocator
 *
 *  @desc   Opens an allocator.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_OpenAllocator (IN AllocatorId mqaId,
                   IN Uint16      numMsgPerPool,
                   IN Uint16      maxMsgSize)
{
    DSP_STATUS  status              = DSP_SOK             ;
    Uint16      msgSize [NUM_POOLS] = {RMQT_CTRLMSG_SIZE,
                                       ALIGN (sizeof (TestMsg))}   ;
    Uint16      numMsg  [NUM_POOLS]                       ;
    MqaBufAttrs mqaAttrs            = {NUM_POOLS,
                                       msgSize,
                                       numMsg}            ;

    numMsg [0]  = numMsgPerPool   ;
    numMsg [1]  = numMsgPerPool   ;
    numMsg [2]  = numMsgPerPool   ;
    msgSize [2] = maxMsgSize      ;

    status = MSGQ_AllocatorOpen (mqaId, (Pvoid) &mqaAttrs) ;

    if (DSP_FAILED (status)) {
        TST_PrnError ("MSGQ_AllocatorOpen failed. Status = [0x%x]", status) ;
    }
    return status ;
}


/** ============================================================================
 *  @func   API_CloseAllocator
 *
 *  @desc   Closes an allocator.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_CloseAllocator (IN AllocatorId mqaId)
{
    DSP_STATUS status = DSP_SOK ;

    status = MSGQ_AllocatorClose (mqaId) ;

    if (DSP_FAILED (status)) {
        TST_PrnError ("MSGQ_AllocatorClose failed. Status = [0x%x]", status) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   API_OpenTransport
 *
 *  @desc   Opens transport.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_OpenTransport (IN TransportId mqtId,
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
        rmqtAttrs.defaultMqaId = mqaId         ;
        status = MSGQ_TransportOpen (mqtId, (Pvoid) &rmqtAttrs) ;
    }

    if (DSP_FAILED (status)) {
        TST_PrnError ("MSGQ_TransportOpen failed. Status = [0x%x]", status) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   API_CloseTransport
 *
 *  @desc   Closes transport.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_CloseTransport (IN TransportId mqtId)
{
    DSP_STATUS status = DSP_SOK ;

    status = MSGQ_TransportClose (mqtId) ;

    if (DSP_FAILED (status)) {
        TST_PrnError ("MSGQ_TransportClose failed. Status = [0x%x]", status) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   API_CreateDspArgumentList
 *
 *  @desc   Create a list of arguments to be passed to the DSP from
 *          the list of Uints.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Char8 **
API_CreateDspArgumentList (IN Uint16 numArgs, ...)
{
    Uint16   i       ;
    Uint16   arg     ;
    Char8 ** argList ;
    va_list  va      ;

    DBC_Require (numArgs > 0) ;

    va_start (va, numArgs) ;
    TST_Alloc ((Void **) &argList, numArgs * sizeof (Char8 *)) ;

    for (i = 0 ; i < numArgs ; i++) {
        arg = va_arg (va, int) ;
        TST_Alloc ((Void **) &(argList [i]), TST_MAX_SRTLEN * sizeof (Char8)) ;
        TST_IntToString (arg, argList [i]) ;
    }

    va_end (va) ;

    return argList ;
}


/** ============================================================================
 *  @func   API_FreeDspArgumentList
 *
 *  @desc   Delete the list created by API_CreateDspArgumentList
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
API_FreeDspArgumentList (IN Uint16 numArgs, IN Char8 ** argList)
{
    Uint16   i ;

    for (i = 0 ; i < numArgs ; i++) {
        TST_Free ((Void **) &(argList [i])) ;
    }
    TST_Free ((Void **) &argList) ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
