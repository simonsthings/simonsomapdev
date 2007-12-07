/** ============================================================================
 *  @file   TST_Initialize.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Defines the interface of TST_Initialize and TST_Finalize function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_INITIALIZE_H)
#define TST_INITIALIZE_H


USES (errbase.h)
USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_Initialize
 *
 *  @desc   Initialization function to Testsuite.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed to Initialize framework.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_Initialize () ;


/** ============================================================================
 *  @func   TST_Finalize
 *
 *  @desc   Finalization function to Testsuite.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed to Finalize framework.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_Finalize () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (TST_INITIALIZE_H) */
