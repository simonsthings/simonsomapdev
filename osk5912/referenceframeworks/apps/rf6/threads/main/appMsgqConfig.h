/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appMsgqConfig.h ========
 *
 *  This file contains the static configuration of the MSGQ module
 *  for the MSGQ examples.
 *  The configuration is a superset of the requirements for each 
 *  example.
 */
#ifndef appMsgqConfig_
#define appMsgqConfig_

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *  The number of local message queues. 
 *  This currently corresponds to the number of reader tasks in the 
 *  system, since there is one receiving queue per task. 
 */
#define NUMMSGQUEUES   7   

/*
 *  The maximum number of remote message queues supported by the 
 *  remote transport
 */ 
#define MAXNUMREMOTEMSGQS 1

/* 
 *  Number of message pools for each BUF allocator. 
 *  This currently corresponds to the number of types of msgs in the
 *  system. Note that DSP/BIOS Link's remote transport requires one
 *  pool for internal use.
 */
#define NUMLOCALMSGPOOLS     1
#define NUMREMOTEMSGPOOLS    2  

/* 
 *  Number of messages in the different pools. These have been 
 *  arbitrarily estimated, and should be optimized depending on
 *  the actual end-application
 */
#define NUMMQTCTRLMSGS 2    // # msgs Link internal use by allocator
#define NUMCTRLMSGS    4    // # GPP->DSP ctrl messages potentially queued up
#define NUMDATAMSGS    4    // # MSGQ_alloc()'ed data (intra-thread) messages 

/* The allocator id and the number of allocators. */
enum {
    REMOTEMQABUFID = 0, // The remote allocator id must be 0 to match GPP side
    LOCALMQABUFID,
    NUMALLOCATORS
};

/* The transport id and the number of transports. */
enum {
    LOCALMQTBIOSQUEID = 0,
    REMOTEMQTID,
    NUMTRANSPORTS
};

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* appMsgqConfig_ */

