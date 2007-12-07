/** ============================================================================
 *  @file   shm_driver.c
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   LINK Main Module implementation. This module implements the
 *          minidriver interface as required by the IOM model of BIOS.
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
#include <iom.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <shm_driver.h>
#include <shm.h>
#include <failure.h>
#include <dbc.h>
#include <dsplink.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   File Id of this file.
 *  ============================================================================
 */
#define FILEID  FID_DRIVER_C

/** ============================================================================
 *  @const  NUM_MSGCHNL
 *
 *  @desc   Number of messaging channels
 *  ============================================================================
 */
#if defined(_MSGQ_COMPONENT)
#define NUM_MSGCHNL 2
#else /* if defined(_MSGQ_COMPONENT) */
#define NUM_MSGCHNL 0
#endif /* #if defined(_MSGQ_COMPONENT) */

#if defined(_MSGQ_COMPONENT)
/** ============================================================================
 *  @macro  IS_VALID_MSGCHNL
 *
 *  @desc   Checks whether the channel ID is a valid messaging channel ID.
 *  ============================================================================
 */
#define IS_VALID_MSGCHNL(chanId) (   (chanId == ID_MSGCHNL_FM_GPP) \
                                  || (chanId == ID_MSGCHNL_TO_GPP))
#endif /* if defined(_MSGQ_COMPONENT) */

/** ============================================================================
 *  @macro  RESET_DSPFREEMASKBIT
 *
 *  @desc   Resets the dspFreeMask bit.
 *  ============================================================================
 */
#define RESET_DSPFREEMASKBIT(chanId) SHM_writeCtlParam(              \
                                SHM_dspFreeMask,                     \
                                (  SHM_readCtlParam(SHM_dspFreeMask) \
                                 & (~(1<<chanId))))

/** ============================================================================
 *  @macro  SET_DSPFREEMASKBIT
 *
 *  @desc   Sets the dspFreeMask bit.
 *  ============================================================================
 */
#define SET_DSPFREEMASKBIT(chanId) SHM_writeCtlParam(                \
                              SHM_dspFreeMask,                       \
                              (  SHM_readCtlParam(SHM_dspFreeMask)   \
                               | (1<<chanId)))

/** ============================================================================
 *  @macro  GET_DSPFREEMASKBIT
 *
 *  @desc   Gets the value of a specific bit of dspFreeMask.
 *  ============================================================================
 */
#define GET_DSPFREEMASKBIT(chanId)                                    \
                             (  (SHM_readCtlParam(SHM_dspFreeMask)    \
                              & (1<<chanId)))

/** ============================================================================
 *  @macro  SET_DSPDATAMASKBIT
 *
 *  @desc   Sets the value of a specific bit of dspDataMask.
 *  ============================================================================
 */
#define SET_DSPDATAMASKBIT(dev,chanId) (dev->dspDataMask =                     \
                                            (dev->dspDataMask | (1<<chanId)))

/** ============================================================================
 *  @macro  RESET_DSPDATAMASKBIT
 *
 *  @desc   Resets the value of a specific bit of dspDataMask.
 *  ============================================================================
 */
#define RESET_DSPDATAMASKBIT(dev,chanId) (dev->dspDataMask =                   \
                                            (dev->dspDataMask & (~(1<<chanId))))

/** ============================================================================
 *  @macro  RESET_DSPFREEBUFFER
 *
 *  @desc   Sets the fields to indicate that DSP does not have any free buffers
 *          available on this channel.
 *  ============================================================================
 */
#if defined(_MSGQ_COMPONENT)
#define RESET_DSPFREEBUFFER(chanId)  (IS_VALID_MSGCHNL(chanId) ?               \
                                       SHM_writeCtlParam(SHM_dspFreeMsg, 0)  : \
                                       RESET_DSPFREEMASKBIT(chanId))
#else /* if defined(_MSGQ_COMPONENT) */
#define RESET_DSPFREEBUFFER(chanId)  RESET_DSPFREEMASKBIT(chanId)
#endif /* if defined(_MSGQ_COMPONENT) */

