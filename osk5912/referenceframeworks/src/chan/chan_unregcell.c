/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_unregcell.c ========
 *  Cell registration.
 */

#pragma CODE_SECTION(CHAN_unregCell, ".text:CHAN_unregCell")

#include <std.h>

#include <mem.h>

#include <icell.h>
#include <chan.h>
#include <icc.h>
#include <utl.h>

/*
 *  ======== CHAN_unregCell ========
 *  This function frees the memory allocated in CHAN_regCell.
 */
Void CHAN_unregCell(ICELL_Handle cellHandle)
{
    /* Make sure parameters are valid */
    UTL_assert(cellHandle != NULL);    

    // Free the input array
    if (cellHandle->inputIcc != NULL) {
        MEM_free(0, cellHandle->inputIcc, 
                 sizeof(ICC_Handle) * cellHandle->inputIccCnt);
    }

    // Free the output array
    if (cellHandle->outputIcc != NULL) {
        MEM_free(0, cellHandle->outputIcc, 
                 sizeof(ICC_Handle) * cellHandle->outputIccCnt); 
    }
   
    return;
}

