/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 * ======== link.cmd ========
 */

/* BIOS automatically generated configuration */
-l genbufscfg.cmd

/* 
 * Link in the XDAIS algorithm whose buffer requirements will 
 * be interrogated and subsequently automatically generated in C source code.
 */
-l fir_ti.l55l

/* Point to the algorithm vendor's IALG v-table (name located in mod_ven.h) */
_MOD_VEN_IALG = _FIR_TI_IALG;

/* Point to the specific parameters for each channel of the FIR algorithm */
_MOD_chanParamPtrs = _FIR_chanParamPtrs;


