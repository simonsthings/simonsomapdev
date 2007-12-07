/** ============================================================================
 *  @file   LocateTimeoutMsg.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\LocateTimeoutMsg
 *
 *  @desc   Implemetation of LocateTimeoutMsg. This test creates a
 *          queue and waits for a message from the GPP. Upon receiving the
 *          message, it interprets the info field of the message as a delay
 *          value and sleeps for the delay time. Then it creates another queue,
 *          deletes the first one and waits for a message on the second
 *          queue before shutting down.
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
#include <platform.h>
#include <mqabuf.h>
#include <mqtbiosque.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Test Headers                */
#include <MsgqUtils.h>
#include <LocateTimeoutMsg.h>


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
 *  @name   DSP_CONTROL_MSGQ_ID
 *
 *  @desc   MSGQ id of the control MSGQ used for the test application.
 *  ============================================================================
 */
#define DSP_CONTROL_MSGQ_ID     0

/** ============================================================================
 *  @name   GPP_CONTROL_MSGQ_ID
 *
 *  @desc   MSGQ id of the control MSGQ on the GPP side used for
 *          the test application.
 *  ============================================================================
 */
#define GPP_CONTROL_MSGQ_ID     0

/** ============================================================================
 *  @name   CONTROL_LOCATE_SLEEP
 *
 *  @desc   Locate timeout for locating the GPP control MSGQ.
 *  ============================================================================
 */
#define CONTROL_LOCATE_SLEEP    1000

/** ============================================================================
 *  @name   trace
 *
 *  @desc   This is the trace object used for writing debug strings
 *          and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;

/** ----------------------------------------------------------------------------
 *  @name   dspControlMsgq/gppControlMsgq
 *
 *  @desc   Message queue handles for control messages.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Handle dspControlMsgq ;
static MSGQ_Handle gppControlMsgq ;

/** ----------------------------------------------------------------------------
 *  @name   testMsgq
 *
 *  @desc   Message queue handles for control messages.
 *  ----------------------------------------------------------------------------
 */
static MSGQ_Handle testMsgq ;

/** ----------------------------------------------------------------------------
 *  @name   dspControlMsgqName
 *
 *  @desc   Name of the DSP control MSGQ.
 *  ----------------------------------------------------------------------------
 */
static Char dspControlMsgqName [MAX_MSGQ_NAME_LEN] ;

/** ----------------------------------------------------------------------------
 *  @name   gppControlMsgqName
 *
 *  @desc   Name of the GPP control MSGQ.
 *  ----------------------------------------------------------------------------
 */
static Char gppControlMsgqName [MAX_MSGQ_NAME_LEN] ;

/** ----------------------------------------------------------------------------
 *  @name   testMsgqName
 *
 *  @desc   Name of the test MSGQ.
 *  ----------------------------------------------------------------------------
 */
static Char testMsgqName [MAX_MSGQ_NAME_LEN] ;

/** ----------------------------------------------------------------------------
 *  @name   dspControlMsgqSem
 *
 *  @desc   Handle of SEM for DSP control MSGQ.
 *  ----------------------------------------------------------------------------
 */
static SEM_Handle dspControlMsgqSem ;

/** ----------------------------------------------------------------------------
 *  @name   testControlSem
 *
 *  @desc   Handle of SEM for test MSGQ.
 *  ----------------------------------------------------------------------------
 */
static SEM_Handle testMsgqSem ;

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
 *  @name   testQueueId
 *
 *  @desc   Message Queue id on which the test is performed.
 *  ----------------------------------------------------------------------------
 */
static Uns testQueueId ;


/** ----------------------------------------------------------------------------
 *  @func   LocateTimeoutMsg_create
 *
 *  @desc   This function allocates the resources needed by the
 *          LocateTimeoutMsg test.
 *
 *  @arg    None
 *
 *  @ret    Status
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LocateTimeoutMsg_execute, LocateTimeoutMsg_delete
 *  ----------------------------------------------------------------------------
 */
