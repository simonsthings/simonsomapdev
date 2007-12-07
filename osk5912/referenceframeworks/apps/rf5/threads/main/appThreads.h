/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appThreads.h ========
 *
 */
#ifndef APPTHREADS_
#define APPTHREADS_

#include <que.h>

#include "appResources.h" /* application-wide information */

#ifdef __cplusplus
extern "C" {
#endif

// SCOM buffer descriptor: list of pointers to individual channel data buffers
typedef struct ScomBufChannels {
    QUE_Elem queElem;                    // must be first field.
    Sample * bufChannel[ NUMCHANNELS ];
} ScomBufChannels;

/* control message format */
typedef struct CtrlMsg {
    LgUns cmd;                       /* Message code */
    LgUns arg1;                      /* First message argument */
    LgUns arg2;                      /* Second message argument */
} CtrlMsg;

/*
 *  Shared scratch memory buckets: most XDAIS algorithms need scratch memory,
 *  and it can be shared among more algorithms they can never be preempted
 *  by one another. By defining "buckets" of shared scratch memory we can
 *  minimize toal scratch memory requirements. When defining a cell, we
 *  set its "shared scratch bucket" to one of the values defined here.
 *  Typically, cells belonging to threads running at same priorities would
 *  go into the same scratch bucket.
 */
enum SSCRBUCKETS {
    THRPROCESSSCRBUCKET = 0, // we have only one thread with XDAIS algorithms
    SCRBUCKETS               // total number of shared scratch buckets
    };


/* prototypes for public functions */
extern Void appThreadInit( Void );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* APPTHREADS_ */ 

