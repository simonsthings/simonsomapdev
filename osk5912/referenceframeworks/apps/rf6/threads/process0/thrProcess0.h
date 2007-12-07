/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrProcess0.h ========
 */

#ifndef THRPROCESS0_
#define THRPROCESS0_

// DSP/BIOS module includes
#include <sem.h>
#include <msgq.h>

// RF module includes
#include <chan.h>
#include <icell.h>

#ifdef __cplusplus
extern "C" {
#endif           

// Enumeration of channel cells (so we use names instead of numbers for index)
enum { 
    THRPROCESS0_CELLFIR  = 0, 
    THRPROCESS0_CELLVOL,
    THRPROCESS0_NUMCELLS 
}; 

// Definition of the structure describing the state of the thread.
typedef struct ThrProcess0 {
    CHAN_Obj       chanList[ NUMCHANNELS ];              
    ICELL_Obj      cellList[ NUMCHANNELS * THRPROCESS0_NUMCELLS ];       
    Sample         *bufInput[ NUMCHANNELS ];
    Sample         *bufOutput[ NUMCHANNELS ];
    Sample         *bufIntermediate;
    SEM_Obj        msgqSemObj;     // Semaphore used for MSGQ
    MSGQ_Handle    msgQueue;       // Receiving Message Queue
} ThrProcess0; 

// prototypes for public functions 
extern Void thrProcess0Init( Void );
extern Void thrProcess0Startup( Void );
extern Void thrProcess0Run( Void );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRPROCESS0_

