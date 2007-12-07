/** ============================================================================
 *  @file   TST_Sleep.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Defines the interface of TST_Sleep function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_SLEEP_H)
#define TST_SLEEP_H


USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_Sleep
 *
 *  @desc   This function sleeps for the specified time.
 *
 *  @arg    sleepTime
 *              The time for which the sleep has to be done.
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
EXPORT_API
Void
TST_Sleep (Uint32 sleepTime) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_SLEEP_H) */
