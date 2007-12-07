/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23.c ========
 *  OSK5912_AIC23_openCodec( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>
#include <osk5912_i2c.h>

/*
 *  ======== OSK5912_AIC23_openCodec ========
 *  Open the codec and return a codec handle
 */
OSK5912_AIC23_CodecHandle OSK5912_AIC23_openCodec( Uint32 id,
                                                OSK5912_AIC23_Config *Config )
{
    /* Reset the AIC23 */
    OSK5912_AIC23_rset( 0, OSK5912_AIC23_RESET, 0 );

    /* Configure the rest of the AIC23 registers */
    OSK5912_AIC23_config( 0, Config );

    /* Config MCBSP */
    DSP_MCBSP1_PCR0 = 0x0003;
    DSP_MCBSP1_RCR1 = 0x0140;
    DSP_MCBSP1_RCR2 = 0x0000;
    DSP_MCBSP1_XCR1 = 0x0140;
    DSP_MCBSP1_XCR2 = 0x0000;
    DSP_MCBSP1_SRGR1 = 0x0000;
    DSP_MCBSP1_SRGR2 = 0x0000;
    DSP_MCBSP1_SPCR1 = 0x0001;
    DSP_MCBSP1_SPCR2 = 0x0101;

    /* Return 0 as a handle */
    return 0;
}
