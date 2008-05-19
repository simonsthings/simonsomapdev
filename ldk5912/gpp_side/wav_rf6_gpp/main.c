/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)/dsp/src/samples/loop/
 *
 *  @desc   Main function that calls SWI or TSK loopback applications based
 *          on the compilation flag SWI_MODE or TSK_MODE.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
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
#include <pool.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <failure.h>
#include <platform.h>

#if defined (CHNL_PCPY_LINK)
#include <buf_pool.h>
#endif /* if defined (CHNL_PCPY_LINK) */

/*  ----------------------------------- Sample Headers              */
#include <tskLoop.h>
#include <swiLoop.h>
#include <loop_config.h>


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID is used by SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID              FID_APP_C


/** ============================================================================
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for TSK based loopback.
 *  ============================================================================
 */
Uint32 xferBufSize ;

/** ============================================================================
 *  @name   numTransfers
 *
 *  @desc   Iterations of data transfer to be done by the application.
 *          A value of 0 in numTransfers implies infinite iterations.
 *  ============================================================================
 */
Uint16 numTransfers ;

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


#if defined (TSK_MODE)
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
static Int tskLoop () ;
#endif /* if defined (TSK_MODE) */


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
#if defined (SWI_MODE)
    /* SWI based loopback */
    SWILOOP_TransferInfo * info;
#else /* if defined (SWI_MODE) */
    /* TSK based loopback */
    TSK_Handle tskLoopTask;
#endif /* if defined (SWI_MODE) */
#if defined (SWI_MODE) || defined (CHNL_PCPY_LINK)
    Int status ;
#endif /* if defined (SWI_MODE) || defined (CHNL_PCPY_LINK) */

#if defined (CHNL_PCPY_LINK)
    Uint32         bufSizes   [NUM_BUF_SIZES] ;
    Uint32         numBufs    [NUM_BUF_SIZES] ;
    BUFPOOL_Params params ;

    /* Close the previously opened dummy pool. */
    SAMPLEPOOL_close (SAMPLE_POOL_ID) ;
#endif /* defined (CHNL_PCPY_LINK) */

    /* Get the number of transfers to be done by the application */
    numTransfers = atoi (argv[1]) ;

    /* Transfer size for loopback given by GPP side */
    xferBufSize  = atoi (argv[0]) / DSP_MAUSIZE ;

#if defined (CHNL_PCPY_LINK)
    bufSizes [0] = xferBufSize ;
    numBufs  [0] = NUM_BUFFERS ;

    params.numBufObjs   = NUM_BUF_SIZES ;
    params.bufSizes     = bufSizes ;
    params.numBuffers   = numBufs ;
    params.segId        = DSPLINK_SEGID ;

    LOOP_Pools [SAMPLE_POOL_ID].initFxn = BUFPOOL_init ;
    LOOP_Pools [SAMPLE_POOL_ID].fxns    = (POOL_Fxns *) &BUFPOOL_FXNS ;
    LOOP_Pools [SAMPLE_POOL_ID].params  = &params ;
    LOOP_Pools [SAMPLE_POOL_ID].object  = NULL ;

    /* Open the pool with new parameters. */
    status = SAMPLEPOOL_open (SAMPLE_POOL_ID, &params) ;
    if (status != SYS_OK) {
        LOG_printf(&trace, "Pool Open Failed.\n");
        SET_FAILURE_REASON (status);
    }
#endif /* defined (CHNL_PCPY_LINK) */

#if defined (SWI_MODE)
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
#else /* if defined (SWI_MODE) */
    /* Creating task for TSKLOOP application */
    tskLoopTask = TSK_create(tskLoop, NULL, 0);
    if (tskLoopTask != NULL) {
        LOG_printf(&trace, "Create TSKLOOP: Success\n");
    }
    else {
        LOG_printf(&trace, "Create TSKLOOP: Failed.\n");
        return;
    }
#endif /* if defined (SWI_MODE) */

    return;
}


#if defined (TSK_MODE)
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
    Int status = SYS_OK;
    TSKLOOP_TransferInfo *info;

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

    return status ;
}
#endif /* if defined (TSK_MODE) */
