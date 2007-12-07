/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== led.c ========
 *  This example blinks LED #0 at a rate of about 2.5 times per second using
 *  the LED module of the the OSK5912 Board Support Library.
 *
 *  The purpose of this example is to demonstrate basic BSL usage as well
 *  as provide a project base for your own code.
 *
 *  Please see the OSK5912 help file under for more detailed information.
 */

/*
 *  The OSK5912 Board Support Library is divided into several modules, each
 *  of which has its own include file.  The file osk5912.h must be included
 *  in every program that uses the BSL.  This example also includes
 *  osk5912_led.h because it uses the LED module.
 *
 *  Note: the LEDs on the OSK5912 are connected to the Power Controller
 *  ( TPS65010 ), and the LEDs can only be addressed via I2C commands.
 */
#include "osk5912.h"
#include "osk5912_led.h"

/*
 *  Run the LED test.
 */
void main( )
{
    /* Initialize the board support library, must be first BSL call */
    OSK5912_init( 192 );

    /* Initialize the LED module */
    OSK5912_LED_init( );

    while( 1 )
    {
        /* Toggle LED #0 */
        OSK5912_LED_toggle( 0 );

        /* Spin in a software delay loop for about 200ms */
        OSK5912_waitusec( 50000 );
    }
}
