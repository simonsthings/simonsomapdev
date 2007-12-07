/** ============================================================================
 *  @file   BVR_MultiProcessAttach.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Interface definition of behavior test to open DSP from
 *          multiple processes.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BVR_MULTIPROCESSATTACH_H)
#define BVR_MULTIPROCESSATTACH_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @func   BVR_MultiProcessAttach
 *
 *  @desc   Function to test the behavior when the DSP is attached
 *          from multiple processes.
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
 *  @enter  argc must be 4.
 *          argv must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_MultiProcessAttach (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (BVR_MULTIPROCESSATTACH) */
