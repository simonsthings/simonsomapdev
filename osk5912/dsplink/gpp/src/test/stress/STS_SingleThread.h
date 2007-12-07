/** ============================================================================
 *  @file   STS_SingleThread.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress
 *
 *  @desc   Interface for single threaded stress test.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (STS_SINGLETHREAD_H)
#define  STS_SINGLETHREAD_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */

/** ============================================================================
 *  @func   STS_SingleThread
 *
 *  @desc   Stress tests main routine.
 *
 *  @arg    argc
 *              Count of the no. of arguments passed.
 *  @arg    argv
 *              List of arguments.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_INVALIDARG
 *              Wrong no of arguments passed.
 *          DSP_EFAIL
 *              Error in execution of test.
 *
 *  @enter  argc must be 7.
 *          argv must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
STS_SingleThread (IN Uint32 argc, IN Pstr * argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (STS_SINGLETHREAD_H) */
