/** ============================================================================
 *  @file   LocateTimeoutMsg.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\LocateTimeoutMsg
 *
 *  @desc   Defines the interface of LocateTimeoutMsg. This test creates a
 *          queue and waits for a message from the GPP. Upon receiving the
 *          message, it interprets the info field of the message as a delay
 *          value and sleeps for the delay time. Then it creates another queue,
 *          deletes the first one and waits for a message on the second
 *          queue before shutting down.
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


#if !defined (LOCATETIMEOUTMSG_H)
#define LOCATETIMEOUTMSG_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   LocateTimeoutMsg
 *
 *  @desc   This function creates a queue and waits for a message
 *          from the GPP. Upon receiving the message, it interprets
 *          the info field of the message as a delay value and sleeps
 *          for the delay time. Then it creates another queue,
 *          deletes the first one and waits for a message on the second
 *          queue before shutting down.
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
LocateTimeoutMsg () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LOCATETIMEOUTMSG_H) */
