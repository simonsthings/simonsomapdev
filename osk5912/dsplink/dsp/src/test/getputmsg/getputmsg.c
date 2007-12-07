/** ============================================================================
 *  @file   GetPutMsg.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\GetPutMsg
 *
 *  @desc   Implemetation of GetPutMsg. This test sends / receives
 *          messages to GPP depending upon the mode of operation.
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
#include <GetPutMsg.h>


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
 *  @desc   Trace object used for writing debug strings and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;

/** ============================================================================
 *  @name   BATCH_SIZE
 *
 *  @desc   The number of of messages to be transferred in one batch.
 *  ============================================================================
 */
#define BATCH_SIZE 25

/** ----------------------------------------------------------------------------
 *  @name   msgqIn
 *
 *  @desc   MSGQ handle used for accessing the input queue.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Handle msgqIn ;

/** ----------------------------------------------------------------------------
 *  @name   msgqOut
 *
 *  @desc   MSGQ handle used for accessing the output queue.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Handle msgqOut ;

/** ----------------------------------------------------------------------------
 *  @name   inMsgqName
 *
 *  @desc   Space for the input message queue name.
 *  ----------------------------------------------------------------------------
 */
static Char inMsgqName [MAX_MSGQ_NAME_LEN] ;

/** ----------------------------------------------------------------------------
 *  @name   outMsgqName
 *
 *  @desc   Space for the output message queue name.
 *  ----------------------------------------------------------------------------
 */
static Char outMsgqName [MAX_MSGQ_NAME_LEN] ;

/** ----------------------------------------------------------------------------
 *  @name   inSemHandle
 *
 *  @desc   The semaphore handle used for notify on the msgqueue.
 *  ----------------------------------------------------------------------------
 */
static SEM_Handle inSemHandle ;

/** ----------------------------------------------------------------------------
 *  @name   numMsgToExchange
 *
 *  @desc   The number of messages to be exchanged.
 *  ----------------------------------------------------------------------------
 */
static Uns numMsgToExchange ;

/** ----------------------------------------------------------------------------
 *  @name   modeOfOperation
 *
 *  @desc   The mode in which the test will execute.
 *
 *          If modeOfOperation == 1 then continuously send messages to GPP
 *          side.
 *
 *          If modeOfOperation == 2 then continuously receive messages from GPP
 *          side.
 *
 *          If modeOfOperation == 3 then continuously receive and send messages
 *          to GPP side.
 *  ----------------------------------------------------------------------------
 */
static Uns modeOfOperation ;

/** ----------------------------------------------------------------------------
 *  @name   msg
 *
 *  @desc   Message pointer to be used for message exchange.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Msg  msg ;

/** ----------------------------------------------------------------------------
 *  @name   msgSize
 *
 *  @desc   Size to be used for message allocation.
 *  ----------------------------------------------------------------------------
 */
static Uns  msgSize ;

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
static Uns numRemoteMsgqs ;

/** ----------------------------------------------------------------------------
 *  @name   receiveMsgqId
 *
 *  @desc   Message queue on which to receive messages.
 *  ----------------------------------------------------------------------------
 */
static Uns receiveMsgqId ;

/** ----------------------------------------------------------------------------
 *  @name   sendMsgqId
 *
 *  @desc   Message queue to which to messages are to be sent.
 *  ----------------------------------------------------------------------------
 */
static Uns sendMsgqId ;


/** ----------------------------------------------------------------------------
 *  @func   GetPutMsg_create
 *
 *  @desc   This function allocates the resources needed by the GetPutMsg test.
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
 *  @see    GetPutMsg_execute, GetPutMsg_delete
 *  ----------------------------------------------------------------------------
 */
static
Uns
GetPutMsg_create () ;


