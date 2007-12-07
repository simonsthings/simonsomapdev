/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== vol_ti_ialg.c ========
 *  VOL Module - TI implementation of the VOL module.
 *
 *  This file contains the implementation of the required IALG interface.
 */

#pragma CODE_SECTION(VOL_TI_alloc, ".text:algAlloc")
#pragma CODE_SECTION(VOL_TI_free, ".text:algFree")
#pragma CODE_SECTION(VOL_TI_initObj, ".text:algInit")
#pragma CODE_SECTION(VOL_TI_moved, ".text:algMoved")

#include <std.h>

#include <ialg.h>
#include "ivol.h"
#include "vol_ti_priv.h"

#define SCALEBUF 1
#define NUMBUFS 2

/*
 *  ======== VOL_TI_alloc ========
 *  Request memory.
 */
Int VOL_TI_alloc(const IALG_Params *algParams,
                 IALG_Fxns **pf, IALG_MemRec memTab[])
{
    const IVOL_Params *params = (Void *)algParams;

    if (params == NULL) { 
        params = &IVOL_PARAMS;  /* Use interface default params */
    }
 
    /* Request memory for VOL object */
    memTab[0].size = sizeof(VOL_TI_Obj);
    memTab[0].alignment = 0;           /* No alignment required */
    memTab[0].space = IALG_EXTERNAL;
    memTab[0].attrs = IALG_PERSIST;

    /* Request memory for scaling buffer */
    memTab[SCALEBUF].size = (params->frameSize) * sizeof(Int);
    memTab[SCALEBUF].alignment = 0;    /* No alignment required */
    memTab[SCALEBUF].space = IALG_DARAM0;
    memTab[SCALEBUF].attrs = IALG_PERSIST;
    
    return (NUMBUFS);
}

/*
 *  ======== VOL_TI_free ========
 *  Return a complete memTab structure.
 */
Int VOL_TI_free(IALG_Handle handle, IALG_MemRec memTab[])
{
    VOL_TI_Obj *vol = (VOL_TI_Obj *)handle;

    VOL_TI_alloc(NULL, NULL, memTab);

    memTab[0].base = handle;

    memTab[SCALEBUF].base = vol->scaleBuf;
    memTab[SCALEBUF].size = vol->frameLen * sizeof(Int);

    return (NUMBUFS);
}

/*
 *  ======== VOL_TI_initObj ========
 *  Initialize instance object.
 */
Int VOL_TI_initObj(IALG_Handle handle,
                const IALG_MemRec memTab[], IALG_Handle p,
                const IALG_Params *algParams)
{
    VOL_TI_Obj *vol = (VOL_TI_Obj *)handle;
    const IVOL_Params *params = (IVOL_Params *)algParams;    
    
    if (params == NULL) {
        params = &IVOL_PARAMS;  /* Use interface default params */
    }

    vol->scaleBuf = memTab[SCALEBUF].base;

    vol->frameLen = params->frameSize;
    vol->gainPercentage = params->gainPercentage;

    return (IALG_EOK);
}

/*
 *  ======== VOL_TI_moved ========
 *  Re-initialize buffer ptrs to new location.
 */
Void VOL_TI_moved(IALG_Handle handle,
                const IALG_MemRec memTab[], IALG_Handle p,
                const IALG_Params *algParams)
{
    VOL_TI_Obj *vol = (VOL_TI_Obj *)handle;

    vol->scaleBuf = memTab[SCALEBUF].base;
}


