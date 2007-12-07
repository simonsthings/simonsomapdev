/** ============================================================================
 *  @file   pmgr_proc.h
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr
 *
 *  @desc   Defines the interfaces and data structures for the sub-component
 *          PMGR_PROC.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PMGR_PROC_H)
#define PMGR_PROC_H


USES (gpptypes.h)
USES (errbase.h)
USES (prcs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   PMGR_PROC_Setup
 *
 *  @desc   Sets up the necessary data structures for the PMGR_PROC
 *          sub-component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EACCESSDENIED
 *              Access denied. Another client has already called
 *              PMGR_PROC_Setup ().
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Destroy
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Setup () ;


/** ============================================================================
 *  @func   PMGR_PROC_Destroy
 *
 *  @desc   Destroys the data structures for the PMGR_PROC component,
 *          allocated earlier by a call to PROC_Setup ().
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EACCESSDENIED
 *              Access denied. Only the client who had successfully called
 *              PMGR_PROC_Setup () can call this function.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Setup
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Destroy () ;


/** ============================================================================
 *  @func   PMGR_PROC_Attach
 *
 *  @desc   Attaches the client to the specified DSP and also
 *          initializes the DSP (if required).
 *
 *  @arg    procId
 *             Processor Identifier.
 *  @arg    attr
 *             Attributes for the processor on which attach is to be done.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_SALREADYATTACHED
 *              Successful attach. Also, indicates that another client has
 *              already attached to DSP.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFAIL
 *              General failure, unable to attach to processor.
 *          DSP_EWRONGSTATE
 *              Incorrect state to complete the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Detach
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Attach (IN    ProcessorId   procId,
                  OPT   ProcAttr *    attr) ;


/** ============================================================================
 *  @func   PMGR_PROC_Detach
 *
 *  @desc   Detaches the client from specified processor.
 *          If the caller is the owner of the processor, this function releases
 *          all the resources that this component uses and puts the DSP in an
 *          unusable state (from application perspective).
 *
 *  @arg    procId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure, unable to detach.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EATTACHED
 *              Not attached to the target processor.
 *          DSP_EWRONGSTATE
 *              Incorrect state to complete the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Attach
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Detach (IN  ProcessorId   procId) ;


/** ============================================================================
 *  @func   PMGR_PROC_GetState
 *
 *  @desc   Gets the current status of DSP by querying the Link Driver.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    procState
 *              Placeholder for processor state.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure, couldn't get state information.
 *
 *  @enter  procId must be valid.
 *          The pointer to ProcState is valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Load, PMGR_PROC_Start, PMGR_PROC_Stop, PMGR_PROC_Idle
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_GetState (IN   ProcessorId   procId,
                    OUT  ProcState *   procState) ;


/** ============================================================================
 *  @func   PMGR_PROC_Load
 *
 *  @desc   Loads the specified DSP executable on the target DSP.
 *          It ensures that the caller owns the DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    imagePath
 *              Full path to the image file to load on DSP.
 *  @arg    argc
 *              Number of arguments to be passed to the DSP executable.
 *  @arg    argv
 *              Arguments to be passed to DSP executable.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EACCESSDENIED
 *              Access denied. Only the owner client is allowed to
 *              load the DSP executable.
 *          DSP_EFILE
 *              Invalid DSP executable.
 *          DSP_EFAIL
 *              General failure, unable to load DSP executable.
 *          DSP_EWRONGSTATE
 *              Incorrect state to complete the requested operation.
 *
 *  @enter  procId must be valid.
 *          Base image path must be valid.
 *          If argc is 0 then argv must be NULL pointer.
 *          If argc is non-zero then argv must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Attach, PMGR_PROC_LoadSection
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Load (IN ProcessorId  procId,
                IN Char8 *      imagePath,
                IN Uint32       argc,
                IN Char8 **     argv) ;


/** ============================================================================
 *  @func   PMGR_PROC_LoadSection
 *
 *  @desc   Loads the specified section of DSP executable onto the target DSP.
 *          It ensures that the client owns the DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    imagePath
 *              Full path to the image file.
 *  @arg    sectID
 *              Section ID of section to load.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFILE
 *              Invalid ImagePath parameter.
 *          DSP_EINVALIDSECTION
 *              Invalid section id.
 *          DSP_EACCESSDENIED
 *              Access denied. Only the owner client is allowed to
 *              load section on the DSP.
 *          DSP_EFAIL
 *              General failure, unable to load section on DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state to complete the requested operation.
 *
 *  @enter  procId must be valid.
 *          Base image path must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Attach, PMGR_PROC_Load
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_LoadSection (IN ProcessorId  procId,
                       IN Char8 *      imagePath,
                       IN Uint32       sectID) ;


/** ============================================================================
 *  @func   PMGR_PROC_Start
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
 *          DSP_EACCESSDENIED
 *              Access denied. Only the owner client is allowed
 *              to start the DSP.
 *          DSP_EFAIL
 *              General failure, unable to start DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state to complete the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Attach, PMGR_PROC_Load, PMGR_PROC_Stop
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Start (IN  ProcessorId  procId) ;


/** ============================================================================
 *  @func   PMGR_PROC_Stop
 *
 *  @desc   Stops the specified DSP.
 *
 *  @arg    procId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EACCESSDENIED
 *              Access denied. Only the owner client is allowed
 *              to stop the DSP.
 *          DSP_EFAIL
 *              General failure, unable to stop DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state to complete the requested operation.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Attach, PMGR_PROC_Load, PMGR_PROC_Start
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Stop (IN   ProcessorId  procId) ;


/** ============================================================================
 *  @func   PMGR_PROC_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations.
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
 *  @see    PMGR_PROC_Attach
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_Control (IN  ProcessorId procId,
                   IN  Int32       cmd,
                   OPT Pvoid       arg) ;


/** ============================================================================
 *  @func   PMGR_PROC_IsAttached
 *
 *  @desc   Function to check whether the client identified by the specified
 *          'client' object is attached to the specified processor.
 *
 *  @arg    procId
 *              Processor Id.
 *  @arg    client
 *              Client identifier.
 *  @arg    isAttached
 *              Place holder for flag indicating the client is attached.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_PROC_Attach
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_PROC_IsAttached (IN  ProcessorId  procId,
                      IN  PrcsObject * client,
                      OUT Bool *       isAttached) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PMGR_PROC_Instrument
 *
 *  @desc   Gets the instrumentation data associated with PMGR_PROC
 *          sub-component.
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
NORMAL_API
DSP_STATUS
PMGR_PROC_Instrument (IN ProcessorId procId, OUT ProcInstrument * retVal) ;
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PMGR_PROC_Debug
 *
 *  @desc   Prints the debug information summarizing the current status
 *          of the PMGR_PROC component.
 *
 *  @arg    procId
 *              Identifier for processor.
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
PMGR_PROC_Debug (IN ProcessorId procId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (PMGR_PROC_H) */
