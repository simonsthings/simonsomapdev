/** ============================================================================
 *  @file   TST_Mem.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils\Linux
 *
 *  @desc   Defines the interfaces of memory allocation and deallocation
 *          Functions used by framework and test suites.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_MEM_H)
#define TST_MEM_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_Alloc
 *
 *  @desc   This function allocates memory for test cases and framework.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
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
DSP_STATUS
TST_Alloc (Void ** pointer, Uint32 size) ;


/** ============================================================================
 *  @func   TST_Free
 *
 *  @desc   This function deallocates memory allocated by test cases and
 *          framework.
 *
 *  @arg    format
 *              The string which is to be printed
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
TST_Free (Void ** pointer) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_MEM_H) */
