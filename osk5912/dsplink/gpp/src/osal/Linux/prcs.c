/** ============================================================================
 *  @file   prcs.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implementation of sub-component PRCS.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <linux/module.h>
#include <linux/mm.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <prcs.h>
#include <mem.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component ID of this module.
 *  ============================================================================
 */
#define  COMPONENT_ID        ID_OSAL_PRCS

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_OSAL_PRCS, __LINE__)


/** ============================================================================
 *  @name   PrcsObject
 *
 *  @desc   Structure to store information regarding current process/thread.
 *          This structure is specific to Linux.
 *
 *  @field  signature
 *              Signature of this structure.
 *  @field  handleToProcess
 *              Handle to current process.
 *  @field  handleToThread
 *              Handle to current thread.
 *  @field  priorityOfProcess
 *              Priority of current process.
 *  @field  priorityOfThread
 *              Priority of current Thread.
 *  ============================================================================
 */
struct PrcsObject_tag {
    Uint32 signature         ;
    Void * handleToProcess   ;
    Void * handleToThread    ;
    Int32  priorityOfProcess ;
    Int32  priorityOfThread  ;
} ;


/** ============================================================================
 *  @name   PRCS_IsInitialized
 *
 *  @desc   Flag to keep track of initialization of this subcomponent.
 *  ============================================================================
 */
STATIC Bool PRCS_IsInitialized = FALSE ;


/** ============================================================================
 *  @func   PRCS_Initialize
 *
 *  @desc   Initializes the PRCS subcomponent.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Initialize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("PRCS_Initialize") ;

    DBC_Require (PRCS_IsInitialized == FALSE) ;

    PRCS_IsInitialized = TRUE ;

    TRC_1LEAVE ("PRCS_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PRCS_Finalize
 *
 *  @desc   Finalizes the PRCS component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Finalize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("PRCS_Finalize") ;

    DBC_Require (PRCS_IsInitialized == TRUE) ;

    PRCS_IsInitialized = FALSE ;

    TRC_1LEAVE ("PRCS_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PRCS_Create
 *
 *  @desc   Creates a PrcsObject and populates it with information to identify
 *          the client.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Create (OUT PrcsObject ** prcsObj)
{
    DSP_STATUS     status                  = DSP_SOK    ;

    TRC_1ENTER ("PRCS_Create", prcsObj) ;

    DBC_Require (PRCS_IsInitialized == TRUE) ;
    DBC_Require (prcsObj != NULL) ;

    if (prcsObj == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        *prcsObj = NULL ;

        status = MEM_Alloc ((Void **) prcsObj,
                            sizeof (PrcsObject),
                            MEM_DEFAULT) ;

        if (DSP_SUCCEEDED (status)) {
            (*prcsObj)->signature         = SIGN_PRCS            ;
            (*prcsObj)->handleToThread    = (Pvoid) current->pid ;
            (*prcsObj)->handleToProcess   = (Pvoid) current->mm  ;
            (*prcsObj)->priorityOfThread  = 0                    ;
            (*prcsObj)->priorityOfProcess = current->prio        ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    DBC_Ensure (   (   DSP_SUCCEEDED (status)
                    && (prcsObj != NULL)
                    && IS_OBJECT_VALID(*prcsObj, SIGN_PRCS))
                || DSP_FAILED (status)) ;

    TRC_1LEAVE ("PRCS_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PRCS_Delete
 *
 *  @desc   Frees up resources used by the specified object.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_Delete (IN PrcsObject * prcsObj)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_1ENTER ("PRCS_Delete", prcsObj) ;

    DBC_Require (PRCS_IsInitialized == TRUE) ;
    DBC_Require (prcsObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (prcsObj, SIGN_PRCS)) ;

    if (IS_OBJECT_VALID (prcsObj, SIGN_PRCS)) {
        prcsObj->signature = SIGN_NULL ;
        status = FREE_PTR (prcsObj) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PRCS_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PRCS_IsEqual
 *
 *  @desc   Compares two clients to check if they are "equal".
 *          On Linux equality is defined by equality between the process id and
 *          thread id.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_IsEqual (IN  PrcsObject *  client1,
              IN  PrcsObject *  client2,
              OUT Bool *        isEqual)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_3ENTER ("PRCS_IsEqual", client1, client2, isEqual) ;

    DBC_Require (PRCS_IsInitialized == TRUE) ;
    DBC_Require (IS_OBJECT_VALID (client1, SIGN_PRCS)) ;
    DBC_Require (IS_OBJECT_VALID (client2, SIGN_PRCS)) ;
    DBC_Require (isEqual != NULL) ;

    if (   (IS_OBJECT_VALID (client1, SIGN_PRCS) == FALSE)
        || (IS_OBJECT_VALID (client2, SIGN_PRCS) == FALSE)
        || (isEqual == NULL)) {

        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        if (   (client1->handleToProcess == client2->handleToProcess)
            && (client1->handleToThread  == client2->handleToThread)) {
            *isEqual = TRUE ;
        }
        else {
            *isEqual = FALSE ;
        }
    }

    TRC_1LEAVE ("PRCS_IsEqual", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PRCS_IsSameContext
 *
 *  @desc   Checks if the two clients share same context (address space).
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRCS_IsSameContext (IN  PrcsObject *  client1,
                    IN  PrcsObject *  client2,
                    OUT Bool *        isSame)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_3ENTER ("PRCS_IsSameContext", client1, client2, isSame) ;

    DBC_Require (PRCS_IsInitialized == TRUE) ;
    DBC_Require (IS_OBJECT_VALID (client1, SIGN_PRCS)) ;
    DBC_Require (IS_OBJECT_VALID (client2, SIGN_PRCS)) ;
    DBC_Require (isSame != NULL) ;

    if (   (IS_OBJECT_VALID (client1, SIGN_PRCS) == FALSE)
        || (IS_OBJECT_VALID (client2, SIGN_PRCS) == FALSE)
        || (isSame == NULL)) {

        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        if (client1->handleToProcess == client2->handleToProcess) {
            *isSame = TRUE ;
        }
        else {
            *isSame = FALSE ;
        }
    }

    TRC_1LEAVE ("PRCS_IsSameContext", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
