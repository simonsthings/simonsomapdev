/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algrf_activate.c ========
 */

#pragma CODE_SECTION(ALGRF_activate, ".text:activate")

#include <std.h>

#include <algrf.h>

/*
 *  ======== ALGRF_activate ========
 *  Initialize scratch memory buffers before processing
 */
Void ALGRF_activate(ALGRF_Handle alg)
{
    if (alg->fxns->algActivate != NULL) {
            alg->fxns->algActivate(alg);
    }
}


