/** ============================================================================
 *  @file   hal_clkrst.h
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Declares necessary functions for CLOCK and RESET modules.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_CLKRST_H)
#define HAL_CLKRST_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/*  ****************************************************************************
 *  DPLL Control Register (CTL_REG)
 *  ****************************************************************************
 */
/*  ============================================================================
 *  @const  DPLL_MULTIPLIER_X
 *
 *  @desc   Multipliers for the DPLL
 *  ============================================================================
 */
#define   DPLL_MULTIPLIER_1      (Int8)1
#define   DPLL_MULTIPLIER_2      (Int8)2
#define   DPLL_MULTIPLIER_4      (Int8)4
#define   DPLL_MULTIPLIER_8      (Int8)8
#define   DPLL_MULTIPLIER_10     (Int8)10
#define   DPLL_MULTIPLIER_12     (Int8)12
#define   DPLL_MULTIPLIER_14     (Int8)14
#define   DPLL_MULTIPLIER_16     (Int8)16
#define   DPLL_MULTIPLIER_20     (Int8)20
#define   DPLL_MULTIPLIER_30     (Int8)30
#define   DPLL_MULTIPLIER_31     (Int8)31

#define   DPLL_MULTIPLIER_MIN    DPLL_MULTIPLIER_1
#define   DPLL_MULTIPLIER_MAX    DPLL_MULTIPLIER_31

/*  ============================================================================
 *  @const  DPLL_DIVIDER_X
 *
 *  @desc   Dividers for the DPLL
 *  ============================================================================
 */
#define   DPLL_DIVIDER_1         (Int8)0
#define   DPLL_DIVIDER_2         (Int8)1
#define   DPLL_DIVIDER_4         (Int8)2
#define   DPLL_DIVIDER_8         (Int8)3

#define   DPLL_DIVIDER_MIN       DPLL_DIVIDER_1
#define   DPLL_DIVIDER_MAX       DPLL_DIVIDER_8


/*  ============================================================================
 *  @const  PLL_MULT_POS, PLL_MULT_NUM
 *
 *  @desc   PLL Multiplier position and number.
 *  ============================================================================
 */
#define PLL_MULT_POS                    7
#define PLL_MULT_NUM                    5

/*  ============================================================================
 *  @const  PLL_DIV_POS, PLL_DIV_NUM
 *
 *  @desc   PLL Divider position and number.
 *  ============================================================================
 */
#define PLL_DIV_POS                     5
#define PLL_DIV_NUM                     2

/*  ============================================================================
 *  @const  PLL_ENABLE_POS, PLL_ENABLE_NUM
 *
 *  @desc   PLL Enable position and number.
 *  ============================================================================
 */
#define PLL_ENABLE_POS                  4
#define PLL_ENABLE_NUM                  1

/*  ============================================================================
 *  @const  BYPASS_DIV_POS, BYPASS_DIV_NUM
 *
 *  @desc   BYPASS divider position and number.
 *  ============================================================================
 */
#define BYPASS_DIV_POS                  2
#define BYPASS_DIV_NUM                  2

/*  ============================================================================
 *  @const  LOCK_POS, LOCK_NUM
 *
 *  @desc   Lock position and number.
 *  ============================================================================
 */
#define LOCK_POS                        0
#define LOCK_NUM                        1


/*  ****************************************************************************
 *  ARM_CKCTL (MPU Clock Control Register)
 *  ****************************************************************************
 */

/*  ============================================================================
 *  @const  ARM_CKCTL_OFFSET
 *
 *  @desc   ARM Clock control offset.
 *  ============================================================================
 */
#define ARM_CKCTL_OFFSET                0x00

/*  ============================================================================
 *  @const  ARM_CKCTL_EN_DSPCK_POS, ARM_CKCTL_EN_DSPCK_NUM
 *
 *  @desc   Position and number of bits to enable clock to DSP.
 *  ============================================================================
 */
#define ARM_CKCTL_EN_DSPCK_POS          13
#define ARM_CKCTL_EN_DSPCK_NUM          1

/*  ============================================================================
 *  @const  ARM_CKCTL_DSP_MMUDIV_POS, ARM_CKCTL_DSP_MMUDIV_NUM
 *
 *  @desc   Position and number of bits to define divisor for the DSPMMU
 *          clock domain.
 *  ============================================================================
 */
#define ARM_CKCTL_DSP_MMUDIV_POS        10
#define ARM_CKCTL_DSP_MMUDIV_NUM        2

