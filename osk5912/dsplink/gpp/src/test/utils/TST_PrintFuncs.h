/** ============================================================================
 *  @file   TST_PrintFuncs.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Defines the interface of various print Functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_PRINTFUNCS_H)
#define TST_PRINTFUNCS_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_PrnError
 *
 *  @desc   This function prints error strings.
 *
 *  @arg    fmt
 *              The string which is to be printed.
 *
 *  @arg    status
 *              The status of the call causing the error.
 *
 *  @ret    None.
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
TST_PrnError (IN Char8 * fmt, IN DSP_STATUS status) ;


/** ============================================================================
 *  @func   TST_PrnDebug
 *
 *  @desc   This function prints debug strings.
 *
 *  @arg    fmt
 *              The string which is to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnDebug (IN Char8 * fmt) ;


/** ============================================================================
 *  @func   TST_PrnArgs
 *
 *  @desc   This function prints arguments used by testcase.
 *
 *  @arg    argc
 *              Number of arguments.
 *  @arg    argv
 *              Pointer to array of arguments to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnArgs (IN Uint32 argc, IN Char8 ** argv) ;


/** ============================================================================
 *  @func   TST_PrnInfo
 *
 *  @desc   This function prints information strings.
 *
 *  @arg    fmt
 *              The string which is to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnInfo (IN Char8 * fmt, ...) ;


/** ============================================================================
 *  @func   TST_PrnStatus
 *
 *  @desc   This function prints status of the tests.
 *
 *  @arg    statusToPrint
 *              The status of the test executed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnStatus (IN DSP_STATUS statusToPrint) ;


/** ============================================================================
 *  @func   TST_PrnTestCase
 *
 *  @desc   This function Testsuite and testcase name.
 *
 *  @arg    testInfo
 *              The pointer to structure containing names of testsuite and
 *              testcase to be executed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnTestCase (IN TestInfo * testInfo) ;


/** ============================================================================
 *  @func   TST_PrnSummary
 *
 *  @desc   This function prints summary of the tests run.
 *
 *  @arg    finalResult
 *              The pointer to structure  containing the results .
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnSummary (IN Result * finalResult) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_PRINTFUNCS_H) */
