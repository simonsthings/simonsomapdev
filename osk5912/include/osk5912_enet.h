/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_enet.h ========
 *  Ethernet interface on the OSK5912 board
 */

#ifndef OSK5912_ENET_
#define OSK5912_ENET_

#ifdef __cplusplus
extern "C" {
#endif



/* ------------------------------------------------------------------------ *
 *  SMC91C96 Registers                                                      *
 * ------------------------------------------------------------------------ */
#define SMC91C96_BASEADDR    0x04800000

#define SMC91C96_TCR         0x00
#define SMC91C96_EPH_STATUS  0x02
#define SMC91C96_RCR         0x04
#define SMC91C96_COUNTER     0x06
#define SMC91C96_MIR         0x08
#define SMC91C96_RPCR        0x0A
#define SMC91C96_BANK        0x0E
#define SMC91C96_CONFIG      0x10
#define SMC91C96_BASE        0x12
#define SMC91C96_IA01        0x14
#define SMC91C96_IA23        0x16
#define SMC91C96_IA45        0x18
#define SMC91C96_GENERAL     0x1A
#define SMC91C96_CONTROL     0x1C
#define SMC91C96_MMU_COMMAND 0x20
#define SMC91C96_PNR         0x22
#define SMC91C96_FIFO_PORTS  0x24
#define SMC91C96_POINTER     0x26
#define SMC91C96_DATAH       0x28
#define SMC91C96_DATAL       0x2A
#define SMC91C96_INTERRUPT   0x2C
#define SMC91C96_MT01        0x30
#define SMC91C96_MT23        0x32
#define SMC91C96_MT45        0x34
#define SMC91C96_MT67        0x36
#define SMC91C96_MGMT        0x38
#define SMC91C96_REVISION    0x3A
#define SMC91C96_ERCV        0x3C

/* ------------------------------------------------------------------------ *
 *  ENET Prototypes                                                         *
 * ------------------------------------------------------------------------ */
/* Write to a SMC91C96 register */
void OSK5912_ENET_rset(Uint16 regnum, Uint16 value);

/* Read from a SMC91C96 register */
Uint16 OSK5912_ENET_rget(Uint16 regnum);

/* Write to an ENET EEPROM address */
void OSK5912_ENET_writeEEPROM(Uint16 addr, Uint16 value);

/* Read from an ENET EEPROM address */
Uint16 OSK5912_ENET_readEEPROM(Uint16 addr);

#ifdef __cplusplus
}
#endif

#endif
