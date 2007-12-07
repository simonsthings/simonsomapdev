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
 *  Apllication IO initialization and priming
 */
#ifndef APPIO_
#define APPIO_

#include <pio.h>           /* PIO adapter declarations */

/*
 *  Declaration of PIO objects used for transfer of data to/from codec,
 *  in case they are ever needed by the application outside of appIO.c.
 */
extern PIO_Obj pioRx;
extern PIO_Obj pioTx;

/*
 *  ======== appIOInit ========
 *  Initialize IOM and other (non-streaming) IO components, if any
 */
extern Void appIOInit();

/*
 *  ======== appIOPrime ========
 *  Prime the input/output data stream pipes
 */
extern Void appIOPrime();

#endif /* APPIO_ */

