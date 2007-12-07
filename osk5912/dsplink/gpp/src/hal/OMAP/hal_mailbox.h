/** ============================================================================
 *  @file   hal_mailbox.h
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Declares necessary functions for Mailbox Interrupt Handling.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_MAILBOX_H)
#define HAL_MAILBOX_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @const  MB_XXX_XXX_OFFSET
 *
 *  @desc   Offsets of Mailbox Interrupt registers (DSP2ARM and ARM2DSP).
 *  ============================================================================
 */
#define MB_ARM2DSP1_REG_OFFSET         0x00
#define MB_ARM2DSP1B_REG_OFFSET        0x04

#define MB_DSP2ARM1_REG_OFFSET         0x08
#define MB_DSP2ARM1B_REG_OFFSET        0x0C

#define MB_DSP2ARM2_REG_OFFSET         0x10
#define MB_DSP2ARM2B_REG_OFFSET        0x14

#define MB_ARM2DSP1_FLAG_REG_OFFSET    0x18
#define MB_DSP2ARM1_FLAG_REG_OFFSET    0x1C
#define MB_DSP2ARM2_FLAG_REG_OFFSET    0x20

#define MB_ARM2DSP2_REG_OFFSET         0x24
#define MB_ARM2DSP2B_REG_OFFSET        0x28

#define MB_ARM2DSP2_FLAG_REG_OFFSET    0x2C


/** ============================================================================
 *  @const  MB_XXX_RESET_VALUE
 *
 *  @desc   Reset values of Mailbox registers.
 *  ============================================================================
 */
#define MB_ARM2DSP1_RESET_VALUE        0x0000
#define MB_ARM2DSP1B_RESET_VALUE       0x0000
#define MB_DSP2ARM1_RESET_VALUE        0x0000
#define MB_DSP2ARM1B_RESET_VALUE       0x0000
#define MB_DSP2ARM2_RESET_VALUE        0x0000
#define MB_DSP2ARM2B_RESET_VALUE       0x0000
#define MB_ARM2DSP2_RESET_VALUE        0x0000
#define MB_ARM2DSP2B_RESET_VALUE       0x0000

/** ============================================================================
 *  @const  MB_XXX_FLAG
 *
 *  @desc   Flag values for ARM2DSP and DSP2ARM Mailboxes.
 *  ============================================================================
 */
#define MB_ARM2DSP_FLAG                0x0001
#define MB_DSP2ARM_FLAG                0x0001


/** ============================================================================
 *  @const  DSP_MAILBOX1_INT, DSP_MAILBOX2_INT, HSB_MAILBOX_INT
 *
 *  @desc   Index to Mailbox Interrupt handlers.
 *  ============================================================================
 */
#define   DSP_MAILBOX1_INT             10
#define   DSP_MAILBOX2_INT             11
#define   HSB_MAILBOX_INT              12


/** ============================================================================
 *  @name   Mailbox_Enum
 *
 *  @desc   Enumerations for identifying mailbox.
 *
 *  @field  ARM2DSP1
 *              Mailbox identifier for ARM 2 DSP.
 *  @field  DSP2ARM1
 *              Mailbox identifier for DSP 2 ARM.
 *  @field  DSP2ARM2
 *              Mailbox identifier for DSP 2 ARM.
 *  @field  ARM2DSP2
 *              Mailbox identifier for ARM 2 DSP.
 *  ============================================================================
 */
typedef enum {
  ARM2DSP1 = 0,
  DSP2ARM1 = 1,
  DSP2ARM2 = 2,
  ARM2DSP2 = 3
} Mailbox_Enum ;


/** ============================================================================
 *  @func   HAL_MailboxIntDisable
 *
 *  @desc   Disable the DSP interrupt (ARM2DSP Mailbox 1).
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MailboxIntDisable (IN HalObject * halObject) ;


/** ============================================================================
 *  @func   HAL_MailboxIntEnable
 *
 *  @desc   Configure & Enable the DSP interrupt (ARM2DSP Mailbox 1).
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    type
 *              Specifies low level, or falling edge sensitive.
 *
 *  @ret    None
 *
 *  @enter  halObject must be valid
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MailboxIntEnable (IN HalObject * halObject, IN Uint32 type) ;


/** ============================================================================
 *  @func   HAL_MailboxIntClear
 *
 *  @desc   Clear mailbox interrupt from the DSP (DSP2ARM Mailbox 1).
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
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Uint16
HAL_MailboxIntClear (IN HalObject * halObject) ;


/** ============================================================================
 *  @func   HAL_MailboxInterruptDSP
 *
 *  @desc   Interrupt the DSP through mailbox (ARM2DSP Mailbox 1).
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    intrVal
 *              Interrupt value.
 *
 *  @ret    TRUE
 *              if interrupting the DSP was successful.
 *          FALSE
 *              Otherwise.
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_MailboxInterruptDSP (IN HalObject * halObject,
                         IN Uint16      intrVal) ;


#if defined (__cplusplus)
}
#endif


#endif  /* !defined (HAL_MAILBOX_H) */
