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
-l rf1cfg.cmd

/* I/O Device Driver(s) and Driver Adapters */
-l pio.l55l
-l c5510_dma_mcbsp.l55l
-l dsk5510_dma_aic23.l55l

/* ALGMIN instantiates XDAIS algos in a 'static' fashion */
-l algmin.l55l

/* UTL debugging module for seamless inclusion/removal of LOG, STS usage */
-l utl.l55l

/* XDAIS algorithm(s) */
-l fir_ti.l55l

/* Point to the algorithm vendor's IMOD v-table (name located in mod_ven.h) */
_FIR_IFIR = _FIR_TI_IFIR;

/* 
 * Define an overlay page to use for code and scratch data overlay.
 * It is very IMPORTANT to make sure the address range specified here
 * matches the one for DARAMOVL defined in the DSP/BIOS configuration file,
 * or the overlay will not work correctly. 
 * Note that these are byte addresses, whereas the ones in the DSP/BIOS
 * configuration file are 16-bit-word-based. 
 */
MEMORY 
{
    PAGE 1: DARAMOVL:    origin = 0xe000, length = 0x2000
}
 
/* 
 *  we *MUST* always fill/initialize BufIdX0 since XDAIS requires
 *  the buffer pointed to in memTab[0] to be initialized to contain all zeros.
 */
SECTIONS
{       
    .bss:firChanBufId00:     fill = 0x0  {} > DARAM

}
/* end of persistent buffer fill section */


/* 
 *  Overlays of XDAIS algorithm scratch memory buffers
 *  Goal is to reduce Data Memory usage. 
 *  
 *  Place all of your algos scratch data mem [tag 'Scr'] in this UNION and the
 *  buffers will be overlaid at same start address. The size will merely be 
 *  the largest algorithm scratch buf size, not the sum of all scratch bufs.
 *
 *  Pre-requisites for XDAIS algorithm scratch memory overlay : 
 *  - do *NOT* put any persistent data buffers here!
 *  - normal scratch memory restrictions ie overlaid memory must be protected
 *    from preemption by other algorithms and channels using that memory. 
 *
 *  Some algorithms request more than 1 scratch buffer per instance/channel.
 *  These cannot be overlaid together. They must have separate data memory.
 *  GROUP construct enables this. Place scratch bufs of SAME channel inside
 *  the GROUP. e.g. if mp3dec algo requests 6 buffers, 2 of which are scratch 
 *  then syntax may be (same channel denoted by '0' after 'Id') :-
 *  GROUP: {                    
 *      .bss:mp3decChanBufId02Scr: {}  
 *      .bss:mp3decChanBufId05Scr: {}  
 *  }
 *
 *  The above mp3dec Scratch Buffers can still be overlaid (UNION) with either
 *  (a) other mp3dec instances/channels scratch bufs
 *  (b) other algorithms scratch bufs 
 *  Place these outside the GROUP but inside the UNION.
 *
 *  If you have no algorithm scratch bufs [tag 'Scr'] comment/delete this unit.
 */

SECTIONS
{   
    UNION: run = DARAMOVL PAGE 1 
    {
        GROUP: {                        
            .bss:firChanBufId02Scr: {}  
        }
        
    }
}
    
/* 
 *  Overlays of code and data when code is used only at initialization time
 *  Goal is to reduce Program Memory usage.
 *
 *  Pre-requisites for overlay : 
 *  - the data which we overlay the code with is NOT used until after
 *    completion of the run-once code. 
 *  - the data must be uninitialized
 */    
SECTIONS
{    

    /* Chip Support Library code used once only */  
    .cslOvlCode: {
        
        /* 
         * These CSL fxns are run once only, before overlaid data 
         * is used 
         */
        *(.text:MCBSP_open)
        *(.text:MCBSP_reset)
        *(.text:MCBSP_config)
        *(.text:MCBSP_start)
        *(.text:DMA_open)
        *(.text:DMA_reset)
        *(.text:DMA_config)
        *(.text:CSL_cfgInit)
                
    } > DARAMOVL   /* defaults to PAGE 0 */ 

    /* XDAIS IALG code used once only */
    .ialgOvlCode: {
        
    } > DARAMOVL   /* defaults to PAGE 0 */

    /* BIOS code used once only */
    .biosOvlCode: {

    } > DARAMOVL   /* defaults to PAGE 0 */   

    /* 
     * Run-once init code. A subsection naming policy has been adopted. 
     * I/O Drivers and XDAIS create code *ALL* are assigned to 
     * .text:init sections
     */
    .initOvlcode {          
        *(.text:init)               /* app specific initialization code */      
    } > DARAMOVL   /* defaults to PAGE 0 */
                
} 

/* end of scratch buffer overlay section */


/* 
 *  Code sections that are linked in by conventional linker yet not 
 *  used or required for RF level 1. The linker has several directives
 *  to allow us to link 'out' such code sections e.g. DSECT, NOLOAD. 
 */
SECTIONS 
{
    .notusedialg {              /* unused XDAIS IALG algorithm code sections */
        *(.text:algAlloc)       /* IALG data buffers requested */
        *(.text:algNumAlloc)    /* num IALG buffers requested */
        *(.text:algControl)     /* algorithm specific control/status */
        *(.text:algFree)        /* IALG buffers to be freed */
    } type = DSECT > PRGDUMMY PAGE 0    /* Dummy (not real) program mem */

    .notusedexit {
        *(.text:exit)           /* app specific finalization code */
    } type = DSECT > PRGDUMMY PAGE 0    /* Dummy (not real) program mem */
} 
/* end of 'linked-out' section */


/* End of link.cmd */



