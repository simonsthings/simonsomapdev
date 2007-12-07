/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#ifndef _CSLR_CLKRST_H_
#define _CSLR_CLKRST_H_

#include <cslr.h>
#include <cslr_clkrst_002.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_ClkrstRegs  * CSL_ClkrstRegsOvly;


inline CSL_ClkrstRegsOvly _CSL_clkrstGetBaseAddr(Uint16 clkrstNum)
{
        return (CSL_ClkrstRegsOvly)_CSL_clkrstlookup[clkrstNum];
}

#endif  /* CSLR_CLKRST_H_ */
