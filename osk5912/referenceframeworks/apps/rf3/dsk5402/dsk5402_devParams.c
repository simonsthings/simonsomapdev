/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== dsk5402_devparams.c ========
 *  DSK5402_DMA_AD50 default driver parameters
 */
#include <std.h>

#include <dsk5402_dma_ad50.h>

/*
 *  ======== DSK5402_DEVPARAMS ========
 *  This static initialization defines the default parameters used for
 *  DSK5402_DMA_AD50 IOM driver
 */
DSK5402_DMA_AD50_DevParams DSK5402_DEVPARAMS = {
    DSK5402_DMA_AD50_VERSION_1,        /* Drivers version uses by application */
    DSK5402_DMA_AD50_RXDMAID_DEFAULT,  /* Uses DMA channel 4 */
    DSK5402_DMA_AD50_TXDMAID_DEFAULT,  /* Uses DMA channel 5 */
    0x80,                       /* Ctrl Register 1 on AD50 device */
    0x00,                       /* Ctrl Register 2 on AD50 device */
    0x00,                       /* Ctrl Register 3 on AD50 device */
    0xa0,                       /* Ctrl Register 4 on AD50 device sample rate */
    DSK5402_DMA_AD50_INTR_MASK_DEFAULT, /* Receiver interrupt mask =  1 */ 
    DSK5402_DMA_AD50_INTR_MASK_DEFAULT, /* Transmitter interrupt mask = 1 */
};


