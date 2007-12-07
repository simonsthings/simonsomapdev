/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== OSK5912_POWER.h ========
 *  Interface for I2C on the OSK5912 board
 */

#ifndef OSK5912_POWER_
#define OSK5912_POWER_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ *
 *  I2C address                                                             *
 * ------------------------------------------------------------------------ */
#define OSK5912_POWER_I2C_ADDR      0x48


/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
/* Power Controller Registers Set Fuction */
Int16 OSK5912_POWER_rset( Uint8 regnum, Uint8 regval );

/* Power Controller Registers Get Fuction */
Int16 OSK5912_POWER_rget( Uint8 regnum, Uint8 *regval );

#ifdef __cplusplus
}
#endif

#endif
