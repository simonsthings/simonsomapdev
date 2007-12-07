/** ============================================================================
 *  @file   message.h
 *
 *  @path   $(DSPLINK)\gpp\src\samples\message
 *
 *  @desc   Defines the configurable parameters for the message test which
 *          sends a message across the DSP processor and receives it back
 *          using DSP/BIOS LINK.
 *          It also does the data verification on the received message.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (MESSAGE_H)
#define MESSAGE_H


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
 *  @func   MESSAGE_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @arg    dspExecutable
 *              DSP executable name.
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
 *  @see    MESSAGE_Delete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MESSAGE_Create (IN Char8 * dspExecutable) ;


/** ============================================================================
 *  @func   MESSAGE_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @arg    numIterations
 *              Number of times to send the message to the DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              MESSAGE execution failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MESSAGE_Delete , MESSAGE_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MESSAGE_Execute (IN Uint32 numIterations) ;


/** ============================================================================
 *  @func   MESSAGE_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          MESSAGE_Create ().
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
 *  @see    MESSAGE_Create
 *  ============================================================================
 */
NORMAL_API
Void
MESSAGE_Delete () ;


/** ============================================================================
 *  @func   MESSAGE_Main
 *
 *  @desc   The OS independent driver function for the MESSAGE sample
 *          application.
 *
 *  @arg    dspExecutable
 *              Name of the DSP executable file.
 *  @arg    strNumIterations
 *              Number of iterations a message is transferred between
 *              GPP and DSP in string format.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MESSAGE_Create, MESSAGE_Execute, MESSAGE_Delete
 *  ============================================================================
 */
NORMAL_API
Void
MESSAGE_Main (IN Char8 * dspExecutable, IN Char8 * strNumIterations) ;


/** ============================================================================
 *  @func   MESSAGE_0Print
 *
 *  @desc   Print a message without any arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\message_os.c
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
MESSAGE_0Print (Char8 * str) ;


/** ============================================================================
 *  @func   MESSAGE_1Print
 *
 *  @desc   Print a message with one arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\message_os.c
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
MESSAGE_1Print (Char8 * str, Uint32 arg) ;


/** ============================================================================
 *  @func   MESSAGE_Sleep
 *
 *  @desc   Sleeps for the specified number of microseconds.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\message_os.c
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
MESSAGE_Sleep (Uint32 uSec) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (MESSAGE_H) */
