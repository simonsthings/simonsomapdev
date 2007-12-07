/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== ivol.c ========
 *  This file defines the default parameter structure for ivol.h
 */
#include <std.h>
#include <ivol.h>

/*
 *  ======== VOL_PARAMS ========
 *  This constant structure defines the default parameters for VOL objects
 */
IVOL_Params IVOL_PARAMS = {
    sizeof(IVOL_Params),
    0,          /* Int frameSize; */
    100         /* Int gainPercentage; */
};

