/** ============================================================================
 *  @file   BVR_ProcStart.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\bvr
 *
 *  @desc   Defines the interface of BVR_ProcStart test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BVR_PROCSTART_H)
#define BVR_PROCSTART_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   BVR_ProcStart
 *
 *  @desc   Entry point for the PROC_Start  BVR.
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
 *
 *  @enter  argc should be 5.
 *          argv should not be null.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_ProcStart (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (BVR_PROCSTART_H) */
