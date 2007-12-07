/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\scale
 *
 *  @desc   Main function that calls SWI or TSK scale applications based
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
#include <failure.h>
#include <gio.h>
#include <msgq.h>
#include <mqtbiosque.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <platform.h>
#include <mqabuf.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Sample Headers              */
#include <scale.h>
#include <swiScale.h>
#include <tskScale.h>


/** ============================================================================
 *  @const  TSK_MODE/SWI_MODE
 *
 *  @desc   Mode of application in which scale application is to be run.
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
 *  @const  NUMMSGPOOLS
 *
 *  @desc   Number of BUF pools in the allocator
 *  ============================================================================
 */
#define NUMMSGPOOLS   2

/** ============================================================================
 *  @name   msgSizes
 *
 *  @desc   Message sizes managed by the allocator.
 *  ============================================================================
 */
static Uint16 msgSizes[NUMMSGPOOLS] = {ALIGN (sizeof (SCALE_ScaleMsg)),
                                       MQTDSPLINK_CTRLMSG_SIZE};

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
 *  @name   msgCount
 *
 *  @desc   Number of messages in each pool managed by the allocator.
 *  ============================================================================
 */
static Uint16 msgCount[NUMMSGPOOLS] = {NUMMSGINPOOL0,
                                       NUMMSGINPOOL1};

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
    {{"MQABUF",                             /* Name of the allocator         */
      (MSGQ_AllocatorFxns *)&MQABUF_FXNS,   /* Allocator interface functions */
      &mqaBufParams,                        /* Allocator configuration       */
      NULL,                                 /* Filled in by allocator        */
      MQABUFID}};                           /* Allocator Id                  */

/** ============================================================================
 *  @name   transports
 *
 *  @desc   Array of transports.
 *  ============================================================================
 */
static MSGQ_TransportObj transports[NUMTRANSPORTS] =
    {{"LOCALMQT",                           /* Name of the transport         */
     (MSGQ_TransportFxns *)&MQTBIOSQUE_FXNS,/* Transport interface functions */
     NULL,                                  /* Transport configuration       */
     NULL,                                  /* Filled in by transport        */
     LOCALMQT},                             /* Transport Id                  */
     {"REMOTEMQT",                          /* Name of the transport         */
     (MSGQ_TransportFxns *)&MQTDSPLINK_FXNS,/* Transport interface functions */
     NULL,                                  /* Transport configuration       */
     NULL,                                  /* Filled in by transport        */
     REMOTEMQT},                            /* Transport Id                  */
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
 *  @const  FILEID
 *
 *  @desc   FILEID is used by SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID  FID_APP_C

/** ============================================================================
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for TSK based scale.
 *  ============================================================================
 */
Uns xferBufSize ;

/** ============================================================================
 *  @name   numTransfers
 *
 *  @desc   Iterations of data transfer to be done by the application.
 *  ============================================================================
 */
Uns numTransfers ;

/** ============================================================================
 *  @name   trace
 *
 *  @desc   trace LOG_Obj used to do LOG_printf
 *  ============================================================================
 */
extern LOG_Obj trace;

/** ============================================================================
 *  @func   atoi
 *
 *  @desc   Converts character string to integer value.
 *
 *  @arg    str
 *              Pointer to character string.
 *
 *  @ret    <integer value>
 *              Converted integer value.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
extern int atoi(const char *str);


#if (APPLICATION_MODE == TSK_MODE)
/** ----------------------------------------------------------------------------
 *  @func   tskScale
 *
 *  @desc   Task for TSK based TSKSCALE application.
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
static Int tskScale();
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
    /* TSK based scale application */
    TSK_Handle              tskScaleTask;
#else /* (APPLICATION_MODE == TSK_MODE) */
    /* SWI based scale application */
    SWISCALE_TransferInfo * info;
    Uns status;
#endif /* (APPLICATION_MODE == TSK_MODE) */

    /* Get the number of transfers to be done by the application */
    numTransfers = atoi (argv[1]) ;

    /* Transfer size for loopback given by GPP side */
    xferBufSize = ALIGN (atoi (argv[0]) / DSP_MAUSIZE) ;

#if (APPLICATION_MODE == TSK_MODE)
    /* Creating task for TSKSCALE application */
    tskScaleTask = TSK_create(tskScale, NULL, 0);
    if (tskScaleTask != NULL) {
        LOG_printf(&trace, "Create TSKSCALE: Success\n");
    }
    else {
        SET_FAILURE_REASON (0x0) ;
        LOG_printf(&trace, "Create TSKSCALE: Failed.\n");
        return;
    }
#else /* (APPLICATION_MODE == TSK_MODE) */
    /* Create phase of SWISCALE application */
    status = SWISCALE_create(&info);
    if (status == SYS_OK) {

        /* Execute phase of SWISCALE application */
        status = SWISCALE_execute(info);
        if (status != SYS_OK) {
            SET_FAILURE_REASON (status);
        }
    }
    else {
        SET_FAILURE_REASON (status);
    }

    /* Delete phase of SWISCALE application: This is not called right now
     * because SWI application runs forever
     */
    /*
    deleteStatus = SWISCALE_delete(info);
    if (deleteStatus != SYS_OK) {
        SET_FAILURE_REASON (deleteStatus);
    }
    */
#endif /* (APPLICATION_MODE == TSK_MODE) */

    return;
}


#if (APPLICATION_MODE == TSK_MODE)
/** ----------------------------------------------------------------------------
 *  @func   tskScale
 *
 *  @desc   Task for TSK based TSKSCALE application.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int tskScale()
{
    TSKSCALE_TransferInfo *info;
    Uns                    status = SYS_OK;

    /* Create Phase */
    status = TSKSCALE_create (&info);

    /* Execute Phase */
    if (status == SYS_OK) {
        status = TSKSCALE_execute (info);
        if (status != SYS_OK) {
            SET_FAILURE_REASON(status);
        }
    }

    /* Delete Phase */
    status = TSKSCALE_delete (info);
    if (status != SYS_OK) {
        SET_FAILURE_REASON(status);
    }
    return status;
}
#endif /* (APPLICATION_MODE == TSK_MODE) */
