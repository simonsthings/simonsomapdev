/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_init.c ========
 *  Init function for CHAN module.
 */

#pragma CODE_SECTION(CHAN_init, ".text:init")

#include <std.h>

#include <chan.h>
#include <sscr.h>
#include <algrf.h>


/*
 *  ======== CHAN_init ========
 *  Initialize the other modules that CHAN uses. 
 */
Void CHAN_init(Void) 
{
    /* Initialize the ALGRF module. */
    ALGRF_init();

    /* Initialize the SSCR module. */
    SSCR_init();
}

