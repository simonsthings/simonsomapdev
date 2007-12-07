/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== c55xx_dma_mcbsp_cslv3.h ========
 *
 *  This file declares an implementation of the IOM interface for
 *  performing streaming I/O with audio codec's on the TI
 *  C55xx DSP's using the DMA with the MCBSP connected to the codec. 
 *
 *  This driver is based on Chip Support Library (CSL) v3.0
 */

#ifndef C55XX_DMA_MCBSP_CSLV3_
#define C55XX_DMA_MCBSP_CSLV3_

#include <iom.h>

#include <stdplus.h>

#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>

/* Version Number definition */
#define C55XX_DMA_MCBSP_CSLV3_VERSION_2 0xAB02


/* Driver function table to be used by applications. */
extern IOM_Fxns C55XX_DMA_MCBSP_CSLV3_FXNS;

typedef struct C55XX_DMA_MCBSP_CSLV3_DevParams {
    Int               versionId;   /* version number to be set by application */
    Uns               rxDmaId;     /* rxDmaId */
    Uns               txDmaId;     /* txDmaId */
    CSL_McbspObj      *mcbspObj;   /* mcbsp config handle */    
    CSL_McbspHwSetup  *mcbspCfg;   /* mcbsp config handle */    
    Uns               rxIerMask[2]; /* receiver interrupt mask */
    Uns               txIerMask[2]; /* transmitter interrupt mask */
} C55XX_DMA_MCBSP_CSLV3_DevParams;

typedef struct C55XX_DMA_MCBSP_CSLV3_ChanParams {
    CSL_DmaObj                  *dmaObj;
    CSL_DmaHwSetup              *dmaCfg;    
} C55XX_DMA_MCBSP_CSLV3_ChanParams;

/* Mini-driver init function -- initializes driver variables, if any */
extern Void C55XX_DMA_MCBSP_CSLV3_init( Void );

#endif

