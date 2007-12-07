/** ============================================================================
 *  @file   TST_Analysis.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Defines the interfaces of TST_Analysis Function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_ANALYSIS_H)
#define TST_ANALYSIS_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   TICS_PER_SEC
 *
 *  @desc   This is the no. of ticks that occur per second.
 *  ============================================================================
 */
#define TICS_PER_SEC 1


/** ============================================================================
 *  @func   TST_ZeroTime
 *
 *  @desc   This function resets the system timer.
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
EXPORT_API
Void
TST_ZeroTime () ;


/** ============================================================================
 *  @func   TST_GetTime
 *
 *  @desc   This function gives the time elapsed since last reset.
 *
 *  @arg    None
 *
 *  @ret    Time elapsed since last reset.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Uint32
TST_GetTime () ;


/** ============================================================================
 *  @func   TST_DoAnalysis
 *
 *  @desc   This function calculates the throughput of the test under
 *          consideration.
 *
 *  @arg    startTime
 *              Starting tickcount value.
 *  @arg    stopTime
 *              Ending tickcount value.
 *  @arg    iterations
 *              It is the number of iterations for which the transfer was made.
 *  @arg    bufSize
 *              Size of buffer based on which throughput is to be calculated.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_DoAnalysis (IN Uint32 startTime, IN Uint32 stopTime,
                IN Uint32 iterations, IN Uint32 bufSize) ;


/** ============================================================================
 *  @func   TST_GetFileSize
 *
 *  @desc   This function gives the size of the file  in bytes.
 *
 *  @arg    fileName
 *              The name of the file whose size is to be determined.
 *  @arg    size
 *              The size of the file.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments.
 *          DSP_EFILE
 *              File error.
 *          DSP_EPOINTER
 *              Invalid file object.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_GetFileSize (IN Char8 * fileName, OUT Uint32 * size) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TST_ANALYSIS_H) */
