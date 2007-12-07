/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== ivol.h ========
 *  IVOL Interface Header
 */
#ifndef IVOL_
#define IVOL_

#include <ialg.h>
#include <xdas.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== IVOL_Handle ========
 *  This handle is used to reference all VOL instance objects
 */
typedef struct IVOL_Obj *IVOL_Handle;

/*
 *  ======== IVOL_Obj ========
 *  This structure must be the first field of all VOL instance objects
 */
typedef struct IVOL_Obj {
    struct IVOL_Fxns *fxns;
} IVOL_Obj;

/*
 *  ======== IVOL_Status ========
 *  Status structure defines the parameters that can be changed or read
 *  during real-time operation of the alogrithm.
 */
typedef struct IVOL_Status {
    Int size;   /* must be first field of all status structures */
    Int gainPercentage;
} IVOL_Status;

/*
 *  ======== IVOL_Cmd ========
 *  The Cmd enumeration defines the control commands for the VOL
 *  control method.
 */
typedef enum IVOL_Cmd {
    IVOL_GETSTATUS,
    IVOL_SETSTATUS
} IVOL_Cmd;

/*
 *  ======== IVOL_Params ========
 *  This structure defines the creation parameters for all VOL objects
 */
typedef struct IVOL_Params {
    Int size;   /* must be first field of all params structures */
    Int frameSize;
    Int gainPercentage;
} IVOL_Params;

/*
 *  ======== IVOL_PARAMS ========
 *  Default parameter values for VOL instance objects
 */
extern IVOL_Params IVOL_PARAMS;

/*
 *  ======== IVOL_Fxns ========
 *  This structure defines all of the operations on VOL objects
 */
typedef struct IVOL_Fxns {
    IALG_Fxns   ialg;    /* IVOL extends IALG */
    Void  (*amplify)(IVOL_Handle handle, XDAS_Int16 *in, XDAS_Int16 *out);
    Int  (*control)(IVOL_Handle handle, IVOL_Cmd cmd, IVOL_Status *status);
} IVOL_Fxns;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* IVOL_ */
