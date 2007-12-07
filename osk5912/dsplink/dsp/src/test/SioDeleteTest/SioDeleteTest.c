/** ============================================================================
 *  @file   SioDeleteTest.c
 *
 *  @path   $ (PROJROOT)\dsp\src\test\SioDeleteTest
 *
 *  @desc   Implemetation of SioDeleteTest.
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
#include <SioDeleteTest.h>


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
 *  @desc   This is the mode in which data channel will be opened.
 *  ============================================================================
 */
static Uns dataChnlMode = 0 ;


/** ============================================================================
 *  @name   queueLength
 *
 *  @desc   This is the number of buffers which will be queued.
 *  ============================================================================
 */
static Uns queueLength = 0 ;


/** ============================================================================
 *  @name   dataBufSize
 *
 *  @desc   Size of the buffer used for issuing buffers on a data channel.
 *  ============================================================================
 */
static Uns dataBufSize = 0 ;


/** ============================================================================
 *  @name   isFlush
 *
 *  @desc   Argument received from GPP side which tells if the flush flag should
 *          be TRUE or FALSE on the data channel
 *  ============================================================================
 */
static Uns isFlush = 0 ;


/** ============================================================================
 *  @name   inputControlChnl
 *
 *  @desc   This is the input stream which is opened for receiving control mesg.
 *          from GPP.
 *  ============================================================================
 */
static SIO_Handle inputControlChnl = NULL ;


/** ============================================================================
 *  @name   outputControlChnl
 *
 *  @desc   This is the output stream which is opened for sending control mesg.
 *          to GPP.
 *  ============================================================================
 */
static SIO_Handle outputControlChnl = NULL ;


/** ============================================================================
 *  @name   dataChnlName
 *
 *  @desc   This is the name of the data stream which is opened for data
 *          communication with GPP.
 *  ============================================================================
 */
static char dataChnlName [20] ;

/** ============================================================================
 *  @name   dataChnl
 *
 *  @desc   This is the input stream which is opened for testing SIO_delete.
 *  ============================================================================
 */
static SIO_Handle dataChnl = NULL ;


/** ============================================================================
 *  @name   dataChnlbuf
 *
 *  @desc   This is the array of the buffers used for data transfer to
 *          GPP.
 *  ============================================================================
 */
static MdUns * dataChnlbuf [MAX_QUEUE] ;


/** ============================================================================
 *  @func   SioDeleteTest_Create
 *
 *  @desc   This function allocates the resources needed by the SioDeleteTest
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
 *  @see    SioDeleteTest_Execute, SioDeleteTest_Delete
 *  ============================================================================
 */
static
Uns
SioDeleteTest_Create () ;


/** ============================================================================
 *  @func   SioDeleteTest_Execute
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
 *  @see    SioDeleteTest_Create, SioDeleteTest_Delete
 *  ============================================================================
 */
static
Uns
SioDeleteTest_Execute () ;


/** ============================================================================
 *  @func   SioDeleteTest_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the SioDeleteTest_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SioDeleteTest_Execute, SioDeleteTest_Create
 *  ============================================================================
 */
static
Void
SioDeleteTest_Delete () ;


/** ============================================================================
 *  @func   SioDeleteTest
 *
 *  @desc   This function is the entry point to the test.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
SioDeleteTest ()
{
    Uns  status = SYS_OK ;

    status = SioDeleteTest_Create () ;

    if (status == SYS_OK) {
        status = SioDeleteTest_Execute () ;
    }

    SioDeleteTest_Delete () ;
}


/** ============================================================================
 *  @func   SioDeleteTest_Create
 *
 *  @desc   This function allocates the resources needed by the SioDeleteTest
 *          test.
 *
 *  @modif  dataChnl, inputControlChnl, outputControlChnl, inputControlChnlbuf,
 *          outputControlChnlbuf
 *  ============================================================================
 */
