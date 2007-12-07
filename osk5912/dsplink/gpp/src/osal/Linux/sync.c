/** ============================================================================
 *  @file   sync.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implementation of sub-component SYNC.
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
#include <asm/semaphore.h>
#include <linux/timer.h>
#include <linux/sched.h>

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
#include <mem.h>
#include <dpc.h>
#include <sync.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Identifier for sub-component.
 *  ============================================================================
 */
#define COMPONENT_ID        ID_OSAL_SYNC

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_OSAL_SYNC, __LINE__)


/** ============================================================================
 *  @name   SyncEvObject
 *
 *  @desc   This object is used for various event related API.
 *
 *  @field  signature
 *              For identification of this object.
 *  @field  eventSem
 *              OS specific semaphore object.
 *  @field  timeoutOccurred
 *              Indicates that timeout had occurred.
 *
 *  @see    None
 *  ============================================================================
 */
struct SyncEvObject_tag {
    Uint32            signature       ;
    struct semaphore  eventSem        ;
    Bool              timeoutOccurred ;
} ;

/** ============================================================================
 *  @name   SyncCsObject
 *
 *  @desc   This object is used by various CS API's.
 *
 *  @field  signature
 *              For identification of this object.
 *  @field  sem
 *              OS specific semaphore that is used to implement CS API.
 *
 *  @see    None
 *  ============================================================================
 */
struct SyncCsObject_tag {
    Uint32            signature ;
    struct semaphore  sem       ;
} ;

/** ============================================================================
 *  @name   SyncSemObject
 *
 *  @desc   This object is used by various SEM API's.
 *
 *  @field  signature
 *              For identification of this object.
 *  @field  semType
 *              Indicates the type of the semaphore (binary or counting).
 *  @field  isSemAvailable
 *              Flag to indicate if the binary semaphore is available.
 *              If flag is TRUE then semaphore is available.
 *              If flag is FALSE then semaphore is not available.
 *  @field  sem
 *              OS specific semaphore.
 *  @field  timeoutOccurred
 *              Indicates that timeout had occurred.
 *
 *  @see    None
 *  ============================================================================
 */
struct SyncSemObject_tag {
    Uint32            signature       ;
    SyncSemType       semType         ;
    Bool              isSemAvailable  ;
    struct semaphore  sem             ;
    Bool              timeoutOccurred ;
} ;


/** ----------------------------------------------------------------------------
 *  @name   SYNC_IsInitialized
 *
 *  @desc   Indicates if the module has been initialized or not.
 *  ----------------------------------------------------------------------------
 */
STATIC Bool SYNC_IsInitialized = FALSE ;

/** ----------------------------------------------------------------------------
 *  @name   SYNC_PreemptLock
 *
 *  @desc   Spin lock structure to provide protection from interrupts,
 *          tasks and tasklets.
 *  ----------------------------------------------------------------------------
 */
STATIC spinlock_t SYNC_PreemptLock = SPIN_LOCK_UNLOCKED ;

/** ----------------------------------------------------------------------------
 *  @name   SYNC_ProtectCsObj
 *
 *  @desc   Handle to the critical section object used for protecting against
 *          other tasks during SYNC_ProtectionStart ()/SYNC_ProtectionEnd ()
 *          calls.
 *  ----------------------------------------------------------------------------
 */
STATIC SyncCsObject * SYNC_ProtectCsObj = NULL ;

/** ----------------------------------------------------------------------------
 *  @func   SYNC_TimeoutHandler
 *
 *  @desc   timeout handler function.
 *
 *  @arg    eventPtr
 *              Pointer to the event object for which timeout has occurred.
 *
 *  @ret    None.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ----------------------------------------------------------------------------
 */
STATIC
void
SYNC_TimeoutHandler (unsigned long eventPtr) ;


/** ----------------------------------------------------------------------------
 *  @func   SYNC_SEM_TimeoutHandler
 *
 *  @desc   timeout handler function.
 *
 *  @arg    semPtr
 *              Pointer to the semaphore object for which timeout has occurred.
 *
 *  @ret    None.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ----------------------------------------------------------------------------
 */
STATIC
void
SYNC_SEM_TimeoutHandler (unsigned long semPtr) ;


