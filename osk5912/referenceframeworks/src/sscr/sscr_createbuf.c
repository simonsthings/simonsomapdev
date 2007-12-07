/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== sscr_createBuf.c ========
 *  SSCR's file for creating the scratch buffers.
 */

#pragma CODE_SECTION(SSCR_createBuf, ".text:SSCR_createBuf")

#include <std.h>

#include <hwi.h>
#include <mem.h>

#include <sscr.h>
#include <utl.h>


/*
 *  ======== SSCR_createBuf ========
 *  Allocate a buffer based on the scrBucketIndex.  This function
 *  can be called multiple times, but only the first time does it 
 *  allocate a buffer per priority bucket.  Afterwards, it simply 
 *  returns the pointer (and size).
 */
Ptr SSCR_createBuf(Uns scrBucketIndex, Uns *scrSize)
{        
    Uns intrState;            // Used to disable interrupts
    Ptr tmpBuffer = NULL;     // Local used to reduce interrupt latency
    SSCR_Bucket *bucketEntry;
        
    /* Make sure valid parameters */
    UTL_assert(scrSize != NULL);
    UTL_assert(scrBucketIndex < _SSCR_bucketCnt);
    
    /* 
     *  Always initialize to zero.  If any problems in allocation or 
     *  error checking, the caller should get zero size.
     */
    *scrSize = 0;
    
    /* Get the bucket */
    bucketEntry = &(_SSCR_bucketList[scrBucketIndex]);    
    
    /* 
     *  The size for this bucket is zero.  In other words, there is nothing
     *  to allocate, so simply return a NULL pointer to indicate this.
     */
    if (bucketEntry->size == 0) {
        return (NULL);
    }

    /* 
     *  User defined bucket.  Simply return the specified size 
     *  and buffer.  
     */ 
    if (bucketEntry->count == _SSCR_USERDEFINED) {
        *scrSize = bucketEntry->size;
        return (bucketEntry->buffer);
    }    

    /*
     *  If the count is zero (e.g. not allocated),
     *  then allocate the buffer (from the heap specified in 
     *  SSCR_setup()).  Alignment has already been factored
     *  in when determining the size in SSCR_prime().
     */ 
    if (bucketEntry->count == 0) {                        
        if ((tmpBuffer = MEM_alloc(_SSCR_heapId, bucketEntry->size, 0)) == 
            MEM_ILLEGAL) {
            /* Return NULL is the allocation failed */
            return (NULL);
        }
    }

    /* 
     *  Temporarily disable interrupts. This is to prevent a very unlikely race
     *  condition, in which multiple SSCR_deleteBuf()'s, SSCR_getBuf()'s or 
     *  SSCR_createBuf()'s are called.
     */
    intrState = HWI_disable();
    
    /* Increment the use count. */
    bucketEntry->count++;

    /* 
     *  If the count is one, then set the buffer to point to the freshly 
     *  allocated block of memory.
     *  If the count is not one at this point then two things could have 
     *  happened:
     *     1. Normal calling sequence.  Simply restore the interrupts
     *     2. Multiple SSCR_createBuf()'s could have been called and a race 
     *        condition occurred.  The race condition would result in multiple
     *        MEM_alloc()'s calls.  Since only one MEM_alloc is needed, 
     *        MEM_free the unnecessary block.
     */
    if (bucketEntry->count == 1) {
        
        bucketEntry->buffer = tmpBuffer;
        
        /* Restore interrupt state */
        HWI_restore( intrState );
    }
    else {
        /* Restore interrupt state */
        HWI_restore( intrState );
        if (tmpBuffer != NULL){            
            /* 
             *  Hit a race condition.  Free this memory since it already 
             *  has been allocated.
             */ 
            MEM_free(_SSCR_heapId, tmpBuffer, bucketEntry->size);
        }
    }    
    
    *scrSize = bucketEntry->size;
    return (bucketEntry->buffer);
}

