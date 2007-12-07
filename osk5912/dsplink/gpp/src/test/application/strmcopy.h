/** ============================================================================
 *  @file   strmcopy.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\application
 *
 *  @desc   Defines the interface of strmcopy test which sends the data across
 *          the LINK with data validation
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (STRMCOPY_H)
#define STRMCOPY_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   StrmCopy
 *
 *  @desc   This test the strmcopy test which sends the data across
 *          the LINK with data validation
 *
 *  @arg    argc
 *              Count of the no. of arguments passed
 *  @arg    argv
 *              List of arguments
 *
 *  @ret    DSP_SOK
 *              Successful
 *          DSP_INVALIDARG
 *              Wrong no of arguments passed to test.
 *          DSP_EFILE
 *              Wrong filename passed.
 *
 *  @enter  The argc should be 6.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
StrmCopy (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (STRMCOPY_H) */
