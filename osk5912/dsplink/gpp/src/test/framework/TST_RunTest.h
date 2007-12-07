/** ============================================================================
 *  @file   TST_RunTest.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Defines the interface of TST_RunTest Function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_RUNTEST_H)
#define TST_RUNTEST_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_RunTest
 *
 *  @desc   This function runs the testcase.
 *
 *  @arg    argc
 *              Count of the no. of arguments passed.
 *  @arg    argv
 *              List of arguments.
 *  @arg    funcPtr
 *              Pointer to the test case to be invoked.
 *  @arg    testName
 *              Pointer to structure containing names of testsuites
 *              and testcase.
 *  @arg    attrs
 *              Pointer to structure of the attributes to be passed to
 *              the framework.
 *  @arg    testState
 *              Defines the state of the test currently executing.
 *  @arg    frameworkResult
 *              Pointer to result structure.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_INVALIDARG
 *              Wrong no of arguments passed.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_RunTest (IN Uint32         argc,
             IN Char8 **       argv,
             IN PtrToTestCase  funcPtr,
             IN TestInfo *     testName,
             IN TST_Attrs *    attrs,
             IN TST_State      testState,
             IN Result *       frameworkResult) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_RUNTEST_H) */
