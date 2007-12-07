/** ============================================================================
 *  @file   SupplierMsg.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\SupplierMsg
 *
 *  @desc   Defines the interface of SupplierMsg. This test waits for a message
 *          from the GPP and interprets the info field of the TestMsg as the
 *          number of buffers to send to GPP. These buffers are sent to the
 *          GPP on channel 1. Subsequently a message is sent to the GPP
 *          denoting the end of buffer queuing. The test passes when the
 *          message reaches the GPP before the requested number of buffers.
 *          The converse does not denote a failure.
 *          This test tests the priority of messages over channels.
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


#if !defined (SUPPLIERMSG_H)
#define SUPPLIERMSG_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   SupplierMsg
 *
 *  @desc   This test waits for a message from the GPP and interprets
 *          the info field of the TestMsg as the number of buffers
 *          to send to GPP. These buffers are sent to the GPP on
 *          channel 1. Subsequently a message is sent to the GPP
 *          denoting the end of buffer queuing. The test passes when the
 *          message reaches the GPP before the requested number of buffers.
 *          The converse does not denote a failure. A request for zero
 *          buffers indicates shutdown.
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
SupplierMsg () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SUPPLIERMSG_H) */
