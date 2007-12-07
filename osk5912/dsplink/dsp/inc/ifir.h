/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== ifir.h ========
 *  IFIR Interface Header
 */
#ifndef IFIR_
#define IFIR_

#include <ialg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== IFIR_Handle ========
 *  This handle is used to reference all FIR instance objects
 */
typedef struct IFIR_Obj *IFIR_Handle;

/*
 *  ======== IFIR_Obj ========
 *  This structure must be the first field of all FIR instance objects
 */
typedef struct IFIR_Obj {
    struct IFIR_Fxns *fxns;
} IFIR_Obj;

/*
 *  ======== IFIR_Status ========
 *  Status structure defines the parameters that can be changed or read
 *  during real-time operation of the alogrithm.
 */
typedef struct IFIR_Status {
    Int size;        /* must be first field of all status structures */
    Short *coeffPtr; /* READ/WRITE */
    
} IFIR_Status;
/*
 *  ======== IFIR_Cmd ========
 *  The Cmd enumeration defines the control commands for the FIR
 *  control method.
 */
typedef enum IFIR_Cmd {
    IFIR_GETSTATUS,
    IFIR_SETSTATUS
} IFIR_Cmd;

/*
 *  ======== IFIR_Params ========
 *  This structure defines the creation parameters for all FIR objects
 */
typedef struct IFIR_Params {
    Int size;   /* must be first field of all params structures */
    Short *coeffPtr;
    Int filterLen;
    Int frameLen;
} IFIR_Params;

/*
 *  ======== IFIR_PARAMS ========
 *  Default parameter values for FIR instance objects
 */
extern IFIR_Params IFIR_PARAMS;

/*
 *  ======== IFIR_Fxns ========
 *  This structure defines all of the operations on FIR objects
 */
typedef struct IFIR_Fxns {
    IALG_Fxns   ialg;    /* IFIR extends IALG */
    Void  (*filter)(IFIR_Handle handle, Short in[], Short out[]);

} IFIR_Fxns;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* IFIR_ */


