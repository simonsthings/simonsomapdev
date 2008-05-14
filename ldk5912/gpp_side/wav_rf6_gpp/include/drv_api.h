/** ============================================================================
 *  @file   drv_api.h
 *
 *  @path   $(DSPLINK)/gpp/src/api/Linux/
 *
 *  @desc   User side driver wrapper interface.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DRV_API_H)
#define DRV_API_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   DRV_Object_tag
 *
 *  @desc   Forward declaration for the OS specific driver object type.
 *  ============================================================================
 */
typedef struct DRV_Object_tag DRV_Object ;


/** ============================================================================
 *  @name   DRV_Initialize
 *
 *  @desc   Initialization function for the driver.
 *
 *  @arg    drvObj
 *              pointer to driver object to be initialized.
 *  @arg    arg
 *              optional argument to be used for initialization.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  drvObj must be a valid pointer.
 *
 *  @leave  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DRV_Initialize (IN OUT DRV_Object ** drvObj, OPT IN OUT Pvoid arg) ;


/** ============================================================================
 *  @name   DRV_Finalize
 *
 *  @desc   Finalization function for the driver.
 *
 *  @arg    drvObj
 *              Pointer to driver object to be finalized.
 *  @arg    arg
 *              Optional argument to be used for finalization.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_SFINALIZED
 *              Operation successfully completed and the object is freed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  drvObj must be a valid pointer.
 *
 *  @leave  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DRV_Finalize (IN DRV_Object * drvObj, OPT IN OUT Pvoid arg) ;


/** ============================================================================
 *  @name   DRV_Invoke
 *
 *  @desc   Trap to underlying kernel side Linux LINK driver.
 *
 *  @arg    drvObj
 *              Pointer to driver.
 *  @arg    cmdId
 *              Command identifier.
 *  @arg    arg1
 *              Optional argument 1.
 *  @arg    arg2
 *              Optional argument 2.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  drvObj must be a valid pointer.
 *
 *  @leave  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DRV_Invoke (IN         DRV_Object * drvObj,
            IN         Uint32       cmdId,
            OPT IN OUT Pvoid        arg1,
            OPT IN OUT Pvoid        arg2) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !define (DRV_API_H) */
