/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_attrs.c ========
 *  Attribute management file.
 */

#pragma CODE_SECTION(CHAN_setAttrs, ".text:CHAN_setAttrs")
#pragma CODE_SECTION(CHAN_getAttrs, ".text:CHAN_getAttrs")

#include <std.h>

#include <chan.h>
#include <utl.h>

/*
 *  ======== CHAN_setAttrs ========
 *  Set the attributes of the channel.  This function can be
 *  called during runtime execution of the application.
 */
Void CHAN_setAttrs(CHAN_Handle chanHandle, CHAN_Attrs *chanAttrs) 
{
    /* Make sure parameters are valid */
    UTL_assert(chanAttrs != NULL);
    UTL_assert(chanHandle != NULL);
    
    /* Update the read/write fields of the channel. */
    chanHandle->state         = chanAttrs->state;
    chanHandle->chanControlCB = chanAttrs->chanControlCB;
}

/*
 *  ======== CHAN_getAttrs ========
 *  Return the attributes of the specified channel into a supplied
 *  CHAN_Attrs structure.
 */
Void CHAN_getAttrs(CHAN_Handle chanHandle, CHAN_Attrs *chanAttrs) 
{
    /* Make sure parameters are valid */
    UTL_assert(chanAttrs != NULL);
    UTL_assert(chanHandle != NULL);    
    
    chanAttrs->state         = chanHandle->state;
    chanAttrs->chanControlCB = chanHandle->chanControlCB;
}


