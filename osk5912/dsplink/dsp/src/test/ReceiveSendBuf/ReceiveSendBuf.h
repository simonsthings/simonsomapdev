/** ============================================================================
 *  @file   ReceiveSendBuf.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\ReceiveSendBuf
 *
 *  @desc   Defines the interface of ReceiveSendBuf.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  This software is copyright to Texas Instruments Incorporated.
 *  Its usage is governed by the terms and conditions stipulated in
 *  the license agreement under which this software has been supplied.
 *
 *  The recipient of this software implicitly accepts the terms of
 *  the license.
 *  ============================================================================
 */


#if !defined (RECEIVESENDBUF_H)
#define RECEIVESENDBUF_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  IN/OUT/OPTIONAL/CONST
 *
 *  @desc   Argument specification syntax
 *  ============================================================================
 */
#define IN                              /* The argument is INPUT  only */
#define OUT                             /* The argument is OUTPUT only */
#define OPT                             /* The argument is OPTIONAL    */
#define CONST   const


/** ============================================================================
 *  @name   INPUTDATACHANNEL
 *
 *  @desc   This is the path for control channel.
 *  ============================================================================
 */
#define INPUTDATACHANNEL  "/dio_dsplink0"


/** ============================================================================
 *  @name   INPUTDATACHANNEL
 *
 *  @desc   This is the path for output channel.
 *  ============================================================================
 */
#define OUTPUTDATACHANNEL  "/dio_dsplink1"


/** ============================================================================
 *  @func   ReceiveSendBuf
 *
 *  @desc   This function communicates with gpp side application and returns
 *          the same buffer which it received.
 *
 *  @arg    None
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
Void
ReceiveSendBuf () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (RECEIVESENDBUF_H) */