/*  ============================================================================
 *  @const  ARM_CKCTL_TCDIV_POS, ARM_CKCTL_TCDIV_NUM
 *
 *  @desc   Position and number of bits to define divisor for the TC
 *          clock domain.
 *  ============================================================================
 */
#define ARM_CKCTL_TCDIV_POS        8
#define ARM_CKCTL_TCDIV_NUM        2

/*  ============================================================================
 *  @const  ARM_CKCTL_DSP_DIV_POS, ARM_CKCTL_DSP_DIV_NUM
 *
 *  @desc   Position and number of bits to define divisor for the DSP
 *          clock domain.
 *  ============================================================================
 */
#define ARM_CKCTL_DSP_DIV_POS           6
#define ARM_CKCTL_DSP_DIV_NUM           2

/*  ============================================================================
 *  @const  ARM_CKCTL_EN_DSPCK
 *
 *  @desc   Mask to enable DSP Clock.
 *  ============================================================================
 */
#define ARM_CKCTL_EN_DSPCK              0x2000

/*  ****************************************************************************
 *  MPU Idle Mode Entry 2 Register (ARM_IDLECT2)
 *  ****************************************************************************
 */

/*  ============================================================================
 *  @const  ARM_IDLECT2_OFFSET
 *
 *  @desc   Offset of ARM_IDLECT2 Register.
 *  ============================================================================
 */
#define ARM_IDLECT2_OFFSET              0x08

/*  ============================================================================
 *  @const  ARM_IDLECT2_EN_GPIOCK_POS, ARM_IDLECT2_EN_GPIOCK_NUM
 *
 *  @desc   Position and number of bits to enable clock of GPIO.
 *  ============================================================================
 */
#define ARM_IDLECT2_EN_GPIOCK_POS       9
#define ARM_IDLECT2_EN_GPIOCK_NUM       1

/*  ============================================================================
 *  @const  ARM_IDLECT2_EN_TIMCK_POS, ARM_IDLECT2_EN_TIMCK_NUM
 *
 *  @desc   Position and number of bits to enable clock to MPUI.
 *  ============================================================================
 */
#define ARM_IDLECT2_EN_TIMCK_POS        7
#define ARM_IDLECT2_EN_TIMCK_NUM        1

/*  ============================================================================
 *  @const  ARM_IDLECT2_EN_MPUICK_POS, ARM_IDLECT2_EN_MPUICK_NUM
 *
 *  @desc   Position and number of bits to enable clock to MPUI.
 *  ============================================================================
 */
#define ARM_IDLECT2_EN_MPUICK_POS       6
#define ARM_IDLECT2_EN_MPUICK_NUM       1

/*  ============================================================================
 *  @const  ARM_IDLECT2_EN_ARMPERCK_POS, ARM_IDLECT2_EN_ARMPERCK_NUM
 *
 *  @desc   Position and number of bits to enable clock to peripherals
 *  ============================================================================
 */
#define ARM_IDLECT2_EN_ARMPERCK_POS     2
#define ARM_IDLECT2_EN_ARMPERCK_NUM     1

/*  ============================================================================
 *  @const  ARM_IDLECT2_EN_XORPCK_POS, ARM_IDLECT2_EN_XORPCK_NUM
 *
 *  @desc   Position and number of bits to enable clock of OS timer.
 *  ============================================================================
 */
#define ARM_IDLECT2_EN_XORPCK_POS       1
#define ARM_IDLECT2_EN_XORPCK_NUM       1


/*  ============================================================================
 *  @const  ARM_IDLECT2_EN_MPUICK
 *
 *  @desc   Mask to enable MPUI Clock.
 *  ============================================================================
 */
#define ARM_IDLECT2_EN_MPUICK           0x0040

/*  ============================================================================
 *  @const  ARM_IDLECT2_EN_XORPCK
 *
 *  @desc   Mask to enable XORP Clock.
 *  ============================================================================
 */
#define ARM_IDLECT2_EN_XORPCK           0x0002


/*  ****************************************************************************
 *  MPU Reset Control 1 Register (ARM_RSTCT1)
 *  ****************************************************************************
 */

/*  ============================================================================
 *  @const  ARM_RSTCT1_OFFSET
 *
 *  @desc   Offset of ARM_RSTCT1 Register.
 *  ============================================================================
 */
#define ARM_RSTCT1_OFFSET               0x10

