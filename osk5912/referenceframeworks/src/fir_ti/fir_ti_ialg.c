/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir_ti_ialg.c ========
 *  Implementation of the FIR_TI.h interface; TI's implementation 
 *  of the IFIR interface. 
 */
#pragma     CODE_SECTION(FIR_TI_activate,       ".text:algActivate")
#pragma     CODE_SECTION(FIR_TI_alloc,          ".text:algAlloc")  
#pragma     CODE_SECTION(FIR_TI_control,        ".text:algControl") 
#pragma     CODE_SECTION(FIR_TI_deactivate,     ".text:algDeactivate")
#pragma     CODE_SECTION(FIR_TI_free,           ".text:algFree")
#pragma     CODE_SECTION(FIR_TI_initObj,        ".text:algInit")
#pragma     CODE_SECTION(FIR_TI_moved,          ".text:algMoved")
#pragma     CODE_SECTION(FIR_TI_numAlloc,       ".text:algNumAlloc")
 
#include    <std.h>
#include    <string.h>

#include    "ifir.h"
#include    "fir_ti.h"
#include    "fir_ti_priv.h"

#define     OBJECT      0
#define     HISTORY     1
#define     WORKBUF     2
#define     NUMBUFS     3


/*
 *  ======== FIR_TI_activate ========
 *  Activate our object; e.g., initialize any scratch memory required
 *  by the FIR_TI processing methods.
 */
Void FIR_TI_activate(IALG_Handle handle)
{
    FIR_TI_Obj *fir = (Void *)handle;
    
    /* copy saved history to working buffer */
    memcpy((Void *)fir->workBuf, (Void *)fir->history, fir->filterLenM1 * sizeof(Short));
}

/*
 *  ======== FIR_TI_alloc ========
 *  Return a table of memory descriptors that describe the memory needed 
 *  to construct a FIR_TI_Obj structure.
 */
Int FIR_TI_alloc(const IALG_Params *firParams, IALG_Fxns **fxns, IALG_MemRec memTab[])
{
    const IFIR_Params *params = (Void *)firParams;
    
    if (params == NULL) 
    {
        params = &IFIR_PARAMS;  /* set default parameters */
    }

    /* Request memory for FIR object */
    memTab[OBJECT].size         = sizeof(FIR_TI_Obj);
    memTab[OBJECT].alignment    = 4;
    memTab[OBJECT].space        = IALG_EXTERNAL;
    memTab[OBJECT].attrs        = IALG_PERSIST;
    
    /*
     *  Request memory filter's "inter-frame" state (i.e., the
     *  delay history)
     *
     *  Note we could have simply added the delay buffer size to the
     *  end of the FIR object by combining this request with the one
     *  above, thereby saving some code.  We separate it here for
     *  clarity.
     */
    memTab[HISTORY].size        = (params->filterLen - 1) * sizeof(Short);
    memTab[HISTORY].alignment   = 2;
    memTab[HISTORY].space       = IALG_EXTERNAL;
    memTab[HISTORY].attrs       = IALG_PERSIST;

    /* Request memory for shared working buffer */
    memTab[WORKBUF].size        =  (params->filterLen - 1 + params->frameLen) * sizeof(Short);
    memTab[WORKBUF].alignment   = 2;
    memTab[WORKBUF].space       = IALG_DARAM0;
    memTab[WORKBUF].attrs       = IALG_SCRATCH;
    
    return (NUMBUFS);
}

/*
 *  ======== FIR_TI_control ========
 */
Int FIR_TI_control(IFIR_Handle handle, IFIR_Cmd cmd, IFIR_Status *status)
{
    FIR_TI_Obj *fir = (FIR_TI_Obj *)handle;

    if (cmd == IFIR_SETSTATUS) 
    {
        fir->coeff = status->coeffPtr;
    }
    else if (cmd == IFIR_GETSTATUS) 
    {
        status->coeffPtr =  fir->coeff;
    }
    else {
        return (IALG_EFAIL);
    }

    return (IALG_EOK);
}

/*
 *  ======== FIR_TI_deactivate ========
 *  Deactivate our object; e.g., save any scratch memory requred
 *  by the FIR_TI processing methods to persistent memory.
 */
Void FIR_TI_deactivate(IALG_Handle handle)
{
    FIR_TI_Obj *fir = (Void *)handle;
    
    /* copy history to external history buffer */
    memcpy((Void *)fir->history, (Void *)fir->workBuf, fir->filterLenM1 * sizeof(Short));

}

/*
 *  ======== FIR_TI_free ========
 *  Return a table of memory pointers that should be freed.  Note
 *  that this should include *all* memory requested in the 
 *  FIR_TI_alloc operation above.
 */
Int FIR_TI_free(IALG_Handle handle, IALG_MemRec memTab[])
{
    FIR_TI_Obj *fir = (Void *)handle;
    
    FIR_TI_alloc(NULL, NULL, memTab);
      
    memTab[OBJECT].base         = fir;
    
    memTab[HISTORY].size        = fir->filterLenM1 * sizeof(Short);
    memTab[HISTORY].base        = fir->history;

    memTab[WORKBUF].size        = (fir->filterLenM1 + fir->frameLen) * sizeof(Short);
    memTab[WORKBUF].base        = fir->workBuf;
    
    return (NUMBUFS);
}

/*  ======== FIR_TI_initObj ========
 *  Initialize the memory allocated for our instance.
 */
Int FIR_TI_initObj(IALG_Handle handle, const IALG_MemRec memTab[], IALG_Handle p, const IALG_Params *firParams)
{
    FIR_TI_Obj *fir = (Void *)handle;
    const IFIR_Params *params = (Void *)firParams;

    if (params == NULL) 
    {
        params = &IFIR_PARAMS;  /* set default parameters */
    }

    fir->workBuf        = memTab[WORKBUF].base;
    fir->history        = memTab[HISTORY].base;
    fir->coeff          = params->coeffPtr;  
    fir->filterLenM1    = params->filterLen - 1;
    fir->frameLen       = params->frameLen;
    
    return (IALG_EOK);
}

/*  ======== FIR_TI_moved ========
 *  Fix up any pointers to data that have been moved by the client.
 */
Void FIR_TI_moved(IALG_Handle handle, const IALG_MemRec memTab[], IALG_Handle p, const IALG_Params *firParams)
{
    FIR_TI_Obj *fir = (Void *)handle;

    fir->workBuf = memTab[WORKBUF].base;
    fir->history = memTab[HISTORY].base;
}

/*  ======== FIR_TI_numAlloc ========
 *  Returns the number of records needed for the MEM tab by the client.
 */
Int FIR_TI_numAlloc()
{
    return(NUMBUFS);
}


