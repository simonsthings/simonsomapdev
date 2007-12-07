/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *    ======== util.h ========
 *
 */

#define MASK00                      0x00000007
#define MASK01                      0x0000000E
#define MASK02                      0x0000001C
#define MASK03                      0x00000038
#define MASK04                      0x00000070
#define MASK05                      0x000000E0
#define MASK06                      0x000001C0
#define MASK07                      0x00000380
#define MASK08                      0x00000700
#define MASK09                      0x00000E00
#define MASK10                      0x00001C00
#define MASK11                      0x00003800
#define MASK12                      0x00007000
#define MASK13                      0x0000E000
#define MASK14                      0x0001C000
#define MASK15                      0x00038000
#define MASK16                      0x00070000
#define MASK17                      0x000E0000
#define MASK18                      0x001C0000
#define MASK19                      0x00380000
#define MASK20                      0x00700000
#define MASK21                      0x00E00000
#define MASK22                      0x01C00000
#define MASK23                      0x03800000
#define MASK24                      0x07000000
#define MASK25                      0x0E000000
#define MASK26                      0x1C000000
#define MASK27                      0x38000000
#define MASK28                      0x70000000
#define MASK29                      0xE0000000
#define MASK30                      0xC0000000
#define MASK31                      0x80000000

#define Read( addr )                    ( addr )
#define Write( addr, data )             ( addr = data )
#define ReadMask( addr, mask )          ( addr & mask )
#define WriteMask( addr, data, mask )   ( addr = ( addr & ~mask ) | data )
#define ClearMask( addr, mask )         ( addr = ( addr & ~mask ) )
#define SetMask( addr, mask )           ( addr = ( addr | mask ) )
#define ReadField( addr, mask )         ( ( addr & mask ) >> firstbit( mask ) )
#define WriteField( addr, data, mask )  ( addr = ( addr & ~mask ) | ( data << firstbit( mask ) ) )

extern int firstbit( unsigned int mask );
