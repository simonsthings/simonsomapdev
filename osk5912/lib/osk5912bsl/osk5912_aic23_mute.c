/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_mute.c ========
 *  OSK5912_AIC23_mute( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>

/*
 *  ======== OSK5912_AIC23_mute ========
 *  Enable/disable codec mute mode
 */
void OSK5912_AIC23_mute( OSK5912_AIC23_CodecHandle hCodec, Uint16 mode )
{
    int regval;

    /* Enable mute if mode is true */
    regval = ( mode ) ? 0x08 : 0x00;

    /* Write to codec registers ( left and right ) */
    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_DIGPATH,
        ( OSK5912_AIC23_rget( hCodec, OSK5912_AIC23_DIGPATH ) & 0xfff7 ) | regval );
}
