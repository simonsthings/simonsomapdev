/** ============================================================================
 *  @file   BVR_DataRead.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Interface definition for behavior test for reading data from DSP.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BVR_DATAREAD_H)
#define BVR_DATAREAD_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @func   BVR_DataRead
 *
 *  @desc   Function to test the behavior of data read test.
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
BVR_DataRead (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (BVR_DATAREAD_H) */
