/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrProcess.h ========
 */

#ifndef THRPROCESS_
#define THRPROCESS_


// DSP/BIOS module includes
#include <mbx.h>

// RF5 module includes
#include <chan.h>
#include <icell.h>

#ifdef __cplusplus
extern "C" {
#endif


// Enumeration of channel cells (so we use names intead of numbers for index)
enum { CELLFIR  = 0, 
       CELLVOL,
       NUMCELLS 
};            

// Types of messages from the control thread accepted by this thread.
enum {
    MSGNEWVOL = 0,
    MSGNEWCOEFF
};

// Set the length of the mailbox for control messages
#define THRPROCESS_MBXLENGTH 5
 
/* Definition of the structure describing the state of the thread. */
typedef struct ThrProcess {
    CHAN_Obj         chanList[ NUMCHANNELS ];              
    ICELL_Obj        cellList[ NUMCHANNELS * NUMCELLS ];       
    Sample          *bufInput[ NUMCHANNELS ];
    Sample          *bufOutput[ NUMCHANNELS ];
    Sample          *bufIntermediate;
    ScomBufChannels  scomMsgRx;
    ScomBufChannels  scomMsgTx;
} ThrProcess; 

/* mailbox for control messages */
extern MBX_Handle mbxProcess;

/* prototypes for public functions */
extern Void thrProcessInit( Void );
extern Void thrProcessStartup( Void );
extern Void thrProcessRun( Void );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRPROCESS_

