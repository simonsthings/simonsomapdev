/** ============================================================================
 *  @file   ReceiveMsg.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\ReceiveMsg
 *
 *  @desc   Defines the interface of ReceiveMsg. This test receives messages
 *          from the GPP and verifies the content of the messages. It also
 *          sends back a report to the GPP using channels indicating
 *          the status of the verification.
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


#if !defined (RECEIVEMSG_H)
#define RECEIVEMSG_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   ReceiveMsg
 *
 *  @desc   This function receives messages from the GPP and
 *          verifies the content of the messages. It also sends
 *          back a report to the GPP using channels indicating
 *          the status of the verification.
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
ReceiveMsg () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (RECEIVEMSG_H) */
