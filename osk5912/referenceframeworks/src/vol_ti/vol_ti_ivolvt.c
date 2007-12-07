/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== vol_ti_ialgvt.c ========
 *  This file contains the function table definitions for the
 *  IALG and IVOL interfaces implemented by the VOL_TI module.
 */
#include <std.h>

#include <ialg.h>
#include "vol_ti.h"
#include "vol_ti_priv.h"

#define IALGFXNS \
    &VOL_TI_IALG,               /* module ID */                         \
    NULL,                       /* activate */                          \
    VOL_TI_alloc,               /* alloc */                             \
    NULL,                       /* deactivate */                        \
    NULL,                       /* control (NULL => no control ops) */  \
    VOL_TI_free,                /* free */                              \
    VOL_TI_initObj,             /* init */                              \
    VOL_TI_moved,               /* moved */                             \
    NULL                        /* numAlloc (NULL => IALG_MAXMEMRECS) */

/*
 *  ======== VOL_TI_IVOL ========
 *  This structure defines TI's implementation of the IVOL interface
 *  for the VOL_TI module.
 */
IVOL_Fxns VOL_TI_IVOL = {       /* module_vendor_interface */
    IALGFXNS,                   /* IALG functions */
    VOL_TI_amplify,             /* The vol fxn */
    VOL_TI_control,             /* Control function */
};

/* Overlay v-tables to save data space */
asm("_VOL_TI_IALG  .set _VOL_TI_IVOL");

