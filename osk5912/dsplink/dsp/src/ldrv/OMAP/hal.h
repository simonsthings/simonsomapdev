/** ============================================================================
 *  @file   hal.h
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   Hardware Abstraction Layer interface definition.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef HAL_
#define HAL_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <emif.h>
#include <interrupt.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @macro  HAL_memReadUns
 *
 *  @desc   Reads 16 bit unsigned integer.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_memReadUns(addr) \
            EMIF_readUns(addr)

/** ============================================================================
 *  @macro  HAL_memWriteUns
 *
 *  @desc   Writes 16 bit unsigned integer.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_memWriteUns(addr,value) \
                        EMIF_writeUns(addr, value)

/** ============================================================================
 *  @macro  HAL_memRead
 *
 *  @desc   Read from the memory to given buffer.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_memRead(fromBuf,toBuffer,maus) \
                        EMIF_read(toBuffer, fromBuf, maus)

/** ============================================================================
 *  @macro  HAL_memWrite
 *
 *  @desc   Writes to the memory from given buffer.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_memWrite(fromBuf,toBuffer,maus) \
                    EMIF_write(toBuffer, fromBuf, maus)

/** ============================================================================
 *  @macro  HAL_disableGPPInt
 *
 *  @desc   Disables the GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_disableGPPInt() INT_disableGPPInt()

/** ============================================================================
 *  @macro  HAL_enableGPPInt
 *
 *  @desc   Enables the GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_enableGPPInt(key)  INT_enableGPPInt(key)

/** ============================================================================
 *  @macro  HAL_registerGPPISR
 *
 *  @desc   Registers ISR for GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_registerGPPISR(func,arg) INT_registerGPPISR(func,arg)

/** ============================================================================
 *  @macro  HAL_sendInt
 *
 *  @desc   Interrupts the GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
#define HAL_sendInt(arg) INT_sendInt(arg)


/** ============================================================================
 *  @func   HAL_init
 *
 *  @desc   Initialization function of HAL module.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Void HAL_init();


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* HAL_ */

