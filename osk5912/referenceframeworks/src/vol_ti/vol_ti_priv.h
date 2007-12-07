/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== vol_ti_priv.h ========
 *  Internal vendor specific (TI) interface header for VOL
 *  algorithm. Only the implementation source files include
 *  this header; this header is not shipped as part of the
 *  algorithm.
 *
 *  This header contains declarations that are specific to
 *  this implementation and which do not need to be exposed
 *  in order for an application to use the VOL algorithm.
 */
#ifndef VOL_TI_PRIV_
#define VOL_TI_PRIV_

#include <ialg.h>
#include "ivol.h"

#ifdef __cplusplus
extern "C" {
#endif

/* instance object */
typedef struct VOL_TI_Obj {
    IALG_Obj    ialg;           /* MUST be first field of all XDAIS algs */
    XDAS_Int16  *scaleBuf;      /* on-chip scaling buffer */
    Int         frameLen;       /* length of the frame scaled */     
    Int         gainPercentage; /* how much to amplify, 100 = no change */
} VOL_TI_Obj;

/*  IALG fxn declarations */
extern Int VOL_TI_alloc(const IALG_Params *algParams, IALG_Fxns **pf,
                        IALG_MemRec memTab[]);

extern Int VOL_TI_free(IALG_Handle handle, IALG_MemRec memTab[]);

extern Int VOL_TI_initObj(IALG_Handle handle,
                          const IALG_MemRec memTab[], IALG_Handle parent,
                          const IALG_Params *algParams);

extern Void VOL_TI_moved(IALG_Handle handle,
                          const IALG_MemRec memTab[], IALG_Handle parent,
                          const IALG_Params *algParams);

/*  IVOL fxn declarations */
extern Void VOL_TI_amplify(IVOL_Handle handle, XDAS_Int16 *in, XDAS_Int16 *out);
extern Int VOL_TI_control(IVOL_Handle handle, IVOL_Cmd cmd, IVOL_Status *status);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* VOL_TI_PRIV_ */


