/** ============================================================================
 *  @file   swiScale.c
 *
 *  @path   $(DSPLINK)\dsp\src\samples\scale
 *
 *  @desc   This is simple DSP/BIOS LINK SWI application that does data transfer
 *          in loopback mode using GIO interface.  It opens two channels one
 *          for input and one for output. It also uses a message queue to
 *          receive a scaling factor. Channel numbers configurable. Maximum
 *          buffer size of input and output channels is also configurable
 *          though #defines in header file. It is important whenever one makes
 *          changes in the configuration of this application. He should also
 *          make changes on the corresponding GPP side application sources.
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
#include <swiScale.h>
#include <scale.h>


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID needed for SET_FAILURE_REASON macro.
 *  ============================================================================
 */
#define FILEID  FID_APP_C

/** ============================================================================
 *  @const  READ_MAILBOX_MASK
 *
 *  @desc   Read mailbox mask.
 *  ============================================================================
 */
#define READ_MAILBOX_MASK   0x1

/** ============================================================================
 *  @const  WRITE_MAILBOX_MASK
 *
 *  @desc   Write mailbox mask.
 *  ============================================================================
 */
#define WRITE_MAILBOX_MASK  0x2

/** ============================================================================
 *  @const  INITIAL_DATA_MAILBOX_VAL
 *
 *  @desc   Initial mailbox value.
 *  ============================================================================
 */
#define INITIAL_DATA_MAILBOX_VAL (READ_MAILBOX_MASK | WRITE_MAILBOX_MASK)

/** ============================================================================
 *  @const  INITIAL_MSG_MAILBOX_VAL
 *
 *  @desc   Initial mailbox value for msgSWI.
 *  ============================================================================
 */
#define INITIAL_MSG_MAILBOX_VAL 0x0

/** ============================================================================
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for SWI based loopback.
 *  ============================================================================
 */
extern Uns xferBufSize ;


/** ----------------------------------------------------------------------------
 *  @func   readFinishCb/writeFinishCb
 *
 *  @desc   Declaration of callback functions to be called by GIO when
 *          IOM_READ/IOM_WRITE request is over.
 *
 *  @arg    arg
 *              Argument given while doing GIO_submit.
 *          status
 *              Status of IO.
 *          bufp
 *              Buffer on which IO has completed.
 *          size
 *              Size of IO done.
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
static Void readFinishCb(Ptr arg, Int status, Ptr bufp, Uns size);
static Void writeFinishCb(Ptr arg, Int status, Ptr bufp, Uns size);

/** ----------------------------------------------------------------------------
 *  @func   dataSWI
 *
 *  @desc   Entry point for SWI that does data processing.
 *
 *  @arg    arg
 *              Argument given while doing GIO_submit.
 *          status
 *              Status of IO.
 *          bufp
 *              Buffer on which IO has completed.
 *          size
 *              Size of IO done.
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
static Void dataSWI(Arg arg0, Arg arg1);

/** ----------------------------------------------------------------------------
 *  @func   msgSWI
 *
 *  @desc   Entry point for SWI that does message processing.
 *
 *  @arg    arg0
 *              Info structure
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
static Void msgSWI(Arg arg0, Arg arg1);


/** ============================================================================
 *  @func   SWISCALE_create
 *
 *  @desc   Create phase of SWISCALE application. It allocates
 *          SWISCALE_TransferInfo structure and intializes it with configured
 *          values.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns SWISCALE_create(SWISCALE_TransferInfo ** infoPtr)
{
    Uns                     status    = SYS_OK;
    SWI_Attrs               swiAttrs  = SWI_ATTRS;
    MSGQ_Attrs              msgqAttrs = MSGQ_ATTRS;
    SWISCALE_TransferInfo * info;

    /* Initialize MSGQ module, allocators and transports */
    MQABUF_init();
    MQTBIOSQUE_init();
    MQTDSPLINK_init();
    MSGQ_init();

    /* Allocate SWISCALE_TransferInfo structure */
    *infoPtr = MEM_calloc(DSPLINK_SEGID, sizeof (SWISCALE_TransferInfo), DSPLINK_ALIGN);
    if (*infoPtr == NULL) {
        status = SYS_EALLOC;
        SET_FAILURE_REASON(status);
    }
    else {
        info = *infoPtr;
    }

    /* Initialize SWISCALE_TransferInfo structure */
    if (status == SYS_OK) {
        info->bufferSize = xferBufSize;
        (info->appReadCb).fxn = readFinishCb;
        (info->appReadCb).arg =(Ptr)info;
        (info->appWriteCb).fxn = writeFinishCb;
        (info->appWriteCb).arg = (Ptr)info;
        info->scalingFactor = 1;
    }

    /* Create channel handles */
    if (status == SYS_OK) {
        GIO_Attrs gioAttrs = GIO_ATTRS;
        info->gioInputChan = GIO_create(SWI_INPUT_CHANNEL, IOM_INPUT,
                                         NULL, NULL, &gioAttrs);
        info->gioOutputChan = GIO_create(SWI_OUTPUT_CHANNEL,
                                          IOM_OUTPUT, NULL, NULL, &gioAttrs);
        if ((info->gioInputChan == NULL) || (info->gioOutputChan == NULL)) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }

    /* Create SWI for sending and receiving data */
    if (status == SYS_OK) {
        swiAttrs.fxn = dataSWI;
        swiAttrs.arg0 = (Arg) info;
        swiAttrs.mailbox = INITIAL_DATA_MAILBOX_VAL;
        info->swiData = SWI_create(&swiAttrs);
        if (info->swiData == NULL) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }

    /* Create SWI for receiving messages */
    if (status == SYS_OK) {
        swiAttrs.fxn = msgSWI;
        swiAttrs.arg0 = (Arg) info;
        swiAttrs.mailbox = INITIAL_MSG_MAILBOX_VAL;
        info->swiMsg = SWI_create(&swiAttrs);
        if (info->swiMsg == NULL) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }

    /* Allocate input and output buffers */
    if (status == SYS_OK) {
        info->inputBuffer = MEM_calloc (DSPLINK_SEGID, info->bufferSize, DSPLINK_ALIGN);
        info->outputBuffer = MEM_calloc (DSPLINK_SEGID, info->bufferSize, DSPLINK_ALIGN);
        if ((info->inputBuffer == NULL) || (info->outputBuffer == NULL)) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }

    /* Create the message queue */
    if (status == SYS_OK) {
        /* Fill in the attributes for this message queue. */
        msgqAttrs.notifyHandle = info->swiMsg;
        msgqAttrs.post         = (MSGQ_Post)SWI_post;
        msgqAttrs.pend         = NULL;

        /* Creating message queue */
        info->msgqHandle = MSGQ_create(SWI_DSP_MSGQNAME, &msgqAttrs);
        if (info->msgqHandle == NULL) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }

    return status;
}


