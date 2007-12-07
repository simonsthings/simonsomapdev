/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash_readBurst.c ========
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_readBurst( Uint32 src, Uint32 dst, Uint32 length )         *
 *      Read data from Flash using a 16-word burst mode, given              *
 *          src = source address                                            *
 *          dest = destination address                                      *
 *          length = length in bytes                                        *
 * ------------------------------------------------------------------------ */
void OSK5912_FLASH_readBurst( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i;
    Uint32 emifs_ccs3 = TC_EMIFS_CCS3;

    /* Modify EMIFS settings to allow for burst reads */
    TC_EMIFS_CCS3 = 0
            | ( emifs_ccs3 & 0x7F90FFF3 )   // Copy existing CCS3 settings
            | TC_EMIFS_CCS_PGWSTEN          // Page Wait State Enable
            | ( 3 << 16 )                   // Page Mode 16-word burst read
            ;

    /* Flash Mode: Read Array */
    *( VUint16* )src = READ_ARRAY_CODE;

    /* Transfer bytes from source to destination */
    for ( i = 0 ; i < length ; i++, dst++, src++ )
        *( VUint8* )dst = *( VUint8* )src;

    /* Restore original EMIFS settings to work with asynchronous reads */
    TC_EMIFS_CCS3 = emifs_ccs3;
}
