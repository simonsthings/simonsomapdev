/** ============================================================================
 *  @file   tskLoop.h
 *
 *  @path   $(DSPLINK)/dsp/src/samples/loop/
 *
 *  @desc   Header file for tskLoop application.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TSKLOOP_)
#define TSKLOOP_


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


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
 *  @name   TSKLOOP_TransferInfo
 *
 *  @desc   Structure used to keep various information needed by various phases
 *          of the application.
 *
 *  @field  inputStream
 *              Handle to input stream.
 *  @field  outputStream
 *              Handle to output stream.
 *  @field  numTransfers
 *              Number of transfers to be done.
 *  @field  bufferSize
 *              Buffer size information.
 *  @field  receivedSize
 *              Actual length of the buffer that was received.
 *  @field  numBuffers
 *              Number of valid buffers in "buffers" array.
 *  @field  buffers
 *              Array of buffer pointers.
 *  ============================================================================
 */
typedef struct TSKLOOP_TransferInfo_tag {
    SIO_Handle inputStream;
    SIO_Handle outputStream;
    Uint16     numTransfers;
    Uint32     bufferSize;
    Uint16     receivedSize;
    Uint16     numBuffers;
    Char *     buffers [MAX_BUFFERS];
} TSKLOOP_TransferInfo;


/** ============================================================================
 *  @func   TSKLOOP_create
 *
 *  @desc   Create phase function of TSKLOOP application.
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
Int TSKLOOP_create (TSKLOOP_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   TSKLOOP_execute
 *
 *  @desc   Excecute phase function of TSKLOOP application.
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
Int TSKLOOP_execute (TSKLOOP_TransferInfo * transferInfo);

/** ============================================================================
 *  @func   TSKLOOP_delete
 *
 *  @desc   Delete phase function of TSKLOOP application.
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
Int TSKLOOP_delete (TSKLOOP_TransferInfo * transferInfo);


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (TSKLOOP_) */
