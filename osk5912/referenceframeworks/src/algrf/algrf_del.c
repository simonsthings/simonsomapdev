/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algrf_del.c ========
 */

#pragma CODE_SECTION(ALGRF_delete, ".text:delete")

#include <std.h>

#include <ialg.h>
#include <algrf.h>


/*
 *  ======== ALGRF_delete ========
 *  Delete an instance of an algorithm
 */
Bool ALGRF_delete(ALGRF_Handle alg)
{
    IALG_MemRec memTab[ALGRF_MAXMEMRECS];       /* Stack-based avoids frag. */
    Int numRecs;
    IALG_Fxns *fxns;
    
    if (alg == NULL || alg->fxns == NULL) {
        return (FALSE);
    }

    fxns = alg->fxns;
    numRecs = (fxns->algNumAlloc != NULL) ?
        fxns->algNumAlloc() : IALG_DEFMEMRECS;

    /* 
     *  Heap-based memTab[] avoided due to risk of fragmentation
     *  A maximum number of memRecs is set. If, in the unlikely case,
     *  more memRecs than max are requested, return failure.
     */
    if (numRecs > ALGRF_MAXMEMRECS) {
        return (FALSE);
    }  
    
    memTab[0].base = alg;

    /*
     *  query the algorithm about its memory allocations, and
     *  then free the memory allocated to the algorithm
     */
    numRecs = fxns->algFree(alg, memTab);
    ALGRF_memFree(memTab, numRecs);
    
    return (TRUE);
}


