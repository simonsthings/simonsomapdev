/** ============================================================================
 *  @file   isr.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implementation of sub-component ISR.
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
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/irq.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <intobject.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <mem.h>
#include <isr.h>
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
#define COMPONENT_ID        ID_OSAL_ISR

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  GEN_SetReason (status, FID_C_OSAL_ISR, __LINE__)

/** ============================================================================
 *  @const  MAX_ISR
 *
 *  @desc   Maximum number of ISRs supported.
 *  ============================================================================
 */
#define MAX_ISR             127


/** ============================================================================
 *  @name   IsrObject_tag
 *
 *  @desc   Defines object to encapsulate the interrupt service routine.
 *          The definition is OS/platform specific.
 *
 *  @field  signature
 *              Signature to identify this object.
 *  @field  refData
 *              Data to be passed to ISR on invocation.
 *  @field  fnISR
 *              Actual Interrupt service routine.
 *  @field  irq
 *              The IRQ number.
 *  @field  enabled
 *              Flag to indicate the ISR is enabled.
 *  ============================================================================
 */
struct IsrObject_tag {
    Uint32   signature ;
    Pvoid    refData   ;
    IsrProc  fnISR     ;
    int      irq       ;
    Bool     enabled   ;
} ;


/** ----------------------------------------------------------------------------
 *  @func   ISR_Callback
 *
 *  @desc   This function is registered as an interrupt handler for all the
 *          irqs on which user wants to register an ISR.
 *          The Linux kernel calls this function when an interrupt occurs on
 *          the specified irq. This function, then, looks up the irq and
 *          invokes the user specified interrupt service routine.
 *
 *  @arg    irq
 *              vector on which the interrupt occurred
 *  @arg    arg
 *              Device object - contains the user defined object that was used
 *              when handler was registered.
 *  @arg    flags
 *              Flags indicating registers to be saved (ignored).
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    ISR_Install
 *  ----------------------------------------------------------------------------
 */
STATIC
void
ISR_Callback (int irq, void * arg, struct pt_regs * flags) ;


/*  ============================================================================
 *  @name   ISR_IsInitialized
 *
 *  @desc   Flag for tracking initialization of subcomponent.
 *  ============================================================================
 */
STATIC Bool        ISR_IsInitialized = FALSE ;

/*  ============================================================================
 *  @name   ISR_InstalledIsrs
 *
 *  @desc   Array to hold all the installed  isrObjects.
 *  ============================================================================
 */
STATIC IsrObject * ISR_InstalledIsrs [MAX_ISR] ;


