/** ============================================================================
 *  @file   MsgqUtils.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\generic
 *
 *  @desc   Defines some utility functions for messaging testsuite.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  This software is copyright to Texas Instruments Incorporated.
 *  Its usage is governed by the terms and conditions stipulated in
 *  the license agreement under which this software has been supplied.
 *
 *  The recipient of this software implicitly accepts the terms of
 *  the license.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <sys.h>
#include <log.h>
#include <mem.h>
#include <sem.h>
#include <string.h>
#include <msgq.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <mqabuf.h>
#include <mqtbiosque.h>
#include <mqtdsplink.h>
#include <platform.h>

/*  ----------------------------------- Test Headers                */
#include <MsgqUtils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_ALLOCATORS
 *
 *  @desc   Number of allocators.
 *  ============================================================================
 */
#define NUM_ALLOCATORS 1

/** ============================================================================
 *  @name   NUM_TRANSPORTS
 *
 *  @desc   Number of transports.
 *  ============================================================================
 */
#define NUM_TRANSPORTS 2

/** ============================================================================
 *  @name   NUM_POOLS
 *
 *  @desc   Number of pools to be configured for the allocator.
 *  ============================================================================
 */
#define NUM_POOLS 3

/** ============================================================================
 *  @name   MAX_LOCAL_MSGQS
 *
 *  @desc   Maximum number of MSGQs created on the DSP.
 *  ============================================================================
 */
#define MAX_LOCAL_MSGQS 3

/** ----------------------------------------------------------------------------
 *  @name   msgQueues
 *
 *  @desc   Pointer to array of message queue objects.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Obj * msgQueues ;

/** ============================================================================
 *  @name   msgSize
 *
 *  @desc   Message sizes managed by the allocator.
 *  ============================================================================
 */
static Uint16 msgSize [NUM_POOLS] = {MQTDSPLINK_CTRLMSG_SIZE,
                                     ALIGN (sizeof (TestMsg))} ;

/** ============================================================================
 *  @name   numMsg
 *
 *  @desc   Number of messages in each pool managed by the allocator.
 *  ============================================================================
 */
static Uint16 numMsg [NUM_POOLS] ;

/** ============================================================================
 *  @name   mqaBufParams
 *
 *  @desc   Configuration parameters for the BUF-based allocator instance.
 *  ============================================================================
 */
static MQABUF_Params mqaBufParams = {NUM_POOLS,    /* Number of pools     */
                                     msgSize,      /* Array of sizes      */
                                     numMsg,       /* Array of msg counts */
                                     0};           /* SegId               */

/** ============================================================================
 *  @name   allocators
 *
 *  @desc   Array of allocators.
 *  ============================================================================
 */
static MSGQ_AllocatorObj allocators [NUM_ALLOCATORS] =
{
    {"MQABUF0",                            /* Name of the allocator         */
     (MSGQ_AllocatorFxns *) &MQABUF_FXNS,  /* Allocator interface functions */
     &mqaBufParams,                        /* Allocator configuration       */
     NULL,                                 /* Filled in by allocator        */
     TST_MQABUF_ID}                        /* Allocator Id                  */
} ;

/** ============================================================================
 *  @name   mqtdsplinkParams
 *
 *  @desc   Configuration parameters for the DSPLINK transport instance.
 *  ============================================================================
 */
static MQTDSPLINK_Params mqtdsplinkParams ;

/** ============================================================================
 *  @name   transports
 *
 *  @desc   Array of transports.
 *  ============================================================================
 */
static MSGQ_TransportObj transports [NUM_TRANSPORTS] =
{
    {"MQTLOCAL",                             /* Name of the transport         */
     (MSGQ_TransportFxns *) &MQTBIOSQUE_FXNS,/* Transport interface functions */
     NULL,                                   /* Transport configuration       */
     NULL,                                   /* Filled in by transport        */
     TST_MQTLOCAL_ID},                       /* Transport Id                  */

     {MQTDSPLINK_NAME,                       /* Name of the transport         */
     (MSGQ_TransportFxns *) &MQTDSPLINK_FXNS,/* Transport interface functions */
     &mqtdsplinkParams,                      /* Transport configuration       */
     NULL,                                   /* Filled in by transport        */
     TST_MQTDSPLINK_ID}                      /* Transport Id                  */
} ;

/** ============================================================================
 *  @name   msgqConfig
 *
 *  @desc   MSGQ configuration information.
 *          Contents of part of this structure is set by the application before
 *          calling MSGQ_init ().
 *  ============================================================================
 */
