/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== c54xx_dma_mcbsp.h ========
 *
 *  This file declares an implementation of the LIO interface for
 *  performing streaming I/O with the TI AD50 audio codec on the TI
 *  5402 DSP Starter Kit (DSK) using McBSP for sample-by-sample on the
 *  5402 device. The codec is configured for 8khz sample rate, 16 bit
 *  data.
 */

#ifndef C54XX_DMA_MCBSP_
#define C54XX_DMA_MCBSP_

#include <iom.h>

#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>

/* Version number definition */
#define C54XX_DMA_MCBSP_VERSION_1  0xAB01
#define NUMCHANS   2    /* IOM_INPUT and IOM_OUTPUT */
#define INPUT      0
#define OUTPUT     1
/* Driver function table to be used by applications. */
extern IOM_Fxns C54XX_DMA_MCBSP_FXNS;

typedef struct C54XX_DMA_MCBSP_DevParams {
    Int                 versionId;
    Uns                 rxDmaId;
    Uns                 txDmaId;
    MCBSP_Config        *mcbspCfg;
    Uns                 rxIntrMask;
    Uns                 txIntrMask;
} C54XX_DMA_MCBSP_DevParams;

typedef struct C54XX_DMA_MCBSP_ChanParams {
    DMA_Config          *dmaCfg;
} C54XX_DMA_MCBSP_ChanParams;

/* Mini-driver init function -- initializes driver variables, if any */
extern Void C54XX_DMA_MCBSP_init( Void );

#endif

