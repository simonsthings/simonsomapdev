/** ============================================================================
 *  @file   hpi_driver.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the HPI link driver between 64x DSP on DM642
 *          and ARM on DM310.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HPI_DRIVER_H)
#define HPI_DRIVER_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   HPI_Initialize
 *
 *  @desc   This function allocates and initializes resources used by the
 *          HPI link Driver.
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
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None.
 *
 *  @see    HPI_Finalize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_Initialize (IN ProcessorId dspId, IN LinkId lnkId) ;


/** ============================================================================
 *  @func   HPI_Finalize
 *
 *  @desc   This function releases the resources allocated earlier by this
 *          link driver in call to HPI_Initialize (). It disables & uninstalls
 *          the Interrupt Service Routine and Delayed Procedure Call.
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
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None.
 *
 *  @see    HPI_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_Finalize (IN ProcessorId dspId, IN LinkId lnkId) ;


/** ============================================================================
 *  @func   HPI_OpenChannel
 *
 *  @desc   This function opens a channel to be used for data transfer.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_CloseChannel
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_OpenChannel (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   HPI_CloseChannel
 *
 *  @desc   This function closes the physical channel corresponding to the
 *          specified logical channel.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not close the channel successfully.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_OpenChannel
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_CloseChannel (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   HPI_CancelIO
 *
 *  @desc   This function cancels IO on a channel.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_OpenChannel, HPI_CloseChannel
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_CancelIO (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   HPI_ISR
 *
 *  @desc   This function is the interrupt service routine for interrupt used
 *          for data transfer on the physical link between GPP and target DSP.
 *
 *  @arg    refData
 *              Argument to the interrupt handler allowing the interrupt
 *              handler to identify the associated link driver information.
 *
 *  @ret    None.
 *
 *  @enter  refData must not be NULL.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_DPC
 *  ============================================================================
 */
NORMAL_API
Void
HPI_ISR (IN Pvoid refData) ;


/** ============================================================================
 *  @func   HPI_DPC
 *
 *  @desc   This function is the delayed procedure for performing actual data
 *          transfer on the physical link between GPP and target DSP.
 *          It runs at a priority level between interrupt service routines and
 *          normal tasks/processes/threads.
 *
 *  @arg    refData
 *              Argument to the delayed procedure call. It identifies the
 *              associated link driver information.
 *
 *  @ret    None
 *
 *  @enter  refData must not be NULL.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_ISR
 *  ============================================================================
 */
NORMAL_API
Void
HPI_DPC (IN Pvoid refData) ;


/** ============================================================================
 *  @func   HPI_ScheduleDPC
 *
 *  @desc   This function schedules the DPC to perform IO operation.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Could not close the channel successfully.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_ScheduleDPC (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   HPI_IO_Request
 *
 *  @desc   This function submits an IO request to the DSP.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid arguments passed.
 *          DSP_EPOINTER
 *              Subcomponent is not initialized.
 *
 *  @enter  dspId must be valid.
 *          chnlId must be valid.
 *
 *  @leave  None.
 *
 *  @see    HPI_Initialize, HPI_OpenChannel
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_IO_Request (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   HPI_HandshakeSetup
 *
 *  @desc   This function initializes the memory area used synchronizing
 *          the DSP with GPP.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_HandshakeStart, HPI_HandshakeComplete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_HandshakeSetup (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   HPI_HandshakeStart
 *
 *  @desc   This function initiates the handshake on the physical link with
 *          the target DSP.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_HandshakeSetup, HPI_HandshakeComplete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_HandshakeStart (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   HPI_HandshakeComplete
 *
 *  @desc   This function completes the handshake on the physical link with
 *          the target DSP. This function block until handshake completes.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  dspId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_Initialize, HPI_HandshakeSetup, HPI_HandshakeComplete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_HandshakeComplete (IN ProcessorId dspId) ;


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   HPI_Debug
 *
 *  @desc   Prints information related to HPI Link Driver.
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
HPI_Debug () ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (HPI_DRIVER_H) */
