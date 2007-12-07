/** ============================================================================
 *  @file   hal_mmu.c
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Defines necessary functions for MMU operations.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <errbase.h>
#include <bitops.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <print.h>

/*  ----------------------------------- Hardware Abstraction Layer  */
#include <hal_object.h>
#include <hal_mmu.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_HAL_MMU

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_HAL_MMU, __LINE__)


/** ============================================================================
 *  @func   HAL_ControlDspMmu
 *
 *  @desc   Controls (RESET/ RELEASE/ ENABLE/ DISABLE) the DSP MMU.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ControlDspMmu (IN HalObject * halObject, MMU_Control cmd)
{
    TRC_2ENTER ("HAL_ControlDspMmu", halObject, cmd) ;

    DBC_Require (halObject != NULL) ;

    switch (cmd) {
    case MMU_Enable :
        SET_BITS (REG16 (halObject->baseDSPMMU + CNTL_REG_OFFSET),
                  MMU_ENABLE_MASK) ;
        break ;

    case MMU_Disable :
        CLEAR_BITS (REG16 (halObject->baseDSPMMU + CNTL_REG_OFFSET),
                    MMU_ENABLE_MASK) ;
        break ;

    case MMU_DisableWtl :
        CLEAR_BITS (REG16 (halObject->baseDSPMMU + CNTL_REG_OFFSET),
                    WTL_ENABLE_MASK) ;
        break ;

    case MMU_Reset :
        CLEAR_BITS (REG16 (halObject->baseDSPMMU + CNTL_REG_OFFSET),
                    RESET_SW_MMU_MASK) ;
        break ;

    case MMU_Release :
        SET_BITS (REG16 (halObject->baseDSPMMU + CNTL_REG_OFFSET),
                  RESET_SW_MMU_MASK) ;
        break ;

    default :
        DBC_Assert (FALSE) ;
        TRC_0PRINT (TRC_LEVEL7, "Invalid command specified.\n") ;
        break ;
    }

    TRC_0LEAVE ("HAL_DspMmuEnable") ;
}


/** ============================================================================
 *  @func   HAL_DspMmuTlbEntry
 *
 *  @desc   To load one item in the TLB, 5 consecutive RHEA register accesses
 *          are required.
 *
 *          This entry is loaded at the address pointed by the lock counter
 *          register.
 *          1. Write CAM msb in CAM_REG_H register.
 *          2. Write CAM lsb in CAM_REG_L register.
 *          3. Write RAM msb in RAM_REG_H register.
 *          4. Write RAM lsb in RAM_REG_L register.
 *          5. Update Lock Counter register.
 *          6. Write 1 in LD_TLB_REG register.
 *
 *          Limitations :
 *          It is possible to load an entry in the TLB only if the WTL
 *          is DISABLED.
 *
 *  @modif  None.
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
                    IN MMU_PRESERVED    p_bit)
{
    Uint16 VA_tag_I2  ;
    Uint16 VA_tag_I1  ;
    Uint32 baseDspMmu ;

    TRC_4ENTER ("HAL_DspMmuTlbEntry",
                    halObject, phyAddr, virAddr, slst_bit) ;
    TRC_4ENTER ("HAL_DspMmuTlbEntry",
                    ap_bits, locked_base_value, current_entry, p_bit) ;

    DBC_Require (halObject != NULL) ;

    baseDspMmu = halObject->baseDSPMMU ;
    VA_tag_I1 = (Uint16) ((virAddr & 0xFFF00000) >> 20) ; /* First  12 bits */
    VA_tag_I2 = (Uint16) ((virAddr & 0x000FFC00) >> 10) ; /* Middle 10 bits */

    /* Write CAM msb and lsb into CAM_REG_H and CAM_REG_L registers */
    REG16 (baseDspMmu + CAM_H_REG_OFFSET) = (Uint16) (VA_tag_I1 >> 2) ;
    REG16 (baseDspMmu + CAM_L_REG_OFFSET) =
                                 (Uint16) (   slst_bit
                                           | (p_bit << 3)
                                           | (VA_tag_I2 << 4)
                                           | (  (VA_tag_I1 & 0x0003) << 14)) ;

    /* Write RAM msb and lsb into RAM_REG_H and RAM_REG_L registers */
    REG16 (baseDspMmu + RAM_H_REG_OFFSET) = (Uint16) (phyAddr >> 16) ;
    REG16 (baseDspMmu + RAM_L_REG_OFFSET) = (Uint16) (  (ap_bits << 8)
                                                      | (phyAddr & 0xFC00)) ;

    /* Update lock counter register */
    REG16 (baseDspMmu + LOCK_REG_OFFSET) =
                                      (Uint16) (  (current_entry << 4)
                                                | (locked_base_value << 10)) ;

    /* Write 1 into LD_TLB_REG register */
    REG16 (baseDspMmu + LD_TLB_REG_OFFSET) = 1 ;

    TRC_0LEAVE ("HAL_DspMmuTlbEntry") ;
}


/** ============================================================================
 *  @func   HAL_DspMmuIntClear
 *
 *  @desc   Checks and clears the MMU fault interrupt.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_DspMmuIntClear (IN HalObject * halObject)
{
#if defined (DDSP_DEBUG)
    Uint16   status          ;
    volatile Uint16 addrLow  ;
    volatile Uint16 addrHigh ;
#endif
    Uint32 baseDspMmu    ;

    TRC_1ENTER ("HAL_DspMmuIntClear", halObject) ;

    DBC_Require (halObject != NULL) ;

    baseDspMmu = halObject->baseDSPMMU ;

#if defined (DDSP_DEBUG)
    /*  Read fault address register */
    addrHigh = REG16 (baseDspMmu + FAULT_ADDR_H_REG_OFFSET) ;
    addrLow  = REG16 (baseDspMmu + FAULT_ADDR_L_REG_OFFSET) ;

    /* Read status register */
    status   = REG16 (baseDspMmu + FAULT_ST_REG_OFFSET) ;

    if (status != 0) {
        TRC_0PRINT (TRC_LEVEL7, "*** DSP MMU Fault ***\n") ;
        TRC_1PRINT (TRC_LEVEL7, "Fault status: 0x%x", status) ;
        TRC_1PRINT (TRC_LEVEL7, "Fault address high: 0x%x", addrHigh) ;
        TRC_1PRINT (TRC_LEVEL7, "Fault address low: 0x%x", addrLow) ;
    }
#endif  /* defined (DDSP_DEBUG) */

    /* Acknowledge interrupt */
    SET_BITS (REG16 (baseDspMmu + IT_ACK_REG_OFFSET),
              IT_ACK_MASK) ;

    TRC_0LEAVE ("HAL_DspMmuIntClear") ;
}


#if defined (__cplusplus)
}
#endif
