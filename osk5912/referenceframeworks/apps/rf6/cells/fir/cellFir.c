/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== cellFir.c ========
 *
 */
#include <std.h>

#include <algrf.h>

#include "cellFir.h"

// v-table for this cell
ICELL_Fxns FIR_CELLFXNS = { 
    NULL,            // cellClose
    FIR_cellControl, // cellControl
    FIR_cellExecute, // cellExecute
    NULL             // cellOpen
};

/*
 *  ======== FIR_cellExecute ========
 *
 */
Bool FIR_cellExecute( ICELL_Handle handle, Arg arg )
{
    IFIR_Fxns  *firFxns   = (IFIR_Fxns *)handle->algFxns;
    IFIR_Handle firHandle = (IFIR_Handle)handle->algHandle;

    // activate instance object
    ALGRF_activate( handle->algHandle );
    
    firFxns->filter( firHandle,
                     (Short *)handle->inputIcc[0]->buffer,
                     (Short *)handle->outputIcc[0]->buffer );
        
    // deactivate instance object
    ALGRF_deactivate( handle->algHandle );

    return ( TRUE );
}

/*
 *  ======== FIR_cellControl ========
 *
 */
Int  FIR_cellControl(ICELL_Handle handle, IALG_Cmd cmd, IALG_Status *status)
{
    Int rc = 0;
    IFIR_Fxns  *firFxns   = (IFIR_Fxns *)handle->algFxns;
    
    if( firFxns->ialg.algControl != NULL ) {
        rc = firFxns->ialg.algControl( handle->algHandle, cmd, status );
    }
    
    return rc;
}

