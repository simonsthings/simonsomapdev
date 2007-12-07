/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_stsReset.c ========
 *
 *  Implementation of UTL_stsReset() function.
 */
#include <std.h>
#include <sts.h>

#include <utl.h>

/*
 *  UTL_sts*(): execution time/period/phase measurement functions
 */

/*
 *  ========= UTL_stsResetFunc ========
 *
 *  Resets the STS object. 
 */
Void UTL_stsResetFunc( UTLSTS_Obj *utlsts ) 
{
    STS_reset( utlsts->sts );
    utlsts->lastTime = 0;
    utlsts->count = 0;
}