/** ============================================================================
 *  @func   ISR_Initialize
 *
 *  @desc   Initialize the module and allocate resource used by this module.
 *
 *  @modif  ISR_InstalledIsrs, ISR_IsInitialized
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Initialize ()
{
    DSP_STATUS  status = DSP_SOK ;
    Uint32      index  = 0       ;

    TRC_0ENTER ("Entered ISR_Initialize ()\n") ;

    DBC_Require (ISR_IsInitialized == FALSE) ;

    if (ISR_IsInitialized == FALSE) {
        for (index = 0 ; index < MAX_ISR ; index ++) {
            ISR_InstalledIsrs [index] = NULL ;
        }
        ISR_IsInitialized = TRUE ;
    }

    DBC_Ensure (ISR_IsInitialized == TRUE) ;

    TRC_1LEAVE ("ISR_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_Finalize
 *
 *  @desc   This function provides an interface to exit from the ISR module.
 *          It frees up all the used ISRs and releases all the resources used by
 *          this module.
 *
 *  @modif  ISR_InstalledIsrs, ISR_IsInitialized
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Finalize ()
{
    DSP_STATUS  status = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;
    Uint32      index  = 0       ;

    TRC_0ENTER ("ISR_Finalize") ;

    DBC_Require (ISR_IsInitialized == TRUE) ;

    if (ISR_IsInitialized == TRUE) {
        for (index  = 0 ; index < MAX_ISR ; index ++) {
            if (ISR_InstalledIsrs [index] != NULL) {
                tmpStatus = ISR_Uninstall (ISR_InstalledIsrs [index]) ;
                if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                    status = tmpStatus ;
                }
            }
        }
        ISR_IsInitialized = FALSE ;
    }

    DBC_Ensure (ISR_IsInitialized == FALSE) ;

    TRC_1LEAVE ("ISR_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_Create
 *
 *  @desc   Creates an ISR object.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Create (IN  IsrProc             fnISR,
            IN  Pvoid               refData,
            IN  InterruptObject *   intObj,
            OUT IsrObject **        isrObj)
{
    DSP_STATUS   status   = DSP_SOK ;

    TRC_4ENTER ("ISR_Create", fnISR, refData, intObj, isrObj) ;

    DBC_Require (ISR_IsInitialized == TRUE) ;
    DBC_Require (fnISR != NULL)  ;
    DBC_Require (isrObj != NULL) ;
    DBC_Require (intObj != NULL) ;
    DBC_Require (   (intObj != NULL)
                 && (intObj->intId >= 0)
                 && (intObj->intId < MAX_ISR)) ;

    if (   (fnISR  == NULL)
        || (intObj == NULL)
        || (isrObj == NULL)
        || (intObj->intId < 0)
        || (intObj->intId >= MAX_ISR)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        status = MEM_Alloc ((Void **) isrObj,
                            sizeof (IsrObject),
                            MEM_DEFAULT) ;

        if (DSP_SUCCEEDED (status)) {
            (*isrObj)->signature = SIGN_ISR       ;
            (*isrObj)->fnISR     = fnISR          ;
            (*isrObj)->irq       = intObj->intId ;
            (*isrObj)->refData   = refData        ;
            (*isrObj)->enabled   = FALSE          ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }


    DBC_Ensure (  (   DSP_SUCCEEDED (status)
                   && IS_OBJECT_VALID (*isrObj, SIGN_ISR))
                || DSP_FAILED (status)) ;

    TRC_1LEAVE ("ISR_Create", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_Delete
 *
 *  @desc   Delete the isrObject.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Delete (IN IsrObject * isrObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("ISR_Delete", isrObj) ;

    DBC_Require (ISR_IsInitialized == TRUE)          ;
    DBC_Require (isrObj != NULL)                     ;
    DBC_Require (IS_OBJECT_VALID (isrObj, SIGN_ISR)) ;

    if (IS_OBJECT_VALID (isrObj, SIGN_ISR) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Verify that the ISR is not installed before deleting the object
         *  --------------------------------------------------------------------
         */
        if (ISR_InstalledIsrs [isrObj->irq] != isrObj) {
            isrObj->signature = SIGN_NULL ;
            status = FREE_PTR (isrObj) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_EACCESSDENIED ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("ISR_Delete", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_Install
 *
 *  @desc   Install an interrupt service routine.
 *          This function calls the request_irq () function and installs
 *          the specified interrupt service routine in non-shared, non-fiq
 *          mode.
 *
 *  @modif  ISR_InstalledIsrs
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Install (IN  Void *      hostConfig,
             IN  IsrObject * isrObj)
{
    DSP_STATUS   status   = DSP_SOK ;
    int          osStatus = 0       ;

    TRC_2ENTER ("ISR_Install", hostConfig, isrObj) ;

    DBC_Require (ISR_IsInitialized == TRUE) ;
    DBC_Require (isrObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (isrObj, SIGN_ISR)) ;
    DBC_Require (   (isrObj != NULL)
                 && (ISR_InstalledIsrs [isrObj->irq] == NULL)) ;

    if (IS_OBJECT_VALID (isrObj, SIGN_ISR) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (ISR_InstalledIsrs [isrObj->irq] != NULL) {
        status = DSP_EACCESSDENIED ;
        SET_FAILURE_REASON ;
    }
    else {
        osStatus = request_irq (isrObj->irq,
                                ISR_Callback,
                                0,                  /* non shared, non-fiq */
                                "DSP/BIOS LINK",
                                isrObj) ;

        if (osStatus != 0) {
            TRC_1PRINT (TRC_LEVEL7, "request_irq failed with error: %d\n",
                        osStatus) ;
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else {
            /*
             *  Maintain the installed ISR object pointer locally for cleanup
             *  on ISR_Finalize.
             */
            ISR_InstalledIsrs [isrObj->irq] = isrObj ;

            /*
             *  The ISR is enabled by default upon install so indicate that in
             *  local state.
             */
            isrObj->enabled = TRUE ;
        }
    }

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (ISR_InstalledIsrs [isrObj->irq] == isrObj))
                || DSP_FAILED (status)) ;

    TRC_1LEAVE ("ISR_Install", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_Uninstall
 *
 *  @desc   Uninstalls an ISR.
 *
 *  @modif  ISR_InstalledIsrs
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Uninstall (IN IsrObject * isrObj)
{
    DSP_STATUS      status   = DSP_SOK ;

    TRC_1ENTER ("ISR_Uninstall", isrObj) ;

    DBC_Require (ISR_IsInitialized == TRUE) ;
    DBC_Require (isrObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (isrObj, SIGN_ISR)) ;
    DBC_Require (   (isrObj!= NULL)
                 && (ISR_InstalledIsrs [isrObj->irq] == isrObj)) ;

    if (IS_OBJECT_VALID (isrObj, SIGN_ISR) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (ISR_InstalledIsrs [isrObj->irq] != isrObj) {
            status = DSP_EACCESSDENIED ;
            SET_FAILURE_REASON ;
    }
    else {
        if (isrObj->enabled == TRUE) {
            status = ISR_Disable (isrObj) ;
        }

        if (DSP_SUCCEEDED (status)) {
            free_irq (isrObj->irq, isrObj) ;
            ISR_InstalledIsrs [isrObj->irq] = NULL ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (ISR_InstalledIsrs [isrObj->irq] == NULL))
                || DSP_FAILED (status)) ;

    TRC_1LEAVE ("ISR_Uninstall", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_Disable
 *
 *  @desc   Disables the specified ISR.
 *          This function calls disable_irq () to disable the ISR.
 *          disble_irq() function doesn't return any value so this function
 *          assumes it was successful.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Disable (IN  IsrObject * isrObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("ISR_Disable", isrObj) ;

    DBC_Require (ISR_IsInitialized == TRUE) ;
    DBC_Require (isrObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (isrObj, SIGN_ISR)) ;
    DBC_Require (   (isrObj != NULL)
                 && (ISR_InstalledIsrs [isrObj->irq] == isrObj)) ;

    if (IS_OBJECT_VALID (isrObj, SIGN_ISR) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (ISR_InstalledIsrs [isrObj->irq] != isrObj) {
            status = DSP_EACCESSDENIED ;
            SET_FAILURE_REASON ;
    }
    else {
        disable_irq (isrObj->irq) ;
        isrObj->enabled = FALSE ;
    }

    TRC_1LEAVE ("ISR_Disable", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_Enable
 *
 *  @desc   Enables the specified ISR.
 *          This function calls enable_irq () to enable the ISR.
 *          enble_irq() function doesn't return any value so this function
 *          assumes it was successful.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Enable (IN  IsrObject * isrObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("ISR_Enable", isrObj) ;

    DBC_Require (ISR_IsInitialized == TRUE) ;
    DBC_Require (isrObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (isrObj, SIGN_ISR)) ;
    DBC_Require (   (isrObj != NULL)
                 && (ISR_InstalledIsrs [isrObj->irq] == isrObj)) ;

    if (IS_OBJECT_VALID (isrObj, SIGN_ISR) == FALSE) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (ISR_InstalledIsrs [isrObj->irq] != isrObj) {
            status = DSP_EACCESSDENIED ;
            SET_FAILURE_REASON ;
        }
    else {
        enable_irq (isrObj->irq) ;
        isrObj->enabled = TRUE ;
    }

    TRC_1LEAVE ("ISR_Enable", status) ;

    return status ;
}


/** ============================================================================
 *  @func   ISR_GetState
 *
 *  @desc   Gets the state of an ISR.
 *          This function first checks if the ISR was installed using this
 *          subcomponent. If yes, then it checks the enabled flag of isrObject.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_GetState (IN  IsrObject *  isrObj,
              OUT ISR_State *  isrState)
{
    DSP_STATUS  status = DSP_SOK ;

    TRC_2ENTER ("ISR_GetState", isrObj, isrState) ;

    DBC_Require (ISR_IsInitialized == TRUE) ;
    DBC_Require (isrObj != NULL) ;
    DBC_Require (isrState != NULL) ;
    DBC_Require (IS_OBJECT_VALID (isrObj, SIGN_ISR)) ;

    if (IS_OBJECT_VALID (isrObj, SIGN_ISR) == FALSE)  {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (isrState == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    if (DSP_SUCCEEDED (status)) {
        *isrState = ISR_Uninstalled ;

            if (ISR_InstalledIsrs [isrObj->irq] == isrObj) {
                if (isrObj->enabled == TRUE) {
                    *isrState = ISR_Enabled ;
                }
                else {
                    *isrState = ISR_Disabled ;
                }
            }
        }

    TRC_1LEAVE ("ISR_GetState", status) ;

    return status ;
}


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   ISR_Debug
 *
 *  @desc   Prints the current status of ISR objects in the system.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
ISR_Debug ()
{
    Int32  i = 0 ;

    TRC_0ENTER ("ISR_Debug ()\n") ;

    DBC_Require (ISR_IsInitialized == TRUE) ;

    for (i = 0 ; i < MAX_ISR ; i++) {
        if (ISR_InstalledIsrs [i] != NULL) {
            TRC_1PRINT (TRC_LEVEL1, "ISR_InstalledIsrs [0x%x]\n", i) ;
            TRC_1PRINT (TRC_LEVEL1, "   signature:   [0x%x]\n",
                                    ISR_InstalledIsrs [i]->signature) ;
            TRC_1PRINT (TRC_LEVEL1, "   refData:     [0x%x]\n",
                                    ISR_InstalledIsrs [i]->refData) ;
            TRC_1PRINT (TRC_LEVEL1, "   fnISR:       [0x%x]\n",
                                    ISR_InstalledIsrs [i]->fnISR) ;
            TRC_1PRINT (TRC_LEVEL1, "   interruptId: [0x%x]\n",
                                    ISR_InstalledIsrs [i]->irq) ;
            TRC_1PRINT (TRC_LEVEL1, "   enabled:    [0x%x]\n",
                                    ISR_InstalledIsrs [i]->enabled) ;
        }
    }


    TRC_0PRINT (TRC_ENTER, "Leaving ISR_Debug ()\n") ;
}
#endif /* defined (DDSP_DEBUG) */


/** ----------------------------------------------------------------------------
 *  @func   ISR_Callback
 *
 *  @desc   Function invoked by OS whenever an interrupt occurs. This
 *          function is registered to handle all interrupts during calls
 *          to ISR_Install.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
void
ISR_Callback (int irq, void * arg, struct pt_regs * flags)
{
    DSP_STATUS   status = DSP_SOK ;
    IsrObject *  isrObj = (IsrObject *) (arg) ;

    if (IS_OBJECT_VALID (isrObj, SIGN_ISR) == TRUE) {
        (*isrObj->fnISR) (isrObj->refData) ;
    }
    else {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
