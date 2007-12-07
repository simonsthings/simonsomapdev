/** ============================================================================
 *  @file   SupplierMsg.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\SupplierMsg
 *
 *  @desc   Implemetation of SupplierMsg. This test waits for a message
 *          from the GPP and interprets the info field of the TestMsg as the
 *          number of buffers to send to GPP. These buffers are sent to the
 *          GPP on channel 1. Subsequently a message is sent to the GPP
 *          denoting the end of buffer queuing. The test passes when the
 *          message reaches the GPP before the requested number of buffers.
 *          The converse does not denote a failure.
 *          This test tests the priority of messages over channels.
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

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <mqabuf.h>
#include <mqtbiosque.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Test Headers                */
#include <MsgqUtils.h>
#include <SupplierMsg.h>
#include <platform.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   NUM_LOCAL_MSGQS
 *
 *  @desc   Number of Msgqs on the DSP.
 *  ============================================================================
 */
#define NUM_LOCAL_MSGQS     1

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
 *  @name   supplyOut
 *
 *  @desc   This is the output stream which is opened for sending packets
 *          to GPP.
 *  ----------------------------------------------------------------------------
 */
static SIO_Handle supplyOut = NULL ;

/** ----------------------------------------------------------------------------
 *  @name   supplyBuf
 *
 *  @desc   This is the pointer to the buffer used for transfer to
 *          GPP.
 *  ----------------------------------------------------------------------------
 */
static MdUns * supplyBuf = NULL ;

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
 *  @name   maxNumBufs
 *
 *  @desc   Maximum number of buffers to be issued without reclaim.
 *  ----------------------------------------------------------------------------
 */
static Uns maxNumBufs ;


/** ----------------------------------------------------------------------------
 *  @func   SupplierMsg_create
 *
 *  @desc   This function allocates the resources needed
 *          by the SupplierMsg test.
 *
 *  @arg    None
 *
 *  @ret    Status
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SupplierMsg_execute, SupplierMsg_delete
 *  ----------------------------------------------------------------------------
 */
static
Uns
SupplierMsg_create () ;


/** ----------------------------------------------------------------------------
 *  @func   SupplierMsg_execute
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
 *  @see    SupplierMsg_delete, SupplierMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
SupplierMsg_execute () ;


/** ----------------------------------------------------------------------------
 *  @func   SupplierMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the SupplierMsg_create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SupplierMsg_execute, SupplierMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Void
SupplierMsg_delete () ;


/** ============================================================================
 *  @func   SupplierMsg
 *
 *  @desc   This test waits for a message from the GPP and interprets
 *          the info field of the TestMsg as the number of buffers
 *          to send to GPP. These buffers are sent to the GPP on
 *          channel 1. Subsequently a message is sent to the GPP
 *          denoting the end of buffer queuing. The test passes when the
 *          message reaches the GPP before the requested number of buffers.
 *          The converse does not denote a failure. A request for zero
 *          buffers indicates shutdown.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
SupplierMsg ()
{
    Uns status = SYS_OK ;

    status = SupplierMsg_create () ;

    if (status == SYS_OK) {
        status = SupplierMsg_execute () ;
    }

    SupplierMsg_delete () ;
}


/** ----------------------------------------------------------------------------
 *  @func   SupplierMsg_create
 *
 *  @desc   This function allocates the resources needed by the SupplierMsg
 *          test.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
SupplierMsg_create ()
{
    MSGQ_Status status    = MSGQ_SUCCESS ;
    Uns         sysStatus = SYS_OK       ;
    MSGQ_Attrs  msgqAttrs = MSGQ_ATTRS   ;
    SEM_Attrs   semAttrs  = SEM_ATTRS    ;
    SIO_Attrs   sioAttrs  = SIO_ATTRS    ;

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
         *  Create a channel for sending the report to GPP
         */
        sioAttrs.nbufs   = maxNumBufs       ;
        sioAttrs.segid   = 0                ;
        sioAttrs.align   = 0                ;
        sioAttrs.flush   = TRUE             ;
        sioAttrs.model   = SIO_ISSUERECLAIM ;
        sioAttrs.timeout = SYS_FOREVER      ;
        supplyOut = SIO_create (OUTPUT_DATA_CHANNEL,
                                SIO_OUTPUT,
                                TST_REPORT_SIZE,
                                &sioAttrs) ;
        if (supplyOut == NULL) {
            sysStatus = SYS_EALLOC ;
            status = MSGQ_EMEMORY ;
            LOG_printf (&trace,
                        "Could not create output stream. Status = 0x%x\n",
                        status) ;
        }
    }

    if (status == MSGQ_SUCCESS) {
        status = MSGQ_ENOTFOUND ;
        GetDsplinkMsgqName (outMsgqName, DSPLINK_GPPMSGQ_NAME, sendMsgqId) ;
        /*
         * Synchronous locate
         */
        while (status == MSGQ_ENOTFOUND) {
            status = MSGQ_locate (outMsgqName, &msgqOut, NULL) ;
            if ((status != MSGQ_ENOTFOUND) && (status != MSGQ_SUCCESS)) {
                sysStatus = SYS_EFAIL ;
                LOG_printf (&trace,
                            "MSGQ_locate (msgqOut) failed. Status = 0x%x\n",
                            status) ;
            }
            else if (status == MSGQ_ENOTFOUND) {
                TSK_sleep (100) ;
            }
        }
    }


    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   SupplierMsg_execute
 *
 *  @desc   This function does the actual exchange with the GPP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
