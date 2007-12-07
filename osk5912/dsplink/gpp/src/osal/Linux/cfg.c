/** ============================================================================
 *  @file   cfg.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implements CFG module functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug                 */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Header                   */
#include <cfgdefs.h>
#include <cfg.h>
#include <print.h>

/*  ----------------------------------- Generic Function              */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_OSAL_CFG

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  GEN_SetReason (status, FID_C_OSAL_CFG, __LINE__)

/*  ============================================================================
 *  Extern declarations
 *
 *  Configuration structures derived from user specified configuration
 *  ============================================================================
 */
EXTERN CFG_Driver   CFG_DbDriver          ;
EXTERN CFG_Gpp      CFG_DbGpp             ;
EXTERN CFG_Dsp      CFG_DbDsp_00          ;
EXTERN CFG_MmuEntry CFG_DbMmuTable_00 []  ;

#if defined (CHNL_COMPONENT)
EXTERN CFG_Link     CFG_DbLinkTable_00 [] ;
#endif

#if defined (MSGQ_COMPONENT)
EXTERN CFG_Mqa      CFG_DbMqa00 ;
EXTERN CFG_Mqt      CFG_DbMqt00 ;
EXTERN CFG_Mqt      CFG_DbMqt01 ;
#endif /* if defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @name   CFG_IsInitialized
 *
 *  @desc   Flag to keep track of initialization of this subcomponent.
 *  ============================================================================
 */
STATIC Bool CFG_IsInitialized = FALSE ;


/** ============================================================================
 *  @func   CFG_Initialize
 *
 *  @desc   This function initializes this sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_Initialize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("CFG_Initialize") ;

    DBC_Require (CFG_IsInitialized == FALSE) ;

    CFG_IsInitialized = TRUE ;

    TRC_1LEAVE ("CFG_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CFG_Finalize
 *
 *  @desc   This function provides an interface to exit from this
 *          sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_Finalize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("CFG_Finalize") ;

    DBC_Require (CFG_IsInitialized == TRUE) ;

    CFG_IsInitialized = FALSE ;

    TRC_1LEAVE ("CFG_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CFG_GetRecord
 *
 *  @desc   Gets the record from the configuration.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_GetRecord (IN Uint32 key, IN Uint32  id, OUT Void * record)
{
    DSP_STATUS      status = DSP_SOK ;
    CFG_Driver *    drvObj = NULL ;
    CFG_Gpp *       gppObj = NULL ;
    CFG_Dsp *       dspObj = NULL ;
    CFG_MmuEntry *  mmuObj = NULL ;
    CFG_MmuEntry *  mmuTmp = NULL ;

#if defined (CHNL_COMPONENT)
    CFG_Link *      lnkObj = NULL ;
    CFG_Link *      lnkTmp = NULL ;
#endif /* if defined (CHNL_COMPONENT) */

#if defined (MSGQ_COMPONENT)
    CFG_Mqa *       mqaObj = NULL ;
    CFG_Mqt *       mqtObj = NULL ;