static
Uns
SioDeleteTest_Create ()
{
    Uns       status                 = SYS_OK ;
    Uns       i                      = 0      ;
    SIO_Attrs inputControlChnlAttrs           ;
    SIO_Attrs outputControlChnlAttrs          ;

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

    if (status == SYS_OK) {
        inputControlChnl = SIO_create (INPUT_CONTROL_CHNL_ID,
                                       SIO_INPUT,
                                       MESG_BUF_SIZE,
                                       &inputControlChnlAttrs) ;
        if (inputControlChnl == NULL) {
           LOG_printf (&trace, "Could not create input control stream") ;
           status = SYS_EALLOC ;
        }
    }

    if (status == SYS_OK) {
        outputControlChnl = SIO_create (OUTPUT_CONTROL_CHNL_ID,
                                       SIO_OUTPUT,
                                       MESG_BUF_SIZE,
                                       &outputControlChnlAttrs) ;
        if (outputControlChnl == NULL) {
           LOG_printf (&trace, "Could not create output control stream") ;
           status = SYS_EALLOC ;
        }
    }

    for (i = 0 ; (i < queueLength) && (status == SYS_OK) ; i++ ) {
       dataChnlbuf [i] = MEM_calloc (DSPLINK_SEGID, dataBufSize, DSPLINK_ALIGN);
        if (dataChnlbuf [i] == NULL) {
           LOG_printf (&trace, "Could not allocate buffer for data transfer") ;
           status = SYS_EALLOC ;
        }
    }

    if (status == SYS_OK) {
       inputControlChnlbuf = MEM_calloc (DSPLINK_SEGID,
                                         MESG_BUF_SIZE,
                                         DSPLINK_ALIGN);
    }
    if (inputControlChnlbuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for input control") ;
       status = SYS_EALLOC ;
    }

    if (status == SYS_OK) {
       outputControlChnlbuf = MEM_calloc (DSPLINK_SEGID,
                                          MESG_BUF_SIZE,
                                          DSPLINK_ALIGN);
    }
    if (outputControlChnlbuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for output control") ;
       status = SYS_EALLOC ;
    }

    return status ;
}


/** ============================================================================
 *  @func   SioDeleteTest_Execute
 *
 *  @desc   This function executes the 'SioDeleteTest' test.  It creates a data
 *          channel (with an ID specified by GPP) on which it queues
 *          'queueLength' buffers. Then the task receives a message on the
 *          input control channel to synchronizeitself with the GPP.Then
 *          it calls SIO_Delete. The status of the whole execution is passed
 *          to GPP.
 *
 *  @modif  dataChnl
 *  ============================================================================
 */
