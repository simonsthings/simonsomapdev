/** ============================================================================
 *  @file   hal.c
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   Implementation of Hardware Abstraction Layer Module.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <sys.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <hal.h>
#include <emif.h>
#include <interrupt.h>
#include <failure.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  FILEID
 *
 *  @desc   File Id of this file.
 *  ============================================================================
 */
#define FILEID  FID_HAL_C


/*  ============================================================================
 *  Create named sections for the functions to allow specific memory placement.
 *  ============================================================================
 */
#pragma CODE_SECTION (HAL_init, ".text:DSPLINK_init")


/** ============================================================================
 *  @func   HAL_init
 *
 *  @desc   Initialization function of HAL module. It initalizes EMIF and
 *          interrupt hardware.
 *
 *  @modif  None
 *  ============================================================================
 */
Void HAL_init()
{
    EMIF_init();
    INT_init();
}


#ifdef __cplusplus
}
#endif /* extern "C" */

