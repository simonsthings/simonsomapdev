/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_loopback.c ========
 *  OSK5912_AIC23_loopback( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>

/*
 *  ======== OSK5912_AIC23_loopback ========
 *  Enable/disable codec loopback mode
 */
void OSK5912_AIC23_loopback( OSK5912_AIC23_CodecHandle hCodec, Uint16 mode )
{
    int regval;

    /* Set bypass bit if mode is true */
    regval = ( mode ) ? 0x8 : 0x0;

    /* Write to codec register */
    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_ANAPATH,
        ( OSK5912_AIC23_rget( hCodec, OSK5912_AIC23_ANAPATH ) & 0xfff7 | regval ) );
}
