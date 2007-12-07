/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_aic23_read16.c ========
 *  OSK5912_AIC23_read16( ) implementation
 */

#include <osk5912.h>
#include <osk5912_aic23.h>

/*
 *  ======== OSK5912_AIC23_read16 ========
 *  Read a 16-bit value from the codec
 */
Uint16 OSK5912_AIC23_read16( OSK5912_AIC23_CodecHandle hCodec, Int16 *val )
{
    /* If McBSP doesn't have new data available, return 0 */
    if ( ! ( DSP_MCBSP1_SPCR1 & MCBSP_SPCR1_RRDY ) )
        return 0;

    /* Read the data */
    *val = DSP_MCBSP1_DRR1;
    return 1;
}
