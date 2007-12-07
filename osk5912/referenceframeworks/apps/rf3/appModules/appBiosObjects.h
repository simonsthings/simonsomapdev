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
#include <pip.h>
#include <swi.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LOG objects */
extern LOG_Obj logTrace;        /* application printf() log */   

/* STS objects */

/* PIP objects */
extern PIP_Obj pipRx;           /* receive pipe */
extern PIP_Obj pipTx;           /* transmit pipe */

extern PIP_Obj pipRx0;          /* receive pipe  - channel 0 */
extern PIP_Obj pipTx0;          /* transmit pipe - channel 0 */
extern PIP_Obj pipRx1;          /* receive pipe  - channel 1 */
extern PIP_Obj pipTx1;          /* transmit pipe - channel 1 */

/* SWI objects: declare those that are or might be posted manually */
extern SWI_Obj swiControl;      /* control thread */

/* heaps in data segments */
extern Int INTERNALHEAP;        /* name of the internal heap */
extern Int EXTERNALHEAP;        /* name of the external heap */

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* APPBIOSOBJECTS_ */

