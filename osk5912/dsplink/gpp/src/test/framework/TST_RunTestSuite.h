/** ============================================================================
 *  @file   TST_RunTestSuite.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Defines the interface of TST_RunTestSuite Function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_RUNTESTSUITE_H)
#define TST_RUNTESTSUITE_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   WhichTestSuite
 *
 *  @desc   This Keeps track of which test suite to execute.
 *  ============================================================================
 */
typedef enum {
    NoTest           = 0x00,
    Test_Api         = 0x01,
    Test_Analysis    = 0x02,
    Test_Behavior    = 0x03,
    Test_Stress      = 0x04,
    Test_Application = 0x05
} WhichTestSuite ;


/** ============================================================================
 *  @func   TST_RunTestSuite
 *
 *  @desc   This function invokes the desired testsuite.
 *
 *  @arg    argc
 *              No of arguments
 *  @arg    argv
 *              List of arguments
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
 *          DSP_EFAIL
 *              Operation failed due to general error.
 *          DSP_EINVALIDARG
 *              Operation failed due to  Invalid arguments.
 *          DSP_EPOINTER
 *              Operation failed due to Invalid pointer.
 *
 *  @enter  argc should be greater than equal to 2
            argv should not be NULL
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_RunTestSuite (IN Int32        argc,
                  IN Char8 **     argv,
                  IN TST_Attrs *  attrs,
                  IN TST_State    testState,
                  IN Result *     frameworkResult) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_RUNTESTSUITE_H) */
