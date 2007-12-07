/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir_setParams.c ========
 */
#include <std.h>

#include <ifir.h>

#include "filterCoeffs.h"

#pragma DATA_SECTION(FIR_STATIC_PARAMS, ".bss:algparams")
#pragma DATA_SECTION(FIR_chanParamPtrs, ".bss:algparams")

/*
 *  ======== FIR_STATIC_PARAMS ========
 *  This static initialization defines the parameters used to
 *  create instances of FIR objects.
 */
IFIR_Params FIR_STATIC_PARAMS[] = {

    {
        sizeof(IFIR_Params),                /* sizeof base params structure */
        (short *)filterCoeffs,              /* coefficient array */
        COEFFSIZE,                          /* filter length */
        FILTERFRAMESIZE,                    /* frame length */
    },
        
    /* +ENTER NEW CHANNEL PARAMETER SETS BELOW+ */
                                                                    
}; 
/* end of FIR_STATIC_PARAMS[] */

/* 
 *  Allow app to address base of each channel's Params. Alternatively if you
 *  wish to use the algorithm default params [e.g. IMOD_PARAMS] set the
 *  channel's chanParamPtr to NULL instead.
 */
IFIR_Params *FIR_chanParamPtrs[] = {
    &FIR_STATIC_PARAMS[0], 
    
    /* +UPDATE PARAM POINTERS TO REFLECT NEW CHAN PARAM SET ADDITIONS+ */
    
};



