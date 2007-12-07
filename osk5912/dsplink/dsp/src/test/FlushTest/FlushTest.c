/** ============================================================================
 *  @file   FlushTest.c
 *
 *  @path   $(DSPLINK)\dsp\src\test\FlushTest
 *
 *  @desc   Implemetation of FlushTest.
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

/*  ----------------------------------- DSP/BIOS LINK Headers           */
#include <platform.h>

/*  ----------------------------------- Testsuite Headers           */
#include <FlushTest.h>


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
 *  @name   dataChnlbuf
 *
 *  @desc   This is the pointer to the buffer used for data transfer to
 *          GPP.
 *  ============================================================================
 */
static MdUns * dataChnlbuf = NULL ;


/** ============================================================================
 *  @name   inputControlChnlbuf
 *
 *  @desc   This is the pointer to the buffer used for receiving control
 *          messages from GPP.
 *  ============================================================================
 */
static MdUns * inputControlChnlbuf = NULL ;


/** ============================================================================
 *  @name   outputControlChnlbuf
 *
 *  @desc   This is the pointer to the buffer used for sending control messages
 *          to GPP.
 *  ============================================================================
 */
static MdUns * outputControlChnlbuf = NULL ;


/** ============================================================================
 *  @name   dataChnlMode
 *
 *  @desc   This is the mode in which data channel is to be opened.
 *  ============================================================================
 */
static Uns dataChnlMode = 0 ;


/** ============================================================================
 *  @name   bufSize
 *
 *  @desc   This is the size of the buffer to be trasferred to GPP.
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
 *  @name   outputControlChnlId
 *
 *  @desc   This is the output stream which is opened for sending control mesg.
 *          to GPP.
 *  ============================================================================
 */
static SIO_Handle outputControlChnl = NULL ;


/** ============================================================================
 *  @func   FlushTest_Create
 *
 *  @desc   This function allocates the resources needed by the ReceiveBuf test.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    FlushTest_Execute, FlushTest_Delete
 *  ============================================================================
 */
static
Uns
FlushTest_Create () ;


/** ============================================================================
 *  @func   FlushTest_Execute
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
 *  @see    FlushTest_Create, FlushTest_Delete
 *  ============================================================================
 */
static
Uns
FlushTest_Execute () ;


/** ============================================================================
 *  @func   FlushTest_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the FlushTest_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    FlushTest_Execute, FlushTest_Create
 *  ============================================================================
 */
static
Void
FlushTest_Delete () ;


/** ============================================================================
 *  @func   FlushTest
 *
 *  @desc   This function is the entry point to the test.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
FlushTest ()
{
    Uns status = SYS_OK ;

    status = FlushTest_Create () ;

    if (status == SYS_OK) {
        status = FlushTest_Execute () ;
    }

    FlushTest_Delete () ;
}


/** ============================================================================
 *  @func   FlushTest_Create
 *
 *  @desc   This function allocates the resources needed by the FlushTest test.
 *
 *  @modif  dataChnl, inputControlChnl, outputControlChnl, dataChnlbuf,
 *          inputControlChnlbuf, outputControlChnlbuf
 *  ============================================================================
 */
static
Uns
FlushTest_Create ()
{
    Uns       status        = SYS_OK ;
    SIO_Attrs dataChnlAttrs          ;
    SIO_Attrs inputControlChnlAttrs  ;
    SIO_Attrs outputControlChnlAttrs ;

    dataChnlAttrs         = SIO_ATTRS        ;
    dataChnlAttrs.nbufs   = 1                ;
    dataChnlAttrs.segid   = 0                ;
    dataChnlAttrs.align   = 0                ;
    dataChnlAttrs.flush   = TRUE             ;
    dataChnlAttrs.model   = SIO_ISSUERECLAIM ;
    dataChnlAttrs.timeout = 500              ;

    inputControlChnlAttrs         = SIO_ATTRS        ;
    inputControlChnlAttrs.nbufs   = 1                ;
    inputControlChnlAttrs.segid   = DSPLINK_SEGID    ;
    inputControlChnlAttrs.align   = DSPLINK_ALIGN    ;
    inputControlChnlAttrs.flush   = TRUE             ;
    inputControlChnlAttrs.model   = SIO_ISSUERECLAIM ;
    inputControlChnlAttrs.timeout = SYS_FOREVER      ;

    outputControlChnlAttrs         = SIO_ATTRS        ;
    outputControlChnlAttrs.nbufs   = 1                ;
    outputControlChnlAttrs.segid   = DSPLINK_SEGID    ;
    outputControlChnlAttrs.align   = DSPLINK_ALIGN    ;
    outputControlChnlAttrs.flush   = TRUE             ;
    outputControlChnlAttrs.model   = SIO_ISSUERECLAIM ;
    outputControlChnlAttrs.timeout = SYS_FOREVER      ;

    if (dataChnlMode == 0) {
        dataChnl = SIO_create (dataChnlName,
                               SIO_INPUT,
                               bufSize,
                               &dataChnlAttrs) ;
        if (dataChnl == NULL) {
           LOG_printf (&trace, "Could not create input data stream") ;
           status = SYS_EALLOC ;
        }
    }
    else if (dataChnlMode == 1) {
        dataChnl = SIO_create (dataChnlName,
                               SIO_OUTPUT,
                               bufSize,
                               &dataChnlAttrs) ;
        if (dataChnl == NULL) {
           LOG_printf (&trace, "Could not create output data stream") ;
           status = SYS_EALLOC ;
        }
    }
    else {
        status = SYS_EALLOC ;
    }

    if (status == SYS_OK) {
        inputControlChnl = SIO_create (INPUT_CONTROL_CHNL_ID,
                                       SIO_INPUT,
                                       bufSize,
                                       &inputControlChnlAttrs) ;
        if (inputControlChnl == NULL) {
           LOG_printf (&trace, "Could not create input control stream") ;
           status = SYS_EALLOC ;
        }
    }

    if (status == SYS_OK) {
        outputControlChnl = SIO_create (OUTPUT_CONTROL_CHNL_ID,
                                       SIO_OUTPUT,
                                       bufSize,
                                       &outputControlChnlAttrs) ;
        if (outputControlChnl == NULL) {
           LOG_printf (&trace, "Could not create output control stream") ;
           status = SYS_EALLOC ;
        }
    }

    if (status == SYS_OK) {
       dataChnlbuf = MEM_calloc (DSPLINK_SEGID, bufSize, DSPLINK_ALIGN);
    }
    if (dataChnlbuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for data transfer") ;
       status = SYS_EALLOC ;
    }

    if (status == SYS_OK) {
       inputControlChnlbuf = MEM_calloc (DSPLINK_SEGID, bufSize, DSPLINK_ALIGN);
    }
    if (inputControlChnlbuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for input control") ;
       status = SYS_EALLOC ;
    }

    if (status == SYS_OK) {
       outputControlChnlbuf = MEM_calloc (DSPLINK_SEGID, bufSize, DSPLINK_ALIGN);
    }
    if (outputControlChnlbuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for output control") ;
       status = SYS_EALLOC ;
    }

    return status ;
}