/** ============================================================================
 *  @macro  SET_DSPFREEBUFFER
 *
 *  @desc   Sets the fields to indicate that DSP has at least one free buffer
 *          available on this channel.
 *  ============================================================================
 */
#if defined(_MSGQ_COMPONENT)
#define SET_DSPFREEBUFFER(chanId)  (IS_VALID_MSGCHNL(chanId) ?                 \
                                       SHM_writeCtlParam(SHM_dspFreeMsg, 1)  : \
                                       SET_DSPFREEMASKBIT(chanId))
#else /* if defined(_MSGQ_COMPONENT) */
#define SET_DSPFREEBUFFER(chanId)  SET_DSPFREEMASKBIT(chanId)
#endif /* if defined(_MSGQ_COMPONENT) */

/** ============================================================================
 *  @macro  GET_DSPFREEBUFFER
 *
 *  @desc   Indicates whether DSP has at least one free buffer available on this
 *          channel.
 *  ============================================================================
 */
#if defined(_MSGQ_COMPONENT)
#define GET_DSPFREEBUFFER(chanId) (IS_VALID_MSGCHNL(chanId) ?                \
                                        SHM_readCtlParam(SHM_dspFreeMsg)  :  \
                                        GET_DSPFREEMASKBIT(chanId))
#else /* if defined(_MSGQ_COMPONENT) */
#define GET_DSPFREEBUFFER(chanId)  GET_DSPFREEMASKBIT(chanId)
#endif /* if defined(_MSGQ_COMPONENT) */

/** ============================================================================
 *  @macro  RESET_DSPOUTPUTBUFFER
 *
 *  @desc   Sets the fields to indicate that DSP does not have any buffer to
 *          output on this channel.
 *  ============================================================================
 */
#if defined(_MSGQ_COMPONENT)
#define RESET_DSPOUTPUTBUFFER(dev, chanId) (IS_VALID_MSGCHNL(chanId) ?         \
                                             dev->outputMsg = 0  :             \
                                             RESET_DSPDATAMASKBIT(dev, chanId))
#else /* if defined(_MSGQ_COMPONENT) */
#define RESET_DSPOUTPUTBUFFER(dev, chanId)  RESET_DSPDATAMASKBIT(dev, chanId)
#endif /* if defined(_MSGQ_COMPONENT) */

/** ============================================================================
 *  @macro  SET_DSPOUTPUTBUFFER
 *
 *  @desc   Sets the fields to indicate that DSP has at least one buffer to
 *          output on this channel.
 *  ============================================================================
 */
#if defined(_MSGQ_COMPONENT)
#define SET_DSPOUTPUTBUFFER(dev, chanId) (IS_VALID_MSGCHNL(chanId) ?           \
                                              dev->outputMsg = 1  :            \
                                              SET_DSPDATAMASKBIT(dev, chanId))
#else /* if defined(_MSGQ_COMPONENT) */
#define SET_DSPOUTPUTBUFFER(dev, chanId)  SET_DSPDATAMASKBIT(dev, chanId)
#endif /* if defined(_MSGQ_COMPONENT) */


/** ============================================================================
 *  @name   SHMLINK_DevObject_tag
 *
 *  @desc   Forward declaration of SHMLINK_DevObject_tag structure.
 *  ============================================================================
 */
struct SHMLINK_DevObject_tag;

/** ============================================================================
 *  @name   SHMLINK_ChannelObject
 *
 *  @desc   Channel object of LINK device.
 *
 *  @field  inUse
 *              Non zero value means this channel is in use.
 *  @field  chanId
 *              Channel identifier.
 *  @field  mode
 *              Mode of channel. Mode can be input or output.
 *  @field  dev
 *              Reference to LINK device structure.
 *  @field  pendingIOQue
 *              Queue for pending IO packets.
 *  @field  flushPacket
 *              In case flush is pending on this channel. This field contains
 *              the flush packet.
 *  @field  cbFxn
 *              IOM callback function.
 *  @field  cbArg
 *              Argument to callback function.
 *  @field  maxBufferSize
 *              Maximum size of buffer that this channel supports.
 *              Defined for debug build only.
 *  @field  maxPendingIOs
 *              Maximum pending IOs that this channel can have.
 *              Defined for debug build only.
 *  @field  currentPendingIOs
 *              Number of pending IOs on this channel.
 *              Defined for debug build only.
 *  ============================================================================
 */
