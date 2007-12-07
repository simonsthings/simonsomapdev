/** ============================================================================
 *  @file   buf.h
 *
 *  @path   $(DSPLINK)\gpp\src\gen
 *
 *  @desc   Defines the interfaces and data structures for the BUF
 *          sub-component.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BUF_H)
#define BUF_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   BufObj
 *
 *  @desc   This structure defines the buffer pool object. It maintains the pool
 *          of buffers of a particular fixed size.
 *
 *  @field  startAddress
 *              Starting address of buffer pool.
 *  @field  size
 *              Size of the buffers in this pool.
 *  @field  nextFree
 *              Pointer to next free buffer.
 *  @field  totalBuffers
 *              Total number of buffers in pool.
 *  @field  freeBuffers
 *              Number of free buffers in pool.
 *  @field  freePool
 *              Indicates whether the buffer pool was allocated within the BUF
 *              component, and should be freed during BUF_Delete ()
 *  ============================================================================
 */
typedef struct BufObj_tag {
    Uint32        startAddress ;
    Uint16        size         ;
    Uint32        nextFree     ;
    Uint16        totalBuffers ;
    Uint16        freeBuffers  ;
    Bool          freePool     ;
} BufObj ;

/** ============================================================================
 *  @name   BufHandle
 *
 *  @desc   This is the Handle to buffer pool objects.
 *  ============================================================================
 */
typedef  BufObj * BufHandle  ;

/** ============================================================================
 *  @name   BufHeader
 *
 *  @desc   This structure defines the buffer header. This structure maintains
 *          information required to link the buffers within each buffer pool.
 *
 *  @field  next
 *              Pointer to next buffer header.
 *  @field  usedOnce
 *              Indicates if the buffer has been used at least once. When
 *              profiling is enabled, this field is used for getting statistics
 *              regarding the usage of the buffers within the buffer pools.
 *  ============================================================================
 */
typedef struct BufHeader_tag {
    struct BufHeader_tag * next ;
#if defined (DDSP_PROFILE)
    Uint16   usedOnce ;
#endif /* if defined (DDSP_PROFILE) */
} BufHeader ;

#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @name   BufStats
 *
 *  @desc   This structure defines instrumentation data for the buffer pools.
 *
 *  @field  size
 *              Size of the buffers in this pool.
 *  @field  totalBuffers
 *              Total number of buffers in pool.
 *  @field  freeBuffers
 *              Number of free buffers in pool.
 *  @field  maxUsed
 *              Maximum number of buffers that have been used at least once
 *              since creation of the buffer pool.
 *  ============================================================================
 */
typedef struct BufStats_tag {
    Uint16        size         ;
    Uint16        totalBuffers ;
    Uint16        freeBuffers  ;
    Uint16        maxUsed      ;
} BufStats ;
#endif /* if defined (DDSP_PROFILE) */

/** ============================================================================
 *  @const  MIN_BUF_SIZE
 *
 *  @desc   This constant defines the minimum size of the buffer to be
 *          allocated.
 *  ============================================================================
 */
#define MIN_BUF_SIZE (sizeof (BufHeader))


/** ============================================================================
 *  @func   BUF_Initialize
 *
 *  @desc   This function initializes the buffer component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    BUF_Finalize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Initialize () ;


/** ============================================================================
 *  @func   BUF_Finalize
 *
 *  @desc   This function finalizes the buffer component.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    BUF_Initialize
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Finalize () ;


/** ============================================================================
 *  @func   BUF_Create
 *
 *  @desc   This function creates and initializes a fixed buffer pool and
 *          returns a handle to the corresponding buffer pool object.
 *
 *  @arg    numBufs
 *              Number of buffers to be created in the pool.
 *  @arg    size
 *              Size of the buffers within the pool.
 *  @arg    bufHandle
 *              Location to receive the handle to the created buffer pool
 *              object.
 *  @arg    bufAddress
 *              Address of the memory reserved for this buffer pool. If a valid
 *              address is specified, no memory is allocated within this
 *              function, and it can be called from within DPC context. The size
 *              of the memory for the buffer pool allocated by the user must be
 *              equal to (size * numBufs).If the address specified is NULL, this
 *              function internally allocates the memory required for the buffer
 *              pool. In this case, this function cannot be called from DPC or
 *              ISR context.
 *
 *  @ret    DSP_SOK
 *              The buffer pool has been successfully created.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  bufHandle must be valid.
 *          size must be greater than or equal to MIN_BUF_SIZE.
 *          numBufs must be greater than 0.
 *
 *  @leave  None
 *
 *  @see    BufHeader , BufObj, BUF_Delete
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Create (IN     Uint16       numBufs,
            IN     Uint16       size,
            OUT    BufHandle  * bufHandle,
            IN OPT Uint32       bufAddress) ;


/** ============================================================================
 *  @func   BUF_Delete
 *
 *  @desc   This function deletes the buffer pool specified by the user.
 *
 *  @arg    bufHandle
 *              Handle to the buffer pool object.
 *
 *  @ret    DSP_SOK
 *              The buffer pool has been successfully deleted.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  bufHandle must be valid.
 *
 *  @leave  None
 *
 *  @see    BufObj, BUF_Create
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Delete (IN  BufHandle  bufHandle) ;


/** ============================================================================
 *  @func   BUF_Alloc
 *
 *  @desc   This function allocates a free buffer from the specified buffer pool
 *          and returns it to the user.
 *
 *  @arg    bufHandle
 *              Handle to the buffer pool object.
 *  @arg    buffer
 *              Location to receive the allocated buffer.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  bufHandle must be valid.
 *          buffer must be valid.
 *
 *  @leave  None
 *
 *  @see    BufObj, BUF_Free
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Alloc (IN  BufHandle  bufHandle, OUT Pvoid * buffer) ;


/** ============================================================================
 *  @func   BUF_Free
 *
 *  @desc   This function frees the buffer specified by the user, and returns it
 *          to the buffer pool.
 *
 *  @arg    bufHandle
 *              Handle to the buffer pool object.
 *  @arg    buffer
 *              Pointer to the buffer to be freed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  bufHandle must be valid.
 *          buffer must be valid.
 *
 *  @leave  None
 *
 *  @see    BufObj, BUF_Alloc
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_Free (IN  BufHandle  bufHandle, IN  Pvoid  buffer) ;


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   BUF_GetStats
 *
 *  @desc   This function gets instrumentation information about the specified
 *          buffer pool.
 *
 *  @arg    bufHandle
 *              Handle to the buffer pool object.
 *  @arg    bufStats
 *              Location to receive the instrumentation information.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  bufHandle must be valid.
 *          bufStats must be valid.
 *
 *  @leave  None
 *
 *  @see    BufObj, BUF_Alloc
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BUF_GetStats (IN  BufHandle  bufHandle, OUT BufStats * bufStats) ;
#endif /* if defined (DDSP_PROFILE) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (BUF_H) */
