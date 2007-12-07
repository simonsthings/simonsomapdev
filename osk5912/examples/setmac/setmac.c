/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== setmac.c ========
 *
 *  This example sets the Ethernet MAC address using the OSK5912 Board Support
 *  Library ( BSL ).  The Ethernat MAC address below belongs to the Spectrum
 *  Digital OUI & is registerd with the namespace authority ( IEEE ).
 *
 *  The purpose of this example is to demostate basic BSL usage as well as
 *  providing a project base for your own code.
 *
 *  Please see the OSK5912 help file for more detailed information.
 */

/*
 *  The OSK5912 Board Support Library is divided into several modules, each
 *  of which has its own include file.  The file osk5912.h must be included
 *  in every program that uses the BSL.  This example also includes
 *  osk5912_enet.h because it will write to the ENET EEPROM attached to the
 *  ENET chip ( SMC91C96 )
 */
#include <stdio.h>
#include <osk5912.h>
#include <osk5912_enet.h>

Uint8 a[6];
Uint8 MAC_newaddr[6];
Uint8 MAC_oldaddr[6];
Uint8 MAC_currentaddr[6];

/*
 *  Set ethernet MAC address
 */
void main( )
{
    Uint16 i;

    /* Initialize the board support library, must be first BSL call */
    OSK5912_init( 192 );

    /* Print original value */
    *( ( Uint16* )&a[0] ) = OSK5912_ENET_rget( SMC91C96_IA01 );
    *( ( Uint16* )&a[2] ) = OSK5912_ENET_rget( SMC91C96_IA23 );
    *( ( Uint16* )&a[4] ) = OSK5912_ENET_rget( SMC91C96_IA45 );

    printf( "Read back address: %02x-%02x-%02x-%02x-%02x-%02x\n",
            a[0], a[1], a[2], a[3], a[4], a[5] );

    for ( i = 0 ; i < 6 ; i++ )
        MAC_oldaddr[i] = a[i];

    MAC_newaddr[0] = 0x00;  // This is part of the Spectrum Digital OUI reserved
    MAC_newaddr[1] = 0x0e;  // portion of the Ethernet MAC namespace which is
    MAC_newaddr[2] = 0x99;  // registered with the namespace authority ( the IEEE ).
    MAC_newaddr[3] = 0x02;  // You are only authorized to use the MAC address prog-
    MAC_newaddr[4] = 0x02;  // rammed into your board at Spectrum Digital.  Any
    MAC_newaddr[5] = 0x00;  // use of the Spectrum Digital namespace is a violation
                            // of the rules imposed by the IEEE.

    printf( "Trying to write:   %02x-%02x-%02x-%02x-%02x-%02x\n",
            MAC_newaddr[0], MAC_newaddr[1], MAC_newaddr[2],
            MAC_newaddr[3], MAC_newaddr[4], MAC_newaddr[5] );

    /* Write the new MAC address */
    OSK5912_ENET_writeEEPROM( 0x20, MAC_newaddr[0] | ( MAC_newaddr[1] << 8 ) );
    OSK5912_ENET_writeEEPROM( 0x21, MAC_newaddr[2] | ( MAC_newaddr[3] << 8 ) );
    OSK5912_ENET_writeEEPROM( 0x22, MAC_newaddr[4] | ( MAC_newaddr[5] << 8 ) );

    /* Disable general purpose access to the EEPROM */
    OSK5912_ENET_rset( SMC91C96_CONTROL, OSK5912_ENET_rget( SMC91C96_CONTROL ) & 0xfffb );

    /* Clear buffer */
    for ( i = 0; i < 6; i++ )
        a[i] = 0;

    /* Reload the MAC address from the EEPROM */
    OSK5912_ENET_rset( SMC91C96_CONTROL, OSK5912_ENET_rget( SMC91C96_CONTROL ) | 0x0002 );
    while ( ( OSK5912_ENET_rget( SMC91C96_CONTROL ) & 0x0002 ) != 0 );

    /* Copy into array */
    *( ( Uint16* )&a[0] ) = OSK5912_ENET_rget( SMC91C96_IA01 );
    *( ( Uint16* )&a[2] ) = OSK5912_ENET_rget( SMC91C96_IA23 );
    *( ( Uint16* )&a[4] ) = OSK5912_ENET_rget( SMC91C96_IA45 );

    printf( "Read back address: %02x-%02x-%02x-%02x-%02x-%02x\n",
        a[0], a[1], a[2], a[3], a[4], a[5] );

    /* Check results */
    for ( i = 0 ; i < 6 ; i++ )
        if ( MAC_newaddr[i] != a[i] )
        {
            printf( "Address NOT written correctly\n" );

            /* Software Breakpoint to Code Composer */
            asm( " .long 0xE1200070" );
            while( 1 );
        }

    /* Address write success */
    printf( "SUCCESS\n" );

	/* Save Current Address */
    for ( i = 0 ; i < 6 ; i++ )
        MAC_currentaddr[i] = a[i];

    /* Software Breakpoint to Code Composer */
    asm( " .long 0xE1200070" );
    while( 1 );
}
