/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *    All rights reserved. Property of Spectrum Digital Incorporated.
 */
 
/*
 *  ======== led.cmd ========
 *  Command file containing memory mapping
 */

MEMORY
{
    /* Inside Internal SRAM - [0x20000000:0x2003E800]*/ 
    INTERNAL_SRAM:  origin = 0x20004000, length 0x0003a800
}

SECTIONS
{
    /* Inside Internal SRAM */ 
    .vector {} > INTERNAL_SRAM
    .stack  {} > INTERNAL_SRAM
    .text:  {} > INTERNAL_SRAM
    .cinit: {} > INTERNAL_SRAM
    .data:  {} > INTERNAL_SRAM
    .bss:   {} > INTERNAL_SRAM
}
