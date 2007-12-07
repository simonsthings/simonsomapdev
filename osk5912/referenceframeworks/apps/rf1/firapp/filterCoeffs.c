/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== filterCoeffs.c ========
 */
#include <std.h>
  
#include "filterCoeffs.h"

/* low pass filter coefficient set for the XDAIS FIR filter */

const short filterCoeffs[COEFFSIZE] = {   

    /* 8kHz, 32 Taps, 1200Hz-1800Hz, pass ripple 0.43dB, stop atten 56.83dB */
    0xffdc,0x0030,0x00f0,0x0171,0x00a4,0xfeb1,0xfda4,0xff7f,
    0x032a,0x0440,0xff98,0xf887,0xf817,0x04d3,0x1a98,0x2ba3,
    0x2ba3,0x1a98,0x04d3,0xf817,0xf887,0xff98,0x0440,0x032a,
    0xff7f,0xfda4,0xfeb1,0x00a4,0x0171,0x00f0,0x0030,0xffdc,

};



