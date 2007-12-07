/** ============================================================================
 *  @file   mqtdsplink.c
 *
 *  @path   $(DSPLINK)\dsp\src\mqt
 *
 *  @desc   Implementation of the remote MQT for GPP-DSP communication.
 *          This module implements the transport interface as required by the
 *          MSGQ model of DSP/BIOS(TM).
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
#include <hwi.h>
#include <mem.h>
#include <sem.h>
#include <que.h>
#include <atm.h>
#include <gio.h>
#include <string.h>
#include <msgq.h>
#include <msgq_mqt.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <dsplink.h>
#include <platform.h>
#include <dbc.h>
#include <mqtdsplink.h>
#include <_mqtdsplink.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  MQTDSPLINK_MEM_SEGID
 *
 *  @desc   Default segment ID to be used for allocating all memory required
 *          for the DSPLINK MQT.
 *  ============================================================================
 */
#define MQTDSPLINK_MEM_SEGID  0

/** ============================================================================
 *  @const  MQTDSPLINK_MEM_ALIGN
 *
 *  @desc   Default alignment to be used for internal memory allocations.
 *  ============================================================================
 */
#define MQTDSPLINK_MEM_ALIGN  0

/** ============================================================================
 *  @const  MAX_MSGQ_NAME_LEN
 *
 *  @desc   Maximum length of the MSGQ names.
 *  ============================================================================
 */
#define MAX_MSGQ_NAME_LEN  20

/** ============================================================================
 *  @const  MAX_DEVICE_NAME_LEN
 *
 *  @desc   Maximum length of the device name.
 *  ============================================================================
 */
#define MAX_DEVICE_NAME_LEN  (MAX_DRIVER_NAME_LEN + 2)

/** ============================================================================
 *  @const  ISR_DISABLE
 *
 *  @desc   To disable interrupts.
 *  ============================================================================
 */
#define ISR_DISABLE  HWI_disable()

/** ============================================================================
 *  @const  ISR_RESTORE
 *
 *  @desc   To restore interrupts.
 *  ============================================================================
 */
#define ISR_RESTORE(key)  HWI_restore(key)

