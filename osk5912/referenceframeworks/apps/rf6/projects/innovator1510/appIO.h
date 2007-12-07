/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appIO.h ========
 *
 */
#ifndef APPIO_
#define APPIO_

#ifdef __cplusplus
extern "C" {
#endif

// The board has a stereo codec
#define NUMCODECCHANS   2

/*
 *  Prototypes of I/O functions invoked from the rest of the app
 */
extern Void appIOInit();

#ifdef __cplusplus
}
#endif // extern "C"

#endif // APPIO_


