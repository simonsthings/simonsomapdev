/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appResources.h ========
 *
 */
#ifndef APPRESOURCES_
#define APPRESOURCES_

// DSP/BIOS module includes
#include <sem.h>

#include "appBiosObjects.h"
#include "appIO.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUMCHANNELS  2  // number of processing channels in processing threads
#define FRAMELEN    80  // number of samples in a frame

typedef Short Sample;

/* The function prototypes. */
Bool binarySemPend(SEM_Handle semHandle, Uns timeout);

#ifdef __cplusplus
}
#endif // extern "C"

#endif // APPRESOURCES_ 