typedef struct SHMLINK_ChannelObject_tag {
    Uns                             inUse;
    Uns                             chanId;
    Uns                             mode;
    struct SHMLINK_DevObject_tag *  dev;
    QUE_Obj                         pendingIOQue;
    IOM_Packet *                    flushPacket;
    IOM_TiomCallback                cbFxn;
    Ptr                             cbArg;
#if defined(_DEBUG)
    Uns                             maxBufferSize;
    Int                             maxPendingIOs;
    Uns                             currentPendingIOs;
#endif /* if defined(_DEBUG) */
} SHMLINK_ChannelObject;

/** ============================================================================
 *  @name   SHMLINK_DevObject
 *
 *  @desc   LINK device structure.
 *
 *  @field  inUse
 *              Non zero value means this LINK device is in use.
 *  @field  numChannels
 *              Maximum channels supported by this device.
 *  @field  dspDataMask
 *              Tells on which channels output buffer available.
 *  @field  lastOutputChannel
 *              Variable indicating on which channel last output was done.
 *  @field  chanObj
 *              Arrary of channel objects that belong to this device.
 *  @field  outputMsg
 *              Indicates whether a message is available on the DSP to be sent
 *              to the GPP.
 *  ============================================================================
 */
typedef struct SHMLINK_DevObject_tag {
    Uns                     inUse;
    Uns                     numChannels;
    Uns                     dspDataMask;
    Uns                     lastOutputChannel;
    SHMLINK_ChannelObject   chanObj [NUM_SHM_CHANNELS + NUM_MSGCHNL];
#if defined (_MSGQ_COMPONENT)
    Uns                     outputMsg;
#endif /* defined (_MSGQ_COMPONENT) */
} SHMLINK_DevObject;