/*  ============================================================================
 *  @const  ARM_RSTCT1_DSP_RST_POS, ARM_RSTCT1_DSP_RST_NUM
 *
 *  @desc   Position and number of bits to reset DSP.
 *  ============================================================================
 */
#define ARM_RSTCT1_DSP_RST_POS          2
#define ARM_RSTCT1_DSP_RST_NUM          1

/*  ============================================================================
 *  @const  ARM_RSTCT1_DSP_EN_POS, ARM_RSTCT1_DSP_EN_NUM
 *
 *  @desc   Position and number of bits to enable DSP.
 *  ============================================================================
 */
#define ARM_RSTCT1_DSP_EN_POS           1
#define ARM_RSTCT1_DSP_EN_NUM           1


/*  ****************************************************************************
 *  MPU Reset Control 2 Register (ARM_RSTCT2)
 *  ****************************************************************************
 */

/*  ============================================================================
 *  @const  ARM_RSTCT2_OFFSET
 *
 *  @desc   Offset of ARM_RSTCT2 Register.
 *  ============================================================================
 */
#define ARM_RSTCT2_OFFSET               0x14

/*  ============================================================================
 *  @const  ARM_RSTCT2_PER_EN_POS, ARM_RSTCT2_PER_EN_NUM
 *
 *  @desc   Position and number of bits to enable external peripherals.
 *  ============================================================================
 */
#define ARM_RSTCT2_PER_EN_POS           0
#define ARM_RSTCT2_PER_EN_NUM           1


/*  ****************************************************************************
 *  MPU System Status Register (ARM_SYSST)
 *  ****************************************************************************
 */

/*  ============================================================================
 *  @const  ARM_SYSST_OFFSET
 *
 *  @desc   Offset of ARM_SYSST Register.
 *  ============================================================================
 */
#define ARM_SYSST_OFFSET                0x18

/*  ============================================================================
 *  @const  ARM_SYSST_CLOCK_SELECT_POS, ARM_SYSST_CLOCK_SELECT_NUM
 *
 *  @desc   Position and number of bits to select clock.
 *  ============================================================================
 */
#define ARM_SYSST_CLOCK_SELECT_POS      11
#define ARM_SYSST_CLOCK_SELECT_NUM      3

/*  ============================================================================
 *  @const  ARM_SYSST_IDLE_DSP_POS, ARM_SYSST_IDLE_DSP_NUM
 *
 *  @desc   Position and number of bits to check state of DSP.
 *  ============================================================================
 */
#define ARM_SYSST_IDLE_DSP_POS          6
#define ARM_SYSST_IDLE_DSP_NUM          1


/*  ****************************************************************************
 *  DSP Idle Mode Entry 2 Register (DSP_IDLECT2)
 *  ****************************************************************************
 */

/*  ============================================================================
 *  @const  DSP_IDLECT2_OFFSET
 *
 *  @desc   Offset of DSP_IDLECT2 Register.
 *  ============================================================================
 */
#define DSP_IDLECT2_OFFSET              0x8

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_TIMCK_POS, DSP_IDLECT2_EN_TIMCK_NUM
 *
 *  @desc   Position and number of bits to enable DSP Timer clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_TIMCK_POS        5
#define DSP_IDLECT2_EN_TIMCK_NUM        1

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_GPIOCK_POS, DSP_IDLECT2_EN_GPIOCK_NUM
 *
 *  @desc   Position and number of bits to enable DSP GPIO peripheral clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_GPIOCK_POS       4
#define DSP_IDLECT2_EN_GPIOCK_NUM       1

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_PERCK_POS, DSP_IDLECT2_EN_PERCK_NUM
 *
 *  @desc   Position and number of bits to enable DSP peripheral clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_PERCK_POS        2
#define DSP_IDLECT2_EN_PERCK_NUM        1

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_XORPCK_POS, DSP_IDLECT2_EN_XORPCK_NUM
 *
 *  @desc   Position and number of bits to enable DSPXOR_CK reference clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_XORPCK_POS       1
#define DSP_IDLECT2_EN_XORPCK_NUM       1

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_WDTCK_POS, DSP_IDLECT2_EN_WDTCK_NUM
 *
 *  @desc   Position and number of bits to enable clock to DSP watchdog timer.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_WDTCK_POS        0
#define DSP_IDLECT2_EN_WDTCK_NUM        1


