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
-l pio.l54
-l c5402_dma_mcbsp.l54
-l dsk5402_dma_ad50.l54

/* ALGMIN instantiates XDAIS algos in a 'static' fashion */
-l algmin.l54

/* UTL debugging module for seamless inclusion/removal of LOG, STS usage */
-l utl.l54

/* XDAIS algorithm(s) */
-l fir_ti.l54

/* Point to the algorithm vendor's IMOD v-table (name located in mod_ven.h) */
_FIR_IFIR = _FIR_TI_IFIR;


/* 
 *  we *MUST* always fill/initialize BufIdX0 since XDAIS requires
 *  the buffer pointed to in memTab[0] to be initialized to contain all zeros.
 */
SECTIONS
{       
    .bss:firChanBufId00:      fill = 0x0  {} > IDATA PAGE 1

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
    UNION: run = IDATAOVL PAGE 1 
    {
        GROUP: {                        
            .bss:firChanBufId02Scr: {}  
        }
        
    }    
} 

/* end of scratch buffer overlay section */


/* 
 *  Overlays of code and data when code is used only at initialization time
 *  Goal is to reduce Program Memory usage.
 *
 *  Pre-requisites for overlay : 
 *  - unified Program and Data memory is available (e.g 54xx DARAM)
 *  - the data which we overlay the code with is NOT used until after
 *    completion of the run-once code. 
 *  - the data must be uninitialized
 */
SECTIONS 
{     
    /*
     *  NOTE - BIOS data sections have ALREADY been linked to IDATAOVL
     *  memory section as the data overlaying the code below. For example,
     *  the PIP's being used were mapped to IDATAOVL in the configuration.
     *
     *  Place your own app data here that meets the prereqs listed above.
     */
               
    /* Chip Support Library code used once only */  
    .cslOvlCode: {
        
        /* These CSL fxns are run once only, before overlaid data is used */
        *(.text:TIMER_open)
        *(.text:MCBSP_open)
        *(.text:MCBSP_reset)
        *(.text:MCBSP_config)
        *(.text:DMA_open)
        *(.text:DMA_reset)
        *(.text:DMA_config)
        *(.text:CSL_cfgInit)
        
    } >         IPROGOVL PAGE 0

    /* XDAIS IALG code used once only */
    .ialgOvlCode: {
        
    } >         IPROGOVL PAGE 0

    /* BIOS code used once only */
    .biosOvlCode: {

    } >         IPROGOVL PAGE 0   

    /* 
     * Run-once init code. A subsection naming policy has been adopted. I/O 
     * Drivers and XDAIS create code *ALL* are assigned to .text:init sections
     */
    .initOvlcode {          
        *(.text:init)               /* app specific initialization code */      
    } >         IPROGOVL PAGE 0
    
} 
/* end of overlay program code section */


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



