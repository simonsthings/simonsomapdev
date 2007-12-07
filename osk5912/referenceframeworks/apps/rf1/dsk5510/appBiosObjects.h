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
 *  LOG, STS, PIP, memory segments, etc.
 *
 *  In the future this file will be automatically generated; currently,
 *  it is necessary to manually update this file whenever BIOS configuration 
 *  is modified.
 */

#ifndef APPBIOSOBJECTS_
#define APPBIOSOBJECTS_

#include <log.h>
#include <sts.h>
#include <pip.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Instrumentation objects */
extern LOG_Obj trace;
extern STS_Obj audioProcessExecTime;   /* holds audio time to completion */

/* PIP objects */
extern PIP_Obj pipRx;           /* receive pipe */
extern PIP_Obj pipTx;           /* transmit pipe */

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* APPBIOSOBJECTS_ */


