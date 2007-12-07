/** ============================================================================
 *  @file   hal_mpui.c
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Defines necessary functions for MPU Port Interface.
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

/*  ----------------------------------- Profiling                   */
/*  ----------------------------------- OSAL Headers                */
#include <print.h>

/*  ----------------------------------- Hardware Abstraction Layer  */
#include <hal_object.h>
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
#define  COMPONENT_ID       ID_HAL_API

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_HAL_API, __LINE__)


/** ============================================================================
 *  @func   HAL_MpuiSetup
 *
 *  @desc   Setup configuration of the ARM Port Interface.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MpuiSetup (IN HalObject *           halObject,
               IN MPUI_FreqMode         freqMode,
               IN MPUI_TimeoutEn        timeoutEn,
               IN MPUI_ErrMode          errMode,
               IN MPUI_AccessFactor     accessFactor,
               IN MPUI_Timeout          timeout,
               IN MPUI_Endianism        endianism,
               IN MPUI_AccessPriority   accessPriority)
{
    TRC_4ENTER ("HAL_MpuiSetup",
                    halObject, freqMode, timeout, errMode) ;
    TRC_4ENTER ("HAL_MpuiSetup",
                    accessFactor, timeout, endianism, accessPriority) ;

    DBC_Require (halObject != NULL) ;

    REG32 (halObject->baseAPIF + MPUI_CTRL_OFFSET) =
                           (freqMode                                       |
                            timeoutEn       << MPUI_TIMEOUT_EN_POSBIT      |
                            errMode         << MPUI_MPUI_ERR_EN_POSBIT     |
                            accessFactor    << MPUI_ACCESS_FACTOR_POSBIT   |
                            timeout         << MPUI_TIMEOUT_POSBIT         |
                            endianism       << MPUI_ENDIANISM_POSBIT       |
                            accessPriority  << MPUI_ACCESS_PRIORITY_POSBIT) ;

    TRC_0LEAVE ("HAL_MpuiSetup") ;
}


/** ============================================================================
 *  @func   HAL_MpuiBootConfig
 *
 *  @desc   Setup boot configuration of the DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MpuiBootConfig (IN HalObject *      halObject,
                   IN MPUI_DSPBootMode  dsp_boot_mode)
{
    TRC_2ENTER ("HAL_MpuiBootConfig", halObject, dsp_boot_mode) ;

    DBC_Require (halObject != NULL) ;

    REG32 (halObject->baseAPIF + MPUI_DSP_BOOT_OFFSET) = dsp_boot_mode ;

    TRC_0LEAVE ("HAL_MpuiBootConfig") ;
}


/** ============================================================================
 *  @func   HAL_MpuiSetSize
 *
 *  @desc   Write size to DSP_MPUI_CONFIG register
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MpuiSetSize (IN HalObject *   halObject,
                IN Uint32        size)
{
    TRC_2ENTER ("HAL_MpuiSetSize", halObject, size) ;

    DBC_Require (halObject != NULL) ;

    REG16 (halObject->baseAPIF + MPUI_SIZE_OFFSET) = (Uint16) size ;

    TRC_0LEAVE ("HAL_MpuiSetSize") ;
}


/** ============================================================================
 *  @func   HAL_MpuiGetSize
 *
 *  @desc   Gets API size.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Uint32
HAL_MpuiGetSize (IN HalObject * halObject)
{
    Uint32 retVal  = 0 ;

    TRC_1ENTER ("HAL_MpuiGetSize", halObject) ;

    DBC_Require (halObject != NULL) ;

    retVal = (Uint32) REG16(halObject->baseAPIF + MPUI_SIZE_OFFSET)  ;

    TRC_1LEAVE ("HAL_MpuiGetSize", retVal) ;

    return retVal ;
}


/** ============================================================================
 *  @func   HAL_IsDspInSAM
 *
 *  @desc   Function checks if MPUI is in SAM mode.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_IsDspInSAM (IN HalObject * halObject)
{
    Bool   retVal       = FALSE  ;
    Uint16 dspStatusVal = 0x0000 ;

    TRC_1ENTER ("HAL_IsDspInSAM", halObject) ;

    DBC_Require (halObject != NULL) ;

    dspStatusVal = REG16 (halObject->baseAPIF + MPUI_DSP_STATUS_OFFSET) ;

    if ((~(dspStatusVal) & HOM_MODE_VAL) == HOM_MODE_VAL) {
        retVal = TRUE ;
    }
    else {
        retVal = FALSE ;
    }

    TRC_1LEAVE ("HAL_IsDspInSAM", retVal) ;

    return retVal ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
