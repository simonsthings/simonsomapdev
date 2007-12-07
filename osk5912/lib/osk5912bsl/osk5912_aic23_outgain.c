/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_setfreq.c ========
 *  OSK5912_AIC23_setFreq( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>

/*
 *  ======== OSK5912_AIC23_outGain ========
 *  Set the output gain on the codec
 */
void OSK5912_AIC23_outGain( OSK5912_AIC23_CodecHandle hCodec, Uint16 outGain )
{
    /* Write to codec registers ( left and right ) */
    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_LEFTHPVOL,
        ( OSK5912_AIC23_rget( hCodec, OSK5912_AIC23_LEFTHPVOL ) & 0xff80 )
        | ( outGain & 0x7f ) );

    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_RIGHTHPVOL,
        ( OSK5912_AIC23_rget( hCodec, OSK5912_AIC23_RIGHTHPVOL ) & 0xff80 )
        | ( outGain & 0x7f ) );
}
