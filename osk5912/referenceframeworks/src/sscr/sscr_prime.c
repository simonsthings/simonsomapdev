/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== sscr_prime.c ========
 *  SSCR's file for determining the scratch buffer sizes.
 */

#pragma CODE_SECTION(SSCR_prime, ".text:SSCR_prime")

#include <std.h>

#include <hwi.h>
#include <ialg.h>

#include <sscr.h>
#include <utl.h>


/*
 *  ======== SSCR_prime ========
 *  This function is called multiple times to determine the worst-case
 *  scratch requirements. The function queries the algorithm to determine
 *  it's scratch requirements. This function does NOT allocate memory.
 *  
 *  Note: The parent concept is not supported in SSCR.
 */ 
Bool SSCR_prime(Uns scrBucketIndex, IALG_Fxns *fxns, IALG_Params *params)
{
    Int i;
    Int numRecs;    
    Uns curScratchReq = 0;
    Uns intrState;                        // Used to disable interrupts    
    IALG_MemRec memTab[_SSCR_MAXMEMRECS]; // Stack-based avoids frag.

    /* Check passed in parameters */
    UTL_assert(scrBucketIndex < _SSCR_bucketCnt);
    UTL_assert(fxns != NULL);
        
    /* Determine how many memTab records are needed */
    numRecs = (fxns->algNumAlloc != NULL) ? 
        fxns->algNumAlloc() : IALG_DEFMEMRECS;

    /* 
     *  Stack-based memTab[] avoids the risk of fragmentation.
     *  A maximum number of memRecs is set. If, in the unlikely case,
     *  more memRecs than max are requested, return failure.
     */
    if (numRecs > _SSCR_MAXMEMRECS) {
        return (FALSE);
    }
    
    /* Query the algorithm about its memory requirements. */
    numRecs = fxns->algAlloc(params, NULL, memTab);    
    if (numRecs <= 0 ) { 
        return (FALSE);
    }
    
    /* 
     *  Loop through the memTab records to find on-chip scratch requests.
     *  Sum the size and alignment of each on-chip scratch memory request to
     *  determine worst-case requirements for the algorithm.
     */
    
    for (i = 0; i < numRecs; i++) {               
        if ( (memTab[i].attrs == IALG_SCRATCH) && 
             (!(IALG_isOffChip(memTab[i].space))) ) {            
            /* 
             *  Add in this memTab record's on-chip memory request into
             *  the running total.              
             */
            curScratchReq += memTab[i].size + memTab[i].alignment;
        }          
    }

    /* 
     *  Temporarily disable interrupts. This is to prevent a very unlikely race 
     *  condition, in which two SSCR_primes are called from separate threads.
     */
    intrState = HWI_disable();

    /* 
     *  Have the total scratch requirement. If user defined bucket, 
     *  make sure that requested memory is not greater than specified size.
     *  If not user defined, check to see if this algorithm instance needs
     *  more than other algorithm instances using this bucket.
     */
    if (curScratchReq > _SSCR_bucketList[scrBucketIndex].size) {
        if (_SSCR_bucketList[scrBucketIndex].count == _SSCR_USERDEFINED) {
            /* Restore interrupt state */
            HWI_restore( intrState );
            
            /* User did not define large enough buffer. */
            return(FALSE);
        }
        else {
            /* 
             *  This request is larger than previous requests in this bucket.
             *  Update table with the new larger size.
             */ 
            _SSCR_bucketList[scrBucketIndex].size = curScratchReq;    
        }     
    }

    /* Restore interrupt state */
    HWI_restore( intrState );

    return (TRUE);
}

