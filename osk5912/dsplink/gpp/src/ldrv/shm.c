/** ============================================================================
 *  @file   shm.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implements the Shared Memory Link Driver interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <intobject.h>
#include <bitops.h>
#include <safe.h>

#include <cfgdefs.h>
#include <dspdefs.h>
#include <linkdefs.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ----------------------------------- OSAL Headers                */
#include <mem_os.h>
#include <dpc.h>
#include <mem.h>
#include <cfg.h>
#include <isr.h>
#include <sync.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Link Driver                 */
#if defined (MSGQ_COMPONENT)
#include <ldrv_mqt.h>
#include <ldrv_mqa.h>
#include <ldrv_msgq.h>
#endif /* if defined (MSGQ_COMPONENT) */

#include <ldrv.h>
#include <ldrv_chnl.h>
#include <ldrv_io.h>

#include <shm.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_SHM

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_SHM, __LINE__)

/*  ============================================================================
 *  @const  OMAP_MAILBOX_INT1
 *
 *  @desc   Interrupt number corresponding to MAILBOX1 interrupt on OMAP.
 *          (Also defined in dsp.c)
 *  ============================================================================
 */
#define OMAP_MAILBOX_INT1   10

/** ============================================================================
 *  @const  OMAP_MAILBOX_SIGNATURE
 *
 *  @desc   Value written the MAILBOX interrupt1 register.
 *          (HEX representation of characters: LN)
 *  ============================================================================
 */
#define OMAP_MAILBOX_SIGNATURE      0x4C4E

#define SHM_IS_INPUTFULL(ctrl)      ((((ctrl)->inputFull) == 1) ? TRUE : FALSE)
#define SHM_IS_OUTPUTFULL(ctrl)     ((((ctrl)->outputFull) == 1) ? TRUE : FALSE)

#define SHM_GET_INPUTID(ctrl)       ((ChannelId) ((ctrl)->inputId))
#define SHM_GET_OUTPUTID(ctrl)      ((ChannelId) ((ctrl)->outputId))

#define SHM_GET_INPUTSIZE(ctrl)     ((ctrl)->inputSize)

/** ============================================================================
 *  @const  SHM_IS_GPPBUFFERFREE
 *
 *  @desc   This macro is used for checking whether data is available on any one
 *          of the channels of the link.
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define SHM_IS_GPPBUFFERFREE(ctrl, chnlId)                                     \
                                (IS_VALID_MSGCHNLID(chnlId) ?                  \
                                 ((ctrl)->gppFreeMsg == 1)           :         \
                                 (TEST_BIT ((ctrl)->gppFreeMask, chnlId)))
#else /* if defined (MSGQ_COMPONENT) */
#define SHM_IS_GPPBUFFERFREE(ctrl, chnlId)                                     \
                                (TEST_BIT ((ctrl)->gppFreeMask, chnlId))
#endif /* defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @const  SET_GPPFREEMASKBIT
 *
 *  @desc   This macro is used for setting the gppFreeMsg or gppFreeMask
 *          depending upon the channel id.
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define SET_GPPFREEMASKBIT(ctrl, chnlId)                                       \
                                (IS_VALID_MSGCHNLID(chnlId) ?                  \
                                 ((ctrl)->gppFreeMsg  = (Uint16) 0x01)  :      \
                                 (SET_BIT((ctrl)->gppFreeMask, chnlId)))
#else /* if defined (MSGQ_COMPONENT) */
#define SET_GPPFREEMASKBIT(ctrl, chnlId)  (SET_BIT((ctrl)->gppFreeMask, chnlId))
#endif /* defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @const  CLEAR_GPPFREEMASKBIT
 *
 *  @desc   This macro is used for clearing the gppFreeMsg or gppFreeMask
 *          depending upon the channel id.
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define CLEAR_GPPFREEMASKBIT(ctrl, chnlId)                                     \
                                (IS_VALID_MSGCHNLID(chnlId) ?                  \
                                 ((ctrl)->gppFreeMsg  = (Uint16) 0x0)  :       \
                                 (CLEAR_BIT((ctrl)->gppFreeMask, chnlId)))
#else /* if defined (MSGQ_COMPONENT) */
#define CLEAR_GPPFREEMASKBIT(ctrl, chnlId)                                     \
                                 (CLEAR_BIT((ctrl)->gppFreeMask, chnlId))
#endif /* defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @const  SET_GPPINFO
 *
 *  @desc   This macro is used for setting the gpp control information
 *          depending upon the channel id.
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define SET_GPPINFO(shmInfo, chnlId)                                           \
                        (IS_VALID_MSGCHNLID(chnlId) ?                          \
                         ((shmInfo)->outputMsg = (Uint16) 0x01)  :             \
                         (SET_BIT((shmInfo)->outputMask, chnlId)))
#else /* if defined (MSGQ_COMPONENT) */
#define SET_GPPINFO(shmInfo, chnlId)    (SET_BIT((shmInfo)->outputMask, chnlId))
#endif /* defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @const  CLEAR_GPPINFO
 *
 *  @desc   This macro is used for clearing the gpp control information
 *          depending upon the channel id.
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define CLEAR_GPPINFO(shmInfo, chnlId)                                         \
                        (IS_VALID_MSGCHNLID(chnlId) ?                          \
                         ((shmInfo)->outputMsg = (Uint16) 0x0)  :              \
                         (CLEAR_BIT(shmInfo->outputMask, chnlId)))
#else /* if defined (MSGQ_COMPONENT) */
#define CLEAR_GPPINFO(shmInfo, chnlId)                                         \
                                (CLEAR_BIT((shmInfo)->outputMask, chnlId))
#endif /* defined (MSGQ_COMPONENT) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @const  MAX_POLL_COUNT
 *
 *  @desc   For debug mode, indicates the maximum count to wait for handshake
 *          to complete before timing out.
 *  ============================================================================
 */
