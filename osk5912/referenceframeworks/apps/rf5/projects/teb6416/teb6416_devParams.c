/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== teb6416_devParams.c ========
 *  TEB6416_EDMA_PCM3002 default driver parameters
 */
#include <std.h>
#include <teb6416_edma_pcm3002.h>

/*
 *  ======== TEB6416_DEVPARAMS ========
 *  This static initialization defines the default parameters used for
 *  TEB6416_EDMA_PCM3002 IOM driver
 */
TEB6416_EDMA_PCM3002_DevParams TEB6416_DEVPARAMS = {
    TEB6416_EDMA_PCM3002_VERSION_1,/* Version number */ 

    FALSE,                        /* Rx/TX buffer on the receiving */   \
                                  /* transmitting end of the EDMA  */
                                  /* FALSE -> buffers in onchip    */
                                  /* memory. No CACHE coherency    */
                                  /* calls                         */
                                  /* TRUE -> buffers in offchip    */
                                  /* memory.  Coherency calls made */
                                  /* in driver                     */
    TEB6416_EDMA_PCM3002_EDMA_ID_DEFAULT, /* Default IRQ # for EDMA is 8 */ 
    TEB6416_EDMA_PCM3002_MR0_DEFAULT,  /* Default value for reg MR0 */
    TEB6416_EDMA_PCM3002_MR1_DEFAULT,  /* Default value for reg MR1 */      
    TEB6416_EDMA_PCM3002_MR2_DEFAULT,  /* Default value for reg MR2 */      
    TEB6416_EDMA_PCM3002_MR3_DEFAULT,  /* Default value for reg MR3 */      
    TEB6416_EDMA_PCM3002_INTR_MASK_DEFAULT, /* Interrupt mask = 1 */
    EDMA_OPT_PRI_HIGH                       /* EDMA transfers priority */
};


