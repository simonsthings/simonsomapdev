/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\loop
 *
 *  @desc   Main function that calls SWI or TSK loopback applications based
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

#include <std.h>
#include <log.h>
#include <swi.h>
#include <sys.h>
#include <sio.h>
#include <tsk.h>
#include <failure.h>
#include <gio.h>
#include <platform.h>

#include <tskLoop.h>
#include <swiLoop.h>

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
 *  @desc   Mode of application in which loopback application is to be run.
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
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for TSK based loopback.
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


/** ----------------------------------------------------------------------------
 *  @func   tskLoop
 *
 *  @desc   Task for TSK based TSKLOOP application.
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
static Int tskLoop();


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
    /* TSK based loopback */
    TSK_Handle tskLoopTask;
#else /* (APPLICATION_MODE == TSK_MODE) */
    /* SWI based loopback */
    SWILOOP_TransferInfo * info;
    Uns status;
#endif /* (APPLICATION_MODE == TSK_MODE) */

    /* Get the number of transfers to be done by the application */
    numTransfers = atoi (argv[1]) ;

    /* Transfer size for loopback given by GPP side */
    xferBufSize = atoi (argv[0]) / DSP_MAUSIZE ;

#if (APPLICATION_MODE == TSK_MODE)
    /* Creating task for TSKLOOP application */
    tskLoopTask = TSK_create(tskLoop, NULL, 0);
    if (tskLoopTask != NULL) {
        LOG_printf(&trace, "Create TSKLOOP: Success\n");
    }
    else {
        SET_FAILURE_REASON (0x0) ;
        LOG_printf(&trace, "Create TSKLOOP: Failed.\n");
        return;
    }
#else /* (APPLICATION_MODE == TSK_MODE) */
    /* Create phase of SWILOOP application */
    status = SWILOOP_create(&info);
    if (status == SYS_OK) {

        /* Execute phase of SWILOOP application */
        status = SWILOOP_execute(info);
        if (status != SYS_OK) {
            SET_FAILURE_REASON (status);
        }
    }
    else {
        SET_FAILURE_REASON (status);
    }

    /* Delete phase of SWILOOP application: This is not called right now
     * because SWI application runs forever
     */
    /*
    deleteStatus = SWILOOP_delete(info);
    if (deleteStatus != SYS_OK) {
        SET_FAILURE_REASON (deleteStatus);
    }
    */
#endif /* (APPLICATION_MODE == TSK_MODE) */

    return;
}


/** ----------------------------------------------------------------------------
 *  @func   tskLoop
 *
 *  @desc   Task for TSK based TSKLOOP application.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int tskLoop()
{
    TSKLOOP_TransferInfo *info;
    Uns status = SYS_OK;

    /* Create Phase */
    status = TSKLOOP_create (&info);

    /* Execute Phase */
    if (status == SYS_OK) {
        status = TSKLOOP_execute (info);
        if (status != SYS_OK) {
            SET_FAILURE_REASON(status);
        }
    }

    /* Delete Phase */
    status = TSKLOOP_delete (info);
    if (status != SYS_OK) {
        SET_FAILURE_REASON(status);
    }
    return status;
}