/** ============================================================================
 *  @func   SWISCALE_execute
 *
 *  @desc   Execute phase of SWISCALE application. It gets the application
 *          started after that SWIs get posted automatically.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns SWISCALE_execute(SWISCALE_TransferInfo * info)
{
    Int iomStatus;
    Uns status     = SYS_OK;

    /* Submit a read request */
    iomStatus = GIO_submit(info->gioInputChan, IOM_READ,
                           info->inputBuffer, &(info->bufferSize),
                           &(info->appReadCb));

    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO;
        SET_FAILURE_REASON(status);
    }

    /* We don't write for the first time so clearing write bit
     * from SWI's mailbox */
    SWI_andn(info->swiData, WRITE_MAILBOX_MASK);

    return status;
}


/** ============================================================================
 *  @func   SWISCALE_delete
 *
 *  @desc   Delete phase of SWISCALE application. Deallocates all the
 *          resources that were allocated during create phase.
 *
 *  @modif  None.
 *  ============================================================================
 */
Uns SWISCALE_delete(SWISCALE_TransferInfo * info)
{
    MSGQ_Status msgqStatus = MSGQ_SUCCESS;
    Uns         status     = SYS_OK;
    Int         iomStatus;

    if (info != NULL) {
        /* Delete data SWI */
        if (info->swiData != NULL) {
            SWI_delete(info->swiData);
        }

        /* Delete data SWI */
        if (info->swiMsg != NULL) {
            SWI_delete(info->swiMsg);
        }

        /* Delete the message queue */
        if (info->msgqHandle != NULL) {
            msgqStatus = MSGQ_delete(info->msgqHandle);
            if ((status == SYS_OK) && (msgqStatus != MSGQ_SUCCESS)) {
                status = SYS_EINVAL;
                SET_FAILURE_REASON(status);
            }
        }

        /* Delete GIO input channel */
        if (info->gioInputChan != NULL) {
            iomStatus = GIO_delete(info->gioInputChan);
            if (iomStatus != IOM_COMPLETED) {
                status = SYS_EBADIO;
                SET_FAILURE_REASON(status);
            }
        }

        /* Delete GIO output channel */
        if (info->gioOutputChan != NULL) {
            iomStatus = GIO_delete(info->gioOutputChan);
            if (iomStatus != IOM_COMPLETED) {
                status = SYS_EBADIO;
                SET_FAILURE_REASON(status);
            }
        }

        /* Deallocate inputBuffer */
        if (info->inputBuffer != NULL) {
            if (!MEM_free(DSPLINK_SEGID, info->inputBuffer, info->bufferSize)) {
                status = SYS_EALLOC;
                SET_FAILURE_REASON(status);
            }
        }

        /* Deallocate outputBuffer */
        if (info->outputBuffer != NULL) {
            if (!MEM_free(DSPLINK_SEGID, info->outputBuffer, info->bufferSize)) {
                status = SYS_EALLOC;
                SET_FAILURE_REASON(status);
            }
        }

        /* Deallocate info structure */
        if (!MEM_free(DSPLINK_SEGID, info, sizeof (SWISCALE_TransferInfo))) {
            status = SYS_EALLOC;
            SET_FAILURE_REASON(status);
        }
    }
    else {
        status = SYS_EALLOC;
    }

    /* Finalize MSGQ module, allocators and transports */
    MSGQ_exit () ;
    MQTDSPLINK_exit () ;
    MQTBIOSQUE_exit () ;
    MQABUF_exit () ;

    return status;
}


