/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash.c ========
 *      Implementation of OSK5912_FLASH_init( ) for Intel & Micron based
 *      Flash devices.
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_init( )                                                   *
 *      Initialize the Flash                                                *
 *                                                                          *
 *  Note:                                                                   *
 *      The Write Protect on the Flash is disabled.  This allows the        *
 *      erase & write flash to work.                                        *
 * ------------------------------------------------------------------------ */
void OSK5912_FLASH_init( )
{
    /* Swap CS0 & CS3 if needed */
    if ( OSK5912_FLASH_BASE == 0x00000000 )
        TC_EMIFS_CONFIG |= TC_EMIFS_CONFIG_BM;
    else if ( OSK5912_FLASH_BASE == 0x0C000000 )
        TC_EMIFS_CONFIG &= ~TC_EMIFS_CONFIG_BM;

    /* Flash Mode: Read Array */
    OSK5912_FLASH_BASE_PTR16 = READ_ARRAY_CODE;

    /* Clear the old Status register */
    OSK5912_FLASH_BASE_PTR16 = CLEAR_STATUS_REGISTER_CODE;

    /* Disable write protection on Flash */
    TC_EMIFS_CONFIG |= TC_EMIFS_CONFIG_WP;
}
