/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSL_CHIP_H_
#define _CSL_CHIP_H_

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_chip.h>

/**************************************************************************\
* CHIP global macro declarations
\**************************************************************************/

/**************************************************************************\
* CHIP global typedef declarations
\**************************************************************************/
/**************************************************************************\
* CHIP global function declarations
\**************************************************************************/

/*
 * ======================================================
 *   @func   CSL_chipInit.c
 *   @desc
 *         Function initializes the CHIP services.
 *         All CHIP services will be active after this call.
 *
 *
 *   @ret  CSL_Status
 *
 *          CSL_SOK.
 *
 *
 *   @eg
 *     CSL_chipInit(); // All TIMER services are active after this call.
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_chipInit (
    void
);
/*
 * ======================================================
 *   @func   csl_chipWriteReg.c
 *   @desc
 *         Function writes the value passed to the specified register.
 *         Register name is given by the enum declaration of CSL_ChipRegIds.
 *
 *
 *   @arg  regId
 *              ENUM to the register Id.
 *   @arg  newRegVal
 *              New register value which needs to be written.
 *   @ret  Uint16
 *
 *          Old register Value.
 *
 *
 *   @eg
 *     Uint16 oldRegVal;
 *     oldRegVal = CSL_chipWriteReg(CSL_CHIP_IER0, newRegVal);
 *
 *
 * ======================================================
*/

Uint16  CSL_chipWriteReg(
    CSL_ChipRegIds            regId,
    Uint16                newRegVal
);

/*
 * ======================================================
 *   @func   csl_chipReadReg.c
 *   @desc
 *         Function returns the value of the specified register.
 *         Register name is given by the enum declaration of CSL_ChipRegIds.
 *
 *
 *   @arg  regId
 *              ENUM to the register Id.
 *   @ret  Uint16
 *
 *          register value is returned.`
 *
 *
 *   @eg
 *     Uint16 regVal;
 *     regVal = CSL_chipReadReg(CSL_CHIP_IER0);
 *
 *
 * ======================================================
*/

Uint16  CSL_chipReadReg(
    CSL_ChipRegIds            regId
);

/**************************************************************************\
* CHIP inline function declarations
\**************************************************************************/

/**************************************************************************\
* CHIP inline function definitions
\**************************************************************************/


#endif