/** ============================================================================
 *  @func   atoi
 *
 *  @desc   Converts string to integer value.
 *
 *  @arg    str
 *              Pointer to string.
 *
 *  @ret    integer value
 *              Converted integer value.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
extern
int
atoi (const char * str) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_open
 *
 *  @desc   Opens the remote MQT and configures it according to the user
 *          attributes.
 *
 *  @arg    mqtHandle
 *              Handle to the MSGQ transport object.
 *  @arg    numLocalMsgQueues
 *              Number of local MSGQs.
 *  @arg    localMsgQueues
 *              Array of local MSGQ objects.
 *
 *  @ret    MSGQ_SUCCESS
 *              Component successfully opened.
 *          MSGQ_EMEMORY
 *              Failure during memory operation.
 *          MSGQ_EFAILURE
 *              Failure in opening the component.
 *
 *  @enter  The handle to the MSGQ transport object must be valid.
 *          The pointer to the array of local MSGQs must be valid.
 *          The pointer to the transport attributes must be valid.
 *
 *  @leave  On success, the mqtInfo field in the MSGQ transport object is set to
 *          a non-NULL value.
 *          On failure, the mqtInfo field in the MSGQ transport object is set to
 *          NULL.
 *
 *  @see    MQTDSPLINK_Params, MQTDSPLINK_State, MQTDSPLINK_close ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_open (MSGQ_TransportHandle  mqtHandle,
                 Uint16                numLocalMsgQueues,
                 MSGQ_Handle           localMsgQueues) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_close
 *
 *  @desc   Closes the remote MQT, and cleans up its state object.
 *
 *  @arg    mqtHandle
 *              Handle to the MSGQ transport object.
 *
 *  @ret    MSGQ_SUCCESS
 *              Component successfully closed.
 *          MSGQ_EMEMORY
 *              Failure during memory operation.
 *          MSGQ_EFAILURE
 *              Failure in closing the component.
 *
 *  @enter  The handle to the MSGQ transport object must be valid.
 *
 *  @leave  The mqtInfo field in the MSGQ transport object is set to NULL.
 *
 *  @see    MQTDSPLINK_State, MQTDSPLINK_open ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_close (MSGQ_TransportHandle mqtHandle) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_locate
 *
 *  @desc   Attempts to locate the specified message queue on the remote
 *          processor.
 *
 *  @arg    mqtHandle
 *              Handle to the MSGQ transport object.
 *  @arg    queueName
 *              Name of the MSGQ to be located.
 *  @arg    msgqHandle
 *              Location to receive the handle to the located message queue.
 *  @arg    attrs
 *              Optional attributes for location of the MSGQ.
 *
 *  @ret    MSGQ_SUCCESS
 *              The message queue has been successfully located.
 *          MSGQ_ENOTFOUND
 *              The message queue does not exist on the remote processor.
 *          MSGQ_ETIMEOUT
 *              Timeout during location of the MSGQ.
 *          MSGQ_EMEMORY
 *              Failure during memory operation.
 *          MSGQ_EFAILURE
 *              Failure during location of the MSGQ.
 *
 *  @enter  The handle to the MSGQ transport object must be valid.
 *          The queueName must be valid.
 *          The msgqHandle and locator parameters must be valid depending on
 *          whether the locate is synchronous or asynchronous
 *
 *  @leave  On success, for synchronous locate, the *msgqHandle is set to a
 *          non-NULL value.
 *          On failure, for synchronous locate, the *msgqHandle is set to NULL.
 *
 *  @see    MQTDSPLINK_Obj, MQTDSPLINK_release ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_locate (MSGQ_TransportHandle mqtHandle,
                   String               queueName,
                   MSGQ_Handle *        msgqHandle,
                   MSGQ_LocateAttrs *   attrs) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_release
 *
 *  @desc   This function releases the MSGQ located during an earlier
 *          MSGQ_locate () or MSGQ_getReplyHandle () call.
 *
 *  @arg    msgqHandle
 *              Handle to the MSGQ to be released.
 *
 *  @ret    MSGQ_SUCCESS
 *              The message queue has been successfully released.
 *          MSGQ_EFAILURE
 *              Failure in releasing the message queue.
 *
 *  @enter  The handle to the message queue must be valid.
 *
 *  @leave  Status field in the MSGQ object is set to MSGQ_EMPTY.
 *
 *  @see    MQTDSPLINK_locate ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_release (MSGQ_Handle msgqHandle) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_put
 *
 *  @desc   Sends a message to the specified remote MSGQ.
 *
 *  @arg    msgqHandle
 *              Handle to the MSGQ to which the message is to be sent.
 *  @arg    msg
 *              Pointer to the message to be sent.
 *
 *  @ret    MSGQ_SUCCESS
 *              The message has been successfully sent.
 *          MSGQ_EFAILURE
 *              Failure in sending the message.
 *
 *  @enter  The handle to the message queue must be valid.
 *          The pointer to the message must be valid.
 *
 *  @leave  None
 *
 *  @see    MQTDSPLINK_sendMessage ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_put (MSGQ_Handle msgqHandle, MSGQ_Msg msg) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_getReplyHandle
 *
 *  @desc   Returns the MSGQ handle to be used for replying to a message
 *          received from a remote application.
 *
 *  @arg    mqtHandle
 *              Handle to the MSGQ transport object.
 *  @arg    msg
 *              Message whose reply handle is to be obtained.
 *
 *  @ret    Handle to the reply message queue
 *              The source MSGQ ID has been successfully obtained for reply.
 *          NULL
 *              No source MSGQ ID has been specified, or an error occurred.
 *
 *  @enter  The handle to the MSGQ transport object must be valid.
 *          The pointer to the message must be valid.
 *
 *  @leave  If the source ID is MSGQ_INVALIDMSGQID, return NULL, otherwise
 *          return a non-NULL MSGQ handle.
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Handle
MQTDSPLINK_getReplyHandle (MSGQ_TransportHandle mqtHandle,
                           MSGQ_Msg             msg) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_putCallback
 *
 *  @desc   Implements the callback that runs when the message to be sent to a
 *          remote MSGQ has been transferred across the physical link.
 *
 *  @arg    arg
 *              Optional argument sent to the submit function.
 *              NULL in this case.
 *  @arg    status
 *              Status of completion of the transfer packet.
 *  @arg    bufPtr
 *              Message buffer on which the I/O has completed.
 *  @arg    size
 *              Number of MADUs of I/O completed.
 *
 *  @ret    None.
 *
 *  @enter  arg must be a valid pointer.
 *          bufPtr must be a valid buffer pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static
Void
MQTDSPLINK_putCallback (Ptr arg, Int status, Ptr bufPtr, Uns size) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_getCallback
 *
 *  @desc   Implements the callback that runs when the message has been received
 *          from the GPP.
 *
 *  @arg    arg
 *              Optional argument sent to the submit function.
 *              NULL in this case.
 *  @arg    bufStatus
 *              Status of completion of the transfer packet.
 *  @arg    bufPtr
 *              Message buffer on which the I/O has completed.
 *  @arg    size
 *              Number of MADUs of I/O completed.
 *
 *  @ret    None.
 *
 *  @enter  arg must be a valid pointer.
 *          bufPtr must be a valid buffer pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static
Void
MQTDSPLINK_getCallback (Ptr arg, Int bufStatus, Ptr bufPtr, Uns size) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_createRemoteMsgq
 *
 *  @desc   Create the remote MSGQ for specified MSGQ ID.
 *
 *  @arg    mqtHandle
 *              Handle to the MSGQ transport object.
 *  @arg    msgqId
 *              ID of the message queue to be created.
 *
 *  @ret    MSGQ_SUCCESS
 *              The remote MSGQ has been successfully created.
 *          MSGQ_EMEMORY
 *              Failure during memory allocation
 *          MSGQ_EFAILURE
 *              Failure during creation of the remote MSGQ.
 *
 *  @enter  The handle to the MSGQ transport object must be valid.
 *          The MSGQ Id must be valid.
 *
 *  @leave  None
 *
 *  @see    MQTDSPLINK_open ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_createRemoteMsgq (MSGQ_TransportHandle mqtHandle, Uint16 msgqId) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_deleteRemoteMsgq
 *
 *  @desc   Delete the remote MSGQ for specified MSGQ ID.
 *
 *  @arg    mqtState
 *              Handle to the MQT state object.
 *  @arg    msgqId
 *              ID of the message queue to be deleted.
 *
 *  @ret    MSGQ_SUCCESS
 *              The remote MSGQ has been successfully deleted.
 *          MSGQ_EMEMORY
 *              Failure during memory operation
 *          MSGQ_EFAILURE
 *              Failure during deletion of the remote MSGQ.
 *
 *  @enter  The handle to the state object must be valid.
 *          The MSGQ Id must be valid.
 *
 *  @leave  None
 *
 *  @see    MQTDSPLINK_createRemoteMsgq ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_deleteRemoteMsgq (MQTDSPLINK_State * mqtState, Uint16 msgqId) ;


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_sendMessage
 *
 *  @desc   Sends a message to the remote MQT.
 *
 *  @arg    mqtState
 *              Handle to the MQT state object.
 *  @arg    msgqId
 *              Pointer to the message to be sent.
 *
 *  @ret    MSGQ_SUCCESS
 *              The message has been successfully sent.
 *          MSGQ_EFAILURE
 *              Failure in sending the message.
 *
 *  @enter  The handle to the state object must be valid.
 *          The pointer to the message must be valid.
 *
 *  @leave  None
 *
 *  @see    MQTDSPLINK_put (), MQTDSPLINK_putCallback ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_sendMessage (MQTDSPLINK_State * mqtState, MSGQ_Msg msg) ;


/** ----------------------------------------------------------------------------
 *  @func   numToStr
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @arg    strNum
 *              Converted string.
 *  @arg    num
 *              1 to 2 digit number to be converted into a string.
 *
 *  @ret    None.
 *
 *  @enter  strNum must be valid.
 *
 *  @leave  None
 *
 *  @see    MQTDSPLINK_open ()
 *  ----------------------------------------------------------------------------
 */
