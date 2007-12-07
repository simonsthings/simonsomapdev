/** ============================================================================
 *  @file   hal_clkrst.c
 *
 *  @path   $(DSPLINK)\gpp\\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Defines necessary functions for CLOCK and RESET modules.
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

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>

/*  ----------------------------------- Hardware Abstraction Layer  */
#include <hal_object.h>
#include <hal_clkrst.h>
#include <hal_mpui.h>

#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_HAL_CLKRST

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_HAL_CLKRST, __LINE__)

/** ============================================================================
 *  @const  TIMEOUT_LOCK
 *
 *  @desc   Timout lock.
 *  ============================================================================
 */
#define TIMEOUT_LOCK    31000


/** ============================================================================
 *  @func   HAL_DPLL1SetClockandLock
 *
 *  @desc   Modify the value output from DPLL.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_DPLL1SetClockandLock (IN HalObject *    halObject,
                          IN Int8           multiplier,
                          IN Int8           divider)
{
    DSP_STATUS status = DSP_SOK ;
    Bool   retVal = TRUE ;
    Uint32 i      = 0    ;
    Uint16 temp   = 0    ;

    TRC_3ENTER ("HAL_DPLL1SetClockandLock", halObject, multiplier, divider) ;

    DBC_Require (halObject != NULL) ;

    if ((divider < DPLL_DIVIDER_MIN) || (divider > DPLL_DIVIDER_MAX)) {
        retVal = FALSE ;

        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }

    if (retVal == TRUE) {
        if (   (multiplier < DPLL_MULTIPLIER_MIN)
            || (multiplier > DPLL_MULTIPLIER_MAX)) {
            retVal = FALSE ;

            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    if (retVal == TRUE) {
        CLEAR_BIT (REG16 (halObject->baseDPLL1), PLL_ENABLE_POS) ;

        temp = REG16 (halObject->baseDPLL1) ;

        SET_NBITS16 (temp, PLL_MULT_POS  , PLL_MULT_NUM, multiplier) ;
        SET_NBITS16 (temp, PLL_DIV_POS   , PLL_DIV_NUM,  divider) ;
        SET_BIT     (temp, PLL_ENABLE_POS) ;

        REG16 (halObject->baseDPLL1) = temp ;

        do {
            i++ ;
        } while (   (!GET_NBITS16 (halObject->baseDPLL1, LOCK_POS, LOCK_NUM))
                 && (i < TIMEOUT_LOCK)) ;
    }

    if (i > TIMEOUT_LOCK) {
        retVal = FALSE ;
    }

    TRC_1LEAVE ("HAL_DPLL1SetClockandLock", retVal) ;

    return retVal ;
}


/** ============================================================================
 *  @func   HAL_SetClockMode
 *
 *  @desc   Set clock mode.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_SetClockMode (IN HalObject * halObject,
                  IN CLOCK_Mode  clockMode)
{
    TRC_2ENTER ("HAL_SetClockMode", halObject, clockMode) ;

    DBC_Require (halObject != NULL) ;

    SET_NBITS16 (REG16 (halObject->baseCLKM1 + ARM_SYSST_OFFSET),
                 ARM_SYSST_CLOCK_SELECT_POS,
                 ARM_SYSST_CLOCK_SELECT_NUM,
                 clockMode);

    TRC_0LEAVE ("HAL_SetClockMode") ;
}


/** ============================================================================
 *  @func   HAL_SetClockDivisor
 *
 *  @desc   Set the divisor for given clock domain.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_SetClockDivisor (IN HalObject *     halObject,
                     IN CLOCK_Domain    domain,
                     IN CLOCK_Divisor   clockDiv)
{
    TRC_3ENTER ("HAL_SetClockDivisor", halObject, domain, clockDiv) ;

    DBC_Require (halObject != NULL) ;

    switch (domain) {
    case CLOCK_DSP:
        SET_NBITS16 (REG16 (halObject->baseCLKM1 + ARM_CKCTL_OFFSET),
                     ARM_CKCTL_DSP_DIV_POS,
                     ARM_CKCTL_DSP_DIV_NUM,
                     clockDiv) ;
        break ;

    case CLOCK_DSP_MMU:
        SET_NBITS16 (REG16 (halObject->baseCLKM1 + ARM_CKCTL_OFFSET),
                     ARM_CKCTL_DSP_MMUDIV_POS,
                     ARM_CKCTL_DSP_MMUDIV_NUM,
                     clockDiv) ;
        break ;

    default:
        TRC_0PRINT (TRC_LEVEL7, "Invalid clock specified.\n") ;
        break ;
    }

    TRC_0LEAVE ("HAL_SetDspClockDivisor") ;
}


/** ============================================================================
 *  @func   HAL_ControlDspInterface
 *
 *  @desc   Controls the priority registers (Rhea module), the EMIF's
 *          configuration registers and API control logic in MEGASTAR3.
 *
 *          If cmd is RESET,
 *              Priority, EMIF configuration registers and the API is reset.
 *          If cmd is RELEASE,
 *              Priority, EMIF configuration registers can be programmed.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ControlDspInterface (IN HalObject * halObject, RSTCT_Control cmd)
{
    TRC_2ENTER ("HAL_ControlDspInterface", halObject, cmd) ;

    DBC_Require (halObject != NULL) ;

    if (cmd == RSTCT_Reset) {
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_RSTCT1_OFFSET),
                   ARM_RSTCT1_DSP_RST_POS) ;
    }
    else if (cmd == RSTCT_Release) {
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_RSTCT1_OFFSET),
                 ARM_RSTCT1_DSP_RST_POS) ;
    }
    else {
        TRC_0PRINT (TRC_LEVEL7, "Invalid request.\n") ;
    }

    TRC_0LEAVE ("HAL_ControlDspInterface") ;
}


/** ============================================================================
 *  @func   HAL_ControlDsp
 *
 *  @desc   Controls the DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ControlDsp (IN HalObject * halObject, RSTCT_Control cmd)
{
    TRC_2ENTER ("HAL_ControlDsp", halObject, cmd) ;

    DBC_Require (halObject != NULL) ;

    if (cmd == RSTCT_Reset) {
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_RSTCT1_OFFSET),
                   ARM_RSTCT1_DSP_EN_POS) ;
    }
    else if (cmd == RSTCT_Release) {
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_RSTCT1_OFFSET),
                 ARM_RSTCT1_DSP_EN_POS) ;

        /*  Wait for DSP to transition from HOM to SAM mode. */
        while (HAL_IsDspInSAM (halObject) == FALSE) ;
    }
    else {
        TRC_0PRINT (TRC_LEVEL7, "Invalid request.\n") ;
    }

    TRC_0LEAVE ("HAL_ControlDsp") ;
}


