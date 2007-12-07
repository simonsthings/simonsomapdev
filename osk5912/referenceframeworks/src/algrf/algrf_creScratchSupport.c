/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algrf_creScratchSupport.c ========
 */

#pragma CODE_SECTION(ALGRF_createScratchSupport, ".text:create")
#pragma CODE_SECTION(ALGRF_memFreeScratchSupport, ".text:create")
#pragma CODE_SECTION(memAllocScratchSupport, ".text:create")

#include <std.h>

#include <ialg.h>
#include <algrf.h>

#include <mem.h>

/*
 *  This macro allows us to use the numeric value of the pointer address
 *  for scratch buffer offset calculations, across all architectures
 */
#ifdef _55l_
#define PtrAddrAsUns(A) ((Uns)((unsigned long)(A) & 0xffff))
#else
#define PtrAddrAsUns(A) ((Uns)(A))
#endif

static Bool memAllocScratchSupport(IALG_MemRec memTab[], Int numRecs,
    Void *scratchBuf, Uns scratchSize);
     
/*
 *  ======== ALGRF_createScratchSupport ========
 *  Create an instance of an algorithm.  This API should be used by 
 *  applications that support shared scratch buffers. The address of the 
 *  scratch buffer must be supplied by the application. Its size must be
 *  >= sum of (sizes + alignments) of scratch bufs of the 'largest' algorithm
 */
ALGRF_Handle ALGRF_createScratchSupport(IALG_Fxns *fxns, IALG_Handle parent,
    IALG_Params *params, Void *scratchBuf, Uns scratchSize)
{
    IALG_MemRec memTab[ALGRF_MAXMEMRECS];       /* Stack-based avoids frag. */
    Int numRecs;
    ALGRF_Handle alg;
    IALG_Fxns *fxnsPtr;     

    if (fxns != NULL) {
        numRecs = (fxns->algNumAlloc != NULL) ?
            fxns->algNumAlloc() : IALG_DEFMEMRECS;
                 
        /* 
         *  Avoided Heap-based memTab[] due to risk of fragmentation
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
        
        if ((memAllocScratchSupport(memTab, numRecs, scratchBuf,
            scratchSize)) == TRUE) {
       
            alg = (IALG_Handle)memTab[0].base;
            alg->fxns = fxns;

            /*
             *  now that we have allocated memory for each memTab entry, 
             *  call algInit so the algorithm can initialize the memory
             */              
            if (fxns->algInit(alg, memTab, parent, params) == IALG_EOK) {                                                         
                return (alg);   /* algInit SUCCESS: return handle to algo */
            }

            /* algInit FAIL: return memTab[] desc for later memory free */
            fxns->algFree(alg, memTab);                         
        }
        
        /* Any one of many possible failures occurred. Release all memory */
        ALGRF_memFreeScratchSupport(memTab, numRecs);
    }

    return (NULL);
}

/*
 *  ======== memAllocScratchSupport ========
 *  Allocate memory for an algorithm.
 *  Do not allocate the memory if it is a request for internal
 *  scratch memory because the address of the scratch buffer is
 *  supplied by the caller.
 */
static Bool memAllocScratchSupport(IALG_MemRec memTab[], Int numRecs,
    Void *scratchBuf, Uns scratchSize)
{
    Int i;
    Char *scratchPtr = (Char *)scratchBuf;    
    
    /*
     *  algorithm standard specifies that memTab[0] must be
     *  initialized with zeros. memTab[0] is always persistent.
     */
    if ((memTab[0].base = MEM_calloc(ALGRF_memSpace(memTab[0].space),
        memTab[0].size, memTab[0].alignment)) == MEM_ILLEGAL) {
        return (FALSE);
    }    
    
    for (i = 1; i < numRecs; i++) {   /* all remaining memTab[] entries */
    
        /* Only treat as 'real' scratch if Internal, scratch. */
        if ( (memTab[i].attrs == IALG_SCRATCH) && 
        (!(IALG_isOffChip(memTab[i].space))) ) {        
                            
            Uns curScratchReq = memTab[i].size + memTab[i].alignment;       
            Int tmpOffset;
            Uns scratchOffset;

            /* Fail if current scratch req is greater than remaining scratch */
            if (curScratchReq > scratchSize) {
                return (FALSE);    
            }
                                                                    
            /*  
             *  An algorithm is permitted to request multiple scratch
             *  buffers.  For each request, set the base of the scratch memory
             *  request equal to the current pointer to the scratch buffer
             *  plus an offset to create the correct alignment.
             *  (Creates correct alignment if alignment is a power of 2.) 
             *  tmpOffset handles an align == 0, since mustnt do ~ on -ve val.
             */
            tmpOffset = (memTab[i].alignment == 0) ? 1 : memTab[i].alignment; 
            scratchOffset = PtrAddrAsUns(scratchPtr) + memTab[i].alignment;
                        
            memTab[i].base = (Void *)(scratchOffset & (~(tmpOffset - 1)));
            
            /* advance pointer to the scratch buf and adjust buffer size */
            scratchPtr += curScratchReq;
            scratchSize -= curScratchReq;            
        } 
        else {  /* 'regular' request e.g. persistent. Allocate the memory */
            if ((memTab[i].base = MEM_alloc(ALGRF_memSpace(memTab[i].space),
                memTab[i].size, memTab[i].alignment)) == MEM_ILLEGAL) {
                return (FALSE);
            }    
        }              
    }
    
    return (TRUE);
}  

/*
 *  ======== ALGRF_memFreeScratchSupport ========
 *  Free the memory allocated for an algorithm.
 *  Do not free the memory block if it is an internal scratch buffer.
 */
Void ALGRF_memFreeScratchSupport(IALG_MemRec memTab[], Int numRecs)
{
    Int i;
    
    for (i = 0; i < numRecs; i++) {   
        if ( (memTab[i].base != MEM_ILLEGAL) &&
             ( (memTab[i].attrs != IALG_SCRATCH) || 
               (IALG_isOffChip(memTab[i].space)) ) ) {   
                    
            MEM_free(ALGRF_memSpace(memTab[i].space), memTab[i].base,
                memTab[i].size);
        }       
    } 
}


  
