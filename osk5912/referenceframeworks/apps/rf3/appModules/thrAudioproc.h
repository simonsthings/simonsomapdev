/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrAudioproc.h ========
 *
 *  Declaration of resources for thread Audioproc
 *
 *  Thread Audioproc processes the audio data.
 *  This file exposes thread Audioproc's structure to all the modules that
 *  need it, as well as its functions.
 */
#ifndef THRAUDIOPROC_
#define THRAUDIOPROC_

#include <pip.h>

#include <algrf.h>          /* algorithm instance creation procedures */

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */
#include "fir.h"            /* interface for the FIR algorithm */
#include "vol.h"            /* interface for the VOL algorithm */

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *  Here we define a structure that contains all the "private"
 *  thread information: algorithm handles, input pipe(s), output
 *  pipe(s), intermediate buffer(s), if any, and all the other
 *  information that encapsulates thread state.
 */
typedef struct ThrAudioproc {
    /* algorithm(s) */
    FIR_Handle  algFIR;     /* an instance of the FIR algorithm */
    VOL_Handle  algVOL;     /* an instance of the VOL algorithm */
    
    /* input pipe(s) */
    PIP_Handle      pipIn;

    /* output pipe(s) */
    PIP_Handle      pipOut;

    /* intermediate buffer(s) */
    Sample         *bufInterm;

    /* everything else that is private for a thread comes here */
    
} ThrAudioproc; 

/* 
 *  Parameters common for all the threads of this type, is any,
 *  should be declared outside of the above structure.
 */

 
/*
 *  Declaration of thread data objects (one thread per channel)
 */
extern ThrAudioproc thrAudioproc[ NUMCHANNELS ];

/*  
 *  declaration of generic function tables for XDAIS algorithms;
 *  the link-edit file link.cmd will resolve this by binding
 *  the name to the actual implementation;
 *  for example, linker command file has a line: 
 *  _FIR_IFIR = _FIR_TI_IFIR
 */
extern IFIR_Fxns FIR_IFIR;      /* FIR algorithm */
extern IVOL_Fxns VOL_IVOL;      /* VOL algorithm */

/*
 *  Declaration of "public" thread functions 
 */
/* init function, called from thrInit() */
extern Void thrAudioprocInit( Void );

/* run function, called by the swiAudioproc SWI object */
extern Void thrAudioprocRun( Arg aChan );

/* parameter change function, called by the control thread */
extern Void thrAudioprocSetOutputVolume( Int chan, Int volume );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* THRAUDIOPROC_ */

