/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#ifndef _CSLR_DMA_H_
#define _CSLR_DMA_H_

#include <cslr.h>
#include <cslr_dma_001.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_DmaRegs                 * CSL_DmaRegsOvly;
typedef volatile ioport CSL_DmaGlobalRegs   * CSL_DmaGlobalRegsOvly;
typedef volatile ioport CSL_DmaChaRegs          * CSL_DmaChaRegsOvly;

inline CSL_DmaRegsOvly _CSL_dmaGetBaseAddr(Uint16 dmaNum)
  {
    return (CSL_DmaRegsOvly)_CSL_dmalookup[dmaNum];
  }

#endif  /* CSLR_DMA_H_ */
