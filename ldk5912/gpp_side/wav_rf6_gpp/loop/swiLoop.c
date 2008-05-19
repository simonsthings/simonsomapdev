/** ============================================================================
 *  @file   swiLoop.c
 *
 *  @path   $(DSPLINK)/dsp/src/samples/loop/
 *
 *  @desc   This is simple LINK SWI application that does data transfer
 *          in loopback mode using GIO interface.  It opens two channels one
 *          for input and one for output. Channel numbers configurable. Maximum
 *          buffer size of input and output channels is also configurable
 *          though #defines in header file. It is important whenever one makes
 *          changes in the configuration of this application. He should also
 *          make changes on the corresponding GPP side application sources.
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
#include <iom.h>
#include <gio.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <failure.h>
#include <platform.h>
#include <dsplink.h>

/*  ----------------------------------- Sample Headers              */
#include <loop_config.h>
#include <swiLoop.h>


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   FILEID needed for SET_FAILURE_REASON macro.
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
 *  @const  WRITE_MAILBOX_MASK
 *
 *  @desc   Initialial mailbox value.
 *  ============================================================================
 */
#define INITIAL_MAILBOX_VAL (READ_MAILBOX_MASK | WRITE_MAILBOX_MASK)

/** ============================================================================
 *  @name   xferBufSize
 *
 *  @desc   Size of the buffer size for SWI based loopback.
 *  ============================================================================
 */
extern Uint32 xferBufSize ;


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
static Void readFinishCb  (Ptr arg, Int status, Ptr bufp, Uns size) ;
static Void writeFinishCb (Ptr arg, Int status, Ptr bufp, Uns size) ;

/** ----------------------------------------------------------------------------
 *  @func   loopbackSWI
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
static Void loopbackSWI (Arg arg0, Arg arg1) ;


/** ============================================================================
 *  @func   SWILOOP_create
 *
 *  @desc   Create phase of SWILOOP application. It allocates
 *          SWILOOP_TransferInfo structure and intializes it with configured
 *          values.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int SWILOOP_create (SWILOOP_TransferInfo ** infoPtr)
{
    Int                     status      = SYS_OK ;
    SWI_Attrs               swiAttrs    = SWI_ATTRS ;
    SWILOOP_TransferInfo *  info;

    /* Allocate SWILOOP_TransferInfo structure */
    *infoPtr = MEM_calloc (DSPLINK_SEGID,
                           sizeof (SWILOOP_TransferInfo),
                           DSPLINK_ALIGN) ;
    if (*infoPtr == NULL) {
        status = SYS_EALLOC ;
        SET_FAILURE_REASON (status) ;
    }
    else {
        info = *infoPtr ;
    }

    /* Initialize SWILOOP_TransferInfo structure */
    if (status == SYS_OK) {
        info->bufferSize = xferBufSize ;
        (info->appReadCb).fxn  = readFinishCb ;
        (info->appReadCb).arg  = (Ptr) info ;
        (info->appWriteCb).fxn = writeFinishCb ;
        (info->appWriteCb).arg = (Ptr) info ;
    }

    /* Create channel handles */
    if (status == SYS_OK) {
        GIO_Attrs gioAttrs  = GIO_ATTRS ;
        info->gioInputChan  = GIO_create (INPUT_CHANNEL,
                                          IOM_INPUT,
                                          NULL,
                                          NULL,
                                          &gioAttrs) ;
        info->gioOutputChan = GIO_create (OUTPUT_CHANNEL,
                                          IOM_OUTPUT,
                                          NULL,
                                          NULL,
                                          &gioAttrs) ;
        if (   (info->gioInputChan  == NULL)
            || (info->gioOutputChan == NULL)) {
            status = SYS_EALLOC ;
            SET_FAILURE_REASON (status) ;
        }
    }

    /* Create SWI for sending and receiving data */
    if (status == SYS_OK) {
        swiAttrs.fxn     = loopbackSWI ;
        swiAttrs.arg0    = (Arg) info ;
        swiAttrs.mailbox = INITIAL_MAILBOX_VAL ;
        info->swi = SWI_create (&swiAttrs) ;
        if (info->swi == NULL) {
            status = SYS_EALLOC ;
            SET_FAILURE_REASON (status) ;
        }
    }

    /* Allocate input and output buffers */
    if (status == SYS_OK) {
        status = DSPLINK_alloc (POOLID,
                                (Ptr *) &(info->inputBuffer),
                                info->bufferSize) ;
        if (status == SYS_OK) {
            status = DSPLINK_alloc (POOLID,
                                    (Ptr *) &(info->outputBuffer),
                                    info->bufferSize) ;
            if (status != SYS_OK) {
                SET_FAILURE_REASON (status) ;
            }
        }
        else {
            SET_FAILURE_REASON (status) ;
        }
    }

    return status ;
}


