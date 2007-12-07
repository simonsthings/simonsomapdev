/** ============================================================================
 *  @file   ldrv_io.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines interfaces exposed by LDRV_IO subcomponent. These services
 *          are used by LDRV_CHNL for performing IO operations. It uses
 *          services from (LDRV_)DSP subcomponent for carrying out the tasks.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LDRV_IO_H)
#define LDRV_IO_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   LDRV_IO_Initialize
 *
 *  @desc   This function allocates and initializes resources used by this
 *          component and registers interrupt handler for handling data
 *          transfer interrupts from DSP.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EMEMORY
 *              Out of memory
 *
 *  @enter  dspId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    LDRV_IO_Finalize, IO_ISR, LDRV_IO_DPC
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Initialize (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_IO_Finalize
 *
 *  @desc   This function finalizes the I/O module for a particular DSP.
 *          Finalizing means No further services will be provided by this
 *          module for this particular DSP.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EMEMORY
 *              Out of memory
 *
 *  @enter  dspId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    LDRV_IO_Initialize
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Finalize (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_IO_OpenChannel
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
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_OpenChannel (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   LDRV_IO_CloseChannel
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
 *              Could not close the channel successfully.
 *
 *  @enter  dspId shall be valid.
 *          chnlId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_CloseChannel (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   LDRV_IO_Cancel
 *
 *  @desc   Cancel a channel.
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
 *  @enter  dspId shall be valid.
 *          chnlId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    None.

 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Cancel (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   LDRV_IO_Request
 *
 *  @desc   This function sends an IO request on specified channel to the
 *          link driver.
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
 *
 *  @enter  dspId shall be valid.
 *          chnlId shall be valid.
 *          This sub-component must have been initialized before calling
 *          this function.
 *
 *  @leave  None.
 *
 *  @see    LDRV_IO_Dispatch
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Request (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   LDRV_IO_ScheduleDPC
 *
 *  @desc   Schedules DPC for IO with the DSP specified by dspId.
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
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_ScheduleDPC (IN ProcessorId dspId, IN ChannelId chnlId) ;


/** ============================================================================
 *  @func   LDRV_IO_HandshakeSetup
 *
 *  @desc   Does necessary initializations for handshake procedure.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  dspId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_HandshakeSetup (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   LDRV_IO_Handshake
 *
 *  @desc   Does the necessary handshake (if required) between for the links
 *          across GPP & DSP.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    None.
 *
 *  @enter  dspId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_IO_Handshake (IN ProcessorId dspId) ;


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_IO_Debug
 *
 *  @desc   Prints the current status of this subcomponent.
 *
 *  @arg    dspId
 *              Processor Identifier.
 *
 *  @ret    None.
 *
 *  @enter  dspId shall be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
LDRV_IO_Debug (IN  ProcessorId  dspId) ;
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LDRV_IO_H) */
