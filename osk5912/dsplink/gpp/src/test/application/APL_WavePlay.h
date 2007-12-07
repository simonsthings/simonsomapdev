/** ============================================================================
 *  @file   APL_WavePlay.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\application
 *
 *  @desc   Defines the interface of Waveplay Application test case.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (APL_WAVEPLAY_H)
#define APL_WAVEPLAY_H


USES (gpptypes.h)
USES (errbase.h)
USES (linkdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  PROCESSOR_ID
 *
 *  @desc   Valid processor id for the wave application.
 *  ============================================================================
 */
#define PROCESSOR_ID 0

/** ============================================================================
 *  @const  CHANNEL_DATA
 *
 *  @desc   Data channel id for the wave application.
 *  ============================================================================
 */
#define CHANNEL_ID   0

/** ============================================================================
 *  @const  BUFSIZE
 *
 *  @desc   Buffer size for the wave application.
 *  ============================================================================
 */
#define BUFSIZE      200

/** ============================================================================
 *  @const  BUFNUM
 *
 *  @desc   No. of buffers required for the wave application.
 *  ============================================================================
 */
#define BUFNUM       1

/** ============================================================================
 *  @const  MODE
 *
 *  @desc   Valid mode for the wave application.
 *  ============================================================================
 */
#define MODE         ChannelMode_Output

/** ============================================================================
 *  @const  ENDIANISM
 *
 *  @desc   Valid endianism for the wave application.
 *  ============================================================================
 */
#define ENDIANISM    Endianism_Default

/** ============================================================================
 *  @const  TIMEOUT
 *
 *  @desc   Valid timeout value for the wave application.
 *  ============================================================================
 */
#define TIMEOUT      WAIT_FOREVER

/** ============================================================================
 *  @macro  FILE_ON_DSP
 *
 *  @desc   Valid dsp image to be loaded on the DSP for the wave application.
 *  ============================================================================
 */
#define DSP_IMAGE  "/opt/dsplink/test/application/application.out"

/** ============================================================================
 *  @macro  CHNL_SIZE
 *
 *  @desc   Valid channel size for the wave application.
 *  ============================================================================
 */
#define CHNL_SIZE    ChannelDataSize_16bits


/** ============================================================================
 *  @func   APL_WavePlay
 *
 *  @desc   This is Waveplay Application test case.
 *
 *  @arg    argc
 *              Count of the no. of arguments passed.
 *  @arg    argv
 *              List of arguments.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_INVALIDARG
 *              Wrong no of arguments passed.
 *
 *  @enter  argc must be 1.
 *          argv must not be null.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
APL_WavePlay (IN Uint32 argc, IN Char8 ** argv) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (APL_WAVEPLAY_H) */
