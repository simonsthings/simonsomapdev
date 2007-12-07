/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== VOL.c ========
 *
 *  Caller-friendly interface for the IVOL algorithm
 *  
 *  Obtained from the vendor-provided vol.c where all ALGRF_ prefixes
 *  were replaced with ALGRF_ (as in ALGRF_activate -> ALGRF_activate)
 *  to use the ALGRF module.
 *
 *  Original comments:
 *  VOL Filter Module - implements all functions and defines all constant
 *  structures common to all VOL filter algorithm implementations.
 */
#include <std.h>

#include <algrf.h>

#include "vol.h"

/*
 *  ======== VOL_apply ========
 *  Apply a VOL filter to the input array and place results in the
 *  output array.
 */
Void VOL_apply( VOL_Handle handle, Short in[], Short out[] )
{
    /* activate instance object */
    ALGRF_activate( (ALGRF_Handle)handle );

    handle->fxns->amplify( handle, (XDAS_Int16 *)in, (XDAS_Int16 *)out ); 

    /* deactivate instance object */
    ALGRF_deactivate( (ALGRF_Handle)handle );
}

/*
 *  ======== VOL_exit ========
 *  Module finalization
 */
Void VOL_exit()
{
}

/*
 *  ======== VOL_init ========
 *  Module initialization
 */
Void VOL_init()
{
}


