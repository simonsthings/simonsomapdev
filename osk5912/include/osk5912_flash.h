/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_flash.h ========
 *      Interface for Intel & Micron Based Flash
 */

#ifndef OSK5912_FLASH_
#define OSK5912_FLASH_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ *
 *  Flash Device - INTEL RC28F256K3C120                                     *
 *                 MICRON MT28f128K3                                        *
 * ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ *
 *  Flash Memory Property Definitions                                       *
 * ------------------------------------------------------------------------ */
#define OSK5912_FLASH_BASE          0x0C000000
#define OSK5912_FLASH_PAGESIZE      0x00020000

// Flash Wirte Burst Size
#define WRITE_BURST_BUFFER_SIZE     16

/* ------------------------------------------------------------------------ *
 *  Flash Memory Command Codes                                              *
 * ------------------------------------------------------------------------ */
#define READ_ARRAY_CODE                     0xFF
#define READ_IDENTIFIER_CODE                0x90
#define READ_QUERY_CODE                     0x98
#define READ_STATUS_REGISTER_CODE           0x70
#define CLEAR_STATUS_REGISTER_CODE          0x50
#define PROGRAM_CODE                        0x40 // -or- 0x10
#define WRITE_TO_BUFFER_CODE                0xE8
#define BUFFERED_EFP_CODE                   0x80
#define BLOCK_ERASE_CODE                    0x20
#define ERASE_PROGRAM_SUSPEND_CODE          0xB0
#define ERASE_PROGRAM_RESUME_CODE           0xD0
#define CONFIGURATION_CODE                  0xB8
#define CONFIG_BLOCK_LOCK_BITS_CODE         0x60
#define SET_LOCK_BITS_CODE                  0x01
#define CLEAR_LOCK_BITS_CODE                0xD0
#define PROTECTION_PROGRAM_CODE             0xC0

/* ------------------------------------------------------------------------ *
 *  Flash Memory Read Status Register Fields                                *
 * ------------------------------------------------------------------------ */
#define READ_STATUS_REGISTER_ISMS           0x80 // WRITE STATE MACHINE
#define READ_STATUS_REGISTER_ESS            0x40 // ERASE SUSPEND
#define READ_STATUS_REGISTER_ECLBS          0x20 // ERASE & CLEAR LOCK BITS
#define READ_STATUS_REGISTER_PSLBS          0x10 // PROGRAM & SET LOCK BIT
#define READ_STATUS_REGISTER_VPENS          0x08 // PROGRAMMING VOLTAGE
#define READ_STATUS_REGISTER_PSS            0x04 // PROGRAM SUSPEND
#define READ_STATUS_REGISTER_DPS            0x02 // DEVICE PROTECT

/* ------------------------------------------------------------------------ *
 *  Flash Memory Data Pointer                                               *
 * ------------------------------------------------------------------------ */
#define OSK5912_FLASH_BASE_PTR32    *( Uint32* )OSK5912_FLASH_BASE
#define OSK5912_FLASH_BASE_PTR16    *( Uint16* )OSK5912_FLASH_BASE
#define OSK5912_FLASH_BASE_PTR8     *( Uint8* ) OSK5912_FLASH_BASE

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */

/* Initialize Flash memory */
void OSK5912_FLASH_init( );

/* Determine the total number of pages of Flash */
Uint32 OSK5912_FLASH_getTotalPages( Uint16 num_flash_device );

/* Calculate the checksum of a data range in Flash */
Uint32 OSK5912_FLASH_checksum( Uint32 start, Uint32 length );

/* Erase blocks Flash memory */
void OSK5912_FLASH_erase( Uint32 start, Uint32 length );

/* Read data from Flash */
void OSK5912_FLASH_read( Uint32 src, Uint32 dst, Uint32 length );

/* Write data to Flash */
void OSK5912_FLASH_write( Uint32 src, Uint32 dst, Uint32 length );

/* Read data from Flash using burst reads */
void OSK5912_FLASH_readBurst( Uint32 src, Uint32 dst, Uint32 length );

/* Write data to Flash using burst writes */
void OSK5912_FLASH_writeBurst( Uint32 src, Uint32 dst, Uint32 length );

#ifdef __cplusplus
}
#endif

#endif

