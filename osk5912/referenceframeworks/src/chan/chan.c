/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== chan.c ========
 *  Global variables for the CHAN module.
 */

#include <std.h>

#include <chan.h>

/* Default channel attributes */
CHAN_Attrs CHAN_ATTRS = {CHAN_ACTIVE,  // state
                         NULL};        // chanControlCB

