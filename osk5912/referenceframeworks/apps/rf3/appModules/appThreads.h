/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appThreads.h ========
 *
 *  Threads-wide constants, global variables, macros, 
 *  buffers, and declaration of functions called from the rest of the app
 */
#ifndef APPTHREADS_
#define APPTHREADS_

#include "appResources.h"   /* application-wide common info */

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *  Application parameters (used by processing/control threads) 
 */


/* 
 *  Declaration of intermediate buffers used by threads' algorithms 
 */

/* this buffer is used by the Audioproc thread. "Sample" is usually 16b wide. */ 
extern Sample bufAudioproc[ FRAMELEN ];


/*
 *  Prototypes of thread functions invoked from the rest of the app
 */

/* function that initializes all the threads */
Void thrInit( Void );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* APPTHREADS_ */ 

