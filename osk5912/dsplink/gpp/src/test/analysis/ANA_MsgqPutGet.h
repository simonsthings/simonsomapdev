/** ============================================================================
 *  @file   ANA_MsgqPutGet.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Defines the interface of ANA_MsgqPutGet behavior test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (ANA_MSGQPUTGET_H)
#define ANA_MSGQPUTGET_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   ANA_MsgqPutGet
 *
 *  @desc   This testcase measures the throughput of the message transfers
 *          to / from DSP depending upon the mode of operation.
 *
 *  @arg    argc
 *              Count of the no. of arguments passed.
 *  @arg    argv
 *              List of arguments.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EINVALIDARG
 *              Wrong no of arguments passed.
 *          DSP_EFAIL
 *              Test case failed.
 *
 *  @enter  argc should be NUM_ARGS (see the C file).
 *          argv should not be null.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ANA_MsgqPutGet (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (ANA_MSGQPUTGET_H) */
