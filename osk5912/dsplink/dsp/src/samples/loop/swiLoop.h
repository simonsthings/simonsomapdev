/** ============================================================================
 *  @file   swiLoop.h
 *
 *  @path   $(DSPLINK)\dsp\src\samples\loop
 *
 *  @desc   Header file for SWI LOOPBACK application.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef SWILOOP_
#define SWILOOP_

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
 *  @name   SWILOOP_TransferInfo
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
 *  @field  readWords
 *              Number of words read from GPP.
 *  @field  swi
 *              SWI structure.
 *  @field  bufferSize
 *              Size of buffer.
 *  ============================================================================
 */
typedef struct SWILOOP_TransferInfo_tag {
    GIO_Handle gioInputChan;
    GIO_Handle gioOutputChan;
    GIO_AppCallback appReadCb;
    GIO_AppCallback appWriteCb;
    Char *inputBuffer;
    Char *outputBuffer;
    Uns readWords;
    SWI_Handle swi;
    Uns bufferSize;
} SWILOOP_TransferInfo;


/** ============================================================================
 *  @func   SWILOOP_create
 *
 *  @desc   Create phase function of SWILOOP application.
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
Uns SWILOOP_create (SWILOOP_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   SWILOOP_execute
 *
 *  @desc   Excecute phase function of SWILOOP application.
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
Uns SWILOOP_execute (SWILOOP_TransferInfo * transferInfo);

/** ============================================================================
 *  @func   SWILOOP_delete
 *
 *  @desc   Delete phase function of SWILOOP application.
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
Uns SWILOOP_delete (SWILOOP_TransferInfo * transferInfo);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SWILOOP_ */
