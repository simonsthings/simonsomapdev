/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#ifndef _CSLR_I2C_H_
#define _CSLR_I2C_H_

#include <cslr.h>
#include <cslr_i2c_002.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_I2cRegs  *CSL_I2cRegsOvly;

inline CSL_I2cRegsOvly _CSL_i2cGetBaseAddr(Uint16 i2cNum)
  {
    return (CSL_I2cRegsOvly)_CSL_i2clookup[i2cNum];
  }


#endif  /* CSLR_I2C_H_ */
