/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrProcess1.h ========
 */

#ifndef THRPROCESS1_
#define THRPROCESS1_

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
    THRPROCESS1_CELLVOL  = 0, 
    THRPROCESS1_NUMCELLS 
}; 

// Definition of the structure describing the state of the thread.
typedef struct ThrProcess1 {
    CHAN_Obj       chanList[ NUMCHANNELS ];              
    ICELL_Obj      cellList[ NUMCHANNELS * THRPROCESS1_NUMCELLS ];       
    Sample         *bufInput[ NUMCHANNELS ];
    Sample         *bufOutput[ NUMCHANNELS ];
    SEM_Obj        msgqSemObj;     // Semaphore used for MSGQ
    MSGQ_Handle    msgQueue;       // Receiving Message Queue
} ThrProcess1; 

// prototypes for public functions
extern Void thrProcess1Init( Void );
extern Void thrProcess1Startup( Void );
extern Void thrProcess1Run( Void );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRPROCESS1_

