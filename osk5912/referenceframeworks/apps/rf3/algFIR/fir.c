/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir.c ========
 *
 *  Caller-friendly interface for the FIR algorithm
 *
 *  Obtained from the vendor-provided fir.c where all ALG_ prefixes
 *  were replaced with ALGRF_ (as in ALG_activate -> ALGRF_activate)
 *  to use the ALGRF module.
 *  
 *  Original comments:
 *  FIR Filter Module - implements all functions and defines all constant
 *  structures common to all FIR filter algorithm implementations.
 */
#include <std.h>

#include <algrf.h>
#include "fir.h"

/*
 *  ======== FIR_apply ========
 *  Apply an FIR filter to the input array and place results in the
 *  output array.
 */
Void FIR_apply( FIR_Handle handle, Short in[], Short out[] )
{
    /* activate instance object */
    ALGRF_activate( (ALGRF_Handle)handle );
    
    handle->fxns->filter( handle, in, out );  /* filter data */

    /* deactivate instance object */
    ALGRF_deactivate( (ALGRF_Handle)handle );
}

/*
 *  ======== FIR_exit ========
 *  Module finalization
 */
Void FIR_exit()
{
}

/*
 *  ======== FIR_init ========
 *  Module initialization
 */
Void FIR_init()
{
}


