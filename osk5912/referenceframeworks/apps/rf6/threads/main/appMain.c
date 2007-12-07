/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appMain.c ========
 */

#include <std.h>

// RF library includes
#include <utl.h>

// App includes
#include "appResources.h"
#include "appThreads.h"
#include "appIO.h"


/*
 *  ======== main ========
 *
 *  Main application function.
 */
Int main(Int argc, Char *argv[])
{
    // name LOG objects to be used for error/warning/general/debug messages.
    UTL_setLogs( &logTrace, &logTrace, &logTrace, &logTrace );

    appIOInit();
    appThreadInit();

    UTL_logDebug( "Application started." );
    
    return (0);
}

