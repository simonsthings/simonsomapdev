/** ============================================================================
 *  @file   EndOfStream.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\EndOfStream
 *
 *  @desc   Implemetation of EndOfStream.
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
#include <EndOfStream.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   BUFSIZE
 *
 *  @desc   Size of the buffer to be used.
 *  ============================================================================
 */
#define BUFSIZE (10 / DSP_MAUSIZE)


/** ============================================================================
 *  @name   trace
 *
 *  @desc   This is the trace object used for writing debug strings
 *          and error messages.
 *  ============================================================================
 */
extern LOG_Obj trace ;


/** ============================================================================
 *  @name   buf
 *
 *  @desc   This is the pointer to the buffer used for data transfer to
 *          GPP.
 *  ============================================================================
 */
Uns * buf = NULL ;


/** ============================================================================
 *  @name   output
 *
 *  @desc   This is the input stream which is opened for sending data
 *          to GPP.
 *  ============================================================================
 */
SIO_Handle output = NULL ;


/** ============================================================================
 *  @func   EndOfStream_Create
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
 *  @see    EndOfStream_Execute, EndOfStream_Delete
 *  ============================================================================
 */
Uns
EndOfStream_Create () ;


/** ============================================================================
 *  @func   EndOfStream_Execute
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
 *  @see    EndOfStream_Create, EndOfStream_Delete
 *  ============================================================================
 */
Uns
EndOfStream_Execute () ;


/** ============================================================================
 *  @func   EndOfStream_Delete
 *
 *  @desc   This function deallocates the resourses allocated by
 *          the EndOfStream_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    EndOfStream_Execute, EndOfStream_Create
 *  ============================================================================
 */
Void
EndOfStream_Delete () ;


/** ============================================================================
 *  @func   EndOfStream
 *
 *  @desc   This function communicates with gpp side application and sends
 *          a data buffer from GPP side.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
EndOfStream ()
{
    Uns status = SYS_OK ;

    status = EndOfStream_Create () ;

    if (status == SYS_OK) {
        status = EndOfStream_Execute () ;
    }

    EndOfStream_Delete () ;
}


/** ============================================================================
 *  @func   EndOfStream_Create
 *
 *  @desc   This function allocates the resourses needed by the ReceiveBuf test.
 *
 *  @modif  output
 *  ============================================================================
 */
Uns
EndOfStream_Create ()
{
    Uns       status = SYS_OK ;
    SIO_Attrs attrs         ;

    attrs         = SIO_ATTRS        ;
    attrs.nbufs   = 1                ;
    attrs.segid   = DSPLINK_SEGID    ;
    attrs.align   = DSPLINK_ALIGN    ;
    attrs.flush   = TRUE             ;
    attrs.model   = SIO_ISSUERECLAIM ;
    attrs.timeout = SYS_FOREVER      ;


   output = SIO_create (OUTPUTDATACHANNEL, SIO_OUTPUT, BUFSIZE, &attrs) ;
   if (output == NULL) {
       LOG_printf (&trace, "Could not create output stream") ;
       status = SYS_EALLOC ;
   }

   if (status == SYS_OK) {
       buf = MEM_calloc(DSPLINK_SEGID, BUFSIZE, DSPLINK_ALIGN);
   }

   if (buf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for data transfer") ;
       status = SYS_EALLOC ;
   }

   return status ;
}


/** ============================================================================
 *  @func   EndOfStream_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
Uns
EndOfStream_Execute ()
{
    Uns         status       = SYS_OK ;
    Int         nbytes       = -1     ;
    Arg         arg                   ;
    Int       * reclaimedBuf          ;

    status = SIO_issue (output, buf, BUFSIZE, 0) ;
    if (status == SYS_OK) {
        nbytes = SIO_reclaim (output, (Ptr *)&reclaimedBuf, &arg);
        if (nbytes < 0) {
            LOG_printf(&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                       status);
            status = SYS_EALLOC ;
        }
    }
    else {
        LOG_printf(&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                   status);
    }

    /*
     * Sending EndOfStream
     */

    status = SIO_issue (output, buf, 0, 0) ;
    if (status == SYS_OK) {
        nbytes = SIO_reclaim (output, (Ptr *)&reclaimedBuf, &arg);
        if (nbytes < 0) {
            LOG_printf(&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                       status);
            status = SYS_EALLOC ;
        }
    }
    else {
        LOG_printf(&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                   status);
    }

    /*
     * Sending EndOfStream again
     */

    status = SIO_issue (output, buf, 0, 0) ;
    if (status == SYS_OK) {
        nbytes = SIO_reclaim (output, (Ptr *)&reclaimedBuf, &arg);
        if (nbytes < 0) {
            LOG_printf(&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                       status);
            status = SYS_EALLOC ;
        }
    }
    else {
        LOG_printf(&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                   status);
    }


    return status ;
}


/** ============================================================================
 *  @func   EndOfStream_Delete
 *
 *  @desc   This function deallocates the resourses allocated by
 *          the ReceiveBuf_Create function.
 *
 *  @modif  output
 *  ============================================================================
 */
Void
EndOfStream_Delete ()
{
    SIO_delete (output);

    output = NULL ;

    if (buf != NULL) {
        MEM_free (DSPLINK_SEGID, buf, BUFSIZE) ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the EndOfStream task.
 *
 *  @modif  None
 *  ============================================================================
 */
Void main (IN Int argc, IN Char * argv [])
{
    TSK_Handle inTask ;

    inTask = TSK_create ((Fxn)EndOfStream, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