static
Uns
SioDeleteTest_Execute ()
{
    Uns       status       = SYS_OK ;
    Int       nMAUs        = -1     ;
    Arg       arg                   ;
    Int   *   reclaimedBuf          ;
    MdUns     i                     ;
    SIO_Attrs dataChnlAttrs         ;

    dataChnlAttrs         = SIO_ATTRS        ;
    dataChnlAttrs.nbufs   = queueLength      ;
    dataChnlAttrs.segid   = 0                ;
    dataChnlAttrs.align   = 0                ;
    dataChnlAttrs.model   = SIO_ISSUERECLAIM ;
    dataChnlAttrs.timeout = SYS_FOREVER      ;

    if (isFlush == 1) {
        dataChnlAttrs.flush   = TRUE ;
    }
    else {
        dataChnlAttrs.flush   = FALSE ;
    }

    if (dataChnlMode == 0) {
        dataChnl = SIO_create (dataChnlName,
                               SIO_INPUT,
                               dataBufSize,
                               &dataChnlAttrs) ;
        if (dataChnl == NULL) {
           LOG_printf (&trace, "Could not create input data stream") ;
           status = SYS_EALLOC ;
        }
    }
    else if (dataChnlMode == 1) {
        dataChnl = SIO_create (dataChnlName,
                               SIO_OUTPUT,
                               dataBufSize,
                               &dataChnlAttrs) ;
        if (dataChnl == NULL) {
           LOG_printf (&trace, "Could not create output data stream") ;
           status = SYS_EALLOC ;
        }
    }
    else {
        status = SYS_EINVAL ;
    }

    for (i = 0 ; (status == SYS_OK) && (i < queueLength) ; i++) {
        status = SIO_issue (dataChnl, dataChnlbuf [i], dataBufSize, 0) ;
        if (status != SYS_OK) {
            LOG_printf (&trace,
                        "DSP BIOS Link: Buffer issue failed %d\n",
                        status) ;
        }
    }
    if (status == SYS_OK) {
        status = SIO_issue (inputControlChnl,
                            inputControlChnlbuf,
                            MESG_BUF_SIZE,
                            0) ;
        if (status != SYS_OK) {
            LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                        status) ;
        }
    }
    if (status == SYS_OK) {
        nMAUs = SIO_reclaim (inputControlChnl, (Ptr *)&reclaimedBuf, &arg) ;
        if (nMAUs < 0) {
            LOG_printf (&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                        status) ;
            status = -nMAUs ;
        }
    }
    if ((status == SYS_OK)) {
        status = SIO_delete (dataChnl) ;
        if (status != SYS_OK) {
            LOG_printf (&trace,
                        "DSP BIOS Link: Call to SIO_delete failed %d\n",
                        status) ;
        }
    }
    if ((status == SYS_OK)) {
        outputControlChnlbuf [0] = 1 ;
    }
    else {
        outputControlChnlbuf [0] = 0 ;
    }

    /*
     * Even though status is not SYS_OK control message is sent to the GPP to
     * intimate the DSP execution status.
     */
    status = SIO_issue (outputControlChnl,
                        outputControlChnlbuf,
                        MESG_BUF_SIZE,
                        0) ;
    if (status != SYS_OK) {
        LOG_printf (&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
        status) ;
    }
    if (status == SYS_OK) {
        nMAUs = SIO_reclaim (outputControlChnl, (Ptr *)&reclaimedBuf, &arg) ;
        if (nMAUs < 0) {
            LOG_printf (&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                        status) ;
            status = -nMAUs ;
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   SioDeleteTest_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the SioDeleteTest_Create function.
 *
 *  @modif  inputControlChnl, outputControlChnl, inputControlChnlbuf,
 *          outputControlChnlbuf
 *  ============================================================================
 */
static
Void
SioDeleteTest_Delete ()
{
    Uns i = 0 ;

    SIO_delete (inputControlChnl) ;

    inputControlChnl = NULL ;

    SIO_delete (outputControlChnl) ;

    outputControlChnl = NULL ;

    for (i = 0 ; (i < queueLength) && (dataChnlbuf [i] != NULL) ; i++) {
        MEM_free (DSPLINK_SEGID, dataChnlbuf [i], MESG_BUF_SIZE) ;
        dataChnlbuf [i] = NULL ;
    }

    if (inputControlChnlbuf != NULL) {
        MEM_free (DSPLINK_SEGID, inputControlChnlbuf, MESG_BUF_SIZE) ;
        inputControlChnlbuf = NULL ;
    }

    if (outputControlChnlbuf != NULL) {
        MEM_free (DSPLINK_SEGID, outputControlChnlbuf, MESG_BUF_SIZE) ;
        outputControlChnlbuf = NULL ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the SioDeleteTest task and receives the
 *          parameters from GPP side.
 *
 *  @modif  dataChnlName, dataChnlMode, dataBufSize, isFlush, inTask
 *  ============================================================================
 */
Void
main (Int argc, Char * argv [])
{
    TSK_Handle inTask ;

    queueLength  = atoi (argv [0]) ;

    SYS_sprintf (dataChnlName, "/dio_dsplink") ;
    strcat (dataChnlName, argv [1]) ;

    dataChnlMode = atoi (argv [2]) ;

    dataBufSize  = (atoi (argv [3]) / DSP_MAUSIZE) ;

    isFlush   = atoi (argv [4]) ;

    inTask = TSK_create ((Fxn)SioDeleteTest, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
