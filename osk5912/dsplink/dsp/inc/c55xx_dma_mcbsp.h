/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== c55xx_dma_mcbsp.h ========
 *
 *  This file declares an implementation of the IOM interface for
 *  performing streaming I/O with audio codec's on the TI
 *  C55xx DSP's using the DMA with the MCBSP connected to the codec. 
 */

#ifndef C55XX_DMA_MCBSP_
#define C55XX_DMA_MCBSP_

#include <iom.h>

#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>

/* Version Number definition */
#define C55XX_DMA_MCBSP_VERSION_1 0xAB01



/* Driver function table to be used by applications. */
extern IOM_Fxns C55XX_DMA_MCBSP_FXNS;

typedef struct C55XX_DMA_MCBSP_DevParams {
    Int           versionId; /* version number to be set by application */
    Uns           rxDmaId; /* rxDmaId */
    Uns           txDmaId; /* txDmaId */
    MCBSP_Config  *mcbspCfg; /* mcbsp config handle */
    Uns           rxIerMask[2]; /* receiver interrupt mask */
    Uns           txIerMask[2]; /* transmitter interrupt mask */
} C55XX_DMA_MCBSP_DevParams;

typedef struct C55XX_DMA_MCBSP_ChanParams {
    DMA_Config          *dmaCfg;
} C55XX_DMA_MCBSP_ChanParams;

/* Mini-driver init function -- initializes driver variables, if any */
extern Void C55XX_DMA_MCBSP_init( Void );

#endif

