/** ============================================================================
 *  @file   ldrv_msgq.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implements the LDRV MSGQ driver.
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
#include <intobject.h>
#include <errbase.h>
#include <safe.h>

#include <dspdefs.h>
#include <linkdefs.h>
#include <msgqdefs.h>

/*  ----------------------------------- Trace & Debug                 */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- OSAL Header                   */
#include <mem.h>
#include <sync.h>
#include <print.h>

/*  ----------------------------------- Generic Function              */
#include <gen_utils.h>

/*  ----------------------------------- Link Driver                   */
#include <ldrv_mqt.h>
#include <ldrv_mqa.h>
#include <ldrv_msgq.h>
#include <ldrv.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent ID.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_MSGQ


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_MSGQ, __LINE__)


/** ----------------------------------------------------------------------------
 *  @name   LdrvMsgqStateObj
 *
 *  @desc   This is the global state object for LDRV MSGQ component.
 *  ----------------------------------------------------------------------------
 */
STATIC LdrvMsgqState LdrvMsgqStateObj ;

/** ============================================================================
 *  @name   LDRV_Obj
 *
 *  @desc   Global defined in ldrv.c.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;

/*  ----------------------------------------------------------------------------
 *  @name   LDRV_MSGQ_IsInitialized
 *
 *  @desc   Flag for tracking initialization of component.
 *  ----------------------------------------------------------------------------
 */
STATIC Bool   LDRV_MSGQ_IsInitialized = FALSE ;


#if defined (DDSP_PROFILE)
/** ----------------------------------------------------------------------------
 *  @func   LDRV_MSGQ_ResetProfile
 *
 *  @desc   This function resets the profile statistics of the specified message
 *          queue.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    msgqId
 *              Message queue identifier.
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
STATIC
NORMAL_API
Void
LDRV_MSGQ_ResetProfile (IN  ProcessorId procId, IN  MsgQueueId msgqId) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ----------------------------------------------------------------------------
 *  @func   LDRV_NumToStr
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @arg    strNum
 *              Converted string.
 *  @arg    num
 *              1 to 2 digit number to be converted into a string.
 *
 *  @ret    None
 *
 *  @enter  strNum must not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
LDRV_NumToStr (Pstr strNum, Uint16 num) ;
#endif /* defined (DDSP_DEBUG) */


/** ============================================================================
 *  @func   LDRV_MSGQ_Setup
 *
 *  @desc   This function initializes the MSGQ component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Setup ()
{
    DSP_STATUS     status        = DSP_SOK ;
    Uint16         i             = 0       ;
    Uint16         j             = 0       ;
    MqaInterface * mqaInterface  = NULL    ;
    MqtInterface * mqtInterface  = NULL    ;

    TRC_0ENTER ("LDRV_MSGQ_Setup") ;

    DBC_Require (LDRV_MSGQ_IsInitialized == FALSE) ;

    /*  Initialize the allocators and transports fields.  */
    LdrvMsgqStateObj.allocators = NULL ;
    LdrvMsgqStateObj.transports = NULL ;

    /*  ------------------------------------------------------------------------
     *  Initialize the global state variable.
     *  ------------------------------------------------------------------------
     */
    LdrvMsgqStateObj.numAllocators    = (Uint16) LDRV_Obj.numMqas  ;
    LdrvMsgqStateObj.numTransports    = (Uint16) LDRV_Obj.numMqts  ;
    LdrvMsgqStateObj.localTransportId = (Uint16) LDRV_Obj.localMqt ;
    LdrvMsgqStateObj.errorHandlerMsgq = MSGQ_INVALID_ID ;
    LdrvMsgqStateObj.errorMqaId       = MSGQ_INVALID_ID ;

    /*  --------------------------------------------------------------------
     *  For each dsp get the id of the MQT being used.
     *  --------------------------------------------------------------------
     */
    for (i = 0 ; (   (i < MAX_PROCESSORS)
                  && (DSP_SUCCEEDED (status))) ; i++) {
        LdrvMsgqStateObj.mqtMap [i] = LDRV_Obj.dspObjects [i].mqtId ;
    }

    /*  ------------------------------------------------------------------------
     *  Allocate memory for the allocator array and fill its fields from
     *  LDRV_Obj.
     *  ------------------------------------------------------------------------
     */
    status = MEM_Alloc ((Void **) &(LdrvMsgqStateObj.allocators),
                        (  LdrvMsgqStateObj.numAllocators
                         * sizeof (LdrvMsgqAllocatorObj)),
                        MEM_DEFAULT) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    for (i = 0 ; (   (i < LdrvMsgqStateObj.numAllocators)
                  && (DSP_SUCCEEDED (status))) ; i++) {
        DBC_Assert (LDRV_Obj.mqaObjects != NULL) ;
        mqaInterface = LDRV_Obj.mqaObjects [i].interface ;

        DBC_Assert (mqaInterface != NULL) ;

        LdrvMsgqStateObj.allocators [i].mqaId = i ;
        LdrvMsgqStateObj.allocators [i].mqaInfo = NULL ;
        LdrvMsgqStateObj.allocators [i].mqaInterface = mqaInterface ;
#if defined (DDSP_DEBUG)
        status = GEN_Strcpyn (
             LDRV_Obj.mqaObjects [i].mqaName,
             LdrvMsgqStateObj.allocators [i].mqaName,
             DSP_MAX_STRLEN) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
#endif /* defined (DDSP_DEBUG) */

        if (DSP_SUCCEEDED (status)) {
            mqaInterface->mqaInitialize () ;
        }
        else {
            for (j = 0 ; j < i ; j++) {
                LdrvMsgqStateObj.allocators [j].mqaInterface->mqaFinalize () ;
            }
        }
    }

    /*  ------------------------------------------------------------------------
     *  Allocate memory for the transport array and fill its fields.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = MEM_Alloc ((Void **) &(LdrvMsgqStateObj.transports),
                            (  LdrvMsgqStateObj.numTransports
                             * sizeof (LdrvMsgqTransportObj)),
                            MEM_DEFAULT) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    for (i = 0 ; (   (i < LdrvMsgqStateObj.numTransports)
                  && (DSP_SUCCEEDED (status))) ; i++) {
        DBC_Assert (LDRV_Obj.mqtObjects != NULL) ;
        mqtInterface = LDRV_Obj.mqtObjects [i].interface ;

        DBC_Assert (mqtInterface != NULL) ;

        LdrvMsgqStateObj.transports [i].mqtId = i ;
        LdrvMsgqStateObj.transports [i].mqtInfo = NULL ;
        LdrvMsgqStateObj.transports [i].mqtInterface = mqtInterface ;
#if defined (DDSP_DEBUG)
        status = GEN_Strcpyn (
                    LDRV_Obj.mqtObjects [i].mqtName,
                    LdrvMsgqStateObj.transports [i].mqtName,
                    DSP_MAX_STRLEN) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
#endif /* defined (DDSP_DEBUG) */

        if (DSP_SUCCEEDED (status)) {
            mqtInterface->mqtInitialize () ;
        }
        else {
            for (j = 0 ; j < i ; j++) {
                LdrvMsgqStateObj.transports [j].mqtInterface->mqtFinalize () ;
            }
        }
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        /*  Initialize local message instrumentation data  */
        for (j = 0 ; j < MAX_MSGQS ; j++) {
            LDRV_Obj.msgqStats.localMsgqData [j].procId = ID_LOCAL_PROCESSOR ;
            LDRV_Obj.msgqStats.localMsgqData [j].msgqId = j ;
            LDRV_MSGQ_ResetProfile (ID_LOCAL_PROCESSOR, j) ;
        }

        /*  Initialize remote message instrumentation data  */
        for (i = 0 ; i < MAX_PROCESSORS ; i++) {
            for (j = 0 ; j < MAX_MSGQS ; j++) {
                LDRV_Obj.msgqStats.msgqData [i][j].procId = i ;
                LDRV_Obj.msgqStats.msgqData [i][j].msgqId = j ;
                LDRV_MSGQ_ResetProfile (i, j) ;
            }
        }
    }
