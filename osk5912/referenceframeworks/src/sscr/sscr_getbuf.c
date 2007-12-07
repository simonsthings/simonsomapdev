/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== sscr_getbuf.c ========
 *  SSCR's file for getting the size and address of the scratch buffers.
 */

#pragma CODE_SECTION(SSCR_getBuf, ".text:sscr_getBuf")

#include <std.h>

#include <hwi.h>

#include <sscr.h>
#include <utl.h>


/*
 *  ======== SSCR_getBuf ========
 *  This function returns the pointer to and the size of the scratch buffer 
 *  for the specified bucket. It does not allocate memory.
 */
Ptr SSCR_getBuf(Uns scrBucketIndex, Uns *scrSize)
{   
    Uns intrState;            // Used to disable interrupts
    Ptr tmpBuffer;
    
    /* Make sure valid parameters */
    UTL_assert(scrSize != NULL);
    UTL_assert(scrBucketIndex < _SSCR_bucketCnt);    
    
    /* 
     *  Temporarily disable interrupts. This is to prevent a very unlikely race
     *  condition, in which multiple SSCR_deleteBuf()'s, SSCR_createBuf()'s or
     *  SSCR_getBuf()'s are called.
     */
    intrState = HWI_disable();

    *scrSize  = _SSCR_bucketList[scrBucketIndex].size;
    tmpBuffer = _SSCR_bucketList[scrBucketIndex].buffer;

    /* Restore interrupt state */
    HWI_restore( intrState );

    return (tmpBuffer);
}


