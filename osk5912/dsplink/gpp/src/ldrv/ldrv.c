/** ============================================================================
 *  @file   ldrv.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implementation of initialization and finalization functionality
 *          for LDRV.
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
#include <intobject.h>
#include <errbase.h>
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
#include <print.h>
#include <mem.h>
#include <cfg.h>
#include <sync.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Link Driver                 */
#if defined (MSGQ_COMPONENT)
#include <ldrv_mqt.h>
#include <ldrv_mqa.h>
#endif /* if defined (MSGQ_COMPONENT) */

#include <ldrv.h>
#include <ldrv_proc.h>
#if defined (CHNL_COMPONENT)
#include <ldrv_chnl.h>
#endif /* if defined (CHNL_COMPONENT) */

/*  ----------------------------------- Hardware Abstration Layer   */


#if defined (__cplusplus)
EXTERN "C" {
#endif

/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_LDRV

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_LDRV, __LINE__)


/** ============================================================================
 *  @name   LDRV_Obj
 *
 *  @desc   Object containing the component specific globals.
 *  ============================================================================
 */
LDRV_Object LDRV_Obj ;


/** ----------------------------------------------------------------------------
 *  @func   LDRV_InitializeDspObj
 *
 *  @desc   Initializes the specified DSP object.
 *          It imports attributes of the DSP object from the configuration
 *          database for initialization.
 *
 *  @arg    dspId
 *              Processor idenfitier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EFAIL
 *              This error is returned in either of the following conditions:
 *              - Could not get record from configuration database.
 *              - Could not validate Link Driver & DSP interfaces.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_FinalizeDspObj
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_InitializeDspObj (ProcessorId dspId) ;


/** ----------------------------------------------------------------------------
 *  @func   LDRV_FinalizeDspObj
 *
 *  @desc   Finalizes the DSP object.
 *
 *  @arg    dspId
 *              Processor idenfitier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_InitializeDspObj
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_FinalizeDspObj (ProcessorId dspId) ;


#if defined (CHNL_COMPONENT)
/** ----------------------------------------------------------------------------
 *  @func   LDRV_CfgGetLinkTable
 *
 *  @desc   Reads the content of linktable associated with specified DSP
 *          from the configuration database.
 *
 *  @arg    dspId
 *              Processor idenfitier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EFAIL
 *              Could not get record from configuration database.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_Initialize
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_CfgGetLinkTable (ProcessorId dspId) ;
#endif /* if defined (CHNL_COMPONENT) */

/** ----------------------------------------------------------------------------
 *  @func   LDRV_CfgGetMmuTable
 *
 *  @desc   Reads the content of MMU table associated with specified DSP
 *          from the configuration database.
 *
 *  @arg    dspId
 *              Processor idenfitier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EFAIL
 *              Could not get record from configuration database.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LDRV_Initialize
 *
 *  @modif  LDRV_Obj.mmuTables [mmuTableId]
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_CfgGetMmuTable (ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_Initialize
 *
 *  @desc   Allocates resources and initializes the LDRV component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_Initialize ()
{
    DSP_STATUS  status    = DSP_SOK ;
    CFG_Driver  cfgDrvObj           ;
    CFG_Gpp     cfgGppObj           ;
    Uint32      i                   ;
#if defined (MSGQ_COMPONENT)
    MqaInterface * mqaInterface = NULL ;
    MqtInterface * mqtInterface = NULL ;
    CFG_Mqa        cfgMqaObj           ;
    CFG_Mqt        cfgMqtObj           ;
#endif /* if defined (MSGQ_COMPONENT) */

    TRC_0ENTER ("LDRV_Initialize") ;

    LDRV_Obj.dspObjects = NULL ;
    LDRV_Obj.linkTables = NULL ;
    LDRV_Obj.mmuTables  = NULL ;
#if defined (MSGQ_COMPONENT)
    LDRV_Obj.mqaObjects = NULL ;
    LDRV_Obj.mqtObjects = NULL ;
