/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash_checksum.c ========
 *      osk5912_flash_checksum( )
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_checksum( Uint32 start, Uint32 length )                   *
 *      Calculate the byte checksum from the (start) address to the length  *
 *      Outputs a 32-bit value for the checksum.                            *
 *          start = start address                                           *
 *          length = length in bytes                                        *
 * ------------------------------------------------------------------------ */
Uint32 OSK5912_FLASH_checksum( Uint32 start, Uint32 length )
{
    Uint32 i, checksum = 0;

    /* Flash Mode: Read Array */
    *( VUint16* )start = READ_ARRAY_CODE;

    /* Calculate checksum by adding each byte in the given range */
    for ( i = 0 ; i < length ; i++ )
        checksum += *( VUint8* )( start++ );

    /* Return the 32-bit checksum value */
    return checksum;
}
