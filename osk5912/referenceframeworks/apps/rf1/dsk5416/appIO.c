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
 *  -- low-level IO -- driver), and every other application-specific IO. 
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

#include <iom.h>                 /* IOM interface definition */
#include <pio.h>                 /* PIO adapter decls. */

#include <dsk5416.h>    /* Global board defs (e.g CPLD regs) for DSK5416 */
#include <dsk5416_dma_pcm3002.h> /* DMA driver for DSK5416 PCM3002 codec */

#include <utl.h>                 /* debug/diagnostics utility functions */

#include "appBiosObjects.h"      /* BIOS objects declared */
#include "appIO.h"               /* application IO initialization, priming */

/* 
 *  Declaration of PIO objects for receive and transmit:
 *  pioRx is the object in charge of transferring data from codec to Rx pipe,
 *  pioTx is the object in charge of transferring data from Tx pipe to codec
 */
PIO_Obj pioRx;
PIO_Obj pioTx;

/*
 *  ======== appIOInit ========
 *
 *  Initialize IOM and other (non-streaming) IO components, if any
 */
 
Void appIOInit() 
{
    /* Initialize the Board Support Library */
    DSK5416_init();

    /* 
     * [Optional] Set up the PCM3002 sampling frequency:
     * The default sampling frequency (fs) of C5416DSK is 48KHz.
     * Other C5416DSK accepted fs are: 6KHz, 8KHz, 12KHz and 24KHz.
     * Any invalid fs will be reset to the default value: 48KHz.
     */
    DSK5416_PCM3002_setFreq(NULL, 48000);  // preserve default sampling freq

    /*  Initialize PIO:
     *  PIO_new initializes a PIO object with the following arguments:
     *  1. address of the PIO object
     *  2. handle of the associated pipe (pipRx for input and pipTx for output)
     *  3. driver name used in Configuration tool
     *  4. mode: input or output
     *  5. optional generic arguments
     */
    PIO_new( &pioRx, &pipRx, "/udevCodec", IOM_INPUT, NULL );
    PIO_new( &pioTx, &pipTx, "/udevCodec", IOM_OUTPUT, NULL );
}

/*
 *  ======== appIOPrime ========
 *
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
    UTL_assert( PIP_getWriterNumFrames( &pipTx ) >= 2 );
    
    PIO_txStart( &pioTx, PIP_getWriterNumFrames( &pipTx ), 0);

    /* Prime the receive side with empty buffers to be filled. */
    PIO_rxStart( &pioRx, PIP_getWriterNumFrames( &pipRx ) );
}