static MSGQ_Config msgqConfig = {NULL,                  /* Set by application */
                                 allocators,
                                 transports,
                                 0,                     /* Set by application */
                                 NUM_ALLOCATORS,
                                 NUM_TRANSPORTS,
                                 TST_MQTLOCAL_ID} ;

/** ============================================================================
 *  @name   MSGQ
 *
 *  @desc   The static configuration of the MSGQ module.  MSGQ is a required
 *          global variable.
 *  ============================================================================
 */
MSGQ_Config * MSGQ = &msgqConfig ;


/** ============================================================================
 *  @name   trace
 *
 *  @desc   This is the trace object used for writing debug strings
 *          and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;


/** ============================================================================
 *  @func   SetupMessaging
 *
 *  @desc   This function does the basic setup for using messaging on the DSP
 *          side.
 *
 *  @modif  None
 *  ============================================================================
 */
Uns
SetupMessaging (Uns numMsgPerPool,
                Uns maxMsgSize,
                Uns numLocalMsgqs,
                Uns numRemoteMsgqs)
{
    Uns        sysStatus = SYS_OK ;

    maxMsgSize /= DSP_MAUSIZE ;
    /*
     *  Set configuration parameters for the BUF-based MQA.
     */
    numMsg [0]  = numMsgPerPool    ;
    numMsg [1]  = numMsgPerPool    ;
    numMsg [2]  = numMsgPerPool    ;
    msgSize [2] = maxMsgSize       ;

    /*
     *  Set configuration parameters for the MSGQ component.
     */
    if (numLocalMsgqs > 0) {
        msgQueues = (MSGQ_Obj *) MEM_calloc (DSPLINK_SEGID,
                                             sizeof (MSGQ_Obj) * numLocalMsgqs,
                                             DSPLINK_ALIGN) ;
    }
    else {
        numLocalMsgqs = 0 ;
    }

    /*
     *  Set configuration parameters for the remote MQT.
     */
    if (maxMsgSize < MQTDSPLINK_CTRLMSG_SIZE) {
        maxMsgSize = MQTDSPLINK_CTRLMSG_SIZE ;
    }

    if (maxMsgSize < ALIGN (sizeof (TestMsg))) {
        maxMsgSize = ALIGN (sizeof (TestMsg)) ;
    }

    mqtdsplinkParams.maxMsgSize   = maxMsgSize ;
    mqtdsplinkParams.maxNumMsgq   = numRemoteMsgqs ;
    mqtdsplinkParams.defaultMqaId = TST_MQABUF_ID ;

    msgqConfig.numMsgQueues = numLocalMsgqs ;
    msgqConfig.msgQueues    = msgQueues ;

    /*
     *  Initialize the MSGQ component.
     */
    MSGQ_init () ;

    /*
     *  Initialize the allocators and transports.
     */
    MQABUF_init();
    MQTBIOSQUE_init();
    MQTDSPLINK_init();

    return sysStatus ;
}


/** ============================================================================
 *  @func   CleanupMessaging
 *
 *  @desc   This function does the cleanup of the initializations done during
 *          setup.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
CleanupMessaging (Uns numLocalMsgqs)
{
    MQTDSPLINK_exit () ;
    MQTBIOSQUE_exit () ;
    MQABUF_exit () ;
    MSGQ_exit () ;
    MEM_free (DSPLINK_SEGID, msgQueues, sizeof (MSGQ_Obj) * numLocalMsgqs) ;
}


/** ============================================================================
 *  @func   GetDsplinkMsgqName
 *
 *  @desc   This function converts id into a string and concatenates it with
 *          the supplied prefix to form the Msgq name.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
GetDsplinkMsgqName(Char * name, Char * prefix, Uns msgq_id)
{
    Char subfix [3] = {'0','0','\0'} ;

    subfix [1] = (msgq_id % 10) + '0' ;
    subfix [0] = (msgq_id / 10) + '0' ;

    strcpy (name, prefix) ;
    strcat (name, subfix) ;
}


/** ============================================================================
 *  @func   BinarySemPend
 *
 *  @desc   MSGQ supports only one reader of a message queue. MSGQ requires that
 *          the notify be binary. This pend function simulates a binary
 *          semaphore in DSP/BIOS. Note: this works since there is only one
 *          reader of a message queue.
 *
 *  @modif  None.
 *  ============================================================================
 */
Bool
BinarySemPend (SEM_Handle semHandle, Uns timeout)
{
    Bool retVal ;

    retVal = SEM_pend (semHandle, timeout) ;
    SEM_reset (semHandle, 0) ;
    return retVal ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
