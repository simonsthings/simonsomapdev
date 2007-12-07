/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_write32.c ========
 *  osk5912_AIC23_write32( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>

/*
 *  ======== OSK5912_AIC23_write32 ========
 *  Write a 32-bit value to the codec
 */
Uint16 OSK5912_AIC23_write32( OSK5912_AIC23_CodecHandle hCodec, Int32 val )
{
    /* If McBSP doesn't have new data available, return false */
    if ( ! ( DSP_MCBSP1_SPCR2 & MCBSP_SPCR2_XRDY ) )
        return 0;

    /* Write 32 bit data value to DXR, shift to match format mode */
    DSP_MCBSP1_DXR2 = ( val >> 16 ) & 0x0000FFFF;
    DSP_MCBSP1_DXR1 = val;

    return 1;
}
