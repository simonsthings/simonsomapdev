/** ============================================================================
 *  @file   ANA_SyncChnlIssueReclaim.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\analysis
 *
 *  @desc   Defines the interface of a test case which tests the Channel issue
 *          reclaim performance synchronously.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (ANA_SYNCCHNLISSUERECLAIM_H)
#define ANA_SYNCCHNLISSUERECLAIM_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   ANA_SyncChnlIssueReclaim
 *
 *  @desc   Entry point for the ANA_SyncChnlIssueReclaim Test.
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
ANA_SyncChnlIssueReclaim (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (ANA_SYNCCHNLISSUERECLAIM_H) */
