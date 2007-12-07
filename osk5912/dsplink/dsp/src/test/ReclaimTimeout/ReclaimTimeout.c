/** ============================================================================
 *  @file   ReclaimTimeout.c
 *
 *  @path   $(DSPLINK)\dsp\src\test\ReclaimTimeout
 *
 *  @desc   Implemetation of ReclaimTimeout.
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


/*  ----------------------------------- DSP/BIOS Headers                */
#include <std.h>
#include <sio.h>
#include <tsk.h>
#include <log.h>
#include <string.h>
#include <clk.h>

/*  ----------------------------------- DSP/BIOS LINK Headers           */
#include <platform.h>

/*  ----------------------------------- Testsuite Headers           */
#include <ReclaimTimeout.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   trace
 *
 *  @desc   This is the trace object used for writing debug strings
 *          and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;

/** ============================================================================
 *  @name   delayForSleep
 *
 *  @desc   This is the number of seconds for which the task should sleep.
 *  ============================================================================
 */
static Uns delayForSleep = 0 ;


/** ============================================================================
 *  @name   dataChnlbuf
 *
 *  @desc   This is the pointer to the buffer used for data transfer to
 *          GPP.
 *  ============================================================================
 */
static Char * dataChnlbuf = NULL ;


/** ============================================================================
 *  @name   inputControlChnlbuf
 *
 *  @desc   This is the pointer to the buffer used for receiving control
 *          messages from GPP.
 *  ============================================================================
 */
static Char * inputControlChnlbuf = NULL ;


/** ============================================================================
 *  @name   dataChnlId
 *
 *  @desc   This is the data channel identifier, sent by the Gpp side.
 *  ============================================================================
 */
static Uns dataChnlId = 0 ;


/** ============================================================================
 *  @name   bufSize
 *
 *  @desc   This is the size of the buffer to be transferred to GPP.
 *  ============================================================================
 */
static Uns bufSize = 0 ;


/** ============================================================================
 *  @name   dataChnlName
 *
 *  @desc   This is the name of the data stream which is opened for data
 *          communication with GPP.
 *  ============================================================================
 */
static char dataChnlName [14] ;


/** ============================================================================
 *  @name   dataChnl
 *
 *  @desc   This is the input stream which is opened for data communication
 *          with GPP.
 *  ============================================================================
 */
static SIO_Handle dataChnl = NULL ;


/** ============================================================================
 *  @name   inputControlChnlId
 *
 *  @desc   This is the input stream which is opened for receiving control mesg.
 *          from GPP.
 *  ============================================================================
 */
static SIO_Handle inputControlChnl = NULL ;


/** ============================================================================
 *  @func   ReclaimTimeout_Create
 *
 *  @desc   This function allocates the resources needed by the ReclaimTimeout
 *          test.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReclaimTimeout_Execute, ReclaimTimeout_Delete
 *  ============================================================================
 */
static
Uns
ReclaimTimeout_Create () ;


/** ============================================================================
 *  @func   ReclaimTimeout_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReclaimTimeout_Create, ReclaimTimeout_Delete
 *  ============================================================================
 */
static
Uns
ReclaimTimeout_Execute () ;


/** ============================================================================
 *  @func   ReclaimTimeout_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the ReclaimTimeout_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReclaimTimeout_Execute, ReclaimTimeout_Create
 *  ============================================================================
 */
static
Void
ReclaimTimeout_Delete () ;


/** ============================================================================
 *  @func   ReclaimTimeout
 *
 *  @desc   This function is the entry point to the test.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
ReclaimTimeout ()
{
    Uns  status = SYS_OK ;

    status = ReclaimTimeout_Create () ;

    if (status == SYS_OK) {
        status = ReclaimTimeout_Execute () ;
    }

    ReclaimTimeout_Delete () ;
}


/** ============================================================================
 *  @func   ReclaimTimeout_Create
 *
 *  @desc   This function allocates the resources needed by the ReclaimTimeout
 *          test.
 *
 *  @modif  dataChnl, inputControlChnl, dataChnlbuf, inputControlChnlbuf
 *  ============================================================================
 */
