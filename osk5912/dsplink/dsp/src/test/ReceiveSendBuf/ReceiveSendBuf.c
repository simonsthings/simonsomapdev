/** ============================================================================
 *  @file   ReceiveSendBuf.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\ReceiveSendBuf
 *
 *  @desc   Implemetation of ReceiveSendBuf.
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

/*  ----------------------------------- DSP/BIOS LINK Headers           */
#include <platform.h>

/*  ----------------------------------- Testsuite Headers           */
#include <ReceiveSendBuf.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   bufSize
 *
 *  @desc   Size of the buffer to be used.
 *  ============================================================================
 */
Uns bufSize ;


/** ============================================================================
 *  @name   modeOfOperation
 *
 *  @desc   Mode in which the test is running.
 *          1 - Send  Buffers.
 *          2 - Receive Buffers.
 *          3 - Receive and then Send Buffers.
 *  ============================================================================
 */
Uns modeOfOperation ;


/** ============================================================================
 *  @name   trace
 *
 *  @desc   This is the trace object used for writing debug strings
 *          and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;


/** ============================================================================
 *  @name   input
 *
 *  @desc   This is the input stream which is opened for receiving data
 *          from GPP.
 *  ============================================================================
 */
SIO_Handle input ;


/** ============================================================================
 *  @name   output
 *
 *  @desc   This is the input stream which is opened for sending data
 *          to GPP.
 *  ============================================================================
 */
SIO_Handle output ;


/** ============================================================================
 *  @name   buf
 *
 *  @desc   Pointer to the buffer used for receiving data from GPP.
 *  ============================================================================
 */
Char * buf = NULL ;


/** ============================================================================
 *  @func   ReceiveSendBuf_Create
 *
 *  @desc   This function allocates the resourses needed by the ReceiveBuf test.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReceiveSendBuf_Execute, ReceiveSendBuf_Delete
 *  ============================================================================
 */
Uns
ReceiveSendBuf_Create () ;


/** ============================================================================
 *  @func   ReceiveSendBuf_Execute
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
 *  @see    ReceiveSendBuf_Delete, ReceiveSendBuf_Create
 *  ============================================================================
 */
Uns
ReceiveSendBuf_Execute () ;


/** ============================================================================
 *  @func   ReceiveSendBuf_Delete
 *
 *  @desc   This function deallocates the resourses allocated by
 *          the ReceiveBuf_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ReceiveSendBuf_Execute, ReceiveSendBuf_Create
 *  ============================================================================
 */
Void
ReceiveSendBuf_Delete () ;


/** ============================================================================
 *  @func   ReceiveSendBuf
 *
 *  @desc   This function communicates with gpp side application and returns
 *          the same buffer which it received.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
ReceiveSendBuf ()
{
    Uns status = SYS_OK ;

    status = ReceiveSendBuf_Create () ;

    if (status == SYS_OK) {
        status = ReceiveSendBuf_Execute () ;
    }

    ReceiveSendBuf_Delete () ;
}


/** ============================================================================
 *  @func   ReceiveSendBuf_Create
 *
 *  @desc   This function allocates the resourses needed by the ReceiveSendBuf
 *          test.
 *
 *  @modif  input, output
 *  ============================================================================
 */
Uns
ReceiveSendBuf_Create ()
{
    Uns       status = SYS_OK ;
    SIO_Attrs attrs           ;


    attrs         = SIO_ATTRS        ;
    attrs.nbufs   = 5                ;
    attrs.segid   = DSPLINK_SEGID    ;
    attrs.align   = DSPLINK_ALIGN    ;
    attrs.flush   = TRUE             ;
    attrs.model   = SIO_ISSUERECLAIM ;
    attrs.timeout = SYS_FOREVER      ;

    if ((modeOfOperation == 2) || (modeOfOperation == 3)) {
        input = SIO_create (INPUTDATACHANNEL, SIO_INPUT, bufSize, &attrs) ;
        if (input == NULL) {
            LOG_printf (&trace, "Could not create input stream") ;
            status = SYS_EALLOC ;
        }
    }

    if ((modeOfOperation == 1) || (modeOfOperation == 3)) {
        if (status == SYS_OK) {
            output = SIO_create (OUTPUTDATACHANNEL,
                                 SIO_OUTPUT,
                                 bufSize,
                                 &attrs) ;
            if (output == NULL) {
                LOG_printf (&trace, "Could not create output stream") ;
                status = SYS_EALLOC ;
            }
        }
    }

    if (status == SYS_OK) {
        buf = MEM_calloc(DSPLINK_SEGID, bufSize, DSPLINK_ALIGN);
    }

    if (buf == NULL) {
        LOG_printf (&trace, "Could not allocate buffer for data transfer") ;
        status = SYS_EALLOC ;
    }

    return status ;

}


/** ============================================================================
 *  @func   ReceiveSendBuf_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
Uns
ReceiveSendBuf_Execute ()
{
    Uns         status       = SYS_OK ;
    Int         nbytes       = -1     ;
    Arg         arg                   ;
    Int       * reclaimedBuf          ;

    while (1) {
        if ((modeOfOperation == 2) || (modeOfOperation == 3)) {
            status = SIO_issue (input, buf, bufSize, 0) ;
            if (status == SYS_OK) {
                nbytes = SIO_reclaim (input, (Ptr *)&reclaimedBuf, &arg);
                if (nbytes < 0) {
                    LOG_printf(&trace, "DSP BIOS Link: "
                               "Buffer reclaim failed %d\n",
                               status);
                    status = SYS_EALLOC ;
                }
            }
            else {
                LOG_printf(&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                    status);
            }
        }

        if ((modeOfOperation == 1) || (modeOfOperation == 3)) {
            if (status == SYS_OK) {
                status = SIO_issue (output, buf, bufSize, 0) ;
            }
            if (status == SYS_OK) {
                nbytes = SIO_reclaim (output, (Ptr *)&reclaimedBuf, &arg);
                if (nbytes < 0) {
                    LOG_printf(&trace, "DSP BIOS Link: "
                               "Buffer reclaim failed %d\n",
                               status);
                    status = SYS_EALLOC ;
                }
            }
            else {
                LOG_printf(&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                           status);
            }
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   ReceiveSendBuf_Delete
 *
 *  @desc   This function deallocates the resourses allocated by
 *          the ReceiveSendBuf_Create function.
 *
 *  @modif  input
 *  ============================================================================
 */
Void
ReceiveSendBuf_Delete ()
{
    if ((modeOfOperation == 2) || (modeOfOperation == 3)) {
        SIO_delete (input)  ;
        input = NULL ;
    }

    if ((modeOfOperation == 1) || (modeOfOperation == 3)) {
        SIO_delete (output) ;
        output = NULL ;
    }

    if (buf != NULL) {
        MEM_free (DSPLINK_SEGID, buf, bufSize) ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the ReceiveSendBuf task.
 *
 *  @modif  bufSize, modeOfOperation
 *  ============================================================================
 */
Void main (IN Int argc, IN Char * argv [])
{
    TSK_Handle inTask ;

    bufSize = atoi (argv[0]) / DSP_MAUSIZE ;
    /*
     * modeOfOperation defines which mode the test is running in.
     * 1 - Send  Buffers.
     * 2 - Receive Buffers.
     * 3 - Receive and then Send Buffers.
     */
    modeOfOperation = atoi (argv[1]) ;

    inTask = TSK_create ((Fxn)ReceiveSendBuf, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
