/*
 *  Copyright 2003 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_enet.c ========
 *  Ethernet interface implementation
 */

#include <osk5912.h>
#include <osk5912_enet.h>

void OSK5912_ENET_rset(Uint16 regnum, Uint16 value)
{
    Int16 bank;

    /* Bank is is bits 7..4 */
    bank = (regnum & 0xf0) >> 4;

    /* Set bank */
    *((Uint16 *)(SMC91C96_BASEADDR + SMC91C96_BANK)) = bank;

    /* Write value, offset is in bits 3..0 */
    *((Uint16 *)(SMC91C96_BASEADDR + (regnum & 0xf))) = value;
}

Uint16 OSK5912_ENET_rget(Uint16 regnum)
{
    Int16 bank;

    /* Bank is is bits 7..4 */
    bank = (regnum & 0xf0) >> 4;

    /* Set bank */
    *((Uint16 *)(SMC91C96_BASEADDR + SMC91C96_BANK)) = bank;

    /* Write value, offset is in bits 3..0 */
    return *((Uint16 *)(SMC91C96_BASEADDR + (regnum & 0xf)));
}

void OSK5912_ENET_writeEEPROM(Uint16 addr, Uint16 value)
{
    /* Enable general purpose access to the EEPROM, point at offset 0 */
    OSK5912_ENET_rset(SMC91C96_CONTROL, OSK5912_ENET_rget(SMC91C96_CONTROL) | 0x0004);
    OSK5912_ENET_rset(SMC91C96_POINTER, addr);

    /* Perform the STORE operation */
    OSK5912_ENET_rset(SMC91C96_GENERAL, value);
    OSK5912_ENET_rset(SMC91C96_CONTROL, OSK5912_ENET_rget(SMC91C96_CONTROL) | 0x0001);
    while ((OSK5912_ENET_rget(SMC91C96_CONTROL) & 0x0001) != 0);

    /* Disable general purpose access to the EEPROM */
    OSK5912_ENET_rset(SMC91C96_CONTROL, OSK5912_ENET_rget(SMC91C96_CONTROL) & 0xfffb);
}

Uint16 OSK5912_ENET_readEEPROM(Uint16 addr)
{
    /* Enable general purpose access to the EEPROM, point at offset 0 */
    OSK5912_ENET_rset(SMC91C96_CONTROL, OSK5912_ENET_rget(SMC91C96_CONTROL) | 0x0004);
    OSK5912_ENET_rset(SMC91C96_POINTER, addr);

    /* Perform the read operation */
    OSK5912_ENET_rset(SMC91C96_CONTROL, OSK5912_ENET_rget(SMC91C96_CONTROL) | 0x0002);
    while ((OSK5912_ENET_rget(SMC91C96_CONTROL) & 0x0002) != 0);

    /* Disable general purpose access to the EEPROM */
    OSK5912_ENET_rset(SMC91C96_CONTROL, OSK5912_ENET_rget(SMC91C96_CONTROL) & 0xfffb);

    return OSK5912_ENET_rget(SMC91C96_GENERAL);
}
