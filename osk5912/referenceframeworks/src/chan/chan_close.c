/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_close.c ========
 *  Function for closing a channel.
 */

#pragma CODE_SECTION(CHAN_close, ".text:CHAN_close")

#include <std.h>

#include <algrf.h>
#include <chan.h>
#include <icell.h>
#include <sscr.h>
#include <utl.h>

/*
 *  ======== CHAN_close ========
 *  This function closes a channel. Closing a channel means doing the
 *  following for each cell in the channel.
 *  1. Call the cellClose() function to close the cell's resources.
 *  2. Call ALGRF_deleteScratchSupport() to free the algorithm's memory. 
 *  3. Call SSCR_deleteBuf() to free the overlaid scratch memory.  Note:
 *     this may not actually free the memory if other algorithms are still
 *     using the memory.
 *  If any of these steps fail, the function returns FALSE.
 */
Bool CHAN_close(CHAN_Handle chanHandle)
{
    Int i;    
    ICELL_Handle cellHandle;

    /* Make sure parameters are valid */
    UTL_assert(chanHandle != NULL);
    
    /* Loop through each cell in the channel */        
    for (i = 0; i < chanHandle->cellCnt; i++) {
        
        cellHandle = &(chanHandle->cellSet[i]);

        /* 
         *  Close the cell if the function is one defined.  The cellClose 
         *  will release the cell resources.  This does not delete the algorithm.
         */
        if (cellHandle->cellFxns->cellClose != NULL) {
            if (cellHandle->cellFxns->cellClose(cellHandle) == FALSE) {
                return (FALSE);
            }
        }
        
        if (cellHandle->algHandle != NULL) {
            
            /* Delete the algorithm. */         
            if (ALGRF_deleteScratchSupport(cellHandle->algHandle) == FALSE) {
                return (FALSE);
            }       
            
            /* Release the scratch buffers */
            if (SSCR_deleteBuf(cellHandle->scrBucketIndex) == FALSE) {
                return (FALSE);
            }
        }
    }

    return (TRUE);
}


