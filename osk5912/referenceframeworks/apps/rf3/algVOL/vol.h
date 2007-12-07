/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== vol.h ========
 *
 *  Caller-friendly interface for the VOL algorithm; made from
 *  vendor's vol.h by replacing all ALG_ prefixes with ALGRF_
 *  (as in ALG_create with ALGRF_create)
 *  
 *  Original comments:
 *  This header defines all types, constants, and functions used by 
 *  applications that use the VOL algorithm.
 *
 *  Applications that use this interface enjoy type safety and
 *  the ability to incorporate multiple implementations of the VOL
 *  algorithm in a single application at the expense of some
 *  additional indirection.
 */
#ifndef VOL_
#define VOL_

#include <ialg.h>

#include <algrf.h>
#include <ivol.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== VOL_Handle ========
 *  VOL algorithm instance handle
 */
typedef struct IVOL_Obj *VOL_Handle;

/*
 *  ======== VOL_Params ========
 *  VOL algorithm instance creation parameters
 */
typedef struct IVOL_Params VOL_Params;

/*
 *  ======== VOL_PARAMS ========
 *  Default instance parameters
 */
#define VOL_PARAMS IVOL_PARAMS

/*
 *  ======== VOL_Cmd ========
 *  VOL instance control commands.
 */
typedef IVOL_Cmd VOL_Cmd;

/* control commands */ 
#define VOL_GETSTATUS IVOL_GETSTATUS
#define VOL_SETSTATUS IVOL_SETSTATUS

/*
 *  ======== VOL_Status ========
 *  VOL control/status structure.
 */
typedef IVOL_Status VOL_Status;

/*
 *  ======== VOL_create ========
 *  Create an instance of an VOL object.
 */
static inline VOL_Handle VOL_create(const IVOL_Fxns *fxns, 
    const VOL_Params *prms)
{
    return ((VOL_Handle)
        ALGRF_create((IALG_Fxns *)fxns, NULL, (IALG_Params *)prms));
}

/*
 *  ======== VOL_apply ========
 *  Apply an VOL filter to the input array and place results in the
 *  output array.
 */
extern Void VOL_apply(VOL_Handle vol, Short in[], Short out[]);

/*
 *  ======== VOL_control ========
 *  Invoke the VOL control function with the specified parameters.
 */
static inline Int VOL_control( VOL_Handle vol, 
    VOL_Cmd cmd, VOL_Status *status ) {
    return (vol->fxns->control( vol, cmd, status ));
}

/*
 *  ======== VOL_delete ========
 *  Delete a VOL instance object
 */
static inline Void VOL_delete(VOL_Handle handle)
{
    ALGRF_delete((ALGRF_Handle)handle);
}


/*
 *  ======== VOL_exit ========
 *  Module finalization
 */
extern Void VOL_exit(Void);

/*
 *  ======== VOL_init ========
 *  Module initialization
 */
extern Void VOL_init(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* VOL_ */

