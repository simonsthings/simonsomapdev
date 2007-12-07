/** ============================================================================
 *  @file   ldrv_io.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implements the interface exposed by LDRV_IO subcomponent.
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
#include <mem.h>
#include <cfg.h>
#include <sync.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Link Driver                 */
#if defined (MSGQ_COMPONENT)
#include <ldrv_mqt.h>
#include <ldrv_mqa.h>
#endif /* if defined (MSGQ_COMPONENT) */

#include <ldrv.h>
#include <ldrv_chnl.h>
#include <ldrv_io.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_IO

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_IO, __LINE__)

/** ============================================================================
 *  @name   LDRV_Obj
 *
 *  @desc   Global defined in ldrv.c.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;


/** ----------------------------------------------------------------------------
 *  @func   LDRV_IO_GetLinkId
 *
 *  @desc   Get the link identifier for a given channel
 *
 *  @arg    dspId
 *              Processor idenfitier.
 *  @arg    chnlId
 *              Channel idenfitier.
 *  @arg    lnkId
 *              Placeholder for the link identifier
 *
 *  @ret    DSP_SOK
 *              If channel id is valid & maps to a link towards DSP.
 *          DSP_EFAIL
 *              If an error was encountered in mapping channel ID to link ID.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *          lnkId must be valid pointer.
 *
 *  @leave  None.
 *
 *  @see    DataInput, DataOutput
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_IO_GetLinkId (IN  ProcessorId  dspId,
                   IN  ChannelId    chnlId,
                   OUT LinkId *     lnkID) ;


/** ============================================================================
 *  @func   LDRV_IO_Initialize
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
LDRV_IO_Initialize (IN ProcessorId dspId)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspObject *          dspObj   = NULL    ;
    LinkInterface *      lnkIntf  = NULL    ;
    Uint32               i                  ;

    TRC_1ENTER ("LDRV_IO_Initialize", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    /*  ------------------------------------------------------------------------
     *  Initialize all links toward the DSP.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;

        for (i = 0 ; (i < dspObj->numLinks) && (DSP_SUCCEEDED (status)) ; i++) {
            lnkIntf = (LinkInterface *) (dspObj->linkTable [i]).interface ;
            status  = (lnkIntf->initialize) (dspId, i) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }

        if (DSP_FAILED (status)) {
            LDRV_IO_Finalize (dspId) ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_IO_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_IO_Finalize
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
LDRV_IO_Finalize (IN ProcessorId dspId)
{
    DSP_STATUS           status    = DSP_SOK ;
    DSP_STATUS           tmpStatus = DSP_SOK ;
    DspObject *          dspObj    = NULL    ;
    LinkInterface *      lnkIntf   = NULL    ;
    Uint32               i                   ;

    TRC_1ENTER ("LDRV_IO_Finalize", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;

    /*  ------------------------------------------------------------------------
     *  Finalize all links toward the DSP.
     *  ------------------------------------------------------------------------
     */
    dspObj = &(LDRV_Obj.dspObjects [dspId]) ;

    for (i = 0 ; (i < dspObj->numLinks) ; i++) {
        lnkIntf = (LinkInterface *) (dspObj->linkTable [i]).interface ;

        tmpStatus = (lnkIntf->finalize) (dspId, i) ;
        if (DSP_FAILED (tmpStatus)) {
            SET_FAILURE_REASON ;

            if (DSP_SUCCEEDED (status)) {
                status = tmpStatus ;
            }
        }
    }

    TRC_1LEAVE ("LDRV_IO_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_IO_OpenChannel
 *
 *  @desc   Open a channel for input/output.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_OpenChannel (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status     = DSP_SOK ;
    DspObject *     dspObj     = NULL    ;
    LinkInterface * lnkIntf    = NULL    ;
    LinkId          lnkId                ;

    TRC_2ENTER ("LDRV_IO_OpenChannel", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (IS_VALID_CHNLID (dspId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    status = LDRV_IO_GetLinkId (dspId, chnlId, &lnkId) ;

    if (DSP_SUCCEEDED (status)) {
        dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
        lnkIntf = (dspObj->linkTable [lnkId]).interface ;

        status = (lnkIntf->openChannel) (dspId, chnlId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_IO_OpenChannel", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_IO_CloseChannel
 *
 *  @desc   Close a channel.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_CloseChannel (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status     = DSP_SOK ;
    DspObject *     dspObj     = NULL    ;
    LinkInterface * lnkIntf    = NULL    ;
    LinkId          lnkId                ;

    TRC_2ENTER ("LDRV_IO_CloseChannel", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (IS_VALID_CHNLID (dspId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    status = LDRV_IO_GetLinkId (dspId, chnlId, &lnkId) ;

    if (DSP_SUCCEEDED (status)) {
        dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
        lnkIntf = (dspObj->linkTable [lnkId]).interface ;

        status = (lnkIntf->closeChannel) (dspId, chnlId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_IO_CloseChannel", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_IO_Cancel
 *
 *  @desc   Cancel a channel.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Cancel (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status     = DSP_SOK ;
    DspObject *     dspObj     = NULL    ;
    LinkInterface * lnkIntf    = NULL    ;
    LinkId          lnkId                ;

    TRC_2ENTER ("LDRV_IO_Cancel", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (IS_VALID_CHNLID (dspId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    status = LDRV_IO_GetLinkId (dspId, chnlId, &lnkId) ;

    if (DSP_SUCCEEDED (status)) {
        dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
        lnkIntf = (dspObj->linkTable [lnkId]).interface ;

        status = (lnkIntf->cancelChannel) (dspId, chnlId) ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_IO_Cancel", status) ;

    return status;
}


/** ============================================================================
 *  @func   LDRV_IO_Request
 *
 *  @desc   This function sends an IO request on specified channel to the
 *          link driver.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Request (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status     = DSP_SOK ;
    DspObject *     dspObj     = NULL    ;
    LinkInterface * lnkIntf    = NULL    ;
    LinkId          lnkId                ;

    TRC_2ENTER ("LDRV_IO_Request", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (IS_VALID_CHNLID (dspId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;

    status = LDRV_IO_GetLinkId (dspId, chnlId, &lnkId) ;

    if (DSP_SUCCEEDED (status)) {
        dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
        lnkIntf = (dspObj->linkTable [lnkId]).interface ;

        status = (lnkIntf->ioRequest) (dspId, chnlId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_IO_Request", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_IO_ScheduleDPC
 *
 *  @desc   Schedules DPC for IO with the DSP specified by dspId.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_ScheduleDPC (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status     = DSP_SOK ;
    DspObject *     dspObj     = NULL    ;
    LinkInterface * lnkIntf    = NULL    ;
    LinkId          lnkId                ;

    TRC_2ENTER ("LDRV_IO_ScheduleDPC", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    status = LDRV_IO_GetLinkId (dspId, chnlId, &lnkId) ;

    if (DSP_SUCCEEDED (status)) {
        dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
        lnkIntf = (dspObj->linkTable [lnkId]).interface ;

        status = (lnkIntf->scheduleDpc) (dspId, chnlId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_IO_ScheduleDPC", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_IO_HandshakeSetup
 *
 *  @desc   Does necessary initializations for handshake procedure.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_HandshakeSetup (IN ProcessorId dspId)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspObject *          dspObj   = NULL    ;
    LinkInterface *      lnkIntf  = NULL    ;
    Uint32               i                  ;

    TRC_1ENTER ("LDRV_IO_HandshakeSetup", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj = &(LDRV_Obj.dspObjects [dspId]) ;

    for (i = 0 ; (i < dspObj->numLinks) ; i++) {
        lnkIntf = (LinkInterface *) (dspObj->linkTable [i]).interface ;

        status = (lnkIntf->handshakeSetup) (dspId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LDRV_IO_HandshakeSetup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_IO_Handshake
 *
 *  @desc   Does the necessary handshake (if required) between for the links
 *          across GPP & DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Handshake (IN ProcessorId dspId)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspObject *          dspObj   = NULL    ;
    LinkInterface *      lnkIntf  = NULL    ;
    Uint32               i                  ;

    TRC_1ENTER ("LDRV_IO_Handshake", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj = &(LDRV_Obj.dspObjects [dspId]) ;

    for (i = 0 ; (i < dspObj->numLinks) ; i++) {
        lnkIntf = (LinkInterface *) (dspObj->linkTable [i]).interface ;

        /*  --------------------------------------------------------------------
         *  Start the handshake
         *  --------------------------------------------------------------------
         */
        status = (lnkIntf->handshakeStart) (dspId) ;
        if (DSP_SUCCEEDED (status)) {
            /*  ----------------------------------------------------------------
             *  Wait for handshake to complete
             *  ----------------------------------------------------------------
             */
            status = (lnkIntf->handshakeComplete) (dspId) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LDRV_IO_Handshake", status) ;

    return status ;
}


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_IO_Debug
 *
 *  @desc   Prints the current status of this subcomponent.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
LDRV_IO_Debug (IN  ProcessorId  dspId)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspObject *          dspObj   = NULL    ;
    LinkInterface *      lnkIntf  = NULL    ;
    Uint32               i                  ;

    TRC_1ENTER ("LDRV_IO_Debug", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;

    for (i = 0 ; (i < dspObj->numLinks) && (DSP_SUCCEEDED (status)) ; i++) {
        lnkIntf = (LinkInterface *) (dspObj->linkTable [i]).interface ;

        TRC_2PRINT (TRC_LEVEL4,
                    "    Link Interface Table [%d] = 0x%x\n",
                    i, lnkIntf) ;
    }

    TRC_1LEAVE ("LDRV_IO_Debug", status) ;
}
#endif /* defined (DDSP_DEBUG) */


/*  ----------------------------------------------------------------------------
 *  @func   LDRV_IO_GetLinkId
 *
 *  @desc   Get the link identifier for a given channel.
 *          If the channel is a messaging channel then the link identifier is
 *          extracted from the respective mqtObject.
 *          Currently validates channelID for only one link.
 *          LinkId 0 defaults to SHM Driver in current implementation.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_IO_GetLinkId (IN  ProcessorId  dspId,
                   IN  ChannelId    chnlId,
                   OUT LinkId *     lnkId)
{
    DSP_STATUS status = DSP_SOK ;

#if defined (MSGQ_COMPONENT)
    Uint32  mqtId ;
#endif /* if defined (MSGQ_COMPONENT) */

    TRC_1ENTER ("LDRV_IO_GetLinkId", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (IS_VALID_CHNLID (dspId, chnlId))
                 || (IS_VALID_MSGCHNLID (chnlId))) ;
    DBC_Require (lnkId != NULL) ;

    if (lnkId != NULL) {
#if defined (MSGQ_COMPONENT)
        if (IS_VALID_MSGCHNLID (chnlId)) {
            mqtId = LDRV_Obj.dspObjects [dspId].mqtId ;
            *lnkId = LDRV_Obj.mqtObjects [mqtId].linkId ;
        }
#endif /* if defined (MSGQ_COMPONENT) */
        if (IS_RANGE_VALID (chnlId, 0, MAX_CHANNELS)) {
            *lnkId = 0 ;
        }
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_IO_GetLinkId", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif
