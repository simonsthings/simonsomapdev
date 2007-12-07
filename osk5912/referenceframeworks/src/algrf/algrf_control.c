/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algrf_control.c ========
 */

#pragma CODE_SECTION(ALGRF_control, ".text:control")

#include <std.h>

#include <ialg.h>
#include <algrf.h>

/*
 *  ======== ALGRF_control ========
 *  Algorithm specific control and status
 */
Int ALGRF_control(ALGRF_Handle alg, IALG_Cmd cmd, IALG_Status *statusPtr)
{
    if (alg && alg->fxns->algControl) {
        return (alg->fxns->algControl(alg, cmd, statusPtr));
    }

    return (IALG_EFAIL);
}


