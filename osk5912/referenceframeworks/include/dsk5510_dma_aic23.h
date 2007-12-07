/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/* 
 *  ======== dsk5510_dma_aic23.h ========
 * 
 *  This file declares an implementation of the IOM interface for
 *  performing streaming I/O with the TI AIC23 audio codec on the 
 *  5510 DSK containing an AIC23 codec, using the DMA for block transfer.
 */

#ifndef DSK5510_DMA_AIC23_
#define DSk5510_DMA_AIC23_

#include <iom.h>

#include <aic23.h>

/* Version number definition */
#define DSK5510_DMA_AIC23_VERSION_1  0xAB01

#define DSK5510_DMA_AIC23_RXDMAID_DEFAULT       4
#define DSK5510_DMA_AIC23_TXDMAID_DEFAULT       5

#define DSK5510_DMA_AIC23_PORTTYPE_DARAM 0
#define DSK5510_DMA_AIC23_PORTTYPE_SARAM 1
#define DSK5510_DMA_AIC23_PORTTYPE_EMIF  2
#define DSK5510_DMA_AIC23_PORTTYPE_DEFAULT DSK5510_DMA_AIC23_PORTTYPE_DARAM

#define DSK5510_DMA_AIC23_IER_MASK_DEFAULT 1
/* Driver function table to be used by applications */
extern IOM_Fxns DSK5510_DMA_AIC23_FXNS;
                
/* Setup structure for the driver (contains only codec registers) */
typedef struct DSK5510_DMA_AIC23_DevParams {
    Int                  versionId;        /* version number */
    Uns                  rxDmaId;          /* receive channel */
    Uns                  txDmaId;          /* transmit channel */
    Uns                  dmaPortType;      /* DMA port type */
    AIC23_Params         aic23;            /* codec parameters (registers) */
    Uns                  rxIerMask[2];     /* receiver interrupt mask */
    Uns                  txIerMask[2];     /* transmitter interrupt mask */
} DSK5510_DMA_AIC23_DevParams;

#define DSK5510_DMA_AIC23_DEVPARAMS_DEFAULT {   \
    DSK5510_DMA_AIC23_VERSION_1,                \
    DSK5510_DMA_AIC23_RXDMAID_DEFAULT,          \
    DSK5510_DMA_AIC23_TXDMAID_DEFAULT,          \
    DSK5510_DMA_AIC23_PORTTYPE_DEFAULT,         \
    AIC23_DEFAULTPARAMS,                        \
    {                                           \
        DSK5510_DMA_AIC23_IER_MASK_DEFAULT,     \
        DSK5510_DMA_AIC23_IER_MASK_DEFAULT      \
    },                                          \
    {                                           \
        DSK5510_DMA_AIC23_IER_MASK_DEFAULT,     \
        DSK5510_DMA_AIC23_IER_MASK_DEFAULT      \
    }                                           \
}

/* Controller init function -- initializes driver variables, if any */
extern Void DSK5510_DMA_AIC23_init( Void );

#endif



