/** ============================================================================
 *  @file   application.h
 *
 *  @path   $(PROJROOT)\dsp\src\test
 *
 *  @desc   Defines the interface of WaveApplication on DSP side.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (WAVEAPPLICATION_H)
#define WAVEAPPLICATION_H


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
 *  @const  CTL_STREAM_MASK
 *
 *  @desc   This is used to test if any data is recieved on control stream.
 *  ============================================================================
 */
#define CTL_STREAM_MASK     2

/** ============================================================================
 *  @const  DATA_STREAM_MASK
 *
 *  @desc   This is used to test if any data is recieved on Data stream.
 *  ============================================================================
 */
#define DATA_STREAM_MASK    1

/** ============================================================================
 *  @const  BUFSIZE
 *
 *  @desc   This is the Buffer size used for transfer.
 *  ============================================================================
 */
#define BUFSIZE       8192

/** ============================================================================
 *  @const  NUMSTREAMS
 *
 *  @desc   This is the no. of streams for which waiting is done.
 *  ============================================================================
 */
#define NUMSTREAMS    2

/** ============================================================================
 *  @const  INPUTDATACHANNEL
 *
 *  @desc   This is the path for input channel.
 *  ============================================================================
 */
#define INPUTDATACHANNEL  "/dio_dsplink0"

/** ============================================================================
 *  @const  INPUTCTLCHANNEL
 *
 *  @desc   This is the path for control channel.
 *  ============================================================================
 */
#define INPUTCTLCHANNEL  "/dio_dsplink1"

/** ============================================================================
 *  @const  CODECPATH
 *
 *  @desc   This is the path for codec driver.
 *  ============================================================================
 */
#define CODECPATH "/codec0"


/** ============================================================================
 *  @name   WaveState
 *
 *  @desc   Enumeration of states in wave application.
 *
 *  @field  WAVE_START
 *              Start mode.
 *  @field  WAVE_STOP
 *              Stop mode.
 *  @field  WAVE_PLAY
 *              Playing mode.
 *  @field  WAVE_PAUSE
 *              Pause mode.
 *  ============================================================================
 */
typedef enum {
    WAVE_START   = 0x0061,
    WAVE_STOP    = 0x0062,
    WAVE_PLAY    = 0x0063,
    WAVE_PAUSE   = 0x0064
} WaveState ;


/** ============================================================================
 *  @func   WaveApplication
 *
 *  @desc   This function communicates with gpp side application and plays
 *          the wave file.
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
Void WaveApplication () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (WAVEAPPLICATION_H) */
