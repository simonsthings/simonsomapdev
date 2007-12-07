/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\message
 *
 *  @desc   Main function that calls SWI or TSK message applications based
 *          on the parameter TSK_MODE.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <log.h>
#include <swi.h>
#include <sys.h>
#include <sio.h>
#include <tsk.h>
#include <gio.h>
#include <msgq.h>
#include <mqtbiosque.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <platform.h>
#include <failure.h>
#include <mqabuf.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Sample Headers              */
#include <tskMessage.h>
#include <swiMessage.h>


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID is used by SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID  FID_APP_C

/** ============================================================================
 *  @const  TSK_MODE/SWI_MODE
 *
 *  @desc   Mode of application in which message application is to be run.
 *  ============================================================================
 */
#define TSK_MODE    1
#define SWI_MODE    2

/** ============================================================================
 *  @name   APPLICATION_MODE
 *
 *  @desc   Mode of the application can be configured here to TSK_MODE or
 *          SWI_MODE.
 *  ============================================================================
 */
#define APPLICATION_MODE TSK_MODE

/** ============================================================================
 *  @const  NUMMSGQUEUES
 *
 *  @desc   Number of local message queues
 *  ============================================================================
 */
#define NUMMSGQUEUES   1

/** ============================================================================
 *  @const  NUMMSGPOOLS
 *
 *  @desc   Number of BUF pools in the allocator
 *  ============================================================================
 */
#define NUMMSGPOOLS   4

/** ============================================================================
 *  @const  NUMMSGINPOOL0
 *
 *  @desc   Number in first message pool.
 *  ============================================================================
 */
#define NUMMSGINPOOL0   1

/** ============================================================================
 *  @const  NUMMSGINPOOL1
 *
 *  @desc   Number in second message pool.
 *  ============================================================================
 */
#define NUMMSGINPOOL1   1

/** ============================================================================
 *  @const  NUMMSGINPOOL2
 *
 *  @desc   Number in third message pool.
 *  ============================================================================
 */
#define NUMMSGINPOOL2   4

/** ============================================================================
 *  @const  NUMMSGINPOOL3
 *
 *  @desc   Number in fourth message pool.
 *  ============================================================================
 */
#define NUMMSGINPOOL3   1

/** ============================================================================
 *  @const  APP_BUFFER_SIZE
 *
 *  @desc   Messaging buffer used by the application.
 *  ============================================================================
 */
#define APP_BUFFER_SIZE             (ALIGN (sizeof (MSGQ_MsgHeader)))

/** ============================================================================
 *  @const  MAX_MSGQS
 *
 *  @desc   Maximum number of MSGQs required by this application.
 *  ============================================================================
 */
#define MAX_MSGQS             1

/** ============================================================================
 *  @name   AllocatorIds
 *
 *  @desc   Enumerated the values of allocator ids.
 *
 *  @field  MQABUFID
 *              The BUF based allocator.
 *  @field  NUMALLOCATORS
 *              Total number of allocators.
 *  ============================================================================
 */
typedef enum {
    MQABUFID = 0,
    NUMALLOCATORS
} AllocatorIds;

/** ============================================================================
 *  @name   TransportIds
 *
 *  @desc   Enumerated the values of transport ids.
 *
 *  @field  LOCALMQT
 *              Id of the local transport.
 *  @field  REMOTEMQT
 *              Id of the remote transport.
 *  @field  NUMTRANSPORTS
 *              Total number of transports.
 *  ============================================================================
 */
typedef enum {
    LOCALMQT = 0,
    REMOTEMQT,
    NUMTRANSPORTS
} TransportIds;

/** ============================================================================
 *  @name   msgSizes
 *
 *  @desc   Message sizes managed by the allocator.
 *  ============================================================================
 */
static Uint16 msgSizes[NUMMSGPOOLS] = {APP_BUFFER_SIZE,
                                       MQTDSPLINK_CTRLMSG_SIZE,
                                       sizeof (MSGQ_AsyncErrorMsg),
                                       sizeof (MSGQ_AsyncLocateMsg)};


/** ============================================================================
 *  @name   msgCount
 *
 *  @desc   Number of messages in each pool managed by the allocator.
 *  ============================================================================
 */
static Uint16 msgCount[NUMMSGPOOLS] = {NUMMSGINPOOL0,
                                       NUMMSGINPOOL1,
                                       NUMMSGINPOOL2,
                                       NUMMSGINPOOL3};

/** ============================================================================
 *  @name   mqaBufParams
 *
 *  @desc   Configuration parameters for the BUF-based allocator instance.
 *  ============================================================================
 */
static MQABUF_Params mqaBufParams = {NUMMSGPOOLS,    /* Number of pools     */
                                     msgSizes,       /* Array of sizes      */
                                     msgCount,       /* Array of msg counts */
                                     0};             /* SegId               */

/** ============================================================================
 *  @name   allocators
 *
 *  @desc   Array of allocators.
 *  ============================================================================
 */
static MSGQ_AllocatorObj allocators[NUMALLOCATORS] =
{
    {"MQABUF",                               /* Name of the allocator         */
      (MSGQ_AllocatorFxns *)&MQABUF_FXNS,    /* Allocator interface functions */
      &mqaBufParams,                         /* Allocator configuration       */
      NULL,                                  /* Filled in by allocator        */
      MQABUFID}                              /* Allocator Id                  */
};

/** ============================================================================
 *  @name   mqtdsplinkParams
 *
 *  @desc   Configuration parameters for the DSPLINK transport instance.
 *  ============================================================================
 */
