/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir_ti.h ========
 *  Interface for the FIR_TI module; TI's implementation 
 *  of the IFIR interface
 */
#ifndef FIR_TI_
#define FIR_TI_

#include <ialg.h>

#include "ifir.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== FIR_TI_IALG ========
 *  TI's implementation of the IALG interface for FIR
 */
extern IALG_Fxns FIR_TI_IALG;

/*
 *  ======== FIR_TI_IFIR ========
 *  TI's implementation of the IFIR interface
 */
extern IFIR_Fxns FIR_TI_IFIR;

/*
 *  ======== FIR_TI_exit ========
 *  Required module finalization function
 */
extern Void FIR_TI_exit(Void);

/*
 *  ======== FIR_TI_init ========
 *  Required module initialization function
 */
extern Void FIR_TI_init(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* FIR_TI_ */


