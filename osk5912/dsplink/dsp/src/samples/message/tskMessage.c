/** ============================================================================
 *  @file   tskMessage.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\message
 *
 *  @desc   This is simple TSK based application that uses MSGQ.  It receives a
 *          messages from the GPP, verifies its content and sends it back to
 *          the GPP.
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
#include <sem.h>
#include <sys.h>
#include <tsk.h>
#include <msgq.h>
#include <mqtbiosque.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <failure.h>
#include <platform.h>
#include <mqabuf.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Sample Headers              */
#include <tskMessage.h>


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID is used by SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID  FID_APP_C

/** ============================================================================
 *  @name   trace
 *
 *  @desc   trace LOG_Obj used to do LOG_printf
 *  ============================================================================
 */
extern LOG_Obj trace;

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
 *  @func   TSKMESSAGE_create
 *
 *  @desc   Create phase function for the TSKMESSAGE application. Initializes
 *          the TSKMESSAGE_TransferInfo structure with the information that will
 *          be used by the other phases of the application.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns TSKMESSAGE_create(TSKMESSAGE_TransferInfo ** infoPtr)
{
    Uns                       status     = SYS_OK;
    MSGQ_Status               msgqStatus = MSGQ_SUCCESS;
    MSGQ_Attrs                msgqAttrs  = MSGQ_ATTRS;
    TSKMESSAGE_TransferInfo * info       = NULL;

    /* Initialize MSGQ module, allocators and transports */
    MQABUF_init();
    MQTBIOSQUE_init();
    MQTDSPLINK_init();
    MSGQ_init();

    /* Allocate TSKMESSAGE_TransferInfo structure that will be initialized
     * and passed to other phases of the application */
    *infoPtr = MEM_calloc(DSPLINK_SEGID,
                          sizeof (TSKMESSAGE_TransferInfo),
                          DSPLINK_ALIGN);
    if (*infoPtr == NULL) {
        status = SYS_EALLOC;
        SET_FAILURE_REASON(status);
    }
    else {
        info = *infoPtr;
    }

    if (status == SYS_OK) {
        /* Set the semaphore to a known state. */
        SEM_new(&(info->notifySemObj), 0);

        /* Fill in the attributes for this message queue. */
        msgqAttrs.notifyHandle = &(info->notifySemObj);
        msgqAttrs.post         = (MSGQ_Post)SEM_post;
        msgqAttrs.pend         = (MSGQ_Pend)binarySemPend;

        /* Creating message queue */
        info->msgqHandle = MSGQ_create(TSK_DSP_MSGQNAME, &msgqAttrs);
        if (info->msgqHandle == NULL) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
        else {
            /* Synchronous locate. */
            msgqStatus = MSGQ_ENOTFOUND ;
            while (msgqStatus == MSGQ_ENOTFOUND) {
                msgqStatus = MSGQ_locate (TSK_GPP_MSGQNAME,
                                          &(info->locatedMsgqHandle),
                                          NULL) ;
                if (msgqStatus == MSGQ_ENOTFOUND) {
                    TSK_sleep (1000) ;
                }
                else if (msgqStatus != MSGQ_SUCCESS) {
                    status = SYS_EBADIO ;
                    LOG_printf (&trace, "MSGQ_locate (msgqOut) failed. "
                                "Status = 0x%x\n", msgqStatus) ;
                }
            }
        }

        /* Initialize the sequenceNumber */
        info->sequenceNumber = 0;
    }

    return status;
}


/** ============================================================================
 *  @func   TSKMESSAGE_execute
 *
 *  @desc   Execute phase function for the TSKMESSAGE application. Application
 *          receives a message, verifies the id and sends it back.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns TSKMESSAGE_execute(TSKMESSAGE_TransferInfo * info)
{
    Uns           status = SYS_OK;
    Bool          done   = FALSE;
    MSGQ_Msg      msg;
    MSGQ_Status   msgqStatus;
    MSGQ_MqtError errorType;

    /* Execute the loop for configured number of transfers */
    while (!done && (status == SYS_OK)) {
        /* Receive a message */
        msg = MSGQ_get(info->msgqHandle, SYS_FOREVER);
        if (msg != NULL) {
            /* Determine which type of message */
            switch (MSGQ_getMsgId(msg)) {
                case MSGQ_ASYNCERRORMSGID:
                    errorType = ((MSGQ_AsyncErrorMsg *)msg)->errorType;
                    LOG_printf(&trace, "Transport error Type = %d", errorType);

                    /* If GPP-side has closed the transport, indicates end of
                     * processing.
                     */
                    if (errorType == MSGQ_MQTERROREXIT) {
                        done = TRUE;
                    }

                    /* Must free the message */
                    MSGQ_free(msg);
                    break;

                default:
                    /* Check to make sure correct message is received */
                    if (MSGQ_getMsgId(msg) != info->sequenceNumber) {
                        LOG_printf(&trace, "Out of sequence message!");
                        status = SYS_EBADIO;
                        SET_FAILURE_REASON(status);
                    }

                    /* Send the message back */
                    msgqStatus = MSGQ_put(info->locatedMsgqHandle,
                                          msg,
                                          info->sequenceNumber,
                                          NULL );
                    if ((status == SYS_OK) && (msgqStatus != MSGQ_SUCCESS)) {
                        status = SYS_EINVAL;
                        SET_FAILURE_REASON(status);
                    }

                    /* Increment the sequenceNumber for next received message */
                    info->sequenceNumber++;
                    break;
            }
        }
        else {
            status = SYS_EINVAL;
            SET_FAILURE_REASON(status);
        }
    }

    return status;
}


/** ============================================================================
 *  @func   TSKMESSAGE_delete
 *
 *  @desc   Delete phase function for the TSKMESSAGE application. It deallocates
 *          all the resources of allocated during create phase of the
 *          application.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns TSKMESSAGE_delete (TSKMESSAGE_TransferInfo * info)
{
    Uns         status     = SYS_OK;
    Bool        freeStatus = FALSE ;
    MSGQ_Status msgqStatus = MSGQ_SUCCESS;

    /* Release the located message queue */
    if (info->locatedMsgqHandle != NULL) {
        msgqStatus = MSGQ_release(info->locatedMsgqHandle);
        if ((status == SYS_OK) && (msgqStatus != MSGQ_SUCCESS)) {
            status = SYS_EINVAL;
            SET_FAILURE_REASON(status);
        }
    }

    /*
     *  Reset the error handler before deleting the MSGQ that receives the error
     *  messages.
     */
    MSGQ_setErrorHandler(NULL, 0);

    /* Delete the message queue */
    if (info->msgqHandle != NULL) {
        msgqStatus = MSGQ_delete(info->msgqHandle);
        if ((status == SYS_OK) && (msgqStatus != MSGQ_SUCCESS)) {
            status = SYS_EINVAL;
            SET_FAILURE_REASON(status);
        }
    }

    /* Free the info structure */
    freeStatus = MEM_free(DSPLINK_SEGID, info, sizeof (TSKMESSAGE_TransferInfo));
    if ((status == SYS_OK) && (freeStatus != TRUE)) {
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

