/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_registers.c ========
 *  OSK5912_AIC23_rset( ) and OSK5912_AIC23_rget( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>
#include <osk5912_i2c.h>

/* Internal codec state used to simulate read/write functionality */
static OSK5912_AIC23_Config codecstate = OSK5912_AIC23_DEFAULTCONFIG;

/*
 *  ======== OSK5912_AIC23_rset ========
 *  Set codec register regnum to value regval
 */

void OSK5912_AIC23_rset( OSK5912_AIC23_CodecHandle hCodec, Uint16 regnum,
                        Uint16 regval )
{
    Uint16 buf = ( ( regval & 0x00FF ) << 8 ) | ( regnum << 1 ) | ( regval >> 8 );
    OSK5912_I2C_write( OSK5912_AIC23_I2CADDR, ( Uint8* )&buf, 2 );
    codecstate.regs[regnum] = regval;
}


/*
 *  ======== OSK5912_AIC23_rget ========
 *  Return value of codec register regnum
 */
Uint16 OSK5912_AIC23_rget( OSK5912_AIC23_CodecHandle hCodec, Uint16 regnum )
{
    if ( regnum < OSK5912_AIC23_NUMREGS )
        return codecstate.regs[regnum];
    else
        return 0;
}

/*
 *  ======== OSK5912_AIC23_config ========
 *  Set the default codec register config values
 */
void OSK5912_AIC23_config( OSK5912_AIC23_CodecHandle hCodec,
                          OSK5912_AIC23_Config *Config )
{
    int i;

    /* Use default parameters if none are given */
    if ( Config == NULL )
    Config = &codecstate;

    /* Configure power down register first */
    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_POWERDOWN,
        Config -> regs[OSK5912_AIC23_POWERDOWN] );

    /* Assign each register */
    for ( i = 0; i < OSK5912_AIC23_NUMREGS; i++ )
        if ( i != OSK5912_AIC23_POWERDOWN )
            OSK5912_AIC23_rset( hCodec, i, Config -> regs[i] );
}
