/** ============================================================================
 *  @file   ldrv_mqa.h
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Defines the interface and structures of LDRV MQT.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LDRV_MQA_H)
#define LDRV_MQA_H


USES (gpptypes.h)
USES (errbase.h)
USES (dsplink.h)
USES (ldrv_msgq.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   LdrvMsgqAllocatorObj
 *
 *  @desc   Forward declaration of LDRV MSGQ allocator object.
 *  ============================================================================
 */
typedef struct LdrvMsgqAllocatorObj_tag  LdrvMsgqAllocatorObj ;

/** ============================================================================
 *  @name   LdrvMsgqAllocatorHandle
 *
 *  @desc   Forward declaration of the handle to LDRV MSGQ allocator object.
 *  ============================================================================
 */
typedef LdrvMsgqAllocatorObj * LdrvMsgqAllocatorHandle ;


/** ============================================================================
 *  @name   FnMqaInitialize
 *
 *  @desc   Signature of the function that performs global initialization of the
 *          buffer MQA.
 *
 *  @arg    None
 *
 *  @ret    None
 *  ============================================================================
 */
typedef Void (*FnMqaInitialize) () ;

/** ============================================================================
 *  @name   FnMqaFinalize
 *
 *  @desc   Signature of the function that performs global finalization of the
 *          buffer MQA.
 *
 *  @arg    None
 *
 *  @ret    None
 *  ============================================================================
 */
typedef Void (*FnMqaFinalize) () ;

/** ============================================================================
 *  @name   FnMqaOpen
 *
 *  @desc   Signature of the function that opens the buffer MQA and configures
 *          it according to the user attributes.
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
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqaOpen) (LdrvMsgqAllocatorHandle  mqaHandle,
                                 Pvoid                    mqaAttrs) ;

/** ============================================================================
 *  @name   FnMqaClose
 *
 *  @desc   Signature of the function that closes the buffer MQA.
 *
 *  @arg    mqaInfo
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
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqaClose) (LdrvMsgqAllocatorHandle  mqaHandle) ;

/** ============================================================================
 *  @name   FnMqaAlloc
 *
 *  @desc   Signature of the function that allocates a message buffer of the
 *          specified size.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *  @arg    size
 *              Pointer to the size of the message to be allocated.
 *  @arg    addr
 *              Location to receive the allocated message.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqaAlloc) (LdrvMsgqAllocatorHandle mqaHandle,
                                  Uint16 *                size,
                                  MsgqMsg *               addr) ;

/** ============================================================================
 *  @name   FnMqaFree
 *
 *  @desc   Signature of the function that frees the message of the specified
 *          size.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *  @arg    addr
 *              Address of the message to be freed.
 *  @arg    size
 *              Size of the message to be freed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnMqaFree) (LdrvMsgqAllocatorHandle mqaHandle,
                                 MsgqMsg                 addr,
                                 Uint16                  size) ;


/** ============================================================================
 *  @name   MqaInterface
 *
 *  @desc   This structure defines the function pointer table that must be
 *          implemented for every MQA in the system.
 *
 *  @field  mqaInitialize
 *              Pointer to MQA initialization function.
 *  @field  mqaFinalize
 *              Pointer to MQA finalization function.
 *  @field  mqaOpen
 *              Pointer to MQA open function.
 *  @field  mqaClose
 *              Pointer to MQA close function.
 *  @field  mqaAlloc
 *              Pointer to MQA function for allocating a message.
 *  @field  mqaFree
 *              Pointer to MQA function for freeing a message.
 *  ============================================================================
 */
struct MqaInterface_tag {
    FnMqaInitialize mqaInitialize ;
    FnMqaFinalize   mqaFinalize   ;
    FnMqaOpen       mqaOpen       ;
    FnMqaClose      mqaClose      ;
    FnMqaAlloc      mqaAlloc      ;
    FnMqaFree       mqaFree       ;
} ;

/** ============================================================================
 *  @name   LdrvMsgqAllocatorObj_tag
 *
 *  @desc   This structure defines the allocator object. There is one instance
 *          of the allocator object per MQA in the system.
 *
 *  @field  mqaName
 *              Name of the MQA. Used for debugging purposes only.
 *  @field  mqaInterface
 *              Pointer to the function table of the MQA represented by the
 *              allocator object.
 *  @field  mqaInfo
 *              State information needed by the allocator. The contents of this
 *              are allocator-specific.
 *  @field  mqaId
 *              ID of the MQA represented by the allocator object.
 *  ============================================================================
 */
struct LdrvMsgqAllocatorObj_tag {
#if defined (DDSP_DEBUG)
    Char8                  mqaName [DSP_MAX_STRLEN] ;
#endif /* if defined (DDSP_DEBUG) */
    MqaInterface *         mqaInterface ;
    Pvoid                  mqaInfo ;
    Uint16                 mqaId   ;
} ;

/** ============================================================================
 *  @name   MqaObject
 *
 *  @desc   This structure defines the MQA object stored in the LDRV object.
 *
 *  @field  mqaName
 *              Name of the MQA. For debugging purposes only.
 *  @field  interface
 *              Function pointer interface to access the functions for this MQA.
 *  ============================================================================
 */
struct MqaObject_tag {
    Char8           mqaName    [DSP_MAX_STRLEN] ;
    MqaInterface *  interface                   ;
} ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (LDRV_MQA_H) */