static
Void
numToStr (String strNum, Uns num) ;


/** ============================================================================
 *  @name   MQTDSPLINK_FXNS
 *
 *  @desc   DSPLINK remote MQT function pointer table.
 *  ============================================================================
 */
MSGQ_TransportFxns MQTDSPLINK_FXNS = {
    &MQTDSPLINK_open,
    &MQTDSPLINK_close,
    MSGQ_MQTCREATENOTIMPL,
    &MQTDSPLINK_locate,
    MSGQ_MQTDELETENOTIMPL,
    &MQTDSPLINK_release,
    MSGQ_MQTGETNOTIMPL,
    &MQTDSPLINK_put,
    &MQTDSPLINK_getReplyHandle
} ;


/** ============================================================================
 *  @name   MQTDSPLINK_PARAMS
 *
 *  @desc   Default parameters structure for the DSPLINK MQT.
 *  ============================================================================
 */
static MQTDSPLINK_Params MQTDSPLINK_PARAMS = {
                    16,                            /* max remote MSGQs */
                    MQTDSPLINK_CTRLMSG_SIZE,       /* max message size */
                    0} ;                           /* default MQA ID   */


/** ============================================================================
 *  Create named sections for all MQTDSPLINK functions to allow specific memory
 *  placement.
 *  ============================================================================
 */
#pragma CODE_SECTION (MQTDSPLINK_init,                ".text:init")
#pragma CODE_SECTION (MQTDSPLINK_exit,                ".text:exit")
#pragma CODE_SECTION (MQTDSPLINK_open,                ".text:MQTDSPLINK_open")
#pragma CODE_SECTION (MQTDSPLINK_createRemoteMsgq,    ".text:MQTDSPLINK_open")
#pragma CODE_SECTION (MQTDSPLINK_close,               ".text:MQTDSPLINK_close")
#pragma CODE_SECTION (MQTDSPLINK_deleteRemoteMsgq,    ".text:MQTDSPLINK_close")

#pragma CODE_SECTION (MQTDSPLINK_locate,             ".text:MQTDSPLINK_locate")
#pragma CODE_SECTION (MQTDSPLINK_release,            ".text:MQTDSPLINK_release")
#pragma CODE_SECTION (MQTDSPLINK_getReplyHandle, \
                                              ".text:MQTDSPLINK_getReplyHandle")

#pragma CODE_SECTION (MQTDSPLINK_put,         ".text:MQTDSPLINK_msgXfer")
#pragma CODE_SECTION (MQTDSPLINK_putCallback, ".text:MQTDSPLINK_msgXfer")
#pragma CODE_SECTION (MQTDSPLINK_getCallback, ".text:MQTDSPLINK_msgXfer")
#pragma CODE_SECTION (MQTDSPLINK_sendMessage, ".text:MQTDSPLINK_msgXfer")



/** ============================================================================
 *  @func   MQTDSPLINK_init
 *
 *  @desc   Performs global initialization of the remote MQT.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
MQTDSPLINK_init  ()
{
    /*  No initialization code is required */
}


