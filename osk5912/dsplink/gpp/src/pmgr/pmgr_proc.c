/** ============================================================================
 *  @file   pmgr_proc.c
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr
 *
 *  @desc   Implementation of sub-component PMGR_PROC.
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
#include <safe.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ----------------------------------- OSAL Headers                */
#include <osal.h>
#include <mem.h>
#include <cfg.h>
#include <prcs.h>
#include <sync.h>
#include <kfile.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Processor Manager           */
#include <pmgr_proc.h>
#if defined (CHNL_COMPONENT)
#include <pmgr_chnl.h>
#endif /* if defined (CHNL_COMPONENT) */

#if defined (MSGQ_COMPONENT)
#include <pmgr_msgq.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Link Driver                 */
#include <ldrv.h>
#include <ldrv_proc.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent ID.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_PMGR_PROC

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_PMGR_PROC, __LINE__)

/** ============================================================================
 *  @name   PMGR_IsSetup
 *
 *  @desc   Flag to indicate that invocation of PMGR_PROC_Setup ()
 *  ============================================================================
 */
STATIC Bool PMGR_IsSetup = FALSE;

/** ============================================================================
 *  @name   PMGR_ClientInfo
 *
 *  @desc   An element that holds process info and that can be manipulated using
 *          LIST.
 *
 *  @field  listElement
 *              Structure that allows it to be used by LIST.
 *  @field  prcsInfo
 *              Placeholder for process information.
 *  ============================================================================
 */
typedef struct PMGR_ClientInfo_tag {
    ListElement   listElement ;
    PrcsObject  * prcsInfo ;
} PMGR_ClientInfo ;


/** ============================================================================
 *  @name   PMGR_PROC_SetupObj
 *
 *  @desc   Object containing information regarding setup of this subcomponent.
 *
 *  @field  signature
 *              Signature of this object.
 *  @field  owner
 *              Identifier of the owner of the subcomponent.
 *  @field  mutex
 *              Critical section object to ensure mutual exclusion.
 *  ============================================================================
 */
typedef struct PMGR_PROC_SetupObj_tag {
    Uint32         signature              ;
    PrcsObject   * owner                  ;
    SyncCsObject * mutex [MAX_PROCESSORS] ;
} PMGR_PROC_SetupObj ;


/** ============================================================================
 *  @name   PMGR_PROC_Object
 *
 *  @desc   Object containing information maintained by this subcomponent.
 *
 *  @field  signature
 *              Signature of this object.
 *  @field  owner
 *              The owner of the processor.
 *  @field  clients
 *              List of clients that have attached to the processor.
 *  @field  entryPoint
 *              Entry point of the executable loaded on target processor.
 *  ============================================================================
 */
typedef struct PMGR_PROC_Object_tag {
    Uint32       signature  ;
    PrcsObject * owner      ;
    List *       clients    ;
    Uint32       entryPoint ;
} PMGR_PROC_Object ;


/** ============================================================================
 *  @name   PMGR_SetupObj
 *
 *  @desc   Object containing information regarding setup of this subcomponent.
 *  ============================================================================
 */
STATIC PMGR_PROC_SetupObj PMGR_SetupObj ;

/** ============================================================================
 *  @name   PMGR_ProcObj
 *
 *  @desc   Array of PMGR_PROC_Object for each processor
 *  ============================================================================
 */
STATIC PMGR_PROC_Object   PMGR_ProcObj [MAX_PROCESSORS] ;


