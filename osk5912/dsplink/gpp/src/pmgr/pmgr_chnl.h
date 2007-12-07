/** ============================================================================
 *  @file   pmgr_chnl.h
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr
 *
 *  @desc   Defines the interfaces and data structures for the sub-component
 *          PMGR_CHNL.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PMGR_CHNL_H)
#define  PMGR_CHNL_H


USES (gpptypes.h)
USES (errbase.h)
USES (linkdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   PMGR_CHNL_Initialize
 *
 *  @desc   Sets up all channel objects in Link Driver.
 *
 *  @arg    procId
 *              Processor Id.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Processor Id must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Finalize, PMGR_CHNL_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Initialize (IN ProcessorId    procId) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Finalize
 *
 *  @desc   Releases all channel objects setup in Link Driver.
 *
 *  @arg    procId
 *              Processor Id.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processor must be initialized.
 *          Processor Id must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize, PMGR_CHNL_Create, PMGR_CHNL_Destroy
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Finalize (IN ProcessorId    procId) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Create
 *
 *  @desc   Creates resources used for transferring data between GPP and DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Id to open.
 *  @arg    attrs
 *              Channel attributes.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *          Attributes must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Create (IN ProcessorId    procId,
                  IN ChannelId      chnlId,
                  IN ChannelAttrs * attrs) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Delete
 *
 *  @desc   Releases channel resources used for transferring data between GPP
 *          and DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processors should have been initialized.
 *          Processor and  channel ids should be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Delete (IN ProcessorId    procId,
                  IN ChannelId      chnlId) ;


/** ============================================================================
 *  @func   PMGR_CHNL_AllocateBuffer
 *
 *  @desc   Allocates an array of buffers of specified size and returns them
 *          to the client.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    size
 *              Size of each buffer.
 *  @arg    bufArray
 *              Pointer to receive array of allocated buffers.
 *  @arg    numBufs
 *              Number of buffers to allocate.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *          bufArray must be valid.
 *          numBufs must be less than maximum limit.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize, PMGR_CHNL_Create, PMGR_CHNL_FreeBuffer
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_AllocateBuffer (IN  ProcessorId procId,
                          IN  ChannelId   chnlId,
                          OUT Char8 **    bufArray,
                          IN  Uint32      size,
                          IN  Uint32      numBufs) ;


/** ============================================================================
 *  @func   PMGR_CHNL_FreeBuffer
 *
 *  @desc   Frees buffer(s) allocated by PMGR_CHNL_AllocateBuffer.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    bufArray
 *              Pointer to the array of buffers to freed.
 *  @arg    numBufs
 *              Number of buffers to be freed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *          bufArray must be valid.
 *          numBufs must be less than maximum limit.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize, PMGR_CHNL_Create, PMGR_CHNL_AllocateBuffer
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_FreeBuffer (IN ProcessorId procId,
                      IN ChannelId   chnlId,
                      IN Char8 **    bufArray,
                      IN Uint32      numBufs) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Issue
 *
 *  @desc   Issues an input or output request on a specified channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    ioReq
 *              Information regarding IO.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *          ioReq must be valid pointer.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Reclaim
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Issue (IN ProcessorId      procId,
                 IN ChannelId        chnlId,
                 IN ChannelIOInfo *  ioReq) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Reclaim
 *
 *  @desc   Gets the buffer back that has been issued to this channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    timeout
 *              Timeout value for this operation.
 *  @arg    ioReq
 *              Information needed for doing reclaim.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          CHNL_E_NOIOC
 *              Timeout parameter was "NO_WAIT", yet no I/O completions were
 *              queued.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *          ioReq must be valid pointer.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Issue
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Reclaim (IN     ProcessorId       procId,
                   IN     ChannelId         chnlId,
                   IN     Uint32            timeout,
                   IN OUT ChannelIOInfo *   ioReq) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Idle
 *
 *  @desc   If the channel is an input stream this function resets the channel
 *          and causes any currently buffered input data to be discarded.  If
 *          the channel is an output channel, this function causes any
 *          currently queued buffers to be transferred through the channel. It
 *          causes the client to wait for as long as it takes for the data to
 *          be transferred through the channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize, PMGR_CHNL_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Idle (IN ProcessorId  procId,
                IN ChannelId    chnlId) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Flush
 *
 *  @desc   Discards all the requested buffers that are pending for transfer
 *          both in case of input mode channel as well as output mode channel.
 *          One must still have to call the PMGR_CHNL_Reclaim to get back the
 *          discarded buffers.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize, PMGR_CHNL_Create, PMGR_CHNL_Issue
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Flush (IN ProcessorId         procId,
                 IN ChannelId           chnlId) ;


/** ============================================================================
 *  @func   PMGR_CHNL_Control
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
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Control (IN  ProcessorId    procId,
                   IN  ChannelId      chnlId,
                   IN  Int32          cmd,
                   OPT Pvoid          arg) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PMGR_CHNL_Instrument
 *
 *  @desc   Gets the instrumentation information related to CHNL's
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
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *          retVal must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
PMGR_CHNL_Instrument (IN  ProcessorId       procId,
                      IN  ChannelId         chnlId,
                      OUT ChnlInstrument *  retVal) ;
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PMGR_CHNL_Debug
 *
 *  @desc   Prints the current status of CHNL subcomponent.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    None
 *
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    PMGR_CHNL_Initialize
 *  ============================================================================
 */
NORMAL_API
Void
PMGR_CHNL_Debug (ProcessorId procId, ChannelId chnlId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (PMGR_CHNL_H) */