static MQTDSPLINK_Params mqtDsplinkParams = {MAX_MSGQS,
                                             MQTDSPLINK_CTRLMSG_SIZE,
                                             MQABUFID} ;


/** ============================================================================
 *  @name   transports
 *
 *  @desc   Array of transports.
 *  ============================================================================
 */
static MSGQ_TransportObj transports[NUMTRANSPORTS] =
{
    {"LOCALMQT",                             /* Name of the transport         */
     (MSGQ_TransportFxns *)&MQTBIOSQUE_FXNS, /* Transport interface functions */
     NULL,                                   /* Transport configuration       */
     NULL,                                   /* Filled in by transport        */
     LOCALMQT},                              /* Transport Id                  */

    {"REMOTMQT",                             /* Name of the transport         */
     (MSGQ_TransportFxns *)&MQTDSPLINK_FXNS, /* Transport interface functions */
     &mqtDsplinkParams,                      /* Transport configuration       */
     NULL,                                   /* Filled in by transport        */
     REMOTEMQT}                              /* Transport Id                  */
};

/** ============================================================================
 *  @name   msgQueues
 *
 *  @desc   Array of message queues.
 *  ============================================================================
 */
static MSGQ_Obj msgQueues[NUMMSGQUEUES];


/** ============================================================================
 *  @name   msgqConfig
 *
 *  @desc   MSGQ configuration information.
 *  ============================================================================
 */
static MSGQ_Config msgqConfig = {msgQueues, allocators, transports,
                                 NUMMSGQUEUES, NUMALLOCATORS, NUMTRANSPORTS,
                                 LOCALMQT};  /* local transport */

/** ============================================================================
 *  @name   MSGQ
 *
 *  @desc   The static configuration of the MSGQ module.  MSGQ is a required
 *          global variable.
 *  ============================================================================
 */
MSGQ_Config *MSGQ = &msgqConfig;


/** ============================================================================
 *  @name   trace
 *
 *  @desc   trace LOG_Obj used to do LOG_printf
 *  ============================================================================
 */
extern LOG_Obj trace;


#if (APPLICATION_MODE == TSK_MODE)
/** ----------------------------------------------------------------------------
 *  @func   tskMessage
 *
 *  @desc   Task for TSK based TSKMESSAGE application.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Int tskMessage();
#endif /* (APPLICATION_MODE == TSK_MODE) */


/** ============================================================================
 *  @func   main
 *
 *  @desc   Entry function.
 *
 *  @modif  None
 *  ============================================================================
 */
Void main(Int argc, Char *argv[])
{
#if (APPLICATION_MODE == TSK_MODE)
    /* TSK based application */
    TSK_Handle                tskMessageTask;
#else /* (APPLICATION_MODE == TSK_MODE) */
    /* SWI based application */
    SWIMESSAGE_TransferInfo * info;
    Uns status;
#endif /* (APPLICATION_MODE == TSK_MODE) */

#if (APPLICATION_MODE == TSK_MODE)
    /* Creating task for TSKMESSAGE application */
    tskMessageTask = TSK_create(tskMessage, NULL, 0);
    if (tskMessageTask != NULL) {
        LOG_printf(&trace, "Create TSKMESSAGE: Success\n");
    }
    else {
        SET_FAILURE_REASON (0x0) ;
        LOG_printf(&trace, "Create TSKMESSAGE: Failed.\n");
        return;
    }
#else /* (APPLICATION_MODE == TSK_MODE) */
    /* Create phase of SWIMESSAGE application */
    status = SWIMESSAGE_create(&info);
    if (status != SYS_OK) {
        SET_FAILURE_REASON (status);
    }
    else {
        /* Set the message queue that will receive any async. errors.
         * The message queue was created in the SWIMESSAGE_create().
         */
        MSGQ_setErrorHandler(info->msgqHandle, MQABUFID);

    /* Delete phase of SWIMESSAGE application: This is not called right now
     * because SWI application runs forever
     */

    /*
     *  Reset the error handler before deleting the MSGQ that receives the error
     *  messages.
     */
    /*
    MSGQ_setErrorHandler(NULL, 0);
    deleteStatus = SWIMESSAGE_delete(info);
    */
    /*
    if (deleteStatus != SYS_OK) {
        SET_FAILURE_REASON (deleteStatus);
    }
    */
    }
#endif /* (APPLICATION_MODE == TSK_MODE) */

    return;
}


#if (APPLICATION_MODE == TSK_MODE)
/** ----------------------------------------------------------------------------
 *  @func   tskMessage
 *
 *  @desc   Task for TSK based TSKMESSAGE application.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int tskMessage()
{
    Uns                      status = SYS_OK;
    TSKMESSAGE_TransferInfo *info;

    /* Create Phase */
    status = TSKMESSAGE_create (&info);

    /* Execute Phase */
    if (status == SYS_OK) {
        /* Set the message queue that will receive any async. errors.
         * The message queue was created in the TSKMESSAGE_create().
         */
        MSGQ_setErrorHandler(info->msgqHandle, MQABUFID);

        /* Start the execution phase. */
        status = TSKMESSAGE_execute (info);
        if (status != SYS_OK) {
            SET_FAILURE_REASON(status);
        }
    }

    /* Delete Phase */
    status = TSKMESSAGE_delete (info);
    if (status != SYS_OK) {
        SET_FAILURE_REASON(status);
    }
    return status;
}
#endif /* (APPLICATION_MODE == TSK_MODE) */
