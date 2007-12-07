/** ============================================================================
 *  @file   shm.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the Shared Memory Link Driver interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (SHM_H)
#define SHM_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
 *  @const  SHM_GPP_TOKEN/SHM_DSP_TOKEN
 *
 *  @desc   Token used for handshake between GPP and DSP.
 *  ============================================================================
 */
#define SHM_GPP_TOKEN   0xBABA
#define SHM_DSP_TOKEN   0xC0C0


/** ============================================================================
 *  @name   SHM_Control
 *
 *  @desc   Defines the control structure used by data transfer protocol
 *          built over the Shared Memory Link Driver.
 *
 *  @field  handshakeGpp
 *              Handshake field to be updated by GPP.
 *  @field  handshakeDsp
 *              Handshake field to be updated by DSP.
 *  @field  dspFreeMask
 *              Mask indicating free channel from DSP
 *              (written by DSP/read by GPP)
 *  @field  gppFreeMask
 *              Mask indicating free channel from GPP
 *              (written by GPP/read by DSP)
 *  @field  inputFull
 *              Input channel has unread data
 *  @field  inputId
 *              Channel for which input is available
 *  @field  inputSize
 *              Size of data block (in DSP Words)
 *  @field  outputFull
 *              Output channel has unread data
 *  @field  OutputId
 *              Channel for which output is available
 *  @field  OutputSize
 *              Size of data block (in DSP Words)
 *  @field  argv
 *              reserved
 *  @field  resv
 *              reserved
 *  @field  dspFreeMsg
 *              Indicates whether a free message is available on the
 *              DSP. (written by DSP/read by GPP)
 *  @field  gppFreeMsg
 *              Indicates whether a free message is available on the
 *              GPP. (written by GPP/read by DSP)
 *  ============================================================================
 */
typedef struct SHM_Control_tag {
    volatile Uint16  handshakeGpp ;
    volatile Uint16  handshakeDsp ;
    volatile Uint16  dspFreeMask  ;
    volatile Uint16  gppFreeMask  ;
    volatile Uint16  inputFull    ;
    volatile Uint16  inputId      ;
    volatile Uint16  inputSize    ;
    volatile Uint16  outputFull   ;
    volatile Uint16  outputId     ;
    volatile Uint16  outputSize   ;
    volatile Uint16  argv         ;
    volatile Uint16  resv         ;
#if defined (MSGQ_COMPONENT)
    volatile Uint16  dspFreeMsg   ;
    volatile Uint16  gppFreeMsg   ;
#endif /* if defined (MSGQ_COMPONENT) */
} SHM_Control ;


/** ============================================================================
 *  @name   SHM_DriverInfo
 *
 *  @desc   Defines the driver information object for the Shared Memory
 *          Link Driver.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    linkId
 *              Link Identifier.
 *  @arg    intObj
 *              Interrupt object used by the Shared Memory link driver.
 *  @arg    isrObj
 *              ISR object used by the Shared Memory link driver.
 *  @arg    dpcObj
 *              DPC object used by the Shared Memory link driver.
 *  @field  ptrControl
 *              Pointer to the shared memory control structure.
 *  @field  ptrInpData
 *              Pointer to the data input area in the shared memory.
 *  @field  ptrOutData
 *              Pointer to the data output area in the shared memory.
 *  @field  maxBufSize
 *              Maximum size of buffer that can be transferred through the
 *              Shared Memory link driver.
 *  @field  lastOutput
 *              The channel ID on which last output was done.
 *  @field  outputMask
 *              Saved output mask for the link toward a processor.
 *  @field  outputMsg
 *              Indicates whether a message is available on the GPP
 *              to be sent to the DSP.
 *  ============================================================================
 */
typedef struct SHM_DriverInfo_tag {
    Uint32          dspId  ;
    Uint32          linkId ;

    InterruptObject intObj ;
    IsrObject *     isrObj ;
    DpcObject *     dpcObj ;

    SHM_Control *   ptrControl ;
    Uint8 *         ptrInpData ;
    Uint8 *         ptrOutData ;
    Uint32          maxBufSize ;

    Uint32          lastOutput ;
    Uint32          outputMask ;

#if defined (MSGQ_COMPONENT)
    Uint32          outputMsg  ;
#endif /* if defined (MSGQ_COMPONENT) */
} SHM_DriverInfo ;


