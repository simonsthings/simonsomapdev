/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_create.c ========
 *  Channel object creation.
 */

#pragma CODE_SECTION(CHAN_create, ".text:create")

#include <std.h>

#include <mem.h>

#include <chan.h>

/*
 *  ======== CHAN_create ========
 *  This function allocates the CHAN_Obj.
 */
CHAN_Handle CHAN_create(Void)
{
    /* 
     *  A CHAN_Obj is always allocated from the DSP/BIOS Objects 
     *  segment which is guaranted to be 0.
     */
    return ((CHAN_Handle)MEM_alloc(0, sizeof(CHAN_Obj), 0));
}


