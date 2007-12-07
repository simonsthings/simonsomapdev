/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== cellVol.h ========
 *
 */
#ifndef CELLVOL_
#define CELLVOL_

#include <ialg.h>

#include <icell.h>

#include "ivol.h"

#ifdef __cplusplus
extern "C" {
#endif

// v-table for this cell
extern ICELL_Fxns VOL_CELLFXNS;

// v-table for this cell's algorithm
extern IVOL_Fxns VOL_IVOL;

/*
 *  ======== function prototypes ========
 */
Int  VOL_cellControl( ICELL_Handle handle, IALG_Cmd cmd, IALG_Status *status);
Bool VOL_cellExecute( ICELL_Handle handle, Arg arg );

#ifdef __cplusplus
}
#endif // extern "C" 

#endif // CELLVOL_

