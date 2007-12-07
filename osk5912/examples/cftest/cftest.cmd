/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *    All rights reserved. Property of Spectrum Digital Incorporated.
 */ 

/*
 *  ======== cftest.cmd ========
 *  Command file containing memory mapping
 */ 

MEMORY
{
    /* Inside Internal SRAM - [0x20000000:0x2003a800]*/ 
    INTERNAL_SRAM:  origin = 0x20000000, length 0x3a800
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
