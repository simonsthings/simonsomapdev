/** ============================================================================
 *  @file   EchoMsg.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\EchoMsg
 *
 *  @desc   Implemetation of EchoMsg. This test waits for a message
 *          from the GPP and sends it back to the GPP after a delay. The
 *          delay is determined from the contents of the message.
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
#include <sio.h>
#include <tsk.h>
#include <log.h>
#include <sem.h>
#include <string.h>
#include <clk.h>
#include <msgq.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <mqabuf.h>
#include <mqtbiosque.h>
#include <mqtdsplink.h>
#include <platform.h>

/*  ----------------------------------- Test Headers                */
#include <MsgqUtils.h>
#include <EchoMsg.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_LOCAL_MSGQS
 *
 *  @desc   Number of MSGQs on the DSP.
 *  ============================================================================
 */
#define NUM_LOCAL_MSGQS     1


/** ============================================================================
 *  @name   trace
 *
 *  @desc   This is the trace object used for writing debug strings
 *          and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;

/** ----------------------------------------------------------------------------
 *  @name   msgqIn/msgqOut
 *
 *  @desc   This is the MSGQ handle to be used to access the queue.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Handle msgqIn  ;
static MSGQ_Handle msgqOut ;

/** ----------------------------------------------------------------------------
 *  @name   inMsgqName/outMsgqName
 *
 *  @desc   Space for the message queue names.
 *  ----------------------------------------------------------------------------
 */
static Char inMsgqName  [MAX_MSGQ_NAME_LEN] ;
static Char outMsgqName [MAX_MSGQ_NAME_LEN] ;

/** ----------------------------------------------------------------------------
 *  @name   semHandle
 *
 *  @desc   The semaphore handle used for notify on the msgqueue.
 *  ----------------------------------------------------------------------------
 */
static SEM_Handle semHandle ;

/** ----------------------------------------------------------------------------
 *  @name   numMsgToExchange
 *
 *  @desc   The number of messages to be received.
 *  ----------------------------------------------------------------------------
 */
static Uns numMsgToExchange = 1 ;

/** ----------------------------------------------------------------------------
 *  @name   numMsgPerPool/maxMsgSize
 *
 *  @desc   Attributes for the allocator.
 *          Number of messages per pool and the maximum message size.
 *  ----------------------------------------------------------------------------
 */
static Uns numMsgPerPool ;
static Uns maxMsgSize    ;

/** ----------------------------------------------------------------------------
 *  @name   numRemoteMsgqs
 *
 *  @desc   Maximum number of Msgqs on the GPP.
 *  ----------------------------------------------------------------------------
 */
static Uns numRemoteMsgqs    ;

/** ----------------------------------------------------------------------------
 *  @name   receiveMsgqId/sendMsgqId
 *
 *  @desc   Message queues on which to receive and send messages.
 *  ----------------------------------------------------------------------------
 */
static Uns receiveMsgqId ;
static Uns sendMsgqId    ;


/** ----------------------------------------------------------------------------
 *  @func   EchoMsg_create
 *
 *  @desc   This function allocates the resources needed by the EchoMsg test.
 *
 *  @arg    None
 *
 *  @ret    Status
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    EchoMsg_execute, EchoMsg_delete
 *  ----------------------------------------------------------------------------
 */
static
Uns
EchoMsg_create () ;


