/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== dsk6713_devParams.c ========
 *  Default codec driver parameters
 */
#include <std.h>
#include <dsk6713_edma_aic23.h>
#include <csl_edma.h>
#include <aic23.h>

/*
 *  ======== DSK6713_DEVPARAMS ========
 *  This static initialization defines the default parameters used for
 *  DSK6713_EDMA_AIC23 IOM driver
 */
DSK6713_EDMA_AIC23_DevParams DSK6713_DEVPARAMS = {
    DSK6713_EDMA_AIC23_VERSION_1, /* Drivers version used by app */     \

    FALSE,                        /* Rx/TX buffer on the receiving */   \
                                  /* transmitting end of the EDMA  */
                                  /* FALSE -> buffers in onchip    */
                                  /* memory. No CACHE coherency    */
                                  /* calls                         */ 
                                  /* TRUE -> buffers in offchip    */
                                  /* memory.  Coherency calls made */
                                  /* in driver                     */
    DSK6713_EDMA_AIC23_IRQID_DEFAULT, /* Default IRQ # for EDMA is 8 */ \

    AIC23_REG0_DEFAULT,                                                 \
    AIC23_REG1_DEFAULT,                                                 \
    AIC23_REG2_DEFAULT,                                                 \
    AIC23_REG3_DEFAULT,                                                 \
    AIC23_REG4_DEFAULT,                                                 \
    AIC23_REG5_DEFAULT,                                                 \
    AIC23_REG6_DEFAULT,                                                 \
    AIC23_REG7_DEFAULT,                                                 \

    AIC23_9BITWORD(              /* REG 8: sample rate */
        /* X      */  0,         /* reserved */
        /* CLKOUT */  0,         /* clock output divider: 2 (MCLK/2) */
        /* CLKIN  */  0,         /* clock input  divider: 2 (MCLK/2) */

        /* SR,BO  */  1,0,0,0,1, /* sampling rate: ADC/DAC 44.1 kHz */
        //            0,1,1,1,0, /* sampling rate: ADC/DAC 96   kHz */
        //            0,0,0,0,0, /* sampling rate: ADC/DAC 48   kHz */
        //            0,1,1,0,0, /* sampling rate: ADC/DAC 32   kHz */
        //            0,0,1,1,0, /* sampling rate: ADC/DAC  8   kHz */

        /* USB/N  */  1          /* clock mode select (USB/normal): USB */
    ),                                                                  \
    
    AIC23_REG9_DEFAULT,                                                 \
    DSK6713_EDMA_AIC23_INTR_MASK_DEFAULT, /* Interrupt mask = 1 */      \
    EDMA_OPT_PRI_HIGH                     /* EDMA transfers priority */ \
};


