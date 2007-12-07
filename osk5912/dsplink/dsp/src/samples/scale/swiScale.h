/** ============================================================================
 *  @file   swiScale.h
 *
 *  @path   $(DSPLINK)\dsp\src\samples\scale
 *
 *  @desc   Header file for swiScale application.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef SWISCALE_
#define SWISCALE_

/*  ----------------------------------- DSP/BIOS Headers            */
#include <msgq.h>

#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  SWI_INPUT_CHANNEL
 *
 *  @desc   Name of the input channel.
 *  ============================================================================
 */
#define SWI_INPUT_CHANNEL          "/dsplink0"

/** ============================================================================
 *  @const  SWI_OUTPUT_CHANNEL
 *
 *  @desc   Name of the output channel.
 *  ============================================================================
 */
#define SWI_OUTPUT_CHANNEL         "/dsplink1"

/** ============================================================================
 *  @const  SWI_DSP_MSGQNAME
 *
 *  @desc   Name of the message queue on the DSP.
 *  ============================================================================
 */
#define SWI_DSP_MSGQNAME           "DSPLINK_DSP00MSGQ00"

/** ============================================================================
 *  @name   SWISCALE_TransferInfo
 *
 *  @desc   Structure used to keep various information needed by various phases
 *          of the application.
 *
 *  @field  gioInputChan
 *              Handle input GIO channel.
 *  @field  gioOutputChan
 *              Handle output GIO channel.
 *  @field  appReadCb
 *              Callback structure for read requests.
 *  @field  appWriteCb
 *              Callback structure for write requests.
 *  @field  inputBuffer
 *              Buffer to receive data from GPP.
 *  @field  outputBuffer
 *              Buffer to send data to GPP.
 *  @field  readMAUs
 *              Number of MAUs read from GPP.
 *  @field  msgqHandle
 *              Handle to created message queue.
 *  @field  swiData
 *              SWI structure for SWI that handles data.
 *  @field  swiMsg
 *              SWI structure for SWI that handles messages.
 *  @field  bufferSize
 *              Size of buffer.
 *  @field  scalingFactor
 *              Used to scale the output buffer values.
 *  ============================================================================
 */
typedef struct SWISCALE_TransferInfo_tag {
    GIO_Handle      gioInputChan;
    GIO_Handle      gioOutputChan;
    GIO_AppCallback appReadCb;
    GIO_AppCallback appWriteCb;
    Char *          inputBuffer;
    Char *          outputBuffer;
    Uns             readMAUs;
    MSGQ_Handle     msgqHandle;
    SWI_Handle      swiData;
    SWI_Handle      swiMsg;
    Uns             bufferSize;
    Uns             scalingFactor;
} SWISCALE_TransferInfo;


/** ============================================================================
 *  @func   SWISCALE_create
 *
 *  @desc   Create phase function of SWISCALE application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Uns SWISCALE_create (SWISCALE_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   SWISCALE_execute
 *
 *  @desc   Excecute phase function of SWISCALE application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Uns SWISCALE_execute (SWISCALE_TransferInfo * transferInfo);

/** ============================================================================
 *  @func   SWISCALE_delete
 *
 *  @desc   Delete phase function of SWISCALE application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Uns SWISCALE_delete (SWISCALE_TransferInfo * transferInfo);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SWISCALE_ */
