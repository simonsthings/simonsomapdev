/** ============================================================================
 *  @file   swiMessage.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\message
 *
 *  @desc   This is simple SWI based application that uses MSGQ interface to
 *          implement messaging from GPP. It receives a messages from the GPP,
 *          verifies its content and sends it back to the GPP.
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
#include <iom.h>
#include <gio.h>
#include <msgq.h>
#include <mqtbiosque.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <failure.h>
#include <platform.h>
#include <mqabuf.h>
#include <mqtdsplink.h>

/*  ----------------------------------- Sample Headers              */
#include <swiMessage.h>


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID needed for SET_FAILURE_REASON macro.
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

/** ----------------------------------------------------------------------------
 *  @func   messageSWI
 *
 *  @desc   Entry point for SWI that does message handling.
 *
 *  @arg    arg0
 *              Pointer to information structure
 *  @arg    arg1
 *              Currently not used
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
static Void messageSWI(Arg arg0, Arg arg1);


/** ============================================================================
 *  @func   SWIMESSAGE_create
 *
 *  @desc   Create phase of SWIMESSAGE application. It allocates
 *          SWIMESSAGE_TransferInfo structure and intializes it with configured
 *          values.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns SWIMESSAGE_create(SWIMESSAGE_TransferInfo ** infoPtr)
{
    Uns                       status      = SYS_OK;
    MSGQ_Status               msgqStatus  = MSGQ_SUCCESS;
    SWI_Attrs                 swiAttrs    = SWI_ATTRS;
    MSGQ_Attrs                msgqAttrs   = MSGQ_ATTRS;
    MSGQ_LocateAttrs          locateAttrs = MSGQ_LOCATEATTRS;
    SWIMESSAGE_TransferInfo * info;

    /* Initialize MSGQ module, allocators and transports */
    MQABUF_init();
    MQTBIOSQUE_init();
    MQTDSPLINK_init();
    MSGQ_init();

    /* Allocate SWIMESSAGE_TransferInfo structure */
    *infoPtr = MEM_calloc(DSPLINK_SEGID,
                          sizeof (SWIMESSAGE_TransferInfo),
                          DSPLINK_ALIGN);
    if (*infoPtr == NULL) {
        status = SYS_EALLOC;
        SET_FAILURE_REASON(status);
    }
    else {
        info = *infoPtr;
    }

    /* Create SWI for receiving and sending messages */
    if (status == SYS_OK) {
        swiAttrs.fxn = messageSWI;
        swiAttrs.arg0 = (Arg) info;
        info->swi = SWI_create(&swiAttrs);
        if (info->swi == NULL) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }

    /* Setup message queues */
    if (status == SYS_OK) {
        /* Fill in the attributes for this message queue. */
        msgqAttrs.notifyHandle = info->swi;
        msgqAttrs.post         = (MSGQ_Post)SWI_post;
        msgqAttrs.pend         = NULL;

        /* Creating message queue */
        info->msgqHandle = MSGQ_create(SWI_DSP_MSGQNAME, &msgqAttrs);
        if (info->msgqHandle == NULL) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }

        /* Asynchronous locate. */
        locateAttrs.replyHandle = info->msgqHandle;
        locateAttrs.mqaId = 0;

        msgqStatus = MSGQ_locate(SWI_GPP_MSGQNAME, NULL, &locateAttrs);
        if ((msgqStatus != MSGQ_SUCCESS) && (status == SYS_OK)) {
            status = SYS_EINVAL;
            SET_FAILURE_REASON(status);
        }

        /* Initialize the sequenceNumber */
        info->sequenceNumber = 0;
    }

    return status;
}

/** ============================================================================
 *  @func   SWIMESSAGE_delete
 *
 *  @desc   Delete phase of SWIMESSAGE application. Deallocates all the
 *          resources that were allocated during create phase.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns SWIMESSAGE_delete(SWIMESSAGE_TransferInfo * info)
{
    Uns         status     = SYS_OK;
    Bool        freeStatus = FALSE;
    MSGQ_Status msgqStatus = MSGQ_SUCCESS;

     /* Release the located message queue */
    if (info->locatedMsgqHandle != NULL) {
        msgqStatus = MSGQ_release(info->locatedMsgqHandle);
        if (msgqStatus != MSGQ_SUCCESS) {
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
    freeStatus = MEM_free(DSPLINK_SEGID, info, sizeof (SWIMESSAGE_TransferInfo));
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


/** ----------------------------------------------------------------------------
 *  @func   messageSWI
 *
 *  @desc   SWI entry point. This SWI is posted when a new message comes in.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void messageSWI(Arg arg0, Arg arg1)
{
    Uns                       status     = SYS_OK;
    MSGQ_Status               msgqStatus = MSGQ_SUCCESS;
    SWIMESSAGE_TransferInfo * info       = (SWIMESSAGE_TransferInfo *)arg0;
    MSGQ_Msg msg;

    /* This is a SWI so timeout must be zero */
    msg = MSGQ_get(info->msgqHandle, 0);
    if (msg != NULL) {
        /* Handle the received message */
        switch (MSGQ_getMsgId(msg)) {
            case MSGQ_ASYNCLOCATEMSGID:
                info->locatedMsgqHandle =
                    ((MSGQ_AsyncLocateMsg *)msg)->msgqHandle;

                /* Must free the message */
                MSGQ_free(msg);
                break;

            case MSGQ_ASYNCERRORMSGID:
                LOG_printf(&trace, "Transport error Type = %d",
                    ((MSGQ_AsyncErrorMsg *)msg)->errorType);

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
        status = SYS_EBADIO;
        SET_FAILURE_REASON(status);
    }
}
