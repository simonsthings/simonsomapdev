/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSL_RESID_H_
#define _CSL_RESID_H_


/* ---- Chip/Device specific details ---- */
#ifdef CSL_CHIP_5910PG1_0

/* ---- Total number of modues : CSL Abstraction ---- */
/* Module ID 0 is reserved. */
#define CSL_CHIP_NUM_MODULES   5

#define CSL_DMA_ID                      (1)
#define CSL_TIMER_ID            (2)
#define CSL_MCBSP_ID            (3)
#define CSL_INTC_ID                     (4)

/* ---- Total Number of Resource, one-to-one correspondence with H/W resource */
/* Resource ID - 0 is resvd. */
#define CSL_CHIP_NUM_RESOURCES 13

#define CSL_DMA_0_CHA0_UID                      (1)
#define CSL_DMA_0_CHA1_UID                      (2)
#define CSL_DMA_0_CHA2_UID                      (3)
#define CSL_DMA_0_CHA3_UID                      (4)
#define CSL_DMA_0_CHA4_UID                      (5)
#define CSL_DMA_0_CHA5_UID                      (6)
#define CSL_TIMER_1_UID                         (7)
#define CSL_TIMER_2_UID                         (8)
#define CSL_TIMER_3_UID                         (9)
#define CSL_MCBSP_1_UID                         (10)
#define CSL_MCBSP_3_UID                         (11)

/*---- Auto-generated XIO masks for peripheral instances ---- */
#define CSL_DMA_0_CHA0_XIO                      (0x00000000)
#define CSL_DMA_0_CHA1_XIO                      (0x00000000)
#define CSL_DMA_0_CHA2_XIO                      (0x00000000)
#define CSL_DMA_0_CHA3_XIO                      (0x00000000)
#define CSL_DMA_0_CHA4_XIO                      (0x00000000)
#define CSL_DMA_0_CHA5_XIO                      (0x00000000)
#define CSL_TIMER_1_XIO                         (0x00000000)
#define CSL_TIMER_2_XIO                         (0x00000000)
#define CSL_TIMER_3_XIO                         (0x00000000)
#define CSL_MCBSP_1_XIO                         (0X00000000)
#define CSL_MCBSP_3_XIO                         (0X00000000)

#else
#error "CHIP symbol is not defined."
#endif /* CSL_CHIP_5910PG1_0 */

#endif /* _CSL_RESID_H_ */

