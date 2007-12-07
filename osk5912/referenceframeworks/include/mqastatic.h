/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== mqastatic.h ========
 *  This is the public include file for this static allocator module
 *  for MSGQ.
 */

#ifndef MQASTATIC_
#define MQASTATIC_

#include <msgq.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Allocator's parameters.
 *     buffer:     Used to hold the buffer specified by the user.
 *     bufferSize: Size of the buffer.
 *     msgSize:    Size of the messages in the buffer.
 */
typedef struct MQASTATIC_Params {
    Ptr         buffer;
    Uint16      bufferSize;
    Uint16      msgSize;
} MQASTATIC_Params;

/* This allocator's interface functions */
extern const MSGQ_AllocatorFxns MQASTATIC_FXNS;

/* Function prototypes */
extern Void MQASTATIC_init(Void); 
extern Void MQASTATIC_exit(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MQASTATIC_ */

