/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== sscr_setup.c ========
 *  SSCR's setup functions.
 */

#pragma CODE_SECTION(SSCR_setup, ".text:setup")

#include <std.h>

#include <atm.h>
#include <mem.h>

#include <sscr.h>
#include <utl.h>


/*
 *  ======== SSCR_setup ========
 *  This function sets-up the heapId.  It also allocates (and initializes)
 *  the bucketList.  The bucketList is an array of SSCR_Bucket's.  Each
 *  bucket will hold one scratch buffer.
 *  The caller can specify the buckets and size via the latter two arguments.
 *  If these are specified, the use count in the bucket structure is set
 *  to _SSCR_USERDEFINED.  SSCR_createBuf() and SSCR_deleteBuf() will not
 *  allocate/free these buffers.
 */
Bool SSCR_setup(Int heapId, Uns bucketCnt, Ptr bucketBuf[], Uns bucketSize[])
{
    Int i;
    static Int firstTime = TRUE;

    /* If this is not the first time, return TRUE immediately */
    if (ATM_seti(&firstTime, FALSE) == FALSE) {
        return (TRUE);
    }

    /*
     *  Allocate _SSCR_bucketList. This is only used by the SSCR module and 
     *  has no alignment requirements. The _SSCR_bucketList is always allocated
     *  from the DSP/BIOS Objects segment which is guaranted to be 0.
     */
    _SSCR_bucketList = MEM_alloc(0, (sizeof(SSCR_Bucket) * bucketCnt), 0);

    if (_SSCR_bucketList == MEM_ILLEGAL) {
        return (FALSE);
    }

    /* set up the rest of the private global variables. */
    _SSCR_heapId = heapId;
    _SSCR_bucketCnt = bucketCnt;

    /* 
     *  Fill in the initial values with either NULL and zeros, or 
     *  with the passed in values. 
     */
    for (i = 0; i < _SSCR_bucketCnt; i++) {
        if ((bucketSize != NULL) && (bucketSize[i] != 0)) {
            /* If the size is non-zero, there must be a bucketBuf specified */
            UTL_assert(bucketBuf    != NULL);
            UTL_assert(bucketBuf[i] != NULL);

            /* A size and pointer was supplied */
            _SSCR_bucketList[i].buffer = bucketBuf[i];
            _SSCR_bucketList[i].size   = bucketSize[i];
            _SSCR_bucketList[i].count  = _SSCR_USERDEFINED;
        }
        else {
            _SSCR_bucketList[i].buffer = NULL;
            _SSCR_bucketList[i].size   = 0;
            _SSCR_bucketList[i].count  = 0;
        }
    }

    return (TRUE);
}


