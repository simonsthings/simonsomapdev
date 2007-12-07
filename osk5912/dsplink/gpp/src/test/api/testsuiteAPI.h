/** ============================================================================
 *  @file   testsuiteAPI.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Defines the interface of testsuiteAPI Function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TESTSUITEAPI_H)
#define TESTSUITEAPI_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TestSuite_API
 *
 *  @desc   This function decides which testcase to run.
 *
 *  @arg    argc
 *              Count of the no. of arguments passed
 *  @arg    argv
 *              List of arguments
 *  @arg    attrs
 *              Pointer to structure of the attributes to be passed to
 *              the framework.
 *  @arg    testState
 *              Defines the state of the test currently executing.
 *  @arg    frameworkResult
 *              Pointer to result structure passed by framework.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *          DSP_EMEMORY
 *              Operation failed due to insufficient memory.
 *          DSP_EPOINTER
 *              Invalid pointer passed.
 *
 *  @enter  argc should not be less than one
 *           argv should not be null
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TestSuite_Api (IN  Int32        argc,
               IN  Char8 **     argv,
               IN  TST_Attrs *  attrs,
               IN  TST_State    testState,
               OUT Result *     frameworkResult) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TESTSUITEAPI_H) */
