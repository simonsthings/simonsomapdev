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

#include <algrf.h>
#include <utl.h>            /* debug/diagnostics utility functions */

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */
#include "thrAudioproc.h"   /* definition of thread Audioproc */
#include "thrControl.h"     /* definition of the control thread */
#include "thrRxSplit.h"     /* definition of the multiplexing thread  */
#include "thrTxJoin.h"      /* definition of the demultiplexing thread */

/* 
 *  Definition of intermediate buffers used by threads' algorithms 
 */
/* this buffer is used by the Audioproc thread */ 
Sample bufAudioproc[ FRAMELEN ];

/*
 *  ========= thrInit ========
 *  initialize all the threads that have Init() function
 */
Void thrInit( Void ) 
{
    /* 
     *  Configure the ALGRF module to tell it the names of heaps for algorithms:
     *  1st argument - name of the heap in internal memory: INTERNALHEAP
     *  2nd argument - name of the heap in external memory: EXTERNALHEAP
     */
    ALGRF_setup( INTERNALHEAP, EXTERNALHEAP ); 
    
    /* 
     *  Here we invoke specific individual initialization functions 
     *  for all the threads that have one (some of them may be empty)
     */
    thrRxSplitInit();       /* RxSplit thread   */
    thrAudioprocInit();     /* Audioproc thread */
    thrTxJoinInit();        /* TxJoin thread    */
    thrControlInit();       /* Control thread   */
    
    /* show heap usage, now that all threads are initialized */
    UTL_showHeapUsage( INTERNALHEAP );
    UTL_showHeapUsage( EXTERNALHEAP );
}

