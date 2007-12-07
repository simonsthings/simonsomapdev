/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  Linker-command file for the RF application
 *
 *  This file governs the linking process. 
 */
/* include config-generated link command file */
-l appcfg.cmd

/* include the RF modules */
-l algrf.l55l       /* XDAIS algorithm instance manager module      */
-l chan.l55l        /* Channel Manager module                       */
-l icc.l55l         /* Inter-Cell Communication module              */
-l sscr.l55l        /* Shared Scratch Module                        */
-l utl.l55l         /* Utility module for debugging and diagnostics */

/* include the MSGQ modules */
-l msgq.l55l 
-l mqtbiosque.l55l
-l dsplinkmsg.l55l  /* mqabuf and remote transport */

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

/* DSP/BIOS Link driver */
-l dsplink.l55l

/* bind the low level codec driver to board implementation*/
-l innov5910_dma_aic23_cslv3.l55l
-l c5910_dma_mcbsp_cslv3.l55l

/* CSL board Library */
-l csl_OMAP5910_l.lib

/* 
 * Place init and exit code into external memory to reduce internal
 *  memory usage 
 */
SECTIONS
{
    .initExitCode {
        *(.text:init)   
        *(.text:create) 
        *(.text:delete)
        *(.text:exit)
    } > EXTMEM
}

/*
 *  NB: When you port this application to another platform, make sure 
 *      the .bss (and .far for c6000) section resides in INTERNAL memory.
 *
 *      Reason is that data is [E]DMA-ed to and from double buffers
 *      statically allocated in threads PreProcess and PostProcess. 
 *      On c6000, if these buffers lie in external memory and are not 
 *      properly aligned and sized, cache consistency problems may occur. 
 *      On some c5000 chips DMA only transfers data to internal mem (eg. c5402)
 */


