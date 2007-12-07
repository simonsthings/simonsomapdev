/** ============================================================================
 *  @file   scale.h
 *
 *  @path   $(DSPLINK)\gpp\src\samples\scale
 *
 *  @desc   Defines the configurable parameters for the scale test which
 *          sends a buffer across the DSP processor and receives a buffer back
 *          using DSP/BIOS LINK. The DSP scales the returned buffer based on
 *          the control message sent by the GPP.
 *          It also does the data verification on the received buffer.
 *          In this application the transfer is done using only one buffer and
 *          no queueing of buffers is done.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (SCALE_H)
#define SCALE_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  ID_PROCESSOR
 *
 *  @desc   The processor id of the processor being used.
 *  ============================================================================
 */
#define ID_PROCESSOR       0


/** ============================================================================
 *  @const  CHNL_ID_OUTPUT
 *
 *  @desc   ID of channel used to send data to DSP.
 *  ============================================================================
 */
#define CHNL_ID_OUTPUT     0


/** ============================================================================
 *  @const  CHNL_ID_INPUT
 *
 *  @desc   ID of channel used to receive data from DSP.
 *  ============================================================================
 */
#define CHNL_ID_INPUT      1


/** ============================================================================
 *  @func   SCALE_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @arg    dspExecutable
 *              DSP executable name.
 *  @arg    bufferSize
 *              String representation of buffer size to be used
 *              for data transfer.
 *  @arg    strNumIterations
 *              Number of iterations a data buffer is transferred between
 *              GPP and DSP in string format.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Resource allocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SCALE_Delete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SCALE_Create (IN Char8 * dspExecutable,
              IN Char8 * strBufferSize,
              IN Char8 * strNumIterations) ;


/** ============================================================================
 *  @func   SCALE_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              SCALE execution failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SCALE_Delete , SCALE_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SCALE_Execute () ;


/** ============================================================================
 *  @func   SCALE_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          SCALE_Create ().
 *          During cleanup, the allocated resources are being freed
 *          unconditionally. Actual applications may require stricter check
 *          against return values for robustness.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Resource deallocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SCALE_Create
 *  ============================================================================
 */
NORMAL_API
Void
SCALE_Delete () ;


/** ============================================================================
 *  @func   SCALE_Main
 *
 *  @desc   The OS independent driver function for the scale sample application.
 *
 *  @arg    dspExecutable
 *              Name of the DSP executable file.
 *  @arg    strBufferSize
 *              Buffer size to be used for data-transfer in string format.
 *  @arg    strNumIterations
 *              Number of iterations a data buffer is transferred between
 *              GPP and DSP in string format.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SCALE_Create, SCALE_Execute, SCALE_Delete
 *  ============================================================================
 */
NORMAL_API
Void
SCALE_Main (IN Char8 * dspExecutable,
            IN Char8 * strBuffersize,
            IN Char8 * strNumIterations) ;


/** ============================================================================
 *  @func   SCALE_0Print
 *
 *  @desc   Print a message without any arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\scale_os.c
 *
 *  @arg    str
 *              String message to be printed.
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
NORMAL_API
Void
SCALE_0Print (Char8 * str) ;


/** ============================================================================
 *  @func   SCALE_1Print
 *
 *  @desc   Print a message with one arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\scale_os.c
 *
 *  @arg    str
 *              String message to be printed.
 *  @arg    arg
 *              Argument to be printed.
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
NORMAL_API
Void
SCALE_1Print (Char8 * str, Uint32 arg) ;


/** ============================================================================
 *  @func   SCALE_Sleep
 *
 *  @desc   Sleeps for the specified number of microseconds.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\scale_os.c
 *
 *  @arg    uSec
 *              Microseconds to sleep.
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
NORMAL_API
Void
SCALE_Sleep (Uint32 uSec) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SCALE_H) */
