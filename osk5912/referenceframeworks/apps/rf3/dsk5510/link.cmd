/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  Linker-command file for the RF3 application
 *
 *  This file governs the linking process. The minimum of information
 *  it needs to contain is:
 *
 *  1) the list of libraries used by the application; they will be
 *     searched for in directories specified in the "build options->
 *     linker->library search path" field of the project
 *  2) inclusion of the configuration file automatically generated
 *     by the configuration database that handles BIOS objects
 *  
 *  In addition, it may define symbols used by the application 
 *  (typical for XDAIS algorithms, an ALG algorithm provided by
 *  vendor VEN has its function table named as ALG_VEN_IALG,
 *  but the application may always refer to it as ALG_IALG,
 *  if there is an assignment _ALG_IALG = _ALG_VEN_IALG in this file.
 *  Binding to a specific implementation of an algorithm then occurs
 *  in the linker-command file. Therefore, if another version of 
 *  algorithm ALG is supplied by vendor VEN2, it only takes to change
 *  the assignment in this file to _ALG_IALG = _ALG_VEN2_IALG
 *  and relink the object files without having to recompile the application.
 *
 *  Finally, it may employ some space optimization techniques, such as
 *  moving all initialization code to external memory or even overlaying
 *  initialization code with data.
 */
 
/* include config-generated link command file */
-l appcfg.cmd

/* include libraries for the IOM driver and PIO adapter */
-l dsk5510_dma_aic23.l55l
-l c5510_dma_mcbsp.l55l
-l pio.l55l

/* include the UTL debugging module (if needed) */
-l utl.l55l

/* include the RF3 module implementing XDAIS algs. instantiation procedures */
-l algrf.l55l
    
/* 
 *  Algorithm FIR: bind the generic FIR symbol to TI's implementation
 *  of the algorithm, and include the appropriate library
 */
-l fir_ti.l55l
_FIR_IFIR = _FIR_TI_IFIR;
 
/* 
 *  Algorithm VOL: bind the generic VOL symbol to TI's implementation
 *  of the algorithm, and include the appropriate library;
 */
-l vol_ti.l55l
_VOL_IVOL = _VOL_TI_IVOL;




