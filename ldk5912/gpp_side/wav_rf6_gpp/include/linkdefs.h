/** ============================================================================
 *  @file   linkdefs.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Definitions of constants and structures for Link.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (LINKDEFS_H)
#define LINKDEFS_H


USES (gpptypes.h)
USES (dsplink.h)
USES (dspdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/*  ============================================================================
 *  @name   LinkInterface
 *
 *  @desc   Forward declaration of LinkInterface structure.
 *  ============================================================================
 */
typedef struct LinkInterface_tag LinkInterface ;

/** ============================================================================
 *  @name   PoolObject
 *
 *  @desc   Forward declaration of PoolObject_tag
 *  ============================================================================
 */
typedef struct PoolObject_tag PoolObject ;

/** ============================================================================
 *  @name   PoolOpenParams
 *
 *  @desc   Forward declaration of PoolOpenParams_tag
 *  ============================================================================
 */
typedef struct PoolOpenParams_tag PoolOpenParams ;


/** ============================================================================
 *  @name   DrvHandshake
 *
 *  @desc   Defines the types of handshake control actions.
 *
 *  @field  DrvHandshakeSetup
 *              Setup the handshaking between the processors.
 *  @field  DrvHandshakeStart
 *              Start the handshake process with the remote processor.
 *  @field  DrvHandshakeCompl
 *              Complete the handshaking with the remote processor.
 *  ============================================================================
 */
typedef enum {
    DrvHandshakeSetup = 0,
    DrvHandshakeStart = 1,
    DrvHandshakeCompl = 2
} DrvHandshake ;


/** ============================================================================
 *  @func   FnIpsInitialize
 *
 *  @desc   Signature of function that initializes an Inter Processor Signaling
 *          component.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    ipsId
 *              IPS Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnIpsInitialize) (IN ProcessorId dspId, IN Uint32 ipsId) ;


/** ============================================================================
 *  @func   FnIpsFinalize
 *
 *  @desc   Signature of function that finalizes an Inter Processor Signaling
 *          component.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    ipsId
 *              IPS Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnIpsFinalize) (IN ProcessorId dspId, IN Uint32 ipsId) ;

/** ============================================================================
 *  @name   FnPoolInitialize
 *
 *  @desc   Signature of function that initialize the plugged memory allocator.
 *
 *  @arg    object
 *              Pointer to the object to be initialized.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnPoolInitialize) (IN PoolObject * object) ;

/** ============================================================================
 *  @name   FnPoolFinalize
 *
 *  @desc   Signature of function that finalize the plugged memory allocator.
 *
 *  @arg    object
 *              Pointer to the object to be finalized.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnPoolFinalize) (IN Pvoid object) ;

/** ============================================================================
 *  @name   FnPoolOpen
 *
 *  @desc   Signature of function that creates the plugged memory allocator.
 *
 *  @arg    object
 *              Pointer to the internal allocator object.
 *  @arg    poolOpenParams
 *              An argument for creating POOL and plugged memory allocator.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnPoolOpen) (IN Pvoid            object,
                                  IN PoolOpenParams * poolOpenParams) ;

/** ============================================================================
 *  @name   FnPoolClose
 *
 *  @desc   Signature of function that destroys plugged memory allocator.
 *
 *  @arg    object
 *              Pointer to the internal allocator object.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnPoolClose) (IN Pvoid object) ;

/** ============================================================================
 *  @name   FnPoolAlloc
 *
 *  @desc   Signature of function that allocates a buffer, and returns the
 *          pointer to the user.
 *
 *  @arg    object
 *              Pointer to the internal allocator object.
 *  @arg    size
 *              Size of the buffer to be allocated.
 *  @arg    bufPtr
 *              Location to receive the allocated buffer.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnPoolAlloc) (IN  Pvoid     object,
                                   IN  Uint32    size,
                                   OUT Pvoid *   bufPtr) ;

/** ============================================================================
 *  @name   FnPoolFree
 *
 *  @desc   Signature of function that frees a buffer.
 *
 *  @arg    object
 *              Pointer to the internal allocator object.
 *  @arg    size
 *              Size of the buffer to be freed.
 *  @arg    bufPtr
 *              Pointer to the buffer to be freed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnPoolFree) (IN Pvoid   object,
                                  IN Uint32  size,
                                  IN Pvoid   bufPtr) ;

/** ============================================================================
 *  @name   FnPoolReconfigure
 *
 *  @desc   Signature of function that reconfigure the plugged memory allocator.
 *
 *  @arg    object
 *              Pointer to the internal allocator object.
 *  @arg    args
 *              Argument for reconfiguring the plugged memory allocator.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EMEMORY
 *              Operation failed due to a memory error.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnPoolReconfigure) (IN Pvoid object, IN Pvoid args) ;


/** ============================================================================
 *  @func   FnLinkInitialize
 *
 *  @desc   Signature of function that allocates and initializes resources
 *          used by the link driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EFAIL
 *              General failure.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkInitialize) (IN ProcessorId dspId) ;

/** ============================================================================
 *  @func   FnLinkFinalize
 *
 *  @desc   Signature of function that de-allocates and finalizes resources
 *          used by the link driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkFinalize) (IN ProcessorId dspId) ;

/** ============================================================================
 *  @func   FnLinkHandshake
 *
 *  @desc   Signature of function that setup, start, complete the handshake
 *          process toward specified DSP on the Link Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *  @arg    hshkCtrl
 *              Handshake control action to be executed.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *  ============================================================================
 */
