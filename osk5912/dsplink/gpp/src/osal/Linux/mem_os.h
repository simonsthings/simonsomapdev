/** ============================================================================
 *  @file   mem_os.h
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Defines the OS dependent attributes & structures for the
 *          sub-component MEM.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (MEM_OS_H)
#define MEM_OS_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  MEM_KERNEL
 *
 *  @desc   Example memory type. One has to handle it in MEM_Alloc if one wants
 *          to implement it.
 *  ============================================================================
 */
#define MEM_KERNEL      GFP_KERNEL


/** ============================================================================
 *  @type   MemAllocAttrs
 *
 *  @desc   OS dependent attributes for allocating memory.
 *
 *  @field  physicalAddress
 *              Physical address of the allocated memory.
 *  ============================================================================
 */
typedef struct MemAllocAttrs_tag {
    Uint32 *    physicalAddress ;
} MemAllocAttrs ;

/** ============================================================================
 *  @type   MemFreeAttrs
 *
 *  @desc   OS dependent attributes for freeing memory.
 *
 *  @field  physicalAddress
 *              Physical address of the memory to be freed.
 *  @field  size
 *              Size of the memory to be freed.
 *  ============================================================================
 */
typedef struct MemFreeAttrs_tag {
    Uint32 *    physicalAddress ;
    Uint32      size ;
} MemFreeAttrs ;


/** ============================================================================
 *  @type   MemMapInfo_tag
 *
 *  @desc   OS dependent definition of the information required for mapping a
 *          memory region.
 *
 *  @field  src
 *              Address to be mapped.
 *  @field  size
 *              Size of memory region to be mapped.
 *  @field  dst
 *              Mapped address.
 *  ============================================================================
 */
struct MemMapInfo_tag {
    Uint32   src  ;
    Uint32   size ;
    Uint32   dst  ;
} ;


/** ============================================================================
 *  @type   MemUnmapInfo_tag
 *
 *  @desc   OS dependent definition of the information required for unmapping
 *          a previously mapped memory region.
 *
 *  @field  addr
 *              Address to be unmapped. This is the address returned as 'dst'
 *              address from a previous call to MEM_Map () in the MemMapInfo
 *              structure.
 *  @field  size
 *              Size of memory region to be unmapped.
 *  ============================================================================
 */
struct MemUnmapInfo_tag {
    Uint32  addr ;
    Uint32  size ;
} ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (MEM_OS_H) */
