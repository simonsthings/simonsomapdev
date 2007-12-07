/** ============================================================================
 *  @file   mqabuf.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the BUF MQA interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (MQABUF_H)
#define MQABUF_H


USES (gpptypes.h)
USES (errbase.h)
USES (buf.h)
USES (ldrv_msgq.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   MqaBufObj
 *
 *  @desc   This structure defines the buffer object for the buffer allocator.
 *
 *  @field  msgSize
 *              Size of the messages in the buffer pool.
 *  @field  msgList
 *              List of messages in the buffer pool.
 *  ============================================================================
 */
typedef struct MqaBufObj_tag {
    Uint16      msgSize ;
    BufHandle   msgList ;
} MqaBufObj ;


/** ============================================================================
 *  @name   MqaBufState
 *
 *  @desc   This structure defines the allocator state object, which exists as
 *          a single instance in the system, and represents the allocator.
 *
 *  @field  numBufPools
 *              Number of buffer pools configured in the MQA.
 *  @field  bufPools
 *              Array of buffer pools for various message sizes. The array is
 *              dynamically allocated of size equal to the one specified by the
 *              user.
 *  @field  phyAddr
 *              Physical address of the buffer pool allocated.
 *  @field  virtAddr
 *              Virtual address of the buffer pool allocated.
 *  @field  size
 *              Size of memory allocated for all buffer pools.
 *  ============================================================================
 */
typedef struct MqaBufState_tag {
    Uint16          numBufPools ;
    MqaBufObj     * bufPools    ;
    Uint32          phyAddr     ;
    Uint32          virtAddr    ;
    Uint32          size        ;
} MqaBufState ;


/** ============================================================================
 *  @func   MQABUF_Initialize
 *
 *  @desc   Performs global initialization of the buffer MQA.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  The component must be unintialized.
 *
 *  @leave  None
 *
 *  @see    MQABUF_Finalize
 *  ============================================================================
 */
NORMAL_API
Void
MQABUF_Initialize () ;


/** ============================================================================
 *  @func   MQABUF_Finalize
 *
 *  @desc   Performs global finalization of the buffer MQA.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  The component must be intialized.
 *
 *  @leave  None
 *
 *  @see    MQABUF_Initialize
 *  ============================================================================
 */
NORMAL_API
Void
MQABUF_Finalize () ;


/** ============================================================================
 *  @func   MQABUF_Open
 *
 *  @desc   Opens the buffer MQA and configures it according to the user
 *          attributes.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *  @arg    mqaAttrs
 *              Attributes for initialization of the MQA component.
 *
 *  @ret    DSP_SOK
 *              This component has been successfully opened.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqaAttrs must be valid.
 *          mqaHandle must be valid.
 *          The component must be intialized.
 *
 *  @leave  None
 *
 *  @see    MQABUF_Attrs, MqaBufState, MQABUF_Close, BUF_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Open (IN  LdrvMsgqAllocatorHandle mqaHandle, IN  Pvoid mqaAttrs) ;


/** ============================================================================
 *  @func   MQABUF_Close
 *
 *  @desc   This function closes the MQA and cleans up its state object.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *
 *  @ret    DSP_SOK
 *              This component has been successfully closed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqaHandle must be valid.
 *          The component must be intialized.
 *
 *  @leave  None
 *
 *  @see    MqaBufState, MQABUF_Open, BUF_Delete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Close (IN  LdrvMsgqAllocatorHandle mqaHandle) ;


/** ============================================================================
 *  @func   MQABUF_Alloc
 *
 *  @desc   This function allocates a message buffer of the specified size.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *  @arg    size
 *              Size of the message to be allocated. On return, it stores the
 *              actual allocated size of the message, which, for the buffer MQA
 *              is the same as the requested size on success, or zero on
 *              failure.
 *  @arg    addr
 *              Location to receive the allocated message.
 *
 *  @ret    DSP_SOK
 *              This component has been successfully closed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqaHandle must be valid.
 *          size must be valid.
 *          addr must be valid.
 *          The component must be intialized.
 *
 *  @leave  None
 *
 *  @see    MqaBufState, MQABUF_Free, BUF_Alloc
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Alloc (IN     LdrvMsgqAllocatorHandle mqaHandle,
              IN OUT Uint16 *                size,
              OUT    MsgqMsg *               addr) ;


/** ============================================================================
 *  @func   MQABUF_Free
 *
 *  @desc   This function frees the message buffer given by the
 *          user of the specified size.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *  @arg    addr
 *              Address of the message to be freed.
 *  @arg    size
 *              Size of the message to be freed.
 *
 *  @ret    DSP_SOK
 *              This component has been successfully closed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  mqaHandle must be valid.
 *          addr must be valid.
 *          The component must be intialized.
 *
 *  @leave  None
 *
 *  @see    MqaBufState, MQABUF_Alloc , BUF_Free
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Free (IN  LdrvMsgqAllocatorHandle mqaHandle,
             IN  MsgqMsg                 addr,
             IN  Uint16                  size) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (MQABUF_H) */
