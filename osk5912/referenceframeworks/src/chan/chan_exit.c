/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan_exit.c ========
 *  Exit function for CHAN module.
 */

#pragma CODE_SECTION(CHAN_exit, ".text:exit")

#include <std.h>

#include <chan.h>
#include <sscr.h>
#include <algrf.h>


/*
 *  ======== CHAN_exit ========
 *  Exit function for CHAN module.
 */
Void CHAN_exit(Void)
{    
    /* Exit the ALGRF module. */
    ALGRF_exit();

    /* Exit the SSCR module. */
    SSCR_exit();
}

