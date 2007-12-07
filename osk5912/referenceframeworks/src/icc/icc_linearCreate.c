/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== icc_linearCreate.c ========
 *  Functions for the creation of the linear ICC object.
 */

#pragma CODE_SECTION(ICC_linearCreate, ".text:ICC_linearCreate")

#include <std.h>

#include <mem.h>

#include <icc_linear.h>

/*
 *  ======== ICC_linearCreate ========
 *  Allocate the linear ICC object.  Then it fills in the appropriate fields.
 *  The object is always created from the segment that contains 
 *  DSP/BIOS objects. 
 *  If the memory allocation fails, return NULL.
 */
ICC_LinearHandle ICC_linearCreate(Ptr buffer, Uns nmaus)
{
    ICC_LinearHandle newLinearIcc;
    
    /* Allocate the linear ICC object from segment for DSP/BIOS Objects */
    newLinearIcc = (ICC_LinearHandle)MEM_alloc(0, sizeof(ICC_LinearObj), 0);
    if (newLinearIcc == MEM_ILLEGAL) {
        return(NULL);
    }
        
    /* Initialize all the fields of the ICC */    
    newLinearIcc->obj.buffer  = buffer;
    newLinearIcc->obj.nmaus   = nmaus;
    newLinearIcc->obj.objType = ICC_LINEAROBJ;
    
    return (newLinearIcc);
}



