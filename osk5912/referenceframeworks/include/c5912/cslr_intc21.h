/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef __CSLR_INTC21_H_
#define __CSLR_INTC21_H_

#include <cslr.h>
#include <cslr_intc21_006.h>
#include <5912.h>


/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_Intc21Regs  * CSL_Intc21RegsOvly;

inline CSL_Intc21RegsOvly _CSL_intc21GetBaseAddr(Uint16 intc21Num)
{
    return (CSL_Intc21RegsOvly)CSL_INTC21_0_REGS;
}

#endif  /* CSLR_INTC21_H_ */
