/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir_ti_priv.h ========
 *  Internal vendor specific (TI) interface header for FIR
 *  algorithm. Only the implementation source files include
 *  this header; this header is not shipped as part of the
 *  algorithm.
 *
 *  This header contains declarations that are specific to
 *  this implementation and which do not need to be exposed
 *  in order for an application to use the FIR algorithm.
 */

#ifndef FIR_TI_PRIV_
#define FIR_TI_PRIV_
 
#include    <std.h>

#include    "ifir.h"
#include    "fir_ti.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== FIR_TI_Obj ========
 */
typedef struct FIR_TI_Obj {
    IALG_Obj    alg;            /* MUST be first field of all FIR objs */
    Short       *workBuf;       /* on-chip scratch history */
    Short       *history;       /* off chip presistant history */
    Short       *coeff;         /* on-chip persistant coeff */
    Int         filterLenM1;    /* length of coefficient array - 1 */
    Int         frameLen;       /* length of input (output) buffer */
} FIR_TI_Obj;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* FIR_TI_PRIV_ */


