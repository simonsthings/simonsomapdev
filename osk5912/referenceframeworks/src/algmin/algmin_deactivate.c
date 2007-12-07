/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algmin_deactivate.c ========
 */

#pragma CODE_SECTION(ALGMIN_deactivate, ".text:deactivate")

#include <std.h>

#include <algmin.h>


/*
 *  ======== ALGMIN_deactivate ========
 *  Save all persistent data to non-scratch memory
 */
Void ALGMIN_deactivate(IALG_Handle alg)
{
    if (alg->fxns->algDeactivate != NULL) {
        alg->fxns->algDeactivate(alg);
    } 
}


