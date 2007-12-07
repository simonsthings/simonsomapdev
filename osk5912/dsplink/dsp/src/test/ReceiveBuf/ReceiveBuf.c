/** ============================================================================
 *  @file   ReceiveBuf.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\ReceiveBuf
 *
 *  @desc   Implemetation of ReceiveBuf.
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
#include <ReceiveBuf.h>


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
 *  @name   input
 *
 *  @desc   This is the input stream which is opened for receiving data
 *          from GPP.
 *  ============================================================================
 */
SIO_Handle input ;


/** ============================================================================
 *  @name   buf
 *
 *  @desc   This is the pointer to the buffer used for data transfer to
 *          GPP.
 *  ============================================================================
 */
Uns * buf = NULL ;


/** ============================================================================
 *  @func   ReceiveBuf_Create
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
 *  @see    ReceiveBuf_Execute, ReceiveBuf_Delete
 *  ============================================================================
 */
Uns
ReceiveBuf_Create () ;


/** ============================================================================
 *  @func   ReceiveBuf_Execute
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
 *  @see    ReceiveBuf_Delete, ReceiveBuf_Create
 *  ============================================================================
 */
Uns
ReceiveBuf_Execute () ;


/** ============================================================================
 *  @func   ReceiveBuf_Delete
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
 *  @see    ReceiveBuf_Execute, ReceiveBuf_Create
 *  ============================================================================
 */
Void
ReceiveBuf_Delete () ;


/** ============================================================================
 *  @func   ReceiveBuf
 *
 *  @desc   This function communicates with gpp side application and receives
 *          a data buffer from GPP side.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
ReceiveBuf ()
{
    Uns status = SYS_OK ;

    status = ReceiveBuf_Create () ;

    if (status == SYS_OK) {
        status = ReceiveBuf_Execute () ;
    }

    ReceiveBuf_Delete () ;
}


/** ============================================================================
 *  @func   ReceiveBuf_Create
 *
 *  @desc   This function allocates the resourses needed by the ReceiveBuf test.
 *
 *  @modif  input
 *  ============================================================================
 */
Uns
ReceiveBuf_Create ()
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


   input = SIO_create (INPUTDATACHANNEL, SIO_INPUT, BUFSIZE, &attrs) ;
   if (input == NULL) {
       LOG_printf (&trace, "Could not create input stream") ;
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
 *  @func   ReceiveBuf_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
Uns
ReceiveBuf_Execute ()
{
    Uns         status     = SYS_OK ;
    Arg         arg                 ;
    Int       * reclaimedBuf        ;
    Int         nbytes = -1         ;

    while (1) {
        status = SIO_issue (input, buf, BUFSIZE, 0) ;
        if (status == SYS_OK) {
            nbytes = SIO_reclaim (input, (Ptr *)&reclaimedBuf, &arg);
            if (nbytes < 0) {
                status = -nbytes ;
                LOG_printf(&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                           status);
            }
        }
        else {
            LOG_printf(&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                       status);
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   ReceiveBuf_Delete
 *
 *  @desc   This function deallocates the resourses allocated by
 *          the ReceiveBuf_Create function.
 *
 *  @modif  input
 *  ============================================================================
 */
Void
ReceiveBuf_Delete ()
{
    SIO_delete (input);

    input = NULL ;

    if (buf != NULL) {
        MEM_free (DSPLINK_SEGID ,buf, BUFSIZE) ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the ReceiveBuf task.
 *
 *  @modif  None
 *  ============================================================================
 */
Void main (IN Int argc, IN Char * argv [])
{
    TSK_Handle inTask ;

    inTask = TSK_create ((Fxn)ReceiveBuf, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
