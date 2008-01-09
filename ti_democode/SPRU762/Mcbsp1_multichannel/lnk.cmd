/*
 *  Copyright 2005 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 */



MEMORY {
   DATA(RWI):  origin = 0x7000,        len = 0x3000
   PROG:       origin = 0x200,         len = 0x6e00
   VECT:       origin = 0xd000,        len = 0x100
}

SECTIONS
{
    .vectors: {} > VECT
    .trcinit: {} > PROG
    .gblinit: {} > PROG
     frt:     {} > PROG
    .text:    {} > PROG
    .cinit:   {} > PROG
    .pinit:   {} > PROG
    .sysinit: {} > PROG
    .bss:     {} > DATA
    .far:     {} > DATA
    .const:   {} > DATA
    .switch:  {} > DATA
    .sysmem:  {} > DATA
    .cio:     {} > DATA
    .MEM$obj: {} > DATA
    .sysheap: {} > DATA
    .sysstack {} > DATA
    .stack:   {} > DATA
}
