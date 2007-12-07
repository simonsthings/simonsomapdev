/** ============================================================================
 *  @file   ReceiveMsg.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\ReceiveMsg
 *
 *  @desc   Implemetation of ReceiveMsg. This test receives messages
 *          from the GPP and verifies the content of the messages. It also
 *          sends back a report to the GPP using channels indicating
 *          the status of the verification.
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
#include <msgq.h>
#include <platform.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <mqabuf.h>
#include <mqtbiosque.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Test Headers                */
#include <MsgqUtils.h>
#include <ReceiveMsg.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_LOCAL_MSGQS
 *
 *  @desc   Number of Msgqs on the DSP.
 *  ============================================================================
 */
#define NUM_LOCAL_MSGQS     16

/** ============================================================================
 *  @name   OUTPUT_DATA_CHANNEL
 *
 *  @desc   This is the path for control channel.
 *  ============================================================================
 */
#define OUTPUT_DATA_CHANNEL    "/dio_dsplink1"


/** ============================================================================
 *  @name   trace
 *
 *  @desc   Trace object used for writing debug strings and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;


/** ----------------------------------------------------------------------------
 *  @name   msgqIn
 *
 *  @desc   MSGQ handle used for accessing the queue.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Handle msgqIn ;

/** ----------------------------------------------------------------------------
 *  @name   inMsgqName
 *
 *  @desc   Space for the message queue name.
 *  ----------------------------------------------------------------------------
 */
static Char inMsgqName [MAX_MSGQ_NAME_LEN] ;

/** ----------------------------------------------------------------------------
 *  @name   semHandle
 *
 *  @desc   The semaphore handle used for notify on the msgqueue.
 *  ----------------------------------------------------------------------------
 */
static SEM_Handle semHandle ;

/** ----------------------------------------------------------------------------
 *  @name   reportOut
 *
 *  @desc   Output stream which is opened for sending report to GPP.
 *  ----------------------------------------------------------------------------
 */
static SIO_Handle reportOut = NULL ;

/** ----------------------------------------------------------------------------
 *  @name   reportBuf
 *
 *  @desc   Pointer to the buffer used for report transfer to GPP.
 *  ----------------------------------------------------------------------------
 */
static Uint16 * reportBuf = NULL ;

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
 *  @name   receiveMsgqId
 *
 *  @desc   Message queue on which to receive messages.
 *  ----------------------------------------------------------------------------
 */
static Uns receiveMsgqId ;


/** ----------------------------------------------------------------------------
 *  @func   ReceiveMsg_create
 *
 *  @desc   This function allocates the resources needed by the ReceiveMsg test.
 *
 *  @arg    None
 *
 *  @ret    SYS_EALLOC
 *              Memory allocation error.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReceiveMsg_execute, ReceiveMsg_delete
 *  ----------------------------------------------------------------------------
 */
static
Uns
ReceiveMsg_create () ;


