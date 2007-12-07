/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef __CSLR_INTC20_H_
#define __CSLR_INTC20_H_

#include <cslr.h>
#include <cslr_intc20_005.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_Intc20Regs  * CSL_Intc20RegsOvly;

inline CSL_Intc20RegsOvly _CSL_intc20GetBaseAddr(Uint16 intc20Num)
{
    return (CSL_Intc20RegsOvly)CSL_INTC20_0_REGS;
}


#endif  /* CSLR_INTC20_H_ */
