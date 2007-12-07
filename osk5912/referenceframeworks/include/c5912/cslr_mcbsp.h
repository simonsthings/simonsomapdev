/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

#ifndef _CSLR_MCBSP_H_
#define _CSLR_MCBSP_H_

#include <cslr.h>
#include <cslr_mcbsp_002.h>
#include <5912.h>


/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_McbspRegs  * CSL_McbspRegsOvly;

inline CSL_McbspRegsOvly _CSL_mcbspGetBaseAddr(Uint16 mcbspNum)
{
    return (CSL_McbspRegsOvly)_CSL_mcbsplookup[mcbspNum];
}

#endif  /* CSLR_MCBSP_H_ */
