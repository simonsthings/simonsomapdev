/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== sscr_exit.c ========
 *  SSCR's exit file.
 */

#pragma CODE_SECTION(SSCR_exit, ".text:exit")

#include <std.h>

#include <mem.h>

#include <sscr.h>

/*
 *  ======== SSCR_exit ========
 *  The SSCR module exit. Release the resources used and set into
 *  a known state.
 */
Void SSCR_exit(Void)
{
    /*
     *  Free _SSCR_bucketList. The _SSCR_bucketList is always allocated
     *  from the DSP/BIOS Objects segment which is guaranted to be 0.
     */ 
    MEM_free(0, _SSCR_bucketList, (sizeof(SSCR_Bucket) * _SSCR_bucketCnt));

    /* Set the globals back to an initial state. */
    _SSCR_heapId     = _SSCR_INVALIDHEAP;
    _SSCR_bucketCnt  = 0;
    _SSCR_bucketList = NULL;
}


