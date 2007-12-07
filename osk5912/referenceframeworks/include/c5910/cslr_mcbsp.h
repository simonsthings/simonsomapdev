/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSLR_MCBSP_H_
#define _CSLR_MCBSP_H_

/***************5910PG1_0***************/
#ifdef  CSL_CHIP_5910PG1_0

#include <cslr.h>
#include <cslr_mcbsp_001.h>

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_MCBSP_CNT                 2
/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_McbspRegs  * CSL_McbspRegsOvly;
/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_MCBSP_0_REGS              ((CSL_McbspRegsOvly) 0x00008C00u)
#define CSL_MCBSP_1_REGS              ((CSL_McbspRegsOvly) 0x0000B800u)

inline CSL_McbspRegsOvly _CSL_mcbspGetBaseAddr(Uint16 mcbspNum)
{
    CSL_McbspRegsOvly lookup[] = { CSL_MCBSP_0_REGS, CSL_MCBSP_1_REGS };
    return lookup[mcbspNum];
}

#else
#error "CHIP symbol not defined"

#endif /* CSL_CHIP_5910PG1_0 */

#endif  /* CSLR_MCBSP_H_ */
