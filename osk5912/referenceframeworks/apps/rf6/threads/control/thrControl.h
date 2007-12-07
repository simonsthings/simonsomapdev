/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrControl.h ========
 *
 */

#ifndef THRCONTROL_
#define THRCONTROL_

// DSP/BIOS module includes
#include <sem.h>
#include <msgq.h>

#ifdef __cplusplus
extern "C" {
#endif

// Types of messages from the control thread. Must match with the GPP side.
enum {
    MSGNEWVOL = 0,
    MSGNEWCOEFF
};

// Definition of the structure describing the state of the thread.
typedef struct ThrControl {
    SEM_Obj        msgqSemObj;     // Semaphore used for MSGQ
    MSGQ_Handle    msgQueue;       // Receiving Message Queue
} ThrControl; 

/* prototypes for public functions */
extern Void thrControlInit( Void );
extern Void thrControlStartup( Void );
extern Void thrControlRun( Void );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // THRCONTROL_

