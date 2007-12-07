/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/* 
 *  ======== osk5912_dma_aic23_cslv3.h ========
 * 
 *  This driver uses Chip Support Library (CSL) v3.x
 *
 *  This file declares an implementation of the IOM interface for
 *  performing streaming I/O with the TI AIC23 audio codec on the 
 *  OSK 5912 containing an AIC23 codec, using the DMA for block transfer.
 */

#ifndef OSK5912_DMA_AIC23_CSLV3_
#define OSK5912_DMA_AIC23_CSLV3_

#include <iom.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Version number definition */
#define OSK5912_DMA_AIC23_CSLV3_VERSION_2  0xAB02

#define OSK5912_DMA_AIC23_CSLV3_RXDMAID_DEFAULT 4
#define OSK5912_DMA_AIC23_CSLV3_TXDMAID_DEFAULT 5

#define OSK5912_DMA_AIC23_CSLV3_PORTTYPE_DARAM 0
#define OSK5912_DMA_AIC23_CSLV3_PORTTYPE_SARAM 1
#define OSK5912_DMA_AIC23_CSLV3_PORTTYPE_EMIF  2
#define OSK5912_DMA_AIC23_CSLV3_PORTTYPE_DEFAULT OSK5912_DMA_AIC23_CSLV3_PORTTYPE_DARAM

#define OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT 1

/* Driver function table to be used by applications */
extern IOM_Fxns OSK5912_DMA_AIC23_CSLV3_FXNS;
                
/* Setup structure for the driver (contains only codec registers) */
typedef struct OSK5912_DMA_AIC23_CSLV3_DevParams {
    Int                  versionId;        /* version number */
    Uns                  rxDmaId;          /* receive channel */
    Uns                  txDmaId;          /* transmit channel */
    Uns                  dmaPortType;      /* DMA port type */
    Uns                  rxIerMask[2];     /* receiver interrupt mask */
    Uns                  txIerMask[2];     /* transmitter interrupt mask */
} OSK5912_DMA_AIC23_CSLV3_DevParams;

#define OSK5912_DMA_AIC23_CSLV3_DEVPARAMS_DEFAULT {     \
    OSK5912_DMA_AIC23_CSLV3_VERSION_2,                  \
    OSK5912_DMA_AIC23_CSLV3_RXDMAID_DEFAULT,            \
    OSK5912_DMA_AIC23_CSLV3_TXDMAID_DEFAULT,            \
    OSK5912_DMA_AIC23_CSLV3_PORTTYPE_DEFAULT,           \
    {                                                   \
        OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT,       \
        OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT        \
    },                                                  \
    {                                                   \
        OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT,       \
        OSK5912_DMA_AIC23_CSLV3_IER_MASK_DEFAULT        \
    }                                                   \
}

/* Controller init function -- initializes driver variables, if any */
extern Void OSK5912_DMA_AIC23_CSLV3_init( Void );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* _OSK5912_DMA_AIC23_CSLV3_ */




