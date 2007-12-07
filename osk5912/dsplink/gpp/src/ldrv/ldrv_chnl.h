/** ============================================================================
 *  @file   ldrv_chnl.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Link driver's channel module interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LDRV_CHNL_H)
#define LDRV_CHNL_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)
USES (errbase.h)


/** ============================================================================
 *  @const  IO Completion State flags.
 *
 *  @desc   Status of completion.
 *  ============================================================================
 */
#define LDRV_CHNL_IOCSTATE_COMPLETE     0x0000  /* IO Completed.             */
#define LDRV_CHNL_IOCSTATE_CANCELED     0x0002  /* IO was cancelled          */
#define LDRV_CHNL_IOCSTATE_TIMEOUT      0x0008  /* Wait for IOC timed out.   */
#define LDRV_CHNL_IOCSTATE_EOS          0x8000  /* End Of Stream reached.    */


/** ============================================================================
 *  @name   ChannelState
 *
 *  @desc   Channel State type.
 *
 *  @field  ChannelState_Ready
 *              Indicates channel is ready.
 *  @field  ChannelState_Idled
 *              Indicates channel is idled.
 *  @field  ChannelState_EOS
 *              Indicates channel is in End of Stream state.
 *  @field  ChannelState_Closed
 *              Indicates channel is in closed state.
 *  ============================================================================
 */
typedef enum  {
    ChannelState_Ready     = 0x01,
    ChannelState_Idled     = 0x02,
    ChannelState_EOS       = 0x04,
    ChannelState_Closed    = 0x08
} ChannelState ;

/** ============================================================================
 *  @name   IOState
 *
 *  @desc   Completion state of IO on a channel.
 *
 *  @field  IOState_Completed
 *              Indicates completion of IO for an IO request on a channel.
 *  @field  IOState_NotCompleted
 *              Indicates non-completion of IO for an IO request on a channel.
 *  ============================================================================
 */
typedef enum  {
    IOState_Completed    = 1,
    IOState_NotCompleted = 2
} IOState ;

/** ============================================================================
 *  @name   FnLdrvChnlCallback
 *
 *  @desc   Signature of callback function that runs when the buffer has been
 *          received from / sent to the DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    statusOfIo
 *              Status of the IO requested.
 *  @arg    buffer
 *              Pointer to the message buffer.
 *  @arg    size
 *              Size of the message buffer.
 *  @arg    arg
 *              Argument given to the callback function.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLdrvChnlCallback) (ProcessorId   procId,
                                           DSP_STATUS    statusOfIo,
                                           Uint8  *      buffer,
                                           Uint32        size,
                                           Pvoid         arg) ;

/** ============================================================================
 *  @name   LDRVChnlIOInfo
 *
 *  @desc   LDRV Channel IO information structure.
 *
 *  @field  buffer
 *              Pointer to data buffer.
 *  @field  size
 *              Size of the data buffer.
 *  @field  arg
 *              Argument to be sent or received.
 *  @field  completionStatus
 *              Completion status of this IO request.
 *  @field  callback
 *              Optional callback function pointer.
 *  @field  dpcContext
 *              Indicates whether the execution is in DPC context.
 *  ============================================================================
 */
typedef struct LDRVChnlIOInfo_tag {
    Pvoid               buffer            ;
    Uint32              size              ;
    Uint32              arg               ;
    IOState             completionStatus  ;
    FnLdrvChnlCallback  callback          ;
    Bool                dpcContext        ;
} LDRVChnlIOInfo ;

/** ============================================================================
 *  @name   LDRVChnlObject
 *
 *  @desc   LDRV Channel object.
 *
 *  @field  signature
 *              Signature of object.
 *  @field  chnlState
 *              State of channel.
 *  @field  freeList
 *              List for free chirps.
 *  @field  requestList
 *              List for requested chirps.
 *  @field  completedList
 *              List for completed chirps.
 *  @field  attrs
 *              Attributes of this channel.
 *  @field  syncEvent
 *              Event to be signaled when some IO is completed for this
 *              channel.
 *  @field  chnlIdleSync
 *              Sync event used by channel idle function call.
 *  ============================================================================
 */
typedef struct LDRVChnlObject_tag {
    Uint32          signature     ;
    ChannelState    chnlState     ;
    List *          freeList      ;
    List *          requestList   ;
    List *          completedList ;
    ChannelAttrs    attrs         ;
    SyncEvObject *  syncEvent     ;
    SyncEvObject *  chnlIdleSync  ;
} LDRVChnlObject ;

