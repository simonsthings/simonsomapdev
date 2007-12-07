/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912.led.h ========
 *  Interface for LEDs on the OSK5912 board
 */

#ifndef OSK5912_LED_
#define OSK5912_LED_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
/* Initialize the LEDs */
void OSK5912_LED_init( );

/* Turn the LED numbered ledNum off */
void OSK5912_LED_off( Uint16 ledNum );

/* Turn the LED numbered ledNum on */
void OSK5912_LED_on( Uint16 ledNum );

/* Toggle the LED numbered ledNum */
void OSK5912_LED_toggle( Uint16 ledNum );

#ifdef __cplusplus
}
#endif

#endif

