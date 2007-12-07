/** ============================================================================
 *  @file   tskScale.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\scale
 *
 *  @desc   This is simple TSK based application that uses SIO interface to
 *          implement scale for GPP. It receives data from the a input
 *          channel and send the same data back from output channel. Input and
 *          output channel numbers are configurable through header file of this
 *          application. It also uses a message queue to receive a
 *          scaling factor for the output data.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
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
#include <msgq.h>
#include <mqtbiosque.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <failure.h>
#include <platform.h>
#include <mqabuf.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Sample Headers              */
#include <tskScale.h>
#include <scale.h>


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
 *  @desc   Size of the buffer size for TSK based scale.
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
 *  @func   binarySemPend
 *
 *  @desc   Simulates binary semaphore in DSP/BIOS when there is only 1 reader.
 *
 *  @modif  None.
 *  ============================================================================
 */
static Bool binarySemPend(SEM_Handle semHandle, Uns timeout);


/** ============================================================================
 *  @func   TSKSCALE_create
 *
 *  @desc   Create phase function for the TSKSCALE application. Initializes the
 *          TSKSCALE_TransferInfo structure with the information that will be
 *          used by the other phases of the application.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns TSKSCALE_create(TSKSCALE_TransferInfo ** infoPtr)
{
    Uns                     status    = SYS_OK;
    MSGQ_Attrs              msgqAttrs = MSGQ_ATTRS;
    SIO_Attrs               attrs     = SIO_ATTRS;
    TSKSCALE_TransferInfo * info      = NULL;

    /* Initialize MSGQ module, allocators and transports */
    MQABUF_init();
    MQTBIOSQUE_init();
    MQTDSPLINK_init();
    MSGQ_init();

    /* Allocate TSKSCALE_TransferInfo structure that will be initialized
     * and passed to other phases of the application */
    *infoPtr = MEM_calloc(DSPLINK_SEGID,
                          sizeof (TSKSCALE_TransferInfo),
                          DSPLINK_ALIGN);
    if (*infoPtr == NULL) {
        status = SYS_EALLOC;
    }
    else {
        info = *infoPtr;
    }

    if (status == SYS_OK) {
        /* Filling up the transfer info structure */
        info->numTransfers  = numTransfers;
        info->bufferSize    = xferBufSize;
        info->numBuffers    = TSK_NUM_BUFFERS;
        info->scalingFactor = 1;

        /* Attributes for the stream creation */
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

        if ((info->inputStream == NULL) || (info->outputStream == NULL)) {
            status = SYS_EALLOC;
        }
    }

    /* Allocating the one buffer that will be used in the transfer */
    if (status == SYS_OK) {
        info->buffer = MEM_calloc(DSPLINK_SEGID,
                                  info->bufferSize,
                                  DSPLINK_ALIGN);
        if (info->buffer == NULL) {
            status = SYS_EALLOC;
        }
    }

    /* Set-up the message queue */
    if (status == SYS_OK) {

        /* Set the semaphore to a know state. */
        SEM_new(&(info->notifySemObj), 0);

        /* Fill in the attributes for this message queue. */
        msgqAttrs.notifyHandle = &(info->notifySemObj);
        msgqAttrs.post         = (MSGQ_Post)SEM_post;
        msgqAttrs.pend         = (MSGQ_Pend)binarySemPend;

        /* Creating message queue */
        info->msgqHandle = MSGQ_create(TSK_DSP_MSGQNAME, &msgqAttrs);
        if ((status == SYS_OK) && (info->msgqHandle == NULL)) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }
    return status;
}


