/** ============================================================================
 *  @file   TST_Print.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Defines the interface of TST_Print Function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_PRINT_H)
#define TST_PRINT_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */

#if defined (UTILS_KERNEL)

/** ============================================================================
 *  @macro  TST_PRINT
 *
 *  @desc   Defines user-kernel independent macro for printing.
 *  ============================================================================
 */
#define TST_PRINT printk

#else /* defined (UTILS_KERNEL) */

/** ============================================================================
 *  @macro  TST_PRINT
 *
 *  @desc   Defines user-kernel independent macro for printing.
 *  ============================================================================
 */
#define TST_PRINT TST_Print

/** ============================================================================
 *  @func   TST_Print
 *
 *  @desc   This function provides standard printf functionality abstraction.
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
TST_Print (Char8 * format, ...) ;

#endif /* defined (UTILS_KERNEL) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_PRINT_H) */
