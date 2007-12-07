/** ============================================================================
 *  @file   ldrv_proc.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Provides Interface Definitions for Link Driver PROC component.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LDRV_PROC_H)
#define LDRV_PROC_H


USES (gpptypes.h)
USES (errbase.h)
USES (profile.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   LDRV_PROC_Initialize
 *
 *  @desc   Allocates resources at GPP side that are required for using DSP.
 *          It also sets up the connection to DSP from the GPP and other
 *          associated peripheral hardware.
 *
 *  @arg    dspId
 *              DSP ID of DSP which is to be initialized.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *
 *  @enter  dspID must be valid.
 *
 *  @leave  DSP is put in Reset state on success.
 *
 *  @see    LDRV_PROC_Finalize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Initialize (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_PROC_Finalize
 *
 *  @desc   Releases all the resources created on GPP side for
 *          the specified DSP.
 *
 *  @arg    dspId
 *              DSP ID of DSP which is to be finalized.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EWRONGTATE
 *              LDRV_PROC_Initialize wasn't called before this function.
 *
 *  @enter  dspId must be valid.
 *          DSP must not be in Unknown state.
 *
 *  @leave  DSP is put in Idle state on success.
 *
 *  @see    LDRV_PROC_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Finalize (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_PROC_Start
 *
 *  @desc   Starts execution of DSP from the specified location.
 *          This function calls LDRV_DSP_Start to actually start the processor.
 *
 *  @arg    dspId
 *              DSP ID of DSP which is to be started.
 *  @arg    dspAddr
 *              Location from where to start.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EWRONGSTATE
 *              DSP not in the right state to execute this function.
 *
 *  @enter  dspId must be valid.
 *          DSP must be in either Loaded or Stopped state.
 *
 *  @leave  DSP is put in Started state on success.
 *
 *  @see    LDRV_PROC_Write, LDRV_PROC_Stop
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Start (IN ProcessorId dspId, IN Uint32 dspAddr) ;


/** ============================================================================
 *  @func   LDRV_PROC_Stop
 *
 *  @desc   Stops the execution of DSP. It does so by the call to LDRV_DSP_Stop.
 *
 *  @arg    dspId
 *              DSP ID of DSP which is to be stoped.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EWRONGSTATE
 *              DSP not in the right state to execute this function.
 *
 *  @enter  dspId must be valid.
 *          DSP must be in either Started or Stopped  state.
 *
 *  @leave  DSP is put in Stopped state on success.
 *
 *  @see    LDRV_PROC_Start
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Stop (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_PROC_Idle
 *
 *  @desc   Puts the processor in idle mode, which means that read and write
 *          can be done to the processor.
 *
 *  @arg    dspId
 *              DSP ID of DSP which is to be stopped.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EWRONGSTATE
 *              LDRV_PROC_Initialize wasn't called before this function.
 *
 *  @enter  dspId must be valid.
 *          DSP must not be in Unknown state.
 *
 *  @leave  DSP is put in Idle state on success.
 *
 *  @see    LDRV_PROC_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Idle (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_PROC_Read
 *
 *  @desc   Reads the DSP's memory space. This function calls LDRV_DSP_Read.
 *
 *  @arg    dspId
 *              DSP ID of DSP whoose memory is to be read.
 *  @arg    dspAddr
 *              Address from where to read.
 *  @arg    endianInfo
 *              This specifies endianness of the data.
 *  @arg    numBytes
 *              Number of bytes to read from the location.
 *  @arg    buffer
 *              Buffer in which store the read data.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EWRONGSTATE
 *              DSP not in the right state to execute this function.
 *
 *  @enter  dspId must be valid.
 *          buffer must be valid.
 *          numBytes must be valid.
 *          DSP must not be in Unknown or Reset state.
 *
 *  @leave  None
 *
 *  @see    LDRV_PROC_Idle, LDRV_PROC_Write
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Read (IN     ProcessorId  dspId,
                IN     Uint32       dspAddr,
                IN     Endianism    endianInfo,
                IN OUT Uint32 *     numBytes,
                OUT    Uint8 *      buffer) ;


/** ============================================================================
 *  @func   LDRV_PROC_Write
 *
 *  @desc   Writes to DSP's memory space. This function calls LDRV_DSP_Write.
 *
 *  @arg    dspId
 *              DSP ID of DSP whoose memory is to be written.
 *  @arg    dspAddr
 *              Address to which we need to write.
 *  @arg    endianInfo
 *              This specifies endianness of the data.
 *  @arg    numBytes
 *              Number of bytes to read from the location.
 *  @arg    buffer
 *              Buffer in which to store the data to write.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EWRONGSTATE
 *              DSP not in the right state to execute this function.
 *
 *  @enter  dspId must be valid.
 *          buffer must be valid.
 *          DSP must not be in Unknown state.
 *
 *  @leave  DSP is put in Loaded state on success.
 *
 *  @see    LDRV_PROC_Idle, LDRV_PROC_Read
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Write (IN  ProcessorId  dspId,
                 IN  Uint32       dspAddr,
                 IN  Endianism    endianInfo,
                 IN  Uint32       numBytes,
                 IN  Uint8 *      buffer) ;


/** ============================================================================
 *  @func   LDRV_PROC_GetState
 *
 *  @desc   Returns the current state of the processor.
 *
 *  @arg    dspId
 *              DSP ID of DSP which is to be stopped.
 *  @arg    procState
 *              Placeholder for DSP Status.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *
 *  @enter  dspId must be valid.
 *          procStatus must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_GetState (IN ProcessorId dspId, OUT ProcState * procState) ;


/** ============================================================================
 *  @func   LDRV_PROC_SetState
 *
 *  @desc   Sets the current state of processor to the specified state.
 *
 *  @arg    dspId
 *              DSP ID of DSP which is to be stopped.
 *  @arg    procState
 *              State to which the processor state is to be set.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_SetState (IN ProcessorId dspId, IN ProcState procState) ;


/** ============================================================================
 *  @func   LDRV_PROC_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    cmd
 *              Command id.
 *  @arg    arg
 *              Optional argument for the specified command.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Control (IN  ProcessorId dspId,
                   IN  Int32       cmd,
                   OPT Pvoid       arg) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LDRV_PROC_Instrument
 *
 *  @desc   Gets the instrumentation information related to PROC's
 *
 *  @arg    procId
 *              Identifier for processor.
 *  @arg    retVal
 *              OUT argument to contain the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              retVal is invalid.
 *
 *  @enter  retVal must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Instrument (IN ProcessorId procId, OUT ProcInstrument * retVal) ;
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_PROC_Debug
 *
 *  @desc   Prints out debug information of PROC module.
 *          It prints all the important information maintained by this module.
 *
 *  @arg    procId
 *              Identifier for processor.
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
LDRV_PROC_Debug (IN ProcessorId procId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LDRV_PROC_H) */
