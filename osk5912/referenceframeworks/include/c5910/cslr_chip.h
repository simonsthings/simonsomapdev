/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSLR_CHIP_H_
#define _CSLR_CHIP_H_

/***************5910PG1_0***************/
#ifdef  CSL_CHIP_5910PG1_0

#include <cslr.h>
#include <cslr_chip_002.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile CSL_ChipRegs  * CSL_ChipRegsOvly;
/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_CHIP_REGS                 ((CSL_ChipRegsOvly) 0x00000000u)

inline CSL_ChipRegsOvly _CSL_chipGetBaseAddr(Uint16 chipNum)
{
    return CSL_CHIP_REGS;
}

#else
#error "CHIP symbol not defined"

#endif /* CSL_CHIP_5910PG1_0 */

#endif  /* CSLR_CHIP_H_ */

