/** ============================================================================
 *  @file   hal_object.h
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP 1510.
 *          Define the Platform specific HAL (Hardware Abstraction Layer)
 *          object.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (HAL_OBJECT_H)
#define HAL_OBJECT_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @name   HalObject
 *
 *  @desc   Hardware Abstraction object.
 *
 *  @field  baseDSPMEM
 *              Base for DSP Memory.
 *  @field  baseCLKM1
 *              Base for CLKM1 registers.
 *  @field  baseCLKM2
 *              Base for CLKM2 registers.
 *  @field  baseDPLL1
 *              Base for DPLL1 register.
 *  @field  baseDSPMMU
 *              Base for DSP MMU registers.
 *  @field  baseAPIF
 *              Base for APIF regsiters.
 *  @field  baseMAILBOX
 *              Base for MAILBOX regsiters.
 *  @field  baseINTH
 *              Base for INTH registers.
 *  ============================================================================
 */
typedef struct HalObject_tag {
    Uint32      signature   ;
    Uint32      baseDSPMEM  ;
    Uint32      baseCLKM1   ;
    Uint32      baseCLKM2   ;
    Uint32      baseDPLL1   ;
    Uint32      baseDSPMMU  ;
    Uint32      baseAPIF    ;
    Uint32      baseMAILBOX ;
    Uint32      baseINTH    ;
} HalObject ;


/** ============================================================================
 *  @func   HAL_Initialize
 *
 *  @desc   Allocates and initializes the HAL object.
 *
 *  @arg    halObject
 *              Platform specific HAL object.
 *
 *  @ret    None
 *
 *  @enter  halObject pointer must be valid.
 *
 *  @leave  None
 *
 *  @see    None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_Initialize (IN ProcessorId dspId, IN HalObject * halObject) ;


/** ============================================================================
 *  @func   HAL_Finalize
 *
 *  @desc   Releases the HAL object.
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
HAL_Finalize (IN HalObject * halObject) ;


#if defined (__cplusplus)
}
#endif


#endif  /* !defined (HAL_OBJECT_H) */
