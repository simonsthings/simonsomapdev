/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSLR_DMA_H_
#define _CSLR_DMA_H_

/***************5910PG1_0***************/
#ifdef  CSL_CHIP_5910PG1_0

#include <cslr.h>
#include <cslr_dma_001.h>

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_DMA_CNT                   1
/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_DmaRegs             * CSL_DmaRegsOvly;
typedef volatile ioport CSL_DmaGlobalRegs   * CSL_DmaGlobalRegsOvly;
typedef volatile ioport CSL_DmaChaRegs          * CSL_DmaChaRegsOvly;

/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_DMA_0_REGS                ((CSL_DmaRegsOvly) 0x00000C00u)

inline CSL_DmaRegsOvly _CSL_dmaGetBaseAddr(Uint16 dmaNum)
{
    CSL_DmaRegsOvly lookup[] = { CSL_DMA_0_REGS };
    return lookup[dmaNum];
}

#else
#error "CHIP symbol not defined"

#endif /* CSL_CHIP_5910PG1_0 */

#endif  /* CSLR_DMA_H_ */
