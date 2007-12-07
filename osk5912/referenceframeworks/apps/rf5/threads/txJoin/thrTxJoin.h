/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrTxJoin.h ========
 *
 */

#ifndef THRTXJOIN_
#define THRTXJOIN_

#include <sio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition of the structure capturing the state of the thread. */
typedef struct ThrTxJoin {
    SIO_Handle outStream;    
} ThrTxJoin;

/* prototypes for public functions */
extern Void thrTxJoinInit( Void );
extern Void thrTxJoinStartup( Void );
extern Void thrTxJoinRun( Void );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRTXJOIN_

