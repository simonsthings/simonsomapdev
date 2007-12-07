/** ============================================================================
 *  @file   STS_MsgqMemoryLeakMsgq.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress
 *
 *  @desc   Defines the interface of STS_MsgqMemoryLeakMsgq test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (STS_MSGQMEMORYLEAKMSGQ_H)
#define STS_MSGQMEMORYLEAKMSGQ_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   STS_MsgqMemoryLeakMsgq
 *
 *  @desc   Tests memory leaks in consecutive create and delete
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
STS_MsgqMemoryLeakMsgq (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (STS_MSGQMEMORYLEAKMSGQ_H) */