#define MAX_POLL_COUNT  100000000
#endif /* defined (DDSP_DEBUG) */

/** ============================================================================
 *  @name   LDRV_Obj
 *
 *  @desc   Global defined in ldrv.c.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;


/** ============================================================================
 *  @name   SHM_DriverInfo
 *
 *  @desc   Array of Shared memory driver info structure.
 *          It assumes only one shared memory link exists between
 *          the GPP and the DSP.
 *  ============================================================================
 */
STATIC SHM_DriverInfo   SHM_DrvInfo [MAX_PROCESSORS] ;


/** ----------------------------------------------------------------------------
 *  @func   SHM_GetData
 *
 *  @desc   This function performs input operation on shared memory channels
 *          from a given DSP.
 *
 *  @arg    dspId
 *              Identifier of DSP from where input is requested.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              Generic failure.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None.
 *
 *  @see    SHM_PutData, SHM_DPC
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_GetData (IN ProcessorId dspId) ;

/** ----------------------------------------------------------------------------
 *  @func   SHM_PutData
 *
 *  @desc   This function performs output operation on shared memory channels
 *          toward a given DSP.
 *
 *  @arg    dspId
 *              Processor idenfitier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              Generic failure.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    SHM_GetData, SHM_DPC
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_PutData (IN ProcessorId dspId) ;


/** ----------------------------------------------------------------------------
 *  @func   GetNextOutputChannel
 *
 *  @desc   Find the channel that has data to send to DSP.
 *
 *  @arg    dspId
 *              Processor idenfitier.
 *  @arg    mask
 *              Mask to be used for searching for ready channels.
 *  @arg    chnlId
 *              Pointer to channel idenfitier to used as starting index to
 *              search for next available channel for output.
 *
 *  @ret    ChannelId of the channel that has data to send to DSP.
 *          MAX_CHANNELS if there is no channel with data.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    DataOutput
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
GetNextOutputChannel (IN  ProcessorId dspId,
                      IN  Uint32      mask,
                      OUT ChannelId * chnlId) ;

/** ----------------------------------------------------------------------------
 *  @func   SHM_InterruptDsp
 *
 *  @desc   Sends MAILBOX1 interrupt to DSP.
 *
 *  @arg    dspId
 *              Processod ID.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    SHM_ClearDspInterrupt, DSP_Interrupt
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_InterruptDsp (ProcessorId dspId) ;


/** ----------------------------------------------------------------------------
 *  @func   SHM_ClearDspInterrupt
 *
 *  @desc   Clears the MAILBOX1 interrupt received from DSP.
 *
 *  @arg    dspId
 *              Processod ID.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    SHM_ClearDspInterrupt, DSP_Interrupt
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_ClearDspInterrupt (ProcessorId dspId) ;


/** ============================================================================
 *  @name   SHM_Interface
 *
 *  @desc   Shared Memory Driver interface.
 *  ============================================================================
 */
LinkInterface SHM_Interface = {
    &SHM_Initialize,
    &SHM_Finalize,
    &SHM_OpenChannel,
    &SHM_CloseChannel,
    &SHM_CancelIO,
    &SHM_IO_Request,
    &SHM_ScheduleDPC,
    &SHM_HandshakeSetup,
    &SHM_HandshakeStart,
    &SHM_HandshakeComplete
} ;


