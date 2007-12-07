/** ============================================================================
 *  @file   hal_object.c
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Creates and Deletes the platform specific HAL object.
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
#include <dsplink.h>
#include <intobject.h>
#include <errbase.h>
#include <bitops.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <mem.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>

/*  ----------------------------------- Hardware Abstraction Layer  */
#include <hal_object.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif

/*  ============================================================================
 *  @const  BASE_
 *
 *  @desc   Base addresses and offsets for various memory segments
 *          required by 'Link.
 *  ============================================================================
 */
#define  BASE_DSPMEM        0xE0000000

#define  BASE_CLKM1         0xFFFECE00
#define  BASE_CLKM2         0xE1008000
#define  BASE_DPLL1         0xFFFECF00
#define  BASE_DSPMMU        0xFFFED200
#define  BASE_APIF          0xFFFEC900
#define  BASE_MAILBOX       0xFFFCF000
#define  BASE_INTH          0xFFFECB00


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_HAL_OBJECT

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_HAL_OBJECT, __LINE__)


/** ============================================================================
 *  @func   HAL_Initialize
 *
 *  @desc   Initializes the HAL object.
 *          The value of baseSHMMEM is not intialized here.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HAL_Initialize (IN ProcessorId dspId, IN HalObject * halObject)
{
    TRC_2ENTER ("HAL_Initialize", dspId, halObject) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (halObject != NULL) ;

    halObject->signature   = SIGN_HAL     ;
    halObject->baseDSPMEM  = BASE_DSPMEM  ;
    halObject->baseCLKM1   = BASE_CLKM1   ;
    halObject->baseCLKM2   = BASE_CLKM2   ;
    halObject->baseDPLL1   = BASE_DPLL1   ;
    halObject->baseDSPMMU  = BASE_DSPMMU  ;
    halObject->baseAPIF    = BASE_APIF    ;
    halObject->baseMAILBOX = BASE_MAILBOX ;
    halObject->baseINTH    = BASE_INTH    ;

    TRC_0PRINT (TRC_LEVEL1, "--- Linear Mappings ---\n") ;
    TRC_1PRINT (TRC_LEVEL1, "baseDSPMEM =0x%x\n", halObject->baseDSPMEM)  ;
    TRC_1PRINT (TRC_LEVEL1, "baseCLKM1  =0x%x\n", halObject->baseCLKM1)   ;
    TRC_1PRINT (TRC_LEVEL1, "baseCLKM2  =0x%x\n", halObject->baseCLKM2)   ;
    TRC_1PRINT (TRC_LEVEL1, "baseDPLL1  =0x%x\n", halObject->baseDPLL1)   ;
    TRC_1PRINT (TRC_LEVEL1, "baseDSPMMU =0x%x\n", halObject->baseDSPMMU)  ;
    TRC_1PRINT (TRC_LEVEL1, "baseAPIF   =0x%x\n", halObject->baseAPIF)    ;
    TRC_1PRINT (TRC_LEVEL1, "baseMAILBOX=0x%x\n", halObject->baseMAILBOX) ;
    TRC_1PRINT (TRC_LEVEL1, "baseINTH   =0x%x\n", halObject->baseINTH)    ;

    TRC_0LEAVE ("HAL_Initialize") ;
}


/** ============================================================================
 *  @func   HAL_Finalize
 *
 *  @desc   Relaeses the HAL object.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_Finalize (IN HalObject * halObject)
{
    TRC_1ENTER ("HAL_Finalize", halObject) ;

    DBC_Require (halObject != NULL) ;

    halObject->signature   = SIGN_NULL ;
    halObject->baseDSPMEM  = 0 ;
    halObject->baseCLKM1   = 0 ;
    halObject->baseCLKM1   = 0 ;
    halObject->baseDPLL1   = 0 ;
    halObject->baseDSPMMU  = 0 ;
    halObject->baseAPIF    = 0 ;
    halObject->baseMAILBOX = 0 ;
    halObject->baseINTH    = 0 ;

    TRC_0LEAVE ("HAL_Finalize") ;
}


#if defined (__cplusplus)
}
#endif
