/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef _CSLR_CHIP_H_
#define _CSLR_CHIP_H_

#include <cslr.h>
#include <cslr_chip_002.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_ChipRegs  * CSL_ChipRegsOvly;


inline CSL_ChipRegsOvly _CSL_chipGetBaseAddr(Uint16 chipNum)
{
    return CSL_CHIP_REGS;
}

#endif  /* CSLR_CHIP_H_ */