#endif /* if defined (MSGQ_COMPONENT) */

    /*  ------------------------------------------------------------------------
     *  Get the driver object
     *  ------------------------------------------------------------------------
     */
    status = CFG_GetRecord (CFG_DRIVER_OBJECT, CFG_ID_NONE, &cfgDrvObj) ;

    /*  ------------------------------------------------------------------------
     *  Get the GPP object
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = CFG_GetRecord (CFG_GPP_OBJECT, CFG_ID_NONE, &cfgGppObj) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    /*  ------------------------------------------------------------------------
     *  Allocate memory to hold the DSP object(s)
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        LDRV_Obj.numDsps = cfgGppObj.numDsps ;

        status = MEM_Calloc ((Void **) &(LDRV_Obj.dspObjects),
                            LDRV_Obj.numDsps * sizeof (DspObject),
                            MEM_DEFAULT) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    /*  ------------------------------------------------------------------------
     *  Populate the Link Table(s)
     *  ------------------------------------------------------------------------
     */
#if defined (CHNL_COMPONENT)
    if (DSP_SUCCEEDED (status)) {
        LDRV_Obj.numLinkTables = cfgDrvObj.linkTables ;

        status = MEM_Calloc ((Void **) &(LDRV_Obj.linkTables),
                             LDRV_Obj.numLinkTables * sizeof (LinkAttrs *),
                             MEM_DEFAULT) ;

        if (DSP_SUCCEEDED (status)) {
            for (i = 0 ;   (DSP_SUCCEEDED (status)
                        &&  (i < LDRV_Obj.numDsps)) ; i++) {
                status = LDRV_CfgGetLinkTable (i) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
#endif /* if defined (CHNL_COMPONENT) */

#if defined (MSGQ_COMPONENT)
    if (DSP_SUCCEEDED (status)) {
        LDRV_Obj.numMqas  = cfgDrvObj.numMqas  ;
        LDRV_Obj.numMqts  = cfgDrvObj.numMqts  ;
        LDRV_Obj.localMqt = cfgDrvObj.localMqt ;

        status = MEM_Alloc ((Void **) &(LDRV_Obj.mqaObjects),
                            (LDRV_Obj.numMqas * sizeof (MqaObject)),
                            MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            for (i = 0 ; (   (i < LDRV_Obj.numMqas)
                          && (DSP_SUCCEEDED (status))) ; i++) {
                /*  ------------------------------------------------------------
                 *  Get the MQA object.
                 *  ------------------------------------------------------------
                 */
                status = CFG_GetRecord (CFG_MQA_OBJECT, i, &cfgMqaObj) ;
                if (DSP_SUCCEEDED (status)) {
                    LDRV_Obj.mqaObjects [i].interface =
                                        (MqaInterface *) (cfgMqaObj.interface) ;
                    mqaInterface = LDRV_Obj.mqaObjects [i].interface ;
                    DBC_Assert (mqaInterface != NULL) ;
                    if (   (mqaInterface->mqaInitialize == NULL)
                        || (mqaInterface->mqaFinalize   == NULL)
                        || (mqaInterface->mqaOpen       == NULL)
                        || (mqaInterface->mqaClose      == NULL)
                        || (mqaInterface->mqaAlloc      == NULL)
                        || (mqaInterface->mqaFree       == NULL)) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }
#if defined (DDSP_DEBUG)
                    else {
                        status = GEN_Strcpyn (
                             cfgMqaObj.mqaName,
                             LDRV_Obj.mqaObjects [i].mqaName,
                             DSP_MAX_STRLEN) ;
                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }
#endif /* defined (DDSP_DEBUG) */
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

    if (DSP_SUCCEEDED (status)) {
        status = MEM_Alloc ((Void **) &(LDRV_Obj.mqtObjects),
                            (LDRV_Obj.numMqts * sizeof (MqtObject)),
                            MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            for (i = 0 ; (   (i < LDRV_Obj.numMqts)
                          && (DSP_SUCCEEDED (status))) ; i++) {
                /*  ------------------------------------------------------------
                 *  Get the MQT object.
                 *  ------------------------------------------------------------
                 */
                status = CFG_GetRecord (CFG_MQT_OBJECT, i, &cfgMqtObj) ;
                if (DSP_SUCCEEDED (status)) {
                    LDRV_Obj.mqtObjects [i].interface =
                                        (MqtInterface *) (cfgMqtObj.interface) ;
                    LDRV_Obj.mqtObjects [i].linkId    = cfgMqtObj.linkId ;

                    mqtInterface = LDRV_Obj.mqtObjects [i].interface ;
                    DBC_Assert (mqtInterface != NULL) ;
                    if (   (mqtInterface->mqtInitialize == NULL)
                        || (mqtInterface->mqtFinalize   == NULL)
                        || (mqtInterface->mqtOpen       == NULL)
                        || (mqtInterface->mqtClose      == NULL)
                        || (mqtInterface->mqtCreate     == NULL)
                        || (mqtInterface->mqtLocate     == NULL)
                        || (mqtInterface->mqtDelete     == NULL)
                        || (mqtInterface->mqtRelease    == NULL)
                        || (mqtInterface->mqtGet        == NULL)
                        || (mqtInterface->mqtPut        == NULL)
                        || (mqtInterface->mqtGetById    == NULL)
#if defined (DDSP_PROFILE)
                        || (mqtInterface->mqtInstrument == NULL)
#endif /* defined (DDSP_PROFILE) */
#if defined (DDSP_DEBUG)
                        || (mqtInterface->mqtDebug      == NULL)
#endif /* defined (DDSP_DEBUG) */
                        || (mqtInterface->mqtGetReplyId == NULL)) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }
#if defined (DDSP_DEBUG)
                    else {
                        status = GEN_Strcpyn (
                             cfgMqtObj.mqtName,
                             LDRV_Obj.mqtObjects [i].mqtName,
                             DSP_MAX_STRLEN) ;
                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }
#endif /* defined (DDSP_DEBUG) */
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

#endif /* if defined (MSGQ_COMPONENT) */

    /*  ------------------------------------------------------------------------
     *  Populate the MMU Table(s)
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        LDRV_Obj.numMmuTables = cfgDrvObj.mmuTables ;

        if (LDRV_Obj.numMmuTables != 0) {
            status = MEM_Calloc ((Void **) &(LDRV_Obj.mmuTables),
                                 LDRV_Obj.numMmuTables * sizeof (DspMmuEntry *),
                                 MEM_DEFAULT) ;

            if (DSP_SUCCEEDED (status)) {
                for (i = 0 ;(   DSP_SUCCEEDED (status)
                             && (i < LDRV_Obj.numDsps)) ; i++) {
                    status = LDRV_CfgGetMmuTable (i) ;
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
            LDRV_Obj.mmuTables = NULL ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    /*  ------------------------------------------------------------------------
     *  Populate the DSP Object(s)
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; DSP_SUCCEEDED (status) && (i < LDRV_Obj.numDsps) ; i++) {

            if (DSP_SUCCEEDED (status)) {
                status = LDRV_InitializeDspObj (i) ;
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_FAILED (status)) {
        LDRV_Finalize () ;
    }

    TRC_1LEAVE ("LDRV_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_Finalize
 *
 *  @desc   Releases resources used by this component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_Finalize ()
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;
    Uint32     i                   ;

    TRC_0ENTER ("LDRV_Finalize") ;

    if (LDRV_Obj.mmuTables != NULL) {
        /*  --------------------------------------------------------------------
         *  Free Memory for MMU Tables
         *  --------------------------------------------------------------------
         */
        for (i = 0 ;
             DSP_SUCCEEDED (status) && (i < LDRV_Obj.numMmuTables) ;
             i++) {
            tmpStatus = FREE_PTR (LDRV_Obj.mmuTables [i]) ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }

        tmpStatus = FREE_PTR (LDRV_Obj.mmuTables) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

#if defined (MSGQ_COMPONENT)
    /*  ------------------------------------------------------------------------
     *  Free Memory for mqaObjects.
     *  ------------------------------------------------------------------------
     */
     if (LDRV_Obj.mqaObjects != NULL ) {
        tmpStatus = FREE_PTR (LDRV_Obj.mqaObjects) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    if (LDRV_Obj.mqtObjects != NULL) {
        /*  ------------------------------------------------------------------------
         *  Free Memory for mqtObjects.
         *  ------------------------------------------------------------------------
         */
        tmpStatus = FREE_PTR (LDRV_Obj.mqtObjects) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }
#endif /* defined (MSGQ_COMPONENT) */

    if (LDRV_Obj.linkTables != NULL) {
        /*  --------------------------------------------------------------------
         *  Free Memory for Link Tables
         *  --------------------------------------------------------------------
         */
        for (i = 0 ;
             DSP_SUCCEEDED (status) && (i < LDRV_Obj.numLinkTables) ;
             i++) {
            tmpStatus = FREE_PTR (LDRV_Obj.linkTables [i]) ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }

        tmpStatus = FREE_PTR (LDRV_Obj.linkTables) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Finalize DSP objects
     *  ------------------------------------------------------------------------
     */
    for (i = 0 ; i < LDRV_Obj.numDsps ; i++) {
        tmpStatus = LDRV_FinalizeDspObj (i) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Free Memory for DSP Objects
     *  ------------------------------------------------------------------------
     */
    tmpStatus = FREE_PTR (LDRV_Obj.dspObjects) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_Finalize", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   LDRV_InitializeDspObj
 *
 *  @desc   Initializes the specified DSP object.
 *          It imports attributes of the DSP object from the configuration
 *          database for initialization.
 *
 *  @modif  LDRV_Obj.dspObjects [dspId]
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_InitializeDspObj (ProcessorId dspId)
{
    DSP_STATUS      status    = DSP_SOK ;
    DspInterface *  dspIntf   = NULL    ;
    DspObject *     dspObj    = NULL    ;
    CFG_Dsp         cfgDspObj           ;

#if defined (CHNL_COMPONENT)
    LinkInterface * linkIntf  = NULL    ;
    Uint32          i                   ;
#endif /* if defined (CHNL_COMPONENT) */

#if defined (DDSP_PROFILE)
    ProcInstrument * procInst = NULL ;
#if defined (CHNL_COMPONENT)
    ChnlInstrument * chnlInst = NULL ;
    Uint32           j               ;
#endif /* if defined (CHNL_COMPONENT) */
#endif /* if defined (DDSP_PROFILE) */

    TRC_0ENTER ("LDRV_InitializeDspObj") ;

    dspObj = &(LDRV_Obj.dspObjects [dspId]) ;

    status = CFG_GetRecord (CFG_DSP_OBJECT, dspId, &cfgDspObj) ;

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Copy attributes of DSP from configuration data
         *  --------------------------------------------------------------------
         */
#if defined (CHNL_COMPONENT)
        dspObj->linkTable     = LDRV_Obj.linkTables [cfgDspObj.linkTable] ;
        dspObj->numLinks      = cfgDspObj.linkTableSize ;
#endif /* defined (CHNL_COMPONENT) */

        dspObj->dspArch         = (DspArch) cfgDspObj.dspArch   ;
        dspObj->loaderInterface = (LoaderInterface  * )
                                  (cfgDspObj.loaderInterface) ;
        dspObj->autoStart       = cfgDspObj.autoStart ;
        dspObj->resetVector     = cfgDspObj.resetVector ;
        dspObj->wordSize        = cfgDspObj.wordSize  ;
        dspObj->endian          = cfgDspObj.endian    ;
        dspObj->interface       = (DspInterface *) (cfgDspObj.interface) ;
#if defined (MSGQ_COMPONENT)
        dspObj->mqtId         = cfgDspObj.mqtId ;
#endif /* defined (MSGQ_COMPONENT) */
        dspObj->mmuFlag         = (Bool) (  (cfgDspObj.mmuFlag == 1)
                                        ? TRUE : FALSE) ;
        if (dspObj->mmuFlag == TRUE) {
            dspObj->mmuTable = LDRV_Obj.mmuTables [cfgDspObj.mmuTable] ;
            dspObj->numMmuEntries = cfgDspObj.mmuTableSize ;

            status = MEM_Calloc ((Void **) &(dspObj->addrMapInGpp),
                                 cfgDspObj.mmuTableSize * sizeof (Uint32),
                                 MEM_DEFAULT) ;
        }
        else {
            dspObj->addrMapInGpp = NULL ;
        }


        if (DSP_SUCCEEDED (status)) {
            status = GEN_Strcpyn (dspObj->dspName,
                                  cfgDspObj.dspName,
                                  DSP_MAX_STRLEN) ;

            if (DSP_SUCCEEDED (status)) {
                status = GEN_Strcpyn (dspObj->execName,
                                      cfgDspObj.execName,
                                      DSP_MAX_STRLEN) ;

                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
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
        /*  --------------------------------------------------------------------
         *  Initialize data structures for profiling
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            procInst = &(LDRV_Obj.procStats.procData [dspId]) ;

            procInst->dataToDsp   = 0 ;
            procInst->dataFromDsp = 0 ;
            procInst->intsToDsp   = 0 ;
            procInst->intsFromDsp = 0 ;
            procInst->activeLinks = 0 ;

#if defined (CHNL_COMPONENT)
            for (j = 0 ; j < MAX_CHANNELS ; j++) {
                chnlInst = &(LDRV_Obj.chnlStats.chnlData [dspId][j]) ;

                chnlInst->chnlId        = j ;
                chnlInst->transferred   = 0 ;
                chnlInst->numBufsQueued = 0 ;
#if defined (DDSP_PROFILE_DETAILED)
                chnlInst->archIndex     = 0 ;
#endif
            }
#endif /* defined (CHNL_COMPONENT) */

            /*  ----------------------------------------------------------------
             *  Allocate memory for profiling statistics of DSP.
             *  ----------------------------------------------------------------
             */
            status = MEM_Alloc ((Void **) &(dspObj->dspStats),
                                sizeof (DspStats),
                                MEM_DEFAULT) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
#endif

        /*  --------------------------------------------------------------------
         *  Initialize links configured for the DSP
         *  --------------------------------------------------------------------
         */
#if defined (CHNL_COMPONENT)
        if (DSP_SUCCEEDED (status)) {
            for (i = 0 ;
                 (i < dspObj->numLinks) && (DSP_SUCCEEDED (status)) ;
                 i++) {
                linkIntf = dspObj->linkTable [i].interface ;
                if (   (linkIntf->initialize  == NULL)
                    || (linkIntf->finalize    == NULL)
                    || (linkIntf->ioRequest   == NULL)
                    || (linkIntf->scheduleDpc == NULL)) {

                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
        }
#endif /* if defined (CHNL_COMPONENT) */

        /*  --------------------------------------------------------------------
         *  Validate DSP Interface
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            dspIntf = dspObj->interface ;

            if (   (dspIntf->setup          == NULL)
                || (dspIntf->initialize     == NULL)
                || (dspIntf->finalize       == NULL)
                || (dspIntf->start          == NULL)
                || (dspIntf->stop           == NULL)
                || (dspIntf->idle           == NULL)
                || (dspIntf->interrupt      == NULL)
                || (dspIntf->clearInterrupt == NULL)
                || (dspIntf->read           == NULL)
                || (dspIntf->write          == NULL)) {

                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_InitializeDspObj", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   LDRV_FinalizeDspObj
 *
 *  @desc   Finalize DSP object.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_FinalizeDspObj (ProcessorId dspId)
{
    DSP_STATUS      status    = DSP_SOK ;
    DSP_STATUS      tmpStatus = DSP_SOK ;
    DspObject *     dspObj    = NULL    ;

    TRC_0ENTER ("LDRV_FinalizeDspObj") ;

    dspObj = &(LDRV_Obj.dspObjects [dspId]) ;

    if (dspObj->addrMapInGpp != NULL) {
        tmpStatus = FREE_PTR (dspObj->addrMapInGpp) ;

        if (DSP_FAILED (tmpStatus)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

#if defined (DDSP_PROFILE)
    tmpStatus = FREE_PTR (dspObj->dspStats) ;

    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("LDRV_FinalizeDspObj", status) ;

    return status ;
}



#if defined (CHNL_COMPONENT)
/*  ----------------------------------------------------------------------------
 *  @func   LDRV_CfgGetLinkTable
 *
 *  @desc   Reads the content of linktable associated with specified DSP
 *          from the configuration database.
 *
 *  @modif  LDRV_Obj.linkTables [linkTableId]
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_CfgGetLinkTable (ProcessorId dspId)
{
    DSP_STATUS  status        = DSP_SOK ;
    LinkAttrs * linkTable     = NULL    ;
    Uint32      linkTableId   ;
    Uint32      linkTableSize ;

    TRC_0ENTER ("LDRV_CfgGetLinkTable") ;

    /*  ------------------------------------------------------------------------
     *  Get the Link Table associated with the DSP
     *  ------------------------------------------------------------------------
     */
    status = CFG_GetNumValue (CFG_DSP_LINKTABLE, dspId, &linkTableId) ;

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Check if the link table isn't already populated.
         *  (A Link table may be shared by many DSPs)
         *  --------------------------------------------------------------------
         */
        if (LDRV_Obj.linkTables [linkTableId] == NULL) {
            /*  ----------------------------------------------------------------
             *  Get the size of Link Table & allocate memory to load it.
             *  ----------------------------------------------------------------
             */
            status = CFG_GetNumValue (CFG_DSP_LINKTABLESIZE,
                                      dspId,
                                      &linkTableSize) ;

            if (DSP_SUCCEEDED (status)) {
                status = MEM_Calloc ((Void **) &linkTable,
                                     linkTableSize * sizeof (LinkAttrs),
                                     MEM_DEFAULT) ;
            }
            else {
                SET_FAILURE_REASON ;
            }

            /*  ----------------------------------------------------------------
             *  Read Link Table into the allocated memory.
             *  ----------------------------------------------------------------
             */
            if (DSP_SUCCEEDED (status)) {
                status = CFG_GetRecord (CFG_LINK_TABLE,
                                        dspId,
                                        linkTable) ;

                if (DSP_SUCCEEDED (status)) {
                    LDRV_Obj.linkTables [linkTableId] = linkTable ;
                }
                else {
                    SET_FAILURE_REASON ;

                    FREE_PTR (linkTable) ;
                }
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CfgGetLinkTable", status) ;

    return status ;
}
#endif /* if defined (CHNL_COMPONENT) */


/*  ----------------------------------------------------------------------------
 *  @func   LDRV_CfgGetMmuTable
 *
 *  @desc   Reads the content of MMU table associated with specified DSP
 *          from the configuration database.
 *
 *  @modif  LDRV_Obj.mmuTables [mmuTableId]
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LDRV_CfgGetMmuTable (ProcessorId dspId)
{
    DSP_STATUS    status   = DSP_SOK ;
    DspMmuEntry * mmuTable = NULL    ;

    Uint32        mmuTableId   ;
    Uint32        mmuTableSize ;

    TRC_0ENTER ("LDRV_CfgGetMmuTable") ;

    /*  ------------------------------------------------------------------------
     *  Get the MMU Table associated with the DSP
     *  ------------------------------------------------------------------------
     */
    status = CFG_GetNumValue (CFG_DSP_MMUTABLE, dspId, &mmuTableId) ;

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Check if the MMU table isn't already populated.
         *  (A MMU table may be shared by many DSPs)
         *  --------------------------------------------------------------------
         */
        if (LDRV_Obj.mmuTables [mmuTableId] == NULL) {
            /*  ----------------------------------------------------------------
             *  Get the size of MMU Table & allocate memory to load it.
             *  ----------------------------------------------------------------
             */
            status = CFG_GetNumValue (CFG_DSP_MMUTABLESIZE,
                                      dspId,
                                      &mmuTableSize) ;

            if (DSP_SUCCEEDED (status)) {
                status = MEM_Calloc ((Void **) &mmuTable,
                                     mmuTableSize * sizeof (DspMmuEntry),
                                     MEM_DEFAULT) ;
            }
            else {
                SET_FAILURE_REASON ;
            }

            /*  ----------------------------------------------------------------
             *  Read MMU Table into the allocated memory.
             *  ----------------------------------------------------------------
             */
            if (DSP_SUCCEEDED (status)) {
                status = CFG_GetRecord (CFG_MMU_TABLE,
                                        dspId,
                                        mmuTable) ;

                if (DSP_SUCCEEDED (status)) {
                    LDRV_Obj.mmuTables [mmuTableId] = mmuTable ;
                }
                else {
                    SET_FAILURE_REASON ;

                    FREE_PTR (mmuTable) ;
                }
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_CfgGetMmuTable", status) ;

    return status ;
}

#if defined (__cplusplus)
}
#endif
