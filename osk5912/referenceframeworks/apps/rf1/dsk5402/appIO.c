/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appIO.c ========
 *
 *  Apllication IO initialization and priming
 *
 *  Application IO consists of streaming IO (which is handled by the IOM 
 *  - Input/Output Module - driver), and every other application-specific IO. 
 *  IOM driver is further interfaced with PIO (PIP IOM) adapter for 
 *  simplicity of use with pipes. PIO connects the low-level driver to
 *  pipes, so once initialized, the incoming data is simply retrieved
 *  from pipes, and the outgoing data is simply put to pipes. PIO and IOM
 *  ensure that the data is streamed to the proper physical device.
 *
 *  We initialize the streaming IO here, and prime it, by placing zeroes
 *  at the output (transmit pipe), so those zeroes will be transferred
 *  while waiting for the first input frames and processing them.
 */

#include <std.h>

#include <iom.h>                /* IOM interface definition */
#include <pio.h>                /* PIO adapter decls. */

#include <dsk5402_dma_ad50.h>   /* driver for DSK5402 AD50 codec, using DMA */

#include <utl.h>                /* debug/diagnostics utility functions */

#include "appBiosObjects.h"     /* BIOS objects declared */
#include "appIO.h"              /* application IO initialization, priming */

/* 
 *  Declaration of PIO objects for receive and transmit:
 *  pioRx is the object in charge of transferring data from codec to Rx pipe,
 *  pioTx is the object in charge of transferring data from Tx pipe to codec
 */
PIO_Obj pioRx;
PIO_Obj pioTx;

/*
 *  ======== appIOInit ========
 *  Initialize IOM and other (non-streaming) IO components, if any
 */
Void appIOInit() 
{
    /* Initialize the PIO Adapter */
    PIO_new( &pioRx, &pipRx, "/udevCodec", IOM_INPUT, NULL );
    PIO_new( &pioTx, &pipTx, "/udevCodec", IOM_OUTPUT, NULL );
}

/*
 *  ======== appIOPrime ========
 *  Prime the input/output data stream pipes and start the transfer
 */
Void appIOPrime() 
{
    /*
     *  Prime the transmit side with buffers of silence (0).
     *  The transmitter should be started before the receiver.
     *  This results in input-to-output latency being one full
     *  buffer period if the pipes are configured for 2 frames.
     *  It also allows one full frame size worth of time for processing.
     */
    PIO_txStart( &pioTx, PIP_getWriterNumFrames( &pipTx ), 0 );

    /* Prime the receive side with empty buffers to be filled. */
    PIO_rxStart( &pioRx, PIP_getWriterNumFrames( &pipRx ) );
}


