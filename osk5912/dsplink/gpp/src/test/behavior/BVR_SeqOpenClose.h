/** ============================================================================
 *  @file   BVR_SeqOpenClose.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Interface definition of behavior test to check CHNL behavior with
 *          sequence of multiple opens and closes.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BVR_SEQOPENCLOSE_H)
#define BVR_SEQOPENCLOSE_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @func   BVR_SeqOpenClose
 *
 *  @desc   Function to test the behavior when CHNL_Open and CHNL_Close is
 *          closed multiple times.
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
 *  @enter  argc must be 3.
 *          argv must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_SeqOpenClose (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (BVR_SEQOPENCLOSE_H) */
