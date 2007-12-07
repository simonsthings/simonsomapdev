/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 *                                                   *
 * "@(#) PSP/CSL  3.0.0.0  (2003-09-30)              *
\*****************************************************/

#ifndef _CSL_RESID_H_
#define _CSL_RESID_H_


/* ---- Total number of modues : CSL Abstraction ---- */
/* Module ID 0 is reserved. */
#define CSL_CHIP_NUM_MODULES   8

#define CSL_DMA_ID          (1)
#define CSL_TIMER_ID        (2)
#define CSL_MCBSP_ID        (3)
#define CSL_INTC_ID         (4)
#define CSL_UART_ID         (5)
#define CSL_IRDA_ID         (6)
#define CSL_I2C_ID          (7)
#define CSL_CLKRST_ID       (8)

/* ---- Total Number of Resource, one-to-one correspondence with H/W resource */
/* Resource ID - 0 is resvd. */
#define CSL_CHIP_NUM_RESOURCES    18

#define CSL_DMA_1_CHA0_UID          (1)
#define CSL_DMA_1_CHA1_UID          (2)
#define CSL_DMA_1_CHA2_UID          (3)
#define CSL_DMA_1_CHA3_UID          (4)
#define CSL_DMA_1_CHA4_UID          (5)
#define CSL_DMA_1_CHA5_UID          (6)
#define CSL_TIMER_1_UID             (7)
#define CSL_TIMER_2_UID             (8)
#define CSL_TIMER_3_UID             (9)
#define CSL_MCBSP_1_UID             (10)
#define CSL_MCBSP_2_UID             (11)
#define CSL_MCBSP_3_UID             (12)
#define CSL_UART_1_UID              (13)
#define CSL_UART_2_UID              (14)
#define CSL_UART_3_UID              (15)
#define CSL_IRDA_1_UID              (CSL_UART_1_UID)
#define CSL_IRDA_3_UID              (CSL_UART_3_UID)
#define CSL_I2C_1_UID               (16)
#define CSL_CLKRST_1_UID            (17)
#define CSL_SSW_1_UID               (18)

/*---- Auto-generated XIO masks for peripheral instances ---- */
#define CSL_DMA_1_CHA0_XIO          (0x00000000)
#define CSL_DMA_1_CHA1_XIO          (0x00000000)
#define CSL_DMA_1_CHA2_XIO          (0x00000000)
#define CSL_DMA_1_CHA3_XIO          (0x00000000)
#define CSL_DMA_1_CHA4_XIO          (0x00000000)
#define CSL_DMA_1_CHA5_XIO          (0x00000000)
#define CSL_TIMER_1_XIO             (0x00000000)
#define CSL_TIMER_2_XIO             (0x00000000)
#define CSL_TIMER_3_XIO             (0x00000000)
#define CSL_MCBSP_1_XIO             (0x00000000)
#define CSL_MCBSP_2_XIO             (0x00000000)
#define CSL_MCBSP_3_XIO             (0x00000000)
#define CSL_UART_1_XIO              (0x00000000)
#define CSL_UART_2_XIO              (0x00000000)
#define CSL_UART_3_XIO              (0x00000000)
#define CSL_IRDA_1_XIO              (CSL_UART_1_XIO)
#define CSL_IRDA_3_XIO              (CSL_UART_3_XIO)
#define CSL_I2C_1_XIO               (0x00000000)
#define CSL_CLKRST_1_XIO            (0x00000000)
#define CSL_SSW_1_XIO               (0x00000000)

#endif /* _CSL_RESID_H_ */