typedef DSP_STATUS (*FnLinkHandshake) (IN ProcessorId   dspId,
                                       IN DrvHandshake hshkCtrl) ;

#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @name   FnLinkDebug
 *
 *  @desc   Signature of the function that prints debug information for the Link
 *          Driver.
 *
 *  @arg    dspId
 *              DSP Identifier.
 *
 *  @ret    None.
 *  ============================================================================
 */
typedef Void (*FnLinkDebug) (IN ProcessorId dspId) ;
#endif /* defined (DDSP_DEBUG) */


/** ============================================================================
 *  @name   PoolInterface
 *
 *  @desc   Interface functions exported by the POOL subcomponent.
 *
 *  @field  open
 *              Function pointer to the plugged pool's open function.
 *  @field  close
 *              Function pointer to the plugged pool's close function.
 *  @field  alloc
 *              Function pointer to the plugged pool's alloc function.
 *  @field  free
 *              Function pointer to the plugged pool's free function.
 *  @field  reconfigure
 *              Function pointer to the plugged pool's reconfigure function.
 *  ============================================================================
 */
typedef struct PoolInterface_tag {
    FnPoolOpen        open        ;
    FnPoolClose       close       ;
    FnPoolAlloc       alloc       ;
    FnPoolFree        free        ;
    FnPoolReconfigure reconfigure ;
} PoolInterface ;

/** ============================================================================
 *  @name   PoolOpenParams_tag
 *
 *  @desc   This structure defines the pool open specific parameters.
 *
 *  @field  params
 *              Pointer to the user provided parameters.
 *  @field  physAddr
 *              Physical address of memory block.
 *  @field  virtAddr
 *              Address of memory block in kernel virtual address space.
 *  @field  size
 *              Size of memory block in bytes.
 *  ============================================================================
 */
struct PoolOpenParams_tag {
    Pvoid   params ;
    Uint32  physAddr ;
    Uint32  virtAddr ;
    Uint32  size ;
} ;

/** ============================================================================
 *  @name   LinkInterface_tag
 *
 *  @desc   Interface functions exported by the Link Driver.
 *
 *  @field  initialize
 *              Function pointer to the Link Driver initialize function.
 *  @field  finalize
 *              Function pointer to the Link Driver finalize function.
 *  @field  handshake
 *              Function pointer to the Link Driver function to setup,start and
 *              complete handshake.
 *  @field  debug
 *              Function pointer to the Link Driver function for printing debug
 *              information
 *  ============================================================================
 */
struct LinkInterface_tag {
    FnLinkInitialize    initialize ;
    FnLinkFinalize      finalize ;
    FnLinkHandshake     handshake ;
#if defined (DDSP_DEBUG)
    FnLinkDebug         debug ;
#endif /* if defined (DDSP_DEBUG) */
} ;


