/** ============================================================================
 *  @file   API_MsgqUtils.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Defines utility functions for API Msgq tests.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (API_MSGQUTILS_H)
#define API_MSGQUTILS_H


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
 *  @func   API_OpenAllocator
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
API_OpenAllocator (IN AllocatorId mqaId,
                   IN Uint16      numMsgPerPool,
                   IN Uint16      maxMsgSize) ;


/** ============================================================================
 *  @func   API_CloseAllocator
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
API_CloseAllocator (IN AllocatorId mqaId) ;


/** ============================================================================
 *  @func   API_OpenTransport
 *
 *  @desc   Opens transport.
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
API_OpenTransport (IN TransportId mqtId,
                   IN AllocatorId mqaId,
                   IN Uint16      maxMsgSize,
                   IN Uint16      numMsgqs) ;


/** ============================================================================
 *  @func   API_CloseTransport
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
API_CloseTransport (IN TransportId mqtId) ;


/** ============================================================================
 *  @func   API_CreateDspArgumentList
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
 *  @see    API_FreeDspArgumentList
 *  ============================================================================
 */
EXPORT_API
Char8 **
API_CreateDspArgumentList (IN Uint16 numArgs, ...) ;


/** ============================================================================
 *  @func   API_FreeDspArgumentList
 *
 *  @desc   Delete the list created by API_CreateDspArgumentList
 *
 *  @arg    numArgs
 *              Number of arguments in the variable list.
 *
 *  @arg    argList
 *              Pointer to the argument list created by
 *              API_CreateDspArgumentList.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    API_CreateDspArgumentList
 *  ============================================================================
 */
EXPORT_API
Void
API_FreeDspArgumentList (IN Uint16 numArgs, IN Char8 ** argList) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (API_MSGQUTILS_H) */