/** ----------------------------------------------------------------------------
 *  @func   EchoMsg_execute
 *
 *  @desc   This function does the actual exchange with the GPP.
 *
 *  @arg    None
 *
 *  @ret    Status
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    EchoMsg_delete, EchoMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
EchoMsg_execute () ;


/** ----------------------------------------------------------------------------
 *  @func   EchoMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the EchoMsg_create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    EchoMsg_execute, EchoMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Void
EchoMsg_delete () ;


/** ============================================================================
 *  @func   EchoMsg
 *
 *  @desc   This test waits for a message from the GPP and sends it
 *          back to the GPP after a delay. The delay is determined from
 *          the contents of the message.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
EchoMsg ()
{
    Uns status = SYS_OK ;

    status = EchoMsg_create () ;

    if (status == SYS_OK) {
        status = EchoMsg_execute () ;
    }

    EchoMsg_delete () ;
}


/** ----------------------------------------------------------------------------
 *  @func   EchoMsg_create
 *
 *  @desc   This function allocates the resources needed by the EchoMsg test.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
EchoMsg_create ()
{
    MSGQ_Status status    = MSGQ_SUCCESS ;
    Uns         sysStatus = SYS_OK       ;
    MSGQ_Attrs  msgqAttrs = MSGQ_ATTRS   ;
    SEM_Attrs   semAttrs  = SEM_ATTRS    ;

    sysStatus = SetupMessaging (numMsgPerPool,
                                maxMsgSize,
                                NUM_LOCAL_MSGQS,
                                numRemoteMsgqs) ;
    if (sysStatus == SYS_OK) {
        /*
         *  Create a message queue that uses the MQTDSPLINK transport interface
         *  and the MQABUF allocation interface.
         */
        semHandle = SEM_create (0, &semAttrs) ;

        if (semHandle != NULL) {
            /* Fill in the attributes for this message queue. */
            msgqAttrs.notifyHandle = semHandle ;
            msgqAttrs.post         = (MSGQ_Post) SEM_post ;
            msgqAttrs.pend         = (MSGQ_Pend) BinarySemPend ;

            GetDsplinkMsgqName (inMsgqName,
                                DSPLINK_DSPMSGQ_NAME,
                                receiveMsgqId) ;

            msgqIn = MSGQ_create (inMsgqName,   /* Name of the msg queue */
                                  &msgqAttrs) ; /* Attributes */
            if (msgqIn == NULL) {
                status = MSGQ_EMEMORY ;
                sysStatus = SYS_EALLOC ;
                LOG_printf (&trace,
                            "MSGQ_create failed. Status = 0x%x\n",
                            status) ;
            }
        }
        else {
            sysStatus = SYS_EALLOC ;
            status = MSGQ_EMEMORY ;
            LOG_printf (&trace, "SEM_create failed. Status = 0x%x\n", status) ;
        }
    }
    else {
        status = MSGQ_EFAILURE ;
        LOG_printf (&trace,
                    "Setup for messaging failed. Status = 0x%x\n",
                    status) ;
    }

    if (status == MSGQ_SUCCESS) {
        /*
         *  Synchronously locate the queue to send the messages.
         */
        GetDsplinkMsgqName (outMsgqName, DSPLINK_GPPMSGQ_NAME, sendMsgqId) ;
        status = MSGQ_ENOTFOUND ;
        while (status == MSGQ_ENOTFOUND) {
            status = MSGQ_locate (outMsgqName, &msgqOut, NULL) ;

            if (status == MSGQ_ENOTFOUND) {
                TSK_sleep (1000) ;
            }
            else if (status != MSGQ_SUCCESS) {
                sysStatus = SYS_EFAIL ;
                LOG_printf (&trace,
                            "MSGQ_locate (msgqOut) failed. Status = 0x%x\n",
                            status) ;
            }
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   EchoMsg_execute
 *
 *  @desc   This function does the actual exchange with the GPP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
EchoMsg_execute ()
{
    Uns         sysStatus = SYS_OK       ;
    MSGQ_Status status    = MSGQ_SUCCESS ;
    Uns         i         = 0            ;
    Uns         delay     = 0            ;
    LgUns       starttime = 0            ;
    LgUns       endtime   = 0            ;
    LgUns       ncounts   = 0            ;

    Uns         ticksForSleeping         ;
    TestMsg *   msg                      ;

    /* Send a message to the GPP to indicate that the DSP is ready */
    msg = (TestMsg *) MSGQ_alloc (TST_MQABUF_ID, ALIGN (sizeof (TestMsg))) ;
    if (msg != NULL) {
        status = MSGQ_put (msgqOut, (MSGQ_Msg) msg, 0, NULL) ;
        if (status != MSGQ_SUCCESS) {
            LOG_printf (&trace,
                        "MSGQ_put failed. Status = 0x%x\n",
                        status) ;
        }
    }
    else {
        status = MSGQ_EMEMORY ;
        LOG_printf (&trace,
                    "MSGQ_alloc failed. Status = 0x%x\n",
                    status) ;
    }

    for (i = 0 ; ((i < numMsgToExchange) && (status == MSGQ_SUCCESS)) ; i++) {
        /*
         *  Get a message from the GPP.
         */
        starttime = CLK_gethtime();
        msg = (TestMsg *) MSGQ_get (msgqIn, SYS_FOREVER) ;
        if (msg == NULL) {
            sysStatus = SYS_EALLOC ;
            status = MSGQ_EFAILURE ;
            LOG_printf (&trace,
                        "MSGQ_get (msgqIn ...) failed. Status = 0x%x\n",
                        status) ;
        }
        else {
            /*
             *  Get the delay value from the message and sleep.
             */
            delay = msg->info ;
            if (delay !=0) {
                ticksForSleeping = ((CLK_countspms () / (CLK_getprd ()))
                                    * delay) ;
                TSK_sleep (ticksForSleeping) ;
            }

            endtime   = CLK_gethtime();
            ncounts   = CLK_countspms();
            msg->info = ((endtime - starttime) * 1000) / ncounts ; /* in usec */

            /*
             *  Send the message to GPP
             */
            status = MSGQ_put (msgqOut, (MSGQ_Msg) msg, 0, msgqIn) ;
            if (status != MSGQ_SUCCESS) {
                sysStatus = SYS_EFAIL ;
                LOG_printf (&trace,
                            "MSGQ_put (msgqOut ...) failed. Status = 0x%x\n",
                            status) ;
            }
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   EchoMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the EchoMsg_create function.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
EchoMsg_delete ()
{
    MSGQ_release (msgqOut) ;
    MSGQ_delete (msgqIn) ;
    msgqIn = msgqOut = NULL ;
    SEM_delete (semHandle) ;
    CleanupMessaging (NUM_LOCAL_MSGQS) ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the EchoMsg task.
 *
 *  @modif  None
 *  ============================================================================
 */
Void main (Int argc, Char * argv [])
{
    TSK_Handle inTask ;

    numMsgPerPool    = atoi (argv [0]) ;
    maxMsgSize       = atoi (argv [1]) ;
    numRemoteMsgqs   = atoi (argv [2]) ;
    receiveMsgqId    = atoi (argv [3]) ;
    sendMsgqId       = atoi (argv [4]) ;
    numMsgToExchange = atoi (argv [5]) ;
    inTask = TSK_create ((Fxn) EchoMsg, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