/** ============================================================================
 *  @name   IpsObject_tag
 *
 *  @desc   Defines the Inter Processor Signaling object.
 *
 *  @field  ipsName
 *              Name of the IPS
 *              Defined only for debug build.
 *  @field  abbr
 *              Abbreviation of the IPS name.
 *              Defined only for debug build.
 *  @field  initialize
 *              Initialize function for the IPS.
 *  @field  finalize
 *              Finalize function for the IPS.
 *  @field  irpQueueLength
 *              Length of the IRP queue within the IPS.
 *  @field  memEntry
 *              ID of the LINK mem information entry in the DSP memTable
 *  @field  size
 *              Size of memory area configured for the IPS component.
 *  @field  arg1
 *              First argument specific to the IPS.
 *  @field  arg2
 *              Second argument specific to the IPS.
 *  ============================================================================
 */
struct IpsObject_tag {
#if defined (DDSP_DEBUG)
    Char8           ipsName [DSP_MAX_STRLEN] ;
    Char8           abbr [DSP_MAX_STRLEN]    ;
#endif /* if defined (DDSP_DEBUG) */
    FnIpsInitialize initialize      ;
    FnIpsFinalize   finalize        ;
#if defined (CHNL_COMPONENT)
    Uint32          irpQueueLength  ;
    Uint32          irpSize         ;
#endif /* if defined (CHNL_COMPONENT) */
    Uint32          memEntry        ;
    Uint32          size            ;
    Uint32          arg1            ;
    Uint32          arg2            ;
} ;

/** ============================================================================
 *  @name   PoolObject_tag
 *
 *  @desc   Structure of LDRV POOL Object.
 *
 *  @field  poolName
 *              Name of the pool
 *              Defined only for debug build.
 *  @field  abbr
 *              Abbreviation of the pool name.
 *              Defined only for debug build.
 *  @field  initialize
 *              Function pointer to the plugged pool's initialize function.
 *  @field  finalize
 *              Function pointer to the plugged pool's finalize function.
 *  @field  interface
 *              Pool interface functions.
 *  @field  dspId
 *              ID of the processor (if any) that uses this pool.
 *  @field  memEntry
 *              ID of the LINK mem information entry in the DSP memTable
 *  @field  addrOffset
 *              Offset of the pool from the base address of the memEntry used
 *              by the pool.
 *  @field  poolSize
 *              Size of the POOL memory block in case of SMA. But for other
 *              allocators it is the maximum size of the memory block to be
 *              used.
 *  @field  arg1
 *              First argument specific to the pool.
 *  @field  arg2
 *              Second argument specific to the pool.
 *  @field  object
 *              Allocator-specific object .
 *  ============================================================================
 */
struct PoolObject_tag {
#if defined (DDSP_DEBUG)
    Char8                poolName [DSP_MAX_STRLEN] ;
    Char8                abbr     [DSP_MAX_STRLEN] ;
#endif /* if defined (DDSP_DEBUG) */
    FnPoolInitialize     initialize ;
    FnPoolFinalize       finalize   ;
    PoolInterface *      interface  ;
    ProcessorId          dspId      ;
    Uint32               memEntry   ;
    Uint32               addrOffset ;
    Uint32               poolSize   ;
    Uint32               arg1       ;
    Uint32               arg2       ;
    Pvoid                object     ;
} ;

/** ============================================================================
 *  @name   LinkObject_tag
 *
 *  @desc   Defines the link object for driver initialization and
 *          synchronization.
 *
 *  @field  linkName
 *              Name of the link.
 *              Defined only for debug build.
 *  @field  abbr
 *              Abbreviation of the link name.
 *              Defined only for debug build.
 *  @field  interface
 *              Pointer to the interface table for the link.
 *  @field  memEntry
 *              ID of the LINK mem information entry in the DSP memTable.
 *  @field  size
 *              Size of the memory area used by the link.
 *  @field  numIps
 *              Number of IPS objects used by the link.
 *  @field  ipsTableId
 *              ID of the IPS table in the link driver object.
 *  ============================================================================
 */
struct LinkObject_tag {
#if defined (DDSP_DEBUG)
    Char8           linkName [DSP_MAX_STRLEN] ;
    Char8           abbr [DSP_MAX_STRLEN]    ;
#endif /* if defined (DDSP_DEBUG) */
    LinkInterface * interface  ;
    Uint32          memEntry   ;
    Uint32          size       ;
    Uint32          numIps     ;
    Uint32          ipsTableId ;
} ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (LINKDEFS_H) */
