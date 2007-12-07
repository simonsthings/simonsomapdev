/** ============================================================================
 *  @file   SioDeleteTest.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\SioDeleteTest
 *
 *  @desc   Defines the interface of SioDeleteTest.
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


#if !defined (SIODELETETEST_H)
#define SIODELETETEST_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MESG_BUF_SIZE
 *
 *  @desc   Size of the message buffer to be transferred to GPP.
 *  ============================================================================
 */
#define MESG_BUF_SIZE     8

/** ============================================================================
 *  @name   MAX_QUEUE
 *
 *  @desc   Maximum queuing length being used.
 *  ============================================================================
 */
#define MAX_QUEUE     16

/** ============================================================================
 *  @name   INPUT_CONTROL_CHNL_ID
 *
 *  @desc   Channel name for input control channel.
 *  ============================================================================
 */
#define INPUT_CONTROL_CHNL_ID      "/dio_dsplink14"

/** ============================================================================
 *  @name   OUTPUT_CONTROL_CHNL_ID
 *
 *  @desc   Channel name for output control channel.
 *  ============================================================================
 */
#define OUTPUT_CONTROL_CHNL_ID     "/dio_dsplink13"


/** ============================================================================
 *  @func   SioDeleteTest
 *
 *  @desc   This function is the entry point to the test.
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
SioDeleteTest () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SIODELETETEST_H) */
