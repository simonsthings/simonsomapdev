/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== fir_ti.c ========
 *  Required module init and exit method for the FIR_TI module.
 */

#pragma CODE_SECTION(FIR_TI_init, ".text:init") 
#pragma CODE_SECTION(FIR_TI_exit, ".text:exit") 

#include <std.h>

/*
 * ======== FIR_TI_exit ========
 * Module finalization method.
 */
Void FIR_TI_exit(Void)
{
}

/*
 * ======== FIR_TI_init ========
 * Module initialization method.
 */
Void FIR_TI_init(Void)
{
}