/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_ALLCLOCKS_POS, DSP_IDLECT2_EN_ALLCLOCKS_NUM
 *
 *  @desc   Position and number of bits to enable all clocks in DSP.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_ALLCLOCKS_POS    0
#define DSP_IDLECT2_EN_ALLCLOCKS_NUM    6

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_TIMCK
 *
 *  @desc   Mask to enable DSP Timer clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_TIMCK            0x0020

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_GPIOCK
 *
 *  @desc   Mask to enable DSP GPIO clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_GPIOCK           0x0010

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_PERCK
 *
 *  @desc   Mask to enable DSP peripheral clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_PERCK            0x0004

/*  ============================================================================
 *  @const  DSP_IDLECT2_EN_XORPCK
 *
 *  @desc   Mask to enable DSP XORP clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_XORPCK           0x0002

/*  ============================================================================
 *  @const  DSP_EN_WDT_CLK
 *
 *  @desc   Mask to enable DSP Watchdog timer clock.
 *  ============================================================================
 */
#define DSP_IDLECT2_EN_WDTCK            0x0001


/** ============================================================================
 *  @name   CLOCK_Domain
 *
 *  @desc   Enumerates the various clock domains.
 *
 *  @field  CLOCK_ARM_TIMER
 *              ARM Timer Clock
 *  @field  CLOCK_ARM_PER
 *              ARM Peripheral
 *  @field  CLOCK_ARM_GPIO
 *              ARM GPIO Clock
 *  @field  CLOCK_ARM_XORP
 *              ARM XORP Clock
 *  @field  CLOCK_MPUI
 *              MPUI Clock
 *  @field  CLOCK_DSP
 *              DSP Clock
 *  @field  CLOCK_DSP_TIMER
 *              DSP Timer Clock
 *  @field  CLOCK_DSP_PER
 *              DSP Peripheral clock
 *  @field  CLOCK_DSP_GPIO
 *              DSP GPIO Clock
 *  @field  CLOCK_DSP_XORP
 *              DSP XORP Clock
 *  @field  CLOCK_DSP_WD
 *              DSP Watchdog timer Clock
 *  @field  CLOCK_DSP_UART
 *              DSP UART Clock
 *  ============================================================================
 */
typedef enum {
    CLOCK_ARM_TIMER,
    CLOCK_ARM_PER,
    CLOCK_ARM_GPIO,
    CLOCK_ARM_XORP,
    CLOCK_MPUI,
    CLOCK_TC,
    CLOCK_DSP,
    CLOCK_DSP_MMU,
    CLOCK_DSP_TIMER,
    CLOCK_DSP_PER,
    CLOCK_DSP_GPIO,
    CLOCK_DSP_XORP,
    CLOCK_DSP_WDT
} CLOCK_Domain ;


/*  ============================================================================
 *  @name   CLOCK_Mode
 *
 *  @desc   Enumerates various clock modes.
 *
 *  @field  CLOCK_MODE_SYNC
 *              Sync mode.
 *  @field  CLOCK_MODE_ASYNC
 *              Async mode.
 *  @field  CLOCK_MODE_SCAL
 *              scal mode.
 *  @field  CLOCK_MODE_ARM_TOTC
 *              ARM_TOTC mode.
 *  @field  CLOCK_MODE_MGS3_TOTC
 *              Megastar TOTC mode.
 *  @field  CLOCK_MODE_BYPASS
 *              Bypass mode.
 *  @field  CLOCK_MODE_TEST
 *              Test mode.
 *  ============================================================================
 */
typedef enum {
    CLOCK_MODE_SYNC      = 0,
    CLOCK_MODE_ASYNC     = 1,
    CLOCK_MODE_SCAL      = 2,
    CLOCK_MODE_ARM_TOTC  = 3,
    CLOCK_MODE_MGS3_TOTC = 4,
    CLOCK_MODE_BYPASS    = 5,
    CLOCK_MODE_TEST      = 6
} CLOCK_Mode ;


/** ============================================================================
 *  @name   CLOCK_Divisor
 *
 *  @desc   Enumerates various clock divisors.
 *
 *  @field  CLOCK_DIV_BY_1
 *              Divide by 1.
 *  @field  CLOCK_DIV_BY_2
 *              Divide by 2.
 *  @field  CLOCK_DIV_BY_4
 *              Divide by 4.
 *  @field  CLOCK_DIV_BY_8
 *              Divide by 8.
 *  ============================================================================
 */