/** ============================================================================
 *  @func   atoi
 *
 *  @desc   Converts character string to integer value.
 *
 *  @arg    str
 *              Pointer to character string.
 *
 *  @ret    <integer value>
 *              Converted integer value.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
extern int atoi(const char *str);


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdBindDev
 *
 *  @desc   Allocates resources needed for intialization of this device.
 *
 *  @arg    devp
 *              Output device structure.
 *          devid
 *              Device number. Not being used for LINK.
 *          devParams
 *              Device parameters.
 *
 *  @ret    IOM_COMPLETED
 *              Successful initialization.
 *          IOM_EINUSE
 *              Device already in use.
 *          IOM_EBADIO
 *              General failure during initialization.
 *          IOM_EBADARGS
 *              Invalid argument passed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdBindDev(Ptr *devp, Int devid, Ptr devParams);


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdCreateChan
 *
 *  @desc   Creates a new channel on given device.
 *
 *  @arg    chanp
 *              Channel pointer where to put newly created channel.
 *          devp
 *              Device on which to create channel.
 *          name
 *              Channel number as character string.
 *          mode
 *              Mode of channel.
 *          chanParams
 *              Channel parameters.
 *          cbFxn
 *              IOM callback function.
 *          cbArg
 *              Argument to IOM callback function.
 *
 *  @ret    IOM_COMPLETED
 *              Successful completion of function.
 *          IOM_EINUSE
 *              Device already in use.
 *          IOM_EBADIO
 *              General failure during operation.
 *          IOM_EBADARGS
 *              Invalid argument passed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdCreateChan(Ptr *chanp, Ptr devp, String name,
                                Int mode, Ptr chanParams,
                                IOM_TiomCallback cbFxn, Ptr cbArg);


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdDeleteChan
 *
 *  @desc   Deletes specified channel.
 *
 *  @arg    chanp
 *              Channel to be deleted.
 *
 *  @ret    IOM_COMPLETED
 *              Successful completion of function.
 *          IOM_EINUSE
 *              Device already in use.
 *          IOM_EBADIO
 *              General failure during operation.
 *          IOM_EBADARGS
 *              Invalid argument passed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdDeleteChan(Ptr chanp);


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdSubmitChan
 *
 *  @desc   Submits a command or IO request on a channel.
 *
 *  @arg    chanp
 *              Channel on which to perform action.
 *          packet
 *              IOM Request packet.
 *
 *  @ret    IOM_COMPLETED
 *              Successful completion of function.
 *          IOM_EINUSE
 *              Device already in use.
 *          IOM_EBADIO
 *              General failure during operation.
 *          IOM_EBADARGS
 *              Invalid argument passed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdSubmitChan(Ptr chanp, IOM_Packet *packet);


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_ISR
 *
 *  @desc   ISR for interrupt from GPP.
 *
 *  @arg    dev
 *              Device structure.
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
static Void SHMLINK_ISR(Ptr dev);


/** ----------------------------------------------------------------------------
 *  @func   abortio
 *
 *  @desc   Aborts all the pending IOs on this channel.
 *
 *  @arg    chan
 *              Channel object pointer.
 *  @arg    status
 *              Status with which the packet is to be aborted.
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
static Void abortio(SHMLINK_ChannelObject * chan, int retStatus);


/** ----------------------------------------------------------------------------
 *  @func   flushOutput
 *
 *  @desc   Flushes all the pending IOs on this channel. Note that this function
 *          is only called for output channels.
 *
 *  @arg    chan
 *              Channel object pointer.
 *          flushPacket
 *              Flush packet.
 *
 *  @ret    IOM_COMPLETED
 *              Flush request completed successfully.
 *          IOM_PENDING
 *              Flush request is pending for completion at later point of time.
 *
 *  @enter  flushPacket in the argument is not NULL.
 *          chan pointer is not NULL.
 *          There is no flush packet already present for this channel.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Int flushOutput(SHMLINK_ChannelObject * chan, IOM_Packet *flushPacket);


/** ----------------------------------------------------------------------------
 *  @func   doInputProcessing
 *
 *  @desc   Perform input for LINK if required.
 *
 *  @arg    dev
 *              Device for which input is to be performed.
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
static Void doInputProcessing(SHMLINK_DevObject *dev);


/** ----------------------------------------------------------------------------
 *  @func   doOutputProcessing
 *
 *  @desc   Performs output for LINK if required.
 *
 *  @arg    dev
 *              Device for which output is to be performed.
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
static Void doOutputProcessing(SHMLINK_DevObject *dev);


/** ----------------------------------------------------------------------------
 *  @func   selectOutputChannel
 *
 *  @desc   Selects the channel which is ready to output and on which GPP side
 *          is also ready to receive.
 *
 *  @arg    dev
 *              Device for which output is to be performed.
 *
 *  @ret    >= (dev->numChannels + NUM_MSGCHNL)
 *              No channel is ready for output.
 *          else
 *              ID of the selected channel that is ready for output.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
static Uns selectOutputChannel(SHMLINK_DevObject *dev);


/** ============================================================================
 *  @name   DSPLINK_FXNS
 *
 *  @desc   DSPLINK function pointer table. Unbind device and control channel
 *          functions are not implemented.
 *  ============================================================================
 */
IOM_Fxns DSPLINK_FXNS = {
    SHMLINK_mdBindDev,
    IOM_UNBINDDEVNOTIMPL,
    IOM_CONTROLCHANNOTIMPL,
    SHMLINK_mdCreateChan,
    SHMLINK_mdDeleteChan,
    SHMLINK_mdSubmitChan
};

/** ============================================================================
 *  @name   DSPLINK_DEV_PARAMS
 *
 *  @desc   Default device parameters.
 *  ============================================================================
 */
