/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algmin_activate.c ========
 */

#pragma CODE_SECTION(ALGMIN_activate, ".text:activate")

#include <std.h>

#include <algmin.h>

/*
 *  ======== ALGMIN_activate ========
 *  Initialize scratch memory buffers before processing
 */
Void ALGMIN_activate(IALG_Handle alg)
{
    if (alg->fxns->algActivate != NULL) {
        alg->fxns->algActivate(alg);
    }
}


