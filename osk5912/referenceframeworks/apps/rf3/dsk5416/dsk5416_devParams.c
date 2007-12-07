/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== dsk5416_devParams.c ========
 *  DSK5416_DMA_PCM3002 default driver parameters
 */
#include <std.h>
#include <dsk5416_dma_pcm3002.h>

/*
 *  ======== DSK5416_DEVPARAMS ========
 *  This static initialization defines the default parameters used for
 *  DSK5416_DMA_PCM3002 IOM driver
 */
DSK5416_DMA_PCM3002_DevParams DSK5416_DEVPARAMS = {
    DSK5416_DMA_PCM3002_VERSION_1, /* Drivers version uses by application */
    DSK5416_DMA_PCM3002_RXDMAID_DEFAULT,  /* Uses DMA channel 4 */
    DSK5416_DMA_PCM3002_TXDMAID_DEFAULT,  /* Uses DMA channel 5 */
    0x1FF,                      /* Control Register 0 on PCM3002 device */
    0x1FF,                      /* Control Register 1 on PCM3002 device */
    0x0,                        /* Control Register 2 on PCM3002 device */
    0x0,                        /* Control Register 3 on PCM3002 device */
    DSK5416_DMA_PCM3002_INTR_MASK_DEFAULT, /* Receiver interrupt mask =  1 */
    DSK5416_DMA_PCM3002_INTR_MASK_DEFAULT, /* Transmitter interrupt mask =  1 */
};


