/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appThreads.c ========
 *
 *  Initialization of all threads, definition of global variables
 *  and buffers
 */
#include <std.h>

/* RF5 library modules */
#include <chan.h>
#include <icc.h>
#include <scom.h>
#include <utl.h>

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */

/* invididual threads */
#include "process/thrProcess.h"
#include "rxSplit/thrRxSplit.h"
#include "txJoin/thrTxJoin.h"
#include "control/thrControl.h"

/*
 *  ======== appThreadInit ========
 *
 *  Initialize application threads.
 */
Void appThreadInit() 
{

    // initialize RF5 modules
    CHAN_init();
    ICC_init();
    SCOM_init();
    
    /* 
     *  Set up RF5 modules
     *
     *  The first three parameters to CHAN_setup are
     *  algrfInternalHeap - heap from which algrf allocates internal
     *                      non-scratch memory requested by XDAIS algorithms.
     *  algrfExternalHeap - heap from which algrf allocates external memory
     *                      requested by XDAIS algorithms.
     *  sscrInternalHeap -  heap from which sscr allocates internal scratch
     *                      memory requested by XDAIS algorithms.
     */
        
    CHAN_setup( INTERNALHEAP, EXTERNALHEAP, INTERNALHEAP, 
                SCRBUCKETS, NULL, NULL );

    // initialize tasks
    thrRxSplitInit();
    thrProcessInit();
    thrTxJoinInit();
    thrControlInit();

    thrRxSplitStartup();
    thrProcessStartup();
    thrTxJoinStartup();
    thrControlStartup();

    // show heap usage, now that all threads are initialized
    UTL_showHeapUsage( INTERNALHEAP );
    UTL_showHeapUsage( EXTERNALHEAP );
}

