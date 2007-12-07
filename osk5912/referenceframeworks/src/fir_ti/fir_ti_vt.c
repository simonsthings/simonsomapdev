/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir_ti_vt.c ========
 *  Definition of FIR_TI_IFIR, the IFIR function structure the defines TI's
 *  implementation of the IFIR interface.  Since IFIR derives from IALG,
 *  we also define the symbol FIR_TI_IALG, TI's implementation of the IALG
 *  interface.
 *
 *  We place these tables in a separate file for two reasons:
 *     1. We want allow one to replace these tables with
 *    different definitions.  For example, one may
 *    want to build a system where the FIR is activated
 *    once and never deactivated, moved, or freed.
 *
 *     2. Eventually there will be a separate "system build"
 *    tool that builds these tables automatically 
 *    and if it determines that only one implementation
 *    of an API exists, "short circuits" the vtable by
 *    linking calls directly to the algorithm's functions.
 */

#include <std.h>

#include "ifir.h"
#include "fir_ti.h" 

extern Void FIR_TI_activate(IALG_Handle);
extern Int  FIR_TI_alloc(const IALG_Params *, struct IALG_Fxns **, IALG_MemRec *);
extern Int  FIR_TI_control(IALG_Handle, IALG_Cmd, IALG_Status *);
extern Void FIR_TI_deactivate(IALG_Handle);
extern Int  FIR_TI_free(IALG_Handle, IALG_MemRec *);
extern Int  FIR_TI_initObj(IALG_Handle, const IALG_MemRec *, IALG_Handle, const IALG_Params *);
extern Void FIR_TI_moved(IALG_Handle, const IALG_MemRec *, IALG_Handle, const IALG_Params *);
extern Int  FIR_TI_numAlloc(Void);
extern Void FIR_TI_filter(IFIR_Handle handle, Short in[], Short out[]);

#define IALGFXNS                                                 \
    &FIR_TI_IALG,       /* module ID */                          \
    FIR_TI_activate,    /* activate */                           \
    FIR_TI_alloc,       /* algAlloc */                           \
    FIR_TI_control,     /* control (NULL => no control ops) */   \
    FIR_TI_deactivate,  /* deactivate */                         \
    FIR_TI_free,        /* free */                               \
    FIR_TI_initObj,     /* init */                               \
    FIR_TI_moved,       /* moved (NULL => not suported) */       \
    FIR_TI_numAlloc     /* numAlloc (NULL => IALG_DEFMEMRECS) */

/*
 *  ======== FIR_TI_IFIR ========
 *  This structure defines TI's implementation of the IFIR interface
 *  for the FIR_TI module.
 */
IFIR_Fxns FIR_TI_IFIR = {   /* module_vendor_interface */
    IALGFXNS,
    FIR_TI_filter,
};

/*
 *  ======== FIR_TI_IALG ========
 *  This structure defines TI's implementation of the IALG interface
 *  for the FIR_TI module.
 */

asm("_FIR_TI_IALG .set _FIR_TI_IFIR");

/*
 *  We could have also duplicate the structure here to allow this code to be 
 *  compiled and run non-DSP platforms at the expense of unnecessary data space
 *  consumed by the definition below.
 *
 *  IALG_Fxns FIR_TI_IALG = {
 *      IALGFXNS
 *  };
 */