DSPLINK_DevParams DSPLINK_DEV_PARAMS = {
    0,
    MAX_CHANNELS
};

/** ============================================================================
 *  @name   defaultChanParams
 *
 *  @desc   Default channel parameters.
 *  ============================================================================
 */
const DSPLINK_ChnlParams defaultChanParams = {
    -1, /* Maximum SHM size */
    -1  /* Infinite buffer queueing */
};

/** ============================================================================
 *  @name   devObj
 *
 *  @desc   LINK device structure.
 *  ============================================================================
 */
static SHMLINK_DevObject devObj;


/*  ============================================================================
 *  Create named sections for the functions to allow specific memory placement.
 *  ============================================================================
 */
#pragma CODE_SECTION (DSPLINK_init,         ".text:init")
#pragma CODE_SECTION (SHMLINK_mdBindDev,    ".text:DSPLINK_init")
#pragma CODE_SECTION (SHMLINK_mdCreateChan, ".text:DSPLINK_create")
#pragma CODE_SECTION (SHMLINK_mdDeleteChan, ".text:DSPLINK_delete")


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdBindDev
 *
 *  @desc   Allocates resources needed for working of the device. This function
 *          returns device structure in output parameter devp.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdBindDev(Ptr *devp, Int devid, Ptr devParams)
{
    Int                status       = IOM_COMPLETED;
    DSPLINK_DevParams *linkDevParam = (DSPLINK_DevParams *) devParams;

    DBC_require(devp != NULL);

    if (linkDevParam == NULL) {
        linkDevParam = &DSPLINK_DEV_PARAMS;
    }

    if (devObj.inUse == 1) {
        /* Dev object already in use... */
        status = IOM_EINUSE;
        SET_FAILURE_REASON(status);
    }
    else if (linkDevParam->numChannels > NUM_SHM_CHANNELS) {
        status = IOM_EBADARGS;
        SET_FAILURE_REASON(status);
    }
    else {
        devObj.inUse = 1;
        devObj.numChannels = linkDevParam->numChannels;
        devObj.lastOutputChannel = 0;
        *devp = &devObj;

        SHM_init();
        SHM_registerGPPISR(SHMLINK_ISR, &devObj);
        SHM_handshake ();
    }

    DBC_ensure((status != IOM_COMPLETED) || (*devp != NULL));

    return status;
}


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdCreateChan
 *
 *  @desc   Creates a new channel on given device.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdCreateChan(Ptr *               chanp,
                                Ptr                 devp,
                                String              name,
                                Int                 mode,
                                Ptr                 chanParams,
                                IOM_TiomCallback    cbFxn,
                                Ptr                 cbArg)
{
    Int                 status   = IOM_COMPLETED;
    DSPLINK_ChnlParams *chanAttr = chanParams;
    SHMLINK_DevObject * dev      = devp;
    Uns                 chanId;

    DBC_require(chanp != NULL);
    DBC_require(dev != NULL);

    if (chanAttr == NULL) {
        chanAttr = (DSPLINK_ChnlParams *) &defaultChanParams;
    }

    if (chanAttr->maxBufferSize == -1) {
        chanAttr->maxBufferSize = SHM_getMaxBufferSize();
    }

    *chanp = NULL;
    chanId = atoi(name);

    if (   (    (chanId >= dev->numChannels)
#if defined(_MSGQ_COMPONENT)
            &&  !(IS_VALID_MSGCHNL(chanId))
#endif /* if defined(_MSGQ_COMPONENT) */
           )
        || (chanAttr->maxBufferSize >  SHM_getMaxBufferSize())) {
        status = IOM_EBADARGS;
        SET_FAILURE_REASON(status);
    } else if (dev->chanObj[chanId].inUse) {
        status = IOM_EINUSE;
        SET_FAILURE_REASON(status);
    } else if (mode == IOM_INOUT) {
        status = IOM_EBADMODE;
        SET_FAILURE_REASON(status);
    }
    else {
        dev->chanObj[chanId].inUse = 1;
        dev->chanObj[chanId].mode = mode;
#if defined(_DEBUG)
        dev->chanObj[chanId].maxPendingIOs = chanAttr->maxPendingIOs;
        dev->chanObj[chanId].currentPendingIOs = 0;
        dev->chanObj[chanId].maxBufferSize = chanAttr->maxBufferSize;
#endif /* if defined(_DEBUG) */
        dev->chanObj[chanId].cbFxn = cbFxn;
        dev->chanObj[chanId].cbArg = cbArg;
        dev->chanObj[chanId].dev = (Ptr)dev;
        dev->chanObj[chanId].chanId = chanId;
        dev->chanObj[chanId].flushPacket = NULL;

        QUE_new(&(dev->chanObj[chanId].pendingIOQue));
        *chanp = &(dev->chanObj[chanId]);
    }

    DBC_ensure((status != IOM_COMPLETED) || (*chanp != NULL));

    return status;
}


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdDeleteChan
 *
 *  @desc   Deletes a channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdDeleteChan(Ptr chanp)
{
    Int                    status = IOM_COMPLETED;
    SHMLINK_ChannelObject *chan   = chanp;

    DBC_require(chanp != NULL);

    chan->inUse = 0;

    DBC_ensure((status != IOM_COMPLETED) || (chan->inUse == 0));

    return status;
}


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_mdSubmitChan
 *
 *  @desc   Submit a command or IO request on a channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int SHMLINK_mdSubmitChan(Ptr chanp, IOM_Packet *packet)
{
    Int                    status = IOM_PENDING;
    SHMLINK_ChannelObject *chan   = chanp;
    Uns                    key;

    DBC_require(packet != NULL);
    DBC_require(chanp != NULL);

    switch (packet->cmd) {
        case IOM_FLUSH:
            DBC_assert (chan->flushPacket == NULL) ; /* One flush request is
                                                      * already pending.
                                                      */
            if (chan->mode == IOM_OUTPUT) {
                status = flushOutput(chan, packet);
            }
            else {
                /* In case of input channel flush is same as abort */
                abortio(chan, IOM_FLUSHED);
                packet->status = IOM_COMPLETED;
                status = IOM_COMPLETED;
            }
            break;

        case IOM_ABORT:
            RESET_DSPOUTPUTBUFFER(chan->dev, chan->chanId);
            abortio(chan, IOM_ABORTED);
            packet->status = IOM_COMPLETED;
            status = IOM_COMPLETED;
            break;

        case IOM_READ: /* Fall through */
        case IOM_WRITE:
            DBC_assert (   (chan->maxPendingIOs == -1)       \
                        || (chan->currentPendingIOs < chan->maxPendingIOs)) ;
            DBC_assert (packet != NULL) ;
            DBC_assert (packet->size <= chan->maxBufferSize) ;
            DBC_assert (    (  (packet->cmd == IOM_READ)     \
                            && (chan->mode  == IOM_INPUT))   \
                        ||  (  (packet->cmd == IOM_WRITE)    \
                            && (chan->mode  == IOM_OUTPUT))) ;
            DBC_assert (    (   (packet->cmd       == IOM_WRITE) \
                             && (chan->flushPacket == NULL))     \
                        ||  (packet->cmd == IOM_READ)) ;  /* Cannot accept write
                                                           * request when flush
                                                           * request is pending
                                                           * on this channel
                                                           */

            key = SHM_disableGPPInt();
            QUE_enqueue(&chan->pendingIOQue, packet);
#if defined(_DEBUG)
            chan->currentPendingIOs++;
#endif /* if defined(_DEBUG) */
            if (packet->cmd == IOM_READ) {
                if (!GET_DSPFREEBUFFER(chan->chanId)) {
                    SET_DSPFREEBUFFER(chan->chanId);
                    SHM_sendInt(NULL);
                }
            }
            else {
                SET_DSPOUTPUTBUFFER(chan->dev, chan->chanId);
                doOutputProcessing(chan->dev);
            }

            SHM_enableGPPInt(key);
            break;

        default:
            status = IOM_EBADIO;
            SET_FAILURE_REASON(status);
            break;
    }
    return status;
}