/** ============================================================================
 *  @name   LDRVChnlIRP
 *
 *  @desc   CHIRP (Channel Input/output Request Packet) data structure.
 *
 *  @field  link
 *              List element header needed for this structure.
 *  @field  buffer
 *              Buffer to be filled/emptied.
 *  @field  arg
 *              Issue reclaim argument.
 *  @field  size
 *              Buffer length.
 *  @field  iocStatus
 *              Status of IO Completion.
 *  @field  callback
 *              Optional callback function.
 *  ============================================================================
 */
typedef struct LDRVChnlIRP_tag {
    ListElement         link          ;
    Uint8  *            buffer        ;
    Uint32              arg           ;
    Uint32              size          ;
    Uint32              iocStatus     ;
    FnLdrvChnlCallback  callback      ;
} LDRVChnlIRP ;


/** ============================================================================
 *  @func   LDRV_CHNL_Initialize
 *
 *  @desc   Create and initializes resources needed for channel objects
 *          (maintained internally).
 *          These channels can be opened later on by call to LDRV_CHNL_Open.
 *
 *  @arg    procId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None.
 *
 *  @see    LDRV_CHNL_Finalize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Initialize (IN ProcessorId procId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_Finalize
 *
 *  @desc   Deletes all channel objects created for this DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *
 *  @leave  None.
 *
 *  @see    LDRV_CHNL_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Finalize (IN ProcessorId procId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_Open
 *
 *  @desc   Opens and prepares a channel for use.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    attrs
 *              Channel attributes.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EINVALIDARG
 *              Invaild arguments.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *          attrs must be a valid pointer.
 *
 *  @leave  Channel must be in Idled state in case of success.
 *
 *  @see    LDRV_CHNL_Close
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Open (IN ProcessorId      procId,
                IN ChannelId        chnlId,
                IN ChannelAttrs *   attrs) ;


/** ============================================================================
 *  @func   LDRV_CHNL_Close
 *
 *  @desc   Closes the channel. No I/O can be performed on a closed channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EINVALIDARG
 *              Invaild arguments.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    LDRV_CHNL_Open
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Close (IN ProcessorId procId,
                 IN ChannelId   chnlId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_AddIORequest
 *
 *  @desc   Adds an IO request to a channel. An IO request may be a request
 *          for transferring a buffer from GPP side to DSP side or vice-versa.
 *          The direction of data transfer is decided by the mode of channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    ioInfo
 *              Information of IO to be performed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EINVALIDARG
 *              Invaild arguments.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *          ioInfo must be a valid pointer.
 *
 *  @leave  None.
 *
 *  @see    LDRV_CHNL_GetIOCompletion
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_AddIORequest (IN ProcessorId      procId,
                        IN ChannelId        chnlId,
                        IN LDRVChnlIOInfo * ioInfo) ;


/** ============================================================================
 *  @func   LDRV_CHNL_GetIOCompletion
 *
 *  @desc   Waits for a specified amount of time for an I/O completion event
 *          on a channel.
 *          Upon successful completion, a buffer is returned as part of ioInfo
 *          structure. A filled buffer is returned in case of an 'Input' channel
 *          and an empty buffer is returned in case of an 'Output' channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    timeout
 *              Timeout for waiting.
 *  @arg    ioInfo
 *              Information of completed request.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EINVALIDARG
 *              Invaild arguments.
 *          DSP_EFAIL
 *              General failure.
 *          CHNL_E_NOIOC
 *              Timeout parameter was "NO_WAIT", yet no I/O completions were
 *              queued.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *          ioInfo must be a valid pointer.
 *
 *  @leave  buffer returned as part of ioInfo is non-NULL in case of
 *          successful completion, otherwise it is NULL.
 *
 *  @see    LDRV_CHNL_AddIORequest
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_GetIOCompletion (IN  ProcessorId      procId,
                           IN  ChannelId        chnlId,
                           IN  Uint32           timeout,
                           OUT LDRVChnlIOInfo * ioInfo) ;


/** ============================================================================
 *  @func   LDRV_CHNL_AddIOCompletion
 *
 *  @desc   Notification for the completion of IO.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    chirp
 *              CHIRP on which completion is done.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invaild arguments.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *          chirp must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_AddIOCompletion (IN ProcessorId   procId,
                           IN ChannelId     chnlId,
                           IN LDRVChnlIRP * chirp) ;


/** ============================================================================
 *  @func   LDRV_CHNL_Idle
 *
 *  @desc   In case of input mode channel this function discards all pending
 *          input requests from the channel. In case of output mode channel,
 *          action of this function depends upon the flush parameter and is as
 *          follows:
 *          If flush is TRUE this function will block till all output buffers
 *          are transferred to the DSP.
 *          If flush is FALSE this function will discard all the output
 *          requests pending on this channel without blocking.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    flush
 *              Boolean parameter tells whether to block or not for output mode
 *              channels.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Memory error occurred.
 *          DSP_EINVALIDARG
 *              Invaild arguments.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *          flush argument must be valid.
 *
 *  @leave  IO request list is empty in case of successful completion.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Idle (IN ProcessorId  procId,
                IN ChannelId    chnlId,
                IN Bool         flush) ;


/** ============================================================================
 *  @func   LDRV_CHNL_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations
 *          on channels.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    cmd
 *              Command id.
 *  @arg    arg
 *              Optional argument for the specified command.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Control (IN  ProcessorId    procId,
                   IN  ChannelId      chnlId,
                   IN  Int32          cmd,
                   OPT Pvoid          arg) ;


/** ============================================================================
 *  @func   LDRV_CHNL_GetChannelMode
 *
 *  @desc   Gets the channel mode.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    mode
 *              Channel mode.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
ChannelMode
LDRV_CHNL_GetChannelMode (IN ProcessorId   procId,
                          IN ChannelId     chnlId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_GetChannelState
 *
 *  @desc   Gets the channel state.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    Channel state.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
ChannelState
LDRV_CHNL_GetChannelState (IN ProcessorId   procId,
                           IN ChannelId     chnlId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_SetChannelState
 *
 *  @desc   Sets the channel state.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    state
 *              State of the channel to be set.
 *
 *  @ret    None.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
LDRV_CHNL_SetChannelState (IN ProcessorId   procId,
                           IN ChannelId     chnlId,
                           IN ChannelState  state) ;


/** ============================================================================
 *  @func   LDRV_CHNL_GetChannelEndianism
 *
 *  @desc   Gets channel endianism information.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    endianism information
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Endianism
LDRV_CHNL_GetChannelEndianism (IN ProcessorId   procId,
                               IN ChannelId     chnlId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_ChannelHasMoreChirps
 *
 *  @desc   Returns TRUE if the channel has more chirps in the IO request queue.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    TRUE
 *              Channel has more chirps in IO request queue.
 *          FALSE
 *              Channel does not have more chirps available in the queue.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Bool
LDRV_CHNL_ChannelHasMoreChirps (IN ProcessorId   procId,
                                IN ChannelId     chnlId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_GetRequestChirp
 *
 *  @desc   Gets a chirp from request queue of a channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    CHIRP from requested queue.
 *
 *  @enter  procId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
LDRVChnlIRP *
LDRV_CHNL_GetRequestChirp (IN ProcessorId   procId,
                           IN ChannelId     chnlId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_HandshakeSetup
 *
 *  @desc   It does initializations related to handshake procedure that are
 *          required before starting of DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_HandshakeSetup (IN ProcessorId procId) ;


/** ============================================================================
 *  @func   LDRV_CHNL_Handshake
 *
 *  @desc   Does the necessary handshake (if required) between for the channels
 *          across GPP & DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Handshake (IN ProcessorId procId) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LDRV_CHNL_Instrument
 *
 *  @desc   Gets the instrumentation information related to the specified
 *          channel.
 *
 *  @arg    procId
 *              Identifier for processor.
 *  @arg    chnlId
 *              Identifier for channel for which instrumentation information
 *              is to be obtained.
 *  @arg    retVal
 *              OUT argument to contain the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              retVal is invalid.
 *
 *  @enter  procId must be valid.
 *          chirp must be valid.
 *          retVal must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_CHNL_Instrument (IN  ProcessorId       procId,
                      IN  ChannelId         chnlId,
                      OUT ChnlInstrument *  retVal) ;
#endif /* defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_CHNL_Debug
 *
 *  @desc   Prints out debug information of CHNL module. It will print all
 *          the important data structures and variable of this module.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    None.
 *
 *  @enter  procId must be valid.
 *          chirp must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
LDRV_CHNL_Debug (ProcessorId procId, ChannelId chnlId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LDRV_CHNL_H) */
