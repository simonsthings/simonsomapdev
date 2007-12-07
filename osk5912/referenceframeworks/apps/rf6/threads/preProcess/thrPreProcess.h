/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrPreProcess.h ========
 *
 */

#ifndef THRPREPROCESS_
#define THRPREPROCESS_

// DSP/BIOS module includes
#include <sio.h>
#include <sem.h>
#include <msgq.h>

#include "appThreads.h"   // application-wide common info 

#ifdef __cplusplus
extern "C" {
#endif

// Definition of the structure capturing the state of the thread.
typedef struct ThrPreProcess {
    String      inStreamName;       // input stream
    String      rxMsgqName;         // receiving MSGQ name
    String      txMsgqName;         // transmitting MSGQ name
    Uns         numPrimingBuffers;  // number of buffers used for priming
    Uns         numChansFromDev;    // number of channels from device
    SIO_Handle  inStream;           // input stream
    SEM_Obj     msgqSemObj;         // Semaphore used for MSGQ
    MSGQ_Handle msgQueue;           // Receiving Message Queue
    Sample     *bufRx;              // buffers for the input device
} ThrPreProcess; 

// prototypes for public functions
extern Void thrPreProcessInit( ThrPreProcess *thr );
extern Void thrPreProcessStartup( ThrPreProcess *thr, Sample *bufRx[] );
extern Void thrPreProcessRun( Arg thr );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRPREPROCESS_

