/*
 *  Copyright 2003 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== OSK5912_led.c ========
 *  LED module for the osk5912
 */

#include <osk5912.h>
#include <osk5912_led.h>


/* ------------------------------------------------------------------------ *
 *  OSK5912_LED_init( )                                                     *
 *      Initialize LEDs on OSK5912                                          *
 *      Default state:                                                      *
 *          Led0 [D3] - OFF                                                 *
 *          Led1 [D2] - OFF                                                 *
 * ------------------------------------------------------------------------ */
void OSK5912_LED_init( )
{
    /* Turn all LEDs off */
    OSK5912_LED_off( 0 );
    OSK5912_LED_off( 1 );
}


/* ------------------------------------------------------------------------ *
 *  OSK5912_LED_off( Uint16 ledNum )                                        *
 *      ledNum = 0 [D3]                                                     *
 *      ledNum = 1 [D2]                                                     *
 * ------------------------------------------------------------------------ */
void OSK5912_LED_off( Uint16 ledNum )
{
    Uint8 regval;

    /* Turn D3 - OFF */
    if ( ledNum == 0 )
    {
        OSK5912_POWER_rget( 0x10, &regval );
        OSK5912_POWER_rset( 0x10, 0xDD & regval );
    }
    /* Turn D2 - OFF */
    if ( ledNum == 1 )
    {
        OSK5912_POWER_rset( 0x0A, 0x00 );
        OSK5912_POWER_rset( 0x0B, 0x00 );
    }
}


/* ------------------------------------------------------------------------ *
 *  OSK5912_LED_on( Uint16 ledNum )                                         *
 *      ledNum = 0 [D3]                                                     *
 *      ledNum = 1 [D2]                                                     *
 * ------------------------------------------------------------------------ */
void OSK5912_LED_on( Uint16 ledNum )
{
    Uint8 regval;

    /* Turn D3 - ON */
    if ( ledNum == 0 )
    {
        OSK5912_POWER_rget( 0x10, &regval );
        OSK5912_POWER_rset( 0x10, 0x22 | regval );
    }
    /* Turn D2 - ON */
    if ( ledNum == 1 )
    {
        OSK5912_POWER_rset( 0x0A, 0x80 );
        OSK5912_POWER_rset( 0x0B, 0x80 );
    }
}


/* ------------------------------------------------------------------------ *
 *  OSK5912_LED_toggle( Uint16 ledNum )                                     *
 *      ledNum = 0 [D3]                                                     *
 *      ledNum = 1 [D2]                                                     *
 * ------------------------------------------------------------------------ */
void OSK5912_LED_toggle( Uint16 ledNum )
{
    Uint8 regval;

    /* Toggle D3 */
    if ( ledNum == 0 )
    {
        OSK5912_POWER_rget( 0x10, &regval );
        if ( ( ( regval & 0x20 ) == 0 ) || ( ( regval & 0x02 ) == 0 ) )
            OSK5912_LED_on( ledNum );
        else
            OSK5912_LED_off( ledNum );
    }
    /* Toggle D2 */
    if ( ledNum == 1 )
    {
        OSK5912_POWER_rget( 0x0B, &regval );
        if ( ( regval & 0x80 ) == 0 )
            OSK5912_LED_on( ledNum );
        else
            OSK5912_LED_off( ledNum );
    }
}