SupplierMsg_execute ()
{
    Uns         sysStatus = SYS_OK       ;
    MSGQ_Status status    = MSGQ_SUCCESS ;
    Int         nMaus     = -1           ;
    Uint16      numPkt    = 1            ;
    Uns         i         = 0            ;

    TestMsg *   msg                      ;
    Arg         arg                      ;

    while ((numPkt > 0) && (status == MSGQ_SUCCESS)) {
        /*
         *  Get a message from the GPP.
         */
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
             *  Get the number of data buffers to send.
             */
            numPkt = msg->info ;

            /*
             *  Allocate and send the data buffers.
             */
            for (i = 0 ; ((i < numPkt) && (status == MSGQ_SUCCESS)) ; i++) {
                supplyBuf = MEM_valloc (DSPLINK_SEGID,
                                        TST_REPORT_SIZE,
                                        DSPLINK_ALIGN,
                                        (char) i) ;
                if (supplyBuf == NULL) {
                    sysStatus = SYS_EALLOC ;
                    status = MSGQ_EMEMORY ;
                    LOG_printf (&trace,
                                "Could not allocate buffer. Status = 0x%x\n",
                                status) ;
                }

                if (status == MSGQ_SUCCESS) {
                    sysStatus = SIO_issue (supplyOut,
                                           supplyBuf,
                                           TST_REPORT_SIZE,
                                           0) ;
                    if ((status == MSGQ_SUCCESS) && (sysStatus != SYS_OK)) {
                        status = MSGQ_EFAILURE ;
                        LOG_printf (&trace,
                                    "Could not send buffer. Status = 0x%x\n",
                                    status) ;
                    }
                }
            }

            /*
             *  Send the message
             */
            if ((status == MSGQ_SUCCESS) && (numPkt > 0)) {
                status = MSGQ_put (msgqOut, (MSGQ_Msg) msg, 0, msgqIn) ;
                if ((status != MSGQ_SUCCESS) && (sysStatus == SYS_OK)) {
                    sysStatus = SYS_EFAIL ;
                    LOG_printf (&trace,
                                "Could not send buffer. Status = 0x%x\n",
                                status) ;
                }
            }

            /*
             *  Reclaim and free the issued packets.
             */
            for (i = 0 ; ((i < numPkt) && (status == MSGQ_SUCCESS)) ; i++) {
                nMaus = SIO_reclaim (supplyOut, (Ptr *) &supplyBuf, &arg) ;
                if (nMaus < 0) {
                    sysStatus = -nMaus ;
                    status = MSGQ_EFAILURE ;
                    LOG_printf (&trace,
                                "DSP/BIOS Link: Buffer reclaim failed %d\n",
                                status) ;
                }
                if (supplyBuf != NULL) {
                    MEM_free (DSPLINK_SEGID, supplyBuf, TST_REPORT_SIZE) ;
                    supplyBuf = NULL ;
                }
            }
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   SupplierMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the SupplierMsg_create function.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
SupplierMsg_delete ()
{
    SIO_delete (supplyOut) ;
    supplyOut = NULL ;
    MSGQ_release (msgqOut) ;
    MSGQ_delete (msgqIn) ;
    msgqIn = msgqOut = NULL ;
    SEM_delete (semHandle) ;
    CleanupMessaging (NUM_LOCAL_MSGQS) ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the SupplierMsg task.
 *
 *  @modif  None
 *  ============================================================================
 */
Void main (Int argc, Char * argv [])
{
    TSK_Handle inTask ;

    numMsgPerPool  = atoi (argv [0]) ;
    maxMsgSize     = atoi (argv [1]) ;
    numRemoteMsgqs = atoi (argv [2]) ;
    receiveMsgqId  = atoi (argv [3]) ;
    sendMsgqId     = atoi (argv [4]) ;
    maxNumBufs     = atoi (argv [5]) ;
    inTask = TSK_create ((Fxn) SupplierMsg, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
