/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== firapp.h ========
 *
 *  Caller-friendly interface for the FIR algorithm.
 *  This header defines all types, constants, and functions used by 
 *  applications that use the FIR algorithm.
 */
#ifndef FIRAPP_
#define FIRAPP_

#include <ifir.h>
#include <algmin.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== FIR_Handle ========
 *  FIR algorithm instance handle
 */
typedef struct IFIR_Obj *FIR_Handle;

/*
 *  ======== FIR_Params ========
 *  FIR algorithm instance creation parameters
 */
typedef struct IFIR_Params FIR_Params;

/* Match the buffer references to those in the pre-generated buffers C file */
extern Char firChanBufId00[];
extern Char firChanBufId01[];
extern Char firChanBufId02Scr[];

/* Enable access to different FIR instantiation paramater sets */
extern IFIR_Params *FIR_chanParamPtrs[];


/*
 *  ======== FIR_new ========
 *  Statically create a FIR instance ie no heaps etc 
 */
static inline FIR_Handle FIR_new(const IFIR_Fxns *fxns, 
        FIR_Params *params, Char *firChanBufs[], SmUns firNumChanBufs)
{
    return ( (FIR_Handle)ALGMIN_new((IALG_Fxns *)fxns, 
        (IALG_Params *)params, firChanBufs, firNumChanBufs) );
}

/*
 *  ======== FIR_apply ========
 *  Apply FIR algorithm to input array and place results in output array.
 */
extern Void FIR_apply(FIR_Handle handle, Short in[], Short out[]);

/*
 *  ======== FIR_init ========
 *  Module initialization
 */
extern Void FIR_init(Void);

/*
 *  ======== FIR_exit ========
 *  Module finalization
 */
extern Void FIR_exit(Void);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* FIRAPP_ */



