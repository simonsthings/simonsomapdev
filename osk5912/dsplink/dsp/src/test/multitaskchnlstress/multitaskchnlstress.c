/** ============================================================================
 *  @file   multitaskchnlstress.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\multitaskchnlstress
 *
 *  @desc   Implemetation of multitaskchnlstress with multiple tasks on
 *          DSP side.
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
#include <multitaskchnlstress.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MAX_CHANNELS
 *
 *  @desc   Maximum number of channels supported by DSP/BIOS LINK.
 *  ============================================================================
 */
#define MAX_CHANNELS 16

/** ============================================================================
 *  @name   DRIVER_NAME_LENGTH
 *
 *  @desc   Maximum length of the name of the channels.
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
static SIO_Handle input [MAX_CHANNELS / 2] ;

/** ============================================================================
 *  @name   tskArgs
 *
 *  @desc   The array of the arguments to be passed to the tasks created for
 *          data transfer.
 *  ============================================================================
 */
static Int tskArgs [MAX_CHANNELS] ;

/** ============================================================================
 *  @name   output
 *
 *  @desc   This is the output  stream which is opened for sending data
 *          to GPP.
 *  ============================================================================
 */
static SIO_Handle output [MAX_CHANNELS / 2] ;

/** ============================================================================
 *  @name   buf
 *
 *  @desc   This is array of pointers to the buffers used for data transfer to
 *          GPP.
 *  ============================================================================
 */
static Uns * buf [MAX_CHANNELS] ;

/** ============================================================================
 *  @name   channels
 *
 *  @desc   This is a 2D array which stores the names of the channels to be
 *          created.
 *  ============================================================================
 */
static char channels [MAX_CHANNELS][DRIVER_NAME_LENGTH] ;


/** ============================================================================
 *  @func   multitaskchnlstress_Create
 *
 *  @desc   This function allocates the resources needed by the
 *          multitaskchnlstress test.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    multitaskchnlstress_Execute, multitaskchnlstress_Delete
 *  ============================================================================
 */
static
Uns
multitaskchnlstress_Create () ;


/** ============================================================================
 *  @func   multitaskchnlstress_Execute
 *
 *  @desc   This function creates multiple tasks which then do the data
 *          transfer with the GPP.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    multitaskchnlstress_Delete, multitaskchnlstress_Create
 *  ============================================================================
 */
static
Uns
multitaskchnlstress_Execute () ;


/** ============================================================================
 *  @func   multitaskchnlstress_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the multitaskchnlstress_Create function.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    multitaskchnlstress_Execute, multitaskchnlstress_Create
 *  ============================================================================
 */
static
Void
multitaskchnlstress_Delete () ;


/** ============================================================================
 *  @func   multitaskchnlstress
 *
 *  @desc   This function communicates with gpp side application and receives
 *          a data buffer from GPP side.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
multitaskchnlstress (Arg arg)
{
    Uns status = SYS_OK ;
    Uns chanId = 0    ;

    chanId = (*(Int*)arg) ;

    status = multitaskchnlstress_Create (chanId) ;

    if (status == SYS_OK) {
        status = multitaskchnlstress_Execute (chanId) ;
    }
    multitaskchnlstress_Delete (chanId) ;
}


/** ============================================================================
 *  @func   multitaskchnlstress_Create
 *
 *  @desc   This function allocates the resources needed by the
 *          multitaskchnlstress test.
 *
 *  @modif  None
 *  ============================================================================
 */