/** ----------------------------------------------------------------------------
 *  @func   SHMLINK_ISR
 *
 *  @desc   ISR for interrupt from GPP.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void SHMLINK_ISR(Ptr devp)
{
    SHMLINK_DevObject *dev = devp;
    Uns                key;

    DBC_require(dev != NULL);

    key = SHM_disableGPPInt();
    doInputProcessing(dev);
    doOutputProcessing(dev);
    SHM_enableGPPInt(key);
}


/** ============================================================================
 *  @func   DSPLINK_init
 *
 *  @desc   Initializes DSPLINK data structures before bind function.
 *
 *  @modif  None
 *  ============================================================================
 */
Void DSPLINK_init()
{
    Uns i;
    devObj.inUse = 0;
    devObj.numChannels = 0;
    devObj.dspDataMask = 0;
#if defined (_MSGQ_COMPONENT)
    devObj.outputMsg   = 0;
#endif /* defined (_MSGQ_COMPONENT) */
    devObj.lastOutputChannel = 0;
    for(i=0; i < NUM_SHM_CHANNELS + NUM_MSGCHNL; i++) {
        devObj.chanObj [i].inUse = 0;
    }
}


/** ----------------------------------------------------------------------------
 *  @func   abortio
 *
 *  @desc   Aborts all the pending IOs on this channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void abortio(SHMLINK_ChannelObject * chan, int retStatus)
{
    IOM_Packet *tmpPacket;
    Uns         key;

    key = SHM_disableGPPInt();
    if (GET_DSPFREEBUFFER(chan->chanId)) {
        RESET_DSPFREEBUFFER(chan->chanId);
        SHM_sendInt(NULL);
    }

    while ((QUE_Handle) (tmpPacket = QUE_get(&chan->pendingIOQue)) !=
                                                          &chan->pendingIOQue) {
#if defined(_DEBUG)
        chan->currentPendingIOs--;
#endif /* defined(_DEBUG) */
        tmpPacket->status = retStatus ;
        (*chan->cbFxn)(chan->cbArg, tmpPacket);
    }

    SHM_enableGPPInt(key);
}


