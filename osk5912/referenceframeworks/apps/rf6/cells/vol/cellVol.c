/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== cellVol.c ========
 *
 */
#include <std.h>
#include <xdas.h>

#include <algrf.h>

#include "cellVol.h"

// v-table for this cell
ICELL_Fxns VOL_CELLFXNS = { 
    NULL,            // cellClose
    VOL_cellControl, // cellControl
    VOL_cellExecute, // cellExecute
    NULL             // cellOpen
};

/*
 *  ======== VOL_cellExecute ========
 *
 */
Bool VOL_cellExecute( ICELL_Handle handle, Arg arg )
{
    IVOL_Fxns  *volFxns   = (IVOL_Fxns *)handle->algFxns;
    IVOL_Handle volHandle = (IVOL_Handle)handle->algHandle;
    
    // activate instance object
    ALGRF_activate( handle->algHandle );
    
    volFxns->amplify( volHandle,
                     (XDAS_Int16 *)handle->inputIcc[0]->buffer,
                     (XDAS_Int16 *)handle->outputIcc[0]->buffer );
        
    // deactivate instance object
    ALGRF_deactivate( handle->algHandle );
    
    return ( TRUE );
}

/*
 *  ======== VOL_cellControl ========
 *
 */
Int  VOL_cellControl(ICELL_Handle handle, IALG_Cmd cmd, IALG_Status *status)
{
    IVOL_Fxns  *volFxns   = (IVOL_Fxns *)handle->algFxns;
    IVOL_Handle volHandle = (IVOL_Handle)handle->algHandle;
    
    return ( volFxns->control( volHandle, 
             (IVOL_Cmd) cmd, (IVOL_Status *)status ) );
}

