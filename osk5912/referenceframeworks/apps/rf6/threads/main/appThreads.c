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
#include <string.h>
#include <mqabuf.h>
#include <mqtbiosque.h>
#include <mqtdsplink.h>

// DSP/BIOS includes
#include <msgq.h>

// RF module includes
#include <chan.h>
#include <icc.h>
#include <utl.h>

// App includes
#include "appResources.h"   // application-wide common info
#include "appThreads.h"     // thread-wide common info

// Thread includes 
#include "process0/thrProcess0.h"
#include "process1/thrProcess1.h"
#include "preProcess/thrPreProcess.h"
#include "postProcess/thrPostProcess.h"
#include "control/thrControl.h"

/*
 *  Priming buffers
 *  Double buffering is set by default to meet real-time in the supplied
 *  RF6 application. Depending on the load on the GPP side, users might 
 *  need to increase the amount of buffering in their adaptation(s). Note
 *  that this could require the user to change the max number of pending IO
 *  supported in the DSP/BIOS Link driver.
 */
#define NUMPRECODECBUFS     2
#define NUMPRELINKBUFS      2
#define NUMPOSTCODECBUFS    2
#define NUMPOSTLINKBUFS     2

Sample bufPreProcessCodec [ NUMPRECODECBUFS  ] [ NUMCODECCHANS * FRAMELEN ];
Sample bufPreProcessLink  [ NUMPRELINKBUFS   ] [ NUMCHANNELS   * FRAMELEN ];
Sample bufPostProcessCodec[ NUMPOSTCODECBUFS ] [ NUMCODECCHANS * FRAMELEN ];
Sample bufPostProcessLink [ NUMPOSTLINKBUFS  ] [ NUMCHANNELS   * FRAMELEN ];

/*
 *  Used to pass the above buffers to the thread startup functions.
 */
static Sample *bufRxCodec[ NUMPRECODECBUFS  ];
static Sample *bufRxLink [ NUMPRELINKBUFS   ];
static Sample *bufTxCodec[ NUMPOSTCODECBUFS ];
static Sample *bufTxLink [ NUMPOSTLINKBUFS  ];

/*
 *  Thread objects which encapsulate the state information 
 *  of the thread.  Note not all the fields in the following
 *  structures are being initialized here.  They will be set up in the 
 *  thrXXXinit functions instead.
 */
ThrPreProcess thrPreProcessCodec = {
        "/dioCodec",                     // Name of input stream 
        "msgqPreProcessCodec",           // receiving MSGQ name
        "msgqProcess0",                  // transmitting MSGQ name
        NUMPRECODECBUFS,                 // number of buffers used for priming
        NUMCODECCHANS,                   // number of channels from device
};

ThrPostProcess thrPostProcessLink = {
        "/dio_dsplink1",                 // Name of output stream 
        "msgqPostProcessLink",           // receiving MSGQ name
        "msgqProcess0",                  // transmitting MSGQ name
        NUMPOSTLINKBUFS,                 // number of buffers used for priming
        NUMCHANNELS,                     // number of channels to device
};

ThrPreProcess thrPreProcessLink = {
        "/dio_dsplink0",                 // Name of input stream 
        "msgqPreProcessLink",            // receiving MSGQ name
        "msgqProcess1",                  // transmitting MSGQ name
        NUMPRELINKBUFS,                  // number of buffers used for priming
        NUMCHANNELS,                     // number of channels from device
};

ThrPostProcess thrPostProcessCodec = {
        "/dioCodec",                     // Name of output stream 
        "msgqPostProcessCodec",          // receiving MSGQ name
        "msgqProcess1",                  // transmitting MSGQ name
        NUMPOSTCODECBUFS,                // number of buffers used for priming
        NUMCODECCHANS,                   // number of channels to device
};


/*
 *  ======== appThreadInit ========
 *
 *  Initialize application threads.
 */
Void appThreadInit() 
{
    Uns i;

    /* Initialize the allocator and transport */
    MQABUF_init();
    MQTBIOSQUE_init();
    MQTDSPLINK_init();
    
    /* Initialize the message queue module */
    MSGQ_init();
        
    // initialize RF modules
    CHAN_init();
    ICC_init();
    
    /* 
     *  Set up RF modules
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
                NUMSCRBUCKETS, NULL, NULL );

    // Clear priming buffers to output silences
    memset( bufPostProcessCodec, 0, sizeof( bufPostProcessCodec ) );
    memset( bufPostProcessLink,  0, sizeof( bufPostProcessLink  ) );

    /*
     * The two data paths are set up as follows:
     *           .------------.      .----------.      .-------------.
     *  Codec -->| Preprocess | ===> | Process0 | ===> | Postprocess |--> Link
     *           `------------'      `----------'      `-------------'
     *           .------------.      .----------.      .-------------.
     *  Link  -->| Preprocess | ===> | Process1 | ===> | Postprocess |--> Codec
     *           `------------'      `----------'      `-------------'
     *
     */

    /*
     *  Initialize the tasks
     */
    thrPreProcessInit( &thrPreProcessCodec );
    thrProcess0Init();
    thrPostProcessInit( &thrPostProcessLink );

    thrPreProcessInit( &thrPreProcessLink );
    thrProcess1Init();
    thrPostProcessInit( &thrPostProcessCodec );

    thrControlInit();

    /*
     *  Set-up buffer pointers for the thread startups
     */    
    for ( i = 0; i < NUMPRECODECBUFS; i++ ) {
        bufRxCodec[i] = bufPreProcessCodec[i];
    }
    
    for ( i = 0; i < NUMPOSTLINKBUFS; i++ ) {
        bufTxLink[i] = bufPostProcessLink[i];
    }     
    
    for ( i = 0; i < NUMPRELINKBUFS; i++ ) {
        bufRxLink[i] = bufPreProcessLink[i];
    }
    
    for ( i = 0; i < NUMPOSTCODECBUFS; i++ ) {
        bufTxCodec[i] = bufPostProcessCodec[i];
    }

    /*
     *  Startup the tasks
     */
    thrPreProcessStartup( &thrPreProcessCodec, bufRxCodec );
    thrProcess0Startup();    
    thrPostProcessStartup( &thrPostProcessLink, bufTxLink );

    thrPreProcessStartup( &thrPreProcessLink, bufRxLink );
    thrProcess1Startup();    
    thrPostProcessStartup( &thrPostProcessCodec, bufTxCodec );
    
    thrControlStartup();

    // show heap usage, now that all threads are initialized
    UTL_showHeapUsage( INTERNALHEAP );
    UTL_showHeapUsage( EXTERNALHEAP );
}