static
Uns
LocateTimeoutMsg_create () ;


/** ----------------------------------------------------------------------------
 *  @func   LocateTimeoutMsg_execute
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
 *  @see    LocateTimeoutMsg_delete, LocateTimeoutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
LocateTimeoutMsg_execute () ;


/** ----------------------------------------------------------------------------
 *  @func   LocateTimeoutMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the LocateTimeoutMsg_create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LocateTimeoutMsg_execute, LocateTimeoutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Void
LocateTimeoutMsg_delete () ;


/** ----------------------------------------------------------------------------
 *  @func   CreateMsgq
 *
 *  @desc   Helper function to create a MSGQ.
 *
 *  @arg    msgqName
 *              Placeholder for the MSGQ name.
 *  @arg    id
 *              ID for the MSGQ.
 *  @arg    semHandle
 *              SEM handle for the MSGQ.
 *  @arg    msgq
 *              Placeholder for MSGQ.
 *
 *  @ret    SYS_OK
 *              Placeholder for MSGQ.
 *          SYS_EFAIL
 *              Failure occurred.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LocateTimeoutMsg_execute, LocateTimeoutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
CreateMsgq (Char * msgqName, Uns id, SEM_Handle * semHandle, MSGQ_Handle * msgq) ;


/** ----------------------------------------------------------------------------
 *  @func   DeleteMsgq
 *
 *  @desc   Delete the MSGQ.
 *
 *  @arg    msgq
 *              Placeholder for MSGQ.
 *
 *  @ret    SYS_OK
 *              Placeholder for MSGQ.
 *          SYS_EFAIL
 *              Failure occurred.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LocateTimeoutMsg_execute, LocateTimeoutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
DeleteMsgq (MSGQ_Handle msgq, SEM_Handle semHandle) ;


/** ----------------------------------------------------------------------------
 *  @func   LocateGppMsgq
 *
 *  @desc   Locate a MSGQ.
 *
 *  @arg    msgqName
 *              Name of the MSGQ.
 *  @arg    id
 *              Identifier of the MSGQ.
 *
 *  @ret    SYS_OK
 *              Placeholder for MSGQ.
 *          SYS_EFAIL
 *              Failure occurred.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LocateTimeoutMsg_execute, LocateTimeoutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
LocateGppMsgq (Char * msgqName, Uns id) ;


/** ----------------------------------------------------------------------------
 *  @func   GetMessage
 *
 *  @desc   Get a message on a MSGQ.
 *
 *  @arg    msgq
 *              MSGQ handle.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LocateTimeoutMsg_execute, LocateTimeoutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
GetMessage (MSGQ_Handle msgq) ;


/** ----------------------------------------------------------------------------
 *  @func   PutMessage
 *
 *  @desc   Put a message on the MSGQ.
 *
 *  @arg    msgq
 *              MSGQ handle.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LocateTimeoutMsg_execute, LocateTimeoutMsg_create
 *  ----------------------------------------------------------------------------
 */
static
Uns
PutMessage (MSGQ_Handle msgq) ;


