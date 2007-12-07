/** ============================================================================
 *  @file   tskScale.h
 *
 *  @path   $(DSPLINK)\dsp\src\samples\scale
 *
 *  @desc   Header file for tskScale application.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef TSKSCALE_
#define TSKSCALE_

/*  ----------------------------------- DSP/BIOS Headers            */
#include <msgq.h>
#include <sem.h>

#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  MAX_BUFFERS
 *
 *  @desc   Size of buffers array.
 *  ============================================================================
 */
#define MAX_BUFFERS            100

/** ============================================================================
 *  @const  TSK_NUM_BUFFERS
 *
 *  @desc   Number of buffers to allocate and use for transfer.
 *  ============================================================================
 */
#define TSK_NUM_BUFFERS          1

/** ============================================================================
 *  @const  TSK_INPUT_CHANNEL
 *
 *  @desc   Name of the input channel.
 *  ============================================================================
 */
#define TSK_INPUT_CHANNEL               "/dio_dsplink0"

/** ============================================================================
 *  @const  TSK_OUTPUT_CHANNEL
 *
 *  @desc   Name of the output channel.
 *  ============================================================================
 */
#define TSK_OUTPUT_CHANNEL              "/dio_dsplink1"

/** ============================================================================
 *  @const  TSK_DSP_MSGQNAME
 *
 *  @desc   Name of the message queue on the DSP.
 *  ============================================================================
 */
#define TSK_DSP_MSGQNAME                "DSPLINK_DSP00MSGQ00"

/** ============================================================================
 *  @name   TSKSCALE_TransferInfo
 *
 *  @desc   Structure used to keep various information needed by various phases
 *          of the application.
 *
 *  @field  inputStream
 *              Handle to input stream.
 *  @field  outputStream
 *              Handle to output stream.
 *  @field  msgqHandle
 *              Handle of created message queue.
 *  @field  notifySemObj
 *              Semaphore used for message notification.
 *  @field  numTransfers
 *              Number of transfers to be done.
 *  @field  bufferSize
 *              Buffer size information.
 *  @field  receivedSize
 *              Actual length of the buffer that was received.
 *  @field  numBuffers
 *              Number of valid buffers in "buffer" field.
 *  @field  buffer
 *              Buffer pointer.
 *  @field  scalingFactor
 *              Used to scale the output buffer values.
 *  ============================================================================
 */
typedef struct TSKSCALE_TransferInfo_tag {
    SIO_Handle  inputStream;
    SIO_Handle  outputStream;
    MSGQ_Handle msgqHandle;
    SEM_Obj     notifySemObj;
    Uns         numTransfers;
    Uns         bufferSize;
    Uns         receivedSize;
    Uns         numBuffers;
    Char *      buffer;
    Uns         scalingFactor;
} TSKSCALE_TransferInfo;


/** ============================================================================
 *  @func   TSKSCALE_create
 *
 *  @desc   Create phase function of TSKSCALE application.
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
Uns TSKSCALE_create (TSKSCALE_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   TSKSCALE_execute
 *
 *  @desc   Excecute phase function of TSKSCALE application.
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
Uns TSKSCALE_execute (TSKSCALE_TransferInfo * transferInfo);

/** ============================================================================
 *  @func   TSKSCALE_delete
 *
 *  @desc   Delete phase function of TSKSCALE application.
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
Uns TSKSCALE_delete (TSKSCALE_TransferInfo * transferInfo);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* TSKSCALE_ */