typedef enum {
    CLOCK_DIV_BY_1 = 0,
    CLOCK_DIV_BY_2 = 1,
    CLOCK_DIV_BY_4 = 2,
    CLOCK_DIV_BY_8 = 3
} CLOCK_Divisor ;


/** ============================================================================
 *  @name   CLOCK_Control
 *
 *  @desc   Defines clock enable/disable actions.
 *  ============================================================================
 */
typedef enum {
    CLOCK_Disable = 0,
    CLOCK_Enable  = 1
} CLOCK_Control ;


/** ============================================================================
 *  @name   RSTCT_Control
 *
 *  @desc   Defines possible control requests using RSTCT register.
 *  ============================================================================
 */
typedef enum {
    RSTCT_Reset   = 0,
    RSTCT_Release = 1,
    RSTCT_Enable  = 2,
    RSTCT_Disable = 3
} RSTCT_Control ;


/** ============================================================================
 *  @func   HAL_DPLL1SetClockandLock
 *
 *  @desc   Modify the value output from DPLL.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    multiplier
 *              Value between 1-31.
 *  @arg    divider
 *              Value between 1-4.
 *
 *  @ret    True
 *              On successful completion.
 *          False
 *              On failure.
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_DPLL1SetClockandLock (IN HalObject * halObject,
                          IN Int8        multiplier,
                          IN Int8        divider) ;


/** ============================================================================
 *  @func   HAL_SetClockMode
 *
 *  @desc   Set the clock mode.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *  @arg    clockMode
 *              The clock mode to be set.
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
Void
HAL_SetClockMode (IN HalObject *  halObject,
                  IN CLOCK_Mode   clockMode) ;


/** ============================================================================
 *  @func   HAL_SetClockDivisor
 *
 *  @desc   Set the divisor for given clock domain.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @arg    domain
 *              Clock domain.
 *
 *  @arg    clockDiv
 *              Value of clock divisor.
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
Void
HAL_SetClockDivisor (IN HalObject *     halObject,
                     IN CLOCK_Domain    domain,
                     IN CLOCK_Divisor   clockDiv) ;


/** ============================================================================
 *  @func   HAL_ControlDspInterface
 *
 *  @desc   Performs control operations on the DSP interface
 *          (API, EMIF, and RHEA).
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
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ControlDspInterface (IN HalObject * halObject, RSTCT_Control cmd) ;


/** ============================================================================
 *  @func   HAL_ControlDsp
 *
 *  @desc   Performs control operations on the DSP.
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
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ControlDsp (IN HalObject * halObject, RSTCT_Control cmd) ;


/** ============================================================================
 *  @func   HAL_ControlExtPer
 *
 *  @desc   Performs control operations (ENABLE/ DISABLE) for external
 *          peripherals connected to ARM Rhea.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @arg    cmd
 *              Control command (ENABLE/ DISABLE).
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
Void
HAL_ControlExtPer (IN HalObject * halObject, RSTCT_Control cmd) ;


/** ============================================================================
 *  @func   HAL_ClockEnable
 *
 *  @desc   Enables the given clock domain.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @arg    domain
 *              Clock domain to be enabled.
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
Void
HAL_ClockEnable (IN HalObject * halObject, CLOCK_Domain domain) ;


/** ============================================================================
 *  @func   HAL_ClockDisable
 *
 *  @desc   Disables the given clock domain.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @arg    domain
 *              Clock domain to be disabled.
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
Void
HAL_ClockDisable (IN HalObject * halObject, CLOCK_Domain domain) ;


/** ============================================================================
 *  @func   HAL_IsClockEnabled
 *
 *  @desc   Checks if given clock domain is enabled.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @arg    domain
 *              Clock domain to be checked.
 *
 *  @ret    TRUE
 *              Clock domain is enabled.
 *          FALSE
 *              Clock domain is not enabled.
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_IsClockEnabled (IN HalObject * halObject, CLOCK_Domain domain) ;


/** ============================================================================
 *  @func   HAL_IsDspIdle
 *
 *  @desc   Checks if DSP is IDLE.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @ret    TRUE
 *              if DSP is IDLE.
 *          FALSE
 *              Otherwise.
 *
 *  @enter  halObject must be valid.
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_IsDspIdle (IN HalObject * halObject) ;


#if defined (__cplusplus)
}
#endif


#endif  /* !defined (HAL_CLKRST_H) */
