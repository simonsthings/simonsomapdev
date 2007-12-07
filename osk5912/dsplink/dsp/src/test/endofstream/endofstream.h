/** ============================================================================
 *  @file   EndOfStream.h
 *
 *  @path   $(PROJROOT)\dsp\src\test\EndOfStream
 *
 *  @desc   Defines the interface of EndOfStream.
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


#if !defined (ENDOFSTREAM_H)
#define ENDOFSTREAM_H


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
#define OUTPUTDATACHANNEL  "/dio_dsplink1"


/** ============================================================================
 *  @func   EndOfStream
 *
 *  @desc   This function communicates with gpp side application and sends
 *          a data buffer from GPP side.
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
EndOfStream () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (ENDOFSTREAM_H) */
