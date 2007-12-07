/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_powerDown.c ========
 *  OSK5912_AIC23_powerDown( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>

/*
 *  ======== OSK5912_AIC23_powerDown ========
 *  Enable/disable powerdown modes for the DAC and ADC codec subsections
 */
void OSK5912_AIC23_powerDown( OSK5912_AIC23_CodecHandle hCodec, Uint16 sect )
{
    /* Write to codec register */
    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_POWERDOWN,
        ( OSK5912_AIC23_rget( hCodec, OSK5912_AIC23_POWERDOWN ) & 0xff00 )
        | ( sect & 0xff ) );
}
