/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== sscr_deletebuf.c ========
 *  SSCR's file for deleting the scratch buffers.
 */

#pragma CODE_SECTION(SSCR_deleteBuf, ".text:SSCR_deleteBuf")

#include <std.h>

#include <hwi.h>
#include <mem.h>

#include <sscr.h>
#include <utl.h>


/*
 *  ======== SSCR_deleteBuf ========
 *  This function frees the buffer for the specified bucket if the usage
 *  count goes to 0.  Note: if the user specified the bucket buffer, the buffer
 *  is not freed.
 *
 *  When a buffer is freed,  
 *   - The buffer pointer in the bucket is set to NULL (not required,
 *     but aids in debugging). 
 *
 */
Bool SSCR_deleteBuf(Uns scrBucketIndex)
{
    Uns intrState;            // Used to disable interrupts
    Ptr tmpBuffer;            // Local used to reduce interrupt latency
    Uns tmpSize;              // Local used to reduce interrupt latency
    SSCR_Bucket *bucketEntry;

    /* Make sure a valid parameter */
    UTL_assert(scrBucketIndex < _SSCR_bucketCnt);
        
    bucketEntry = &(_SSCR_bucketList[scrBucketIndex]);    

    /* 
     *  Temporarily disable interrupts. This is to prevent a very unlikely race
     *  condition, in which multiple SSCR_deleteBuf()'s, SSCR_getBuf()'s  or 
     *  SSCR_createBuf()'s are called.
     */
    intrState = HWI_disable();

    /*
     *  If the buffer is user defined or the count is zero, do not free
     *  the buffer. If the size of the scratch for this bucket is zero, 
     *  then nothing should have been allocated. So simply return.
     */ 
    if ((bucketEntry->count == _SSCR_USERDEFINED)  || 
        (bucketEntry->count == 0)                  || 
        (bucketEntry->size  == 0)) {

        /* Restore interrupt state */
        HWI_restore( intrState );
        return (TRUE);
    }    

    /* 
     *  Decrement use count. The above check makes sure that it is 
     *  okay to decrement here.
     */
    bucketEntry->count--;    
    
    /* If the use count is zero, delete the memory. */
    if (bucketEntry->count == 0) {      

        /* 
         *  To minimize the time interrupts are disabled, use local 
         *  variables to store information needed for the MEM_free.
         */
        tmpBuffer = bucketEntry->buffer;
        tmpSize   = bucketEntry->size;

        /* Reset the buffer. */
        bucketEntry->buffer = NULL;

        /* Restore interrupt state */
        HWI_restore( intrState );

        /* Free the memory and reset the bucket. */
        if (MEM_free(_SSCR_heapId, tmpBuffer, tmpSize) == FALSE) {
            /* Free failure.  */
            return (FALSE);
        }
    }
    else {
        /* Restore interrupt state */
        HWI_restore( intrState );
    }
    
    return (TRUE);
}

