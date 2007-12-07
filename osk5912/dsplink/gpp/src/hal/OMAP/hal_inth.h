/** ============================================================================
 *  @file   hal_inth.h
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Declares necessary functions for interrupt handling.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_INTH_H)
#define HAL_INTH_H


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @const  INTH_IT_REG_OFFSET
 *
 *  @desc   Interrupt register.
 *  ============================================================================
 */
#define INTH_IT_REG_OFFSET             0x00

/** ============================================================================
 *  @const  INTH_MASK_IT_REG_OFFSET
 *
 *  @desc   Mask Interrupt register.
 *  ============================================================================
 */
#define INTH_MASK_IT_REG_OFFSET        0x04

/** ============================================================================
 *  @const  INTH_SOURCE_IRQ_REG_OFFSET
 *
 *  @desc   Src Binary coded IRQ register.
 *  ============================================================================
 */
#define INTH_SOURCE_IRQ_REG_OFFSET     0x08

/** ============================================================================
 *  @const  INTH_SOURCE_FIQ_REG_OFFSET
 *
 *  @desc   Src Binary coded FIQ register.
 *  ============================================================================
 */
#define INTH_SOURCE_FIQ_REG_OFFSET     0x0C

/** ============================================================================
 *  @const  INTH_SOURCE_BIN_IRQ_REG_OFFSET
 *
 *  @desc   Src Binary coded IRQ register.
 *  ============================================================================
 */
#define INTH_SOURCE_BIN_IRQ_REG_OFFSET 0x10

/** ============================================================================
 *  @const  INTH_SOURCE_BIN_FIQ_REG_OFFSET
 *
 *  @desc   Src Binary coded FIQ register.
 *  ============================================================================
 */
#define INTH_SOURCE_BIN_FIQ_REG_OFFSET 0x14

/** ============================================================================
 *  @const  INTH_CTRL_REG_OFFSET
 *
 *  @desc   Control register offset.
 *  ============================================================================
 */
#define INTH_CTRL_REG_OFFSET           0x18

/** ============================================================================
 *  @const  INTH_IT_LEVEL_REG_OFFSET
 *
 *  @desc   Interrupt Level registers.
 *  ============================================================================
 */
#define INTH_IT_LEVEL_REG_OFFSET       0x1C

/** ============================================================================
 *  @const  INTH_FIQNIRQ_POSBIT
 *
 *  @desc   IRQ bit position.
 *  ============================================================================
 */
#define INTH_FIQNIRQ_POSBIT               0

/** ============================================================================
 *  @const  INTH_PRIORITY_POSBIT
 *
 *  @desc   Priority bit position.
 *  ============================================================================
 */
#define INTH_PRIORITY_POSBIT              2

/** ============================================================================
 *  @const  INTH_SENSITIVE_EDGE_POSBIT
 *
 *  @desc   Sensitive edge bit position.
 *  ============================================================================
 */
#define INTH_SENSITIVE_EDGE_POSBIT        1

/** ============================================================================
 *  @name   INTH_InterruptKind
 *
 *  @desc   Kind of interrupt - FIQ or IRQ.
 *
 *  @field  INTH_IRQ
 *              IRQ interrupt.
 *  @field  INTH_FIQ
 *              FIQ interrupt.
 *  ============================================================================
 */
typedef enum {
    INTH_IRQ = 0,
    INTH_FIQ = 1
} INTH_InterruptKind ;


/** ============================================================================
 *  @name   INTH_SensitiveEdge
 *
 *  @desc   When to sense an interrupt - Falling edge or Low level.
 *
 *  @field  FALLING_EDGE_SENSITIVE
 *              Sense interrupt on falling edge.
 *  @field  LOW_LEVEL_SENSITIVE
 *              Sense interrupt on low level.
 *  ============================================================================
 */
typedef enum {
    FALLING_EDGE_SENSITIVE = 0,
    LOW_LEVEL_SENSITIVE    = 1
} INTH_SensitiveEdge ;


#if defined (__cplusplus)
}
#endif


#endif  /* !defined (HAL_INTH_H) */
