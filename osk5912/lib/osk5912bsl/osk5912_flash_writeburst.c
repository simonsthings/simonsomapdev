/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash_writeburst.c ========
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_writeBurst( Uint32 src, Uint32 dst, Uint32 length )       *
 *      Write to Flash, given                                               *
 *          src = source address                                            *
 *          dest = destination address                                      *
 *          length = length in bytes                                        *
 *                                                                          *
 *  Note:                                                                   *
 *      Limit is only allowed to write in a single page                     *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void OSK5912_FLASH_writeBurst( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i, j;
    Uint32 current_addr;
    Uint32 current_len;

    /* Flash Mode: Read Array */
    *( VUint16* )dst = READ_ARRAY_CODE;

    /* Write in 32-word Bursts */
    for ( i = 0 ; i < ( length >> 1 ) ; i += WRITE_BURST_BUFFER_SIZE,
                                    dst += ( WRITE_BURST_BUFFER_SIZE << 1 ) )
    {
        /* Set current address to start of dst */
        current_addr = dst;

        /* Set current length */
        current_len = ( ( length - i ) >= WRITE_BURST_BUFFER_SIZE ) ?
                            WRITE_BURST_BUFFER_SIZE :
                            length - i ;

        /* Flash Mode: Write to Buffer */
        *( VUint16* )current_addr = WRITE_TO_BUFFER_CODE;

        /* Send the size of the buffer to write */
        *( VUint16* )current_addr = current_len - 1;

        /* Send the raw data */
        for ( j = 0 ; j < current_len ; j++, current_addr += 2, src+=2 )
             *( VUint16* )current_addr = *( VUint16* )src;

        /* Flash Mode: Program */
        *( VUint16* )current_addr = ERASE_PROGRAM_RESUME_CODE;

        /* Wait for operation to complete */
        while ( ! ( *( VUint16* )current_addr & 0x80 ) );

        /* Flash Mode: Read Array */
        *( VUint16* )current_addr = READ_ARRAY_CODE;
    }
}
