/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algmin_new.c ========
 *
 *  Instantiate a 'static' XDAIS algorithm.
 *  Very low overhead method of bringing up an algorithm. No memory heap
 *  usage, no buffer allocations. Initializes algorithm to use buffers
 *  whose alignment, size etc have been preconfigured to match algorithm needs
 */
#include <std.h>

#include <algmin.h>

#pragma CODE_SECTION(ALGMIN_new,           ".text:init")


/*
 *  ======== ALGMIN_new ========
 */
IALG_Handle ALGMIN_new(IALG_Fxns *fxns, IALG_Params *params, 
        Char *algChanBufs[], SmUns numAlgChanBufs)
{
    /* Use a large enough Mem Tab structure for any algorithm */
    IALG_MemRec memTab[ALGMIN_MAXMEMRECS];  
    IALG_Handle alg;
    Int i;
                            
    /*  Initialize algo memory with preconfigured static buffer addresses */
    for (i=0; i<numAlgChanBufs; i++)    /* loop thro num memTab's of algo */
    {
        memTab[i].base = algChanBufs[i];
    }
    
    /* bind handle to algorithm's instance object and function table */
    alg = memTab[0].base;
    alg->fxns = fxns;  
    
    if (fxns->algInit(alg, memTab, NULL, params) == IALG_EOK) {
        return (alg);   /* algInit SUCCESS: return handle to algo */
    }
    else {            
        return (NULL);  /* algInit FAILURE: return NULL */
    }
}

 
