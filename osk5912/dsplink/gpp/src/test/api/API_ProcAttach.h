/** ============================================================================
 *  @file   API_ProcAttach.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Defines the interface of PROC_Attach API test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (API_PROCATTACH_H)
#define API_PROCATTACH_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   API_ProcAttach
 *
 *  @desc   Entry point for the PROC_Attach  API Test.
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
 *  @enter  argc should be 3.
 *          argv should not be null.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_ProcAttach (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (API_PROCATTACH_H) */
