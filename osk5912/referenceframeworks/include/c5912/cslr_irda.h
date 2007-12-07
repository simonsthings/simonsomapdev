/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#ifndef _CSLR_IRDA_H_
#define _CSLR_IRDA_H_

#include <cslr.h>
#include <cslr_irda_002.h>
#include <5912.h>



/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_IrdaRegs  * CSL_IrdaRegsOvly;

inline CSL_IrdaRegsOvly _CSL_irdaGetBaseAddr(Uint16 irdaNum)
{
    return (CSL_IrdaRegsOvly)_CSL_irdalookup[irdaNum];
}

#endif  /* CSLR_IRDA_H_ */
