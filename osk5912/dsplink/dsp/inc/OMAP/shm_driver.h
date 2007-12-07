/** ============================================================================
 *  @file   shm_driver.h
 *
 *  @path   $(DSPLINK)\dsp\inc\OMAP
 *
 *  @desc   Defines the interface of DSPLINK Main Interface Module. It provides
 *          the standard minidriver function interface for DSPLINK.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef SHM_DRIVER_
#define SHM_DRIVER_


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @name   DSPLINK_ChnlParams
 *
 *  @desc   Channel parameters to be passed while creating channels.
 *
 *  @field  maxBufferSize
 *              Maximum size of buffer that this channel will handle.
 *              - A value of -1 indicates buffer size is maximum allowed by the
 *                shared memory size.
 *  @field  maxPendingIOs
 *              Maximum buffers that can pend on this channel.
 *              - A value of -1 indicates no restriction on buffer queueing.
 *  ============================================================================
 */
typedef struct DSPLINK_ChnlParams_tag {
    Int maxBufferSize;
    Int maxPendingIOs;
} DSPLINK_ChnlParams;

/** ============================================================================
 *  @name   DSPLINK_DevParams
 *
 *  @desc   Device parameters of the SHM link driver.
 *
 *  @field  pid
 *              Processor Identifier. Not used currently.
 *  @field  numChannels
 *              Number of channels that can be opened for this device.
 *  ============================================================================
 */
typedef struct DSPLINK_DevParams_tag {
    Int pid;
    Int numChannels;
} DSPLINK_DevParams;


/** ============================================================================
 *  @func   DSPLINK_init
 *
 *  @desc   Initializes DSPLINK data structures before bind function.
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
extern Void DSPLINK_init();


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* SHM_DRIVER_ */
