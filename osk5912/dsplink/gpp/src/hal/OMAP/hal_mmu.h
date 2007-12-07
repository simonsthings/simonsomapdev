/** ============================================================================
 *  @file   hal_mmu.h
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Declares necessary functions for MMU operations.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_MMU_H)
#define HAL_MMU_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/*  ============================================================================
 *  @const  XXXX_OFFSET
 *
 *  @desc   Offset of MMU specific registers on DSP.
 *  ============================================================================
 */
#define CNTL_REG_OFFSET             0x08
#define FAULT_ADDR_H_REG_OFFSET     0x0C
#define FAULT_ADDR_L_REG_OFFSET     0x10
#define FAULT_ST_REG_OFFSET         0x14
#define IT_ACK_REG_OFFSET           0x18
#define LOCK_REG_OFFSET             0x24
#define LD_TLB_REG_OFFSET           0x28
#define CAM_H_REG_OFFSET            0x2C
#define CAM_L_REG_OFFSET            0x30
#define RAM_H_REG_OFFSET            0x34
#define RAM_L_REG_OFFSET            0x38


/** ============================================================================
 *  @const  RESET_SW_MMU_MASK
 *
 *  @desc   MMU Reset mask.
 *  ============================================================================
 */
#define RESET_SW_MMU_MASK           0x0001

/** ============================================================================
 *  @const  MMU_ENABLE_MASK
 *
 *  @desc   MMU Enable mask.
 *  ============================================================================
 */
#define MMU_ENABLE_MASK             0x0002

/** ============================================================================
 *  @const  WTL_ENABLE_MASK
 *
 *  @desc   Walking table logic mask.
 *  ============================================================================
 */
#define WTL_ENABLE_MASK             0x0004

/** ============================================================================
 *  @const  IT_ACK_MASK
 *
 *  @desc   IT acknowledge register.
 *  ============================================================================
 */
#define IT_ACK_MASK                 0x0001


/** ============================================================================
 *  @name   MMU_SIZE
 *
 *  @desc   Size of a memory unit that can be mapped using DSP MMU.
 *
 *  @field  MMU_SIZE_SECTION
 *              Size of a section (1MB).
 *  @field  MMU_SIZE_LARGE
 *              Size of a large page (64KB).
 *  @field  MMU_SIZE_SMALL
 *              Size of a small page (4KB)
 *  @field  MMU_SIZE_TINY
 *              Size of a tiny page (1KB)
 *  ============================================================================
 */
typedef enum {
    MMU_SIZE_SECTION = 0x00100000,
    MMU_SIZE_LARGE   = 0x00010000,
    MMU_SIZE_SMALL   = 0x00001000,
    MMU_SIZE_TINY    = 0x00000400
} MMU_SIZE ;

/** ============================================================================
 *  @name   MMU_SLST
 *
 *  @desc   Enum indicating size of DSP MMU TLB Entry.
 *
 *  @field  MMU_SECTION
 *              Identifies section.
 *  @field  MMU_LARGE_PAGE
 *              Identifies large page.
 *  @field  MMU_SMALL_PAGE
 *              Identifies small page.
 *  @field  MMU_TINY_PAGE
 *              Identifies tiny page.
 *  ============================================================================
 */
typedef enum {
    MMU_SECTION    = 0,
    MMU_LARGE_PAGE = 1,
    MMU_SMALL_PAGE = 2,
    MMU_TINY_PAGE  = 3
} MMU_SLST ;

/** ============================================================================
 *  @name   MMU_PRESERVED
 *
 *  @desc   Enum indicating whether entry is preserved.
 *
 *  @field  MMU_ENTRY_NOT_PRESERVED
 *              Indicates entry is not preserved.
 *  @field  MMU_ENTRY_PRESERVED
 *              Indicates entry is preserved.
 *  ============================================================================
 */
typedef enum {
    MMU_ENTRY_NOT_MMU_PRESERVED = 0,
    MMU_ENTRY_MMU_PRESERVED     = 1
} MMU_PRESERVED ;


/** ============================================================================
 *  @name   MMU_AP
 *
 *  @desc   Access permissions.
 *
 *  @field  MMU_NOT_ACCESSIBLE
 *              Identifies not accessible.
 *  @field  MMU_READ_ONLY
 *              Identifies not readable
 *  @field  MMU_FULL_ACCESS
 *              Identifies full access.
 *  ============================================================================
 */
typedef enum {
    MMU_NOT_ACCESSIBLE = 0,
    MMU_READ_ONLY      = 2,
    MMU_FULL_ACCESS    = 3
} MMU_AP ;


/** ============================================================================
 *  @name   MMU_Control
 *
 *  @desc   Defines possible control requests for the DSP MMU.
 *
 *  @field  MMU_Reset
 *              Reset the DSP MMU.
 *  @field  MMU_Release
 *              Release the DSP MMU.
 *  @field  MMU_Enable
 *              Enable the DSP MMU.
 *  @field  MMU_Disable
 *              Disable the DSP MMU.
 *  @field  MMU_DisableWtl
 *              Disable Walking Table Logic in DSP MMU.
 *  ============================================================================
 */
typedef enum {
    MMU_Reset      = 0,
    MMU_Release    = 1,
    MMU_Enable     = 2,
    MMU_Disable    = 3,
    MMU_DisableWtl = 4
} MMU_Control ;


/** ============================================================================
 *  @func   HAL_ControlDspMmu
 *
 *  @desc   Performs control operations on the DSP MMU.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @arg    cmd
 *              Control command.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ControlDspMmu (IN HalObject * halObject, MMU_Control cmd) ;


/** ============================================================================
 *  @func   HAL_DspMmuDisableWtl
 *
 *  @desc   Disables the Walking Table Logic.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_DspMmuDisableWtl (IN HalObject * halObject) ;


/** ============================================================================
 *  @func   HAL_DspMmuTlbEntry
 *
 *  @desc   Setup DSP MMU TLB Entry.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    ulPhyAddr
 *              Physical Address in the entry.
 *  @arg    ulVirAddr
 *              Virtual  Address in the entry.
 *  @arg    slst_bit
 *              Indicating size of DSP MMU TLB entry.
 *  @arg    ap_bits
 *              Access privilege.
 *  @arg    locked_base_value
 *              This value must be between 0 and 31.
 *  @arg    current_entry
 *              This value must be between base-value and 31.
 *  @arg    p_bit
 *              Entry is preserved or not.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_DspMmuTlbEntry (IN HalObject *      halObject,
                    IN Uint32           phyAddr,
                    IN Uint32           virAddr,
                    IN MMU_SLST         slst_bit,
                    IN MMU_AP           ap_bits,
                    IN Char8            locked_base_value,
                    IN Char8            current_entry,
                    IN MMU_PRESERVED    p_bit) ;


/** ============================================================================
 *  @func   HAL_DspMmuIntClear
 *
 *  @desc   Check and clear the MMU fault interrupt.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_DspMmuIntClear (IN HalObject * halObject) ;


#if defined (__cplusplus)
}
#endif


#endif  /* !defined (HAL_MMU_H) */
