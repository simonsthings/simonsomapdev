/** ============================================================================
 *  @file   Benchmarking.h
 *
 *  @path   $(PROJROOT)\dsp\src\samples\benchmarking
 *
 *  @desc   Defines the interface of Benchmarking.
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


#if !defined (BENCHMARKING_H)
#define BENCHMARKING_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   INPUTDATACHANNEL
 *
 *  @desc   This is the device name for input channel.
 *  ============================================================================
 */
#define INPUTDATACHANNEL  "/dio_dsplink0"


/** ============================================================================
 *  @func   Benchmarking
 *
 *  @desc   This function receives buffers from the gpp side.
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
Benchmarking () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (BENCHMARKING_H) */
