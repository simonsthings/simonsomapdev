/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrRxSplit.h ========
 *
 */

#ifndef THRRXSPLIT_
#define THRRXSPLIT_

#include <sio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition of the structure capturing the state of the thread. */
typedef struct ThrRxSplit {
    SIO_Handle inStream;
} ThrRxSplit; 

/* prototypes for public functions */
extern Void thrRxSplitInit( Void );
extern Void thrRxSplitStartup( Void );
extern Void thrRxSplitRun( Void );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRRXSPLIT_

