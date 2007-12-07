/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_stsPhase.c ========
 *
 *  Implementation of UTL_phase() function.
 */
#include <std.h>
#include <sts.h>

#include <utl.h>

/*
 *  UTL_sts*(): execution time/period/phase measurement functions
 */
 
/*
 *  ========= UTL_stsPhaseFunc ========
 *
 *  Stores absolute difference between the two last timestamps for two
 *  STS objects in the third STS object, if they have been called the
 *  same number of times, and returns the difference.
 */
LgUns UTL_stsPhaseFunc( UTLSTS_Obj *utlstsSrc1, UTLSTS_Obj *utlstsSrc2, 
                        UTLSTS_Obj *utlstsDst ) 
{
    LgInt phase;

    if (utlstsSrc1->count != utlstsSrc2->count) {
        return 0;
    }
    phase = utlstsSrc1->lastTime - utlstsSrc2->lastTime;
    if (phase < 0) {
        phase = -phase;
    }
    STS_add( utlstsDst->sts, phase );

    return ((LgUns)phase);
}