/** ----------------------------------------------------------------------------
 *  @func   GetPutMsg_execute
 *
 *  @desc   This function does the message exchange with the GPP.
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
 *  @see    GetPutMsg_delete, GetPutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
GetPutMsg_execute () ;


/** ----------------------------------------------------------------------------
 *  @func   GetPutMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the GetPutMsg_create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    GetPutMsg_execute, GetPutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Void
GetPutMsg_delete () ;


/** ============================================================================
 *  @func   GetPutMsg
 *
 *  @desc   This function receives / sends messages to the GPP depending upon
 *          the mode of operation.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
GetPutMsg ()
{
    Uns status = SYS_OK ;

    status = GetPutMsg_create () ;

    if (status == SYS_OK) {
        status = GetPutMsg_execute () ;
    }

    GetPutMsg_delete () ;
}


/** ----------------------------------------------------------------------------
 *  @func   GetPutMsg_create
 *
 *  @desc   This function allocates the resources needed by the GetPutMsg test.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
GetPutMsg_create ()
{
    MSGQ_Status      status      = MSGQ_SUCCESS     ;
    Uns              sysStatus   = SYS_OK           ;
    MSGQ_Attrs       msgqAttrs   = MSGQ_ATTRS       ;
    SEM_Attrs        semAttrs    = SEM_ATTRS        ;
    MSGQ_LocateAttrs locateAttrs = MSGQ_LOCATEATTRS ;

    sysStatus = SetupMessaging (numMsgPerPool,
                                maxMsgSize,
                                NUM_LOCAL_MSGQS,
                                numRemoteMsgqs) ;
    if (sysStatus != SYS_OK) {
        status = MSGQ_EFAILURE ;
        LOG_printf (&trace,
                    "Setup for messaging failed. Status = 0x%x\n",
                    status) ;
    }
    else {
        /*
         *  Create a message queue that uses the MQTDSPLINK transport interface
         *  and the MQABUF allocation interface.
         */
        semAttrs  = SEM_ATTRS ;
        inSemHandle = SEM_create (0, &semAttrs) ;

        if (inSemHandle != NULL) {
            /* Fill in the attributes for this message queue. */
            msgqAttrs.notifyHandle = inSemHandle ;
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

    if (status == MSGQ_SUCCESS) {
        GetDsplinkMsgqName (outMsgqName,
                            DSPLINK_GPPMSGQ_NAME,
                            sendMsgqId) ;
        /*
         * Synchronous locate
         */
        locateAttrs.mqaId   = TST_MQABUF_ID ;
        status = MSGQ_ENOTFOUND ;
        while (status == MSGQ_ENOTFOUND) {
            status = MSGQ_locate (outMsgqName, &msgqOut, &locateAttrs) ;
            if (status == MSGQ_ENOTFOUND) {
                TSK_sleep (1000) ;
            }
            else if (status != MSGQ_SUCCESS) {
                sysStatus = SYS_EFAIL ;
                LOG_printf (&trace, "MSGQ_locate (msgqOut) failed. "
                            "Status = 0x%x\n", status) ;
            }
        }
    }

    /*  ------------------------------------------------------------------------
     *  Sending a message to GPP side to indicate that the locate is complete
     *  and waiting for a message from GPP side.
     *  ------------------------------------------------------------------------
     */
    if (status == MSGQ_SUCCESS) {
        msg = MSGQ_alloc (TST_MQABUF_ID, ALIGN (msgSize)) ;
        if (msg != NULL) {
            status = MSGQ_put (msgqOut, msg, 0, NULL) ;
            if (status != MSGQ_SUCCESS) {
                sysStatus = SYS_EFAIL ;
                LOG_printf (&trace,
                            "MSGQ_put failed. Status = 0x%x\n",
                            status) ;
            }
        }
        else {
            sysStatus = SYS_EALLOC ;
            status = MSGQ_EMEMORY ;
            LOG_printf (&trace,
                        "MSGQ_alloc failed. Status = 0x%x\n",
                        status) ;
        }
    }

    if (status == MSGQ_SUCCESS) {
        msg = MSGQ_get (msgqIn, SYS_FOREVER) ;
        if (msg == NULL) {
            status = SYS_EALLOC ;
            LOG_printf (&trace,
                        "MSGQ_get (msgqIn) failed. Status = 0x%x\n",
                        MSGQ_EFAILURE) ;
        }
        else {
            MSGQ_free ((MSGQ_Msg) msg) ;
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   GetPutMsg_execute
 *
 *  @desc   This function does the message exchange with the GPP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
GetPutMsg_execute ()
{
    MSGQ_Status status    = MSGQ_SUCCESS ;
    Uns         sysStatus = SYS_OK       ;
    Uint16      j         = 0            ;
    Uns         i         = 0            ;

    for (i = 0 ; ((i < numMsgToExchange) && (status == SYS_OK)) ;
         i += BATCH_SIZE) {
        for (j = 0 ; (j < BATCH_SIZE) && (status == SYS_OK) ;
             j++) {
            if (modeOfOperation == 2 || modeOfOperation == 3) {
                msg = MSGQ_get (msgqIn, SYS_FOREVER) ;
                if (msg == NULL) {
                    status = SYS_EALLOC ;
                    LOG_printf (&trace,
                                "MSGQ_get (msgqIn) failed. Status = 0x%x\n",
                                MSGQ_EFAILURE) ;
                }
                else {
                    MSGQ_free ((MSGQ_Msg) msg) ;
                }
            }

            if (   (status == MSGQ_SUCCESS)
                && (   modeOfOperation == 1
                    || modeOfOperation == 3)) {
                msg = MSGQ_alloc (TST_MQABUF_ID, ALIGN (msgSize)) ;
                if (msg != NULL) {
                    status = MSGQ_put (msgqOut, msg, 0, NULL) ;
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
            }
        }
        /*  ----------------------------------------------------------------
         *  Synchronize with the GPP side.
         *  ----------------------------------------------------------------
         */
        if (status == MSGQ_SUCCESS) {
            if (modeOfOperation == 1) {
                msg = MSGQ_get (msgqIn, SYS_FOREVER) ;
                if (msg == NULL) {
                    status = SYS_EALLOC ;
                    LOG_printf (&trace,
                                "MSGQ_get (msgqIn) failed. Status = 0x%x\n",
                                MSGQ_EFAILURE) ;
                }
                else {
                    MSGQ_free ((MSGQ_Msg) msg) ;
                }
            }
            else if (modeOfOperation == 2) {
                msg = MSGQ_alloc (TST_MQABUF_ID, ALIGN (msgSize)) ;
                if (msg != NULL) {
                    status = MSGQ_put (msgqOut, msg, 0, NULL) ;
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
            }
        }
    }

    if (status != MSGQ_SUCCESS) {
        sysStatus = SYS_EFAIL ;
    }
    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   GetPutMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the GetPutMsg_create function.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
GetPutMsg_delete ()
{
    /*  --------------------------------------------------------------------
     *  Synchronizing with the GPP side for cleanup.
     *  --------------------------------------------------------------------
     */
    msg = MSGQ_get (msgqIn, SYS_FOREVER) ;
    if (msg == NULL) {
        LOG_printf (&trace,
                    "MSGQ_get (msgqIn) failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }
    else {
        MSGQ_free ((MSGQ_Msg) msg) ;
    }

    MSGQ_release (msgqOut) ;
    MSGQ_delete (msgqIn) ;
    msgqIn = NULL ;
    SEM_delete (inSemHandle) ;
    CleanupMessaging (NUM_LOCAL_MSGQS) ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the GetPutMsg task.
 *
 *  @modif  None
 *  ============================================================================
 */
Void main (Int argc, Char * argv [])
{
    TSK_Handle    inTask ;

    numMsgPerPool    = atoi (argv [0]) ;
    maxMsgSize       = atoi (argv [1]) ;
    numRemoteMsgqs   = atoi (argv [2]) ;
    receiveMsgqId    = atoi (argv [3]) ;
    sendMsgqId       = atoi (argv [4]) ;
    numMsgToExchange = atoi (argv [5]) ;
    modeOfOperation  = atoi (argv [6]) ;

    msgSize = maxMsgSize / DSP_MAUSIZE ;
    inTask = TSK_create ((Fxn) GetPutMsg, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