/** ----------------------------------------------------------------------------
 *  @func   flushOutput
 *
 *  @desc   Flushes all the pending IOs on this channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Int flushOutput(SHMLINK_ChannelObject * chan, IOM_Packet *flushPacket)
{
    Int status = IOM_PENDING;
    Uns key;

    DBC_require(flushPacket != NULL);
    DBC_require(chan != NULL);
    DBC_require((chan != NULL) && (chan->flushPacket == NULL));

    key = SHM_disableGPPInt();

    if (QUE_empty(&chan->pendingIOQue)) {
        flushPacket->status = IOM_COMPLETED;
        status = IOM_COMPLETED;
    }
    else {
        chan->flushPacket = flushPacket;
    }

    SHM_enableGPPInt(key);

    return status;
}


/** ----------------------------------------------------------------------------
 *  @func   doInputProcessing
 *
 *  @desc   Perform input if required.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void doInputProcessing(SHMLINK_DevObject *dev)
{
    SHMLINK_ChannelObject *chan;
    IOM_Packet             *packet;
    Uns                    chanId;
    Uns                    shmBufLen;
    Uns                    packetLen;

    if (SHM_readCtlParam(SHM_inputFull)) {
        chanId = SHM_readCtlParam(SHM_inputId);

        shmBufLen = SHM_readCtlParam(SHM_inputSize);
        chan = &(dev->chanObj [chanId]);
        if ((QUE_Handle) (packet = QUE_get(&chan->pendingIOQue)) !=
                                                          &chan->pendingIOQue) {
#if defined(_DEBUG)
            chan->currentPendingIOs--;
#endif /* if defined(_DEBUG) */

            if (packet->size < shmBufLen) {
                /* We don't have sufficient buffer, data is truncated */
                packetLen = packet->size;
            }
            else {
                packetLen = packet->size = shmBufLen;
            }

            SHM_readInputBuffer(packet->addr, packetLen);
            if (QUE_empty(&chan->pendingIOQue)) {
                RESET_DSPFREEBUFFER(chanId);
            }
            SHM_writeCtlParam(SHM_inputFull, 0);
            SHM_sendInt(NULL);
            packet->status = IOM_COMPLETED;
            (*chan->cbFxn)(chan->cbArg, packet);
        }
        else { /* We have data available for a channel but channel
                  does not have any buffer; We have to drop the data */
            SHM_writeCtlParam(SHM_inputFull, 0);
        }
    }
}


