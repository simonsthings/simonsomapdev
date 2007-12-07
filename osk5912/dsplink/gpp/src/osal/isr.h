/** ============================================================================
 *  @file   isr.h
 *
 *  @path   $(DSPLINK)\gpp\src\osal
 *
 *  @desc   Defines the interfaces and data structures for the
 *          sub-component ISR.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (ISR_H)
#define ISR_H


USES (gpptypes.h)
USES (errbase.h)
USES (intobject.h)

#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @name   ISR_State
 *
 *  @desc   Enumerates the various states of ISR.
 *
 *  @field  ISR_Installed
 *              Indicates that the ISR is installed.
 *  @field  ISR_Uninstalled
 *              Indicates that the ISR is uninstalled.
 *  @field  ISR_Disabled
 *              Indicates that the ISR is disabled.
 *  @field  ISR_Enabled
 *              Indicates that the ISR is enabled.
 *  ============================================================================
 */
typedef enum {
    ISR_Installed   = 0,
    ISR_Uninstalled = 1,
    ISR_Disabled    = 2,
    ISR_Enabled     = 3
} ISR_State ;


/** ============================================================================
 *  @name   IsrObject
 *
 *  @desc   Forward declaration for IsrObject, actual definition is
 *          OS dependent.
 *  ============================================================================
 */
typedef struct IsrObject_tag IsrObject ;

/** ============================================================================
 *  @name   IsrProc
 *
 *  @desc   Function prototype for an ISR. The user defined function
 *          to be invoked as an ISR should conform to this signature.
 *
 *  @arg    refData
 *             Data to be passed to ISR when invoked.
 *
 *  @ret    None
 *  ============================================================================
 */
typedef Void (*IsrProc) (Pvoid refData) ;


/** ============================================================================
 *  @func   ISR_Initialize
 *
 *  @desc   Initializes and allocates resources used by ISR subcomponent.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory.
 *
 *  @enter  None
 *
 *  @leave  ISR must be initialized.
 *
 *  @see    ISR_Install, ISR_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Initialize () ;


/** ============================================================================
 *  @func   ISR_Finalize
 *
 *  @desc   Releases resources reserved for ISR subcomponent.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory.
 *
 *  @enter  ISR must be initialized.
 *
 *  @leave  None
 *
 *  @see    ISR_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Finalize () ;


/** ============================================================================
 *  @func   ISR_Create
 *
 *  @desc   Creates an ISR object.
 *
 *  @arg    fnISR
 *              User defined interrupt service routine.
 *  @arg    refData
 *              Argument to be passed to ISR when it is invoked.
 *  @arg    intObj
 *              Interrupt information (OS and hardware dependent).
 *  @arg    isrObj
 *              Out argument for IsrObject.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments were passed to function.
 *          DSP_EMEMORY
 *              Out of memory error.
 *
 *  @enter  ISR must be initialized.
 *          isrObj must be valid pointer.
 *          intObj must be a valid pointer.
 *          fnISR must be a valid function pointer.
 *
 *  @leave  A valid IsrObject is returned on success.
 *
 *  @see    ISR_Delete
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Create (IN  IsrProc           fnISR,
            IN  Pvoid             refData,
            IN  InterruptObject * intObj,
            OUT IsrObject **      isrObj) ;


/** ============================================================================
 *  @func   ISR_Delete
 *
 *  @desc   Deletes the isrObject.
 *
 *  @arg    isrObj
 *              Object to be deleted.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid isrObj object.
 *          DSP_EMEMORY
 *              Free memory error.
 *          DSP_EACCESSDENIED
 *              isrObj not uninstalled.
 *
 *  @enter  ISR must be initialized.
 *          isrObj must be a valid object.
 *
 *  @leave  None
 *
 *  @see    ISR_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Delete (IN IsrObject * isrObj) ;


/** ============================================================================
 *  @func   ISR_Install
 *
 *  @desc   Installs an interrupt service routine defined by isrObj.
 *
 *  @arg    hostConfig
 *              Void pointer containing installation information related
 *              to installation of an ISR.
 *  @arg    isrObj
 *              The isrObj object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid isrObj object.
 *          DSP_EACCESSDENIED
 *              An ISR is already installed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  ISR must be initialized.
 *          isrObj must be valid.
 *
 *  @leave  The isr is installed on success.
 *          isrObj contains a valid IsrObject.
 *
 *  @see    ISR_Func, ISR_Uninstall
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Install (IN  Void *       hostConfig,
             IN  IsrObject *  isrObj) ;


/** ============================================================================
 *  @func   ISR_Uninstall
 *
 *  @desc   Uninstalls the interrupt service routine defined by isrObj.
 *
 *  @arg    isrObj
 *             ISR object to uninstall.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid isrObj object.
 *          DSP_EACCESSDENIED
 *              ISR is already uninstalled.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  ISR must be initialized.
 *          isrObj must be a valid IsrObject.
 *
 *  @leave  None
 *
 *  @see    ISR_Install
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Uninstall (IN IsrObject * isrObj) ;


/** ============================================================================
 *  @func   ISR_Disable
 *
 *  @desc   Disables an ISR associated with interrupt Id of isrObject.
 *
 *  @arg    isrObj
 *              ISR object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EACCESSDENIED
 *              ISR is not installed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  ISR must be initialized.
 *          isrObj must be a valid object.
 *
 *  @leave  None
 *
 *  @see    ISR_Enable, ISR_Install
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Disable (IN  IsrObject * isrObj) ;


/** ============================================================================
 *  @func   ISR_Enable
 *
 *  @desc   Reactivates ISRs based on the specified flags argument. The flags
 *          argument must be obtained with an earlier call to ISR_Disable.
 *
 *  @arg    isrObj
 *              ISR object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EACCESSDENIED
 *              ISR is not installed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  ISR must be initialized.
 *          isrObj must be a valid object.
 *
 *  @leave  None
 *
 *  @see    ISR_Disable
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_Enable (IN  IsrObject * isrObj) ;


/** ============================================================================
 *  @func   ISR_GetState
 *
 *  @desc   Gets the state of an ISR.
 *
 *  @arg    isrObj
 *              The ISR object.
 *  @arg    isrState
 *              Current status of the ISR.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPOINTER
 *              Invalid isrObj object.
 *          DSP_EINVALIDARG
 *              Invalid isrStatus pointer.
 *
 *  @enter  isrObj must be a valid IsrObject.
 *          isrStatus must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    ISR_Install, ISR_Uninstall, ISR_Enable, ISR_Disable
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ISR_GetState (IN  IsrObject *  isrObj,
              OUT ISR_State *  isrState) ;


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   ISR_Debug
 *
 *  @desc   Prints the current status of ISR objects.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
ISR_Debug () ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (ISR_H) */
