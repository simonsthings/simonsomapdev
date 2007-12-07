/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_i2c.h ========
 *  Interface for I2C on the OSK5912 board
 */

#ifndef OSK5912_I2C_
#define OSK5912_I2C_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ *
 *  I2C Transfer modes                                                      *
 * ------------------------------------------------------------------------ */
#define OSK5912_I2C_MODE_SADP       3   // (S)tart..(A)ddr..(D)ata..(n)..sto(P)
#define OSK5912_I2C_MODE_SADD       2   // (S)tart..(A)ddr..(D)ata..(n)..(D)ata

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
/* Initialize I2C */
void OSK5912_I2C_init( );

/* Write to an I2C slave device */
Uint16 OSK5912_I2C_write( Uint16 slave_addr, Uint8* data, Uint16 length );

/* Read from an I2C slave device */
Uint16 OSK5912_I2C_read( Uint16 slave_addr, Uint8* data, Uint16 length );

#ifdef __cplusplus
}
#endif

#endif
