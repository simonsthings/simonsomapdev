/** ============================================================================
 *  @file   osal.h
 *
 *  @path   $(DSPLINK)\gpp\src\osal
 *
 *  @desc   Defines the interfaces for initializing and finalizing OSAL.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (OSAL_H)
#define OSAL_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @func   OSAL_Initialize
 *
 *  @desc   Initializes the OS Adaptation layer.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    OSAL_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_Initialize () ;


/** ============================================================================
 *  @func   OSAL_Finalize
 *
 *  @desc   Releases OS adaptation layer resources indicating that they would
 *          no longer be used.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Out of memory error.
 *          DSP_EFAIL
 *              General error from GPP-OS.
 *
 *  @enter  Subcomponent must be initialized.
 *
 *  @leave  None
 *
 *  @see    OSAL_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_Finalize () ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (OSAL_H) */
