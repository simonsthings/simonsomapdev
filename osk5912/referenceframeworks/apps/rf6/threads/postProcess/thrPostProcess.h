/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrPostProcess.h ========
 *
 */

#ifndef THRPOSTPROCESS_
#define THRPOSTPROCESS_

// DSP/BIOS module includes
#include <sio.h>
#include <sem.h>
#include <msgq.h>

#include "appThreads.h"   // application-wide common info

#ifdef __cplusplus
extern "C" {
#endif

/* Definition of the structure capturing the state of the thread. */
typedef struct ThrPostProcess {
    String      outStreamName;       // Name of output stream     
    String      rxMsgqName;          // receiving MSGQ name
    String      txMsgqName;          // transmitting MSGQ name
    Uns         numPrimingBuffers;   // number of buffers used for priming
    Uns         numChansToDev;       // number of channels sent to device    
    SIO_Handle  outStream;           // Output stream
    SEM_Obj     msgqSemObj;          // Semaphore used for MSGQ
    MSGQ_Handle msgQueue;            // Receiving Message Queue
    
    /* 
     * Data buffers used for receiving data messages from preceding task in 
     * processing chain 
     */
    Sample bufInput[ NUMCHANNELS ][ FRAMELEN ];
} ThrPostProcess;

// prototypes for public functions
extern Void thrPostProcessInit( ThrPostProcess *thr );
extern Void thrPostProcessStartup( ThrPostProcess *thr, Sample *bufTx[] );
extern Void thrPostProcessRun( Arg thr );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRPOSTPROCESS_

