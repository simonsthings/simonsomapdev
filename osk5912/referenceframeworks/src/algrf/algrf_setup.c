/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algrf_setup.c ========
 */

#pragma CODE_SECTION(ALGRF_setup, ".text:setup")

#include <std.h>

#include <atm.h>

#include <algrf.h>

ALGRF_Config ALGRF_config = {
        0,  /* default segment for Internal Heap */
        0,  /* default segment for External Heap */
};

ALGRF_Config* ALGRF = &ALGRF_config;

/*
 *  ======== ALGRF_setup ========
 *  Configure the system to use the selected heaps.
 *  
 */
Void ALGRF_setup(Int internalHeap, Int externalHeap)
{
    static Int firstTime = TRUE;

    /* Only set the first time. */
    if (ATM_seti(&firstTime, FALSE) == TRUE) {
        ALGRF->INTHEAP = internalHeap;
        ALGRF->EXTHEAP = externalHeap;
    }
}



