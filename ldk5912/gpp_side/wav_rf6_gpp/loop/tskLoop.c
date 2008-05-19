/** ============================================================================
 *  @file   tskLoop.c
 *
 *  @path   $(DSPLINK)/dsp/src/samples/loop/
 *
 *  @desc   This is simple TSK based application that uses SIO interface to
 *          implement loopback for GPP. It receives data from the a input
 *          channel and send the same data back from output channel. Input and
 *          output channel numbers are configurable through header file of this
 *          application.
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

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <failure.h>
#include <dsplink.h>
#include <platform.h>

/*  ----------------------------------- Sample Headers              */
#include <loop_config.h>
#include <tskLoop.h>


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID is used by SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID  FID_APP_C

/** ============================================================================
 *  @const  POOLID
 *
 *  @desc   Pool identifier for this application.
 *  ============================================================================
 */
#define POOLID  0

/** ============================================================================
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for TSK based loopback.
 *  ============================================================================
 */
extern Uint32 xferBufSize ;

/** ============================================================================
 *  @name   numTransfers
 *
 *  @desc   Iterations of data transfer to be done by the application.
 *          A value of 0 in numTransfers implies infinite iterations.
 *  ============================================================================
 */
//extern Uint16 numTransfers ;


/** ============================================================================
 *  @func   TSKLOOP_create
 *
 *  @desc   Create phase function for the TSKLOOP application. Initializes the
 *          TSKLOOP_TransferInfo structure with the information that will be
 *          used by the other phases of the application.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int TSKLOOP_create (TSKLOOP_TransferInfo ** infoPtr)
{
    Int                     status = SYS_OK ;
    TSKLOOP_TransferInfo *  info = NULL ;
    SIO_Attrs               attrs ;
    Uint16                  i ;
    Uint16                  j ;

    /* Allocate TSKLOOP_TransferInfo structure that will be initialized
     * and passed to other phases of the application
     */
    *infoPtr = MEM_calloc (DSPLINK_SEGID,
                           sizeof (TSKLOOP_TransferInfo),
                           DSPLINK_ALIGN) ;
    if (*infoPtr == NULL) {
        status = SYS_EALLOC ;
    }
    else {
        info = *infoPtr ;
    }

    if (status == SYS_OK) {
        /* Filling up the transfer info structure */
        //info->numTransfers =  numTransfers ;
        info->bufferSize   =  xferBufSize ;
        info->numBuffers   =  TSK_NUM_BUFFERS ;

        /* Attributes for the stream creation */
        attrs = SIO_ATTRS ;
        attrs.nbufs   = info->numBuffers ;
        attrs.segid   = DSPLINK_SEGID ;
        attrs.align   = DSPLINK_ALIGN ;
        attrs.flush   = TRUE ;
        attrs.model   = SIO_ISSUERECLAIM ;
        attrs.timeout = SYS_FOREVER ;

        /* Creating input and output streams */
        info->inputStream = SIO_create (INPUT_CHANNEL,
                                        SIO_INPUT,
                                        info->bufferSize,
                                        &attrs) ;
        info->outputStream = SIO_create (OUTPUT_CHANNEL,
                                         SIO_OUTPUT,
                                         info->bufferSize,
                                         &attrs) ;

        if (   (info->inputStream  == NULL)
            || (info->outputStream == NULL)) {
            status = SYS_EALLOC ;
        }
    }

    /* Allocating all the buffers that will be used in the transfer */
    if (status == SYS_OK) {
        for (i = 0 ; (i < info->numBuffers) && (status == SYS_OK) ; i++) {
            status = DSPLINK_alloc (POOLID,
                                    (Ptr *) &(info->buffers [i]),
                                    info->bufferSize) ;
            if (status != SYS_OK) {
                for (j = 0 ; j < i ; j++) {
                    DSPLINK_free (POOLID, info->buffers [i], info->bufferSize) ;
                    info->buffers [j] = NULL ;
                }
                status = SYS_EALLOC ;
            }
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   TSKLOOP_execute
 *
 *  @desc   Execute phase function for the TSKLOOP application. Application
 *          receives the data from the input channel and sends the same data
 *          back on output channel. Channel numbers can be configured through
 *          header file.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int TSKLOOP_execute(TSKLOOP_TransferInfo * info)
{
    Int         status  = SYS_OK ;
    Char *      buffer  = info->buffers [0] ;
    Arg         arg     = 0 ;
    //Uint32      i ;
    Int         nmadus ;
    buffers = {1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,
               1, 10, 20, 1,1, 10, 20, 1,};

/* Do not require the for loop for iteration and also the code for receiving data buffer from GPP as we are sending    the data buffer to the GPP */ 
    /* Execute the loop for configured number of transfers
     * A value of 0 in numTransfers implies infinite iterations
     */
   /* for (i = 0 ;
         (   ((info->numTransfers == 0) || (i < info->numTransfers))
          && (status == SYS_OK)) ;
         i++) {  */
        /*================== Receive a data buffer from GPP======================================= */
      /*  status = SIO_issue(info->inputStream,
                           buffer,
                           info->bufferSize,
                           arg) ;
        if (status == SYS_OK) {
            nmadus = SIO_reclaim (info->inputStream,
                                  (Ptr *) &buffer,
                                  &arg) ;
            if (nmadus < 0) {
                status = -nmadus ;
                SET_FAILURE_REASON (status) ;
            }
            else {
                info->receivedSize = nmadus ;
            }
        }
        else {
            SET_FAILURE_REASON(status);
        } */

        /* Do processing on this buffer */
       // if (status == SYS_OK) {
            /* Add code to process the buffer here*/
       // } 

 /*=================Sending Data Buffer to GPP======================================*/ 
        /* Send the processed buffer back to GPP */
        if (status == SYS_OK) {
            status = SIO_issue(info->outputStream,
                               buffer,
                               info->receivedSize,
                               arg);

            if (status == SYS_OK) {
                nmadus = SIO_reclaim (info->outputStream,
                                      (Ptr *) &(buffer),
                                      &arg) ;
                if (nmadus < 0) {
                    status = -nmadus ;
                    SET_FAILURE_REASON (status) ;
                }
            }
            else {
                SET_FAILURE_REASON (status) ;
            }
        }
   // } /* end of 'for' loop */ 
    return status ;
}


/** ============================================================================
 *  @func   TSKLOOP_delete
 *
 *  @desc   Delete phase function for the TSKLOOP application. It deallocates
 *          all the resources of allocated during create phase of the
 *          application.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int TSKLOOP_delete (TSKLOOP_TransferInfo * info)
{
    Int     status     = SYS_OK ;
    Uint16  tmpStatus = SYS_OK ;
    Bool    freeStatus = FALSE ;
    Uint16  j ;

    /* Delete input stream */
    if (info->inputStream != NULL) {
        status = SIO_delete (info->inputStream) ;
        if (status != SYS_OK) {
            SET_FAILURE_REASON (status) ;
        }
    }

    /* Delete output stream */
    if (info->outputStream != NULL) {
        tmpStatus = SIO_delete(info->outputStream);
        if ((status == SYS_OK) && (tmpStatus != SYS_OK)) {
            status = tmpStatus ;
            SET_FAILURE_REASON (status) ;
        }
    }

    /* Delete the buffers */
    if (info->numBuffers > 0) {
        for (j = 0 ;
             (j < info->numBuffers) && (info->buffers [j] != NULL) ;
             j++) {
            DSPLINK_free (POOLID, info->buffers [j], info->bufferSize) ;
        }
    }

    /* Free the info structure */
    freeStatus = MEM_free (DSPLINK_SEGID, info, sizeof (TSKLOOP_TransferInfo)) ;
    if ((status == SYS_OK) && (freeStatus != TRUE)) {
        status = SYS_EFREE ;
        SET_FAILURE_REASON (status) ;
    }

    return status ;
}
