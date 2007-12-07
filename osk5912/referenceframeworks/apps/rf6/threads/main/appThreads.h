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

#include <msgq.h>

#include "appResources.h" // application-wide information

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Application-specific message IDs for MSGQ
 */
enum {
    CTRLMSGID = 0,
    PROCESS0_DATAMSGID,
    PROCESS1_DATAMSGID,
    PREPROCESS_DATAMSGID,
    POSTPROCESS_DATAMSGID,
    NUMMSGIDS
};
    
/*
 *  Control message data structure.  Must match exactly with 
 *  GPP-side.
 */
typedef struct CtrlMsg {
    MSGQ_MsgHeader header;    
    Uint16 cmd;                       // Message code
    Uint16 arg1;                      // First message argument
    Uint16 arg2;                      // Second message argument
} CtrlMsg;

/*
 *  LocalDataMsg are data messages exchanged between tasks in the DSP
 *  application
 */
typedef struct LocalDataMsg {
    MSGQ_MsgHeader header;    
    Sample *bufChannel[NUMCHANNELS];  //pointers to channel data buffers
} LocalDataMsg;

/*
 *  Shared scratch memory buckets: most XDAIS algorithms need scratch memory,
 *  and it can be shared among more algorithms they can never be preempted
 *  by one another. By defining "buckets" of shared scratch memory we can
 *  minimize total scratch memory requirements. When defining a cell, we
 *  set its "shared scratch bucket" to one of the values defined here.
 *  Typically, cells belonging to threads running at same priorities would
 *  go into the same scratch bucket.
 */
enum SSCRBUCKETS {
    /*
     *  Both processing threads are set at same priority, hence we can use 
     *  the same scratch bucket for all algorithms in both Process tasks.
     *  NOTE: if you DO change the priority of the Process tasks to be 
     *  different from each other, you MUST use different scratch buckets
     *  for the different Process tasks.
     */
    THRPROCESSSCRBUCKET = 0, 
    NUMSCRBUCKETS               // total number of shared scratch buckets
    };
    
// prototypes for public functions
extern Void appThreadInit( Void );

#ifdef __cplusplus
}
#endif // extern "C"

#endif // APPTHREADS_  


