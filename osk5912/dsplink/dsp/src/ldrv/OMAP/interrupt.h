/** ============================================================================
 *  @file   interrupt.h
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   This file declares supporting functions of  HAL (Hardware
 *          Abstraction Layer) for OMAP1510 device.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef INTERRUPT_
#define INTERRUPT_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @func   INT_init
 *
 *  @desc   Initializes the interrupt handler hardware.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    EMIF_init
 *  ============================================================================
 */
Void  INT_init();

/** ============================================================================
 *  @func   INT_disableGPPInt
 *
 *  @desc   Disbles GPP interrupt.
 *
 *  @arg    None
 *
 *  @ret    <0-maxIntValue>
 *              Key to enable GPP interrupt again.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    INT_init
 *  ============================================================================
 */
Uns INT_disableGPPInt();

/** ============================================================================
 *  @func   INT_enableGPPInt
 *
 *  @desc   Enables GPP interrupt.
 *
 *  @arg    key
 *              Key to enable GPP interrupt.
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
Void INT_enableGPPInt(Uns key);

/** ============================================================================
 *  @func   INT_registerGPPISR
 *
 *  @desc   Register ISR for GPP interrupt.
 *
 *  @arg    func
 *              Function to register.
 *          arg
 *              Argument to function.
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
Void INT_registerGPPISR(Ptr func, Ptr arg);

/** ============================================================================
 *  @func   INT_sendInt
 *
 *  @desc   Send interrupt to GPP.
 *
 *  @arg    arg
 *              Optional argument to for sending interrupt.
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
Void INT_sendInt(Ptr arg);

/** ============================================================================
 *  @func   INT_isrStub
 *
 *  @desc   ISR function for Mailbox interrupt.
 *
 *  @arg    arg
 *              Argument for ISR function.
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
Void INT_isrStub(Ptr arg);


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* INTERRUPT_ */