/** ============================================================================
 *  @func   SHM_Initialize
 *
 *  @desc   This function allocates and initializes resources used by the
 *          Shared Memory Driver.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    linkId
 *              Link Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EMEMORY
 *              Out of memory.
 *
 *  @enter  Processor ID must be valid.
 *
 *  @leave  None.
 *
 *  @see    SHM_Finalize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_Initialize (IN ProcessorId dspId, IN LinkId lnkId) ;


/** ============================================================================
 *  @func   SHM_Finalize
 *
 *  @desc   This function de-allocates and finalizes resources used by the
 *          Shared Memory Driver.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    linkId
 *              Link Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *
 *  @enter  Processor ID must be valid.
 *
 *  @leave  None.
 *
 *  @see    SHM_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_Finalize (IN ProcessorId dspId, IN LinkId lnkId) ;


/** ============================================================================
 *  @func   SHM_OpenChannel
 *
 *  @desc   Open a channel for input/output.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not open the channel successfully.
 *
 *  @enter  dspId shall be valid.
 *          chnlId shall be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_OpenChannel (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   SHM_CloseChannel
 *
 *  @desc   Close a channel.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not open the channel successfully.
 *
 *  @enter  dspId shall be valid.
 *          chnlId shall be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_CloseChannel (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   SHM_CancelIO
 *
 *  @desc   All the requests for this driver has been cancelled.
 *          SHM driver clears the gppFreeMask bit or outputMask bit
 *          of that channel.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not open the channel successfully.
 *
 *  @enter  dspId shall be valid.
 *          chnlId shall be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_CancelIO (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   SHM_IO_Request
 *
 *  @desc   This function sends an interrupt to DSP, requesting data transfer.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              chnlId is invalid.
 *          DSP_EPOINTER
 *              Subcomponent is not initialized.
 *
 *  @enter  dspId shall be valid.
 *          chnlId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    LDRV_IO_Dispatch
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_IO_Request (IN ProcessorId dspId, IN ChannelId chnlId) ;

/** ============================================================================
 *  @func   SHM_ScheduleDPC
 *
 *  @desc   Schedules a DPC for IO on specified channel.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *
 *  @enter  This sub-component must have been initialized before calling
 *          this function.
 *          dspId shall be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_ScheduleDPC (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   SHM_HandshakeSetup
 *
 *  @desc   Does initialization of handshake fields in shared memory.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *
 *  @enter  dspId shall be valid.
 *
 *  @leave  None
 *
 *  @see    SHM_HandshakeComplete, LDRV_IO_Handshake
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_HandshakeSetup (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   SHM_HandshakeStart
 *
 *  @desc   Starts the handshake process toward specified DSP on this driver.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SHM_HandshakeComplete, LDRV_IO_Handshake
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_HandshakeStart (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   SHM_HandshakeComplete
 *
 *  @desc   Completes the handshake process toward specified DSP on this driver.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    SHM_HandshakeStart, LDRV_IO_Handshake
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SHM_HandshakeComplete (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   SHM_ISR
 *
 *  @desc   Interrupt handler for IO operations.
 *
 *  @arg    refData
 *              Argument to the interrupt handler allowing the interrupt
 *              handler to identify the associated SHM driver information.
 *
 *  @ret    None
 *
 *  @enter  refData shall not be NULL.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
SHM_ISR (IN Pvoid refData) ;


/** ============================================================================
 *  @func   SHM_DPC
 *
 *  @desc   Deferred procedure call for performing all IO. This function gets
 *          scheduled by IO_ISR when any IO is to be performed between DSP
 *          and GPP.
 *
 *  @arg    refData
 *              Argument to the delayed procedure call. It identifies the
 *              associated SHM driver information.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    IO_ISR, SHM_Dispatch
 *  ============================================================================
 */
NORMAL_API
Void
SHM_DPC (IN Pvoid refData) ;


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   SHM_Debug
 *
 *  @desc   Prints the current status of this subcomponent.
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
NORMAL_API
Void
SHM_Debug () ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SHM_H) */
