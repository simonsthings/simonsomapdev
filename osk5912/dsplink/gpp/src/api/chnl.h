/** ============================================================================
 *  @file   chnl.h
 *
 *  @path   $(DSPLINK)\gpp\src\api
 *
 *  @desc   Defines the interfaces and data structures for the API sub-component
 *          CHNL.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (CHNL_H)
#define  CHNL_H


USES (gpptypes.h)
USES (errbase.h)
USES (linkdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   CHNL_Create
 *
 *  @desc   Creates resources used for transferring data between GPP and DSP.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Id to open.
 *  @arg    attrs
 *              Channel attributes - if NULL, default attributes are applied.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *          Attributes must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Create (IN ProcessorId      procId,
             IN ChannelId        chnlId,
             IN ChannelAttrs *   attrs) ;


/** ============================================================================
 *  @func   CHNL_Delete
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
 *          DSP_EINVALIDARG
 *              Invalid Parameter passed.
 *
 *  @enter  Channels for specified processors should have been initialized.
 *          Processor and  channel ids should be valid.
 *
 *  @leave  None
 *
 *  @see    CHNL_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Delete (IN ProcessorId    procId,
             IN ChannelId      chnlId) ;


/** ============================================================================
 *  @func   CHNL_AllocateBuffer
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
 *          DSP_EINVALIDARG
 *              Invalid parameter passed.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    CHNL_Create, CHNL_FreeBuffer
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_AllocateBuffer (IN  ProcessorId procId,
                     IN  ChannelId   chnlId,
                     OUT Char8 **    bufArray,
                     IN  Uint32      size,
                     IN  Uint32      numBufs) ;


/** ============================================================================
 *  @func   CHNL_FreeBuffer
 *
 *  @desc   Frees buffer(s) allocated by CHNL_AllocateBuffer.
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
 *          DSP_EINVALIDARG
 *              Invalid parameter passed.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    CHNL_Create, CHNL_AllocateBuffer
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_FreeBuffer (IN ProcessorId procId,
                 IN ChannelId   chnlId,
                 IN Char8 **    bufArray,
                 IN Uint32      numBufs) ;


/** ============================================================================
 *  @func   CHNL_Issue
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
 *          DSP_EINVALIDARG
 *              Invalid parameter passed.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    CHNL_Reclaim
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Issue (IN ProcessorId      procId,
            IN ChannelId        chnlId,
            IN ChannelIOInfo *  ioReq) ;


/** ============================================================================
 *  @func   CHNL_Reclaim
 *
 *  @desc   Gets the buffer back that has been issued to this channel.
 *          This call blocks for specified timeout value ranging from NO_WAIT
 *          to WAIT_FOREVER.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *  @arg    timeout
 *              Timeout value for this operation.
 *              Unit of timeout is OS dependent.
 *  @arg    ioReq
 *              Information needed for doing reclaim.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *          DSP_EMEMORY
 *              Operation failed due to memory error.
 *          DSP_EINVALIDARG
 *              Invalid parameter passed.
 *          CHNL_E_NOIOC
 *              Timeout parameter was "NO_WAIT", yet no I/O completions were
 *              queued.
 *
 *  @enter  Channels for specified processors must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    CHNL_Issue
 *  ============================================================================
 */
EXPORT_API
EXPORT_API
DSP_STATUS
CHNL_Reclaim (IN     ProcessorId       procId,
              IN     ChannelId         chnlId,
              IN     Uint32            timeout,
              IN OUT ChannelIOInfo *   ioReq) ;


/** ============================================================================
 *  @func   CHNL_Idle
 *
 *  @desc   In the input mode channel, this function resets the channel
 *          and causes any currently buffered input data to be discarded.
 *
 *          In the output mode channel, this function causes any currently
 *          queued buffers to be transferred through the channel. It causes
 *          the client to wait for as long as it takes for the data to be
 *          transferred through the channel.
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
 *          DSP_EINVALIDARG
 *              Invalid parameter passed.
 *
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    CHNL_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Idle (IN ProcessorId  procId,
           IN ChannelId    chnlId) ;


/** ============================================================================
 *  @func   CHNL_Flush
 *
 *  @desc   Discards all the requested buffers that are pending for transfer
 *          both in case of input mode channel as well as output mode channel.
 *          One must still have to call the CHNL_Reclaim to get back the
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
 *          DSP_EINVALIDARG
 *              Invalid parameter passed.
 *
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    CHNL_Create, CHNL_Issue
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Flush (IN ProcessorId         procId,
            IN ChannelId           chnlId) ;


/** ============================================================================
 *  @func   CHNL_Control
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
 *          DSP_EINVALIDARG
 *              Invalid parameter passed.
 *
 *  @enter  Channels for specified processor must be initialized.
 *          Processor and  channel ids must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Control (IN  ProcessorId    procId,
              IN  ChannelId      chnlId,
              IN  Int32          cmd,
              OPT Pvoid          arg) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   CHNL_Instrument
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
 *  @enter  retVal must be a valid pointer.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
CHNL_Instrument (IN  ProcessorId      procId,
                 IN  ChannelId        chnlId,
                 OUT ChnlInstrument * retVal) ;
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   CHNL_Debug
 *
 *  @desc   Prints the current status of CHNL subcomponent.
 *
 *  @arg    procId
 *              Identifier for processor.
 *  @arg    chnlId
 *              Identifier for channel.
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
CHNL_Debug (IN ProcessorId procId,
            IN ChannelId   chnlId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (CHNL_H) */
