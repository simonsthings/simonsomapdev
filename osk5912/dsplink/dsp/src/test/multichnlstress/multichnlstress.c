/** ============================================================================
 *  @file   multichnlstress.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\multichnlstress
 *
 *  @desc   Implemetation of multichnlstress.
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
#include <multichnlstress.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   DRIVER_NAME_LENGTH
 *
 *  @desc   Length of the driver name.
 *  ============================================================================
 */
#define DRIVER_NAME_LENGTH 15

/** ============================================================================
 *  @name   bufSize
 *
 *  @desc   Size of the buffer to be used.
 *  ============================================================================
 */
static Uns bufSize ;

/** ============================================================================
 *  @name   numChirps
 *
 *  @desc   This is the number of times Issue Reclaim will be done on the
 *          same channel in one iteration.
 *  ============================================================================
 */
static Uns numChirps = 0 ;

/** ============================================================================
 *  @name   iterations
 *
 *  @desc   This is the number of times the sequence of Issue-Reclaim will
 *          be done.
 *  ============================================================================
 */
static Uns iterations = 0 ;

/** ============================================================================
 *  @name   MAX_CHANNELS
 *
 *  @desc   Maximum number of channels supported by DSP/BIOS LINK.
 *  ============================================================================
 */
#define MAX_CHANNELS 16

/** ============================================================================
 *  @name   numChannels
 *
 *  @desc   Number of channels on which data transfer is being done.
 *  ============================================================================
 */
static Uns numChannels = 0 ;


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
SIO_Handle input [MAX_CHANNELS / 2] ;


/** ============================================================================
 *  @name   output
 *
 *  @desc   This is the output  stream which is opened for sending data
 *          to GPP.
 *  ============================================================================
 */
SIO_Handle output [MAX_CHANNELS / 2] ;


/** ============================================================================
 *  @name   buf
 *
 *  @desc   This is the pointer to the buffer used for data transfer to
 *          GPP.
 *  ============================================================================
 */
Char * buf = NULL ;


/** ============================================================================
 *  @func   multichnlstress_Create
 *
 *  @desc   This function allocates the resources needed by the multichnlstress
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
 *  @see    multichnlstress_Execute, multichnlstress_Delete
 *  ============================================================================
 */
Uns
multichnlstress_Create () ;


/** ============================================================================
 *  @func   multichnlstress_Execute
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
 *  @see    multichnlstress_Delete, multichnlstress_Create
 *  ============================================================================
 */
Uns
multichnlstress_Execute () ;


/** ============================================================================
 *  @func   multichnlstress_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the multichnlstress_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    multichnlstress_Execute, multichnlstress_Create
 *  ============================================================================
 */
Void
multichnlstress_Delete () ;


/** ============================================================================
 *  @func   multichnlstress
 *
 *  @desc   This function communicates with gpp side application and receives
 *          a data buffer from GPP side.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
multichnlstress ()
{
    Uns status = SYS_OK ;

    status = multichnlstress_Create () ;

    if (status == SYS_OK) {
        status = multichnlstress_Execute () ;
    }

    multichnlstress_Delete () ;
}


/** ============================================================================
 *  @func   multichnlstress_Create
 *
 *  @desc   This function allocates the resources needed by the multichnlstress
 *          test.
 *
 *  @modif  input, output
 *  ============================================================================
 */
Uns
multichnlstress_Create ()
{
    Uns       status       = SYS_OK ;
    SIO_Attrs attrs                 ;
    Int       i                     ;
    Int       index                 ;
    Char      chnlName [3]          ;
    Char      channels [MAX_CHANNELS][DRIVER_NAME_LENGTH] ;

    attrs         = SIO_ATTRS        ;
    attrs.nbufs   = 1                ;
    attrs.segid   = DSPLINK_SEGID    ;
    attrs.align   = DSPLINK_ALIGN    ;
    attrs.flush   = TRUE             ;
    attrs.model   = SIO_ISSUERECLAIM ;
    attrs.timeout = SYS_FOREVER      ;

    for (i = 0 ; (i < numChannels) && (status == SYS_OK) ; i++ ) {
        SYS_sprintf (channels [i], "/dio_dsplink") ;
        SYS_sprintf (chnlName, "%d", i) ;
        strcat (channels [i], chnlName) ;
        index = (Int)(i / 2) ;
        if ((i % 2) == 0) {
            input [index] = SIO_create (channels [i],
                                        SIO_INPUT,
                                        bufSize,
                                        &attrs) ;
            if (input [index] == NULL) {
               status = SYS_EALLOC ;
               LOG_printf (&trace, "Could not create input stream",
                           status) ;
            }
        }
        else {
           output [index] = SIO_create (channels [i],
                                        SIO_OUTPUT,
                                        bufSize,
                                        &attrs) ;
           if (output [index] == NULL) {
               status = SYS_EALLOC ;
               LOG_printf (&trace, "Could not create output stream",
                           status) ;

           }
        }
    }

    if (status == SYS_OK) {
        buf = MEM_calloc(DSPLINK_SEGID, bufSize, DSPLINK_ALIGN);
    }

    if (buf == NULL) {
       status = SYS_EALLOC ;
       LOG_printf (&trace, "Could not allocate buffer for data transfer",
                            status) ;

    }

    return status ;
}


