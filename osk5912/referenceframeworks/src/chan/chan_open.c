/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_open.c ========
 *  Function for opening a channel.
 */

#pragma CODE_SECTION(CHAN_open, ".text:CHAN_open")

#include <std.h>

#include <algrf.h>
#include <chan.h>
#include <icell.h>
#include <sscr.h>
#include <utl.h>

/*
 *  ======== CHAN_open ========
 *  Everything is now ready to actually create each algorithm instance
 *  in the channels.  For each cell in the channel the following is done:
 *  1. The scratch buffer and its size is returned from SSCR_createBuf().  
 *     The scratch buffer will be passed into ALGRF_createScratchSuppport().
 *  2. The algorithm is created via ALGRF_createScratchSupport().
 *  3. The cell's open function is called to initialize an cell resources.
 *  If any of these steps fail, the function returns FALSE.  
 *
 *  Note: The parent instance technique is not supported by the CHAN module.
 */
Bool CHAN_open(CHAN_Handle chanHandle, ICELL_Obj cellSet[], 
               Uns cellCnt, CHAN_Attrs *chanAttrs)
{
    Int i;    
    Ptr scratchBuf;
    Uns scratchSize;
    ICELL_Handle cellHandle;

    /* Make sure parameters are valid */
    UTL_assert(chanHandle != NULL);
    UTL_assert(cellSet    != NULL);

    /* Use default attributes if none supplied */
    if (chanAttrs == NULL) {        
        chanAttrs = &CHAN_ATTRS;
    }

    chanHandle->cellSet       = cellSet;
    chanHandle->cellCnt       = cellCnt;
    chanHandle->state         = chanAttrs->state;
    chanHandle->chanControlCB = chanAttrs->chanControlCB;
    

    /* Loop through each cell in the channel. */
    for (i = 0; i < cellCnt; i++) {
            
        cellHandle = &(cellSet[i]);

        /*
         *  If the algFxns is NULL, then this cell does not contain an 
         *  algorithm.  This is not an error condition.
         */ 
        if (cellHandle->algFxns != NULL ) {
            /*
             *  Get the scratch buffer and size of the buffer.  This call will
             *  return NULL for the buffer and zero for the size if there are
             *  no on-chip scratch requirements for the specified bucket.
             *  ALGRF_createScratchSupport() will return a NULL handle if a 
             *  scratch buffer is required, but not supplied.
             */
            scratchBuf = SSCR_createBuf(cellHandle->scrBucketIndex, &scratchSize);
            
            /* 
             *  Create the algorithm using the scratch buffers returned from the 
             *  SSCR module.  
             */            
            cellHandle->algHandle = ALGRF_createScratchSupport(cellHandle->algFxns,
                                                               NULL,
                                                               cellHandle->algParams,
                                                               scratchBuf, 
                                                               scratchSize);
            if (cellHandle->algHandle == NULL) {
                return (FALSE);
            }
            UTL_showAlgMemName(cellHandle->algHandle, cellHandle->name);
        }            

        /* Call the cell's open function to initialize the cell */
        if (cellHandle->cellFxns->cellOpen != NULL) {
            if (cellHandle->cellFxns->cellOpen(cellHandle) == FALSE) {
                return (FALSE);
            }
        }        
    }

    return (TRUE);
}