/** ----------------------------------------------------------------------------
 *  @func   readFinishCb
 *
 *  @desc   Function to be registered as callback funtion for IOM_READ command
 *          of GIO_submit.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void readFinishCb(Ptr arg, Int status, Ptr bufp, Uns size)
{
    SWISCALE_TransferInfo *info =(SWISCALE_TransferInfo *)arg;

    info->readMAUs = size;
    if(status != IOM_COMPLETED) {
        SET_FAILURE_REASON(status);
    }
    /* Give indication to SWI that READ is over */
    SWI_andn(info->swiData, READ_MAILBOX_MASK);
}


/** ----------------------------------------------------------------------------
 *  @func   writeFinishCb
 *
 *  @desc   Function to registered as callback funtion for IOM_WRITE
 *          command of GIO_submit.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void writeFinishCb(Ptr arg, Int status, Ptr bufp, Uns size)
{
    SWISCALE_TransferInfo *info =(SWISCALE_TransferInfo *)arg;

    if(status != IOM_COMPLETED) {
        SET_FAILURE_REASON(status);
    }
    /* Give indication to SWI that WRITE is over */
    SWI_andn(info->swiData, WRITE_MAILBOX_MASK);
}


/** ----------------------------------------------------------------------------
 *  @func   dataSWI
 *
 *  @desc   SWI entry point. This SWI runs for the following reasons:
 *             - IOM_READ and IOM_WRITE requests both get completed.
 *               It puts does buffer processing and starts the next IOM_READ
 *               and IOM_WRITE requests.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void dataSWI(Arg arg0, Arg arg1)
{
    SWISCALE_TransferInfo * info      = (SWISCALE_TransferInfo *)arg0;
    Uns                     status    = SYS_OK;
    Uns                     numMAUs   = info->readMAUs;
    Int                     iomStatus;
    Uns                     i;

    /* Do processing of data here */
    for (i=0; i < numMAUs; i++) {
        info->outputBuffer[i] = info->inputBuffer[i] *
                                info->scalingFactor;
    }

    /* Submit a Read data request */
    iomStatus = GIO_submit(info->gioInputChan, IOM_READ,
                           info->inputBuffer, &(info->bufferSize),
                           &(info->appReadCb));
    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO;
        SET_FAILURE_REASON(status);
    }

    /* Submit a Write data request */
    if (status == SYS_OK) {
        iomStatus = GIO_submit(info->gioOutputChan, IOM_WRITE,
                               info->outputBuffer, &numMAUs,
                               &(info->appWriteCb));
    }
    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO;
        SET_FAILURE_REASON(status);
    }
}


/** ----------------------------------------------------------------------------
 *  @func   msgSWI
 *
 *  @desc   SWI entry point. This SWI runs for the following reasons:
 *             - When a GPP message arrives.
 *               The scaling factor is updated accordingly.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void msgSWI(Arg arg0, Arg arg1)
{
    SWISCALE_TransferInfo * info      = (SWISCALE_TransferInfo *)arg0;
    Uns                     status    = SYS_OK;
    MSGQ_Msg                msg;

    msg = MSGQ_get(info->msgqHandle, 0);
    while (msg != NULL) {
        if (MSGQ_getMsgId(msg) == SCALE_REQUEST_MSGID) {
            info->scalingFactor = ((SCALE_ScaleMsg *)msg)->scalingFactor;
        }
        else {
            status = SYS_EBADIO;
            SET_FAILURE_REASON(status);
        }
        MSGQ_free(msg);

        /* See if there are other messages to process */
        msg = MSGQ_get(info->msgqHandle, 0);
    }
}