#endif /* if defined (MSGQ_COMPONENT) */

    Uint32          i      ;
    Uint32          tabID  ;
    Uint32          tabLen ;

    TRC_3ENTER ("CFG_GetRecord", key, id, record) ;

    DBC_Require (CFG_IsInitialized == TRUE) ;
    DBC_Require (record != NULL) ;

    if (record == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        switch (key) {
        case CFG_DRIVER_OBJECT:
            /*  ----------------------------------------------------------------
             *  The driver object
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *((CFG_Driver *) record) = *drvObj ;
            }
            break ;

#if defined (MSGQ_COMPONENT)
        case CFG_MQA_OBJECT:
            /*  ----------------------------------------------------------------
             *  The driver object
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                mqaObj = &CFG_DbMqa00 ;
            }

            if (mqaObj== NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *((CFG_Mqa *) record) = *mqaObj ;
            }
            break ;
        case CFG_MQT_OBJECT:
            /*  ----------------------------------------------------------------
             *  The driver object
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                mqtObj = &CFG_DbMqt00 ;
            }
            if (id == 1) {
                mqtObj = &CFG_DbMqt01 ;
            }

            if (mqtObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *((CFG_Mqt *) record) = *mqtObj ;
            }
            break ;
#endif /* if defined (MSGQ_COMPONENT) */
        case CFG_GPP_OBJECT:
            /*  ----------------------------------------------------------------
             *  The GPP object
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                gppObj = &CFG_DbGpp ;
            }

            if (gppObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *((CFG_Gpp *) record) = *gppObj ;
            }
            break ;

        case CFG_DSP_OBJECT:
            /*  ----------------------------------------------------------------
             *  The DSP object
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *((CFG_Dsp *) record) = *dspObj ;
            }
            break ;

#if defined (CHNL_COMPONENT)
        case CFG_LINK_TABLE:
            /*  ----------------------------------------------------------------
             *  The Link driver object
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                tabID  = dspObj->linkTable ;
                if (tabID == 0) {
                    lnkObj = &CFG_DbLinkTable_00 [0] ;
                }

                if (lnkObj == NULL) {
                    status = DSP_EFAIL ;
                }
                else {
                    tabLen = dspObj->linkTableSize ;
                    lnkTmp = (CFG_Link *) record ;

                    for (i = 0 ; i < tabLen ; i++) {
                        *(lnkTmp++) = *(lnkObj++) ;
                    }
                }
            }
            break ;
#endif

        case CFG_MMU_TABLE:
            /*  ----------------------------------------------------------------
             *  The MMU table
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                tabID  = dspObj->mmuTable ;
                if (tabID == 0) {
                    mmuObj = &CFG_DbMmuTable_00 [0] ;
                }

                if (mmuObj == NULL) {
                    status = DSP_EFAIL ;
                }
                else {
                    tabLen = dspObj->mmuTableSize ;
                    mmuTmp = (CFG_MmuEntry *) record ;

                    for (i = 0 ; i < tabLen ; i++) {
                        *(mmuTmp++) = *(mmuObj++) ;
                    }
                }
            }
            break ;

        default:
            /*  ----------------------------------------------------------------
             *  Invalid key
             *  ----------------------------------------------------------------
             */
            status = DSP_EINVALIDARG ;
            break ;
        }
    }

    TRC_1LEAVE ("CFG_GetRecord", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CFG_GetNumValue
 *
 *  @desc   Gets the numeric value type of configuration.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_GetNumValue (IN Uint32 key, IN Uint32 id, OUT Uint32 * value)
{
    DSP_STATUS      status = DSP_SOK ;
    CFG_Driver *    drvObj = NULL ;
    CFG_Gpp *       gppObj = NULL ;
    CFG_Dsp *       dspObj = NULL ;

    TRC_3ENTER ("CFG_GetNumValue", key, id, value) ;

    DBC_Require (CFG_IsInitialized == TRUE) ;
    DBC_Require (value != NULL) ;

    if (value == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        switch (key) {
        case CFG_DRIVER_COMPONENTS:
            /*  ----------------------------------------------------------------
             *  Number of components in the driver
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = drvObj->components ;
            }
            break ;

#if defined (MSGQ_COMPONENT)
        case CFG_DSP_MQTID:
            /*  ----------------------------------------------------------------
             *  Number of components in the driver
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->mqtId ;
            }
            break ;
        case CFG_DRIVER_NUMMQTS:
            /*  ----------------------------------------------------------------
             *  Number of components in the driver
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = drvObj->numMqts ;
            }
            break ;
        case CFG_DRIVER_NUMMQAS:
            /*  ----------------------------------------------------------------
             *  Number of components in the driver
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = drvObj->numMqas ;
            }
            break ;
#endif /* if defined (MSGQ_COMPONENT) */
        case CFG_DRIVER_QUEUE:
            /*  ----------------------------------------------------------------
             *  Queue length supported by driver
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = drvObj->queueLength ;
            }
            break ;

        case CFG_DRIVER_LINKTABLES:
            /*  ----------------------------------------------------------------
             *  Number of link tables in "this" configuration.
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = drvObj->linkTables ;
            }
            break ;

        case CFG_DRIVER_MMUTABLES:
            /*  ----------------------------------------------------------------
             *  Number of MMU tables in "this" configuration.
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = drvObj->mmuTables ;
            }
            break ;

        case CFG_GPP_NUMDSPS:
            /*  ----------------------------------------------------------------
             *  Number of DSPs attached to the GPP.
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                gppObj = &CFG_DbGpp ;
            }

            if (gppObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = gppObj->numDsps ;
            }
            break ;

        case CFG_DSP_LINKTABLE:
            /*  ----------------------------------------------------------------
             *  ID of the link table corresponding to a DSP
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->linkTable ;
            }
            break ;

        case CFG_DSP_LINKTABLESIZE:
            /*  ----------------------------------------------------------------
             *  Size of the link table corresponding to a DSP
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->linkTableSize ;
            }
            break ;

        case CFG_DSP_AUTOSTART:
            /*  ----------------------------------------------------------------
             *  Auto-start flag
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->autoStart ;
            }
            break ;

        case CFG_DSP_WORDSIZE:
            /*  ----------------------------------------------------------------
             *  Size of word on DSP
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->wordSize ;
            }
            break ;

        case CFG_DSP_ENDIANISM:
            /*  ----------------------------------------------------------------
             *  Endianism of DSP
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->endian ;
            }
            break ;

        case CFG_DSP_MMUFLAG:
            /*  ----------------------------------------------------------------
             *  Is DSP MMU used?
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->mmuFlag ;
            }
            break ;

        case CFG_DSP_MMUTABLE:
            /*  ----------------------------------------------------------------
             *  ID of MMU table used by the DSP
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->mmuTable ;
            }
            break ;

        case CFG_DSP_MMUTABLESIZE:
            /*  ----------------------------------------------------------------
             *  Size of MMU table used by the DSP
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                *value = dspObj->mmuTableSize ;
            }
            break ;

        default:
            /*  ----------------------------------------------------------------
             *  Invalid key
             *  ----------------------------------------------------------------
             */
            status = DSP_EINVALIDARG ;
            break ;
        }
    }

    TRC_1LEAVE ("CFG_GetNumValue", status) ;

    return status ;
}


/** ============================================================================
 *  @func   CFG_GetStrValue
 *
 *  @desc   Gets the string value type of configuration.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CFG_GetStrValue (IN Uint32 key, IN Uint32 id, OUT Pstr string)
{
    DSP_STATUS      status = DSP_SOK ;
    CFG_Driver *    drvObj = NULL ;
    CFG_Gpp *       gppObj = NULL ;
    CFG_Dsp *       dspObj = NULL ;

    TRC_3ENTER ("CFG_GetStrValue", key, id, string) ;

    DBC_Require (CFG_IsInitialized == TRUE) ;
    DBC_Require (string != NULL) ;

    if (string == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        switch (key) {
        case CFG_DRIVER_NAME:
            /*  ----------------------------------------------------------------
             *  Fetch the driver name.
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                drvObj = &CFG_DbDriver ;
            }

            if (drvObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                GEN_Strcpyn (string, drvObj->driverName, CFG_MAX_STRLEN) ;
            }
            break ;

        case CFG_GPP_NAME:
            /*  ----------------------------------------------------------------
             *  Fetch the GPP name.
             *  ----------------------------------------------------------------
             */
            if (id == CFG_ID_NONE) {
                gppObj = &CFG_DbGpp ;
            }

            if (gppObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                GEN_Strcpyn (string, gppObj->gppName, CFG_MAX_STRLEN) ;
            }
            break ;

        case CFG_DSP_NAME:
            /*  ----------------------------------------------------------------
             *  Fetch the DSP name.
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                GEN_Strcpyn (string, dspObj->dspName, CFG_MAX_STRLEN) ;
            }
            break ;

        case CFG_DSP_EXECUTABLE:
            /*  ----------------------------------------------------------------
             *  Fetch the dsp executable binary file name.
             *  ----------------------------------------------------------------
             */
            if (id == 0) {
                dspObj = &CFG_DbDsp_00 ;
            }

            if (dspObj == NULL) {
                status = DSP_EINVALIDARG ;
            }
            else {
                GEN_Strcpyn (string, dspObj->execName, CFG_MAX_STRLEN) ;
            }
            break ;

        default:
            /*  ----------------------------------------------------------------
             *  Invalid key
             *  ----------------------------------------------------------------
             */
            status = DSP_EINVALIDARG ;
            break ;
        }
    }

    TRC_1LEAVE ("CFG_GetStrValue", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
