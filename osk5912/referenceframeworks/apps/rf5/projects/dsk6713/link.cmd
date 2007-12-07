/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  Linker-command file for the RF5 application
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

/* include the RF5 module */
-l algrf.l62        /* XDAIS algorithm instance manager module      */
-l chan.l62         /* Channel Manager module                       */
-l icc.l62          /* Inter-Cell Communication module              */
-l scom.l62         /* Synchronized communication module            */
-l sscr.l62         /* Shared Scratch Module                        */
-l utl.l62          /* Utility module for debugging and diagnostics */

/* 
 *  Algorithm FIR: bind the generic FIR symbol to TI's implementation
 *  of the algorithm, and include the appropriate library
 */
-l fir_ti.l62
_FIR_IFIR = _FIR_TI_IFIR;

/* 
 *  Algorithm VOL: bind the generic VOL symbol to TI's implementation
 *  of the algorithm, and include the appropriate library;
 */
-l vol_ti.l62
_VOL_IVOL = _VOL_TI_IVOL;

/* bind the low level codec driver */
-l dsk6713_edma_aic23.l67
-l c6x1x_edma_mcbsp.l67

/*
 *  NB: When you port this application to another platform, make sure 
 *      the .bss (and .far for c6000) section resides in INTERNAL memory.
 *
 *      Reason is that data is [E]DMA-ed to and from double buffers
 *      statically allocated in threads RxSplit and TxJoin. 
 *      On c6000, if these buffers lie in external memory and are not 
 *      properly aligned and sized, cache consistency problems may occur. 
 *      On some c5000 chips DMA only transfers data to internal mem (eg. c5402)
 */

