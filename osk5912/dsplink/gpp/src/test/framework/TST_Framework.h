/** ============================================================================
 *  @file   TST_Framework.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Defines the interface of Entry point to FrameWork function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_FRAMEWORK_H)
#define TST_FRAMEWORK_H


USES (errbase.h)
USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MAX_BUFSIZE
 *
 *  @desc   Maximum buffer size that is supported.
 *  ============================================================================
 */
#define MAX_BUFSIZE 128

/** ============================================================================
 *  @name   MAX_BUFFERS
 *
 *  @desc   Maximum number of buffers that are supported.
 *  ============================================================================
 */
#define MAX_BUFFERS 30

/** ============================================================================
 *  @name   TST_MAX_SRTLEN
 *
 *  @desc   Maximum length of string supported.
 *  ============================================================================
 */
#define TST_MAX_SRTLEN 32


/** ============================================================================
 *  @name   PtrToTestCase
 *
 *  @desc   Its Function signature to Test cases.
 *  ============================================================================
 */
typedef DSP_STATUS (*PtrToTestCase) (IN Uint32 argc, IN Char8 ** argv) ;


/** ============================================================================
 *  @name   ArrayPtr
 *
 *  @desc   Its signature of pointer to a 2D array.
 *  ============================================================================
 */
typedef Char8  (*ArrayPtr) [MAX_BUFFERS][MAX_BUFSIZE] ;


/** ============================================================================
 *  @name   TST_Attrs
 *
 *  @desc   The structure stores the attributes to be passed to the framework.
 *
 *  @field  reserved
 *             Reserved for future use.
 *  ============================================================================
 */
typedef struct TST_Attrs_tag {
    Uint32 reserved ;
} TST_Attrs ;


/** ============================================================================
 *  @name   Result
 *
 *  @desc   The structure stores the data pertaining to the test.
 *
 *  @field  testCount
 *              Count of tests to be run.
 *  @field  testRun
 *              Count of actual tests run.
 *  @field  passCount
 *              Count of tests which passed.
 *  @field  failCount
 *              Count of tests which failed.
 *  ============================================================================
 */
typedef struct Result_tag {
    Uint32 testCount ;
    Uint32 testRun   ;
    Uint32 passCount ;
    Uint32 failCount ;
} Result ;


/** ============================================================================
 *  @name   TestInfo
 *
 *  @desc   The structure stores the names of the testsuite and testcase.
 *
 *  @field  testSuite
 *              Name of the TestSuite.
 *  @field  testCase
 *              Name of the TestCase.
 *  ============================================================================
 */
typedef struct TestInfo_tag {
    Char8 * testSuite ;
    Char8 * testCase  ;
} TestInfo ;


/** ============================================================================
 *  @name   TST_State
 *
 *  @desc   This Keeps track of which state the testsuite is in.
 *  ============================================================================
 */
typedef enum {
    TST_RunAll                = 0x00,
    TST_IgnoreTestCase        = 0x01,
    TST_IgnoreTestScenario    = 0x02
} TST_State ;


/** ============================================================================
 *  @func   TST_Main
 *
 *  @desc   Main FrameWork function.
 *
 *  @arg    argc
 *              No. of arguments.
 *  @arg    argv
 *              List of arguments.
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
Void
TST_Main (IN Uint32 argc, IN Char8 ** argv) ;


/** ============================================================================
 *  @func   TST_ExecuteScript
 *
 *  @desc   This function is invoked with the script argument and it also
 *          invokes the desired testcase.
 *
 *  @arg    argc
 *              No. of arguments.
 *  @arg    argv
 *              List of arguments.
 *  @arg    attrs
 *              Pointer to structure of the attributes to be passed to
 *              the framework.
 *  @arg    testState
 *              Defines the state of the test currently executing.
 *  @arg    frameworkResult
 *              Pointer to result structure.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_INVALIDARG
 *              Wrong no of arguments passed.
 *          DSP_EMEMORY
 *              Operation failed due to insufficient memory.
 *          DSP_EPOINTER
 *              Invalid pointer passed.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ExecuteScript (IN  Uint32       argc,
                   IN  Char8 **     argv,
                   IN  TST_Attrs *  attrs,
                   IN  TST_State    testState,
                   OUT Result *     frameworkResult) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (TST_FRAMEWORK_H) */
