/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== cellFir.h ========
 *
 */
#ifndef CELLFIR_
#define CELLFIR_

#include <ialg.h>

#include <icell.h>

#include "ifir.h"

#ifdef __cplusplus
extern "C" {
#endif

/* v-table for this cell */
extern ICELL_Fxns FIR_CELLFXNS;

/* v-table for this cell's algorithm */
extern IFIR_Fxns FIR_IFIR;

/*
 *  ======== function prototypes ========
 */
Int  FIR_cellControl( ICELL_Handle handle, IALG_Cmd cmd, IALG_Status *status);
Bool FIR_cellExecute( ICELL_Handle handle, Arg arg );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif  /* CELLFIR_ */

