/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_setup.c ========
 */

#pragma CODE_SECTION(CHAN_setup, ".text:setup")

#include <std.h>

#include <algrf.h>
#include <sscr.h>
#include <chan.h>

/*
 *  ======== CHAN_setup ========
 *  Sets-up heap ids and the other modules that CHAN uses. 
 *
 *  algrfInternalHeap - heap from which algrf allocates internal non-scratch
 *                      memory requested by XDAIS algorithms.
 *  algrfExternalHeap - heap from which algrf allocates external memory
 *                      requested by XDAIS algorithms.
 *  sscrInternalHeap -  heap from which sscr allocates internal scratch
 *                      memory requested by XDAIS algorithms. 
 */
Bool CHAN_setup(Int algrfInternalHeap, Int algrfExternalHeap, 
                Int sscrInternalHeap, Uns bucketCnt, Ptr bucketBuf[], 
                Uns bucketSize[])
{   
    /* Set-up the ALGRF module */
    ALGRF_setup(algrfInternalHeap, algrfExternalHeap);    

    /* Set-up the SSCR module */
    return (SSCR_setup(sscrInternalHeap, bucketCnt, bucketBuf, bucketSize));
}


