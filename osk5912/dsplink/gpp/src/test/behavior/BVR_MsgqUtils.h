/** ============================================================================
 *  @file   BVR_MsgqUtils.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Defines utility functions for BVR Msgq tests.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BVR_MSGQUTILS_H)
#define BVR_MSGQUTILS_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  TST_MQA_ID
 *
 *  @desc   The Allocator ID to be used.
 *  ============================================================================
 */
#define TST_MQA_ID 0

/** ============================================================================
 *  @macro  TST_LMQT_ID
 *
 *  @desc   The Transport ID to be used for local transport.
 *  ============================================================================
 */
#define TST_LMQT_ID 0

/** ============================================================================
 *  @macro  TST_RMQT_ID
 *
 *  @desc   The Transport ID to be used for remote transport.
 *  ============================================================================
 */
#define TST_RMQT_ID 1

/** ============================================================================
 *  @name  MAX_TST_MSQ_DATA
 *
 *  @desc   Maximum length of data in the test msg.
 *  ============================================================================
 */
#define TST_MSQ_DATA_SIZE   32

/** ============================================================================
 *  @macro  TST_REPORT_SIZE
 *
 *  @desc   Size of the report sent by the DSP side.
 *  ============================================================================
 */
#define TST_REPORT_SIZE     ALIGN (2)


/** ============================================================================
 *  @name   TestMsg
 *
 *  @desc   The buffer structure to be used for messaging.
 *
 *  @field  elem
 *              The header required by MSGQ implementation.
 *  @field  info
 *              A 16 bit info field used differently by different tests.
 *  @field  buffer
 *              buffer required for storing the messages esp. for
 *              data verification.
 *  ============================================================================
 */
typedef struct TestMsg_tag {
    MsgqMsgHeader   elem                       ;
    Uint16          info                       ;
    Uint16          buffer [TST_MSQ_DATA_SIZE] ;
} TestMsg ;

/** ============================================================================
 *  @name   ArgsToThread
 *
 *  @desc   The information is passed to each thread created.
 *
 *  @field  procId
 *              Id of the processor which is to be attached.
 *  @field  msgqId
 *              Id of the queue.
 *  @field  numMsg
 *              Number of messages to be transfered.
 *  @field  status
 *              Status of the execution of the thread.
 *  ============================================================================
 */
typedef struct ArgsToThread_tag {
    ProcessorId  procId ;
    MsgQueueId   msgqId ;
    Uint16       numMsg ;
    DSP_STATUS   status ;
} ArgsToThread ;


/** ============================================================================
 *  @func   BVR_OpenAllocator
 *
 *  @desc   Opens an allocator for default operation.
 *
 *  @arg    mqaId
 *              Allocator Id to be opened.
 *  @arg    numMsgPerPool
 *              Number of messages per pool of the allocator.
 *  @arg    maxMsgSize
 *              Maximum size of the message to be configured in the allocator.
 *
 *  @ret    DSP_SOK
 *              The MQA component has been successfully opened.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_OpenAllocator (IN AllocatorId mqaId,
                   IN Uint16      numMsgPerPool,
                   IN Uint16      maxMsgSize) ;


/** ============================================================================
 *  @func   BVR_CloseAllocator
 *
 *  @desc   Closes an allocator.
 *
 *  @arg    mqaId
 *              Allocator Id to be closed.
 *
 *  @ret    DSP_SOK
 *              The MQA component has been successfully closed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_CloseAllocator (IN AllocatorId mqaId) ;


/** ============================================================================
 *  @func   BVR_OpenTransport
 *
 *  @desc   Opens transport. Assumes the TST_LMQT_ID on GPP is local transport.
 *
 *  @arg    mqtId
 *              Transport Id to be opened.
 *  @arg    mqaId
 *              Default mqaid to be used for opening the transport.
 *  @arg    maxMsgSize
 *              Maximum size of the message configured in the allocator.
 *  @arg    numMsgqs
 *              Maximum number of message queues on the local processor.
 *
 *  @ret    DSP_SOK
 *              The MQT component has been successfully opened.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_OpenTransport (IN TransportId mqtId,
                   IN AllocatorId mqaId,
                   IN Uint16      maxMsgSize,
                   IN Uint16      numMsgqs) ;


/** ============================================================================
 *  @func   BVR_CloseTransport
 *
 *  @desc   Closes transport.
 *
 *  @arg    mqtId
 *              Transport Id to be closed.
 *
 *  @ret    DSP_SOK
 *              The MQT component has been successfully opened.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_CloseTransport (IN TransportId mqtId) ;


/** ============================================================================
 *  @func   BVR_SetupMessaging
 *
 *  @desc   Setup the proc and messaging components. This function calls
 *          PROC_Setup, MSGQ_AllocatorOpen, PROC_Attach, PROC_Load, PROC_Start
 *          and MSGQ_TransportOpen.
 *
 *  @arg    procId
 *              DSP identifier.
 *  @arg    numMsgPerPool
 *              Number of messages per pool of the allocator.
 *  @arg    maxMsgSize
 *              Maximum size of the message to be configured in the allocator.
 *  @arg    numMsgqs
 *              Maximum number of message queues on the local processor.
 *  @arg    imagePath
 *              Full path to the image file to load on DSP.
 *  @arg    argc
 *              Number of argument to be passed to the base image upon start.
 *  @arg    argv
 *              Arguments to be passed to DSP main application.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFILE
 *              Invalid base image.
 *          DSP_EFAIL
 *              General failure, unable to load image on DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    BVR_CleanupMessaging
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_SetupMessaging (IN ProcessorId procId,
                    IN Uint16      numMsgPerPool,
                    IN Uint16      maxMsgSize,
                    IN Uint16      numMsgqs,
                    IN Char8 *     imagePath,
                    IN Uint32      argc,
                    IN Char8 **    argv) ;


/** ============================================================================
 *  @func   BVR_CleanupMessaging
 *
 *  @desc   Close and cleanup the initializations done by BVR_SetupMessaging
 *
 *  @arg    procId
 *              DSP identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EACCESSDENIED
 *              Not allowed to access the DSP.
 *          DSP_EFAIL
 *              General failure, unable to load image on DSP.
 *          DSP_EWRONGSTATE
 *              Incorrect state for completing the requested operation.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    BVR_SetupMessaging
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_CleanupMessaging (IN ProcessorId  procId) ;


/** ============================================================================
 *  @func   BVR_CreateDspArgumentList
 *
 *  @desc   Create a list of arguments to be passed to the DSP from
 *          the list of Uint16.
 *
 *  @arg    numArgs
 *              Number of arguments in the variable list.
 *
 *  @ret    The pointer to the argument list created.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    BVR_FreeDspArgumentList
 *  ============================================================================
 */
EXPORT_API
Char8 **
BVR_CreateDspArgumentList (IN Uint16 numArgs, ...) ;


/** ============================================================================
 *  @func   BVR_FreeDspArgumentList
 *
 *  @desc   Delete the list created by BVR_CreateDspArgumentList
 *
 *  @arg    numArgs
 *              Number of arguments in the variable list.
 *
 *  @arg    argList
 *              Pointer to the argument list created by
 *              BVR_CreateDspArgumentList.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    BVR_CreateDspArgumentList
 *  ============================================================================
 */
EXPORT_API
Void
BVR_FreeDspArgumentList (IN Uint16 numArgs, IN Char8 ** argList) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif /* !defined (BVR_MSGQUTILS_H) */