/** ============================================================================
 *  @func   MQTDSPLINK_exit
 *
 *  @desc   Performs global finalization of the remote MQT.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
MQTDSPLINK_exit ()
{
    /*  No finalization code is required */
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_open
 *
 *  @desc   Opens the remote MQT and configures it according to the user
 *          attributes.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_open (MSGQ_TransportHandle  mqtHandle,
                 Uint16                numLocalMsgQueues,
                 MSGQ_Handle           localMsgQueues)
{
    MSGQ_Status          status     = MSGQ_SUCCESS ;
    MQTDSPLINK_Params *  params     = mqtHandle->mqtParams ;
    GIO_Attrs            gioAttrs   = GIO_ATTRS ;
    Char                 msgChnlFmGpp [MAX_DEVICE_NAME_LEN] ;
    Char                 msgChnlToGpp [MAX_DEVICE_NAME_LEN] ;
    MQTDSPLINK_State *   mqtState ;
    Char                 strChnlId [3] ;
    Int                  iomStatus ;
    Uint16               i ;
    Uns                  msgSize ;

    DBC_require (mqtHandle      != NULL) ;
    DBC_require (localMsgQueues != NULL) ;

    /*  If no attributes are specified, use the default ones. */
    if (params == NULL) {
        params = (MQTDSPLINK_Params *) &MQTDSPLINK_PARAMS ;
    }

    /*  Allocate and initialize the MQT state object */
    mqtState = MEM_calloc (MQTDSPLINK_MEM_SEGID,
                           sizeof (MQTDSPLINK_State),
                           MQTDSPLINK_MEM_ALIGN) ;
    if (mqtState != MEM_ILLEGAL) {
        mqtHandle->mqtInfo        = (Ptr) mqtState ;
        mqtState->mqtId           = mqtHandle->mqtId ;
        mqtState->numRemoteMsgq   = params->maxNumMsgq ;
        mqtState->maxMsgSize      = params->maxMsgSize ;
        mqtState->numLocalMsgq    = numLocalMsgQueues ;
        mqtState->localMsgqs      = localMsgQueues ;
        mqtState->getCallback.fxn = &MQTDSPLINK_getCallback ;
        mqtState->putCallback.fxn = &MQTDSPLINK_putCallback ;
        /*  Send the state object handle as argument to the callback
         *  functions
         */
        mqtState->getCallback.arg = (Ptr) mqtState ;
        mqtState->putCallback.arg = (Ptr) mqtState ;
        mqtState->defaultMqaId    = params->defaultMqaId ;
        QUE_new (&(mqtState->msgQueue)) ;

        /* Allocate the array of remote MSGQ objects */
        mqtState->remoteMsgqs = MEM_calloc (
                                       MQTDSPLINK_MEM_SEGID,
                                       (params->maxNumMsgq * sizeof (MSGQ_Obj)),
                                       MQTDSPLINK_MEM_ALIGN) ;
        if (mqtState->remoteMsgqs != MEM_ILLEGAL) {
            for (i = 0 ; i < params->maxNumMsgq ; i++) {
                status = MQTDSPLINK_createRemoteMsgq (mqtHandle, i) ;
                if (status != MSGQ_SUCCESS) {
                    break ;
                }
            }

            if (status == MSGQ_SUCCESS) {
                /* Allocate the buffer used for priming the input channel */
                mqtState->getBuffer = MEM_alloc (MQTDSPLINK_MEM_SEGID,
                                                 params->maxMsgSize,
                                                 MQTDSPLINK_MEM_ALIGN) ;
                if (mqtState->getBuffer == MEM_ILLEGAL) {
                    status = MSGQ_EMEMORY ;
                }
            }
        }
        else {
            status = MSGQ_EMEMORY ;
        }
    }
    else {
        status = MSGQ_EMEMORY ;
    }

    if (status == MSGQ_SUCCESS) {
        /*  Set the physical link to be used */
        strcpy (msgChnlFmGpp, DSPLINK_DRIVER_NAME) ;
        numToStr (strChnlId, ID_MSGCHNL_FM_GPP) ;
        strcat (msgChnlFmGpp, strChnlId) ;

        strcpy (msgChnlToGpp, DSPLINK_DRIVER_NAME) ;
        numToStr (strChnlId, ID_MSGCHNL_TO_GPP) ;
        strcat (msgChnlToGpp, strChnlId) ;

        /*  Create GIO channels for messaging */
        mqtState->outChan = GIO_create (msgChnlToGpp, /* Device name */
                                        IOM_OUTPUT,   /* Output mode */
                                        NULL,     /* status not required */
                                        NULL,     /* no device parameters */
                                        &gioAttrs) ; /* attributes */
        if (mqtState->outChan != NULL) {
            mqtState->inpChan = GIO_create (msgChnlFmGpp, /* Device name */
                                        IOM_INPUT,    /* Input mode */
                                        NULL,         /* status not required */
                                        NULL,         /* no device parameters */
                                        &gioAttrs) ;  /* attributes */
            if (mqtState->inpChan != NULL) {
                /* Prime input channel with the getBuffer */
                msgSize = (Uns) mqtState->maxMsgSize ;
                iomStatus = GIO_submit (mqtState->inpChan,  /* Device handle */
                              IOM_READ,                     /* Read command */
                              mqtState->getBuffer,          /* Buffer pointer */
                              &msgSize,                     /* Buffer size */
                              &(mqtState->getCallback)) ;   /* callback info */

                if (iomStatus != IOM_PENDING) {
                    status = MSGQ_EFAILURE ;
                }
            }
            else {
                status = MSGQ_EFAILURE ;
            }
        }
        else {
            status = MSGQ_EFAILURE ;
        }
    }

    /*  Cleanup any allocated resources on failure */
    if (status != MSGQ_SUCCESS) {
        if (mqtState != MEM_ILLEGAL) {
            MQTDSPLINK_close (mqtHandle) ;
        }
        mqtHandle->mqtInfo = NULL ;
    }

    DBC_ensure (    ((status == MSGQ_SUCCESS) && (mqtHandle->mqtInfo != NULL))
                ||  (status != MSGQ_SUCCESS)) ;
    DBC_ensure (    ((status != MSGQ_SUCCESS) && (mqtHandle->mqtInfo == NULL))
                ||  (status == MSGQ_SUCCESS)) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_close
 *
 *  @desc   Closes the remote MQT, and cleans up its state object.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_close (MSGQ_TransportHandle mqtHandle)
{
    MSGQ_Status          status      = MSGQ_SUCCESS ;
    MSGQ_Status          tmpStatus   = MSGQ_SUCCESS ;
    Bool                 freeStatus  = TRUE ;
    MQTDSPLINK_State *   mqtState ;
    QUE_Handle           queHandle ;
    MSGQ_Msg             msgBuffer ;
    Int                  iomStatus ;
    Uint16               i ;

    DBC_require (mqtHandle != NULL) ;

    mqtState = (MQTDSPLINK_State *) mqtHandle->mqtInfo ;
    DBC_assert (mqtState != NULL) ;

    if (mqtState->inpChan != NULL) {
        /* Flush the input channel to reclaim the getBuffer */
        iomStatus = GIO_flush (mqtState->inpChan) ;
        if (iomStatus != IOM_COMPLETED) {
            status = MSGQ_EFAILURE ;
        }

        iomStatus = GIO_delete (mqtState->inpChan) ;
        mqtState->inpChan = NULL ;
        if (iomStatus != IOM_COMPLETED) {
            status = MSGQ_EFAILURE ;
        }
    }

    if ((mqtState->getBuffer != NULL) && (mqtState->outChan != NULL)) {
        /*  Send a synchronous MQT shutdown message to the GPP */
        /*  Reuse the getBuffer */
        ((MSGQ_Msg)(mqtState->getBuffer))->dstId = ID_MQTDSPLINK_CTRL ;
        ((MSGQ_Msg)(mqtState->getBuffer))->msgId = MqtCmdExit ;
        ((MSGQ_Msg)(mqtState->getBuffer))->size  = MQTDSPLINK_CTRLMSG_SIZE ;
        iomStatus = GIO_write (mqtState->outChan,           /* Device handle */
                               mqtState->getBuffer,         /* Buffer pointer */
                               (Uns *) &(mqtState->maxMsgSize)) ; /* Msg size */
        if (iomStatus != IOM_COMPLETED) {
            /*
             * This error is ignored since the GPP may have closed the transport
             * before this call.
             */
            status = MSGQ_SUCCESS ;
        }
    }

    if (mqtState->outChan != NULL) {
        iomStatus = GIO_flush (mqtState->outChan) ;
        if (iomStatus != IOM_COMPLETED) {
            status = MSGQ_EFAILURE ;
        }

        iomStatus = GIO_delete (mqtState->outChan) ;
        mqtState->outChan = NULL ;
        if (iomStatus != IOM_COMPLETED) {
            status = MSGQ_EFAILURE ;
        }
    }

    if (mqtState->getBuffer != NULL) {
        /* Free the getBuffer. */
        freeStatus =    MEM_free (MQTDSPLINK_MEM_SEGID,
                                  mqtState->getBuffer,
                                  mqtState->maxMsgSize)
                     && freeStatus ;
        mqtState->getBuffer = NULL ;
    }

    /*  Free all messages queued up on the MQT */
    queHandle = &(mqtState->msgQueue) ;
    while ((QUE_Handle) (msgBuffer = QUE_get (queHandle)) != queHandle) {
        MSGQ_free (msgBuffer) ;
    }

    if (mqtState->remoteMsgqs != NULL) {
        for (i = 0 ; i < mqtState->numRemoteMsgq ; i++) {
            tmpStatus = MQTDSPLINK_deleteRemoteMsgq (mqtState, i) ;
            if (tmpStatus != MSGQ_SUCCESS) {
                status = tmpStatus ;
            }
        }

        freeStatus =    MEM_free (MQTDSPLINK_MEM_SEGID,
                                  mqtState->remoteMsgqs,
                                  (mqtState->numRemoteMsgq * sizeof (MSGQ_Obj)))
                     && freeStatus ;
        mqtState->remoteMsgqs = NULL ;
    }

    freeStatus =    MEM_free (MQTDSPLINK_MEM_SEGID,
                              mqtState,
                              sizeof (MQTDSPLINK_State))
                 && freeStatus ;

    if (freeStatus == FALSE) {
        status = MSGQ_EMEMORY ;
    }

    mqtHandle->mqtInfo = NULL ;

    DBC_ensure (mqtHandle->mqtInfo == NULL) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_locate
 *
 *  @desc   Attempts to locate the specified message queue on the remote
 *          processor.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_locate (MSGQ_TransportHandle mqtHandle,
                   String               queueName,
                   MSGQ_Handle *        msgqHandle,
                   MSGQ_LocateAttrs *   attrs)
{
    MSGQ_Status          status     = MSGQ_ENOTFOUND ;
    Bool                 sync       = FALSE ;
    MSGQ_Handle          locateMsgq = NULL ;
    MQTDSPLINK_State *   mqtState ;
    MQTDSPLINK_CtrlMsg * locateMsg ;
    MQTDSPLINK_Handle    mqtRepository ;
    Uint16               len ;
    Char                 strMsgqId [3] ;
    Uint16               msgqId ;
    Uint16               timeout ;

    DBC_require (mqtHandle != NULL) ;
    DBC_require (queueName != NULL) ;
    DBC_require (   (   (attrs != NULL)                       /* sync */  \
                     && (attrs->replyHandle == NULL)                      \
                     && (msgqHandle != NULL))                             \
                 || (   (attrs != NULL)                       /* async */ \
                     && (attrs->replyHandle != NULL)                      \
                     && (msgqHandle == NULL))) ;

    mqtState = (MQTDSPLINK_State *) mqtHandle->mqtInfo ;
    DBC_assert (mqtState != NULL) ;

    len = strlen (DSPLINK_GPPMSGQ_NAME) ;

    /* Do not attempt to locate any MSGQs that are not meant for DSPLINK */
    if ((strncmp (queueName, DSPLINK_GPPMSGQ_NAME, len)) == 0) {
        /*  Check whether the location is synchronous or asynchronous */
        if (attrs->replyHandle == NULL) {
            timeout = attrs->timeout ;
            sync = TRUE ;
        }

        /*  Convert the MSGQ name into the MSGQ ID */
        strMsgqId [0] = queueName [len] ;
        strMsgqId [1] = queueName [len + 1] ;
        strMsgqId [2] = '\0' ;
        msgqId = atoi (strMsgqId) ;

        if (msgqId < mqtState->numRemoteMsgq) {
            locateMsgq = &(mqtState->remoteMsgqs [msgqId]) ;
            mqtRepository  = (MQTDSPLINK_Handle) (locateMsgq->mqtRepository) ;

            /* Allocate the locate message buffer */
            locateMsg = (MQTDSPLINK_CtrlMsg *) MSGQ_alloc (mqtState->defaultMqaId,
                                                           MQTDSPLINK_CTRLMSG_SIZE) ;
            if (locateMsg != NULL) {
                locateMsg->msgHeader.dstId = ID_MQTDSPLINK_CTRL ;
                locateMsg->msgHeader.msgId = MqtCmdLocate ;
                locateMsg->ctrlMsg.locateMsg.msgqId = msgqId ;
                if (sync == TRUE) {
                    /* Synchronous locate */
                    locateMsg->ctrlMsg.locateMsg.semHandle =
                                               (Uint32)&(mqtRepository->locateSem) ;
                }
                else {
                    /* Asynchronous locate */
                    locateMsg->ctrlMsg.locateMsg.semHandle   = NULL ;
                    locateMsg->ctrlMsg.locateMsg.replyHandle = (Uint32)
                                                               attrs->replyHandle ;
                    locateMsg->ctrlMsg.locateMsg.mqaId       = attrs->mqaId ;
                    locateMsg->ctrlMsg.locateMsg.arg         = (Uint32) attrs->arg ;
                }

                status = MQTDSPLINK_sendMessage (mqtState, (MSGQ_Msg) locateMsg) ;
            }
            else {
                status = MSGQ_EMEMORY ;
            }
        }
        else {
            status = MSGQ_ENOTFOUND ;
        }
    }

    if (status == MSGQ_SUCCESS) {
        if (sync == TRUE) {
            /*  Synchronous location */
            if (SEM_pend (&(mqtRepository->locateSem), timeout) == TRUE) {
                if (locateMsgq->status == MSGQ_INUSE) {
                    *msgqHandle = locateMsgq ;
                }
                else {
                    status = MSGQ_ENOTFOUND ;
                }
            }
            else {
                status = MSGQ_ETIMEOUT ;
            }
        }
    }

    /*  Cleanup on failure to locate the MSGQ */
    if (status != MSGQ_SUCCESS) {
        if (locateMsgq != NULL) {
            MQTDSPLINK_release (locateMsgq) ;
        }
        if (sync == TRUE) {
            *msgqHandle = NULL ;
        }
    }

    DBC_ensure (    (sync == FALSE)
                ||  (   (status == MSGQ_SUCCESS)
                     && (sync == TRUE)
                     && (*msgqHandle != NULL))
                ||  (status != MSGQ_SUCCESS)) ;

    DBC_ensure (    (sync == FALSE)
                ||  (   (status != MSGQ_SUCCESS)
                     && (sync == TRUE)
                     && (*msgqHandle == NULL))
                ||  (status == MSGQ_SUCCESS)) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_release
 *
 *  @desc   This function releases the MSGQ located during an earlier
 *          MSGQ_locate () or MSGQ_getReplyHandle () call.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_release (MSGQ_Handle msgqHandle)
{
    DBC_require (msgqHandle != NULL) ;

    msgqHandle->status = MSGQ_EMPTY ;

    DBC_ensure (msgqHandle->status == MSGQ_EMPTY) ;

    return MSGQ_SUCCESS ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_put
 *
 *  @desc   Sends a message to the specified remote MSGQ.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_put (MSGQ_Handle msgqHandle, MSGQ_Msg msg)
{
    MQTDSPLINK_State * mqtState ;

    DBC_require (msgqHandle != NULL) ;
    DBC_require (msg        != NULL) ;

    /* Verify that the destination MSGQ was previously successfully located */
    DBC_assert (msgqHandle->status == MSGQ_INUSE) ;

    mqtState = (MQTDSPLINK_State *) (msgqHandle->mqtHandle->mqtInfo) ;

    return MQTDSPLINK_sendMessage (mqtState, msg) ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_getReplyHandle
 *
 *  @desc   Returns the MSGQ handle to be used for replying to a message
 *          received from a remote application.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Handle
MQTDSPLINK_getReplyHandle (MSGQ_TransportHandle mqtHandle, MSGQ_Msg msg)
{
    MSGQ_Handle        replyHandle = NULL ;
    MQTDSPLINK_State * mqtState ;

    DBC_require (mqtHandle != NULL) ;
    DBC_require (msg       != NULL) ;

    mqtState = (MQTDSPLINK_State *) mqtHandle->mqtInfo ;
    DBC_assert (mqtState != NULL) ;

    if (msg->srcId != MSGQ_INVALIDMSGQID) {
        replyHandle = &(mqtState->remoteMsgqs [msg->srcId]) ;
        replyHandle->status = MSGQ_INUSE ;
    }

    DBC_ensure (    (   (msg->srcId != MSGQ_INVALIDMSGQID)
                     && (replyHandle != NULL))
                ||  (   (msg->srcId == MSGQ_INVALIDMSGQID)
                     && (replyHandle == NULL))) ;

    return replyHandle ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_putCallback
 *
 *  @desc   Implements the callback that runs when the message to be sent to a
 *          remote MSGQ has been transferred across the physical link.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
static
Void
MQTDSPLINK_putCallback (Ptr arg, Int status, Ptr bufPtr, Uns size)
{
    MQTDSPLINK_State * mqtState = (MQTDSPLINK_State *) arg ;
    volatile Int       iomStatus ; /* volatile to prevent compilation warning */
    MSGQ_Msg           msg ;
    QUE_Handle         queHandle ;
    Uns                msgSize ;

    DBC_require (arg    != NULL) ;
    DBC_require (bufPtr != NULL) ;

    if (status == IOM_COMPLETED) {
        queHandle = &(mqtState->msgQueue) ;
        /* Get the message from the queue. */
        msg = (MSGQ_Msg) QUE_get (queHandle) ;
        DBC_assert ((QUE_Handle) msg != queHandle) ;
        DBC_assert (msg == (MSGQ_Msg) bufPtr) ;

        /* Free the buffer. */
        MSGQ_free (msg) ;

        /*  If the queue is not empty, submit the next message to the output
         *  channel
         */
        if ((QUE_Handle) (msg = QUE_head (queHandle)) != queHandle) {
            msgSize = (Uns) msg->size ;
            iomStatus = GIO_submit (mqtState->outChan,      /* Device handle */
                                    IOM_WRITE,              /* Write command */
                                    msg,                    /* Buffer pointer */
                                    &msgSize,                  /* Buffer size */
                                    &(mqtState->putCallback)) ;/* callback */

            DBC_assert (iomStatus == IOM_PENDING) ;
        }
    }
    else if (msg->dstId != ID_MQTDSPLINK_CTRL) {
        /* If the I/O has failed/aborted for a user message, send an async error
         * message
         */
        MSGQ_sendErrorMsg ((Uint16) MSGQ_MQTFAILEDPUT,
                           (Arg) &(mqtState->remoteMsgqs [msg->dstId]),
                           (Arg) status) ;
    }
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_getCallback
 *
 *  @desc   Implements the callback that runs when the message has been received
 *          from the GPP.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
static
Void
MQTDSPLINK_getCallback (Ptr arg, Int bufStatus, Ptr bufPtr, Uns size)
{
    MSGQ_Status          status     = MSGQ_SUCCESS ;
    MQTDSPLINK_State *   mqtState   = (MQTDSPLINK_State *) arg ;
    MSGQ_Msg             getMsg     = (MSGQ_Msg) bufPtr ;
    volatile Int         iomStatus ;
    MSGQ_Handle          msgqHandle ;
    MSGQ_Handle          srcHandle ;
    Char                 strMsgqName [MAX_MSGQ_NAME_LEN] ;
    Char                 strMsgqId [3] ;
    Uint16               msgqId ;
    Uns                  msgSize ;

    DBC_require (arg    != NULL) ;
    DBC_require (bufPtr != NULL) ;

    /*  Do not perform any callback activities if the I/O has aborted */
    if (bufStatus == IOM_COMPLETED) {
        if (getMsg->dstId != ID_MQTDSPLINK_CTRL) {
            msgqId = getMsg->dstId ;

            strcpy (strMsgqName, DSPLINK_DSPMSGQ_NAME) ;
            numToStr (strMsgqId, msgqId) ;
            strcat (strMsgqName, strMsgqId) ;

            status = MSGQ_locateLocal (strMsgqName, &msgqHandle) ;

            /*  Regular message processing */
            if (   (status == MSGQ_SUCCESS)
                && (msgqHandle->status == MSGQ_INUSE)) {
                MSGQ_Msg newMsg ;
                /* Allocate a buffer of the actual received size */
                newMsg = MSGQ_alloc (getMsg->mqaId, getMsg->size) ;
                if (newMsg == NULL) {
                    /* Try allocating from the default MQA ID, in case of
                     * mismatch between the MQAs on the two processors
                     */
                    newMsg = MSGQ_alloc (mqtState->defaultMqaId, getMsg->size) ;
                }
                DBC_assert (newMsg != NULL) ;

                if (newMsg != NULL) {
                    /* Copy message from the getMsg into newMsg */
                    memcpy (newMsg, getMsg, getMsg->size) ;
                    /* Update the MQT ID in the message to this MQT's ID */
                    newMsg->mqtId = mqtState->mqtId ;

                    /* Determine if srcHandle was specified */
                    if (newMsg->srcId != MSGQ_INVALIDMSGQID) {
                        srcHandle = &(mqtState->remoteMsgqs [newMsg->srcId]) ;
                    }
                    else {
                        srcHandle = NULL ;
                    }

                    /* Queue up the message on the destination local MSGQ */
                    status = MSGQ_put (msgqHandle,
                                       newMsg,
                                       newMsg->msgId,
                                       srcHandle) ;
                    DBC_assert (status == MSGQ_SUCCESS) ;
                }
            }
        }
        else {
            /*  Control message processing */
            MQTDSPLINK_CtrlMsg * ctrlMsg ;
            MSGQ_Handle          msgq ;

            ctrlMsg = (MQTDSPLINK_CtrlMsg *) getMsg ;
            if (getMsg->msgId == MqtCmdLocate) {
                /*  Locate control message */
                msgqId = ctrlMsg->ctrlMsg.locateMsg.msgqId ;

                strcpy (strMsgqName, DSPLINK_DSPMSGQ_NAME) ;
                numToStr (strMsgqId, msgqId) ;
                strcat (strMsgqName, strMsgqId) ;

                status = MSGQ_locateLocal (strMsgqName, &msgq) ;

                if ((status == MSGQ_SUCCESS) || (status == MSGQ_ENOTFOUND)) {
                    ctrlMsg = (MQTDSPLINK_CtrlMsg *)
                                      MSGQ_alloc (mqtState->defaultMqaId,
                                                  MQTDSPLINK_CTRLMSG_SIZE) ;
                    if (ctrlMsg != NULL) {
                        ctrlMsg->msgHeader.msgId = MqtCmdLocateAck ;
                        ctrlMsg->msgHeader.mqtId = mqtState->mqtId ;
                        ctrlMsg->msgHeader.dstId = ID_MQTDSPLINK_CTRL ;
                        ctrlMsg->ctrlMsg.locateAckMsg.msgqId = msgqId ;
                        if (status == MSGQ_SUCCESS) {
                            ctrlMsg->ctrlMsg.locateAckMsg.msgqFound = 1 ;
                        }
                        else {
                            ctrlMsg->ctrlMsg.locateAckMsg.msgqFound = 0 ;
                        }
                        status = MQTDSPLINK_sendMessage (mqtState,
                                                         (MSGQ_Msg) ctrlMsg) ;
                        DBC_assert (status == MSGQ_SUCCESS) ;
                    }
                }
            }
            else if (getMsg->msgId == MqtCmdLocateAck) {
                /*  Locate acknowledgement control message */
                MSGQ_AsyncLocateMsg *  newMsg ;
                Uint16                 msgqId ;

                msgqId = ctrlMsg->ctrlMsg.locateAckMsg.msgqId ;

                msgq = &(mqtState->remoteMsgqs [msgqId]) ;

                if (ctrlMsg->ctrlMsg.locateAckMsg.msgqFound == 1) {
                    msgq->status = MSGQ_INUSE ;
                }
                else {
                    msgq->status = MSGQ_EMPTY ;
                }

                if (ctrlMsg->ctrlMsg.locateAckMsg.semHandle != NULL) {
                    /*  Synchronous location */
                    SEM_post ((SEM_Handle)
                               ctrlMsg->ctrlMsg.locateAckMsg.semHandle) ;
                }
                else {
                    /*  Asynchronous location */
                    MSGQ_Handle replyMsgq = (MSGQ_Handle)
                                     ctrlMsg->ctrlMsg.locateAckMsg.replyHandle ;
                    msgqId = replyMsgq->msgqId ;
                    if (    (ctrlMsg->ctrlMsg.locateAckMsg.msgqFound == 1)
                        &&  (replyMsgq->status == MSGQ_INUSE)) {
                        /* Allocate a message for the locate ack */
                        newMsg = (MSGQ_AsyncLocateMsg *) MSGQ_alloc (
                                            ctrlMsg->ctrlMsg.locateAckMsg.mqaId,
                                            sizeof (MSGQ_AsyncLocateMsg)) ;

                        DBC_assert (newMsg != NULL) ;
                        if (newMsg != NULL) {
                            newMsg->arg = (Arg)
                                          ctrlMsg->ctrlMsg.locateAckMsg.arg ;
                            newMsg->msgqHandle = msgq ;

                            status = MSGQ_put (replyMsgq,
                                               (MSGQ_Msg) newMsg,
                                               MSGQ_ASYNCLOCATEMSGID,
                                               NULL) ;
                            DBC_assert (status == MSGQ_SUCCESS) ;
                        }
                    }
                }
            }
            else if (getMsg->msgId == MqtCmdExit) {
                /*  MQT exit notification control message */
                MSGQ_sendErrorMsg ((Uint16) MSGQ_MQTERROREXIT,
                                   (Arg)    mqtState->mqtId,
                                   (Arg)    NULL) ;
            }
        }

        /* Reissue the getMsg on the input channel */
        msgSize = (Uns) mqtState->maxMsgSize ;
        iomStatus = GIO_submit (mqtState->inpChan,          /* Device handle  */
                                IOM_READ,                   /* Read command   */
                                getMsg,                     /* Buffer pointer */
                                &msgSize,                   /* Msg size       */
                                &(mqtState->getCallback)) ; /* callback info  */
        DBC_assert (iomStatus == IOM_PENDING) ;
    }
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_createRemoteMsgq
 *
 *  @desc   Create the remote MSGQ for specified MSGQ ID.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_createRemoteMsgq (MSGQ_TransportHandle mqtHandle,
                             Uint16               msgqId)
{
    MSGQ_Status        status   = MSGQ_SUCCESS ;
    MSGQ_Handle        msgq ;
    MQTDSPLINK_State * mqtState ;
    MQTDSPLINK_Handle  mqtRepository ;
    Char               strMsgqId [3] ;

    DBC_require (mqtHandle != NULL) ;
    DBC_require (msgqId    != MSGQ_INVALIDMSGQID) ;

    mqtState = (MQTDSPLINK_State *) mqtHandle->mqtInfo ;

    DBC_assert (msgqId < mqtState->numRemoteMsgq) ;

    msgq = &(mqtState->remoteMsgqs [msgqId]) ;
    /*  Initialize the non-NULL fields of the MSGQ object */
    msgq->notifyHandle    = NULL ;
    msgq->pend            = NULL ;
    msgq->post            = NULL ;
    msgq->mqtHandle       = mqtHandle ;
    msgq->mqtGet          = MQTDSPLINK_FXNS.mqtGet ;
    msgq->mqtPut          = MQTDSPLINK_FXNS.mqtPut ;
    msgq->msgqId          = msgqId ;
    msgq->status          = MSGQ_EMPTY ;

    msgq->msgqName = MEM_alloc (MQTDSPLINK_MEM_SEGID,
                                (MAX_MSGQ_NAME_LEN * sizeof (Char)),
                                MQTDSPLINK_MEM_ALIGN) ;
    if (msgq->msgqName != MEM_ILLEGAL) {
        strcpy (msgq->msgqName, DSPLINK_GPPMSGQ_NAME) ;
        numToStr (strMsgqId, msgqId) ;
        strcat (msgq->msgqName, strMsgqId) ;

        msgq->mqtRepository = MEM_alloc (MQTDSPLINK_MEM_SEGID,
                                         sizeof (MQTDSPLINK_Obj),
                                         MQTDSPLINK_MEM_ALIGN) ;
        if (msgq->mqtRepository != MEM_ILLEGAL) {
            /* Initialize the MQT instance object */
            mqtRepository = (MQTDSPLINK_Handle) (msgq->mqtRepository) ;
            SEM_new (&(mqtRepository->locateSem), 0) ;
        }
        else {
            status = MSGQ_EMEMORY ;
        }
    }
    else {
        status = MSGQ_EMEMORY ;
    }

    if (status != MSGQ_SUCCESS) {
        /* Cleanup all allocated resources on failure */
        MQTDSPLINK_deleteRemoteMsgq (mqtState, msgqId) ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_deleteRemoteMsgq
 *
 *  @desc   Delete the remote MSGQ for specified MSGQ ID.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_deleteRemoteMsgq (MQTDSPLINK_State * mqtState, Uint16 msgqId)
{
    MSGQ_Status status      = MSGQ_SUCCESS ;
    Bool        freeStatus  = TRUE ;
    MSGQ_Handle msgqHandle ;

    DBC_require (mqtState != NULL) ;
    DBC_require (msgqId   != MSGQ_INVALIDMSGQID) ;
    DBC_require (msgqId < mqtState->numRemoteMsgq) ;

    msgqHandle = &(mqtState->remoteMsgqs [msgqId]) ;
    msgqHandle->status = MSGQ_EMPTY ;

    if (msgqHandle->msgqName != NULL) {
        freeStatus = MEM_free (MQTDSPLINK_MEM_SEGID,
                               msgqHandle->msgqName,
                               (MAX_MSGQ_NAME_LEN * sizeof (Char))) ;
        msgqHandle->msgqName = NULL ;
    }

    if (msgqHandle->mqtRepository != NULL) {
        freeStatus =    MEM_free (MQTDSPLINK_MEM_SEGID,
                                  msgqHandle->mqtRepository,
                                  sizeof (MQTDSPLINK_Obj))
                     && freeStatus ;
        msgqHandle->mqtRepository = NULL ;
    }

    if (freeStatus == FALSE) {
        status = MSGQ_EMEMORY ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQTDSPLINK_sendMessage
 *
 *  @desc   Sends a message to the remote MQT.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQTDSPLINK_sendMessage (MQTDSPLINK_State * mqtState, MSGQ_Msg msg)
{
    MSGQ_Status        status     = MSGQ_SUCCESS ;
    Bool               queEmpty   = FALSE ;
    Int                iomStatus ;
    Uns                key ;
    Uns                msgSize ;

    DBC_require (mqtState != NULL) ;
    DBC_require (msg      != NULL) ;

    key = ISR_DISABLE ;
    if (QUE_empty (&(mqtState->msgQueue))) {
        queEmpty = TRUE ;
    }

    /* Add the message to the end of the message queue */
    QUE_put (&(mqtState->msgQueue), msg) ;
    ISR_RESTORE (key) ;

    if (queEmpty == TRUE) {
        msgSize = (Uns) msg->size ;
        iomStatus = GIO_submit (mqtState->outChan,          /* Device handle  */
                                IOM_WRITE,                  /* Write command  */
                                msg,                        /* Buffer pointer */
                                &msgSize,                   /* Buffer size    */
                                &(mqtState->putCallback)) ; /* callback info  */

        if (iomStatus != IOM_PENDING) {
            status = MSGQ_EFAILURE ;
        }
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   numToStr
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
numToStr (String strNum, Uns num)
{
    Char tens ;

    DBC_require (strNum != NULL) ;

    tens = (Char) num / 10 ;
    num  = num % 10 ;

    strNum [0] = tens + '0' ;
    strNum [1] = (Char) num + '0' ;
    strNum [2] = '\0' ;
}


#ifdef __cplusplus
}
#endif /* extern "C" */
