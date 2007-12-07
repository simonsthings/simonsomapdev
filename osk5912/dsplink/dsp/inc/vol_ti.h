/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== vol_ti.h ========
 *  Interface for the VOL_TI module; TI's implementation 
 *  of the IVOL interface
 */
#ifndef VOL_TI_
#define VOL_TI_

#include <ialg.h>

#include "ivol.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== VOL_TI_IALG ========
 *  TI's implementation of the IALG interface for VOL
 */
extern IALG_Fxns VOL_TI_IALG;

/*
 *  ======== VOL_TI_IVOL ========
 *  TI's implementation of the IVOL interface
 */
extern IVOL_Fxns VOL_TI_IVOL;

/*
 *  ======== VOL_TI_exit ========
 *  Required module finalization function
 */
extern Void VOL_TI_exit(Void);

/*
 *  ======== VOL_TI_init ========
 *  Required module initialization function
 */
extern Void VOL_TI_init(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* VOL_TI_ */


