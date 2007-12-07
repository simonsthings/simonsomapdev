/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash_write.c ========
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_write( Uint32 src, Uint32 dst, Uint32 length )            *
 *      Write to Flash, given                                               *
 *          src = source address                                            *
 *          dest = destination address                                      *
 *          length = length in bytes                                        *
 * ------------------------------------------------------------------------ */
void OSK5912_FLASH_write( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i = 0;
    VUint8 tmpval;
    VUint16 oldval;
    VUint16 newval;

    /* Flash Mode: Read Array */
    *( VUint16* )dst = READ_ARRAY_CODE;

    /* -------- Align to 16-bit words -------- */
    if ( dst & 1 )
    {
        /* Retrieve the data to write and overwrite*/
        tmpval = *( Uint8* )src;
        oldval = *( VUint16* )dst;
        newval = ( oldval & 0x00ff ) + ( tmpval << 8 );

        /* Program one 16-bit word */
        *( VUint16* )dst = PROGRAM_CODE;
        *( VUint16* )dst = newval;

        /* Flash Mode: Read Status Register */
        *( VUint16* )dst = READ_STATUS_REGISTER_CODE;

        /* Wait for operation to complete */
        while( !( *( VUint16* )dst & READ_STATUS_REGISTER_ISMS ) );

        /* Flash Mode: Read Array */
        *( VUint16* )dst = READ_ARRAY_CODE;

        /* Increment variables */
        i++;
        length--;
        src++;
        dst++;
    }

    /* -------- Main Flash writing loop -------- */
    for ( ; i < ( length & 0xFFFFFFFE ) ; i += 2, src += 2, dst += 2 )
    {
        /* Retrieve the data to write and overwrite*/
        newval =  ( *( Uint8* )( src ) );
        newval += ( *( Uint8* )( src + 1 ) ) << 8;

        /* Program one 16-bit word */
        *( VUint16* )dst = PROGRAM_CODE;
        *( VUint16* )dst = newval;

        /* Flash Mode: Read Status Register */
        *( VUint16* )dst = READ_STATUS_REGISTER_CODE;

        /* Wait for operation to complete */
        while( !( *( VUint16* )dst & READ_STATUS_REGISTER_ISMS ) );

        /* Flash Mode: Read Array */
        *( VUint16* )dst = READ_ARRAY_CODE;
    }

    /* -------- Write any data leftover -------- */
    if ( length & 1 )
    {
        /* Retrieve the data to write and overwrite*/
        tmpval = *( Uint8* )src;
        oldval = *( VUint16* )dst;
        newval = ( oldval & 0xff00 ) + ( tmpval );

        /* Program one 16-bit word */
        *( VUint16* )dst = PROGRAM_CODE;
        *( VUint16* )dst = newval;

        /* Flash Mode: Read Status Register */
        *( VUint16* )dst = READ_STATUS_REGISTER_CODE;

        /* Wait for operation to complete */
        while( !( *( VUint16* )dst & READ_STATUS_REGISTER_ISMS ) );

        /* Flash Mode: Read Array */
        *( VUint16* )dst = READ_ARRAY_CODE;
    }
}
