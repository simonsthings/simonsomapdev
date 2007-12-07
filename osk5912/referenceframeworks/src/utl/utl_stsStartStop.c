/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_stsStartStop.c ========
 *
 *  Implementation of UTL_stsStart() and UTL_stsStop() functions.
 */
#include <std.h>
#include <clk.h>
#include <sts.h>

#include <utl.h>


/*
 *  UTL_sts*(): execution time/period/phase measurement functions
 */
 
/*
 *  ========= UTL_stsStartFunc ========
 *
 *  Start/stop procedures for measuring execution time of a segment of code. 
 *  Start also returns current time, Stop returns delta time.
 *  These functions are not called directly, but via a macro.
 */
LgUns UTL_stsStartFunc( UTLSTS_Obj *utlsts ) 
{
    LgUns hirestime;

    hirestime = CLK_gethtime();
    utlsts->lastTime = hirestime;
    STS_set( utlsts->sts, hirestime );

    return (hirestime);
}

/*
 *  ========= UTL_stsStopFunc ========
 *
 *  Start/stop procedures for measuring execution time of a segment of code. 
 *  Start also returns current time, Stop returns delta time.
 */
LgUns UTL_stsStopFunc( UTLSTS_Obj *utlsts ) 
{
    LgUns hirestime = CLK_gethtime(), delta;

    STS_delta( utlsts->sts, hirestime );
    delta = hirestime - utlsts->lastTime;
    utlsts->lastTime = 0;

    return (delta);
}