static
Uns
ReclaimTimeout_Create ()
{
    Uns       status                = SYS_OK ;
    SIO_Attrs dataChnlAttrs                  ;
    SIO_Attrs inputControlChnlAttrs          ;

    dataChnlAttrs         = SIO_ATTRS        ;
    dataChnlAttrs.nbufs   = 1                ;
    dataChnlAttrs.segid   = DSPLINK_SEGID    ;
    dataChnlAttrs.align   = DSPLINK_ALIGN    ;
    dataChnlAttrs.flush   = TRUE             ;
    dataChnlAttrs.model   = SIO_ISSUERECLAIM ;
    dataChnlAttrs.timeout = SYS_FOREVER      ;

    inputControlChnlAttrs         = SIO_ATTRS        ;
    inputControlChnlAttrs.nbufs   = 1                ;
    inputControlChnlAttrs.segid   = DSPLINK_SEGID    ;
    inputControlChnlAttrs.align   = DSPLINK_ALIGN    ;
    inputControlChnlAttrs.flush   = TRUE             ;
    inputControlChnlAttrs.model   = SIO_ISSUERECLAIM ;
    inputControlChnlAttrs.timeout = SYS_FOREVER      ;

    if ((dataChnlId % 2) == 0) {
        dataChnl = SIO_create (dataChnlName,
                               SIO_INPUT,
                               bufSize,
                               &dataChnlAttrs) ;
        if (dataChnl == NULL) {
           LOG_printf (&trace, "Could not create input data stream") ;
           status = SYS_EALLOC ;
        }
    }
    else {
        dataChnl = SIO_create (dataChnlName,
                               SIO_OUTPUT,
                               bufSize,
                               &dataChnlAttrs) ;
        if (dataChnl == NULL) {
           LOG_printf (&trace, "Could not create output data stream") ;
           status = SYS_EALLOC ;
        }
    }

    inputControlChnl = SIO_create (INPUT_CONTROL_CHNL_ID,
                                   SIO_INPUT,
                                   bufSize,
                                   &inputControlChnlAttrs) ;
    if (inputControlChnl == NULL) {
       LOG_printf (&trace, "Could not create input control stream") ;
       status = SYS_EALLOC ;
    }

    if (status == SYS_OK) {
       dataChnlbuf = MEM_calloc (DSPLINK_SEGID, bufSize, DSPLINK_ALIGN) ;
    }
    if (dataChnlbuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for data transfer") ;
       status = SYS_EALLOC ;
    }

    if (status == SYS_OK) {
       inputControlChnlbuf = MEM_calloc (DSPLINK_SEGID,
                                         bufSize,
                                         DSPLINK_ALIGN) ;
    }
    if (inputControlChnlbuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for input control") ;
       status = SYS_EALLOC ;
    }

    return status ;
}


/** ============================================================================
 *  @func   ReclaimTimeout_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
static
Uns
ReclaimTimeout_Execute ()
{
    Uns     status       = SYS_OK ;
    Int     nbytes       = -1 ;
    Char  * reclaimedBuf = NULL ;
    Arg     arg ;
    Uns     ticksForSleeping ;

    ticksForSleeping = ((CLK_countspms () / CLK_getprd ()) * delayForSleep) ;
    status = SIO_issue (inputControlChnl, inputControlChnlbuf, bufSize, 0) ;
    if (status == SYS_OK) {
        nbytes = SIO_reclaim (inputControlChnl, (Ptr *)&reclaimedBuf, &arg) ;
        if (nbytes < 0) {
            LOG_printf (&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                        status) ;
            status = -nbytes ;
        }
    }
    else {
        LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                    status) ;
    }

    /* Task sleeps to allow gpp side to execute CHNL_Reclaim with
     * finite timeout
     */
    TSK_sleep (ticksForSleeping) ;

    if (status == SYS_OK) {
        status = SIO_issue (dataChnl, dataChnlbuf, bufSize, 0) ;
    }

    if (status == SYS_OK) {
        nbytes = SIO_reclaim (dataChnl, (Ptr *)&reclaimedBuf, &arg) ;
        if (nbytes < 0) {
            LOG_printf (&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                        status) ;
            status = -nbytes ;
        }
    }
    else {
        LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                    status) ;
    }


    return status ;
}


/** ============================================================================
 *  @func   ReclaimTimeout_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the ReclaimTimeout_Create function.
 *
 *  @modif  dataChnl, inputControlChnl, dataChnlbuf, inputControlChnlbuf
 *  ============================================================================
 */
static
Void
ReclaimTimeout_Delete ()
{
    SIO_delete (dataChnl) ;

    dataChnl = NULL ;

    SIO_delete (inputControlChnl) ;

    inputControlChnl = NULL ;

    if (dataChnlbuf != NULL) {
        MEM_free (DSPLINK_SEGID, dataChnlbuf, bufSize) ;
        dataChnlbuf = NULL ;
    }

    if (inputControlChnlbuf != NULL) {
        MEM_free (DSPLINK_SEGID, inputControlChnlbuf, bufSize) ;
        inputControlChnlbuf = NULL ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the ReclaimTimeout task and receives the
 *          parameters from GPP side.
 *
 *  @modif  dataChnlId, iterations, bufSize, inTask
 *  ============================================================================
 */
Void
main (IN Int argc, IN Char * argv [])
{
    TSK_Handle inTask ;

    SYS_sprintf (dataChnlName, "/dio_dsplink") ;
    strcat (dataChnlName, argv [0]) ;

    dataChnlId = atoi (argv [0]) ;

    delayForSleep = atoi (argv [1]) ;

    bufSize = (atoi (argv [2]) / DSP_MAUSIZE) ;

    inTask = TSK_create ((Fxn)ReclaimTimeout, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
