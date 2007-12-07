/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir.h ========
 *
 *  Caller-friendly interface for the FIR algorithm; made from
 *  vendor's fir.h by replacing all ALG_ prefixes with ALGRF_
 *  (as in ALG_create with ALGRF_create)
 *  
 *  Original comments:
 *  This header defines all types, constants, and functions used by 
 *  applications that use the FIR algorithm.
 *
 *  Applications that use this interface enjoy type safety and
 *  the ability to incorporate multiple implementations of the FIR
 *  algorithm in a single application at the expense of some
 *  additional indirection.
 */
#ifndef FIR_
#define FIR_

#include <algrf.h>
#include <ifir.h>

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

/*
 *  ======== FIR_PARAMS ========
 *  Default instance parameters
 */
#define FIR_PARAMS IFIR_PARAMS

/*
 *  ======== FIR_apply ========
 *  Apply an FIR filter to the input array and place results in the
 *  output array.
 */
extern Void FIR_apply(FIR_Handle fir, Short in[], Short out[]);

/*
 *  ======== FIR_create ========
 *  Create an instance of an FIR object.
 */
static inline FIR_Handle FIR_create(const IFIR_Fxns *fxns, 
    const FIR_Params *prms)
{
    return ((FIR_Handle)
        ALGRF_create((IALG_Fxns *)fxns, NULL, (IALG_Params *)prms));
}

/*
 *  ======== FIR_delete ========
 *  Delete an FIR instance object
 */
static inline Void FIR_delete(FIR_Handle handle)
{
    ALGRF_delete((ALGRF_Handle)handle);
}


/*
 *  ======== FIR_exit ========
 *  Module finalization
 */
extern Void FIR_exit(Void);

/*
 *  ======== FIR_init ========
 *  Module initialization
 */
extern Void FIR_init(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* FIR_ */