/** ----------------------------------------------------------------------------
 *  @func   doOutputProcessing
 *
 *  @desc   Perform output if required.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static Void doOutputProcessing(SHMLINK_DevObject *dev)
{
    SHMLINK_ChannelObject *chan;
    IOM_Packet            *packet;
    Uns                    chanId;

    if (!SHM_readCtlParam (SHM_outputFull)) {
        chanId = selectOutputChannel(dev);
        if (chanId < ((Uns) dev->numChannels + NUM_MSGCHNL)) {
            chan = &(dev->chanObj[chanId]);
            packet = QUE_get(&chan->pendingIOQue);
            DBC_assert ((QUE_Handle) packet != &chan->pendingIOQue) ;

#if defined(_DEBUG)
            chan->currentPendingIOs--;
#endif /* if defined(_DEBUG) */
            SHM_writeOutputBuffer(packet->addr, packet->size);
            SHM_writeCtlParam(SHM_outputSize, packet->size);
            SHM_writeCtlParam(SHM_outputId, chanId);
            SHM_writeCtlParam(SHM_outputFull, 1);
            SHM_sendInt(NULL);

            packet->status = IOM_COMPLETED;
            (*chan->cbFxn)(chan->cbArg, packet);

            if (QUE_empty(&chan->pendingIOQue)) {
                RESET_DSPOUTPUTBUFFER(chan->dev, chan->chanId);
                if (chan->flushPacket != NULL) {
                    chan->flushPacket->status = IOM_COMPLETED;
                    (*chan->cbFxn)(chan->cbArg, chan->flushPacket);
                    chan->flushPacket = NULL;
                }
            }

        }
    }
}


/** ----------------------------------------------------------------------------
 *  @func   selectOutputChannel
 *
 *  @desc   Selects the channel which is ready to output.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
Uns selectOutputChannel(SHMLINK_DevObject *dev)
{
    Uns numChannels = (Uns) dev->numChannels ;
    Uns selectId    = numChannels + NUM_MSGCHNL ;
    Int     startId ;
    Int readyForOutputChanMask ;

#if defined(_MSGQ_COMPONENT)
    /* Check if the messaging channel is ready for output */
    if (dev->outputMsg & SHM_readCtlParam (SHM_gppFreeMsg)) {
        selectId = dev->lastOutputChannel = ID_MSGCHNL_TO_GPP ;
    }
#endif /* if defined(_MSGQ_COMPONENT) */

    if (selectId == (numChannels + NUM_MSGCHNL)) {
        readyForOutputChanMask =   dev->dspDataMask
                                 & SHM_readCtlParam (SHM_gppFreeMask) ;
        if (readyForOutputChanMask != 0) {
            startId = dev->lastOutputChannel ;

            do
             {
                startId++ ;
                startId = ((startId == numChannels) ? 0 : startId) ;
                if (readyForOutputChanMask & (1 << startId)) {
                    selectId = dev->lastOutputChannel = startId ;
                    break ;
                }
            } while (startId != dev->lastOutputChannel) ;
        }
    }

    return selectId ;
}


#ifdef __cplusplus
}
#endif /* extern "C" */