/** ============================================================================
 *  @func   FlushTest_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
static
Uns
FlushTest_Execute ()
{
    Uns     status       = SYS_OK ;
    Int     nMAUs        = -1     ;
    Int   * reclaimedBuf = NULL   ;
    Arg     arg                   ;
    MdUns   bytesReceived         ;

    status = SIO_issue (inputControlChnl, inputControlChnlbuf, bufSize, 0) ;
    if (status == SYS_OK) {
        nMAUs = SIO_reclaim (inputControlChnl, (Ptr *)&reclaimedBuf, &arg) ;
        if (nMAUs < 0) {
            status = -nMAUs ;
            LOG_printf (&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                        status);

        }
    }
    else {
        LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                    status);
    }

    status = SIO_issue (dataChnl, dataChnlbuf, bufSize, 0) ;
    if (status == SYS_OK) {
        nMAUs = SIO_reclaim (dataChnl, (Ptr *)&reclaimedBuf, &arg);
        if (nMAUs < 0) {
            status = -nMAUs ;
            if (status != SYS_ETIMEOUT) {
                LOG_printf (&trace,
                            "DSP BIOS Link: Buffer reclaim failed %d\n",
                            status);
            }
        }
    }
    else {
        LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                    status) ;
    }

    if ((status == SYS_OK) || (status == SYS_ETIMEOUT)) {
        if (status == SYS_ETIMEOUT) {
            bytesReceived = 0 ;
        }
        else {
            bytesReceived = nMAUs ;
        }
        outputControlChnlbuf [0] = bytesReceived ;
        status = SIO_issue (outputControlChnl,
                            outputControlChnlbuf,
                            bufSize,
                            0) ;
    }
    if (status == SYS_OK) {
        nMAUs = SIO_reclaim (outputControlChnl, (Ptr *)&reclaimedBuf, &arg) ;
        if (nMAUs < 0) {
            status = -nMAUs ;
            LOG_printf (&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                        status) ;
        }
    }
    else {
        LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                    status) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   FlushTest_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the FlushTest_Create function.
 *
 *  @modif  dataChnl, inputControlChnl, outputControlChnl, dataChnlbuf,
 *          inputControlChnlbuf, outputControlChnlbuf
 *  ============================================================================
 */
static
Void
FlushTest_Delete ()
{
    SIO_delete (dataChnl);

    dataChnl = NULL ;

    SIO_delete (inputControlChnl);

    inputControlChnl = NULL ;

    SIO_delete (outputControlChnl);

    outputControlChnl = NULL ;

    if (dataChnlbuf != NULL) {
        MEM_free (DSPLINK_SEGID, dataChnlbuf, bufSize) ;
        dataChnlbuf = NULL ;
    }

    if (inputControlChnlbuf != NULL) {
        MEM_free (DSPLINK_SEGID, inputControlChnlbuf, bufSize) ;
        inputControlChnlbuf = NULL ;
    }

    if (outputControlChnlbuf != NULL) {
        MEM_free (DSPLINK_SEGID, outputControlChnlbuf, bufSize) ;
        outputControlChnlbuf = NULL ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the FlushTest task and receives the
 *          parameters from GPP side.
 *
 *  @modif  dataChnlId, bufSize, inTask, dataChnlMode
 *  ============================================================================
 */
Void
main (IN Int argc, IN Char * argv [])
{
    TSK_Handle inTask ;

    SYS_sprintf (dataChnlName, "/dio_dsplink") ;
    strcat (dataChnlName, argv [0]) ;

    dataChnlMode = atoi (argv [1]) ;

    bufSize = (atoi (argv [2]) / DSP_MAUSIZE) ;

    inTask = TSK_create ((Fxn)FlushTest, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
