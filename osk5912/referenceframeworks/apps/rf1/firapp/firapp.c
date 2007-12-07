/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== firapp.c ========
 *
 *  Caller-friendly interface for the FIR algorithm
 */
#include <std.h>

#include <algmin.h>

#include "firapp.h"

#pragma CODE_SECTION(FIR_init,          ".text:init")
#pragma CODE_SECTION(FIR_exit,          ".text:exit")


/*
 *  ======== FIR_apply ========
 *  Apply FIR algorithm to input array and place results in output array.
 */
Void FIR_apply(FIR_Handle handle, Short in[], Short out[])
{
    IALG_Handle alg = (IALG_Handle)handle;
    
    /* Initialize scratch memory buffers before processing */
    ALGMIN_activate(alg);
    
    handle->fxns->filter( handle, in, out );      /* process data */
    
    /* Save all persistent data to non-scratch memory */
    ALGMIN_deactivate(alg);    
}

/*
 *  ======== FIR_init ========
 *  Module initialization
 */
Void FIR_init()
{
}

/*
 *  ======== FIR_exit ========
 *  Module finalization
 */
Void FIR_exit()
{
}



