/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef __CSLR_INTC2_H_
#define __CSLR_INTC2_H_

/***************5910PG1_0***************/
#ifdef  CSL_CHIP_5910PG1_0

#include <cslr.h>
#include <_cslr_intc2_007.h>

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/

#ifndef CSL_INTC2_CNT
#define CSL_INTC2_CNT                  1
#endif
/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_Intc2Regs  * CSL_Intc2RegsOvly;
/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_INTC2_0_REGS               ((CSL_Intc2RegsOvly) 0x00004800u)

inline CSL_Intc2RegsOvly _CSL_intc2GetBaseAddr(Uint16 intc2Num)
{
    return CSL_INTC2_0_REGS;
}

#else
#error "CHIP symbol not defined"

#endif /* CSL_CHIP_5910PG1_0 */

#endif  /* CSLR_INTC2_H_ */

