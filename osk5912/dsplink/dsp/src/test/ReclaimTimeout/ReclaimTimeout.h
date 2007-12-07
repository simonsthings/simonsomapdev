/** ============================================================================
 *  @file   ReclaimTimeout.h
 *
 *  @path   $(DSPLINK)\dsp\src\test\ReclaimTimeout
 *
 *  @desc   Defines the interface of ReclaimTimeout.
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


#if !defined (RECLAIMTIMEOUT_H)
#define RECLAIMTIMEOUT_H


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
 *  @name   INPUT_CONTROL_CHNL_ID
 *
 *  @desc   Channel name for input control channel.
 *  ============================================================================
 */
#define INPUT_CONTROL_CHNL_ID      "/dio_dsplink14"

/** ============================================================================
 *  @func   ReclaimTimeout
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
ReclaimTimeout () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (RECLAIMTIMEOUT_H) */
