/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== mqtbiosque.h ========
 *  The public header file for the DSP/BIOS QUE based transport.
 */

#ifndef MQTBIOSQUE_
#define MQTBIOSQUE_

#include <msgq.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This transport's interface functions.*/
extern const MSGQ_TransportFxns MQTBIOSQUE_FXNS;

/* Function prototypes */
extern Void MQTBIOSQUE_exit(Void);
extern Void MQTBIOSQUE_init(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MQTBIOSQUE_ */

