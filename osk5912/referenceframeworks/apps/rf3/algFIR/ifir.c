/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== ifir.c ========
 *  IFIR default instance creation parameters
 */
#include <std.h>
#include <ifir.h>

/*
 *  ======== IFIR_PARAMS ========
 *  This static initialization defines the default parameters used to
 *  create instances of FIR objects.
 */
IFIR_Params IFIR_PARAMS = {
    sizeof(IFIR_Params),    /* sizeof this structure */
    NULL,                   /* coefficient array */
    0,                      /* filter length */
    0,                      /* frame length */
};

