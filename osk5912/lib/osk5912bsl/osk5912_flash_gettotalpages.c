/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== OSK5912_FLASH_getTotalPages.c ========
 *      Determine the total number of pages of Flash memory
 */

#include <osk5912.h>
#include <osk5912_flash.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_FLASH_getTotalPages( Uint16 num_flash_device )                  *
 *      Determine the total number of pages of Flash                        *
 * ------------------------------------------------------------------------ */
Uint32 OSK5912_FLASH_getTotalPages( Uint16 num_flash_device )
{
    Uint16 mfg_id, dev_id;
    Uint32 total_pages = 0;
    Uint32 current_page_addr = OSK5912_FLASH_BASE;
    Uint32 i;

    /* Flash Mode: Read Array */
    OSK5912_FLASH_BASE_PTR16 = READ_ARRAY_CODE;

    /* Loop through the total number of Flash devices */
    for ( i = 0 ; i < num_flash_device ; i++ )
    {
        /* Flash Mode: Read Indentifier */
        *( VUint16* )current_page_addr = READ_IDENTIFIER_CODE;

        /* Read MFG_ID & DEV_ID to determine total number of pages in each */
        mfg_id = *( VUint16* )( current_page_addr );
        dev_id = *( VUint16* )( current_page_addr + 2 );

        /* Flash Mode: Read Array */
        *( VUint16* )current_page_addr = READ_ARRAY_CODE;

        /* Intel Flash */
        if ( mfg_id == 0x89 )
        {
            if ( dev_id == 0x8801 )
                total_pages += 64;
            else if ( dev_id == 0x8802 )
                total_pages += 128;
            else if ( dev_id == 0x8803 )
                total_pages += 256;
            /* Micron Flash w/ INTEL Flash device id codes */
            else if ( dev_id == 0x16 )
                total_pages += 32;
            else if ( dev_id == 0x17 )
                total_pages += 64;
            else if ( dev_id == 0x18 )
                total_pages += 128;
        }
        /* Micron Flash */
        else if ( mfg_id == 0x2C )
        {
            if ( dev_id == 0x16 )
                total_pages += 32;
            else if ( dev_id == 0x17 )
                total_pages += 64;
            else if ( dev_id == 0x18 )
                total_pages += 128;
        }
        /* Unknown Device */
        else
            break;

        /* Set the current page addr to take into account the new sizes */
        current_page_addr += total_pages * OSK5912_FLASH_PAGESIZE;
    }

    /* Return total pages of Flash */
    return total_pages;
}
