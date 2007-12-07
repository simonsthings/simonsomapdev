/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== dsk5402_dma_ad50.h ========
 */

#ifndef DSK5402_DMA_AD50_
#define DSK5402_DMA_AD50_

#include <iom.h>

#include <ad50.h>

/* Version number definition */
#define DSK5402_DMA_AD50_VERSION_1 0xAB01

#define DSK5402_DMA_AD50_RXDMAID_DEFAULT 4
#define DSK5402_DMA_AD50_TXDMAID_DEFAULT 5
#define DSK5402_DMA_AD50_INTR_MASK_DEFAULT 1
/* Driver function table to be used by applications. */
extern IOM_Fxns DSK5402_DMA_AD50_FXNS;

/* Setup device parameter structure for the driver */
typedef struct DSK5402_DMA_AD50_DevParams {
    Int         versionId;      /* version number to be set by application */
    Uns         rxDmaId;        /* rxDmaId */
    Uns         txDmaId;        /* txDmaId */
    AD50_Params ad50;           /* codec parameters (registers) */
    Uns         rxIntrMask;     /* receiver interrupt mask */
    Uns         txIntrMask;     /* transmitter interruptmask */
} DSK5402_DMA_AD50_DevParams;

#define DSK5402_DMA_AD50_DEVPARAMS_DEFAULT {    \
    DSK5402_DMA_AD50_VERSION_1,                 \
    DSK5402_DMA_AD50_RXDMAID_DEFAULT,           \
    DSK5402_DMA_AD50_TXDMAID_DEFAULT,           \
    AD50_DEFAULTPARAMS,                         \
    DSK5402_DMA_AD50_INTR_MASK_DEFAULT,         \
    DSK5402_DMA_AD50_INTR_MASK_DEFAULT          \
}

/* Mini-driver init function -- initializes driver variables, if any */
extern Void DSK5402_DMA_AD50_init( Void );

#endif

