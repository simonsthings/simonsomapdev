/** ============================================================================
 *  @file   application.c
 *
 *  @path   $(PROJROOT)\dsp\src\test\application
 *
 *  @desc   Implemetation of WaveApplication on DSP side.
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
#include <sio.h>
#include <log.h>
#include <tsk.h>
#include <application.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ----------------------------------------------------------------------------
 *  @name   trace
 *
 *  @desc   Log object used to print the Information messages on DSP/BIOS.
 *  ----------------------------------------------------------------------------
 */
extern
LOG_Obj
trace ;


/** ============================================================================
 *  @name   input
 *
 *  @desc   Input stream opened by the application to recieve wave data from
 *          the DSP.
 *  ============================================================================
 */
SIO_Handle
input = NULL ;


/** ============================================================================
 *  @name   codecOutput
 *
 *  @desc   Output stream opened by the application to write data to the Audio
 *          codec driver.
 *  ============================================================================
 */
SIO_Handle
codecOutput = NULL ;


/** ============================================================================
 *  @name   wavBuf
 *
 *  @desc   Pointer to the buffer used for receiving data from GPP.
 *  ============================================================================
 */
Uns *
wavBuf = NULL ;


/** ----------------------------------------------------------------------------
 *  @func   Audio_Create
 *
 *  @desc   This function creates the streams required by the application.
 *
 *  @arg    None
 *
 *  @ret    TRUE
 *              Operation successfully completed.
 *          FALSE
 *              Operation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static
Bool
Audio_Create () ;


/** ----------------------------------------------------------------------------
 *  @func   Audio_Execute
 *
 *  @desc   This function creates the streams required by the application.
 *
 *  @arg    None
 *
 *  @ret    TRUE
 *              Operation successfully completed.
 *          FALSE
 *              Operation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static
Bool
Audio_Execute () ;


/** ----------------------------------------------------------------------------
 *  @func   Audio_Delete
 *
 *  @desc   This function deletes the streams used by the application.
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
static
Void
Audio_Delete () ;


/** ============================================================================
 *  @func   WaveApplication
 *
 *  @desc   This is the entry point to thewave application.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
WaveApplication ()
{
    Bool status = TRUE ;

    status = Audio_Create () ;

    if (status == TRUE ) {
        status = Audio_Execute () ;
    }

    Audio_Delete () ;
}


/** ----------------------------------------------------------------------------
 *  @func   Audio_Create
 *
 *  @desc   This function creates the streams required by the application.
 *
 *  @modif  state, input, control, codecOutput
 *  ----------------------------------------------------------------------------
 */
static
Bool
Audio_Create ()
{
    Bool      status = TRUE ;
    SIO_Attrs attrs         ;

    attrs         = SIO_ATTRS        ;
    attrs.nbufs   = 2                ;
    attrs.segid   = 0                ;
    attrs.align   = 0                ;
    attrs.flush   = TRUE             ;
    attrs.model   = SIO_ISSUERECLAIM ;
    attrs.timeout = SYS_FOREVER      ;


    input = SIO_create (INPUTDATACHANNEL, SIO_INPUT, BUFSIZE, &attrs) ;

    if (input == NULL) {
        LOG_printf (&trace, "Could not create input stream") ;
        status = FALSE ;
    }

    if (status == TRUE) {
        codecOutput = SIO_create (CODECPATH, SIO_OUTPUT, BUFSIZE, &attrs) ;
        if (codecOutput == NULL) {
            LOG_printf (&trace, "Could not create output stream") ;
            status = FALSE ;
        }
    }

    if (status == TRUE) {
        wavBuf = MEM_calloc(0, BUFSIZE, 0);
    }

    if (wavBuf == NULL) {
       LOG_printf (&trace, "Could not allocate buffer for data transfer") ;
       status = SYS_EALLOC ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   Audio_Delete
 *
 *  @desc   This function deletes the streams used by the application.
 *
 *  @modif  state, input, control, codecOutput
 *  ----------------------------------------------------------------------------
 */
static
Void
Audio_Delete ()
{
    SIO_delete (input) ;
    input = NULL ;

    SIO_delete (codecOutput) ;
    codecOutput = NULL ;

    if (wavBuf != NULL) {
        MEM_free (0, wavBuf, BUFSIZE) ;
    }
}


/** ----------------------------------------------------------------------------
 *  @func   Audio_Execute
 *
 *  @desc   This function deletes the streams used by the application.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Bool
Audio_Execute ()
{
    Bool      status        = TRUE   ;
    Uns       statusOfIssue = SYS_OK ;
    Int       nbytes        = 1      ;
    Arg       arg                    ;

    statusOfIssue = SIO_issue (codecOutput, wavBuf, BUFSIZE, 0) ;
    if (statusOfIssue != SYS_OK) {
        status = FALSE ;
        LOG_printf(&trace, "CODEC : Buffer issuing failed %d\n",
                   status);
    }

    while ((status == TRUE) && (nbytes > 0)) {
        statusOfIssue = SIO_issue (input, wavBuf, BUFSIZE, 0) ;
        if (statusOfIssue == SYS_OK) {
            nbytes = SIO_reclaim (input, (Ptr *)&wavBuf, &arg) ;
            if (nbytes < 0) {
                LOG_printf(&trace, "DSP BIOS Link: Buffer reclaim failed %d\n",
                           status);
                status = FALSE ;
            }
        }
        else {
            LOG_printf(&trace, "DSP BIOS Link: Buffer issuing failed %d\n",
                       status);
            status = FALSE ;
        }

        if (status == TRUE) {
            statusOfIssue = SIO_issue (codecOutput, wavBuf, BUFSIZE, 0) ;
            if (statusOfIssue == SYS_OK) {
                nbytes = SIO_reclaim (codecOutput, (Ptr *)&wavBuf, &arg) ;
                if (nbytes < 0) {
                    LOG_printf(&trace,
                               "CODEC: Buffer reclaim failed %d\n",
                               status);
                    status = FALSE ;
                }
            }
            else {
                LOG_printf(&trace, "CODEC : Buffer issuing failed %d\n",
                           status);
                status = FALSE ;
            }
        }
    }
    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   main
 *
 *  @desc   main function of the application.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
Void
main ()
{
    TSK_Handle inTask ;

    inTask = TSK_create ((Fxn)WaveApplication, NULL, 0) ;

    if (inTask == NULL) {
        LOG_printf (&trace, "Task creation failed\n") ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
