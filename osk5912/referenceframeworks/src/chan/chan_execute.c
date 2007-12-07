/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_execute.c ========
 *  The execute function for the CHAN module.
 */

#pragma CODE_SECTION(CHAN_execute, ".text:CHAN_execute")

#include <std.h>

#include <icell.h>
#include <chan.h>
#include <utl.h>

/*
 *  ======== CHAN_execute ========
 *  CHAN module's execution function.  This function executes each cell 
 *  in the channel. The cellExecute() will call the algorithm's runtime
 *  functions.
 *  arg usage is left to the discretion of the user of this module.
 */
Bool CHAN_execute(CHAN_Handle  chanHandle, Arg arg) 
{
    Int i;        
    ICELL_Handle cellHandle;

    /* Make sure parameters are valid */
    UTL_assert(chanHandle != NULL);

    /* If the channel is not active, do not execute it */
    if (chanHandle->state != CHAN_ACTIVE) {
        return (TRUE);
    }

    /* Call the chanControl callback if present */
    if (chanHandle->chanControlCB != NULL) {
        if (chanHandle->chanControlCB(chanHandle) == FALSE) {
            return (FALSE);
        }
    }
    
    /*
     *  Loops through each cell in the channel and calls the 
     *  cell's execute function.
     */ 
    for (i = 0; i < chanHandle->cellCnt; i++) {
        
        cellHandle = &(chanHandle->cellSet[i]);
        
         /* Execute the cell */
        if (cellHandle->cellFxns->cellExecute(cellHandle, arg) == FALSE) {
            return (FALSE);
        }
    }    
    
    return (TRUE);
}

