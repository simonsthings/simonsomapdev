/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== dsk6x11_devParams.c ========
 *  DSK6X11_EDMA_AD535 default driver parameters
 */
#include <std.h>
#include <dsk6x11_edma_ad535.h>

/*
 *  ======== DSK6X11_DEVPARAMS ========
 *  This static initialization defines the default parameters used for
 *  DSK6X11_EDMA_AD535 IOM driver
 */
DSK6X11_EDMA_AD535_DevParams DSK6X11_DEVPARAMS = {
    DSK6X11_EDMA_AD535_VERSION_1, /* Drivers version used by app */     \

    FALSE,                        /* Rx/TX buffer on the receiving */   \
                                  /* transmitting end of the EDMA  */
                                  /* FALSE -> buffers in onchip    */
                                  /* memory. No CACHE coherency    */
                                  /* calls                         */
                                  /* TRUE -> buffers in offchip    */
                                  /* memory.  Coherency calls made */
                                  /* in driver                     */

    DSK6X11_EDMA_AD535_EDMA_ID_DEFAULT, /* Default IRQ # for EDMA is 8 */

    /* Expand the following literal to change individual reg values */ 
    DSK6X11_AD535_DEFAULTPARAMS,

    DSK6X11_EDMA_AD535_INTR_MASK_DEFAULT, /* Interrupt mask = 1 */
    EDMA_OPT_PRI_HIGH                     /* EDMA transfers priority */
};