/** ============================================================================
 *  @func   SHM_Initialize
 *
 *  @desc   This function allocates and initializes resources used by this
 *          component and registers interrupt handler for handling data
 *          transfer interrupts from DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_Initialize (IN ProcessorId dspId, IN LinkId lnkId)
{
    DSP_STATUS       status   = DSP_SOK ;
    SHM_DriverInfo * shmInfo  = NULL    ;
    DspObject *      dspObj   = NULL    ;
    LinkAttrs *      linkAttr = NULL    ;
    MemMapInfo mapInfo ;
#if defined (DDSP_DEBUG)
    Uint32           i ;
#endif  /* if defined (DDSP_DEBUG) */

    TRC_2ENTER ("SHM_Initialize", dspId, lnkId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj   = &(LDRV_Obj.dspObjects [dspId]) ;
    linkAttr = LDRV_Obj.linkTables [lnkId] ;
    shmInfo  = &(SHM_DrvInfo [dspId]) ;

    /*  ------------------------------------------------------------------------
     *  Map Shared memory area
     *  (tbd: Currently 1 Mb is mapped)
     *  ------------------------------------------------------------------------
     */
    mapInfo.src  = linkAttr->argument1 ;
    mapInfo.size = 0x100000 ;

    status = MEM_Map (&mapInfo) ;

    if (DSP_SUCCEEDED (status)) {
        shmInfo->ptrControl = (SHM_Control *) mapInfo.dst ;

        /*  --------------------------------------------------------------------
         *  Initialize the SHM Driver information fields
         *  --------------------------------------------------------------------
         */
        shmInfo->dspId      = dspId ;
        shmInfo->linkId     = lnkId ;

        shmInfo->intObj.intId = OMAP_MAILBOX_INT1 ;
        shmInfo->dpcObj     = NULL ;
        shmInfo->isrObj     = NULL ;

        /* TBD : Decide on MAX BUF size vs. 0.5MB */
        shmInfo->maxBufSize = linkAttr->maxBufSize ;
        shmInfo->ptrInpData = (Uint8 *) ((Uint8 *) (shmInfo->ptrControl) +
                                         sizeof (SHM_Control)) ;
        shmInfo->ptrOutData = (Uint8 *) (shmInfo->ptrInpData +
                                         (0x100000-sizeof(SHM_Control))/2) ;

        shmInfo->lastOutput = 0x0 ;
        shmInfo->outputMask = 0x0 ;
#if defined (MSGQ_COMPONENT)
        shmInfo->outputMsg  = 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

        /*  --------------------------------------------------------------------
         *  Initialize control structure.
         *  --------------------------------------------------------------------
         */
        shmInfo->ptrControl->handshakeGpp = (Uint16) 0x0 ;
        shmInfo->ptrControl->handshakeDsp = (Uint16) 0x0 ;
        shmInfo->ptrControl->dspFreeMask  = (Uint16) 0x0 ;
        shmInfo->ptrControl->gppFreeMask  = (Uint16) 0x0 ;
        shmInfo->ptrControl->inputFull    = (Uint16) 0x0 ;
        shmInfo->ptrControl->inputId      = (Uint16) 0x0 ;
        shmInfo->ptrControl->inputSize    = (Uint16) 0x0 ;
        shmInfo->ptrControl->outputFull   = (Uint16) 0x0 ;
        shmInfo->ptrControl->outputId     = (Uint16) 0x0 ;
        shmInfo->ptrControl->outputSize   = (Uint16) 0x0 ;
        shmInfo->ptrControl->argv         = (Uint16) 0x0 ;
        shmInfo->ptrControl->resv         = (Uint16) 0x0 ;
#if defined (MSGQ_COMPONENT)
        shmInfo->ptrControl->dspFreeMsg   = (Uint16) 0x0 ;
        shmInfo->ptrControl->gppFreeMsg   = (Uint16) 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

        /*  --------------------------------------------------------------------
         *  Initialize the input & output data area
         *  --------------------------------------------------------------------
         */
#if defined (DDSP_DEBUG)
        for (i = 0 ; i < shmInfo->maxBufSize ; i++) {
            *((volatile Uint8 *) (shmInfo->ptrInpData) + i) = 0xCD ;
            *((volatile Uint8 *) (shmInfo->ptrOutData) + i) = 0xEF ;
        }
#endif

        /*  --------------------------------------------------------------------
         *  Get link specific DPC function & create the DPC Object
         *  --------------------------------------------------------------------
         */
        status = DPC_Create (SHM_DPC, shmInfo, &(shmInfo->dpcObj)) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }


    /*  ----------------------------------------------------------------
     *  Get link specific interrupt information & create the ISR Object
     *  ----------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = ISR_Create (SHM_ISR,
                             shmInfo,
                             &(shmInfo->intObj),
                             &(shmInfo->isrObj)) ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    /*  ----------------------------------------------------------------
     *  Install the ISR, Assign DPC & Enable interrupt
     *  ----------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = ISR_Install (NULL, shmInfo->isrObj) ;

        if (DSP_SUCCEEDED (status)) {
            status  = (*(dspObj->interface->enableInterrupt)) (dspId,
                                                    dspObj,
                                                    &(shmInfo->intObj)) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("SHM_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_Finalize
 *
 *  @desc   This function finalizes the I/O module for a particular DSP.
 *          Finalizing means No further services will be provided by this
 *          module for this particular DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_Finalize (IN ProcessorId dspId, IN LinkId lnkId)
{
    DSP_STATUS       status    = DSP_SOK ;
    DSP_STATUS       tmpStatus = DSP_SOK ;
    SHM_DriverInfo * shmInfo   = NULL    ;
    DspObject *      dspObj    = NULL    ;
    MemUnmapInfo     unmapInfo ;
#if defined (DDSP_DEBUG)
    Uint32           i ;
#endif  /* if defined (DDSP_DEBUG) */

    TRC_2ENTER ("SHM_Finalize", dspId, lnkId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    shmInfo = &(SHM_DrvInfo [dspId]) ;

    /*  ------------------------------------------------------------------------
     *  Clear memory for Shared Memory Control & Data area
     *  ------------------------------------------------------------------------
     */
#if defined (DDSP_DEBUG)
    shmInfo->ptrControl->dspFreeMask  = (Uint16) 0x0 ;
    shmInfo->ptrControl->gppFreeMask  = (Uint16) 0x0 ;
    shmInfo->ptrControl->inputFull    = (Uint16) 0x0 ;
    shmInfo->ptrControl->inputId      = (Uint16) 0x0 ;
    shmInfo->ptrControl->inputSize    = (Uint16) 0x0 ;
    shmInfo->ptrControl->outputFull   = (Uint16) 0x0 ;
    shmInfo->ptrControl->outputId     = (Uint16) 0x0 ;
    shmInfo->ptrControl->outputSize   = (Uint16) 0x0 ;
    shmInfo->ptrControl->argv         = (Uint16) 0x0 ;
    shmInfo->ptrControl->resv         = (Uint16) 0x0 ;
#if defined (MSGQ_COMPONENT)
    shmInfo->ptrControl->dspFreeMsg   = (Uint16) 0x0 ;
    shmInfo->ptrControl->gppFreeMsg   = (Uint16) 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

    for (i = 0 ; i < shmInfo->maxBufSize ; i++) {
        *((volatile Uint8 *) (shmInfo->ptrInpData) + i) = 0xCD ;
        *((volatile Uint8 *) (shmInfo->ptrOutData) + i) = 0xEF ;
    }
#endif

    /*  ------------------------------------------------------------------------
     *  Disable the interrupt, uinstall it & delete the ISR object
     *  ------------------------------------------------------------------------
     */
    status = (*(dspObj->interface->disableInterrupt)) (dspId,
                                                       dspObj,
                                                       &(shmInfo->intObj)) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    tmpStatus = ISR_Uninstall (shmInfo->isrObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    tmpStatus = ISR_Delete (shmInfo->isrObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    shmInfo->isrObj = NULL ;

    /*  ------------------------------------------------------------------------
     *  Cancel DPC & Free the DPC Object
     *  ------------------------------------------------------------------------
     */
    tmpStatus = DPC_Cancel (shmInfo->dpcObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    tmpStatus = DPC_Delete (shmInfo->dpcObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    shmInfo->dpcObj = NULL ;

    /*  --------------------------------------------------------------------
     *  Unmap the Shared Memory Area
     *  --------------------------------------------------------------------
     */
    if (shmInfo->ptrControl != NULL) {
        unmapInfo.addr = (Uint32) shmInfo->ptrControl ;

        if (unmapInfo.addr != 0) {
            tmpStatus = MEM_Unmap (&unmapInfo) ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }
    }

    /*  --------------------------------------------------------------------
     *  Un-initialize the SHM Driver information fields
     *  --------------------------------------------------------------------
     */
    shmInfo->ptrControl = NULL ;
    shmInfo->maxBufSize = 0 ;
    shmInfo->ptrInpData = NULL ;
    shmInfo->ptrOutData = NULL ;
#if defined (MSGQ_COMPONENT)
    shmInfo->outputMsg  = 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

    TRC_1LEAVE ("SHM_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_OpenChannel
 *
 *  @desc   Open a channel for input/output.
 *          This driver ensures that the bit corresponding to given channel
 *          in outputMask/gppFreeMask is cleared.
 *          No specific processing is required
 *          when the channel is closed.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_OpenChannel (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS          status   = DSP_SOK ;
    SHM_DriverInfo *    shmInfo  = NULL    ;
    SHM_Control *       shmCtrl  = NULL    ;
    ChannelMode         chnlMode           ;

    TRC_2ENTER ("SHM_OpenChannel", dspId, chnlId) ;

    shmInfo = &(SHM_DrvInfo [dspId]) ;

    shmCtrl = shmInfo->ptrControl ;

    chnlMode = LDRV_CHNL_GetChannelMode (dspId, chnlId) ;

    if (chnlMode == ChannelMode_Input) {
        CLEAR_GPPFREEMASKBIT (shmCtrl, chnlId) ;
    }
    else {
        CLEAR_GPPINFO (shmInfo, chnlId) ;
    }

    TRC_1LEAVE ("SHM_OpenChannel", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_CloseChannel
 *
 *  @desc   Close a channel.
 *          This driver ensures that the bit corresponding to given channel
 *          in outputMask/gppFreeMask is cleared.
 *          No specific processing is required
 *          when the channel is closed.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_CloseChannel (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS          status   = DSP_SOK ;
    SHM_DriverInfo *    shmInfo  = NULL    ;
    SHM_Control *       shmCtrl  = NULL    ;
    ChannelMode         chnlMode           ;

    TRC_2ENTER ("SHM_CloseChannel", dspId, chnlId) ;

    shmInfo = &(SHM_DrvInfo [dspId]) ;

    shmCtrl = shmInfo->ptrControl ;

    chnlMode = LDRV_CHNL_GetChannelMode (dspId, chnlId) ;

    if (chnlMode == ChannelMode_Input) {
        CLEAR_GPPFREEMASKBIT (shmCtrl, chnlId) ;
    }
    else {
        CLEAR_GPPINFO (shmInfo, chnlId) ;
    }

    TRC_1LEAVE ("SHM_CloseChannel", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_CancelIO
 *
 *  @desc   All the requests for this driver have been cancelled.
 *          SHM driver clears the bit of that channel in gppFreeMask or
 *          outputMask.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_CancelIO (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS          status   = DSP_SOK ;
    SHM_DriverInfo *    shmInfo  = NULL    ;
    SHM_Control *       shmCtrl  = NULL    ;
    ChannelMode         chnlMode           ;

    TRC_2ENTER ("SHM_CancelIO", dspId, chnlId) ;

    shmInfo = &(SHM_DrvInfo [dspId]) ;

    shmCtrl = shmInfo->ptrControl ;

    chnlMode = LDRV_CHNL_GetChannelMode (dspId, chnlId) ;

    if (chnlMode == ChannelMode_Input) {
        CLEAR_GPPFREEMASKBIT (shmCtrl, chnlId) ;
    }
    else {
        CLEAR_GPPINFO (shmInfo, chnlId) ;
    }

    TRC_1LEAVE ("SHM_CancelIO", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_ISR
 *
 *  @desc   Interrupt handler for IO operations. It schedules a DPC to defer
 *          the actual data copy operation.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
SHM_ISR (Pvoid refData)
{
    DSP_STATUS          status   = DSP_SOK ;
    SHM_DriverInfo *    shmInfo  = (SHM_DriverInfo *) refData ;

    if (shmInfo != NULL) {
        status = SHM_ClearDspInterrupt (shmInfo->dspId) ;

        if (DSP_SUCCEEDED (status)) {
            status = DPC_Schedule (shmInfo->dpcObj) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
#if defined (DDSP_PROFILE)
            (LDRV_Obj.procStats.procData [shmInfo->dspId].intsFromDsp)++ ;
#endif /* defined (DDSP_PROFILE) */
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
}


/** ============================================================================
 *  @func   SHM_DPC
 *
 *  @desc   Deferred procedure call for performing IO operations.
 *          SHM_ISR schedules this DPC.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
SHM_DPC (IN Pvoid refData)
{
    DSP_STATUS          status   = DSP_SOK ;
    SHM_DriverInfo *    shmInfo  = (SHM_DriverInfo *) refData ;

    TRC_1ENTER ("SHM_DPC", refData) ;

    DBC_Require (refData != NULL) ;

    if (shmInfo != NULL) {
        status = SHM_GetData (shmInfo->dspId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
            TRC_1PRINT (TRC_LEVEL7, "SHM_GetData failed. [0x%x]\n", status) ;
        }

        status = SHM_PutData (shmInfo->dspId) ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
            TRC_1PRINT (TRC_LEVEL7, "SHM_PutData failed. [0x%x]\n", status) ;
        }
    }

    TRC_1LEAVE ("SHM_DPC", status) ;
}


/** ============================================================================
 *  @func   SHM_ScheduleDPC
 *
 *  @desc   Deferred procedure call for performing IO operations.
 *          SHM_ISR schedules this DPC.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_ScheduleDPC (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS  status  = DSP_SOK             ;

    TRC_2ENTER ("SHM_ScheduleDPC", dspId, chnlId) ;

    status = DPC_Schedule (SHM_DrvInfo [dspId].dpcObj) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SHM_ScheduleDPC", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_IO_Request
 *
 *  @desc   This function shall request a input/output buffer transfer.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_IO_Request (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS          status     = DSP_SOK ;
    SHM_DriverInfo *    shmInfo    = NULL    ;
    SHM_Control *       shmCtrl    = NULL    ;
    ChannelMode         chnlMode             ;

    TRC_2ENTER ("SHM_IO_Request", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (IS_VALID_CHNLID (dspId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    shmInfo = &(SHM_DrvInfo [dspId]) ;
    shmCtrl = shmInfo->ptrControl ;

    chnlMode = LDRV_CHNL_GetChannelMode (dspId, chnlId) ;
    if (chnlMode == ChannelMode_Input) {
        /*  --------------------------------------------------------------------
         *  Input Channel
         *  --------------------------------------------------------------------
         */
        SET_GPPFREEMASKBIT (shmCtrl, chnlId) ;

        TRC_0PRINT (TRC_LEVEL1, "INPUT Channel. Writing free mask \n") ;

        status = SHM_InterruptDsp (dspId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
#if defined (DDSP_PROFILE)
        if (DSP_SUCCEEDED (status)) {
            (LDRV_Obj.procStats.procData [dspId].intsToDsp)++ ;
        }
#endif /* defined (DDSP_PROFILE) */
    }
    else if (chnlMode == ChannelMode_Output) {
        /*  --------------------------------------------------------------------
         *  Output Channel
         *  --------------------------------------------------------------------
         */
        SET_GPPINFO (shmInfo,chnlId) ;

        TRC_0PRINT (TRC_LEVEL1, "OUTPUT Channel. Writing avail mask") ;
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SHM_IO_Request", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_HandshakeSetup
 *
 *  @desc   Does initialization of handshake fields in shared memory.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_HandshakeSetup (IN ProcessorId dspId)
{
    DSP_STATUS           status     = DSP_SOK ;
    SHM_Control *        shmCtrl    = NULL    ;

    TRC_1ENTER ("SHM_IO_HandshakeSetup", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    shmCtrl  = SHM_DrvInfo [dspId].ptrControl ;

    shmCtrl->handshakeDsp = 0x0 ;
    shmCtrl->handshakeGpp = 0x0 ;

    TRC_1LEAVE ("SHM_IO_HandshakeSetup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_HandshakeStart
 *
 *  @desc   Starts the handshake process toward specified DSP on this driver.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_HandshakeStart (IN ProcessorId dspId)
{
    DSP_STATUS           status     = DSP_SOK ;
    SHM_Control *        shmCtrl    = NULL    ;

    TRC_1ENTER ("SHM_IO_HandshakeStart", dspId) ;

    shmCtrl  = SHM_DrvInfo [dspId].ptrControl ;

    shmCtrl->handshakeGpp = SHM_GPP_TOKEN ;

    TRC_1LEAVE ("SHM_IO_HandshakeStart", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SHM_HandshakeComplete
 *
 *  @desc   Completes the handshake process toward specified DSP on this driver.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_HandshakeComplete (IN ProcessorId dspId)
{
    DSP_STATUS           status     = DSP_SOK ;
    SHM_Control *        shmCtrl    = NULL    ;

#if defined (DDSP_DEBUG)
    volatile Uint32      i          = 0       ;
#endif /* defined (DDSP_DEBUG) */

    TRC_1ENTER ("SHM_HandshakeComplete", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    shmCtrl  = SHM_DrvInfo [dspId].ptrControl ;

    while (   (shmCtrl->handshakeDsp != SHM_DSP_TOKEN)
           && DSP_SUCCEEDED (status)) {
#if defined (DDSP_DEBUG)
        i++ ;
        if (i == MAX_POLL_COUNT) {
            status = DSP_EFAIL ;
        }
#endif /* defined (DDSP_DEBUG) */
    }

    TRC_1LEAVE ("SHM_HandshakeComplete", status) ;

    return status ;
}


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   SHM_Debug
 *
 *  @desc   Prints information related to Shared Memory Driver.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
SHM_Debug (ProcessorId dspId)
{
    SHM_DriverInfo *    shmInfo = NULL ;
    SHM_Control *       shmCtrl = NULL ;

    TRC_1ENTER ("SHM_Debug", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (SHM_DrvInfo [dspId].ptrControl != NULL) ;

    shmInfo = &(SHM_DrvInfo [dspId]) ;
    TRC_0PRINT (TRC_LEVEL4, "SHM Driver Info\n") ;
    TRC_1PRINT (TRC_LEVEL4, "    PTR Control = 0x%x\n", shmInfo->ptrControl) ;
    TRC_1PRINT (TRC_LEVEL4, "    MAX BufSize = 0x%x\n", shmInfo->maxBufSize) ;
    TRC_1PRINT (TRC_LEVEL4, "    PTR InpData = 0x%x\n", shmInfo->ptrInpData) ;
    TRC_1PRINT (TRC_LEVEL4, "    PTR OutData = 0x%x\n", shmInfo->ptrOutData) ;

    shmCtrl = shmInfo->ptrControl ;

    TRC_0PRINT (TRC_LEVEL4, "SHM Control Area") ;
    TRC_1PRINT (TRC_LEVEL4, "    handshakeDsp = 0x%x\n",
                                                     shmCtrl->handshakeDsp) ;
    TRC_1PRINT (TRC_LEVEL4, "    handshakeGpp = 0x%x\n",
                                                     shmCtrl->handshakeGpp) ;
    TRC_1PRINT (TRC_LEVEL4, "    dspFreeMask  = 0x%x\n", shmCtrl->dspFreeMask) ;
    TRC_1PRINT (TRC_LEVEL4, "    gppFreeMask  = 0x%x\n", shmCtrl->gppFreeMask) ;
    TRC_1PRINT (TRC_LEVEL4, "    inputFull    = 0x%x\n", shmCtrl->inputFull) ;
    TRC_1PRINT (TRC_LEVEL4, "    inputId      = 0x%x\n", shmCtrl->inputId) ;
    TRC_1PRINT (TRC_LEVEL4, "    inputSize    = 0x%x\n", shmCtrl->inputSize) ;
    TRC_1PRINT (TRC_LEVEL4, "    outputFull   = 0x%x\n", shmCtrl->outputFull) ;
    TRC_1PRINT (TRC_LEVEL4, "    outputId     = 0x%x\n", shmCtrl->outputId) ;
    TRC_1PRINT (TRC_LEVEL4, "    outputSize   = 0x%x\n", shmCtrl->outputSize) ;
    TRC_1PRINT (TRC_LEVEL4, "    argv         = 0x%x\n", shmCtrl->argv) ;
    TRC_1PRINT (TRC_LEVEL4, "    resv         = 0x%x\n", shmCtrl->resv) ;

    TRC_0LEAVE ("SHM_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


/** ----------------------------------------------------------------------------
 *  @func   SHM_GetData
 *
 *  @desc   Attempts to read data from DSP side on a 'ready channel'.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_GetData (IN ProcessorId dspId)
{
    DSP_STATUS       status     = DSP_SOK ;
    SHM_DriverInfo * shmInfo    = NULL    ;
    SHM_Control *    shmCtrl    = NULL    ;
    LDRVChnlIRP *    chirp      = NULL    ;

    Uint32           bytes      = 0                 ;
    Bool             fClearChnl = FALSE             ;
    ChannelMode      chnlMode   = ChannelMode_Input ;

    ChannelId        chnlId    ;
    ChannelState     chnlState ;
    Endianism        endianism ;
    Uint32           wordSize  ;

    TRC_1ENTER ("SHM_GetData", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    shmInfo  = &(SHM_DrvInfo [dspId]) ;
    shmCtrl  = SHM_DrvInfo [dspId].ptrControl ;
    wordSize = LDRV_Obj.dspObjects [dspId].wordSize ;

    /*  ------------------------------------------------------------------------
     *  Look for channel on which data is available for input
     *  ------------------------------------------------------------------------
     */
    if (shmCtrl->inputFull == 1) {
        chnlId = SHM_GET_INPUTID (shmCtrl) ;
        if (IS_VALID_CHNLID (dspId, chnlId) || (IS_VALID_MSGCHNLID (chnlId))) {
            chnlState = LDRV_CHNL_GetChannelState (dspId, chnlId) ;

            if (chnlState != ChannelState_Closed) {
                chnlMode  = LDRV_CHNL_GetChannelMode  (dspId, chnlId) ;
                DBC_Assert (chnlMode == ChannelMode_Input) ;
            }

            /*  ----------------------------------------------------------------
             *  Check if channel is 'input mode' & 'ready for input'
             *  ----------------------------------------------------------------
             */
            if (   (chnlMode == ChannelMode_Input)
                && ((chnlState & ~ChannelState_EOS) == ChannelState_Ready)) {
                /*  ------------------------------------------------------------
                 *  Get the 'next' chirp on IO request queue for the channel
                 *  ------------------------------------------------------------
                 */
                chirp = LDRV_CHNL_GetRequestChirp (dspId, chnlId) ;

                if (chirp == NULL) {
                    /*  --------------------------------------------------------
                     *  Though 'input full', there are no buffers indicating
                     *  that channel is idling.
                     *  Clear out the physical input channel.
                     *  --------------------------------------------------------
                     */
                    DBC_Assert (!SHM_IS_GPPBUFFERFREE (shmCtrl, chnlId)) ;
                    fClearChnl = TRUE ;
                }
                else {
                    bytes = SHM_GET_INPUTSIZE (shmCtrl) * wordSize ;
                    if (bytes == 0) {
                        /*  ----------------------------------------------------
                         *  Buffer length of zero bytes indicates EOS.
                         *  This Asertion fails if DSP send EOS more than once
                         *  on a given channel
                         *  ----------------------------------------------------
                         */
                        DBC_Assert ((chnlState & ChannelState_EOS) == 0) ;

                        /*  ----------------------------------------------------
                         *  Update channel state & IO Completion status
                         *  ----------------------------------------------------
                         */
                        chnlState        = ChannelState_EOS ;
                        chirp->iocStatus = LDRV_CHNL_IOCSTATE_EOS ;

                        LDRV_CHNL_SetChannelState (dspId, chnlId, chnlState) ;
                    }
                    else {
                        /*  ----------------------------------------------------
                         *  Assert that buffer issued by client is big enough
                         *  to read the input buffer.
                         *  ----------------------------------------------------
                         */
                        DBC_Assert (chirp->size >= bytes) ;

                        /*  ----------------------------------------------------
                         *  Read the input buffer
                         *  Number of bytes read is minimum of:
                         *  - bytes written by the DSP
                         *  - size of user buffer.
                         *  ----------------------------------------------------
                         */
                        bytes     = MIN (bytes, chirp->size) ;
                        endianism = LDRV_CHNL_GetChannelEndianism (dspId,
                                                                   chnlId) ;

                        status    = MEM_Copy (chirp->buffer,
                                              shmInfo->ptrInpData,
                                              bytes,
                                              endianism) ;

                        if (DSP_SUCCEEDED (status)) {
                            chirp->size = bytes ;
                            chirp->iocStatus |=
                                       LDRV_CHNL_IOCSTATE_COMPLETE ;
#if defined (DDSP_PROFILE)
                            LDRV_Obj.chnlStats.chnlData [dspId][chnlId]
                                .transferred += bytes ;
#endif /* defined (DDSP_PROFILE) */
                        }
                        else {
                            SET_FAILURE_REASON ;
                        }
                    }

                    if (DSP_SUCCEEDED (status)) {
                        /*  ----------------------------------------------------
                         *  Intimate DSP if no more IO buffers are available
                         *  ----------------------------------------------------
                         */
                        if (LDRV_CHNL_ChannelHasMoreChirps (dspId, chnlId)
                                                                    == FALSE) {
                            CLEAR_GPPFREEMASKBIT (shmCtrl, chnlId) ;
                        }
                        fClearChnl = TRUE ;
                    }
                }
            }
            else {
                /*  ------------------------------------------------------------
                 *  Channel is either not 'input mode' or 'not ready' for input
                 *  ------------------------------------------------------------
                 */
                if (chnlMode == ChannelMode_Output) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }

                fClearChnl = TRUE ;
            }

            /*  ----------------------------------------------------------------
             *  Intimate the DSP that buffer has been read.
             *  ----------------------------------------------------------------
             */
            if (fClearChnl && DSP_SUCCEEDED (status)) {
                shmCtrl->inputFull = 0 ;

                status = SHM_InterruptDsp (dspId) ;

                if (DSP_SUCCEEDED (status)) {
                    /*  This will happen in the race condition where DSP reads
                     *  the gppFreeMask for this channel before it was cleared.
                     *  In this case, IOC cannot be added, since GPP is not
                     *  ready to receive any data.
                     */
                    if (chirp != NULL) {
                        status = LDRV_CHNL_AddIOCompletion (dspId,
                                                            chnlId,
                                                            chirp) ;
                        if (DSP_SUCCEEDED (status)) {
                            /* Call the callback function if it is specified */
                            if (chirp->callback != NULL) {
                                status = chirp->callback (dspId,
                                                          status,
                                                          chirp->buffer,
                                                          chirp->size,
                                                          (Pvoid) chirp->arg) ;
                                if (DSP_FAILED (status)) {
                                    SET_FAILURE_REASON ;
                                }
                            }
                        }
                        else {
                            SET_FAILURE_REASON ;
                        }

                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
        }
        else {
            /*  ----------------------------------------------------------------
             *  Incorrect channel ID indicated from the DSP
             *  ----------------------------------------------------------------
             */
            TRC_1PRINT (TRC_LEVEL1,
                        "Channel ID from shared memory is incorrect [%x]\n",
                        chnlId) ;
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("SHM_GetData", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   SHM_PutData
 *
 *  @desc   Attempts to write data to DSP side on a 'ready channel'.
 *          Data for requested channel is written only when the shared
 *          memory control structure indicates that there isn't already
 *          some 'written' by GPP but 'not read' by DSP.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_PutData (IN ProcessorId dspId)
{
    DSP_STATUS       status     = DSP_SOK ;
    SHM_DriverInfo * shmInfo    = NULL    ;
    SHM_Control *    shmCtrl    = NULL    ;
    LDRVChnlIRP *    chirp      = NULL    ;
    Uint32           bytes      = 0       ;
    Uint16           outputMask           ;
    ChannelId        chnlId               ;
    ChannelState     chnlState            ;
    Endianism        endianism            ;
    Uint32           wordSize             ;

    TRC_1ENTER ("SHM_PutData", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    shmInfo  = &(SHM_DrvInfo [dspId]) ;
    shmCtrl  = SHM_DrvInfo [dspId].ptrControl ;
    wordSize = LDRV_Obj.dspObjects [dspId].wordSize ;

    /*  ------------------------------------------------------------------------
     *  Look for channel on which data is available for output
     *  ------------------------------------------------------------------------
     */
    if (shmCtrl->outputFull == 0) {
        outputMask = shmInfo->outputMask ;
        status = GetNextOutputChannel (dspId,
                                      (outputMask & shmCtrl->dspFreeMask),
                                      &chnlId) ;

        if (DSP_SUCCEEDED (status)) {
            /*  ----------------------------------------------------------------
             *  Check if channel is 'ready for 'output'
             *  ----------------------------------------------------------------
             */
            chnlState = LDRV_CHNL_GetChannelState (dspId, chnlId) ;
            if ((chnlState & ~ChannelState_EOS) == ChannelState_Ready) {
                TRC_1PRINT (TRC_LEVEL2, "Output : ChnlId = [0x%x]\n", chnlId) ;

                /*  ------------------------------------------------------------
                 *  Get the 'next' chirp on IO request queue for the channel
                 *  ------------------------------------------------------------
                 */
                chirp = LDRV_CHNL_GetRequestChirp (dspId, chnlId) ;
                if (chirp == NULL) {
                    /*  --------------------------------------------------------
                     *  Function was called without a pending IO request.
                     *  Shouldn't reach here...
                     *  --------------------------------------------------------
                     */
                    status = DSP_EPOINTER ;
                    SET_FAILURE_REASON ;
                }
                else {
                    /*  --------------------------------------------------------
                     *  Check if any more IO buffers are available
                     *  --------------------------------------------------------
                     */
                    if (LDRV_CHNL_ChannelHasMoreChirps (dspId, chnlId)
                                                                 == FALSE) {
                        CLEAR_GPPINFO (shmInfo, chnlId) ;
                    }

                    /*  --------------------------------------------------------
                     *  Write the user buffer
                     *  Number of bytes written is minumum of:
                     *  - Size of user buffer
                     *  - Maximum buffer size supported by SHM driver
                     *  --------------------------------------------------------
                     */
                    endianism = LDRV_CHNL_GetChannelEndianism (dspId, chnlId) ;
                    bytes = MIN (chirp->size, shmInfo->maxBufSize) ;

                    status = MEM_Copy (shmInfo->ptrOutData,
                                       chirp->buffer,
                                       bytes,
                                       endianism) ;

                    if (DSP_SUCCEEDED (status)) {
                        /*  ----------------------------------------------------
                         *  Intimate DSP that a buffer is available
                         *  ----------------------------------------------------
                         */
                        shmCtrl->outputId   = (Uint16) chnlId ;
                        shmCtrl->outputSize =
                                  (Uint16) (  (bytes + (wordSize - 1))
                                            / wordSize) ;

                        shmCtrl->outputFull = 1 ;
                        status = SHM_InterruptDsp (dspId) ;

                        if (DSP_SUCCEEDED (status)) {
                            chirp->iocStatus &= LDRV_CHNL_IOCSTATE_COMPLETE ;

                            status = LDRV_CHNL_AddIOCompletion (dspId,
                                                                chnlId,
                                                                chirp) ;

                            if (DSP_SUCCEEDED (status)) {
                                /* Call the callback function if it is specified */
                                if (chirp->callback != NULL) {
                                    status = chirp->callback (dspId,
                                                              status,
                                                              chirp->buffer,
                                                              chirp->size,
                                                              (Pvoid) chirp->arg) ;
                                    if (DSP_FAILED (status)) {
                                        SET_FAILURE_REASON ;
                                    }
                                }
                            }
                            else {
                                SET_FAILURE_REASON ;
                            }
                        }
                        else {
                            SET_FAILURE_REASON ;
                        }

#if defined (DDSP_PROFILE)
                        LDRV_Obj.chnlStats.chnlData [dspId][chnlId].transferred
                            += bytes ;
#endif /* defined (DDSP_PROFILE) */
                    }
                }
            }
            else {
                /*  ------------------------------------------------------------
                 *  Function was called when channel wasn't ready for output.
                 *  Shouldn't reach here...
                 *  ------------------------------------------------------------
                 */
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            /*  ----------------------------------------------------------------
             *  It is a valid condition that a channel may not be available
             *  for output. Modify the status back to 'success'.
             *  ----------------------------------------------------------------
             */
            status = DSP_SOK ;
        }
    }

    TRC_1LEAVE ("SHM_PutData", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   GetNextOutputChannel
 *
 *  @desc   Find the channel that has data to send to DSP.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
GetNextOutputChannel (IN  ProcessorId dspId,
                      IN  Uint32      mask,
                      OUT ChannelId * pChnlId)
{
    DSP_STATUS          status    = DSP_SOK      ;
    SHM_DriverInfo *    shmInfo   = NULL         ;
    Bool                done      = FALSE        ;
    ChannelId           startId   = MAX_CHANNELS ;
    ChannelId           selectId  = MAX_CHANNELS ;
    Bool                isChnlSet = FALSE        ;
    Uint32              shift                    ;
#if defined (MSGQ_COMPONENT)
    SHM_Control *       shmCtrl    = NULL    ;
#endif /* if defined (MSGQ_COMPONENT) */

    TRC_3ENTER ("GetNextOutputChannel", dspId, mask, pChnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (pChnlId != NULL) ;

    *pChnlId = MAX_CHANNELS ;

    shmInfo = &(SHM_DrvInfo [dspId]) ;

#if defined (MSGQ_COMPONENT)
    shmCtrl  = SHM_DrvInfo [dspId].ptrControl ;

    if (   (shmInfo->outputMsg  == (Uint16) 0x01)
        && (shmCtrl->dspFreeMsg == (Uint16) 0x01)) {
        *pChnlId = ID_MSGCHNL_TO_DSP ;
        isChnlSet = TRUE ;
    }
#endif /* if defined (MSGQ_COMPONENT) */

    if (isChnlSet == FALSE) {
        if (mask != 0) {
            startId  = shmInfo->lastOutput ;
            selectId = startId ;

            do {
                selectId = selectId + 1 ;
                selectId = ((selectId == MAX_CHANNELS) ? 0 : selectId) ;

                shift = (1 << selectId) ;

                if ((mask & shift) != 0) {
                    shmInfo->lastOutput = selectId ;
                    done = TRUE ;
                }
            } while ((selectId != startId) && (done == FALSE)) ;
        }

        if (done == TRUE) {
            *pChnlId = selectId ;
        }
        else {
            status = DSP_ENOTFOUND ;
        }
    }

    DBC_Ensure (   (   DSP_SUCCEEDED (status)
                    && (   (IS_VALID_MSGCHNLID (*pChnlId))
                        || (IS_VALID_CHNLID (dspId, *pChnlId))))
                || (DSP_FAILED (status) && (*pChnlId == MAX_CHANNELS))) ;


    TRC_1LEAVE ("GetNextOutputChannel", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   SHM_InterruptDsp
 *
 *  @desc   Maps the shared memory area.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_InterruptDsp (ProcessorId dspId)
{
    DSP_STATUS          status     = DSP_SOK ;
    DspObject *         dspObj     = NULL    ;
    DspInterface *      dspIntf    = NULL    ;
    Uint16              intVal     = OMAP_MAILBOX_SIGNATURE ;
    InterruptObject     intObj     ;

    TRC_1ENTER ("SHM_InterruptDsp", dspId) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    intObj.intId = OMAP_MAILBOX_INT1 ;

    status = (*(dspIntf->interrupt)) (dspId,
                                      dspObj,
                                      &intObj,
                                      (Pvoid) &intVal) ;

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        (LDRV_Obj.procStats.procData [dspId].intsToDsp)++ ;
    }
#endif /* defined (DDSP_PROFILE) */

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SHM_InterruptDsp", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   SHM_ClearDspInterrupt
 *
 *  @desc   Maps the shared memory area.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SHM_ClearDspInterrupt (ProcessorId dspId)
{
    DSP_STATUS          status     = DSP_SOK ;
    DspObject *         dspObj     = NULL    ;
    DspInterface *      dspIntf    = NULL    ;
    Uint16              intVal     = OMAP_MAILBOX_SIGNATURE ;
    InterruptObject     intObj     ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    intObj.intId = OMAP_MAILBOX_INT1 ;

    status = (*(dspIntf->clearInterrupt)) (dspId,
                                           dspObj,
                                           &intObj,
                                           (Pvoid) &intVal) ;

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        (LDRV_Obj.procStats.procData [dspId].intsToDsp)++ ;
    }
#endif /* defined (DDSP_PROFILE) */

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif
