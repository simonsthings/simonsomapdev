/** ============================================================================
 *  @file   proc.h
 *
 *  @path   $(DSPLINK)/gpp/src/api/
 *
 *  @desc   Defines the interfaces and data structures for the API sub-component
 *          PROC.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PROC_H)
#define PROC_H


USES (gpptypes.h)
USES (errbase.h)
USES (dsplink.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
 *  @const  PROC_CTRL_CMD_GEN_BASE, PROC_CTRL_CMD_GEN_END
 *
 *  @desc   Range for PROC_Control () commands to be handled by the generic
 *          implementation of PROC component.
 *  ============================================================================
 */
#define PROC_CTRL_CMD_GEN_BASE                                     0x00000100
#define PROC_CTRL_CMD_GEN_LAST                                     0x00000200

/*  ============================================================================
 *  @const  PROC_CTRL_CMD_MAP_DSPMEM, PROC_CTRL_CMD_UNMAP_DSPMEM
 *
 *  @desc   Commands for mapping/ unmapping DSP memory.
 *  ============================================================================
 */
#define PROC_CTRL_CMD_MAP_DSPMEM          (PROC_CTRL_CMD_GEN_BASE + 0x00000001)
#define PROC_CTRL_CMD_UNMAP_DSPMEM        (PROC_CTRL_CMD_GEN_BASE + 0x00000002)


/** ============================================================================
 *  @name   ProcMemMapInfo
 *
 *  @desc   Control information for mapping DSP memory region in GPP's address
 *          space.
 *
 *  @field  dspAddr
 *              Address in the DSP memory space to be mapped
 *  @field  size
 *              Size of memory to be mapped
 *  @field  mappedAddr
 *              Mapped address in the user memory space
 *  @field  mappedSize
 *              Size of memory actually mapped
 *  ============================================================================
 */
typedef struct ProcMemMapInfo_tag {
    Uint32   dspAddr ;
    Uint32   size    ;
    Uint32   mappedAddr ;
    Uint32   mappedSize ;
} ProcMemMapInfo ;