/** ============================================================================
 *  @func   PMGR_PROC_Setup
 *
 *  @desc   Sets up the necessary data structures for the PMGR_PROC
 *          sub-component.
 *          Also stores the client who called this function. This information
 *          is used in PROC_Destroy ().
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Setup ()
{
    DSP_STATUS status = DSP_SOK ;
    Uint32     i      = 0       ;

    if (PMGR_IsSetup == FALSE) {
        status = OSAL_Initialize () ;

        TRC_0ENTER ("PMGR_PROC_Setup") ;
        TRC_1PRINT (TRC_LEVEL1, "PMGR_IsSetup : %d\n", PMGR_IsSetup) ;

        PMGR_IsSetup = TRUE ;

        PMGR_SetupObj.owner     = NULL ;
        PMGR_SetupObj.signature = SIGN_PROC_SETUP ;

        for (i = 0 ; i < MAX_PROCESSORS ; i++) {
            PMGR_SetupObj.mutex [i] = NULL ;

            PMGR_ProcObj [i].signature = SIGN_NULL ;
            PMGR_ProcObj [i].owner     = NULL      ;
            PMGR_ProcObj [i].clients   = NULL      ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = PRCS_Create (&(PMGR_SetupObj.owner)) ;

            if (DSP_SUCCEEDED (status)) {
                status = LDRV_Initialize () ;

                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
#if defined (MSGQ_COMPONENT)
                else {
                    status = PMGR_MSGQ_Setup () ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
#endif /* if defined (MSGQ_COMPONENT) */
            }
            else {
                SET_FAILURE_REASON ;
            }

            for (i = 0 ; (i < MAX_PROCESSORS) && DSP_SUCCEEDED (status) ; i++) {
                status = SYNC_CreateCS (&(PMGR_SetupObj.mutex [i])) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
            }
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_FAILED (status)) {
            PMGR_PROC_Destroy () ;
        }
    }
    else {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_PROC_Setup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_Destroy
 *
 *  @desc   Destroys the data structures for the PMGR_PROC component,
 *          allocated earlier by a call to PROC_Setup ().
 *          This function ensures that the calling client has performed
 *          PROC_Setup ().
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Destroy ()
{
    DSP_STATUS   status    = DSP_SOK ;
    DSP_STATUS   tmpStatus = DSP_SOK ;
    Uint32       i         = 0       ;
    Bool         isOwner   = FALSE   ;
    PrcsObject * client    = NULL    ;

    TRC_0ENTER ("PMGR_PROC_Destroy") ;

    if (PMGR_IsSetup == FALSE) {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }
    else {
        if (PMGR_SetupObj.owner != NULL) {
            status = PRCS_Create (&client) ;
            if (DSP_SUCCEEDED (status)) {
                status = PRCS_IsEqual (client, PMGR_SetupObj.owner, &isOwner) ;
            }
            else {
                SET_FAILURE_REASON ;
            }

            if (DSP_SUCCEEDED (status)) {
                if (isOwner == FALSE) {
                    status = DSP_EACCESSDENIED ;
                    SET_FAILURE_REASON ;
                }
                else {
                    for (i = 0 ; i < MAX_PROCESSORS ; i++) {
                        if (PMGR_SetupObj.mutex [i] != NULL) {
                            tmpStatus = SYNC_DeleteCS (
                                            PMGR_SetupObj.mutex [i]) ;
                            if (   DSP_FAILED (tmpStatus)
                                && DSP_SUCCEEDED (status)) {
                                status = tmpStatus ;
                                SET_FAILURE_REASON ;
                            }
                            PMGR_SetupObj.mutex [i]    = NULL ;
                            PMGR_ProcObj [i].signature = SIGN_NULL ;
                        }
                    }
#if defined (MSGQ_COMPONENT)
                    tmpStatus = PMGR_MSGQ_Destroy () ;
                    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
#endif /* if defined (MSGQ_COMPONENT) */
                    tmpStatus = LDRV_Finalize () ;
                    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }

                    if (PMGR_SetupObj.owner != NULL) {
                        tmpStatus = PRCS_Delete (PMGR_SetupObj.owner) ;
                        PMGR_SetupObj.owner = NULL ;

                        if (   DSP_FAILED (tmpStatus)
                            && DSP_SUCCEEDED (status)) {
                            status = tmpStatus ;
                            SET_FAILURE_REASON ;
                        }
                    }
                }
            }
            else {
                SET_FAILURE_REASON ;
            }

            if (client != NULL) {
                tmpStatus = PRCS_Delete (client) ;
                client = NULL ;
                if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                    tmpStatus = status ;
                    SET_FAILURE_REASON ;
                }
            }
        }
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = OSAL_Finalize () ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    PMGR_SetupObj.signature = SIGN_NULL ;
    PMGR_IsSetup = FALSE ;

    TRC_1LEAVE ("PMGR_PROC_Destroy", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_Attach
 *
 *  @desc   Attaches the client to the specified DSP and also
 *          initializes the DSP (if required).
 *          The first caller to this function is designated as the owner of
 *          the DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Attach (IN  ProcessorId procId,
                  OPT ProcAttr *  attr)
{
    DSP_STATUS        status    = DSP_SOK ;
    DSP_STATUS        tmpStatus = DSP_SOK ;
    PMGR_ClientInfo * client    = NULL    ;
    PrcsObject *      prcsInfo  = NULL    ;
    Bool              attached  = FALSE   ;

    TRC_2ENTER ("PMGR_PROC_Attach", procId, attr) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    DBC_Assert (PMGR_SetupObj.owner != NULL) ;

    if (PMGR_SetupObj.owner == NULL) {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }
    else {
        status = PRCS_Create (&prcsInfo) ;
        if (DSP_SUCCEEDED (status)) {
            status = SYNC_EnterCS (PMGR_SetupObj.mutex [procId]) ;
            if (   (PMGR_ProcObj [procId].owner == NULL)
                && DSP_SUCCEEDED (status)) {
                /*  ------------------------------------------------------------
                 *  No client is attached to the specified processor so far.
                 *  The calling client is designated as the owner.
                 *  ------------------------------------------------------------
                 */

                if (DSP_SUCCEEDED (status)) {
                    status = LIST_Create (&(PMGR_ProcObj [procId].clients)) ;

                    if (DSP_SUCCEEDED (status)) {
                        status = LDRV_PROC_Initialize (procId) ;
                        if (DSP_SUCCEEDED (status)) {
                            PMGR_ProcObj [procId].owner     = prcsInfo  ;
                            PMGR_ProcObj [procId].signature = SIGN_PROC ;
                        }
                        else {
                            LIST_Delete (PMGR_ProcObj [procId].clients) ;
                            PMGR_ProcObj [procId].clients = NULL ;
                            SET_FAILURE_REASON ;
                        }
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }

#if defined (CHNL_COMPONENT)
                    if (DSP_SUCCEEDED (status)) {
                        status = PMGR_CHNL_Initialize (procId) ;
                        if (DSP_FAILED (status)) {
                            LIST_Delete (PMGR_ProcObj [procId].clients) ;
                            PMGR_ProcObj [procId].clients   = NULL ;
                            PMGR_ProcObj [procId].signature = SIGN_NULL ;
                            PMGR_ProcObj [procId].owner     = NULL ;
                            SET_FAILURE_REASON ;
                        }
                    }
#endif  /* if defined (CHNL_COMPONENT) */
                }
                else {
                    SET_FAILURE_REASON ;
                }

                tmpStatus = SYNC_LeaveCS (PMGR_SetupObj.mutex [procId]) ;
                if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
            }
            else if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
            else {
                /*  ------------------------------------------------------------
                 *  Client is not the owner.
                 *  ------------------------------------------------------------
                 */
                tmpStatus = SYNC_LeaveCS (PMGR_SetupObj.mutex [procId]) ;

                if (DSP_FAILED (tmpStatus)) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
                else {
                    status = PMGR_PROC_IsAttached
                                        (procId, prcsInfo, &attached) ;
                }

                if (DSP_SUCCEEDED (status)) {
                    tmpStatus = SYNC_EnterCS (PMGR_SetupObj.mutex [procId]) ;

                    if (DSP_FAILED (tmpStatus)) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
                if (DSP_SUCCEEDED (status)) {
                    /* Check again whether owner has detached, to prevent a race
                     * condition.
                     */
                    if (PMGR_ProcObj [procId].owner != NULL) {
                        if (attached == TRUE) {
                            status = DSP_SALREADYATTACHED ;
                        }
                        else {
                            status = MEM_Alloc ((Void **) &client,
                                                sizeof (PMGR_ClientInfo),
                                                MEM_DEFAULT) ;

                            if (DSP_SUCCEEDED (status)) {
                                client->prcsInfo = prcsInfo ;
                                status = LIST_InitializeElement (
                                                &(client->listElement)) ;
                            }
                            else {
                                SET_FAILURE_REASON ;
                            }

                            if (DSP_SUCCEEDED (status)) {
                                status = LIST_PutTail (
                                             PMGR_ProcObj [procId].clients,
                                             &(client->listElement)) ;

                                if (DSP_FAILED (status)) {
                                    status = DSP_EFAIL ;
                                    FREE_PTR (client) ;
                                    SET_FAILURE_REASON ;
                                }
                            }
                            else {
                                FREE_PTR (client) ;
                                SET_FAILURE_REASON ;
                            }
                        }
                    }
                    else {
                        TRC_0PRINT (
                           TRC_LEVEL4,
                           "Processor attach, detach are not synchronised\n") ;
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }

                    tmpStatus = SYNC_LeaveCS (PMGR_SetupObj.mutex [procId]) ;
                    if (   DSP_FAILED (tmpStatus)
                        && DSP_SUCCEEDED (status)) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }

            if (DSP_FAILED (status) || (status == DSP_SALREADYATTACHED)) {
                PRCS_Delete (prcsInfo) ;
                prcsInfo = NULL ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_PROC_Attach", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_Detach
 *
 *  @desc   Detaches the client from specified processor.
 *          If the caller is the owner of the processor, this function releases
 *          all the resources that this component uses and puts the DSP in an
 *          unusable state (from application perspective).
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Detach (IN ProcessorId procId)
{
    DSP_STATUS        status     = DSP_SOK ;
    DSP_STATUS        tmpStatus  = DSP_SOK ;
    PrcsObject *      client     = NULL    ;
    Bool              isEqual    = FALSE   ;
    PMGR_ClientInfo * clientInfo = NULL    ;
    PMGR_ClientInfo * nextClient = NULL    ;

    TRC_1ENTER ("PMGR_PROC_Detach", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (PMGR_ProcObj [procId].owner == NULL) {
        status = DSP_EATTACHED ;
        SET_FAILURE_REASON ;
    }
    else {
        status = PRCS_Create (&client) ;
        if (DSP_SUCCEEDED (status)) {
            status = PRCS_IsEqual (client,
                                   PMGR_ProcObj [procId].owner,
                                   &isEqual) ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            status = SYNC_EnterCS (PMGR_SetupObj.mutex [procId]) ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        if (isEqual) {
            /*  ------------------------------------------------------------
             *  Client is the owner.
             *  Remove all attached clients and delete the client list.
             *  ------------------------------------------------------------
             */
#if defined (CHNL_COMPONENT)
            status = PMGR_CHNL_Finalize (procId) ;

            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
#endif /* if defined (CHNL_COMPONENT) */

            tmpStatus = LDRV_PROC_Finalize (procId) ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }

            while (  (!LIST_IsEmpty (PMGR_ProcObj [procId].clients))
                   && DSP_SUCCEEDED (status)) {

                status = LIST_GetHead (PMGR_ProcObj [procId].clients,
                                          (ListElement **) &clientInfo) ;
                if (DSP_SUCCEEDED (status)) {
                    status = PRCS_Delete (clientInfo->prcsInfo) ;
                    clientInfo->prcsInfo = NULL ;

                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }

                    tmpStatus = FREE_PTR (clientInfo) ;
                    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }

            tmpStatus = LIST_Delete (PMGR_ProcObj [procId].clients) ;
            PMGR_ProcObj [procId].clients = NULL ;

            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }

            tmpStatus = PRCS_Delete (PMGR_ProcObj [procId].owner) ;
            PMGR_ProcObj [procId].owner     = NULL ;
            PMGR_ProcObj [procId].signature = SIGN_NULL ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            /*  ------------------------------------------------------------
             *  Client is not an owner.
             *  Find the element in the list and then remove it.
             *  ------------------------------------------------------------
             */
            if (LIST_IsEmpty (PMGR_ProcObj [procId].clients)) {
                status = DSP_EATTACHED ;
                SET_FAILURE_REASON ;
            }
            else {
                status = LIST_First (PMGR_ProcObj [procId].clients,
                                     (ListElement **) &clientInfo) ;
                while (   (clientInfo != NULL)
                       && (isEqual == FALSE)
                       && DSP_SUCCEEDED (status)) {
                    status = PRCS_IsEqual (clientInfo->prcsInfo,
                                           client,
                                           &isEqual) ;

                    if (DSP_SUCCEEDED (status)) {
                        if (isEqual == TRUE) {
                            status = LIST_RemoveElement (
                                        PMGR_ProcObj [procId].clients,
                                        (ListElement *) clientInfo) ;

                            if (DSP_FAILED (status)) {
                                status = DSP_EFAIL ;
                                SET_FAILURE_REASON ;
                            }

                            tmpStatus = PRCS_Delete (clientInfo->prcsInfo) ;
                            clientInfo->prcsInfo = NULL ;
                            if (   DSP_FAILED (tmpStatus)
                                && DSP_SUCCEEDED (status)) {
                                status = tmpStatus ;
                                SET_FAILURE_REASON ;
                            }

                            tmpStatus = FREE_PTR (clientInfo) ;
                            if (   DSP_FAILED (tmpStatus)
                                && DSP_SUCCEEDED (status)) {
                                status = tmpStatus ;
                                SET_FAILURE_REASON ;
                            }
                        }
                        else {
                            status = LIST_Next (PMGR_ProcObj [procId].clients,
                                                (ListElement *)  clientInfo,
                                                (ListElement **) &nextClient) ;

                            if (DSP_SUCCEEDED (status)) {
                                clientInfo = nextClient ;
                            }
                            else {
                                status = DSP_EFAIL ;
                                SET_FAILURE_REASON ;
                            }
                        }
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }
                }

                if (DSP_FAILED (status)) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
                else {
                    if (isEqual == FALSE) {
                        status = DSP_EATTACHED ;
                        SET_FAILURE_REASON ;
                    }
                }
            }
        }

        tmpStatus = SYNC_LeaveCS (PMGR_SetupObj.mutex [procId]) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (client != NULL) {
        tmpStatus = PRCS_Delete (client) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
        client = NULL ;  /* Make client NULL irrespective of status above */
    }

    TRC_1LEAVE ("PMGR_PROC_Detach", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_GetState
 *
 *  @desc   Gets the current status of DSP by querying the Link Driver.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_GetState (IN   ProcessorId    procId,
                    OUT  ProcState *    procState)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("PMGR_PROC_GetState", procId, status) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (procState != NULL ) ;

    status = LDRV_PROC_GetState (procId, procState) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_PROC_GetState", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_Load
 *
 *  @desc   Loads the specified DSP executable on the target DSP.
 *          It ensures that the caller owns the DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Load (IN   ProcessorId  procId,
                IN   Char8 *      imagePath,
                IN   Uint32       argc,
                IN   Char8 **     argv)
{
    DSP_STATUS        status       = DSP_SOK ;
    DSP_STATUS        tmpStatus    = DSP_SOK ;
    Bool              isOwner      = FALSE   ;
    PrcsObject  *     client       = NULL    ;
    LoaderInterface * loaderIntf   = NULL    ;
    CFG_Dsp           cfgDspObj              ;
    LoaderObject      loaderObj              ;

    TRC_4ENTER ("PMGR_PROC_Load", procId, imagePath, argc, argv) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (imagePath != NULL ) ;
    DBC_Require (   ((argc != 0) && (argv != NULL))
                 || ((argc == 0) && (argv == NULL))) ;

    if (PMGR_ProcObj [procId].owner == NULL) {
        status = DSP_EATTACHED ;
        SET_FAILURE_REASON ;
    }
    else {
        status = PRCS_Create (&client) ;

        if (DSP_SUCCEEDED (status)) {
            status = PRCS_IsEqual (client,
                                   PMGR_ProcObj [procId].owner,
                                   &isOwner) ;

            if (DSP_SUCCEEDED (status)) {
                if (isOwner) {
                    status = CFG_GetRecord (CFG_DSP_OBJECT,
                                            procId,
                                            &cfgDspObj) ;
                    if (DSP_SUCCEEDED (status)) {
                        loaderIntf = cfgDspObj.loaderInterface ;
                        loaderObj.baseImage = imagePath ;
                        loaderObj.dspArch   = (DspArch) cfgDspObj.dspArch ;
                        loaderObj.endian    = (Endianism) cfgDspObj.endian ;
                        loaderObj.wordSize  = cfgDspObj.wordSize ;
                        status = (*(loaderIntf->load)) (
                                        procId,
                                        &loaderObj,
                                        argc,
                                        argv,
                                        &(PMGR_ProcObj [procId].entryPoint)) ;
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }
                    if (DSP_SUCCEEDED (status)) {
                        status = LDRV_PROC_SetState (procId, ProcState_Loaded) ;

                        if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }
                    else {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    status = DSP_EACCESSDENIED ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }

            tmpStatus = PRCS_Delete (client) ;
            client = NULL ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                tmpStatus = status ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_PROC_Load", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_LoadSection
 *
 *  @desc   Loads the specified section of DSP executable onto the target DSP.
 *          It ensures that the client owns the DSP.
 *          Currently this function is not implemented.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_LoadSection (IN    ProcessorId  procId,
                       IN    Char8 *      imagePath,
                       IN    Uint32       sectID)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("PMGR_PROC_LoadSection", procId, imagePath, sectID) ;

    status = DSP_ENOTIMPL ;

    TRC_1LEAVE ("PMGR_PROC_LoadSection", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_Start
 *
 *  @desc   Starts execution of the loaded code on DSP from the starting
 *          point specified in the DSP executable loaded earlier by call to
 *          PROC_Load ().
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Start (IN  ProcessorId  procId)
{
    DSP_STATUS   status    = DSP_SOK ;
    DSP_STATUS   tmpStatus = DSP_SOK ;
    Bool         isOwner   = FALSE   ;
    PrcsObject * client    = NULL    ;

    TRC_1ENTER ("PMGR_PROC_Start", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (PMGR_ProcObj [procId].owner == NULL) {
        status = DSP_EATTACHED ;
        SET_FAILURE_REASON ;
    }
    else {
        status = PRCS_Create (&client) ;

        if (DSP_SUCCEEDED (status)) {
            status = PRCS_IsEqual (client,
                                   PMGR_ProcObj [procId].owner,
                                   &isOwner) ;

            if (DSP_SUCCEEDED (status)) {
                if (isOwner) {
                    status = LDRV_PROC_Start (procId,
                                              PMGR_ProcObj [procId].entryPoint) ;

                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    status = DSP_EACCESSDENIED ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }

            tmpStatus = PRCS_Delete (client) ;
            client = NULL ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                tmpStatus = status ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_PROC_Start", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_Stop
 *
 *  @desc   Stops the specified DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Stop (IN  ProcessorId  procId)
{
    DSP_STATUS   status    = DSP_SOK ;
    DSP_STATUS   tmpStatus = DSP_SOK ;
    Bool         isOwner   = FALSE   ;
    PrcsObject * client    = NULL    ;

    TRC_1ENTER ("PMGR_PROC_Stop", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (PMGR_ProcObj [procId].owner == NULL) {
        status = DSP_EATTACHED ;
        SET_FAILURE_REASON ;
    }
    else {
        status = PRCS_Create (&client) ;

        if (DSP_SUCCEEDED (status)) {
            status = PRCS_IsEqual (client,
                                   PMGR_ProcObj [procId].owner,
                                   &isOwner) ;

            if (DSP_SUCCEEDED (status)) {
                if (isOwner) {
                    status = LDRV_PROC_Stop (procId) ;

                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    status = DSP_EACCESSDENIED ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }

            tmpStatus = PRCS_Delete (client) ;
            client = NULL ;
            if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                tmpStatus = status ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PMGR_PROC_Stop", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations on
 *          the DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Control (IN  ProcessorId procId,
                   IN  Int32       cmd,
                   OPT Pvoid       arg)
{
    DSP_STATUS    status  = DSP_SOK ;

    TRC_3ENTER ("PMGR_PROC_Control", procId, cmd, arg) ;

    status = DSP_ENOTIMPL ;

    TRC_1LEAVE ("PMGR_PROC_Control", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PMGR_PROC_IsAttached
 *
 *  @desc   Function to check whether the client identified by the specified
 *          'client' object is attached to the specified processor.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_IsAttached (IN  ProcessorId  procId,
                      IN  PrcsObject * client,
                      OUT Bool *       isAttached)
{
    DSP_STATUS        status     = DSP_SOK ;
    DSP_STATUS        tmpStatus  = DSP_SOK ;
    Bool              isEqual    = FALSE   ;
    PMGR_ClientInfo * clientInfo = NULL    ;
    PMGR_ClientInfo * nextClient = NULL    ;

    TRC_3ENTER ("PMGR_PROC_IsAttached", procId, client, isAttached) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (client != NULL) ;
    DBC_Require (isAttached != NULL) ;

    status = SYNC_EnterCS (PMGR_SetupObj.mutex [procId]) ;

    if (PMGR_ProcObj [procId].owner == NULL && DSP_SUCCEEDED (status)) {
        *isAttached = FALSE ;
    }
    else if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    else {
        status = PRCS_IsSameContext (client,
                                     PMGR_ProcObj [procId].owner,
                                     &isEqual) ;

        if (DSP_SUCCEEDED (status)) {
            if (isEqual == FALSE) {
                if (LIST_IsEmpty (PMGR_ProcObj [procId].clients)) {
                    *isAttached = FALSE ;
                }
                else {
                    status = LIST_First (PMGR_ProcObj [procId].clients,
                                         (ListElement **) &clientInfo) ;

                    if (DSP_FAILED (status)) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }

                    while (   (clientInfo != NULL)
                           && (isEqual == FALSE)
                           && DSP_SUCCEEDED (status)) {

                        status = PRCS_IsSameContext (clientInfo->prcsInfo,
                                                     client,
                                                     &isEqual) ;

                        if (DSP_SUCCEEDED (status) && (isEqual == FALSE)) {
                            status = LIST_Next (PMGR_ProcObj [procId].clients,
                                                (ListElement *)  clientInfo,
                                                (ListElement **) &nextClient) ;

                            if (DSP_SUCCEEDED (status)) {
                                clientInfo = nextClient ;
                            }
                            else {
                                SET_FAILURE_REASON ;
                            }
                        }
                        else if (DSP_FAILED (status)) {
                            SET_FAILURE_REASON ;
                        }
                    }

                    *isAttached = isEqual ;
                }
            }
            else {
                *isAttached = TRUE ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    tmpStatus = SYNC_LeaveCS (PMGR_SetupObj.mutex [procId]) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_PROC_IsAttached", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PMGR_PROC_Instrument
 *
 *  @desc   Gets the instrumentation data associated with PMGR_PROC
 *          sub-component.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Instrument (IN ProcessorId procId, OUT ProcInstrument * retVal)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("PMGR_PROC_Instrument", retVal) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (retVal != NULL) ;

    status = LDRV_PROC_Instrument (procId, retVal) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PMGR_PROC_Instrument () [0x%x]\n",
                status) ;

    return status ;
}
#endif

#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PMGR_PROC_Debug
 *
 *  @desc   Prints the debug information summarizing the current status
 *          of the PMGR_PROC component.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
PMGR_PROC_Debug (IN ProcessorId procId)
{
    DSP_STATUS        status      = DSP_SOK ;
    PMGR_ClientInfo * clientInfo  = NULL    ;
    PMGR_ClientInfo * nextClient  = NULL    ;
    Uint32            clientCount = 0       ;

    TRC_1ENTER ("PMGR_PROC_Debug", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    TRC_1PRINT (TRC_LEVEL4,
                "   Setup owner = [0x%x]\n",
                PMGR_SetupObj.owner) ;

    TRC_1PRINT (TRC_LEVEL4, "Processor Id : %d\n", procId) ;

    TRC_1PRINT (TRC_LEVEL4, "PMGR_IsSetup: %d\n", PMGR_IsSetup) ;

    if (PMGR_IsSetup) {
        TRC_1PRINT (TRC_LEVEL4,
                 "   ownerAttached = %s\n",
                 ((PMGR_ProcObj [procId].owner != NULL) ? "True" : "False")) ;

        TRC_1PRINT (TRC_LEVEL4,
                    "   Attach owner = [0x%x]\n",
                    PMGR_ProcObj [procId].owner) ;

        TRC_1PRINT (TRC_LEVEL4,
                    "   entryPoint     = [0x%x]\n",
                    PMGR_ProcObj [procId].entryPoint) ;

        TRC_1PRINT (TRC_LEVEL4,
                    "   Client : [0x%x]\n",
                    PMGR_ProcObj [procId].clients) ;

        if (PMGR_ProcObj [procId].clients != NULL) {
            status = LIST_First (PMGR_ProcObj [procId].clients,
                                 (ListElement **) &clientInfo) ;
            if (DSP_SUCCEEDED (status)) {
                while (clientInfo != NULL) {
                    clientCount++ ;

                    TRC_2PRINT (TRC_LEVEL4,
                                "        client [%d] = [0x%x]\n",
                                clientCount, &(clientInfo->prcsInfo)) ;

                    status = LIST_Next (PMGR_ProcObj [procId].clients,
                                        (ListElement *) clientInfo,
                                        (ListElement **) &nextClient) ;
                    if (DSP_FAILED (status)) {
                        clientInfo = NULL ;
                    }
                    else {
                        clientInfo = nextClient ;
                    }
                }
            }
            else {
                TRC_0PRINT (TRC_LEVEL4, "Client list is empty\n") ;
            }

            if (DSP_SUCCEEDED (status)) {
                LDRV_PROC_Debug (procId) ;
            }
        }
    }
    else {
        TRC_0PRINT (TRC_LEVEL4, "Processor is not yet setup\n") ;
    }

    TRC_1LEAVE ("PMGR_PROC_Debug", status) ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
