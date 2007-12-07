/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appBiosObjects.h ========
 *
 *  Declaration of all BIOS objects and constants defined in BIOS config 
 *  that can be of interest to the user: 
 *  LOG, STS, PIP, heaps in data segments, etc.
 *
 *  In the future this file will be automatically generated; currently,
 *  it is necessary to manually update this file whenever the BIOS config 
 *  is modified.
 */
#ifndef APPBIOSOBJECTS_
#define APPBIOSOBJECTS_

#include <log.h>
#include <sts.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LOG objects */
extern LOG_Obj logTrace;

/* STS objects */
extern STS_Obj stsTime0;
extern STS_Obj stsTime1;
extern STS_Obj stsTime2;
extern STS_Obj stsTime3;
extern STS_Obj stsTime4;
extern STS_Obj stsTime5;
extern STS_Obj stsTime6;
extern STS_Obj stsTime7;
extern STS_Obj stsTime8;
extern STS_Obj stsTime9;

/* heaps in data segments */
extern Int INTERNALHEAP;
extern Int EXTERNALHEAP;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* APPBIOSOBJECTS_ */

