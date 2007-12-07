/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#ifndef _CSLR_SSW_H_
#define _CSLR_SSW_H_

#include <cslr.h>
#include <cslr_ssw_002.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_SswRegs  *CSL_SswRegsOvly;

inline CSL_SswRegsOvly _CSL_sswGetBaseAddr(Uint16 sswNum)
  {
    return (CSL_SswRegsOvly)_CSL_sswlookup[sswNum];
  }


#endif  /* CSLR_SSW_H_ */
