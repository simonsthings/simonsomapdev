/** ============================================================================
 *  @file   tskLoop.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\loop
 *
 *  @desc   This is simple TSK based application that uses SIO interface to
 *          implement loopback for GPP. It receives data from the a input
 *          channel and send the same data back from output channel. Input and
 *          output channel numbers are configurable through header file of this
 *          application.
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
#include <log.h>
#include <swi.h>
#include <sys.h>
#include <sio.h>
#include <tsk.h>
#include <failure.h>

#include <tskLoop.h>
#include <platform.h>

/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID is used by SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID  FID_APP_C

/** ============================================================================
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for TSK based loopback.
 *  ============================================================================
 */
extern  Uns xferBufSize ;

/** ============================================================================
 *  @name   numTransfers
 *
 *  @desc   Iterations of data transfer to be done by the application.
 *  ============================================================================
 */
extern Uns numTransfers ;


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
Uns TSKLOOP_create(TSKLOOP_TransferInfo ** infoPtr)
{
    Uns status = SYS_OK;
    TSKLOOP_TransferInfo * info = NULL;
    SIO_Attrs   attrs;

    /* Allocate TSKLOOP_TransferInfo structure that will be initialized
     * and passed to other phases of the application */
    *infoPtr = MEM_calloc(DSPLINK_SEGID,
                          sizeof(TSKLOOP_TransferInfo),
                          DSPLINK_ALIGN);
    if (*infoPtr == NULL) {
        status = SYS_EALLOC;
    }
    else {
        info = *infoPtr;
    }

    if (status == SYS_OK) {
        /* Filling up the transfer info structure */
        info->numTransfers =  numTransfers;
        info->bufferSize   =  xferBufSize;
        info->numBuffers   =  TSK_NUM_BUFFERS;

        /* Attributes for the stream creation */
        attrs = SIO_ATTRS;
        attrs.nbufs = info->numBuffers;
        attrs.segid = DSPLINK_SEGID;
        attrs.align = DSPLINK_ALIGN;
        attrs.flush = TRUE;
        attrs.model = SIO_ISSUERECLAIM;
        attrs.timeout = SYS_FOREVER;

        /* Creating input and output streams */
        info->inputStream = SIO_create(TSK_INPUT_CHANNEL, SIO_INPUT,
                                       info->bufferSize, &attrs);
        info->outputStream = SIO_create(TSK_OUTPUT_CHANNEL, SIO_OUTPUT,
                                        info->bufferSize, &attrs);

        if (   (info->inputStream == NULL)
            || (info->outputStream == NULL)) {
            status = SYS_EALLOC;
        }
    }

    /* Allocating all the buffers that will be used in the transfer */
    if (status == SYS_OK) {
        Uns i;
        for (i = 0; (i < info->numBuffers) && (status == SYS_OK); i++) {
            info->buffers[i] = MEM_calloc(DSPLINK_SEGID,
                                          info->bufferSize,
                                          DSPLINK_ALIGN);
            if (info->buffers[i] == NULL) {
                Uns j;
                for (j = 0; j < i; j++) {
                    MEM_free(DSPLINK_SEGID, info->buffers[j], info->bufferSize);
                    info->buffers[j] = NULL;
                }
                status = SYS_EALLOC;
            }
        }
    }

    return status;
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
Uns TSKLOOP_execute(TSKLOOP_TransferInfo * info)
{
    Uns status = SYS_OK;
    Char * buffer = info->buffers[0];
    Arg arg = 0;
    Uns i;
    Uns *tmpBuffer;
    Int nmadus;

    /* Execute the loop for configured number of transfers */
    for (i = 0; (i < info->numTransfers) && (status == SYS_OK); i++) {
        /* Receive a data buffer from GPP */
        status = SIO_issue(info->inputStream,
                           buffer,
                           info->bufferSize,
                           arg);
        if (status == SYS_OK) {
            nmadus = SIO_reclaim(info->inputStream,
                                 (Ptr *)&tmpBuffer,
                                 &arg);
            if (nmadus < 0) {
                status = -nmadus;
                SET_FAILURE_REASON(status);
            }
            else {
                info->receivedSize = nmadus;
            }
        }
        else {
            SET_FAILURE_REASON(status);
        }

        /* Do processing on this buffer */
        if (status == SYS_OK) {
            /* Add code to process the buffer here*/
        }

        /* Send the processed buffer back to GPP */
        if (status == SYS_OK) {
            status = SIO_issue(info->outputStream,
                               buffer,
                               info->receivedSize,
                               arg);

            if (status == SYS_OK) {
                nmadus = SIO_reclaim(info->outputStream,
                                     (Ptr *)&(buffer),
                                     &arg);
                if (nmadus < 0) {
                    status = -nmadus;
                    SET_FAILURE_REASON(status);
                }
            }
            else {
                SET_FAILURE_REASON(status);
            }
        }
    }
    return status;
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
Uns TSKLOOP_delete (TSKLOOP_TransferInfo * info)
{
    Uns  status     = SYS_OK;
    Uns  funcStatus = SYS_OK;
    Bool freeStatus = FALSE ;
    Uns  j;

    /* Delete input stream */
    if (info->inputStream != NULL) {
        funcStatus = SIO_delete(info->inputStream);
        if (funcStatus != SYS_OK) {
            status = funcStatus;
        }
    }

    /* Delete output stream */
    if (info->outputStream != NULL) {
        funcStatus = SIO_delete(info->outputStream);
        if (funcStatus != SYS_OK) {
            status = funcStatus;
        }
    }

    /* Delete the buffers */
    if (info->numBuffers > 0) {
        for (j = 0; (j < info->numBuffers) && (info->buffers[j] != NULL); j++) {
            freeStatus = MEM_free(DSPLINK_SEGID, info->buffers[j], info->bufferSize);
            if (freeStatus != TRUE) {
                status = SYS_EFREE;
            }
        }
    }

    /* Free the info structure */
    freeStatus = MEM_free(DSPLINK_SEGID, info, sizeof(TSKLOOP_TransferInfo));
    if (freeStatus != TRUE) {
        status = SYS_EFREE;
    }
    return status;
}
