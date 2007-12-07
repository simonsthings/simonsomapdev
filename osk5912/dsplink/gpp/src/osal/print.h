/** ============================================================================
 *  @file   print.h
 *
 *  @path   $(DSPLINK)\gpp\src\osal
 *
 *  @desc   Interface declaration of OS printf abstraction.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)
USES (errbase.h)


/** ============================================================================
 *  @func   PRINT_Initialize
 *
 *  @desc   Initializes the PRINT sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
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
PRINT_Initialize () ;


/** ============================================================================
 *  @func   PRINT_Finalize
 *
 *  @desc   Releases resources used by this sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error from GPP-OS.
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
PRINT_Finalize () ;


/** ============================================================================
 *  @func   PRINT_Printf
 *
 *  @desc   Provides standard printf functionality abstraction.
 *
 *  @arg    format
 *              Format string.
 *  @arg    ...
 *              Variable argument list.
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
#if defined (TRACE_KERNEL)
EXPORT_API
Void
PRINT_Printf (Pstr format, ...) ;
#endif

#if defined (TRACE_USER)
/*  ----------------------------------------------------------------------------
 *  Extern declaration for printf to avoid compiler warning.
 *  ----------------------------------------------------------------------------
 */
EXTERN int printf (const char * format, ...) ;

#define PRINT_Printf printf
#endif


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
