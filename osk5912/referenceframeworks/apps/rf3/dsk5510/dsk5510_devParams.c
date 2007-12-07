/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== dsk5510_devParams.c ========
 *  DSK5510_DMA_AIC23 default driver parameters
 */
#include <std.h>
#include <dsk5510_dma_aic23.h>
#include <aic23.h>

/*
 *  ======== DSK5510_DEVPARAMS ========
 *  This static initialization defines the default parameters used for
 *  DSK5510_DMA_AIC23 IOM driver
 */
DSK5510_DMA_AIC23_DevParams DSK5510_DEVPARAMS = {
    DSK5510_DMA_AIC23_VERSION_1,  /* Drivers version used by app */     \
    DSK5510_DMA_AIC23_RXDMAID_DEFAULT,  /* Uses DMA channel 4 */        \
    DSK5510_DMA_AIC23_TXDMAID_DEFAULT,  /* Uses DMA channel 5 */        \
    DSK5510_DMA_AIC23_PORTTYPE_DEFAULT, /* Uses DARAM port type */      \
    AIC23_REG0_DEFAULT,                                                 \
    AIC23_REG1_DEFAULT,                                                 \
    AIC23_REG2_DEFAULT,                                                 \
    AIC23_REG3_DEFAULT,                                                 \
    AIC23_REG4_DEFAULT,                                                 \
    AIC23_REG5_DEFAULT,                                                 \
    AIC23_REG6_DEFAULT,                                                 \
    AIC23_REG7_DEFAULT,                                                 \

    AIC23_9BITWORD(              /* REG 8: sample rate */                    
        /* X     */   0,         /* reserved */                          
        /* CLKOUT*/   0,         /* clock output divider: 2 (MCLK/2)*/  
        /* CLKIN */   0,         /* clock input  divider: 2 (MCLK/2)*/  

        /* SR,BO */   1,0,0,0,1, /* sampling rate: ADC/DAC 44.1 kHz */   
        //            0,1,1,1,0, /* sampling rate: ADC/DAC 96   kHz */   
        //            0,0,0,0,0, /* sampling rate: ADC/DAC 48   kHz */   
        //            0,1,1,0,0, /* sampling rate: ADC/DAC 32   kHz */   
        //            0,0,1,1,0, /* sampling rate: ADC/DAC  8   kHz */   

        /* USB/N */   1          /* clock mode select (USB/normal): USB */ 
    ),                                                                  \
    
    AIC23_REG9_DEFAULT,                                                 \
    {                            /* Receiver interrupt masks = 1 */     \
    DSK5510_DMA_AIC23_IER_MASK_DEFAULT,                                 \
    DSK5510_DMA_AIC23_IER_MASK_DEFAULT                                  \
    },                                                                  \
    {                            /* Transmitter interrupt masks = 1 */  \
    DSK5510_DMA_AIC23_IER_MASK_DEFAULT,                                 \
    DSK5510_DMA_AIC23_IER_MASK_DEFAULT                                  \
    }                                                                   \
};