static
Uns
multitaskchnlstress_Create (Uns chanId)
{
    Uns        status = SYS_OK ;
    SIO_Attrs  attrs           ;
    Int        index           ;
    char       chnlName [3]    ;

    attrs         = SIO_ATTRS        ;
    attrs.nbufs   = 1                ;
    attrs.segid   = DSPLINK_SEGID    ;
    attrs.align   = DSPLINK_ALIGN    ;
    attrs.flush   = TRUE             ;
    attrs.model   = SIO_ISSUERECLAIM ;
    attrs.timeout = SYS_FOREVER      ;

    SYS_sprintf (channels [chanId], "/dio_dsplink") ;
    SYS_sprintf (chnlName, "%d", chanId) ;
    strncat (channels [chanId], chnlName, 3) ;

    index = (Int)(chanId / 2) ;
    if ((chanId % 2) == 0) {
        input [index] = SIO_create (channels [chanId],
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
       output [index] = SIO_create (channels [chanId],
                                    SIO_OUTPUT,
                                    bufSize,
                                    &attrs) ;
       if (output [index] == NULL) {
           status = SYS_EALLOC ;
           LOG_printf (&trace, "Could not create output stream",
                       status) ;
       }
    }

    if (status == SYS_OK) {
        buf [chanId] = MEM_calloc (DSPLINK_SEGID, bufSize, DSPLINK_ALIGN);
    }

    if (buf [chanId] == NULL) {
       status = SYS_EALLOC ;
       LOG_printf (&trace, "Could not allocate buffer for data transfer",
                   status) ;
    }

    return status ;
}


/** ============================================================================
 *  @func   multitaskchnlstress_Execute
 *
 *  @desc   This function creates multiple tasks which then do the data
 *          transfer with the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
static
Uns
multitaskchnlstress_Execute (Uns chanId)
{
    Uns   status       = SYS_OK ;
    Int   nbytes       = -1     ;
    Int * reclaimedBuf          ;
    Int   i                     ;
    Int   j                     ;
    Int   index                 ;
    Arg   reclaimArg            ;

    index = (Int)(chanId / 2) ;
    for (i = 0 ;    ((i < iterations)  || (iterations == 0))
                 && (status == SYS_OK) ; i++) {
        for (j = 0 ; (j < numChirps) && (status == SYS_OK) ; j++) {
            if ((chanId % 2) == 0) {
                status = SIO_issue (input [index], buf [chanId], bufSize, 0) ;
                if (status == SYS_OK) {
                    nbytes = SIO_reclaim (input [index],
                                          (Ptr *)&reclaimedBuf,
                                          &reclaimArg) ;
                    if (nbytes < 0) {
                        status = -nbytes ;
                        LOG_printf (&trace, "DSP BIOS Link: "
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
                status = SIO_issue (output [index], buf [chanId], bufSize, 0) ;
                if (status == SYS_OK) {
                    nbytes = SIO_reclaim (output [index],
                                          (Ptr *)&reclaimedBuf,
                                          &reclaimArg);
                    if (nbytes < 0) {
                        status = -nbytes ;
                        LOG_printf (&trace, "DSP BIOS Link: "
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

    return status ;
}


/** ============================================================================
 *  @func   multitaskchnlstress_Delete
 *
 *  @desc   This function deallocates the resources allocated by
 *          the multitaskchnlstress_Create function.
 *
 *  @modif  None
 *  ============================================================================
 */
static
Void
multitaskchnlstress_Delete (Uns chanId)
{
    Int index ;

    index = (Int)(chanId / 2) ;
    if ((chanId % 2) == 0) {
        SIO_delete (input  [index]) ;
    }
    else {
        SIO_delete (output [index]) ;
    }
    if (buf [chanId] != NULL) {
        MEM_free (0, buf [chanId], bufSize) ;
    }
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   This function creates the multitaskchnlstress task.
 *
 *  @modif  iterations, bufSize, numChirps, inTask
 *  ============================================================================
 */
Void
main (IN Int argc, IN Char * argv [])
{
    TSK_Handle inTask ;
    Int        i      ;

    iterations = atoi (argv[0]) ;
    bufSize    = (atoi (argv[1]) / DSP_MAUSIZE) ;
    numChirps  = atoi (argv[2]) ;
    numChannels  = atoi (argv[3]) ;

    for (i = 0 ; i < numChannels ; i++) {
        tskArgs [i] = i ;
        inTask = TSK_create ((Fxn)multitaskchnlstress, NULL, &(tskArgs [i])) ;
        if (inTask == NULL) {
            LOG_printf (&trace, "Task creation failed\n") ;
        }
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
