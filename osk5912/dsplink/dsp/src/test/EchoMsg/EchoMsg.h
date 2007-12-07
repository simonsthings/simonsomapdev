/** ============================================================================
 *  @file   EchoMsg.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\EchoMsg
 *
 *  @desc   Defines the interface of EchoMsg. This test waits for a message
 *          from the GPP and sends it back to the GPP after a delay. The
 *          delay is determined from the contents of the message.
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


#if !defined (ECHOMSG_H)
#define ECHOMSG_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   EchoMsg
 *
 *  @desc   This test waits for a message from the GPP and sends it
 *          back to the GPP after a delay. The delay is determined from
 *          the contents of the message.
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
EchoMsg () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (ECHOMSG_H) */
