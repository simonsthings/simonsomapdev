/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/* 
 *  ======== dsk5416_dma_pcm3002.h ========
 * 
 *  This file declares an implementation of the IOM interface for
 *  performaing streaming I/O with the TI PCM3002 audio codec on the TI
 *  5416 DSP Starter Kit (DSK) using DMA and McBSP.  
 */

#ifndef DSK5416_DMA_PCM3002_
#define DSK5416_DMA_PCM3002_

#include <iom.h>

#include <dsk5416_pcm3002.h>

/* Version Number definition */
#define DSK5416_DMA_PCM3002_VERSION_1 0xAB01

#define DSK5416_DMA_PCM3002_RXDMAID_DEFAULT     4
#define DSK5416_DMA_PCM3002_TXDMAID_DEFAULT     5

#define DSK5416_DMA_PCM3002_INTR_MASK_DEFAULT 1

/* Driver function table to be used by applications. */
extern IOM_Fxns DSK5416_DMA_PCM3002_FXNS;

/* Setup device parameter structure for the driver */
typedef struct DSK5416_DMA_PCM3002_DevParams {
    Int                    versionId;  /* Version number to be set by app */
    Uns                    rxDmaId;
    Uns                    txDmaId;
    DSK5416_PCM3002_Config pcm3002;     /* codec parameters (registers) */
    Uns                    rxIntrMask;
    Uns                    txIntrMask;
} DSK5416_DMA_PCM3002_DevParams;

#define DSK5416_DMA_PCM3002_DEVPARAMS_DEFAULT { \
    DSK5416_DMA_PCM3002_VERSION_1,              \
    DSK5416_DMA_PCM3002_RXDMAID_DEFAULT,        \
    DSK5416_DMA_PCM3002_TXDMAID_DEFAULT,        \
    PCM3002_DEFAULTCONFIG,                      \
    DSK5416_DMA_PCM3002_INTR_MASK_DEFAULT,      \
    DSK5416_DMA_PCM3002_INTR_MASK_DEFAULT       \
}

/* Mini-driver init function -- initializes driver variables, if any */
extern Void DSK5416_DMA_PCM3002_init( Void );

#endif