/** ============================================================================
 *  @func   SWILOOP_execute
 *
 *  @desc   Execute phase of SWILOOP application. It get the application
 *          started after that SWIs get posted automatically.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int SWILOOP_execute (SWILOOP_TransferInfo * info)
{
    Int status = SYS_OK ;
    Int iomStatus ;

    /* Submit a read request */
    iomStatus = GIO_submit (info->gioInputChan,
                            IOM_READ,
                            info->inputBuffer,
                            (size_t *) &(info->bufferSize),
                            &(info->appReadCb)) ;

    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO ;
        SET_FAILURE_REASON (status) ;
    }

    /* We don't write for the first time so clearing write bit
     * from SWI's mailbox */
    SWI_andn (info->swi, WRITE_MAILBOX_MASK) ;

    return status ;
}


/** ============================================================================
 *  @func   SWILOOP_delete
 *
 *  @desc   Delete phase of SWILOOP application. Deallocates all the
 *          resources that were allocated during create phase.
 *
 *  @modif  None.
 *  ============================================================================
 */
Int SWILOOP_delete (SWILOOP_TransferInfo * info)
{
    Int status = SYS_OK ;
    Int iomStatus ;

    if (info != NULL) {
        /* Delete SWI */
        if (info->swi != NULL) {
            SWI_delete (info->swi) ;
        }

        /* Delete GIO input channel */
        if (info->gioInputChan != NULL) {
            iomStatus = GIO_delete (info->gioInputChan) ;
            if (iomStatus != IOM_COMPLETED) {
                status = SYS_EBADIO ;
                SET_FAILURE_REASON (status) ;
            }
        }

        /* Delete GIO output channel */
        if (info->gioOutputChan != NULL) {
            iomStatus = GIO_delete (info->gioOutputChan) ;
            if (iomStatus != IOM_COMPLETED) {
                status = SYS_EBADIO ;
                SET_FAILURE_REASON (status) ;
            }
        }

        /* Deallocate inputBuffer */
        if (info->inputBuffer != NULL) {
            DSPLINK_free (POOLID, info->inputBuffer, info->bufferSize) ;
        }

        /* Deallocate outputBuffer */
        if (info->outputBuffer != NULL) {
            DSPLINK_free (POOLID, info->outputBuffer, info->bufferSize) ;
        }

        /* Deallocate info structure */
        if (!MEM_free (DSPLINK_SEGID, info, sizeof (SWILOOP_TransferInfo))) {
            status = SYS_EALLOC ;
            SET_FAILURE_REASON (status) ;
        }
    }
    else {
        status = SYS_EALLOC ;
    }

    return status ;
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
static Void readFinishCb (Ptr arg, Int status, Ptr bufp, Uns size)
{
    SWILOOP_TransferInfo * info = (SWILOOP_TransferInfo *) arg ;

    /* Received full buffer from the GPP */
    info->inputBuffer = bufp ;

    info->readWords = size ;
    if (status != IOM_COMPLETED) {
        SET_FAILURE_REASON (status) ;
    }
    /* Give indication to SWI that READ is over */
    SWI_andn (info->swi, READ_MAILBOX_MASK) ;
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
static Void writeFinishCb (Ptr arg, Int status, Ptr bufp, Uns size)
{
    SWILOOP_TransferInfo *info = (SWILOOP_TransferInfo *) arg ;

    (Void) size ; /* To remove compiler warning */

    /* Received empty buffer from the GPP */
    info->outputBuffer = bufp ;

    if (status != IOM_COMPLETED) {
        SET_FAILURE_REASON(status) ;
    }
    /* Give indication to SWI that WRITE is over */
    SWI_andn (info->swi, WRITE_MAILBOX_MASK) ;
}


/** ----------------------------------------------------------------------------
 *  @func   loopbackSWI
 *
 *  @desc   SWI entry point. This SWI runs every time both IOM_READ and
 *          IOM_WRITE request get completed. It puts does processing and put
 *          next IOM_READ and IOM_WRITE requests.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void loopbackSWI (Arg arg0, Arg arg1)
{
    Int                    status          = SYS_OK;
    SWILOOP_TransferInfo * info            = (SWILOOP_TransferInfo *) arg0;
    Uns                    numWordsToWrite = info->readWords;
    Int                    iomStatus;
    Uns i;

    (Void) arg1 ; /* To remove compiler warning */

    /* Do processing of data here */
    for (i = 0 ; i < info->readWords ; i++) {
        info->outputBuffer [i] = info->inputBuffer [i] ;
    }

    /* Submit a Read data request */
    iomStatus = GIO_submit (info->gioInputChan,
                            IOM_READ,
                            info->inputBuffer,
                            (size_t *) &(info->bufferSize),
                            &(info->appReadCb)) ;

    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO ;
        SET_FAILURE_REASON (status) ;
    }

    /* Submit a Write data request */
    if (status == SYS_OK) {
        iomStatus = GIO_submit (info->gioOutputChan,
                                IOM_WRITE,
                                info->outputBuffer,
                                &numWordsToWrite,
                                &(info->appWriteCb)) ;
    }
    if (iomStatus != IOM_PENDING) {
        status = SYS_EBADIO ;
        SET_FAILURE_REASON (status) ;
    }
}
