/** ============================================================================
 *  @file   TST_TestInputs.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Defines the interface of TST_TestInputs Function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_TESTINPUTS_H)
#define TST_TESTINPUTS_H


USES (gpptypes.h)
USES (errbase.h)
USES (tst_framework.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_TestInputs
 *
 *  @desc   It  parses the data file to reads one line.
 *
 *  @arg    argc
 *              No. of arguments in the line parsed.
 *  @arg    argv
 *              The pointer to the list of arguments.
 *  @arg    filePtr
 *              File pointer of the file to be parsed.
 *  @arg    isTestCommented
 *              The pointer to the flag which tells if the test is commented
 *              out.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument given.
 *          DSP_EFILE
 *              File error.
 *
 *  @enter  filePtr should not be NULL.
 *          argc should not be NULL.
 *          argv should not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_TestInputs (IN  Void *   filePtr,
                OUT Uint32 * argc,
                OUT Char8    arguments [][MAX_BUFSIZE],
                OUT Bool *   isTestCommented) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_TESTINPUTS_H) */
