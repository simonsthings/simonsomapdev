/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_delete.c ========
 *  Channel object deletion.
 */

#pragma CODE_SECTION(CHAN_delete, ".text:delete")

#include <std.h>

#include <mem.h>

#include <chan.h>
#include <utl.h>

/*
 *  ======== CHAN_delete ========
 *  This function frees the CHAN_Obj.
 */
Bool CHAN_delete(CHAN_Handle chanHandle)
{    
    /* Make sure parameters are valid */
    UTL_assert(chanHandle != NULL);

    /* 
     *  Free the memory that was created in CHAN_create()
     *  A CHAN_Obj is always allocated from the DSP/BIOS Objects 
     *  segment which is guaranted to be 0.
     */
    return (MEM_free(0, chanHandle, sizeof(CHAN_Obj)));
}


