/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_write16.c ========
 *  OSK5912_AIC23_write16( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>


/*
 *  ======== OSK5912_AIC23_write16 ========
 *  Write a 16-bit value to the codec
 */
Uint16 OSK5912_AIC23_write16( OSK5912_AIC23_CodecHandle hCodec, Int16 val )
{
    /* If McBSP not ready for new data, return false */
    if ( ! ( DSP_MCBSP1_SPCR2 & MCBSP_SPCR2_XRDY ) )
        return 0;

    /* Write 16 bit data value to DXR */
    DSP_MCBSP1_DXR1 = val;
    return 1;
}

