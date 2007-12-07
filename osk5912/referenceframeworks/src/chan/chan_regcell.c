/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_regcell.c ========
 *  Cell registration.
 */

#pragma CODE_SECTION(CHAN_regCell, ".text:CHAN_regCell")

#include <std.h>

#include <mem.h>

#include <icell.h>
#include <sscr.h>
#include <chan.h>
#include <icc.h>
#include <utl.h>

/*
 *  ======== CHAN_regCell ========
 *  This function calls SSCR_prime() to determine the scratch overlays. 
 *  It also sets-up the input and output ICC objects.
 */
Bool CHAN_regCell(ICELL_Handle cellHandle, 
                  ICC_Handle iccIn[], Uns iccInCnt,
                  ICC_Handle iccOut[],Uns iccOutCnt)
{
    Int i;
    
    /* Make sure parameters are valid */
    UTL_assert(cellHandle != NULL);    
    
    /*
     *  SSCR_prime() will be called to determine worst-case scratch 
     *  requirements if an algorithm is specified.
     */
    if (cellHandle->algFxns != NULL) {
        if (SSCR_prime(cellHandle->scrBucketIndex,
                       cellHandle->algFxns,
                       cellHandle->algParams) == FALSE) {
            return (FALSE);
        }
    }    
    
    /* 
     *  Set up the input ICC Object list.  This includes allocating the space
     *  to store the pointers, copying the pointers into the allocated space,
     *  and setting the count field in the cell.
     */
    cellHandle->inputIccCnt = iccInCnt;
    cellHandle->inputIcc    = NULL;
    if ( iccIn != NULL ) {
        
        cellHandle->inputIcc = MEM_alloc(0, sizeof(ICC_Handle) * iccInCnt, 0); 
        if (cellHandle->inputIcc == MEM_ILLEGAL) {
            return (FALSE);
        }
        
        for (i = 0; i < iccInCnt; i++) { 
            cellHandle->inputIcc[i] = iccIn[i]; 
        }
    }    

    /* Set up the output ICC Object list in the same manner as the input list */
    cellHandle->outputIccCnt = iccOutCnt;
    cellHandle->outputIcc    = NULL;
    if ( iccOut != NULL ) {
        
        cellHandle->outputIcc = MEM_alloc(0, sizeof(ICC_Handle) * iccOutCnt, 0); 
        if (cellHandle->outputIcc == MEM_ILLEGAL) {
            return (FALSE);
        }

        for (i = 0; i < iccOutCnt; i++) { 
            cellHandle->outputIcc[i] = iccOut[i]; 
        }
    }    
   
    return (TRUE);
}

