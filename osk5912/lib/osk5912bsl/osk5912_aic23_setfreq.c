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

/* Table of supported frequencies */
static Uint16 freqtable[] =
{
    OSK5912_AIC23_FREQ_8KHZ, 0x06, // 8000 Hz
    OSK5912_AIC23_FREQ_16KHZ, 0x2c, // 16000 Hz
    OSK5912_AIC23_FREQ_24KHZ, 0x20, // 24000 Hz
    OSK5912_AIC23_FREQ_32KHZ, 0x0c, // 32000 Hz
    OSK5912_AIC23_FREQ_44KHZ, 0x11, // 44100 Hz
    OSK5912_AIC23_FREQ_48KHZ, 0x00, // 48000 Hz
    OSK5912_AIC23_FREQ_96KHZ, 0x0e, // 96000 Hz
    0, 0                            // End of table
};

/*
 *  ======== OSK5912_AIC23_setFreq ========
 *  Set the codec sample rate frequency
 */
void OSK5912_AIC23_setFreq( OSK5912_AIC23_CodecHandle hCodec, Uint32 freq )
{
    Uint16 regval, curr;

    /* Calculate codec clock control register setting, assume USB Mode( 12MHz )*/
    /* regval will contain CLKIN, SR3..SR0, BOSR */
    curr = 0;
    while( 1 )
    {
        /* Do nothing if frequency doesn't match */
        if ( freqtable[curr] == 0 )
            return;

        /* Check for match */
        if ( freqtable[curr] == freq )
        {
            regval = freqtable[curr + 1];
            break;
        }

        /* Set up for next pair */
        curr += 2;
    }

    /* Write to codec register */
    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_SAMPLERATE,
        ( OSK5912_AIC23_rget( hCodec, OSK5912_AIC23_SAMPLERATE ) & 0xff81 )
        | ( ( regval & 0x3f ) << 1 ) );
}
