/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_power.c ========
 *      Power Controller Registers functions
 */

#include <osk5912.h>
#include <osk5912_i2c.h>
#include <osk5912_power.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_POWER_rset( Uint8 regnum, Uint8 regval )                        *
 *      Set Power Controller register                                       *
 * ------------------------------------------------------------------------ */
Int16 OSK5912_POWER_rset( Uint8 regnum, Uint8 regval )
{
    Uint8 buffer8[2];
    buffer8[0] = regnum;
    buffer8[1] = regval;

    return OSK5912_I2C_write( OSK5912_POWER_I2C_ADDR, buffer8, 2 );
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_POWER_rget( Uint8 regnum, Uint8 *regval )                       *
 *      Get the value of a Power Controller register                        *
 * ------------------------------------------------------------------------ */
Int16 OSK5912_POWER_rget( Uint8 regnum, Uint8 *regval )
{
    Uint16 status = 0;
    Uint8 buffer8[1];

    buffer8[0] = regnum;

    status |= OSK5912_I2C_write( OSK5912_POWER_I2C_ADDR, buffer8, 1 );
    status |= OSK5912_I2C_read( OSK5912_POWER_I2C_ADDR, buffer8, 1 );

    *regval = buffer8[0];

    return status;
}