#endif /* defined (DDSP_PROFILE) */

    if (DSP_FAILED (status)) {
        LDRV_MSGQ_IsInitialized = FALSE ;
        if (LdrvMsgqStateObj.allocators != NULL) {
            FREE_PTR (LdrvMsgqStateObj.allocators) ;
        }
        if (LdrvMsgqStateObj.transports != NULL) {
            FREE_PTR (LdrvMsgqStateObj.transports) ;
        }
    }
    else {
        LDRV_MSGQ_IsInitialized = TRUE ;
    }

    DBC_Ensure (   (   (LDRV_MSGQ_IsInitialized == TRUE)
                    && (DSP_SUCCEEDED (status)))
                || (   (LDRV_MSGQ_IsInitialized == FALSE)
                    && (DSP_FAILED (status)))) ;

    TRC_1LEAVE ("LDRV_MSGQ_Setup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Destroy
 *
 *  @desc   This function finalizes the MSGQ component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Destroy ()
{
    DSP_STATUS     status        = DSP_SOK ;
    DSP_STATUS     tmpStatus     = DSP_SOK ;
    Uint16         i             = 0       ;
    MqaInterface * mqaInterface  = NULL    ;
    MqtInterface * mqtInterface  = NULL    ;

#if defined (DDSP_PROFILE)
    Uint16 j = 0 ;
#endif /* defined (DDSP_PROFILE) */

    TRC_0ENTER ("LDRV_MSGQ_Destroy") ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        /*  Finalize local message instrumentation data  */
        for (j = 0 ; j < MAX_MSGQS ; j++) {
            LDRV_MSGQ_ResetProfile (ID_LOCAL_PROCESSOR, j) ;
        }

        /*  Finalize remote message instrumentation data  */
        for (i = 0 ; i < MAX_PROCESSORS ; i++) {
            for (j = 0 ; j < MAX_MSGQS ; j++) {
                LDRV_MSGQ_ResetProfile (i, j) ;
            }
        }
    }
#endif /* defined (DDSP_PROFILE) */

    /*  ------------------------------------------------------------------------
     *  Free memory allocated for the allocator.
     *  ------------------------------------------------------------------------
     */
    if (LdrvMsgqStateObj.allocators != NULL) {
        for (i = 0 ; i < LdrvMsgqStateObj.numAllocators ; i++) {
            mqaInterface = LdrvMsgqStateObj.allocators [i].mqaInterface ;
            if (mqaInterface != NULL) {
                mqaInterface->mqaFinalize () ;
            }
        }
        status = FREE_PTR (LdrvMsgqStateObj.allocators) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Free memory allocated for the transport.
     *  ------------------------------------------------------------------------
     */
    if (LdrvMsgqStateObj.transports != NULL) {
        for (i = 0 ; i < LdrvMsgqStateObj.numTransports ; i++) {
            mqtInterface = LdrvMsgqStateObj.transports [i].mqtInterface ;
            if (mqtInterface != NULL) {
                mqtInterface->mqtFinalize () ;
            }
        }
        tmpStatus = FREE_PTR (LdrvMsgqStateObj.transports) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    LDRV_MSGQ_IsInitialized = FALSE ;

    DBC_Ensure (LDRV_MSGQ_IsInitialized == FALSE) ;

    TRC_1LEAVE ("LDRV_MSGQ_Destroy", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_AllocatorOpen
 *
 *  @desc   This function initializes the MQA component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_AllocatorOpen (IN  AllocatorId mqaId,
                         IN  Pvoid       mqaAttrs,
                         OUT Pvoid *     mqaInfo)
{
    DSP_STATUS              status        = DSP_SOK ;
    MqaInterface *          mqaInterface  = NULL    ;
    LdrvMsgqAllocatorHandle mqaHandle     = NULL    ;

    TRC_3ENTER ("LDRV_MSGQ_AllocatorOpen", mqaId, mqaAttrs, mqaInfo) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (mqaAttrs != NULL) ;
    DBC_Require (mqaInfo != NULL) ;
    DBC_Require (mqaId < LdrvMsgqStateObj.numAllocators) ;

    if (mqaId >= LdrvMsgqStateObj.numAllocators) {
        status = DSP_EMEMORY ;
        SET_FAILURE_REASON ;
    }
    else {
        mqaHandle    = &(LdrvMsgqStateObj.allocators [mqaId]) ;
        mqaInterface = mqaHandle->mqaInterface ;

        DBC_Assert (mqaInterface != NULL) ;

        /*  --------------------------------------------------------------------
         *  Call the open function for the corresponding allocator.
         *  --------------------------------------------------------------------
         */
        status = mqaInterface->mqaOpen (mqaHandle, mqaAttrs) ;

        *mqaInfo = mqaHandle->mqaInfo ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LDRV_MSGQ_AllocatorOpen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_AllocatorClose
 *
 *  @desc   This function finalizes the MQA component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_AllocatorClose (IN  AllocatorId mqaId)
{
    DSP_STATUS                status        = DSP_SOK ;
    MqaInterface *            mqaInterface  = NULL    ;
    LdrvMsgqAllocatorHandle   mqaHandle     = NULL    ;

    TRC_1ENTER ("LDRV_MSGQ_AllocatorClose", mqaId) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (mqaId < LdrvMsgqStateObj.numAllocators) ;

    if (mqaId >= LdrvMsgqStateObj.numAllocators) {
        status = DSP_EMEMORY ;
        SET_FAILURE_REASON ;
    }
    else {
        mqaHandle    = &(LdrvMsgqStateObj.allocators [mqaId]) ;
        mqaInterface = mqaHandle->mqaInterface ;

        DBC_Assert (mqaInterface != NULL) ;

        /*  --------------------------------------------------------------------
         *  Call the close function for the corresponding allocator.
         *  --------------------------------------------------------------------
         */
        status = mqaInterface->mqaClose (mqaHandle) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LDRV_MSGQ_AllocatorClose", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_TransportOpen
 *
 *  @desc   This function opens the MQT component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_TransportOpen (IN  TransportId mqtId, IN  Pvoid mqtAttrs)
{
    DSP_STATUS                status        = DSP_SOK ;
    MqtInterface *            mqtInterface  = NULL    ;
    Bool                      found         = FALSE   ;
    ProcessorId               procId        = 0       ;
    Uint16                    i             = 0       ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL    ;

    TRC_2ENTER ("LDRV_MSGQ_TransportOpen", mqtId, mqtAttrs) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (mqtAttrs != NULL) ;
    DBC_Require (mqtId < LdrvMsgqStateObj.numTransports) ;

    mqtHandle    = &(LdrvMsgqStateObj.transports [mqtId]) ;
    mqtInterface = mqtHandle->mqtInterface ;

    DBC_Assert (mqtInterface != NULL) ;

    if (mqtId == LdrvMsgqStateObj.localTransportId) {
        /*  The local transport is to be opened.  */
        procId = ID_LOCAL_PROCESSOR ;
        found = TRUE ;
    }
    else {
        for (i = 0 ; (i < MAX_PROCESSORS) && (found != TRUE) ; i++) {
            if (mqtId == LdrvMsgqStateObj.mqtMap [i]) {
                /*  The remote transport is to be opened.  */
                procId = i ;
                found = TRUE ;
            }
        }
    }

    /*  ------------------------------------------------------------------------
     *  If the mqtId specified was a valid one, call the transport open function
     *  for the corresponding transport.
     *  ------------------------------------------------------------------------
     */
    if (found == TRUE) {
        LdrvMsgqStateObj.transports [mqtId].procId = procId ;
        status = mqtInterface->mqtOpen (mqtHandle, mqtAttrs) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        /*  No valid transport was found for the specified mqtId.  */
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        /*  Initialize message instrumentation data.  */
        for (i = 0 ; i < MAX_MSGQS ; i++) {
            LDRV_MSGQ_ResetProfile (procId, i) ;
        }
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("LDRV_MSGQ_TransportOpen", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_TransportClose
 *
 *  @desc   This function closes the MQT component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_TransportClose (IN  TransportId mqtId)
{
    DSP_STATUS                status        = DSP_SOK ;
    MqtInterface *            mqtInterface  = NULL    ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL    ;

#if defined (DDSP_PROFILE)
    Uint16      i      = 0 ;
#endif /* defined (DDSP_PROFILE) */

    TRC_1ENTER ("LDRV_MSGQ_TransportClose", mqtId) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (mqtId < LdrvMsgqStateObj.numTransports) ;

    mqtHandle    = &(LdrvMsgqStateObj.transports [mqtId]) ;
    mqtInterface = mqtHandle->mqtInterface ;

    DBC_Assert (mqtInterface != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Call the close function for the corresponding transport.
     *  ------------------------------------------------------------------------
     */
    status = mqtInterface->mqtClose (mqtHandle) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    /*  Finalize message instrumentation data.  */
    for (i = 0 ; i < MAX_MSGQS ; i++) {
        LDRV_MSGQ_ResetProfile (mqtHandle->procId, i) ;
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("LDRV_MSGQ_TransportClose", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Create
 *
 *  @desc   This function creates the message queue to be used for receiving
 *          messages, identified through the specified MSGQ ID.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Create (IN MsgQueueId  msgqId, IN OPT MsgqAttrs * msgqAttrs)
{
    DSP_STATUS                status        = DSP_SOK ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL    ;

#if defined (DDSP_PROFILE)
    ProcessorId procId = 0 ;
#endif /* defined (DDSP_PROFILE) */

    TRC_2ENTER ("LDRV_MSGQ_Create", msgqId, msgqAttrs) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    mqtHandle = &(LdrvMsgqStateObj.transports
                    [LdrvMsgqStateObj.localTransportId]) ;

    /*  Local transport should have been opened.  */
    DBC_Assert (mqtHandle->mqtInfo != NULL) ;

    DBC_Assert (mqtHandle->mqtInterface != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Call the create function for the local transport.
     *  ------------------------------------------------------------------------
     */
    status = mqtHandle->mqtInterface->mqtCreate (mqtHandle,
                                                 msgqId,
                                                 msgqAttrs) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        procId = mqtHandle->procId ;
        if (IS_LOCAL_PROCID (procId)) {
            LDRV_Obj.msgqStats.localMsgqData [msgqId].procId =
                                                        ID_LOCAL_PROCESSOR ;
            LDRV_Obj.msgqStats.localMsgqData [msgqId].msgqId = msgqId ;
            LDRV_Obj.msgqStats.localMsgqData [msgqId].isValid = TRUE ;
            LDRV_Obj.msgqStats.localMsgqData [msgqId].transferred = 0 ;
            LDRV_Obj.msgqStats.localMsgqData [msgqId].queued = 0 ;
        }
        else {
            LDRV_Obj.msgqStats.msgqData [procId][msgqId].procId = procId ;
            LDRV_Obj.msgqStats.msgqData [procId][msgqId].msgqId = msgqId ;
            LDRV_Obj.msgqStats.msgqData [procId][msgqId].isValid = TRUE ;
            LDRV_Obj.msgqStats.msgqData [procId][msgqId].transferred = 0 ;
            LDRV_Obj.msgqStats.msgqData [procId][msgqId].queued = 0 ;
        }
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("LDRV_MSGQ_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Locate
 *
 *  @desc   This function verifies the existence and status of the message queue
 *          identified by the specified MSGQ ID, on the specified processor.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Locate (IN  ProcessorId       procId,
                  IN  MsgQueueId        msgqId,
                  IN  MsgqLocateAttrs * attrs)
{
    DSP_STATUS                status        = DSP_SOK         ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL            ;
    TransportId               mqtId         = MSGQ_INVALID_ID ;

    TRC_3ENTER ("LDRV_MSGQ_Locate", procId, msgqId, attrs) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (attrs != NULL) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require ((IS_VALID_PROCID (procId)) || (IS_LOCAL_PROCID (procId))) ;

    if (IS_LOCAL_PROCID (procId)) {
        /*  --------------------------------------------------------------------
         *  Get the mqtHandle for the local transport.
         *  --------------------------------------------------------------------
         */
        mqtHandle = &(LdrvMsgqStateObj.transports
                        [LdrvMsgqStateObj.localTransportId]) ;
    }
    else if (IS_VALID_PROCID (procId)) {
        /*  --------------------------------------------------------------------
         *  Get the mqtHandle for the corresponding transport.
         *  --------------------------------------------------------------------
         */
        mqtId = LdrvMsgqStateObj.mqtMap [procId] ;
        mqtHandle = &(LdrvMsgqStateObj.transports [mqtId]) ;
    }

    /*  The MQT component must be opened.  */
    DBC_Assert (mqtHandle->mqtInfo != NULL) ;
    DBC_Assert (mqtHandle->mqtInterface != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Call the locate function for the corresponding transport.
     *  ------------------------------------------------------------------------
     */
    status = mqtHandle->mqtInterface->mqtLocate (mqtHandle, msgqId, attrs) ;

    if ((DSP_FAILED (status)) && (status != DSP_ENOTFOUND)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_MSGQ_Locate", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Delete
 *
 *  @desc   This function deletes the message queue identified by the specified
 *          MSGQ ID.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Delete (IN  MsgQueueId msgqId)
{
    DSP_STATUS                status        = DSP_SOK ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL    ;

    TRC_1ENTER ("LDRV_MSGQ_Delete", msgqId) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    mqtHandle = &(LdrvMsgqStateObj.transports
                    [LdrvMsgqStateObj.localTransportId]) ;

    /*  Local transport must be opened. */
    DBC_Assert (mqtHandle->mqtInfo != NULL) ;
    DBC_Assert (mqtHandle->mqtInterface != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Call the delete function for the local transport.
     *  ------------------------------------------------------------------------
     */
    status = mqtHandle->mqtInterface->mqtDelete (mqtHandle, msgqId) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        /*  Finalize message instrumentation data.  */
        LDRV_MSGQ_ResetProfile (mqtHandle->procId, msgqId) ;
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("LDRV_MSGQ_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Release
 *
 *  @desc   This function releases the MSGQ located through an earlier call to
 *          locate the MSGQ.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Release (IN  ProcessorId procId, IN  MsgQueueId  msgqId)
{
    DSP_STATUS                status        = DSP_SOK         ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL            ;
    TransportId               mqtId         = MSGQ_INVALID_ID ;

    TRC_2ENTER ("LDRV_MSGQ_Release", procId, msgqId) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;
    DBC_Require ((IS_VALID_PROCID (procId)) || (IS_LOCAL_PROCID (procId))) ;

    if (IS_LOCAL_PROCID (procId)) {
        /*  --------------------------------------------------------------------
         *  Get the mqtHandle for the local transport.
         *  --------------------------------------------------------------------
         */
        mqtHandle = &(LdrvMsgqStateObj.transports
                        [LdrvMsgqStateObj.localTransportId]) ;
    }
    else if (IS_VALID_PROCID (procId)) {
        /*  --------------------------------------------------------------------
         *  Get the mqtHandle for the remote transport.
         *  --------------------------------------------------------------------
         */
        mqtId = LdrvMsgqStateObj.mqtMap [procId] ;
        mqtHandle = &(LdrvMsgqStateObj.transports [mqtId]) ;
    }

    /*  The MQT component must be opened.  */
    DBC_Assert (mqtHandle->mqtInfo != NULL) ;
    DBC_Assert (mqtHandle->mqtInterface != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Call the release function for the corresponding transport.
     *  ------------------------------------------------------------------------
     */
    status = mqtHandle->mqtInterface->mqtRelease (mqtHandle, msgqId) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_MSGQ_Release", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Alloc
 *
 *  @desc   This function allocates a message, and returns the pointer to the
 *          user.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Alloc (IN  AllocatorId mqaId, IN  Uint16 size, OUT MsgqMsg * msg)
{
    DSP_STATUS                status        = DSP_SOK ;
    MqaInterface *            mqaInterface  = NULL    ;
    LdrvMsgqAllocatorHandle   mqaHandle     = NULL    ;

    TRC_3ENTER ("LDRV_MSGQ_Alloc", mqaId, size, msg) ;

    DBC_Require (msg != NULL) ;
    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;

    if (mqaId >= LdrvMsgqStateObj.numAllocators) {
        status = DSP_EMEMORY ;
        SET_FAILURE_REASON ;
    }
    else {
        mqaHandle    = &(LdrvMsgqStateObj.allocators [mqaId]) ;
        mqaInterface = mqaHandle->mqaInterface ;

        /*  The MQA component must be opened.  */
        DBC_Assert (mqaHandle->mqaInfo != NULL) ;
        DBC_Assert (mqaInterface != NULL) ;

        /*  --------------------------------------------------------------------
         *  Call the alloc function for the corresponding allocator.
         *  --------------------------------------------------------------------
         */
        status = mqaInterface->mqaAlloc (mqaHandle, &size, msg) ;

        if (DSP_SUCCEEDED (status)) {
            /*  ----------------------------------------------------------------
             *  Initialize the fields of the msg returned by the allocator.
             *  ----------------------------------------------------------------
             */
            (*msg)->mqaId  = mqaId ;
            (*msg)->size   = size ;
            (*msg)->msgId  = MSGQ_INVALID_ID ;
            (*msg)->srcId  = MSGQ_INVALID_ID ;
            (*msg)->mqtId  = MSGQ_INVALID_ID ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("LDRV_MSGQ_Alloc", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Free
 *
 *  @desc   This function frees a message.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Free (IN  MsgqMsg msg)
{
    DSP_STATUS                status        = DSP_SOK ;
    MqaInterface *            mqaInterface  = NULL    ;
    LdrvMsgqAllocatorHandle   mqaHandle     = NULL    ;
    AllocatorId               mqaId         = 0       ;

    TRC_1ENTER ("LDRV_MSGQ_Free", msg) ;

    DBC_Require (msg != NULL) ;
    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;

    mqaId = msg->mqaId ;

    mqaHandle = &(LdrvMsgqStateObj.allocators[mqaId]) ;
    mqaInterface = mqaHandle->mqaInterface ;

    /*  The MQA component must be opened.  */
    DBC_Assert (mqaHandle->mqaInfo != NULL) ;
    DBC_Assert (mqaInterface != NULL) ;
    DBC_Assert (mqaId < LdrvMsgqStateObj.numAllocators) ;

    /*  ------------------------------------------------------------------------
     *  Call the free function for the corresponding allocator.
     *  ------------------------------------------------------------------------
     */
    status = mqaInterface->mqaFree (mqaHandle, msg, msg->size) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_MSGQ_Free", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Put
 *
 *  @desc   This function sends a message to the specified MSGQ on the specified
 *          processor.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Put (IN     ProcessorId procId,
               IN     MsgQueueId  destMsgqId,
               IN     MsgqMsg     msg,
               IN OPT Uint16      msgId,
               IN OPT MsgQueueId  srcMsgqId)
{
    DSP_STATUS                status        = DSP_SOK         ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL            ;
    TransportId               mqtId         = MSGQ_INVALID_ID ;

    TRC_5ENTER ("LDRV_MSGQ_Put", procId, destMsgqId, msg, msgId, srcMsgqId) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (IS_VALID_MSGQID (destMsgqId)) ;
    DBC_Require ((IS_VALID_PROCID (procId)) || (IS_LOCAL_PROCID (procId))) ;

    /*  ------------------------------------------------------------------------
     *  Extract the msgId, srcId and dstId from the message.
     *  ------------------------------------------------------------------------
     */
    msg->msgId = msgId ;
    msg->srcId = srcMsgqId ;
    msg->dstId = destMsgqId ;

    if (IS_LOCAL_PROCID (procId)) {
        mqtHandle = &(LdrvMsgqStateObj.transports
                        [LdrvMsgqStateObj.localTransportId]) ;
    }
    else if (IS_VALID_PROCID (procId)) {
        mqtId = LdrvMsgqStateObj.mqtMap [procId] ;
        mqtHandle = &(LdrvMsgqStateObj.transports [mqtId]) ;
        msg->mqtId = mqtId ;
    }

    /*  The MQT component must be opened.  */
    DBC_Assert (mqtHandle->mqtInfo != NULL) ;
    DBC_Assert (mqtHandle->mqtInterface != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Call the put function for the corresponding transport.
     *  ------------------------------------------------------------------------
     */
    status = mqtHandle->mqtInterface->mqtPut (mqtHandle, destMsgqId, msg) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        if (IS_LOCAL_PROCID (procId)) {
            LDRV_Obj.msgqStats.localMsgqData [destMsgqId].queued++ ;
        }
        else {
            LDRV_Obj.msgqStats.msgqData [procId][destMsgqId].queued++ ;
        }
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("LDRV_MSGQ_Put", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_Get
 *
 *  @desc   This function receives a message on the specified MSGQ.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Get (IN  MsgQueueId msgqId, IN  Uint32 timeout, OUT MsgqMsg * msg)
{
    DSP_STATUS                status        = DSP_SOK ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL    ;

    TRC_3ENTER ("LDRV_MSGQ_Get", msgqId, timeout, msg) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    mqtHandle = &(LdrvMsgqStateObj.transports
                    [LdrvMsgqStateObj.localTransportId]) ;

    /*  The local MQT component must be opened.  */
    DBC_Assert (mqtHandle->mqtInfo != NULL) ;
    DBC_Assert (mqtHandle->mqtInterface != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Call the get function for the local transport.
     *  ------------------------------------------------------------------------
     */
    status = mqtHandle->mqtInterface->mqtGet (mqtHandle, msgqId, timeout, msg) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        LDRV_Obj.msgqStats.localMsgqData [msgqId].transferred++ ;
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("LDRV_MSGQ_Get", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_GetReplyId
 *
 *  @desc   This function extracts the MSGQ ID and processor ID to be used for
 *          replying to a received message.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_GetReplyId (IN  MsgqMsg       msg,
                      OUT ProcessorId * procId,
                      OUT MsgQueueId *  msgqId)
{
    DSP_STATUS                status        = DSP_SOK         ;
    TransportId               mqtId         = MSGQ_INVALID_ID ;
    Bool                      found         = FALSE           ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL            ;
    Uint16                    i             = 0               ;

    TRC_3ENTER ("LDRV_MSGQ_GetReplyId", msg, procId, msgqId) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (msg != NULL) ;
    DBC_Require (procId != NULL) ;
    DBC_Require (msgqId != NULL) ;

    /*  ------------------------------------------------------------------------
     *  Extract the mqt id of the transport from which the message was
     *  delivered.
     *  ------------------------------------------------------------------------
     */
    mqtId = msg->mqtId ;

    if (mqtId < LdrvMsgqStateObj.numTransports) {
        for (i = 0 ; (i < MAX_PROCESSORS) && (found != TRUE) ; i++) {
            if (mqtId == LdrvMsgqStateObj.mqtMap [i]) {
                *procId = i ;
                found = TRUE ;
            }
        }

        /*  --------------------------------------------------------------------
         *  If a valid mqtId was there call the getreplyid for the corresponding
         *  transport.
         *  --------------------------------------------------------------------
         */
        if (found == TRUE) {
            mqtHandle = &(LdrvMsgqStateObj.transports [mqtId]) ;

            /*  The MQT component must be opened.  */
            DBC_Assert (mqtHandle->mqtInfo != NULL) ;
            DBC_Assert (mqtHandle->mqtInterface != NULL) ;

            status = mqtHandle->mqtInterface->mqtGetReplyId (mqtHandle,
                                                             msg,
                                                             msgqId) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_MSGQ_GetReplyId", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_SetErrorHandler
 *
 *  @desc   This function allows the user to designate a MSGQ as an
 *          error-handler MSGQ to receive asynchronous error messages from the
 *          transports.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_SetErrorHandler (IN  MsgQueueId msgqId, IN  Uint16 mqaId)
{
    DSP_STATUS                status        = DSP_SOK ;
    LdrvMsgqAllocatorHandle   mqaHandle     = NULL    ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL    ;

    TRC_2ENTER ("LDRV_MSGQ_SetErrorHandler", msgqId, mqaId) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;
    DBC_Require (IS_VALID_MSGQID (msgqId) || (msgqId == MSGQ_INVALID_ID)) ;
    DBC_Require (   (mqaId < LdrvMsgqStateObj.numAllocators)
                 || (mqaId == MSGQ_INVALID_ID)) ;

    mqtHandle = &(LdrvMsgqStateObj.transports
                    [LdrvMsgqStateObj.localTransportId]) ;

    if (   (msgqId != MSGQ_INVALID_ID)
        && (mqaId < LdrvMsgqStateObj.numAllocators)) {
        /*  The local MQT component must be opened.  */
        DBC_Assert (mqtHandle->mqtInfo != NULL) ;

        mqaHandle = &(LdrvMsgqStateObj.allocators [mqaId])  ;

        DBC_Assert (mqaHandle->mqaInfo != NULL) ;

        /*  --------------------------------------------------------------------
         *  Set the error message queue if the allocator and local transport
         *  specified have been opened.
         *  --------------------------------------------------------------------
         */
        if ((mqtHandle->mqtInfo != NULL) && (mqaHandle->mqaInfo != NULL)) {
            LdrvMsgqStateObj.errorHandlerMsgq = msgqId ;
            LdrvMsgqStateObj.errorMqaId = mqaId ;
        }
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }
    else if (msgqId == MSGQ_INVALID_ID) {
        /*  --------------------------------------------------------------------
         *  Unset the error handler.
         *  --------------------------------------------------------------------
         */
        LdrvMsgqStateObj.errorHandlerMsgq = msgqId ;
        LdrvMsgqStateObj.errorMqaId = mqaId ;
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_MSGQ_SetErrorHandler", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_SendErrorMsg
 *
 *  @desc   This function sends an asynchronous error message of a particular
 *          type to the user-defined error handler MSGQ.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_SendErrorMsg (IN  MsgqErrorType errorType,
                        IN  Pvoid         arg1,
                        IN  Pvoid         arg2,
                        IN  Pvoid         arg3)
{
    DSP_STATUS                status        = DSP_SOK ;
    LdrvMsgqTransportHandle   mqtHandle     = NULL    ;
    LdrvMsgqAllocatorHandle   mqaHandle     = NULL    ;
    MsgqAsyncErrorMsg       * errorMsg      = NULL    ;

    TRC_4ENTER ("LDRV_MSGQ_SendErrorMsg", errorType, arg1, arg2, arg3) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;

    if (   (LdrvMsgqStateObj.errorMqaId       != MSGQ_INVALID_ID)
        && (LdrvMsgqStateObj.errorHandlerMsgq != MSGQ_INVALID_ID)) {
        mqtHandle = &(LdrvMsgqStateObj.transports
                            [LdrvMsgqStateObj.localTransportId]) ;
        mqaHandle = &(LdrvMsgqStateObj.allocators
                            [LdrvMsgqStateObj.errorMqaId]) ;

        status  = LDRV_MSGQ_Alloc (LdrvMsgqStateObj.errorMqaId,
                                   sizeof (MsgqAsyncErrorMsg),
                                   (MsgqMsg *) &errorMsg) ;
        if (DSP_SUCCEEDED (status)) {
            errorMsg->errorType = (Uint16) errorType ;

            errorMsg->arg1 = arg1 ;
            errorMsg->arg2 = arg2 ;
            errorMsg->arg3 = arg3 ;
            status = LDRV_MSGQ_Put (ID_LOCAL_PROCESSOR,
                                    LdrvMsgqStateObj.errorHandlerMsgq,
                                    (MsgqMsg) errorMsg,
                                    MSGQ_ASYNC_ERROR_MSGID,
                                    MSGQ_INVALID_ID) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Not setting error in this case as its a valid condition that user does
     *  not want to receive error notifications.
     *  ------------------------------------------------------------------------
     */

    TRC_1LEAVE ("LDRV_MSGQ_SendErrorMsg", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_MSGQ_NotImpl
 *
 *  @desc   This function should be used in interface tables where some
 *          functions are not being implemented.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_NotImpl ()
{
    return DSP_ENOTIMPL ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LDRV_MSGQ_Instrument
 *
 *  @desc   This function gets the instrumentation information related to the
 *          specified message queue.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
LDRV_MSGQ_Instrument (IN  ProcessorId      procId,
                      IN  MsgQueueId       msgqId,
                      OUT MsgqInstrument * retVal)
{
    DSP_STATUS              status    = DSP_SOK         ;
    LdrvMsgqTransportHandle mqtHandle = NULL            ;
    TransportId             mqtId     = MSGQ_INVALID_ID ;

    TRC_3ENTER ("LDRV_MSGQ_Instrument", procId, msgqId, retVal) ;

    DBC_Require (LDRV_MSGQ_IsInitialized == TRUE) ;

    if (LDRV_MSGQ_IsInitialized == TRUE) {
        /*  --------------------------------------------------------------------
         *  Extract the handle of the transport and call the instrumentation
         *  function of the corresponding transport.
         *  --------------------------------------------------------------------
         */
        if (IS_LOCAL_PROCID (procId)) {
            mqtHandle = &(LdrvMsgqStateObj.transports
                            [LdrvMsgqStateObj.localTransportId]) ;
        }
        else if (IS_VALID_PROCID (procId)) {
            mqtId = LdrvMsgqStateObj.mqtMap [procId] ;
            mqtHandle = &(LdrvMsgqStateObj.transports [mqtId]) ;
        }
        else {
            TRC_0PRINT (TRC_LEVEL4, "Invalid proc id has been specified.\n") ;
        }

        if (mqtHandle->mqtInfo != NULL) {
            mqtHandle->mqtInterface->mqtInstrument (mqtHandle, msgqId, retVal) ;
        }
        else {
            TRC_0PRINT (TRC_LEVEL4, "This transport is not open.\n") ;
        }
    }
    else {
        TRC_0PRINT (TRC_LEVEL4, "MSGQ component is not yet initialized.\n") ;
    }

    TRC_1LEAVE ("LDRV_MSGQ_Instrument", status) ;

    return status ;
}
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_MSGQ_Debug
 *
 *  @desc   This function prints the debug information related to the
 *          specified message queue.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
LDRV_MSGQ_Debug (IN  ProcessorId procId, IN  MsgQueueId msgqId)
{
    LdrvMsgqTransportHandle mqtHandle = NULL            ;
    TransportId             mqtId     = MSGQ_INVALID_ID ;

    Char8 msgqName [DSP_MAX_STRLEN] ;
    Char8 strMsgqId [3] ;

    TRC_2ENTER ("LDRV_MSGQ_Debug", procId, msgqId) ;

    DBC_Require ((IS_VALID_PROCID (procId)) || (IS_LOCAL_PROCID (procId))) ;
    DBC_Require (IS_VALID_MSGQID (msgqId)) ;

    if (LDRV_MSGQ_IsInitialized == TRUE) {
        DBC_Assert (strMsgqId != NULL) ;
        if (IS_LOCAL_PROCID (procId)) {
            /*  These functions can't fail as they are operating on non NULL
             *  pointers.
             */
            GEN_Strcpyn (msgqName, DSPLINK_GPPMSGQ_NAME, DSP_MAX_STRLEN) ;
        }
        else {
            /*  These functions can't fail as they are operating on non NULL
             *  pointers.
             */
            GEN_Strcpyn (msgqName, DSPLINK_DSPMSGQ_NAME, DSP_MAX_STRLEN) ;
        }

        LDRV_NumToStr (strMsgqId, msgqId) ;
        GEN_Strcatn (msgqName, strMsgqId, DSP_MAX_STRLEN) ;

        /*  --------------------------------------------------------------------
         *  Print the LDRV MSGQ state info fields.
         *  --------------------------------------------------------------------
         */
        TRC_0PRINT (TRC_LEVEL4, "========================================.\n") ;
        TRC_1PRINT (TRC_LEVEL4, "MSGQ Name  = [%s]\n", msgqName) ;
        TRC_1PRINT (TRC_LEVEL4, "numAllocators = 0x[%x].\n",
                                        LdrvMsgqStateObj.numAllocators) ;
        TRC_1PRINT (TRC_LEVEL4, "numTransports = 0x[%x].\n",
                                        LdrvMsgqStateObj.numTransports) ;
        TRC_1PRINT (TRC_LEVEL4, "localTransportId = 0x[%x].\n",
                                        LdrvMsgqStateObj.localTransportId) ;
        TRC_1PRINT (TRC_LEVEL4, "errorHandlerMsgq = 0x[%x].\n",
                                        LdrvMsgqStateObj.errorHandlerMsgq) ;
        TRC_1PRINT (TRC_LEVEL4, "errorMqaId = 0x[%x].\n",
                                        LdrvMsgqStateObj.errorMqaId) ;

        /*  --------------------------------------------------------------------
         *  Extract the handle of the transport and call the debug
         *  function of the corresponding transport.
         *  --------------------------------------------------------------------
         */
        if (IS_LOCAL_PROCID (procId)) {
            mqtHandle = &(LdrvMsgqStateObj.transports
                            [LdrvMsgqStateObj.localTransportId]) ;
        }
        else if (IS_VALID_PROCID (procId)) {
            mqtId = LdrvMsgqStateObj.mqtMap [procId] ;
            mqtHandle = &(LdrvMsgqStateObj.transports [mqtId]) ;
        }

        if (mqtHandle->mqtInfo != NULL) {
            mqtHandle->mqtInterface->mqtDebug (mqtHandle, msgqId) ;
        }
        else {
            TRC_0PRINT (TRC_LEVEL4, "This transport is not open.\n") ;
        }
    }
    else {
        TRC_0PRINT (TRC_LEVEL4, "MSGQ component not yet initialized.\n") ;
    }

    TRC_0LEAVE ("LDRV_MSGQ_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (DDSP_PROFILE)
/** ----------------------------------------------------------------------------
 *  @func   LDRV_MSGQ_ResetProfile
 *
 *  @desc   This function resets the profile statistics of the specified message
 *          queue.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
LDRV_MSGQ_ResetProfile (IN  ProcessorId procId, IN  MsgQueueId msgqId)
{
    if (IS_LOCAL_PROCID (procId)) {
        /*  Reset local message instrumentation data  */
        LDRV_Obj.msgqStats.localMsgqData [msgqId].isValid = FALSE ;
        LDRV_Obj.msgqStats.localMsgqData [msgqId].transferred = 0 ;
        LDRV_Obj.msgqStats.localMsgqData [msgqId].queued = 0 ;
    }
    else {
        /*  Initialize remote message instrumentation data  */
        LDRV_Obj.msgqStats.msgqData [procId][msgqId].isValid = FALSE ;
        LDRV_Obj.msgqStats.msgqData [procId][msgqId].transferred = 0 ;
        LDRV_Obj.msgqStats.msgqData [procId][msgqId].queued = 0 ;
    }
}
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ----------------------------------------------------------------------------
 *  @func   LDRV_NumToStr
 *
 *  @desc   Converts a 1 or 2 digit number to a 2 digit string.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
LDRV_NumToStr (Pstr strNum, Uint16 num)
{
    Char8 tens ;

    TRC_2ENTER ("LDRV_NumToStr", strNum, num) ;

    DBC_Require (strNum != NULL) ;

    tens = (Char8) num / 10 ;
    num  = num % 10 ;

    strNum [0] = tens + '0' ;
    strNum [1] = (Char8) num + '0' ;
    strNum [2] = '\0' ;

    TRC_0LEAVE ("LDRV_NumToStr") ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
