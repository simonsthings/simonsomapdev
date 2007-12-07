/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash_read.c ========
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_read( Uint32 src, Uint32 dst, Uint32 length )             *
 *      Read data from Flash , given                                        *
 *          src = source address                                            *
 *          dest = destination address                                      *
 *          length = length in bytes                                        *
 * ------------------------------------------------------------------------ */
void OSK5912_FLASH_read( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i;

    /* Flash Mode: Read Array */
    *( VUint16* )src = READ_ARRAY_CODE;

    /* Transfer bytes from source to destination */
    for ( i = 0 ; i < length ; i++ )
        *( VUint8* )dst++ = *( VUint8* )src++;
}
