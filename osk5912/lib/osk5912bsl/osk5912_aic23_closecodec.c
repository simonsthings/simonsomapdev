/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_closeCodec.c ========
 *  OSK5912_AIC23_closeCodec( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>

/*
 *  ======== OSK5912_AIC23_closeCodec ========
 *  Close the codec
 */
void OSK5912_AIC23_closeCodec( OSK5912_AIC23_CodecHandle hCodec )
{
    /* Close MCBSP data channel */
    DSP_MCBSP1_SPCR1 = 0;
    DSP_MCBSP1_SPCR2 = 0;

    /* Turn off AIC23 codec */
    OSK5912_AIC23_rset( hCodec, OSK5912_AIC23_POWERDOWN, 0xff );
}
