/** ============================================================================
 *  @file   emif.c
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   Implementation of supporting functions of HAL (Hardware
 *          Abstraction Layer) for EMIF.
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

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <emif.h>
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
#define FILEID  FID_EMIF_C


/*  ============================================================================
 *  Create named sections for the functions to allow specific memory placement.
 *  ============================================================================
 */
#pragma CODE_SECTION (EMIF_init, ".text:DSPLINK_init")


/** ============================================================================
 *  @func   EMIF_init
 *
 *  @desc   Initializes the EMIF hardware.
 *
 *  @modif  None
 *  ============================================================================
 */
Void  EMIF_init()
{
    /* Nothing to be done for EMIF initialization */
}


#ifdef __cplusplus
}
#endif /* extern "C" */
