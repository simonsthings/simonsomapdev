/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_stsPeriod.c ========
 *
 *  Implementation of the utl_stsPeriod function.
 */
#include <std.h>
#include <clk.h>
#include <sts.h>

#include <utl.h>


/*
 *  UTL_sts*(): execution time/period/phase measurement functions
 */
 
/*
 *  ========= UTL_stsPeriodFunc ========
 *
 *  Measures delta time between two calls and also returns the delta.
 */
LgUns UTL_stsPeriodFunc( UTLSTS_Obj *utlsts ) 
{
    LgUns hirestime = CLK_gethtime(), delta;

    if (utlsts->count == 0) {
        /* first time this one is ever used */
        utlsts->lastTime = hirestime;
        STS_set( utlsts->sts, hirestime );
        delta = 0;
    } else {
        delta = hirestime - utlsts->lastTime;
        STS_delta( utlsts->sts, hirestime );
        utlsts->lastTime = hirestime;
    }
    utlsts->count++;

    return (delta);
}


