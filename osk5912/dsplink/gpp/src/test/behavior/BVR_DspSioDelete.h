/** ============================================================================
 *  @file   BVR_DspSioDelete.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Interface definition for behavior test BVR_DspSioDelete.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BVR_DSPSIODELETE_H)
#define BVR_DSPSIODELETE_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @func   BVR_DspSioDelete
 *
 *  @desc   Function to test the behavior DSP SIO_delete when flush flag is TRUE
 *          and FALSE. In this function the GPP side first sends a message on
 *          the control channel to synchronize with DSP. It then, depending upon
 *          the channel mode and flush flag issues and reclaims the buffer(s)
 *          to the DSP side. The DSP image executes and then it sends the status
 *          of its execution on input control channel. If the DSP side
 *          successfully passes then the test passes.
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
 *          DSP_EFAIL
 *              Error in execution of test.
 *
 *  @enter  argc must equal to 10.
 *          argv must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_DspSioDelete (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (BVR_DSPSIODELETE_H) */
