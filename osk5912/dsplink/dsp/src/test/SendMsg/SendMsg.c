/** ============================================================================
 *  @file   SendMsg.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\SendMsg
 *
 *  @desc   Implemetation of SendMsg. This test continuously sends
 *          messages to GPP with a predefined data in the message.
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
#include <platform.h>

/*  ----------------------------------- Test Headers                */
#include <MsgqUtils.h>
#include <SendMsg.h>


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
 *  @desc   Message MSGQ handle used for accessing the queue.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Handle   msgqIn = NULL ;
static MSGQ_Handle * msgqOut ;

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
static SEM_Handle semHandle = NULL ;

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
 *  @name   maxRemoteMsgqs/numRemoteMsgqs
 *
 *  @desc   Maximum/actual number of Msgqs on the GPP.
 *  ----------------------------------------------------------------------------
 */
static Uns maxRemoteMsgqs    ;
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
 *  @func   SendMsg_create
 *
 *  @desc   This function allocates the resources needed by the SendMsg test.
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
 *  @see    SendMsg_execute, SendMsg_delete
 *  ----------------------------------------------------------------------------
 */
static
Uns
SendMsg_create () ;


/** ----------------------------------------------------------------------------
 *  @func   SendMsg_execute
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
 *  @see    SendMsg_delete, SendMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
SendMsg_execute () ;


/** ----------------------------------------------------------------------------
 *  @func   SendMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the SendMsg_create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SendMsg_execute, SendMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Void
SendMsg_delete () ;


/** ============================================================================
 *  @func   SendMsg
 *
 *  @desc   This function communicates with gpp side application and
 *          continuously sends messages to GPP side.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
SendMsg ()
{
    Uns status = SYS_OK ;

    status = SendMsg_create () ;

    if (status == SYS_OK) {
        status = SendMsg_execute () ;
    }

    SendMsg_delete () ;
}


/** ----------------------------------------------------------------------------
 *  @func   SendMsg_create
 *
 *  @desc   This function allocates the resources needed by the SendMsg test.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
SendMsg_create ()
{
    MSGQ_Status      status           = MSGQ_SUCCESS     ;
    Uns              sysStatus        = SYS_OK           ;
    MSGQ_Attrs       msgqAttrs        = MSGQ_ATTRS       ;
    SEM_Attrs        semAttrs         = SEM_ATTRS        ;
    MSGQ_LocateAttrs locateAttrs      = MSGQ_LOCATEATTRS ;
    Int              i                                   ;

    sysStatus = SetupMessaging (numMsgPerPool,
                                maxMsgSize,
                                NUM_LOCAL_MSGQS,
                                maxRemoteMsgqs) ;
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
        msgqOut = (MSGQ_Handle *) MEM_calloc (
                                          DSPLINK_SEGID,
                                          sizeof (MSGQ_Handle) * numRemoteMsgqs,
                                          DSPLINK_ALIGN) ;
        if (msgqOut == NULL) {
            sysStatus = SYS_EALLOC ;
            status = MSGQ_EMEMORY ;
            LOG_printf (&trace, "MEM_calloc failed. Status = 0x%x\n", status) ;
        }
    }

    /*
     *  Locate the queues to send the messages.
     */
    for (i = 0 ; (i < numRemoteMsgqs) && (status == MSGQ_SUCCESS) ; i++) {
        GetDsplinkMsgqName (outMsgqName,
                            DSPLINK_GPPMSGQ_NAME,
                            sendMsgqId + i) ;
        /*
         * Synchronous locate
         */
        locateAttrs.mqaId   = TST_MQABUF_ID ;
        status = MSGQ_ENOTFOUND ;
        while (status == MSGQ_ENOTFOUND) {
            status = MSGQ_locate (outMsgqName, &(msgqOut [i]), &locateAttrs) ;
            if (status == MSGQ_ENOTFOUND) {
                TSK_sleep (1000) ;
            }
            else if (status != MSGQ_SUCCESS) {
                sysStatus = SYS_EFAIL ;
                LOG_printf (&trace, "MSGQ_locate (msgqOut [%d]) failed. "
                            "Status = 0x%x\n", i, status) ;
            }
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   SendMsg_execute
 *
 *  @desc   This function does the actual exchange with the GPP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
SendMsg_execute ()
{
    Uns         sysStatus = SYS_OK       ;
    MSGQ_Status status    = MSGQ_SUCCESS ;
    Uns         i         = 0            ;
    Uns         j         = 0            ;
    Uint16      k         = 0            ;
    TestMsg *   msg       = NULL         ;

    for (i = 0 ; ((i < numMsgToExchange) && (sysStatus == SYS_OK)) ; i++) {
        for (j = 0 ; ((j < numRemoteMsgqs) && (sysStatus == SYS_OK)) ; j++) {
            /*
             *  Allocate a buffer from the allocator
             */
            msg = (TestMsg *) MSGQ_alloc (TST_MQABUF_ID,
                                          ALIGN (sizeof (TestMsg))) ;
            if (msg == NULL) {
                sysStatus = SYS_EALLOC ;
                status = MSGQ_EMEMORY ;
                LOG_printf (&trace,
                            "MSGQ_alloc failed. Status = 0x%x\n",
                            status) ;
            }
            else {
                /*
                 *  Fill the msg buffer and send it
                 */
                for (k = 0 ; k < TST_MSGQ_DATA_SIZE ; k++) {
                    msg->buffer[k] = k ;
                }
                status = MSGQ_put (msgqOut [j], (MSGQ_Msg) msg, 0, msgqIn) ;
                if (status != MSGQ_SUCCESS) {
                    sysStatus = SYS_EFAIL ;
                    LOG_printf (&trace,
                                "MSGQ_put failed. Status = 0x%x\n",
                                status) ;
                }
            }
        }
    }

    /*
     *  Wait forever for the message
     */
    msg = (TestMsg *) MSGQ_get (msgqIn, SYS_FOREVER) ;
    if (msg == NULL) {
        status = SYS_EALLOC ;
        LOG_printf (&trace,
                    "MSGQ_get (msgqIn ...) failed. Status = 0x%x\n",
                    status) ;
    }
    else {
        MSGQ_free ((MSGQ_Msg) msg) ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   SendMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the SendMsg_create function.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
SendMsg_delete ()
{
    Int i         ;

    for (i = 0 ; (i < numRemoteMsgqs) && (msgqOut [i] != NULL) ; i++) {
        MSGQ_release (msgqOut [i]) ;
        msgqOut [i] = NULL ;
    }
    MEM_free (DSPLINK_SEGID, msgqOut, sizeof (MSGQ_Handle) * numRemoteMsgqs) ;
    msgqOut = NULL ;
    if (msgqIn != NULL) {
        MSGQ_delete (msgqIn) ;
    }
    msgqIn = NULL ;
    if (semHandle != NULL) {
        SEM_delete (semHandle) ;
    }
    CleanupMessaging (NUM_LOCAL_MSGQS) ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the SendMsg task.
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
    maxRemoteMsgqs   = numRemoteMsgqs + sendMsgqId ;
    inTask = TSK_create ((Fxn) SendMsg, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