/** ----------------------------------------------------------------------------
 *  @func   ReceiveMsg_execute
 *
 *  @desc   This function does the actual exchange with the GPP.
 *
 *  @arg    None
 *
 *  @ret    SYS_EINVAL
 *              Invalid arguments.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReceiveMsg_delete, ReceiveMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
ReceiveMsg_execute () ;


/** ----------------------------------------------------------------------------
 *  @func   ReceiveMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the ReceiveMsg_create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReceiveMsg_execute, ReceiveMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Void
ReceiveMsg_delete () ;


/** ============================================================================
 *  @func   ReceiveMsg
 *
 *  @desc   This function receives messages from the GPP and
 *          verifies the content of the messages. It also sends
 *          back a report to the GPP using channels indicating
 *          the status of the verification.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
ReceiveMsg ()
{
    Uns status = SYS_OK ;

    status = ReceiveMsg_create () ;

    if (status == SYS_OK) {
        status = ReceiveMsg_execute () ;
    }

    ReceiveMsg_delete () ;
}


/** ----------------------------------------------------------------------------
 *  @func   ReceiveMsg_create
 *
 *  @desc   This function allocates the resources needed by the ReceiveMsg test.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
ReceiveMsg_create ()
{
    MSGQ_Status status      = MSGQ_SUCCESS ;
    Uns         sysStatus   = SYS_OK       ;
    MSGQ_Attrs  msgqAttrs   = MSGQ_ATTRS   ;
    SEM_Attrs   semAttrs    = SEM_ATTRS    ;
    SIO_Attrs   sioAttrs    = SIO_ATTRS    ;

    sysStatus = SetupMessaging (numMsgPerPool,
                                maxMsgSize,
                                NUM_LOCAL_MSGQS,
                                numRemoteMsgqs) ;
    if (sysStatus == SYS_OK) {
        /*
         *  Create a message queue that uses the MQTDSPLINK transport interface
         *  and the MQABUF allocation interface.
         */
        semAttrs  = SEM_ATTRS ;
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
         *  Create a channel for sending the report to GPP
         */
        sioAttrs.nbufs   = 1                ;
        sioAttrs.segid   = 0                ;
        sioAttrs.align   = 0                ;
        sioAttrs.flush   = TRUE             ;
        sioAttrs.model   = SIO_ISSUERECLAIM ;
        sioAttrs.timeout = SYS_FOREVER      ;
        reportOut = SIO_create (OUTPUT_DATA_CHANNEL,
                                SIO_OUTPUT,
                                TST_REPORT_SIZE,
                                &sioAttrs) ;
        if (reportOut == NULL) {
            sysStatus = SYS_EALLOC ;
            status = MSGQ_EMEMORY ;
            LOG_printf (&trace,
                        "Could not create output stream. Status = 0x%x\n",
                        status) ;
        }
        else {
            /*
             *  Allocate buffer to be sent to GPP for the report.
             */
            reportBuf = MEM_calloc (DSPLINK_SEGID, TST_REPORT_SIZE, DSPLINK_ALIGN) ;
            if (reportBuf == NULL) {
                sysStatus = SYS_EALLOC ;
                status = MSGQ_EMEMORY ;
                LOG_printf (&trace,
                            "Could not allocate buffer for data transfer") ;
            }
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   ReceiveMsg_execute
 *
 *  @desc   This function does the actual exchange with the GPP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
ReceiveMsg_execute ()
{
    Uns       status  = SYS_OK ;
    Uns       msgSize = 0      ;
    Int       nMaus   = -1     ;
    Uint16    j       = 0      ;
    Uns       i       = 0      ;
    Uint16    recId   = 0      ;

    TestMsg * msg              ;
    Arg       arg              ;

    for (i = 0 ; ((i < numMsgToExchange) && (status == SYS_OK)) ; i++) {
        /*
         *  Wait forever for the message
         */
        msg = (TestMsg *) MSGQ_get (msgqIn, SYS_FOREVER) ;
        if (msg == NULL) {
            status = SYS_EALLOC ;
            LOG_printf (&trace,
                        "MSGQ_get (msgqIn ...) failed. Status = 0x%x\n",
                        MSGQ_EFAILURE) ;
        }
        else {
            /*
             *  Find the size of the data part of the message.
             *  For testing variable sized messages, we need to
             *  adjust the size.
             */
            msgSize  = TST_MSGQ_DATA_SIZE ;
            msgSize += (  (  (  MSGQ_getMsgSize ((MSGQ_Msg) msg)
                              - sizeof (TestMsg))
                           * DSP_MAUSIZE)
                        / 2) ;

            /*
             *  If Id is 1, perform validation and send report.
             *  Fill the report buffer 1 for success and 0 for fail.
             */
            recId = MSGQ_getMsgId ((MSGQ_Msg) msg) ;
            if (recId == 1) {
                *reportBuf = 1 ;
                for (j = 0 ; j < msgSize ; j++) {
                    if (msg->buffer[j] != j) {
                        LOG_printf (&trace, "Mismatch on the data") ;
                        status = SYS_EFAIL ;
                        *reportBuf = 0 ;
                    }
                }

                /*
                 *  Send the buffer to GPP
                 */
                status = SIO_issue (reportOut,
                                    reportBuf,
                                    TST_REPORT_SIZE,
                                    0) ;
                if (status != SYS_OK) {
                    LOG_printf (&trace,
                                "DSP/BIOS Link: Buffer issuing failed %d\n",
                                status) ;
                }
                else {
                    nMaus = SIO_reclaim (reportOut, (Ptr *) &reportBuf, &arg) ;
                    if (nMaus < 0) {
                        status = -nMaus ;
                        LOG_printf (&trace,
                                    "DSP/BIOS Link: Buffer reclaim failed %d\n",
                                    status) ;
                    }
                }
            }
        }
        /*
         *  Free the message
         */
        MSGQ_free ((MSGQ_Msg) msg) ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   ReceiveMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the ReceiveMsg_create function.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
ReceiveMsg_delete ()
{
    SIO_delete (reportOut) ;
    reportOut = NULL ;
    if (reportBuf != NULL) {
        MEM_free (DSPLINK_SEGID, reportBuf, TST_REPORT_SIZE) ;
    }
    reportBuf = NULL ;
    MSGQ_delete (msgqIn) ;
    msgqIn = NULL ;
    SEM_delete (semHandle) ;
    CleanupMessaging (NUM_LOCAL_MSGQS) ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the ReceiveMsg task.
 *
 *  @modif  None
 *  ============================================================================
 */
Void main (Int argc, Char * argv [])
{
    TSK_Handle    inTask ;
    volatile Uns  dummy  ; /* To remove compile warning */

    numMsgPerPool    = atoi (argv [0]) ;
    maxMsgSize       = atoi (argv [1]) ;
    numRemoteMsgqs   = atoi (argv [2]) ;
    receiveMsgqId    = atoi (argv [3]) ;
    dummy            = atoi (argv [4]) ;  /* Ignore this argument. */
    numMsgToExchange = atoi (argv [5]) ;
    inTask = TSK_create ((Fxn) ReceiveMsg, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
