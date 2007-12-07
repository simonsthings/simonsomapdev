/** ============================================================================
 *  @file   SendMsg.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\SendMsg
 *
 *  @desc   Defines the interface of SendMsg. This test continuously
 *          sends messages to GPP with a predefined data in the message.
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


#if !defined (SENDMSG_H)
#define SENDMSG_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   SendMsg
 *
 *  @desc   This function communicates with gpp side application and
 *          continuously sends messages to GPP side.
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
SendMsg () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SENDMSG_H) */
