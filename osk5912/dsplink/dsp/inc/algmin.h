/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== algmin.h ========
 */
#ifndef ALGMIN_
#define ALGMIN_

#include <std.h>

#include <ialg.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Maximum number of memTab descriptors returned by an algorithm */
#define ALGMIN_MAXMEMRECS      16


/* Instantiate a 'static' XDAIS algorithm */
extern IALG_Handle ALGMIN_new(IALG_Fxns *fxns, IALG_Params *params, 
        Char *algChanBufs[], SmUns numAlgChanBufs);

/* Initialize scratch memory buffers before processing */
extern Void ALGMIN_activate(IALG_Handle alg);

/* Save all persistent data to non-scratch memory */    
extern Void ALGMIN_deactivate(IALG_Handle alg);
  
/* ALGMIN Module Initialization */
extern Void ALGMIN_init(Void);

/* ALGMIN Module Finalization */
extern Void ALGMIN_exit(Void);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* ALGMIN_ */



