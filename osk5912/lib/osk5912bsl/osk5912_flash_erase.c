/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash_erase.c ========
 *      osk5912_flash_erase( )
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_erase( Uint32 start, Uint32 length )                      *
 *      Erase the blocks of Flash memory in the specified range             *
 *          start = start address                                           *
 *          length = length in bytes                                        *
 * ------------------------------------------------------------------------ */
void OSK5912_FLASH_erase( Uint32 start, Uint32 length )
{
    Uint32 end;
    VUint32 flash_addr;
    VUint8 *pFlashAddr;

    /* Calculate extents of range to erase */
    end = start + length - 1;

    /* Erase the pages of flash contained within the address range */
    /* Alignment must be 16-bit words */
    flash_addr = start & 0xFFFFFFFE;

    /* Loop until the the flash_addr is passed end */
    for ( ; flash_addr <= end ; flash_addr += OSK5912_FLASH_PAGESIZE )
    {
        pFlashAddr = ( Uint8* )( flash_addr );

        /* Flash Mode: Read Array */
        *pFlashAddr = READ_ARRAY_CODE;

        /* -------- Unlock block -------- */
        *pFlashAddr = CONFIG_BLOCK_LOCK_BITS_CODE;
        *pFlashAddr = CLEAR_LOCK_BITS_CODE;

        /* Flash Mode: Read Status Registers */
        *pFlashAddr = READ_STATUS_REGISTER_CODE;

        /* Wait for operation to complete */
        while( !( *pFlashAddr & READ_STATUS_REGISTER_ISMS ) );

        /* Check for error */
        if ( *pFlashAddr & READ_STATUS_REGISTER_ECLBS )
        {
            /* Flash Mode: Read Array */
            *pFlashAddr = READ_ARRAY_CODE;

            /* Clear status reg */
            *pFlashAddr = CLEAR_STATUS_REGISTER_CODE;
        }
        else
        {
            /* Flash Mode: Read Array */
            *pFlashAddr = READ_ARRAY_CODE;
        }

        /* -------- Erase block -------- */
        *pFlashAddr = BLOCK_ERASE_CODE;
        *pFlashAddr = ERASE_PROGRAM_RESUME_CODE;

        /* Flash Mode: Read Status Registers */
        *pFlashAddr = READ_STATUS_REGISTER_CODE;

        /* Wait for operation to complete */
        while( !( *pFlashAddr & READ_STATUS_REGISTER_ISMS ) );

        /* Flash Mode: Read Array */
        *pFlashAddr = READ_ARRAY_CODE;
    }
}
