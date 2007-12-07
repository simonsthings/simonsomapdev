/** ============================================================================
 *  @file   API_DspDisableInterrupt.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\dsp
 *
 *  @desc   Defines the interface of DSP_DisableInterrupt API test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (API_DSPDISABLEINTERRUPT_H)
#define API_DSPDISABLEINTERRUPT_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   API_DspDisableInterrupt
 *
 *  @desc   Tests the DSP_DisableInterrupt API.
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
 *  @enter  argc should be 4.
 *          argv should not be null.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_DspDisableInterrupt (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (API_DSPDISABLEINTERRUPT_H) */
