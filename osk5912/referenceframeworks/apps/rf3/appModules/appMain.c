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
 *
 *  Startup module for application initialization: first initializes the
 *  I/O, then initializes all the threads, and primes the I/O pipes.
 *  Any application-specific module initialization should be called from here.
 */

#include <std.h>

#include <utl.h>            /* debug/diagnostics utility functions */

#include "appIO.h"          /* application IO initialization and priming */
#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info (thrInit() decl.) */

/*
 *  ======== main ========
 *  main() initializes IO, threads, and anything else the application
 *  might need to initialize
 */
Void main()
{   
    /* name LOG objects to be used for error/warning/general/debug messages */
    UTL_setLogs( &logTrace, &logTrace, &logTrace, &logTrace );
    
    /* initialize I/O */
    appIOInit();

    /* initialize all the threads */
    thrInit();          

    /* prime I/O */
    appIOPrime();

    /* greet the user */
    UTL_logDebug( "Application started." );

    /* and fall into BIOS idle loop */
    return;                 
}