/** ============================================================================
 *  @func   multichnlstress_Execute
 *
 *  @desc   This function does the actual data transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
Uns
multichnlstress_Execute ()
{
    Uns   status     = SYS_OK ;
    Int   nbytes     = -1     ;
    Arg   arg                 ;
    Int * reclaimedBuf        ;
    Int   i                   ;
    Int   j                   ;
    Int   k                   ;
    Int   index               ;

    for (k = 0 ;   ((k < iterations)  || (iterations == 0))
                 && (status == SYS_OK) ; k++) {
        for (j = 0 ; (j < numChannels) && (status == SYS_OK) ; j++) {
            index = (Int)(j / 2) ;
            for (i = 0 ; (i < numChirps) && (status == SYS_OK) ; i++) {
                if ((j % 2) == 0) {
                    status = SIO_issue (input [index], buf, bufSize, 0) ;
                    if (status == SYS_OK) {
                        nbytes = SIO_reclaim (input [index],
                                              (Ptr *)&reclaimedBuf,
                                              &arg);
                        if (nbytes < 0) {
                            status = -nbytes ;
                            LOG_printf(&trace, "DSP BIOS Link: "
                                               "Buffer reclaim failed %d\n",
                                               status);
                        }
                    }
                    else {
                        LOG_printf(&trace, "DSP BIOS Link: "
                                           "Buffer issuing failed %d\n",
                                           status);
                    }
                }
                else {
                    status = SIO_issue (output [index], buf, bufSize, 0) ;
                    if (status == SYS_OK) {
                        nbytes = SIO_reclaim (output [index],
                                              (Ptr *)&reclaimedBuf,
                                              &arg);
                        if (nbytes < 0) {
                            status = -nbytes ;
                            LOG_printf(&trace, "DSP BIOS Link: "
                                               "Buffer reclaim failed %d\n",
                                               status);
                            }
                    }
                    else {
                        LOG_printf(&trace, "DSP BIOS Link: "
                                           "Buffer issuing failed %d\n",
                                           status);
                    }
                }
            }
        }
    }
    return status ;
}


/** ============================================================================
 *  @func   multichnlstress_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the multichnlstress_Create function.
 *
 *  @modif  buf
 *  ============================================================================
 */
Void
multichnlstress_Delete ()
{
    Int j     = 0 ;
    Int index = 0 ;

    for (j = 0 ; (j < numChannels) ; j++) {
        index = (Int)(j / 2) ;
        if ((j % 2) == 0) {
            SIO_delete (input  [index]) ;
        }
        else {
            SIO_delete (output [index]) ;
        }
    }
    if (buf != NULL) {
        MEM_free (DSPLINK_SEGID, buf, bufSize) ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the multichnlstress task.
 *
 *  @modif  iterations, bufSize, numChirps, numChannels, inTask
 *  ============================================================================
 */
Void main (IN Int argc, IN Char * argv [])
{
    TSK_Attrs  attrs  = TSK_ATTRS ;
    TSK_Handle inTask             ;

    iterations  = atoi (argv[0]) ;
    bufSize     = (atoi (argv[1]) / DSP_MAUSIZE) ;
    numChirps   = atoi (argv[2]) ;
    numChannels = atoi (argv[3]) ;

    attrs.stacksize = 2000 ;

    inTask = TSK_create ((Fxn)multichnlstress, &attrs, 0) ;


    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
