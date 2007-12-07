/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef _CSL_CHIP_H_
#define _CSL_CHIP_H_

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_chip.h>

/**
 * @brief   Enumeration of the C55 system registers
 *
 * This enumeration contains the list of registers that can be manipulated
 * using the CSL_chipReadReg(..) and CSL_chipWriteReg(..) APIs
 */
typedef enum  {
   CSL_CHIP_REG_IER0    = 0x0000u,
   CSL_CHIP_REG_IFR0    = 0x0001u,
   CSL_CHIP_REG_ST0_55  = 0x0002u,
   CSL_CHIP_REG_ST1_55  = 0x0003u,
   CSL_CHIP_REG_ST3_55  = 0x0004u,
   CSL_CHIP_REG_PDP     = 0x002Fu,
   CSL_CHIP_REG_IER1    = 0x0045u,
   CSL_CHIP_REG_IFR1    = 0x0046u,
   CSL_CHIP_REG_DBIER0  = 0x0047u,
   CSL_CHIP_REG_DBIER1  = 0x0048u,
   CSL_CHIP_REG_IVPD    = 0x0049u,
   CSL_CHIP_REG_IVPH    = 0x004Au,
   CSL_CHIP_REG_ST2_55  = 0x004Bu
} CSL_ChipReg;

/**
 * @brief   Initialize the CHIP module
 *
 * Any relevant module-specific initialization is performed here. This API
 * should be called before invoking any other API in the CHIP module.
 * @b Example:
 * @verbatim
      CSL_sysInit( );
      if (CSL_chipInit( ) != CSL_SOK) {
        // module initialization failed! //
      }
  
   @endverbatim
 * 
 * @return  CSL_SOK on success
 */

CSL_Status
    CSL_chipInit (
        void
);


/*
 * @brief   Write to the specified register
 *
 * This function could be used to write to the CPU registers. The
 * register that can be specified could be one of those enumerated
 * in CSL_ChipReg.
 *
 * @b Example:
 * @verbatim
  
       Uint16 oldRegVal;
       oldRegVal = CSL_chipWriteReg(CSL_CHIP_REG_IER0, newRegVal);

   @endverbatim
 *
 * @return  Value of the register before the write
*/
Uint16
    CSL_chipWriteReg (
        CSL_ChipReg     reg,  /**< identifies the register */
        Uint16          val   /**< value to write */
);


/*
 * @brief   Read from specified register
 *
 * This function could be used to read from the CPU registers. The
 * register can be specified could be one of those enumerated in
 * CSL_ChipReg.
 *
 * @b Example:
 * @verbatim
  
       Uint16 regVal;
       regVal = CSL_chipReadReg(CSL_CHIP_REG_IER0);
  
   @endverbatim
 *
 * @return  Value of the register read in
*/

Uint16
    CSL_chipReadReg (
        CSL_ChipReg     reg /**< identifies the register */
);


#endif
