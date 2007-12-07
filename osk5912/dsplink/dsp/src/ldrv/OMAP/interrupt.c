/** ============================================================================
 *  @file   interrupt.c
 *
 *  @path   $(DSPLINK)\dsp\src\ldrv\OMAP
 *
 *  @desc   Implementation of helper functions of Hardware Abstraction Layer.
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
#include <c55.h>
#include <hwi.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
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
#define FILEID  FID_INTERRUPT_C

/** ============================================================================
 *  @macro  REG16
 *
 *  @desc   Register access for 16 bit word.
 *  ============================================================================
 */
#define REG16(A)    (*(volatile ioport unsigned short*)(A))

/** ============================================================================
 *  @macro  Interrupt related defines.
 *
 *  @desc   Interrupt handler related defines.
 *  ============================================================================
 */
#define SHM_INTERRUPT_MASK      0x0020   /* host interrupt bit position */
#define MB_INTERRUPT_ID         5

#define MB_DSP2ARM_FLAG_MASK    0x0001

#define MB_BASE_IO_ADDR         0xF800
#define MB_ARM2DSP1_OFFSET      0x00
#define MB_ARM2DSP1B_OFFSET     0x02
#define MB_DSP2ARM1B_OFFSET     0x06
#define MB_DSP2ARM1_FLAG_OFFSET 0x0E

#define MB_ARM2DSP1_ADDR        (MB_BASE_IO_ADDR + MB_ARM2DSP1_OFFSET)
#define MB_ARM2DSP1B_ADDR       (MB_BASE_IO_ADDR + MB_ARM2DSP1B_OFFSET)
#define MB_DSP2ARM1B_ADDR       (MB_BASE_IO_ADDR + MB_DSP2ARM1B_OFFSET)
#define MB_DSP2ARM1_FLAG_ADDR   (MB_BASE_IO_ADDR + MB_DSP2ARM1_FLAG_OFFSET)

#define OMAP_MAILBOX_SIGNATURE  0x4C4E


/** ============================================================================
 *  @name   isrFxn
 *
 *  @desc   ISR function pointer.
 *  ============================================================================
 */
static void (*isrFxn)(Ptr);

/*  ============================================================================
 *  Create named sections for the functions to allow specific memory placement.
 *  ============================================================================
 */
#pragma CODE_SECTION (INT_init,           ".text:DSPLINK_init")
#pragma CODE_SECTION (INT_registerGPPISR, ".text:DSPLINK_init")


/** ============================================================================
 *  @func   INT_init
 *
 *  @desc   Initializes the interrupt handler hardware.
 *
 *  @modif  None
 *  ============================================================================
 */
Void INT_init()
{
    /* Initialize global variable */
    isrFxn = NULL;
}


/** ============================================================================
 *  @func   INT_disableGPPInt
 *
 *  @desc   Disables GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
Uns INT_disableGPPInt()
{
    return C55_disableIMR0(SHM_INTERRUPT_MASK);
}


/** ============================================================================
 *  @func   INT_enableGPPInt
 *
 *  @desc   Enables GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
Void INT_enableGPPInt(Uns oldMask)
{
    C55_enableIMR0(oldMask);
}


/** ============================================================================
 *  @func   INT_registerGPPISR
 *
 *  @desc   Register ISR for GPP interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
Void INT_registerGPPISR(Ptr func, Ptr arg)
{
    volatile Uns mbxVal;
    HWI_Attrs hwiAttrs;
    Uns       hold ;

    hold = HWI_disable() ;

    hwiAttrs.arg = arg;
    hwiAttrs.ier0mask = 0x20;
    hwiAttrs.ier1mask = 0;

    isrFxn = (Void (*)(Ptr)) func;
    mbxVal = REG16 (MB_ARM2DSP1B_ADDR); /* Clear interrupt */
    HWI_dispatchPlug(MB_INTERRUPT_ID, (Fxn)INT_isrStub, &hwiAttrs);
    HWI_restore (hold) ;

    /* enable interrupt from host */
    C55_enableIMR0(SHM_INTERRUPT_MASK);
}


/** ============================================================================
 *  @func   INT_sendGPPInt
 *
 *  @desc   Send interrupt to GPP.
 *
 *  @modif  None
 *  ============================================================================
 */
Void INT_sendInt(Ptr arg)
{
    for(;;) {
        if ((REG16(MB_DSP2ARM1_FLAG_ADDR) & MB_DSP2ARM_FLAG_MASK ) == 0x0000) {
            REG16 (MB_DSP2ARM1B_ADDR) = OMAP_MAILBOX_SIGNATURE;
            break;
        }
    }  /* end for(;;) */
}


/** ============================================================================
 *  @func   INT_isrStub
 *
 *  @desc   Interrupt service routine stub.
 *
 *  @modif  None
 *  ============================================================================
 */
Void INT_isrStub(Ptr arg)
{
    volatile Uns mbxVal;

    /* read mailbox to clear interrupt signal */
    mbxVal = REG16 (MB_ARM2DSP1B_ADDR);

    if (isrFxn != NULL ) {
        (*isrFxn)(arg);
    }
}


#ifdef __cplusplus
}
#endif /* extern "C" */