/** ============================================================================
 *  @func   SYNC_Initialize
 *
 *  @desc   Initialize SYNC sub-component by allocating all resources.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_Initialize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("SYNC_Initialize") ;

    DBC_Require (SYNC_IsInitialized == FALSE) ;

    /*  Create the SYNC protection CS object  */
    status = SYNC_CreateCS (&SYNC_ProtectCsObj) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    SYNC_IsInitialized = TRUE ;

    TRC_1LEAVE ("SYNC_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_Finalize
 *
 *  @desc   Releases resources used by the sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_Finalize ()
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_0ENTER ("SYNC_Finalize") ;

    DBC_Require (SYNC_IsInitialized == TRUE) ;

    /*  Delete the SYNC protection CS object  */
    if (SYNC_ProtectCsObj != NULL) {
        status = SYNC_DeleteCS (SYNC_ProtectCsObj) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        SYNC_ProtectCsObj = NULL ;
    }

    SYNC_IsInitialized = FALSE ;

    TRC_1LEAVE ("SYNC_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_OpenEvent
 *
 *  @desc   Creates and initializes an event object for thread
 *          synchronization. The event is initialized to a
 *          non-signaled state.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_OpenEvent (OUT SyncEvObject ** event,
                IN  SyncAttrs *     attr)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_2ENTER ("SYNC_OpenEvent", event, attr) ;

    DBC_Require (event != NULL) ;
    DBC_Require (attr != NULL) ;

    if (attr == NULL) {
        if (event != NULL) {
            *event = NULL ;
        }
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (event == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        status = MEM_Calloc ((Void **) event, sizeof (SyncEvObject),
                            MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            sema_init (&((*event)->eventSem), 0) ;
            (*event)->signature = SIGN_SYNC ;
        }
        else {
            SET_FAILURE_REASON ;
            *event = NULL ;
        }
    }

    DBC_Ensure (   ((event == NULL) && DSP_FAILED (status))
                 || ((*event == NULL) && DSP_FAILED (status))
                 || (   ((*event)->signature == SIGN_SYNC)
                     && DSP_SUCCEEDED (status))) ;

    TRC_1LEAVE ("SYNC_OpenEvent", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_CloseEvent
 *
 *  @desc   Closes the handle corresponding to an event. It also frees the
 *          resources allocated, if any, during call to SYNC_OpenEvent ().
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_CloseEvent (IN SyncEvObject * event)
{
    DSP_STATUS   status   = DSP_SOK    ;

    TRC_1ENTER ("SYNC_CloseEvent", event) ;

    DBC_Require (IS_OBJECT_VALID (event, SIGN_SYNC)) ;
    DBC_Require (event != NULL) ;

    status = (IS_OBJECT_VALID (event, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        event->signature = SIGN_NULL ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = FREE_PTR (event) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("SYNC_CloseEvent", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_ResetEvent
 *
 *  @desc   Reset the synchronization object to non-signaled state.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_ResetEvent (IN SyncEvObject * event)
{
    DSP_STATUS   status   = DSP_SOK    ;

    TRC_1ENTER ("SYNC_ResetEvent", event) ;

    DBC_Require (event != NULL) ;
    DBC_Require (IS_OBJECT_VALID (event, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (event, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        /* If event semaphore is 1. it will be made 0 by next statement */
        down_trylock (&(event->eventSem)) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SYNC_ResetEvent", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_SetEvent
 *
 *  @desc   Set the state of synchronization object to signaled and unblock all
 *          threads waiting for it.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_SetEvent (IN SyncEvObject * event)
{
    DSP_STATUS status   = DSP_SOK    ;
    Uint32     irqFlags              ;

    TRC_1ENTER ("SYNC_SetEvent", event) ;

    DBC_Require (IS_OBJECT_VALID (event, SIGN_SYNC)) ;
    DBC_Require (event != NULL) ;

    status = (IS_OBJECT_VALID (event, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        irqFlags = SYNC_SpinLockStart () ;
        /* If event semaphore is 1. it will be made 0 by next statement */
        down_trylock (&(event->eventSem)) ;
        /* Now make eventSem 1 */
        up (&(event->eventSem)) ;
        SYNC_SpinLockEnd (irqFlags) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SYNC_SetEvent", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_WaitOnEvent
 *
 *  @desc   Wait for an event to be signaled for a specified amount of time.
 *          It is possible to wait infinitely. This function must 'block'
 *          and not 'spin'.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_WaitOnEvent (IN SyncEvObject * event, IN Uint32 timeout)
{
    DSP_STATUS        status         = DSP_SOK ;
    Bool              timerCreated   = FALSE   ;
    struct timer_list timer                    ;
    Uint32            irqFlags                 ;

    TRC_2ENTER ("SYNC_WaitOnEvent", event, timeout) ;

    DBC_Require (event != NULL) ;
    DBC_Require (IS_OBJECT_VALID (event, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (event, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        event->timeoutOccurred = FALSE ;
        /* Put up a timer in case of a finite non zero timeout. This will
         * invoke a function at specified time to give the semaphore and do
         * timeout on the event.
         */
        if ((timeout != SYNC_WAITFOREVER) && (timeout != SYNC_NOWAIT)) {
            init_timer (&timer) ;
            /* jiffies represent current internal timer count. */
            timer.expires = jiffies + (timeout * HZ)/1000 ;
            timer.data = (unsigned long) event ;
            timer.function = SYNC_TimeoutHandler ;
            add_timer (&timer) ;
            timerCreated = TRUE ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        if (timeout != SYNC_NOWAIT) {
            if (down_interruptible (&(event->eventSem))) {
                /*
                 * Sleep has been broken prematurely: Results are
                 * undefined in this error condition.
                 */
                TRC_0PRINT (TRC_LEVEL7,
                            "Interrupted while waiting on semaphore\n") ;
                if (timerCreated == TRUE) {
                    /* We don't care about the return value it only
                     * tells that if the timer was queued or not
                     * at the time of deletion
                     */
                    del_timer_sync (&timer) ;
                    /* Make sure event is still in reset state */
                    down_trylock (&(event->eventSem)) ;
                }
                status = SYNC_E_FAIL ;
                SET_FAILURE_REASON ;
            }
            else {
                if (timerCreated == TRUE) {
                    /* We don't care about the return value it only
                     * tells that if the timer was queued or not
                     * at the time of deletion
                     */
                    del_timer_sync (&timer) ;
                }
                /* Make sure we maintain the semaphore to 1 */
                irqFlags = SYNC_SpinLockStart () ;
                down_trylock (&(event->eventSem)) ;
                up (&(event->eventSem)) ;
                SYNC_SpinLockEnd (irqFlags) ;
            }
        }
        else {
            /* For zero timeout just call down_trylock to try to get the
             * semaphore without waiting */
            irqFlags = SYNC_SpinLockStart () ;
            if (down_trylock (&(event->eventSem))) {
                status = SYNC_E_FAIL ;
                SET_FAILURE_REASON ;
            }
            else {
                up (&(event->eventSem)) ;
            }
            SYNC_SpinLockEnd (irqFlags) ;
        }
    }

    /* In case semaphore is given by the timeout handler. Set the status
     * to DSP_ETIMEOUT */
    if (DSP_SUCCEEDED (status) && event->timeoutOccurred) {
        status = DSP_ETIMEOUT ;
    }

    TRC_1LEAVE ("SYNC_WaitOnEvent", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_WaitOnMultipleEvents
 *
 *  @desc   Waits on muliple events. Returns when any of the event is set.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_WaitOnMultipleEvents (IN  SyncEvObject **   syncEvents,
                           IN  Uint32            count,
                           IN  Uint32            timeout,
                           OUT Uint32 *          index)
{
    DSP_STATUS      status = DSP_SOK ;

    TRC_4ENTER ("SYNC_WaitOnMultipleEvents", syncEvents, count,
                                             timeout, index) ;
    DBC_Require (syncEvents != NULL) ;
    DBC_Require (index != NULL) ;

    /* Not Implemented */
    status = DSP_ENOTIMPL ;
    SET_FAILURE_REASON ;

    TRC_1LEAVE ("SYNC_WaitOnMultipleEvents", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_CreateCS
 *
 *  @desc   Creates the Critical section structure.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_CreateCS (OUT SyncCsObject ** cSObj)
{
    DSP_STATUS   status   = DSP_SOK    ;

    TRC_1ENTER ("SYNC_CreateCS", cSObj) ;

    DBC_Require (cSObj != NULL) ;

    if (cSObj == NULL) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        status =  MEM_Calloc ((Void **) cSObj, sizeof (SyncCsObject),
                              MEM_DEFAULT) ;
    }

    if (DSP_SUCCEEDED (status)) {
        sema_init (&((*cSObj)->sem), 1) ;
        (*cSObj)->signature = SIGN_SYNC ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   ((cSObj == NULL) && DSP_FAILED (status))
                 || ((*cSObj == NULL) && DSP_FAILED (status))
                 || (   ((*cSObj)->signature == SIGN_SYNC)
                     && DSP_SUCCEEDED (status))) ;

    TRC_1LEAVE ("SYNC_CreateCS", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_DeleteCS
 *
 *  @desc   Deletes the critical section object.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_DeleteCS (IN SyncCsObject * cSObj)
{
    DSP_STATUS   status   = DSP_SOK    ;

    TRC_1ENTER ("SYNC_DeleteCS", cSObj) ;

    DBC_Require (cSObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (cSObj, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (cSObj, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        cSObj->signature = SIGN_NULL ;
        status = FREE_PTR (cSObj) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SYNC_DeleteCS", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_EnterCS
 *
 *  @desc   This function enters the critical section that is passed as
 *          argument to it. After successful return of this function no other
 *          thread can enter until this thread exit CS by calling SYNC_LeaveCS
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_EnterCS (IN SyncCsObject * cSObj)
{
    DSP_STATUS   status   = DSP_SOK    ;

    TRC_1ENTER ("SYNC_EnterCS", cSObj) ;

    DBC_Require (cSObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (cSObj, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (cSObj, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        if (down_interruptible (&(cSObj->sem))) {
            /* Premature ending of sleep */
            status = SYNC_E_FAIL ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SYNC_EnterCS", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_LeaveCS
 *
 *  @desc   This function makes critical section available for other threads
 *          to enter.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_LeaveCS (IN SyncCsObject * cSObj)
{
    DSP_STATUS   status   = DSP_SOK    ;

    TRC_1ENTER ("SYNC_LeaveCS", cSObj) ;

    DBC_Require (cSObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (cSObj, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (cSObj, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        up (&(cSObj->sem)) ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SYNC_LeaveCS", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_CreateSEM
 *
 *  @desc   Creates the semaphore object.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_CreateSEM (OUT SyncSemObject ** semObj, IN SyncAttrs * attr)
{
    DSP_STATUS   status   = DSP_SOK ;

    TRC_2ENTER ("SYNC_CreateSEM", semObj, attr) ;

    DBC_Require (semObj != NULL) ;
    DBC_Require (attr != NULL) ;

    if ((semObj == NULL) || (attr == NULL)) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (   ((SyncSemType)(attr->flag) != SyncSemType_Binary)
             && ((SyncSemType)(attr->flag) != SyncSemType_Counting))  {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        status =  MEM_Calloc ((Void **) semObj, sizeof (SyncSemObject),
                              MEM_DEFAULT) ;

        if (DSP_SUCCEEDED (status)) {
            sema_init(&((*semObj)->sem), 0) ;
            (*semObj)->semType = (SyncSemType)(attr->flag) ;
            if ((*semObj)->semType == SyncSemType_Binary) {
                (*semObj)->isSemAvailable = FALSE ;
            }
            (*semObj)->signature = SIGN_SYNC ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    if ((DSP_FAILED (status)) && (semObj != NULL)) {
        *semObj = NULL ;
    }

    DBC_Ensure (   ((semObj == NULL) && DSP_FAILED (status))
                 || ((*semObj == NULL) && DSP_FAILED (status))
                 || (   ((*semObj)->signature == SIGN_SYNC)
                     && DSP_SUCCEEDED (status))) ;

    TRC_1LEAVE ("SYNC_CreateSEM", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_DeleteSEM
 *
 *  @desc   Deletes the semaphore object.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_DeleteSEM (IN SyncSemObject * semObj)
{
    DSP_STATUS   status   = DSP_SOK ;

    TRC_1ENTER ("SYNC_DeleteSEM", semObj) ;

    DBC_Require (semObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (semObj, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (semObj, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        semObj->signature = SIGN_NULL ;
        status = FREE_PTR (semObj) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SYNC_DeleteSEM", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_WaitSEM
 *
 *  @desc   This function waits on the semaphore.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_WaitSEM (IN SyncSemObject * semObj, IN Uint32  timeout)
{
    DSP_STATUS        status         = DSP_SOK ;
    Bool              timerCreated   = FALSE   ;
    Bool              semAvailable   = FALSE   ;
    struct timer_list timer                    ;
    Uint32            irqFlags                 ;

    TRC_2ENTER ("SYNC_WaitSEM", semObj, timeout) ;

    DBC_Require (semObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (semObj, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (semObj, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        semObj->timeoutOccurred = FALSE ;
        /*  Put up a timer in case of a finite non zero timeout. This will
         *  invoke a function at specified time to give the semaphore and do
         *  timeout on the semaphore.
         */
        if ((timeout != SYNC_WAITFOREVER) && (timeout != SYNC_NOWAIT)) {
            init_timer (&timer) ;
            /* jiffies represent current internal timer count. */
            timer.expires = jiffies + (timeout * HZ)/1000 ;
            timer.data = (unsigned long) semObj ;
            timer.function = SYNC_SEM_TimeoutHandler ;
            add_timer (&timer) ;
            timerCreated = TRUE ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        if (semObj->semType == SyncSemType_Binary) {
            irqFlags = SYNC_SpinLockStart () ;
            if (semObj->isSemAvailable == TRUE) {
                semObj->isSemAvailable = FALSE ;
                semAvailable = TRUE ;
                /* Make sure semaphore is still in reset state */
                down_trylock (&(semObj->sem)) ;
            }
            SYNC_SpinLockEnd (irqFlags) ;

            if ((semAvailable == TRUE) && (timerCreated == TRUE)) {
                 del_timer_sync (&timer) ;
            }
        }

        while ((DSP_SUCCEEDED (status)) && (semAvailable != TRUE)) {
            if (timeout != SYNC_NOWAIT) {
                if (down_interruptible (&(semObj->sem))) {
                    /*
                     *  Sleep has been broken prematurely: Results are
                     *  undefined in this error condition.
                     */
                    TRC_0PRINT (TRC_LEVEL7,
                                "Interrupted while waiting on semaphore\n") ;
                    if (timerCreated == TRUE) {
                        /*  We don't care about the return value it only
                         *  tells that if the timer was queued or not
                         *  at the time of deletion
                         */
                        del_timer_sync (&timer) ;
                    }
                    status = SYNC_E_FAIL ;
                    SET_FAILURE_REASON ;
                }
                else {
                    if (timerCreated == TRUE) {
                        /*  We don't care about the return value it only
                         *  tells that if the timer was queued or not
                         *  at the time of deletion
                         */
                        del_timer_sync (&timer) ;
                        if (semObj->timeoutOccurred) {
                            status = DSP_ETIMEOUT ;
                        }
                    }

                    if (DSP_SUCCEEDED (status)) {
                        if (semObj->semType == SyncSemType_Binary) {
                            irqFlags = SYNC_SpinLockStart () ;
                            if (semObj->isSemAvailable == TRUE) {
                                semObj->isSemAvailable = FALSE ;
                                semAvailable = TRUE ;
                            }
                            SYNC_SpinLockEnd (irqFlags) ;
                        }
                        else {
                            semAvailable = TRUE ;
                        }
                    }
                }
            }
            else {
                /*  For zero timeout just call down_trylock to try to get the
                 *  semaphore without waiting
                 */
                if (down_trylock (&(semObj->sem))) {
                    status = SYNC_E_FAIL ;
                    SET_FAILURE_REASON ;
                }
                else {
                    if (semObj->semType == SyncSemType_Binary) {
                        irqFlags = SYNC_SpinLockStart () ;
                        if (semObj->isSemAvailable == TRUE) {
                            semObj->isSemAvailable = FALSE ;
                            semAvailable = TRUE ;
                        }
                        SYNC_SpinLockEnd (irqFlags) ;
                    }
                    else {
                        semAvailable = TRUE ;
                    }
                }
            }
        }
    }

    /*  In this case semaphore is given by the timeout handler. Set the status
     *  to DSP_ETIMEOUT.
     */
    if (DSP_SUCCEEDED (status) && (semObj->timeoutOccurred)) {
        status = DSP_ETIMEOUT ;
    }

    TRC_1LEAVE ("SYNC_WaitSEM", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_SignalSEM
 *
 *  @desc   This function signals the semaphore and makes it available for other
 *          threads.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
SYNC_SignalSEM (IN SyncSemObject * semObj)
{
    DSP_STATUS   status   = DSP_SOK    ;
    Uint32       irqFlags              ;

    TRC_1ENTER ("SYNC_SignalSEM", semObj) ;

    DBC_Require (semObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (semObj, SIGN_SYNC)) ;

    status = (IS_OBJECT_VALID (semObj, SIGN_SYNC) ? DSP_SOK : DSP_EPOINTER) ;

    if (DSP_SUCCEEDED (status)) {
        if (semObj->semType == SyncSemType_Counting) {
            up (&(semObj->sem)) ;
        }
        else {
            irqFlags = SYNC_SpinLockStart () ;
            if (semObj->isSemAvailable != TRUE) {
                semObj->isSemAvailable = TRUE ;
                /* Now make binary semaphore 1 */
                up (&(semObj->sem)) ;
            }
            SYNC_SpinLockEnd (irqFlags) ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("SYNC_SignalSEM", status) ;

    return status ;
}


/** ============================================================================
 *  @func   SYNC_ProtectionStart
 *
 *  @desc   Marks the start of protected code execution.
 *          DSP/BIOS Link implements DPC using tasklets.
 *          This function acheives protection by disabling DPCs.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
SYNC_ProtectionStart ()
{
    DSP_STATUS status = DSP_SOK ;
    TRC_0ENTER ("SYNC_ProtectionStart") ;

    status = SYNC_EnterCS (SYNC_ProtectCsObj) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    DPC_Disable () ;

    TRC_0LEAVE ("SYNC_ProtectionStart") ;
}


/** ============================================================================
 *  @func   SYNC_ProtectionEnd
 *
 *  @desc   Marks the end of protected code execution.
 *          DSP/BIOS Link implements DPC using tasklets.
 *          This function enables DPCs.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
SYNC_ProtectionEnd ()
{
    DSP_STATUS status = DSP_SOK ;
    TRC_0ENTER ("SYNC_ProtectionEnd") ;

    DPC_Enable () ;
    status = SYNC_LeaveCS (SYNC_ProtectCsObj) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_0LEAVE ("SYNC_ProtectionEnd") ;
}


/** ============================================================================
 *  @func   SYNC_SpinLockStart
 *
 *  @desc   Begin protection of code through spin lock with all ISRs disabled.
 *          Calling this API protects critical regions of code from preemption
 *          by tasks, DPCs and all interrupts.
 *          This API can be called from DPC context.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Uint32
SYNC_SpinLockStart ()
{
    Uint32 irqFlags ;

    TRC_0ENTER ("SYNC_SpinLockStart") ;

    spin_lock_irqsave (&SYNC_PreemptLock, irqFlags) ;

    TRC_1LEAVE ("SYNC_SpinLockStart", irqFlags) ;

    return irqFlags ;
}


/** ============================================================================
 *  @func   SYNC_SpinLockEnd
 *
 *  @desc   End protection of code through spin lock with all ISRs disabled.
 *          This API can be called from DPC context.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
SYNC_SpinLockEnd (Uint32 irqFlags)
{
    TRC_0ENTER ("SYNC_SpinLockEnd") ;

    spin_unlock_irqrestore (&SYNC_PreemptLock, irqFlags) ;

    TRC_0LEAVE ("SYNC_SpinLockEnd") ;
}


/** ----------------------------------------------------------------------------
 *  @func   SYNC_TimeoutHandler
 *
 *  @desc   Handler function for timeout.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
void
SYNC_TimeoutHandler (unsigned long eventPtr)
{
    SyncEvObject * event = (SyncEvObject *) eventPtr ;

    event->timeoutOccurred = TRUE ;
    down_trylock (&(event->eventSem)) ;
    up (&(event->eventSem)) ;
}


/** ----------------------------------------------------------------------------
 *  @func   SYNC_SEM_TimeoutHandler
 *
 *  @desc   Handler function for timeout.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
void
SYNC_SEM_TimeoutHandler (unsigned long semPtr)
{
    SyncSemObject * semObj = (SyncSemObject *) semPtr ;

    semObj->timeoutOccurred = TRUE ;
    down_trylock (&(semObj->sem)) ;
    up (&(semObj->sem)) ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
