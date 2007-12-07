/** ============================================================================
 *  @file   emif.h
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   This file declares supporting functions of  HAL (Hardware
 *          Abstraction Layer) for OMAP1510 device EMIF.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef EMIF_
#define EMIF_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @macro  EMIF_readUns
 *
 *  @desc   Reads 16 bit unsigned value from address.
 *  ============================================================================
 */
#define EMIF_readUns(addr) (*(volatile Uns *)addr)

/** ============================================================================
 *  @macro  EMIF_writeUns
 *
 *  @desc   Writes 16 bit unsigned value to given address.
 *  ============================================================================
 */
#define EMIF_writeUns(addr, value) ((*(volatile Uns *)addr) = value)

/** ============================================================================
 *  @macro  EMIF_read
 *
 *  @desc   Reads data from "src" to "dest". "num" indicates the number of MAUs
 *          to be copied.
 *  ============================================================================
 */
#define EMIF_read(dest, src, num) memcpy(dest, src, num)

/** ============================================================================
 *  @macro  EMIF_write
 *
 *  @desc   Writes data from "src" to "dest". "num" indicates the number of MAUs
 *          to be copied.
 *  ============================================================================
 */
#define EMIF_write(dest, src, num) memcpy(dest, src, num)

/** ============================================================================
 *  @func   EMIF_init
 *
 *  @desc   Initializes the EMIF hardware.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    INT_init
 *  ============================================================================
 */
Void  EMIF_init();


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* EMIF_ */