/** ============================================================================
 *  @func   PROC_Setup
 *
 *  @desc   Sets up the necessary data structures for the PROC sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    PROC_Destroy
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Setup (Void) ;


/** ============================================================================
 *  @func   PROC_Destroy
 *
 *  @desc   Destroys the data structures for the PROC component,
 *          allocated earlier by a call to PROC_Setup ().
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    PROC_Setup
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Destroy (Void) ;


/** ============================================================================
 *  @func   PROC_Attach
 *
 *  @desc   Attaches the client to the specified DSP and also
 *          initializes the DSP (if required).
 *
 *  @arg    procId
 *             DSP identifier.
 *  @arg    attr
 *             Attributes for the processor on which attach is to be done.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_SALREADYATTACHED
 *              Successful attach. Also, indicates that another client has
 *              already attached to DSP.
 *          DSP_EINVALIDARG
 *              Parameter ProcId is invalid.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFAIL
 *              General failure, unable to attach to processor.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PROC_Detach
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Attach (IN  ProcessorId procId,
             OPT ProcAttr *  attr) ;


/** ============================================================================
 *  @func   PROC_Detach
 *
 *  @desc   Detaches the client from specified processor.
 *          If the caller is the owner of the processor, this function releases
 *          all the resources that this component uses and puts the DSP in an
 *          unusable state (from application perspective).
 *
 *  @arg    procId
 *              DSP identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId parameter.
 *          DSP_EFAIL
 *              General failure, unable to detach.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EATTACHED
 *              Not attached to the target processor.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PROC_Attach
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Detach (IN  ProcessorId procId) ;


/** ============================================================================
 *  @func   PROC_GetState
 *
 *  @desc   Gets the current status of DSP by querying the Link Driver.
 *
 *  @arg    procId
 *              DSP identifier.
 *  @arg    procState
 *              Placeholder for processor state.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId and/or procState argument.
 *
 *  @enter  procId must be valid.
 *          procState must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    PROC_Load, PROC_Start, PROC_Stop, PROC_Idle
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_GetState (IN   ProcessorId   procId,
               OUT  ProcState *   procState) ;


/** ============================================================================
 *  @func   PROC_Load
 *
 *  @desc   Loads the specified DSP executable on the target DSP.
 *          It ensures that the caller owns the DSP.
 *
 *  @arg    procId
 *              DSP identifier.
 *  @arg    imagePath
 *              Full path to the image file to load on DSP.
 *  @arg    argc
 *              Number of argument to be passed to the base image upon start.
 *  @arg    argv
 *              Arguments to be passed to DSP main application.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId argument.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EPENDING
 *              H/W specific error. The request can’t be serviced at this
 *              point of time.
 *          DSP_EFILE
 *              Invalid base image.
 *          DSP_ESIZE
 *              Size of the .args section is not sufficient to hold the passed
 *              arguments.
 *          DSP_EFAIL
 *              General failure, unable to load image on DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  procId must be valid.
 *          imagePath must be a valid pointer.
 *          If argc is 0 then argv must be NULL pointer.
 *          If argc is non-zero then argv must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    PROC_Attach, PROC_LoadSection
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Load (IN   ProcessorId  procId,
           IN   Char8 *      imagePath,
           IN   Uint32       argc,
           IN   Char8 **     argv) ;


/** ============================================================================
 *  @func   PROC_LoadSection
 *
 *  @desc   Loads the specified section of DSP executable onto the target DSP.
 *          It ensures that the client owns the DSP.
 *
 *  @arg    procId
 *              DSP identifier.
 *  @arg    imagePath
 *              Full path to the image file.
 *  @arg    sectID
 *              Section ID of section to load.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId argument.
 *          DSP_EPENDING
 *              H/W specific error. The request can’t be serviced at this
 *              point of time.
 *          DSP_EFILE
 *              Invalid ImagePath parameter.
 *          DSP_EINVALIDSECTION
 *              Invalid section name.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFAIL
 *              General failure, unable to load section on DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  procId must be valid.
 *          imagePath must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    PROC_Attach, PROC_Load
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_LoadSection (IN    ProcessorId  procId,
                  IN    Char8 *      imagePath,
                  IN    Uint32       sectID) ;


/** ============================================================================
 *  @func   PROC_Read
 *
 *  @desc   This function allows GPP side applications to read from the DSP
 *          memory space.
 *
 *  @arg    procId
 *              Processor ID of the target DSP.
 *  @arg    dspAddr
 *              Address of the DSP memory region from where to read.
 *  @arg    numBytes
 *              Number of bytes to be read
 *  @arg    buffer
 *              User specified buffer to contain the data from DSP memory space
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPENDING
 *              H/W specific error. The request can’t be serviced at this
 *              point of time.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure, unable to load section on DSP.
 *
 *  @enter  procId must be valid.
 *          buffer must be valid.
 *
 *  @leave  None
 *
 *  @see    PROC_Write
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Read (IN     ProcessorId    procId,
           IN     Uint32         dspAddr,
           IN     Uint32         numBytes,
           IN OUT Pvoid          buffer) ;


/** ============================================================================
 *  @func   PROC_Write
 *
 *  @desc   This function allows GPP side applications to write to the DSP
 *          memory space
 *
 *  @arg    procId
 *              Processor ID of the target DSP.
 *  @arg    dspAddr
 *              Address of the DSP memory region from where the data is
 *              to be written to.
 *  @arg    numBytes
 *              Number of bytes to be written
 *  @arg    buffer
 *              User specified buffer with data to be written into DSP memory
 *              space
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EPENDING
 *              H/W specific error. The request can’t be serviced at this
 *              point of time.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure, unable to load section on DSP.
 *
 *  @enter  procId must be valid.
 *          buffer must be valid.
 *
 *  @leave  None
 *
 *  @see    PROC_Read
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Write (IN ProcessorId    procId,
            IN Uint32         dspAddr,
            IN Uint32         numBytes,
            IN Pvoid          buffer) ;


/** ============================================================================
 *  @func   PROC_Start
 *
 *  @desc   Starts execution of the loaded code on DSP from the starting
 *          point specified in the DSP executable loaded earlier by call to
 *          PROC_Load ().
 *
 *  @arg    procId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId argument.
 *          DSP_EPENDING
 *              H/W specific error. The request can’t be serviced at this
 *              point of time.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFAIL
 *              General failure, unable to start DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PROC_Attach, PROC_Load, PROC_Stop
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Start (IN ProcessorId procId) ;


/** ============================================================================
 *  @func   PROC_Stop
 *
 *  @desc   Stops the DSP.
 *
 *  @arg    procId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid ProcId argument.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFAIL
 *              General failure, unable to stop DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PROC_Attach, PROC_Load, PROC_Start
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Stop (IN ProcessorId procId) ;


/** ============================================================================
 *  @func   PROC_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations on
 *          the DSP.
 *
 *  @arg    procId
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
 *  @see    PROC_Attach
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Control (IN  ProcessorId procId,
              IN  Int32       cmd,
              OPT Pvoid       arg) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PROC_Instrument
 *
 *  @desc   Gets the instrumentation data associated with PROC sub-component.
 *
 *  @arg    procId
 *              Identifier for processor for which instrumentation
 *              information is to be obtained.
 *  @arg    retVal
 *              OUT argument to contain the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              retVal is invalid.
 *
 *  @enter  procId must be valid.
 *          retVal must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Instrument (IN ProcessorId procId, OUT ProcInstrument * retVal) ;
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PROC_Debug
 *
 *  @desc   Prints the debug information summarizing the current status
 *          of the PROC component.
 *
 *  @arg    procId
 *              Identifier for processor.
 *
 *  @ret    None
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
Void
PROC_Debug (IN ProcessorId procId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (PROC_H) */