/** ============================================================================
 *  @func   LocateTimeoutMsg
 *
 *  @desc   This function creates a queue and waits for a message
 *          from the GPP. Upon receiving the message, it interprets
 *          the info field of the message as a delay value and sleeps
 *          for the delay time. Then it creates another queue,
 *          deletes the first one and waits for a message on the second
 *          queue before shutting down.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
LocateTimeoutMsg ()
{
    Uns status = SYS_OK ;

    status = LocateTimeoutMsg_create () ;

    if (status == SYS_OK) {
        status = LocateTimeoutMsg_execute () ;
    }

    LocateTimeoutMsg_delete () ;
}


/** ----------------------------------------------------------------------------
 *  @func   LocateTimeoutMsg_create
 *
 *  @desc   This function allocates the resources needed by the
 *          LocateTimeoutMsg test.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
LocateTimeoutMsg_create ()
{
    MSGQ_Status status    = MSGQ_SUCCESS ;
    Uns         sysStatus = SYS_OK       ;

    sysStatus = SetupMessaging (numMsgPerPool, maxMsgSize,
                                NUM_LOCAL_MSGQS, numRemoteMsgqs) ;

    if (sysStatus == SYS_OK) {
        sysStatus = CreateMsgq (dspControlMsgqName,
                                DSP_CONTROL_MSGQ_ID,
                                &dspControlMsgqSem,
                                &dspControlMsgq) ;
    }
    else {
        LOG_printf (&trace,
                    "Setup for messaging failed. Status = 0x%x\n",
                    status) ;
    }

    if (sysStatus == SYS_OK) {
        sysStatus = LocateGppMsgq (gppControlMsgqName, GPP_CONTROL_MSGQ_ID) ;
    }
    else {
        LOG_printf (&trace,
                    "Failed in creation of MSGQ. Status = 0x%x\n",
                    status) ;
    }

    if (status == SYS_OK) {
        status = PutMessage (gppControlMsgq) ;
    }
    else {
        status = MSGQ_EFAILURE ;
        LOG_printf (&trace,
                    "Setup for messaging failed. Status = 0x%x\n",
                    status) ;
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   LocateTimeoutMsg_execute
 *
 *  @desc   This function does the actual exchange with the GPP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
LocateTimeoutMsg_execute ()
{
    Uns sysStatus = SYS_OK ;

    sysStatus = GetMessage (dspControlMsgq) ;
    if (sysStatus == SYS_OK) {
        sysStatus = CreateMsgq (testMsgqName,
                                testQueueId,
                                &testMsgqSem,
                                &testMsgq) ;
    }
    else {
        LOG_printf (&trace,
                    "GetMessage failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }

    if (sysStatus == SYS_OK) {
        sysStatus = PutMessage (gppControlMsgq) ;
    }
    else {
        LOG_printf (&trace,
                    "CreateMsgq failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }

    if (sysStatus == SYS_OK) {
        sysStatus = GetMessage (dspControlMsgq) ;
    }
    else {
        LOG_printf (&trace,
                    "PutMessage failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }

    if (sysStatus == SYS_OK) {
        sysStatus = DeleteMsgq (testMsgq, testMsgqSem) ;
    }
    else {
        LOG_printf (&trace,
                    "GetMessage failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }

    if (sysStatus == SYS_OK) {
        sysStatus = PutMessage (gppControlMsgq) ;
    }
    else {
        LOG_printf (&trace,
                    "CreateMsgq failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }

    if (sysStatus != SYS_OK) {
        LOG_printf (&trace,
                    "PutMessage failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   LocateTimeoutMsg_delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the LocateTimeoutMsg_create function.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
LocateTimeoutMsg_delete ()
{
    GetMessage (dspControlMsgq) ;
    DeleteMsgq (dspControlMsgq, dspControlMsgqSem) ;
    CleanupMessaging (NUM_LOCAL_MSGQS) ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the LocateTimeoutMsg task.
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
    testQueueId    = atoi (argv [3]) ;
    inTask = TSK_create ((Fxn) LocateTimeoutMsg, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


/** ----------------------------------------------------------------------------
 *  @func   CreateMsgq
 *
 *  @desc   Helper function to create a MSGQ.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
CreateMsgq (Char * msgqName, Uns id, SEM_Handle * semHandle, MSGQ_Handle * msgq)
{
    Uns        sysStatus = SYS_OK     ;
    MSGQ_Attrs msgqAttrs = MSGQ_ATTRS ;
    SEM_Attrs  semAttrs  = SEM_ATTRS  ;

    GetDsplinkMsgqName (msgqName, DSPLINK_DSPMSGQ_NAME, id) ;

    /*  Create the control MSGQ for the DSP side  */
    *(semHandle) = SEM_create (0, &semAttrs) ;
    if (*(semHandle) != NULL) {
        msgqAttrs.notifyHandle = *(semHandle) ;
        msgqAttrs.post         = (MSGQ_Post) SEM_post ;
        msgqAttrs.pend         = (MSGQ_Pend) BinarySemPend ;
        *(msgq) = MSGQ_create (msgqName, &msgqAttrs) ;
        if (*(msgq) == NULL) {
            sysStatus = SYS_EALLOC   ;
            LOG_printf (&trace,
                        "MSGQ_create failed. Status = 0x%x\n",
                        sysStatus) ;
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   DeleteMsgq
 *
 *  @desc   Delete the MSGQ.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
DeleteMsgq (MSGQ_Handle msgq, SEM_Handle semHandle)
{
    Uns sysStatus = SYS_OK ;
    MSGQ_Status msgqStatus ;

    msgqStatus = MSGQ_delete (msgq) ;
    if (msgqStatus != MSGQ_SUCCESS) {
        sysStatus = SYS_EFAIL ;
        LOG_printf (&trace,
                    "MSGQ_delete failed. Status = 0x%x\n",
                    MSGQ_EFAILURE) ;
    }

    SEM_delete (semHandle) ;

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   LocateGppMsgq
 *
 *  @desc   Locate a MSGQ.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
LocateGppMsgq (Char * msgqName, Uns id)
{
    Uns              sysStatus   = SYS_OK           ;
    MSGQ_LocateAttrs locateAttrs = MSGQ_LOCATEATTRS ;
    MSGQ_Status      msgqStatus  = MSGQ_SUCCESS     ;

    GetDsplinkMsgqName (msgqName, DSPLINK_GPPMSGQ_NAME, id) ;
    locateAttrs.mqaId = TST_MQABUF_ID ;
    msgqStatus = MSGQ_ENOTFOUND ;
    while (msgqStatus == MSGQ_ENOTFOUND) {
        msgqStatus = MSGQ_locate (msgqName, &gppControlMsgq, &locateAttrs) ;
        if (msgqStatus == MSGQ_ENOTFOUND) {
            TSK_sleep (CONTROL_LOCATE_SLEEP) ;
        }
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   GetMessage
 *
 *  @desc   Get a message on a MSGQ.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
GetMessage (MSGQ_Handle msgq)
{
    Uns       sysStatus = SYS_OK ;
    TestMsg * msg ;

    msg = (TestMsg *) MSGQ_get (msgq, SYS_FOREVER) ;
    if (msg == NULL) {
        sysStatus = SYS_EFAIL ;
    }
    else {
        MSGQ_free ((MSGQ_Msg) msg) ;
    }

    return sysStatus ;
}


/** ----------------------------------------------------------------------------
 *  @func   PutMessage
 *
 *  @desc   Put a message on the MSGQ.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Uns
PutMessage (MSGQ_Handle msgq)
{
    Uns         sysStatus = SYS_OK ;
    MSGQ_Status msgqStatus ;
    TestMsg *   msg ;

    msg = (TestMsg *) MSGQ_alloc (TST_MQABUF_ID, ALIGN (sizeof (TestMsg))) ;
    if (msg != NULL) {
        msgqStatus = MSGQ_put (msgq, (MSGQ_Msg) msg, 0, NULL) ;
        if (msgqStatus != MSGQ_SUCCESS) {
            sysStatus = SYS_EFAIL ;
            LOG_printf (&trace,
                        "MSGQ_put failed. Status = 0x%x\n",
                        msgqStatus) ;
        }
    }
    else {
        sysStatus = SYS_EALLOC ;
        msgqStatus = MSGQ_EMEMORY ;
        LOG_printf (&trace,
                    "MSGQ_alloc failed. Status = 0x%x\n",
                    msgqStatus) ;
    }

    return sysStatus ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