/** ============================================================================
 *  @func   HAL_ControlExtPer
 *
 *  @desc   ENABLE/DISABLE external peripherals connected to ARM Rhea.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ControlExtPer (IN HalObject * halObject, IN RSTCT_Control cmd)
{
    TRC_2ENTER ("HAL_CtrlExtPer", halObject, cmd) ;

    DBC_Require (halObject != NULL) ;

    if (cmd == RSTCT_Enable) {
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_RSTCT2_OFFSET),
                 ARM_RSTCT2_PER_EN_POS) ;
    }
    else if (cmd == RSTCT_Disable) {
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_RSTCT2_OFFSET),
                   ARM_RSTCT2_PER_EN_POS) ;
    }
    else {
        TRC_0PRINT (TRC_LEVEL7, "Invalid request.\n") ;
    }

    TRC_0LEAVE ("HAL_CtrlExtPer") ;
}


/** ============================================================================
 *  @func   HAL_ClockEnable
 *
 *  @desc   Enables the given clock domain.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ClockEnable (IN HalObject * halObject, CLOCK_Domain domain)
{
    TRC_2ENTER ("HAL_ClockEnable", halObject, domain) ;

    DBC_Require (halObject != NULL) ;

    switch (domain) {
    case CLOCK_ARM_TIMER:
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                 ARM_IDLECT2_EN_TIMCK_POS) ;
        break ;

    case CLOCK_ARM_PER:
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                 ARM_IDLECT2_EN_ARMPERCK_POS) ;
        break ;

    case CLOCK_ARM_GPIO:
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                 ARM_IDLECT2_EN_GPIOCK_POS) ;
        break ;

    case CLOCK_ARM_XORP:
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                 ARM_IDLECT2_EN_XORPCK_POS) ;
        break ;

    case CLOCK_MPUI:
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                 ARM_IDLECT2_EN_MPUICK_POS) ;
        break ;

    case CLOCK_DSP:
        SET_BIT (REG16 (halObject->baseCLKM1 + ARM_CKCTL_OFFSET),
                 ARM_CKCTL_EN_DSPCK_POS) ;
        break ;

    case CLOCK_DSP_TIMER:
        SET_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                 DSP_IDLECT2_EN_TIMCK_POS) ;
        break ;

    case CLOCK_DSP_PER:
        SET_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                 DSP_IDLECT2_EN_PERCK_POS) ;
        break ;

    case CLOCK_DSP_GPIO:
        SET_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                 DSP_IDLECT2_EN_GPIOCK_POS) ;
        break ;

    case CLOCK_DSP_XORP:
        SET_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                 DSP_IDLECT2_EN_XORPCK_POS) ;
        break ;

    case CLOCK_DSP_WDT:
        SET_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                 DSP_IDLECT2_EN_WDTCK_POS) ;
        break ;

    default:
        TRC_0PRINT (TRC_LEVEL7, "Invalid clock specified.\n") ;
        break ;
    }

    TRC_0LEAVE ("HAL_ClockEnable") ;
}


/** ============================================================================
 *  @func   HAL_ClockDisable
 *
 *  @desc   Disables the given clock domain.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_ClockDisable (IN HalObject * halObject, CLOCK_Domain domain)
{
    TRC_2ENTER ("HAL_ClockDisable", halObject, domain) ;

    DBC_Require (halObject != NULL) ;

    switch (domain) {
    case CLOCK_ARM_TIMER:
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                   ARM_IDLECT2_EN_TIMCK_POS) ;
        break ;

    case CLOCK_ARM_PER:
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                   ARM_IDLECT2_EN_ARMPERCK_POS) ;
        break ;

    case CLOCK_ARM_GPIO:
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                   ARM_IDLECT2_EN_GPIOCK_POS) ;
        break ;

    case CLOCK_ARM_XORP:
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                   ARM_IDLECT2_EN_XORPCK_POS) ;
        break ;

    case CLOCK_MPUI:
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                   ARM_IDLECT2_EN_MPUICK_POS) ;
        break ;

    case CLOCK_DSP:
        CLEAR_BIT (REG16 (halObject->baseCLKM1 + ARM_CKCTL_OFFSET),
                   ARM_CKCTL_EN_DSPCK_POS) ;
        break ;

    case CLOCK_DSP_TIMER:
        CLEAR_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                   DSP_IDLECT2_EN_TIMCK_POS) ;
        break ;

    case CLOCK_DSP_PER:
        CLEAR_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                   DSP_IDLECT2_EN_PERCK_POS) ;
        break ;

    case CLOCK_DSP_GPIO:
        CLEAR_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                   DSP_IDLECT2_EN_GPIOCK_POS) ;
        break ;

    case CLOCK_DSP_XORP:
        CLEAR_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                   DSP_IDLECT2_EN_XORPCK_POS) ;
        break ;

    case CLOCK_DSP_WDT:
        CLEAR_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                   DSP_IDLECT2_EN_WDTCK_POS) ;
        break ;

    default:
        TRC_0PRINT (TRC_LEVEL7, "Invalid clock specified.\n") ;
        break ;
    }

    TRC_0LEAVE ("HAL_ClockDisable") ;
}


/** ============================================================================
 *  @func   HAL_IsClockEnabled
 *
 *  @desc   Checks if given clock domain is enabled.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_IsClockEnabled (IN HalObject * halObject, CLOCK_Domain domain)
{
    Bool retVal = FALSE ;

    TRC_2ENTER ("HAL_IsClockEnabled", halObject, domain) ;

    DBC_Require (halObject != NULL) ;

    switch (domain) {
    case CLOCK_ARM_TIMER:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                           ARM_IDLECT2_EN_TIMCK_POS) ;
        break ;

    case CLOCK_ARM_PER:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                           ARM_IDLECT2_EN_ARMPERCK_POS) ;
        break ;

    case CLOCK_ARM_GPIO:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                           ARM_IDLECT2_EN_GPIOCK_POS) ;
        break ;

    case CLOCK_ARM_XORP:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                           ARM_IDLECT2_EN_XORPCK) ;
        break ;

    case CLOCK_MPUI:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM1 + ARM_IDLECT2_OFFSET),
                           ARM_IDLECT2_EN_MPUICK_POS) ;
        break ;

    case CLOCK_DSP:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM1 + ARM_CKCTL_OFFSET),
                           ARM_CKCTL_EN_DSPCK_POS) ;
        break ;

    case CLOCK_DSP_TIMER:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                           DSP_IDLECT2_EN_TIMCK_POS) ;
        break ;

    case CLOCK_DSP_PER:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                           DSP_IDLECT2_EN_PERCK_POS) ;
        break ;

    case CLOCK_DSP_GPIO:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                           DSP_IDLECT2_EN_GPIOCK_POS) ;
        break ;

    case CLOCK_DSP_XORP:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                           DSP_IDLECT2_EN_XORPCK_POS) ;
        break ;

    case CLOCK_DSP_WDT:
        retVal = TEST_BIT (REG16 (halObject->baseCLKM2 + DSP_IDLECT2_OFFSET),
                           DSP_IDLECT2_EN_WDTCK_POS) ;
        break ;

    default:
        TRC_0PRINT (TRC_LEVEL7, "Invalid clock specified.\n") ;
        break ;
    }

    TRC_1LEAVE ("HAL_IsClockEnabled", retVal) ;

    return retVal ;
}


/** ============================================================================
 *  @func   HAL_IsDspIdle
 *
 *  @desc   Checks if DSP is IDLE.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_IsDspIdle (IN HalObject * halObject)
{
    Bool retVal = FALSE ;

    TRC_1ENTER ("HAL_DspIsIDLE", halObject) ;

    DBC_Require (halObject != NULL) ;

    retVal = TEST_BIT (REG16(halObject->baseCLKM1 + ARM_SYSST_OFFSET),
                       ARM_SYSST_IDLE_DSP_POS) ;

    TRC_1LEAVE ("HAL_DspIsIDLE", retVal) ;

    return retVal ;
}


#if defined (__cplusplus)
}
#endif
