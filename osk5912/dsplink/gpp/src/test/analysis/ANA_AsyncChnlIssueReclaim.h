/** ============================================================================
 *  @file   ANA_AsyncChnlIssueReclaim.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\analysis
 *
 *  @desc   Defines the interface of a test case which tests the Channel issue 
 *          reclaim performance asynchronously
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (ANA_ASYNCCHNLISSUERECLAIM_H)
#define ANA_ASYNCCHNLISSUERECLAIM_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   ANA_AsyncChnlIssueReclaim
 *
 *  @desc   This test case tests the Channel issue  reclaim performance
 *          asynchronously
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
ANA_AsyncChnlIssueReclaim (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (ANA_ASYNCCHNLISSUERECLAIM_H) */
