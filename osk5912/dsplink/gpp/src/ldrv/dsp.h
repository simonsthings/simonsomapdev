/** ============================================================================
 *  @file   dsp.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines interface exposed by DSP sub-component.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DSP_H)
#define DSP_H


USES (gpptypes.h)
USES (errbase.h)
USES (intobject.h)
USES (dspdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   DSP_Setup
 *
 *  @desc   Sets up components to make DSP reachable from GPP.
 *          Causes no state transition on DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid.
 *
 *  @enter  dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Setup (IN ProcessorId dspId, IN DspObject * dspObj) ;


/** ============================================================================
 *  @func   DSP_Initialize
 *
 *  @desc   Resets the DSP and initializes the components required by DSP.
 *          Puts the DSP in RESET state.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Setup, DSP_Finalize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Initialize (IN ProcessorId  dspId, IN DspObject * dspObj) ;


/** ============================================================================
 *  @func   DSP_Finalize
 *
 *  @desc   Resets the DSP and puts it into IDLE Mode.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Finalize (IN ProcessorId dspId, IN DspObject * dspObj) ;


/** ============================================================================
 *  @func   DSP_Start
 *
 *  @desc   Causes DSP to start execution from the given DSP address.
 *          DSP is put to STARTED state after successful completion.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              Address to start execution from.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Stop
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Start (IN ProcessorId dspId, IN DspObject * dspObj, IN Uint32 dspAddr) ;


/** ============================================================================
 *  @func   DSP_Stop
 *
 *  @desc   Stops execution on DSP.
 *          DSP is put to STOPPED state after successful completion.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Start
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Stop (IN ProcessorId dspId, IN DspObject * dspObj) ;


/** ============================================================================
 *  @func   DSP_Idle
 *
 *  @desc   Idles the DSP.
 *          DSP is put to IDLE state after successful completion.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Start
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Idle (IN ProcessorId dspId, IN DspObject * dspObj) ;


/** ============================================================================
 *  @func   DSP_EnableInterrupt
 *
 *  @desc   Enables the specified interrupt for communication with DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Setup, DSP_Interrupt, DSP_DisableInterrupt
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_EnableInterrupt (IN ProcessorId         dspId,
                     IN DspObject *         dspObj,
                     IN InterruptObject *   intInfo) ;


/** ============================================================================
 *  @func   DSP_DisableInterrupt
 *
 *  @desc   Disables the specified interrupt for communication with DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Setup, DSP_Interrupt, DSP_EnableInterrupt
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_DisableInterrupt (IN ProcessorId        dspId,
                      IN DspObject *        dspObj,
                      IN InterruptObject *  intInfo) ;

/** ============================================================================
 *  @func   DSP_Interrupt
 *
 *  @desc   Sends the specified interrupt to DSP.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *  @arg    arg
 *              Pointer to a value to send with the interrupt.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid.
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    DSP_Start
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Interrupt (IN     ProcessorId       dspId,
               IN     DspObject *       dspObj,
               IN     InterruptObject * intInfo,
               IN OPT Pvoid             arg) ;


/** ============================================================================
 *  @func   DSP_ClearInterrupt
 *
 *  @desc   Clear the DSP Interrupt.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    intInfo
 *              Pointer to object containing interrupt information.
 *  @arg    retVal
 *              Interrupt value present before clearing the interrupt.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *          retval must be a valid pointer.
 *
 *  @leave  None.
 *
 *  @see    DSP_Start
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_ClearInterrupt (IN  ProcessorId         dspId,
                    IN  DspObject *         dspObj,
                    IN  InterruptObject *   intInfo,
                    OUT Pvoid               retVal) ;


/** ============================================================================
 *  @func   DSP_Read
 *
 *  @desc   Reads data from DSP.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              DSP address to read from.
 *  @arg    endianInfo
 *              endianness of data - indicates whether swap is required or not.
 *  @arg    numBytes
 *              Number of bytes to read.
 *  @arg    buffer
 *              Buffer to hold the read data.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *          numBytes must be a valid pointer.
 *          buffer must be a valid pointer.
 *
 *  @leave  On successful completion, holds the number of bytes read.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Read (IN     ProcessorId  dspId,
          IN     DspObject *  dspObj,
          IN     Uint32       dspAddr,
          IN     Endianism    endianInfo,
          IN OUT Uint32 *     numBytes,
          OUT    Uint8 *      buffer) ;


/** ============================================================================
 *  @func   DSP_Write
 *
 *  @desc   Writes data to DSP.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              DSP address to write to.
 *  @arg    endianInfo
 *              endianness of data - indicates whether swap is required or not.
 *  @arg    numBytes
 *              Number of bytes to write.
 *  @arg    buffer
 *              Buffer containing data to be written.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Arguments dspId and/or dspObj are invalid..
 *          DSP_EFAIL
 *              DSP_Setup function wasn't called before calling this function.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *          buffer must be a valid pointer.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Write (IN ProcessorId dspId,
           IN DspObject * dspObj,
           IN Uint32      dspAddr,
           IN Endianism   endianInfo,
           IN Uint32      numBytes,
           IN Uint8 *     buffer) ;


/** ============================================================================
 *  @func   DSP_Control
 *
 *  @desc   Hook for performing device dependent control operation.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    cmd
 *              Command id.
 *  @arg    arg
 *              Optional argument for the specified command.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments specified.
 *
 *  @enter  DSP_Setup must be called before calling this function.
 *          dspId must be a valid DSP identifier.
 *          dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Control (IN  ProcessorId dspId,
             IN  DspObject * dspObj,
             IN  Int32       cmd,
             OPT Pvoid       arg) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   DSP_Instrument
 *
 *  @desc   Gets the instrumentation information related to the specified
 *          DSP object.
 *
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    retVal
 *              Placeholder to return the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument(s).
 *
 *  @enter  dspObj must be valid.
 *          retVal must be valid.
 *
 *  @leave  None.
 *
 *  @see    DspObject
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Instrument (IN DspObject * dspObj, OUT DspStats * retVal) ;

#endif /* if defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   DSP_Debug
 *
 *  @desc   Prints debug information of the specified DSP object.
 *
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *
 *  @enter  dspObj must be pointing to a valid DSP object.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
DSP_Debug (IN DspObject * dspObj) ;
#endif /* if defined (DDSP_DEBUG) */



#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (DSP_H) */
