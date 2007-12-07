/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algrf_cre.c ========
 */
 
#pragma CODE_SECTION(ALGRF_create, ".text:create")
#pragma CODE_SECTION(ALGRF_memFree, ".text:create" )
#pragma CODE_SECTION(memAlloc, ".text:create")

#include <std.h>

#include <ialg.h>
#include <algrf.h>

#include <mem.h>

static Bool memAlloc(IALG_MemRec memTab[], Int numRecs);

/*
 *  ======== ALGRF_create ========
 *  Create an instance of an algorithm
 */
ALGRF_Handle ALGRF_create(IALG_Fxns *fxns, IALG_Handle parent,
    IALG_Params *params)
{
    IALG_MemRec memTab[ALGRF_MAXMEMRECS];       /* Stack-based avoids frag. */
    Int numRecs;
    ALGRF_Handle alg;
    IALG_Fxns *fxnsPtr;

    if (fxns != NULL) {
        numRecs = (fxns->algNumAlloc != NULL) ?
            fxns->algNumAlloc() : IALG_DEFMEMRECS;
        
        /* 
         *  Heap-based memTab[] avoided due to risk of fragmentation
         *  A maximum number of memRecs is set. If, in the unlikely case,
         *  more memRecs than max are requested, return failure.
         */
        if (numRecs > ALGRF_MAXMEMRECS) {
            return (NULL);
        }  
                                                   
        /* query the algorithm about its memory requirements */
        numRecs = fxns->algAlloc(params, &fxnsPtr, memTab);

        if (numRecs <= 0 ) { 
            return (NULL);
        }
            
        if ((memAlloc(memTab, numRecs)) == TRUE) {
            alg = (IALG_Handle)memTab[0].base;
            alg->fxns = fxns;

            /*
             *  now that memAlloc has allocated memory for each memory
             *  block that the algorithm requested, call algInit so the
             *  algorithm can initialize the memory
             */
            if (fxns->algInit(alg, memTab, parent, params) == IALG_EOK) {
                return (alg);   /* algInit SUCCESS: return handle to algo */
            }

            /* algInit FAIL: return memTab[] desc for later memory free */
            fxns->algFree(alg, memTab);                         
        }
        
        /* Any one of many possible failures occurred. Release all memory */
        ALGRF_memFree(memTab, numRecs);
    }

    return (NULL);
}

/*
 *  ======== ALGRF_memFree ========
 *  Free the memory allocated for an algorithm
 */
Void ALGRF_memFree(IALG_MemRec memTab[], Int numRecs)
{
    Int i;
    
    for (i = 0; i < numRecs; i++) {
        if (memTab[i].base != MEM_ILLEGAL) {   
            MEM_free(ALGRF_memSpace(memTab[i].space), memTab[i].base,
                memTab[i].size);
        }
    } 
}

/*
 *  ======== memAlloc ========
 *  Allocate memory for an algorithm
 */
static Bool memAlloc(IALG_MemRec memTab[], Int numRecs)
{
    Int i;
    
    /*
     *  algorithm standard specifies that memTab[0] must be
     *  initialized with zeros
     */
    if ((memTab[0].base = MEM_calloc(ALGRF_memSpace(memTab[0].space),
        memTab[0].size, memTab[0].alignment)) == MEM_ILLEGAL) {
        return (FALSE);
    }
    
    for (i = 1; i < numRecs; i++) {     /* all remaining memTab[] entries */     
        if ((memTab[i].base = MEM_alloc(ALGRF_memSpace(memTab[i].space),
            memTab[i].size, memTab[i].alignment)) == MEM_ILLEGAL) {
            return (FALSE);
        }    
    }
    
    return (TRUE);
} 