/** ============================================================================
 *  @func   TSKSCALE_execute
 *
 *  @desc   Execute phase function for the TSKSCALE application. Application
 *          receives the data from the input channel and sends the same data
 *          back on output channel. Channel numbers can be configured through
 *          header file.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns TSKSCALE_execute(TSKSCALE_TransferInfo * info)
{
    Uns      status    = SYS_OK;
    Char *   buffer    = (Char *) info->buffer;
    Arg      arg       = 0;
    Uns      i, j;
    Int      nmadus;
    MSGQ_Msg msg;

    /* Execute the loop for configured number of transfers */
    for (i = 0; (i < info->numTransfers) && (status == SYS_OK); i++) {
        /* Receive a data buffer from GPP */
        status = SIO_issue(info->inputStream,
                           buffer,
                           info->bufferSize,
                           arg);
        if (status == SYS_OK) {
            nmadus = SIO_reclaim(info->inputStream,
                                 (Ptr *)&buffer,
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

        /* Check to see if there is a scaling message */
        msg = MSGQ_get(info->msgqHandle, 0);
        if (msg != NULL) {
           if (MSGQ_getMsgId(msg) == SCALE_REQUEST_MSGID) {
               info->scalingFactor = ((SCALE_ScaleMsg *)msg)->scalingFactor;
           }
           else {
               status = SYS_EBADIO;
               SET_FAILURE_REASON(status);
           }
           MSGQ_free(msg);
        }

        /* Do processing on this buffer */
        if (status == SYS_OK) {
            for (j = 0; (j < info->receivedSize) && (status == SYS_OK); j++) {
                buffer[j] *= info->scalingFactor;
            }
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
 *  @func   TSKSCALE_delete
 *
 *  @desc   Delete phase function for the TSKSCALE application. It deallocates
 *          all the resources of allocated during create phase of the
 *          application.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns TSKSCALE_delete (TSKSCALE_TransferInfo * info)
{
    Uns         status     = SYS_OK;
    Uns         funcStatus = SYS_OK;
    Bool        freeStatus = FALSE;
    MSGQ_Status msgqStatus = MSGQ_SUCCESS;

    /* Delete the message queue */
    if (info->msgqHandle != NULL) {
        msgqStatus = MSGQ_delete(info->msgqHandle);
        if (msgqStatus != MSGQ_SUCCESS) {
            status = SYS_EINVAL;
            SET_FAILURE_REASON(status);
        }
    }

    /* Delete input stream */
    if (info->inputStream != NULL) {
        funcStatus = SIO_delete(info->inputStream);
        if (funcStatus != SYS_OK) {
            status = funcStatus;
            SET_FAILURE_REASON(status);
        }
    }

    /* Delete output stream */
    if (info->outputStream != NULL) {
        funcStatus = SIO_delete(info->outputStream);
        if (funcStatus != SYS_OK) {
            status = funcStatus;
            SET_FAILURE_REASON(status);
        }
    }

    /* Delete the buffer */
    if (info->numBuffers > 0) {
        if (info->buffer != NULL) {
            freeStatus = MEM_free(DSPLINK_SEGID, info->buffer, info->bufferSize);
            if (freeStatus != TRUE) {
                status = SYS_EFREE;
                SET_FAILURE_REASON(status);
            }
        }
    }

    /* Free the info structure */
    freeStatus = MEM_free(DSPLINK_SEGID, info, sizeof (TSKSCALE_TransferInfo));
    if (freeStatus != TRUE) {
        status = SYS_EFREE;
        SET_FAILURE_REASON(status);
    }

    /* Finalize MSGQ module, allocators and transports */
    MSGQ_exit () ;
    MQTDSPLINK_exit () ;
    MQTBIOSQUE_exit () ;
    MQABUF_exit () ;

    return status;
}

/** ============================================================================
 *  @func   binarySemPend
 *
 *  @desc   MSGQ supports only one reader of a message queue. MSGQ requires that
 *          the notify be binary. This pend function simulates a binary
 *          semaphore in DSP/BIOS. Note: this works since there is only one
 *          reader of a message queue.
 *
 *  @modif  None.
 *  ============================================================================
 */
Bool binarySemPend(SEM_Handle semHandle, Uns timeout)
{
    Bool retVal;

    retVal = SEM_pend(semHandle, timeout);
    SEM_reset(semHandle, 0);
    return retVal;
}
