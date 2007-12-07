#ifndef _CSLR_UART_001_H_
#define _CSLR_UART_001_H_
/*********************************************************************
 * Copyright (C) 2003-2004 Texas Instruments Incorporated.
 * All Rights Reserved
 *********************************************************************/
 /** \file cslr_uart_001.h
 *
 * \brief This file contains the Register Desciptions for UART
 *
 *********************************************************************/

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint8 RHR_THR_DLL;
    const char RSVD0[1];
    volatile Uint8 IER_DLH;
    const char RSVD1[1];
    volatile Uint8 IIR_FCR_EFR;
    const char RSVD2[1];
    volatile Uint8 LCR;

    const char RSVD3[1];
    volatile Uint8 MCR_XON1;
    const char RSVD4[1];
    volatile Uint8 LSR_XON2;
    const char RSVD5[1];
    volatile Uint8 MSR_TCR_XOFF1;
    const char RSVD6[1];
    volatile Uint8 SPR_TLR_XOFF2;
    const char RSVD7[1];
    volatile Uint8 MDR1;
    const char RSVD8[11];
    volatile Uint8 UASR;
    const char RSVD9[3];
    volatile Uint8 SCR;
    const char RSVD10[1];
    volatile Uint8 SSR;
    const char RSVD11[5];
    volatile Uint8 MVR;
    const char RSVD12[1];
    volatile Uint8 SYSC;
    const char RSVD13[1];
    volatile Uint8 SYSS;
    const char RSVD14[1];
    volatile Uint8 WER;
} CSL_UartRegs;


/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* RHR_THR_DLL */

#define CSL_UART_RHR_THR_DLL_RHR_THR_DLL_MASK (0x000000FFu)
#define CSL_UART_RHR_THR_DLL_RHR_THR_DLL_SHIFT (0x00000000u)
#define CSL_UART_RHR_THR_DLL_RHR_THR_DLL_RESETVAL (0x00000000u)

#define CSL_UART_RHR_THR_DLL_RESETVAL    (0x00000000u)

/* RHR */

#define CSL_UART_RHR_RHR_MASK            (0x000000FFu)
#define CSL_UART_RHR_RHR_SHIFT           (0x00000000u)
#define CSL_UART_RHR_RHR_RESETVAL        (0x00000000u)

#define CSL_UART_RHR_RESETVAL            (0x00000000u)

/* THR */

#define CSL_UART_THR_THR_MASK            (0x000000FFu)
#define CSL_UART_THR_THR_SHIFT           (0x00000000u)
#define CSL_UART_THR_THR_RESETVAL        (0x00000000u)

#define CSL_UART_THR_RESETVAL            (0x00000000u)

/* DLL */

#define CSL_UART_DLL_CLOCK_LSB_MASK      (0x000000FFu)
#define CSL_UART_DLL_CLOCK_LSB_SHIFT     (0x00000000u)
#define CSL_UART_DLL_CLOCK_LSB_RESETVAL  (0x00000000u)

#define CSL_UART_DLL_RESETVAL            (0x00000000u)

/* IER_DLH */

#define CSL_UART_IER_DLH_IER_DLH_MASK    (0x000000FFu)
#define CSL_UART_IER_DLH_IER_DLH_SHIFT   (0x00000000u)
#define CSL_UART_IER_DLH_IER_DLH_RESETVAL (0x00000000u)

#define CSL_UART_IER_DLH_RESETVAL        (0x00000000u)

/* IER */

#define CSL_UART_IER_CTS_IT_MASK         (0x00000080u)
#define CSL_UART_IER_CTS_IT_SHIFT        (0x00000007u)
#define CSL_UART_IER_CTS_IT_RESETVAL     (0x00000000u)

#define CSL_UART_IER_RTS_IT_MASK         (0x00000040u)
#define CSL_UART_IER_RTS_IT_SHIFT        (0x00000006u)
#define CSL_UART_IER_RTS_IT_RESETVAL     (0x00000000u)

#define CSL_UART_IER_XOFF_IT_MASK        (0x00000020u)
#define CSL_UART_IER_XOFF_IT_SHIFT       (0x00000005u)
#define CSL_UART_IER_XOFF_IT_RESETVAL    (0x00000000u)

#define CSL_UART_IER_SLEEP_MODE_MASK     (0x00000010u)
#define CSL_UART_IER_SLEEP_MODE_SHIFT    (0x00000004u)
#define CSL_UART_IER_SLEEP_MODE_RESETVAL (0x00000000u)

#define CSL_UART_IER_MODEM_STS_IT_MASK   (0x00000008u)
#define CSL_UART_IER_MODEM_STS_IT_SHIFT  (0x00000003u)
#define CSL_UART_IER_MODEM_STS_IT_RESETVAL (0x00000000u)

#define CSL_UART_IER_LINE_STS_IT_MASK    (0x00000004u)
#define CSL_UART_IER_LINE_STS_IT_SHIFT   (0x00000002u)
#define CSL_UART_IER_LINE_STS_IT_RESETVAL (0x00000000u)

#define CSL_UART_IER_THR_IT_MASK         (0x00000002u)
#define CSL_UART_IER_THR_IT_SHIFT        (0x00000001u)
#define CSL_UART_IER_THR_IT_RESETVAL     (0x00000000u)

#define CSL_UART_IER_RHR_IT_MASK         (0x00000001u)
#define CSL_UART_IER_RHR_IT_SHIFT        (0x00000000u)
#define CSL_UART_IER_RHR_IT_RESETVAL     (0x00000000u)

#define CSL_UART_IER_RESETVAL            (0x00000000u)

/* DLH */

#define CSL_UART_DLH_CLOCK_MSB_MASK      (0x0000003Fu)
#define CSL_UART_DLH_CLOCK_MSB_SHIFT     (0x00000000u)
#define CSL_UART_DLH_CLOCK_MSB_RESETVAL  (0x00000000u)

#define CSL_UART_DLH_RESETVAL            (0x00000000u)

/* IIR_FCR_EFR */

#define CSL_UART_IIR_FCR_EFR_IIR_FCR_EFR_MASK (0x000000FFu)
#define CSL_UART_IIR_FCR_EFR_IIR_FCR_EFR_SHIFT (0x00000000u)
#define CSL_UART_IIR_FCR_EFR_IIR_FCR_EFR_RESETVAL (0x00000000u)

#define CSL_UART_IIR_FCR_EFR_RESETVAL    (0x00000000u)

/* IIR */

#define CSL_UART_IIR_FCR_MIRROR_MASK     (0x000000C0u)
#define CSL_UART_IIR_FCR_MIRROR_SHIFT    (0x00000006u)
#define CSL_UART_IIR_FCR_MIRROR_RESETVAL (0x00000000u)
#define CSL_UART_IIR_FCR_MIRROR_DISABLE  (0x00000000u)
#define CSL_UART_IIR_FCR_MIRROR_ENABLE   (0x00000003u)

#define CSL_UART_IIR_IT_TYPE_MASK        (0x0000003Eu)
#define CSL_UART_IIR_IT_TYPE_SHIFT       (0x00000001u)
#define CSL_UART_IIR_IT_TYPE_RESETVAL    (0x00000000u)
#define CSL_UART_IIR_IT_TYPE_RX_LINE_STATUS (0x00000003u)
#define CSL_UART_IIR_IT_TYPE_RX_TIMEOUT  (0x00000006u)
#define CSL_UART_IIR_IT_TYPE_RHR         (0x00000002u)
#define CSL_UART_IIR_IT_TYPE_THR         (0x00000001u)
#define CSL_UART_IIR_IT_TYPE_MSR         (0x00000000u)
#define CSL_UART_IIR_IT_TYPE_XOFF_SPCHAR (0x00000008u)
#define CSL_UART_IIR_IT_TYPE_RCD         (0x00000010u)

#define CSL_UART_IIR_IT_PENDING_MASK     (0x00000001u)
#define CSL_UART_IIR_IT_PENDING_SHIFT    (0x00000000u)
#define CSL_UART_IIR_IT_PENDING_RESETVAL (0x00000001u)
#define CSL_UART_IIR_IT_PENDING_NONE     (0x00000001u)
#define CSL_UART_IIR_IT_PENDING_PEND     (0x00000000u)

#define CSL_UART_IIR_RESETVAL            (0x00000001u)

/* EFR */

#define CSL_UART_EFR_AUTO_CTS_EN_MASK    (0x00000080u)
#define CSL_UART_EFR_AUTO_CTS_EN_SHIFT   (0x00000007u)
#define CSL_UART_EFR_AUTO_CTS_EN_RESETVAL (0x00000000u)
#define CSL_UART_EFR_AUTO_CTS_EN_DISABLE (0x00000000u)
#define CSL_UART_EFR_AUTO_CTS_EN_ENABLE  (0x00000001u)

#define CSL_UART_EFR_AUTO_RTS_EN_MASK    (0x00000040u)
#define CSL_UART_EFR_AUTO_RTS_EN_SHIFT   (0x00000006u)
#define CSL_UART_EFR_AUTO_RTS_EN_RESETVAL (0x00000000u)
#define CSL_UART_EFR_AUTO_RTS_EN_DISABLE (0x00000000u)
#define CSL_UART_EFR_AUTO_RTS_EN_ENABLE  (0x00000001u)

#define CSL_UART_EFR_SPECIAL_CHAR_DETECT_MASK (0x00000020u)
#define CSL_UART_EFR_SPECIAL_CHAR_DETECT_SHIFT (0x00000005u)
#define CSL_UART_EFR_SPECIAL_CHAR_DETECT_RESETVAL (0x00000000u)
#define CSL_UART_EFR_SPECIAL_CHAR_DETECT_DISABLE (0x00000000u)
#define CSL_UART_EFR_SPECIAL_CHAR_DETECT_ENABLE (0x00000001u)

#define CSL_UART_EFR_ENHANCED_EN_MASK    (0x00000010u)
#define CSL_UART_EFR_ENHANCED_EN_SHIFT   (0x00000004u)
#define CSL_UART_EFR_ENHANCED_EN_RESETVAL (0x00000000u)
#define CSL_UART_EFR_ENHANCED_EN_DISABLE (0x00000000u)
#define CSL_UART_EFR_ENHANCED_EN_ENABLE  (0x00000001u)

#define CSL_UART_EFR_SW_FLOW_CONTROL_MASK (0x0000000Fu)
#define CSL_UART_EFR_SW_FLOW_CONTROL_SHIFT (0x00000000u)
#define CSL_UART_EFR_SW_FLOW_CONTROL_RESETVAL (0x00000000u)

#define CSL_UART_EFR_RESETVAL            (0x00000000u)

/* FCR */

#define CSL_UART_FCR_RX_FIFO_TRIG_MASK   (0x000000C0u)
#define CSL_UART_FCR_RX_FIFO_TRIG_SHIFT  (0x00000006u)
#define CSL_UART_FCR_RX_FIFO_TRIG_RESETVAL (0x00000000u)

#define CSL_UART_FCR_TX_FIFO_TRIG_MASK   (0x00000030u)
#define CSL_UART_FCR_TX_FIFO_TRIG_SHIFT  (0x00000004u)
#define CSL_UART_FCR_TX_FIFO_TRIG_RESETVAL (0x00000000u)

#define CSL_UART_FCR_DMA_MODE_MASK       (0x00000008u)
#define CSL_UART_FCR_DMA_MODE_SHIFT      (0x00000003u)
#define CSL_UART_FCR_DMA_MODE_RESETVAL   (0x00000000u)

#define CSL_UART_FCR_TX_FIFO_CLEAR_MASK  (0x00000004u)
#define CSL_UART_FCR_TX_FIFO_CLEAR_SHIFT (0x00000002u)
#define CSL_UART_FCR_TX_FIFO_CLEAR_RESETVAL (0x00000000u)
#define CSL_UART_FCR_TX_FIFO_CLEAR_DISABLE (0x00000000u)
#define CSL_UART_FCR_TX_FIFO_CLEAR_ENABLE (0x00000001u)

#define CSL_UART_FCR_RX_FIFO_CLEAR_MASK  (0x00000002u)
#define CSL_UART_FCR_RX_FIFO_CLEAR_SHIFT (0x00000001u)
#define CSL_UART_FCR_RX_FIFO_CLEAR_RESETVAL (0x00000000u)
#define CSL_UART_FCR_RX_FIFO_CLEAR_DISABLE (0x00000000u)
#define CSL_UART_FCR_RX_FIFO_CLEAR_ENABLE (0x00000001u)

#define CSL_UART_FCR_FIFO_EN_MASK        (0x00000001u)
#define CSL_UART_FCR_FIFO_EN_SHIFT       (0x00000000u)
#define CSL_UART_FCR_FIFO_EN_RESETVAL    (0x00000000u)
#define CSL_UART_FCR_FIFO_EN_DISABLE     (0x00000000u)
#define CSL_UART_FCR_FIFO_EN_ENABLE      (0x00000001u)

#define CSL_UART_FCR_RESETVAL            (0x00000000u)

/* LCR */

#define CSL_UART_LCR_DIV_EN_MASK         (0x00000080u)
#define CSL_UART_LCR_DIV_EN_SHIFT        (0x00000007u)
#define CSL_UART_LCR_DIV_EN_RESETVAL     (0x00000000u)
#define CSL_UART_LCR_DIV_EN_DISABLE      (0x00000000u)
#define CSL_UART_LCR_DIV_EN_ENABLE       (0x00000001u)

#define CSL_UART_LCR_BREAK_EN_MASK       (0x00000040u)
#define CSL_UART_LCR_BREAK_EN_SHIFT      (0x00000006u)
#define CSL_UART_LCR_BREAK_EN_RESETVAL   (0x00000000u)
#define CSL_UART_LCR_BREAK_EN_DISABLE    (0x00000000u)
#define CSL_UART_LCR_BREAK_EN_ENABLE     (0x00000001u)

#define CSL_UART_LCR_PARITY_TYPE2_MASK   (0x00000020u)
#define CSL_UART_LCR_PARITY_TYPE2_SHIFT  (0x00000005u)
#define CSL_UART_LCR_PARITY_TYPE2_RESETVAL (0x00000000u)

#define CSL_UART_LCR_PARITY_TYPE1_MASK   (0x00000010u)
#define CSL_UART_LCR_PARITY_TYPE1_SHIFT  (0x00000004u)
#define CSL_UART_LCR_PARITY_TYPE1_RESETVAL (0x00000000u)

#define CSL_UART_LCR_PARITY_EN_MASK      (0x00000008u)
#define CSL_UART_LCR_PARITY_EN_SHIFT     (0x00000003u)
#define CSL_UART_LCR_PARITY_EN_RESETVAL  (0x00000000u)

#define CSL_UART_LCR_NB_STOP_MASK        (0x00000004u)
#define CSL_UART_LCR_NB_STOP_SHIFT       (0x00000002u)
#define CSL_UART_LCR_NB_STOP_RESETVAL    (0x00000000u)
#define CSL_UART_LCR_NB_STOP_01          (0x00000000u)
#define CSL_UART_LCR_NB_STOP_1_5         (0x00000001u)
#define CSL_UART_LCR_NB_STOP_02          (0x00000001u)

#define CSL_UART_LCR_CHAR_LENGTH_MASK    (0x00000003u)
#define CSL_UART_LCR_CHAR_LENGTH_SHIFT   (0x00000000u)
#define CSL_UART_LCR_CHAR_LENGTH_RESETVAL (0x00000000u)
#define CSL_UART_LCR_CHAR_LENGTH_05      (0x00000000u)
#define CSL_UART_LCR_CHAR_LENGTH_06      (0x00000001u)
#define CSL_UART_LCR_CHAR_LENGTH_07      (0x00000002u)
#define CSL_UART_LCR_CHAR_LENGTH_08      (0x00000003u)

#define CSL_UART_LCR_RESETVAL            (0x00000000u)

/* MCR_XON1 */

#define CSL_UART_MCR_XON1_MCR_XON1_MASK  (0x000000FFu)
#define CSL_UART_MCR_XON1_MCR_XON1_SHIFT (0x00000000u)
#define CSL_UART_MCR_XON1_MCR_XON1_RESETVAL (0x00000000u)

#define CSL_UART_MCR_XON1_RESETVAL       (0x00000000u)

/* MCR */

#define CSL_UART_MCR_TCR_TLR_MASK        (0x00000040u)
#define CSL_UART_MCR_TCR_TLR_SHIFT       (0x00000006u)
#define CSL_UART_MCR_TCR_TLR_RESETVAL    (0x00000000u)
#define CSL_UART_MCR_TCR_TLR_DISABLE     (0x00000000u)
#define CSL_UART_MCR_TCR_TLR_ENABLE      (0x00000001u)

#define CSL_UART_MCR_XON_EN_MASK         (0x00000020u)
#define CSL_UART_MCR_XON_EN_SHIFT        (0x00000005u)
#define CSL_UART_MCR_XON_EN_RESETVAL     (0x00000000u)
#define CSL_UART_MCR_XON_EN_DISABLE      (0x00000000u)
#define CSL_UART_MCR_XON_EN_ENABLE       (0x00000001u)

#define CSL_UART_MCR_LOOPBACK_EN_MASK    (0x00000010u)
#define CSL_UART_MCR_LOOPBACK_EN_SHIFT   (0x00000004u)
#define CSL_UART_MCR_LOOPBACK_EN_RESETVAL (0x00000000u)
#define CSL_UART_MCR_LOOPBACK_EN_DISABLE (0x00000000u)
#define CSL_UART_MCR_LOOPBACK_EN_ENABLE  (0x00000001u)

#define CSL_UART_MCR_CD_STS_CH_MASK      (0x00000008u)
#define CSL_UART_MCR_CD_STS_CH_SHIFT     (0x00000003u)
#define CSL_UART_MCR_CD_STS_CH_RESETVAL  (0x00000000u)
#define CSL_UART_MCR_CD_STS_CH_HIGH      (0x00000000u)
#define CSL_UART_MCR_CD_STS_CH_LOW       (0x00000001u)

#define CSL_UART_MCR_RI_STS_CH_MASK      (0x00000004u)
#define CSL_UART_MCR_RI_STS_CH_SHIFT     (0x00000002u)
#define CSL_UART_MCR_RI_STS_CH_RESETVAL  (0x00000000u)
#define CSL_UART_MCR_RI_STS_CH_HIGH      (0x00000000u)
#define CSL_UART_MCR_RI_STS_CH_LOW       (0x00000001u)

#define CSL_UART_MCR_RTS_MASK            (0x00000002u)
#define CSL_UART_MCR_RTS_SHIFT           (0x00000001u)
#define CSL_UART_MCR_RTS_RESETVAL        (0x00000000u)
#define CSL_UART_MCR_RTS_DISABLE         (0x00000000u)
#define CSL_UART_MCR_RTS_ENABLE          (0x00000001u)

#define CSL_UART_MCR_DTR_MASK            (0x00000001u)
#define CSL_UART_MCR_DTR_SHIFT           (0x00000000u)
#define CSL_UART_MCR_DTR_RESETVAL        (0x00000000u)
#define CSL_UART_MCR_DTR_DISABLE         (0x00000000u)
#define CSL_UART_MCR_DTR_ENABLE          (0x00000001u)

#define CSL_UART_MCR_RESETVAL            (0x00000000u)

/* XON1 */

#define CSL_UART_XON1_XON_WORD1_MASK     (0x000000FFu)
#define CSL_UART_XON1_XON_WORD1_SHIFT    (0x00000000u)
#define CSL_UART_XON1_XON_WORD1_RESETVAL (0x00000000u)

#define CSL_UART_XON1_RESETVAL           (0x00000000u)

/* LSR_XON2 */

#define CSL_UART_LSR_XON2_LSR_XON2_MASK  (0x000000FFu)
#define CSL_UART_LSR_XON2_LSR_XON2_SHIFT (0x00000000u)
#define CSL_UART_LSR_XON2_LSR_XON2_RESETVAL (0x00000000u)

#define CSL_UART_LSR_XON2_RESETVAL       (0x00000000u)

/* LSR */

#define CSL_UART_LSR_RX_FIFO_STS_MASK    (0x00000080u)
#define CSL_UART_LSR_RX_FIFO_STS_SHIFT   (0x00000007u)
#define CSL_UART_LSR_RX_FIFO_STS_RESETVAL (0x00000000u)

#define CSL_UART_LSR_TX_SR_E_MASK        (0x00000040u)
#define CSL_UART_LSR_TX_SR_E_SHIFT       (0x00000006u)
#define CSL_UART_LSR_TX_SR_E_RESETVAL    (0x00000001u)

#define CSL_UART_LSR_TX_FIFO_E_MASK      (0x00000020u)
#define CSL_UART_LSR_TX_FIFO_E_SHIFT     (0x00000005u)
#define CSL_UART_LSR_TX_FIFO_E_RESETVAL  (0x00000001u)

#define CSL_UART_LSR_RX_BI_MASK          (0x00000010u)
#define CSL_UART_LSR_RX_BI_SHIFT         (0x00000004u)
#define CSL_UART_LSR_RX_BI_RESETVAL      (0x00000000u)

#define CSL_UART_LSR_RX_FE_MASK          (0x00000008u)
#define CSL_UART_LSR_RX_FE_SHIFT         (0x00000003u)
#define CSL_UART_LSR_RX_FE_RESETVAL      (0x00000000u)

#define CSL_UART_LSR_RX_PE_MASK          (0x00000004u)
#define CSL_UART_LSR_RX_PE_SHIFT         (0x00000002u)
#define CSL_UART_LSR_RX_PE_RESETVAL      (0x00000000u)

#define CSL_UART_LSR_RX_OE_MASK          (0x00000002u)
#define CSL_UART_LSR_RX_OE_SHIFT         (0x00000001u)
#define CSL_UART_LSR_RX_OE_RESETVAL      (0x00000000u)

#define CSL_UART_LSR_RX_FIFO_E_MASK      (0x00000001u)
#define CSL_UART_LSR_RX_FIFO_E_SHIFT     (0x00000000u)
#define CSL_UART_LSR_RX_FIFO_E_RESETVAL  (0x00000000u)

#define CSL_UART_LSR_RESETVAL            (0x00000060u)

/* XON2 */

#define CSL_UART_XON2_XON_WORD2_MASK     (0x000000FFu)
#define CSL_UART_XON2_XON_WORD2_SHIFT    (0x00000000u)
#define CSL_UART_XON2_XON_WORD2_RESETVAL (0x00000000u)

#define CSL_UART_XON2_RESETVAL           (0x00000000u)

/* MSR_TCR_XOFF1 */

#define CSL_UART_MSR_TCR_XOFF1_MSR_TCR_XOFF1_MASK (0x000000FFu)
#define CSL_UART_MSR_TCR_XOFF1_MSR_TCR_XOFF1_SHIFT (0x00000000u)
#define CSL_UART_MSR_TCR_XOFF1_MSR_TCR_XOFF1_RESETVAL (0x00000000u)

#define CSL_UART_MSR_TCR_XOFF1_RESETVAL  (0x00000000u)

/* MSR */

#define CSL_UART_MSR_NCD_STS_MASK        (0x00000080u)
#define CSL_UART_MSR_NCD_STS_SHIFT       (0x00000007u)
#define CSL_UART_MSR_NCD_STS_RESETVAL    (0x00000000u)

#define CSL_UART_MSR_NRI_STS_MASK        (0x00000040u)
#define CSL_UART_MSR_NRI_STS_SHIFT       (0x00000006u)
#define CSL_UART_MSR_NRI_STS_RESETVAL    (0x00000000u)

#define CSL_UART_MSR_NDSR_STS_MASK       (0x00000020u)
#define CSL_UART_MSR_NDSR_STS_SHIFT      (0x00000005u)
#define CSL_UART_MSR_NDSR_STS_RESETVAL   (0x00000000u)

#define CSL_UART_MSR_NCTS_STS_MASK       (0x00000010u)
#define CSL_UART_MSR_NCTS_STS_SHIFT      (0x00000004u)
#define CSL_UART_MSR_NCTS_STS_RESETVAL   (0x00000000u)

#define CSL_UART_MSR_DCD_STS_MASK        (0x00000008u)
#define CSL_UART_MSR_DCD_STS_SHIFT       (0x00000003u)
#define CSL_UART_MSR_DCD_STS_RESETVAL    (0x00000000u)

#define CSL_UART_MSR_RI_STS_MASK         (0x00000004u)
#define CSL_UART_MSR_RI_STS_SHIFT        (0x00000002u)
#define CSL_UART_MSR_RI_STS_RESETVAL     (0x00000000u)

#define CSL_UART_MSR_DSR_STS_MASK        (0x00000002u)
#define CSL_UART_MSR_DSR_STS_SHIFT       (0x00000001u)
#define CSL_UART_MSR_DSR_STS_RESETVAL    (0x00000000u)

#define CSL_UART_MSR_CTS_STS_MASK        (0x00000001u)
#define CSL_UART_MSR_CTS_STS_SHIFT       (0x00000000u)
#define CSL_UART_MSR_CTS_STS_RESETVAL    (0x00000000u)

#define CSL_UART_MSR_RESETVAL            (0x00000000u)

/* XOFF1 */

#define CSL_UART_XOFF1_XOFF_WORD1_MASK   (0x000000FFu)
#define CSL_UART_XOFF1_XOFF_WORD1_SHIFT  (0x00000000u)
#define CSL_UART_XOFF1_XOFF_WORD1_RESETVAL (0x00000000u)

#define CSL_UART_XOFF1_RESETVAL          (0x00000000u)

/* TCR */

#define CSL_UART_TCR_RX_FIFO_TRIG_START_MASK (0x000000F0u)
#define CSL_UART_TCR_RX_FIFO_TRIG_START_SHIFT (0x00000004u)
#define CSL_UART_TCR_RX_FIFO_TRIG_START_RESETVAL (0x00000000u)

#define CSL_UART_TCR_RX_FIFO_TRIG_HALT_MASK (0x0000000Fu)
#define CSL_UART_TCR_RX_FIFO_TRIG_HALT_SHIFT (0x00000000u)
#define CSL_UART_TCR_RX_FIFO_TRIG_HALT_RESETVAL (0x0000000Fu)

#define CSL_UART_TCR_RESETVAL            (0x0000000Fu)

/* SPR_TLR_XOFF2 */

#define CSL_UART_SPR_TLR_XOFF2_SPR_TLR_XOFF2_MASK (0x000000FFu)
#define CSL_UART_SPR_TLR_XOFF2_SPR_TLR_XOFF2_SHIFT (0x00000000u)
#define CSL_UART_SPR_TLR_XOFF2_SPR_TLR_XOFF2_RESETVAL (0x00000000u)

#define CSL_UART_SPR_TLR_XOFF2_RESETVAL  (0x00000000u)

/* XOFF2 */

#define CSL_UART_XOFF2_XOFF_WORD2_MASK   (0x000000FFu)
#define CSL_UART_XOFF2_XOFF_WORD2_SHIFT  (0x00000000u)
#define CSL_UART_XOFF2_XOFF_WORD2_RESETVAL (0x00000000u)

#define CSL_UART_XOFF2_RESETVAL          (0x00000000u)

/* SPR */

#define CSL_UART_SPR_SPR_WORD_MASK       (0x000000FFu)
#define CSL_UART_SPR_SPR_WORD_SHIFT      (0x00000000u)
#define CSL_UART_SPR_SPR_WORD_RESETVAL   (0x00000000u)

#define CSL_UART_SPR_RESETVAL            (0x00000000u)

/* TLR */

#define CSL_UART_TLR_RX_FIFO_TRIG_DMA_MASK (0x000000F0u)
#define CSL_UART_TLR_RX_FIFO_TRIG_DMA_SHIFT (0x00000004u)
#define CSL_UART_TLR_RX_FIFO_TRIG_DMA_RESETVAL (0x00000000u)

#define CSL_UART_TLR_TX_FIFO_TRIG_DMA_MASK (0x0000000Fu)
#define CSL_UART_TLR_TX_FIFO_TRIG_DMA_SHIFT (0x00000000u)
#define CSL_UART_TLR_TX_FIFO_TRIG_DMA_RESETVAL (0x00000000u)

#define CSL_UART_TLR_RESETVAL            (0x00000000u)

/* MDR1 */

#define CSL_UART_MDR1_FRAME_END_MODE_MASK (0x00000080u)
#define CSL_UART_MDR1_FRAME_END_MODE_SHIFT (0x00000007u)
#define CSL_UART_MDR1_FRAME_END_MODE_RESETVAL (0x00000000u)
#define CSL_UART_MDR1_FRAME_END_MODE_FRM_LEN (0x00000000u)
#define CSL_UART_MDR1_FRAME_END_MODE_EOT (0x00000001u)

#define CSL_UART_MDR1_SIP_MODE_MASK      (0x00000040u)
#define CSL_UART_MDR1_SIP_MODE_SHIFT     (0x00000006u)
#define CSL_UART_MDR1_SIP_MODE_RESETVAL  (0x00000000u)
#define CSL_UART_MDR1_SIP_MODE_MANUAL    (0x00000000u)
#define CSL_UART_MDR1_SIP_MODE_AUTOMATIC (0x00000001u)

#define CSL_UART_MDR1_SCT_MASK           (0x00000020u)
#define CSL_UART_MDR1_SCT_SHIFT          (0x00000005u)
#define CSL_UART_MDR1_SCT_RESETVAL       (0x00000000u)
#define CSL_UART_MDR1_SCT_DISABLE        (0x00000000u)
#define CSL_UART_MDR1_SCT_ENABLE         (0x00000001u)

#define CSL_UART_MDR1_SET_TXIR_MASK      (0x00000010u)
#define CSL_UART_MDR1_SET_TXIR_SHIFT     (0x00000004u)
#define CSL_UART_MDR1_SET_TXIR_RESETVAL  (0x00000000u)
#define CSL_UART_MDR1_SET_TXIR_NONE      (0x00000000u)
#define CSL_UART_MDR1_SET_TXIR_HIGH      (0x00000001u)

#define CSL_UART_MDR1_IR_SLEEP_MASK      (0x00000008u)
#define CSL_UART_MDR1_IR_SLEEP_SHIFT     (0x00000003u)
#define CSL_UART_MDR1_IR_SLEEP_RESETVAL  (0x00000000u)
#define CSL_UART_MDR1_IR_SLEEP_DISABLE   (0x00000000u)
#define CSL_UART_MDR1_IR_SLEEP_ENABLE    (0x00000001u)

#define CSL_UART_MDR1_MODE_SELECT_MASK   (0x00000007u)
#define CSL_UART_MDR1_MODE_SELECT_SHIFT  (0x00000000u)
#define CSL_UART_MDR1_MODE_SELECT_RESETVAL (0x00000007u)
#define CSL_UART_MDR1_MODE_SELECT_16X    (0x00000000u)
#define CSL_UART_MDR1_MODE_SELECT_SIR    (0x00000001u)
#define CSL_UART_MDR1_MODE_SELECT_16XAUTOBAUD (0x00000002u)
#define CSL_UART_MDR1_MODE_SELECT_13X    (0x00000003u)
#define CSL_UART_MDR1_MODE_SELECT_MIR    (0x00000004u)
#define CSL_UART_MDR1_MODE_SELECT_FIR    (0x00000005u)
#define CSL_UART_MDR1_MODE_SELECT_DISABLE (0x00000007u)

#define CSL_UART_MDR1_RESETVAL           (0x00000007u)

/* UASR */

#define CSL_UART_UASR_PARITY_TYPE_MASK   (0x000000C0u)
#define CSL_UART_UASR_PARITY_TYPE_SHIFT  (0x00000006u)
#define CSL_UART_UASR_PARITY_TYPE_RESETVAL (0x00000000u)
#define CSL_UART_UASR_PARITY_TYPE_NONE   (0x00000000u)
#define CSL_UART_UASR_PARITY_TYPE_SPACE  (0x00000001u)
#define CSL_UART_UASR_PARITY_TYPE_EVEN   (0x00000002u)
#define CSL_UART_UASR_PARITY_TYPE_ODD    (0x00000003u)

#define CSL_UART_UASR_BIT_BY_CHAR_MASK   (0x00000020u)
#define CSL_UART_UASR_BIT_BY_CHAR_SHIFT  (0x00000005u)
#define CSL_UART_UASR_BIT_BY_CHAR_RESETVAL (0x00000000u)
#define CSL_UART_UASR_BIT_BY_CHAR_7      (0x00000000u)
#define CSL_UART_UASR_BIT_BY_CHAR_8      (0x00000001u)

#define CSL_UART_UASR_SPEED_MASK         (0x0000001Fu)
#define CSL_UART_UASR_SPEED_SHIFT        (0x00000000u)
#define CSL_UART_UASR_SPEED_RESETVAL     (0x00000000u)
#define CSL_UART_UASR_SPEED_NONE         (0x00000000u)
#define CSL_UART_UASR_SPEED_1152         (0x00000001u)
#define CSL_UART_UASR_SPEED_576          (0x00000002u)
#define CSL_UART_UASR_SPEED_384          (0x00000003u)
#define CSL_UART_UASR_SPEED_288          (0x00000004u)
#define CSL_UART_UASR_SPEED_192          (0x00000005u)
#define CSL_UART_UASR_SPEED_144          (0x00000006u)
#define CSL_UART_UASR_SPEED_096          (0x00000007u)
#define CSL_UART_UASR_SPEED_048          (0x00000008u)
#define CSL_UART_UASR_SPEED_024          (0x00000009u)
#define CSL_UART_UASR_SPEED_012          (0x0000000Au)

#define CSL_UART_UASR_RESETVAL           (0x00000000u)

/* SCR */

#define CSL_UART_SCR_RX_TRIG_GRANU1_MASK (0x00000080u)
#define CSL_UART_SCR_RX_TRIG_GRANU1_SHIFT (0x00000007u)
#define CSL_UART_SCR_RX_TRIG_GRANU1_RESETVAL (0x00000000u)
#define CSL_UART_SCR_RX_TRIG_GRANU1_DISABLE (0x00000000u)
#define CSL_UART_SCR_RX_TRIG_GRANU1_ENABLE (0x00000001u)

#define CSL_UART_SCR_TX_TRIG_GRANU1_MASK (0x00000040u)
#define CSL_UART_SCR_TX_TRIG_GRANU1_SHIFT (0x00000006u)
#define CSL_UART_SCR_TX_TRIG_GRANU1_RESETVAL (0x00000000u)
#define CSL_UART_SCR_TX_TRIG_GRANU1_DISABLE (0x00000000u)
#define CSL_UART_SCR_TX_TRIG_GRANU1_ENABLE (0x00000001u)

#define CSL_UART_SCR_DSR_IT_MASK         (0x00000020u)
#define CSL_UART_SCR_DSR_IT_SHIFT        (0x00000005u)
#define CSL_UART_SCR_DSR_IT_RESETVAL     (0x00000000u)
#define CSL_UART_SCR_DSR_IT_DISABLE      (0x00000000u)
#define CSL_UART_SCR_DSR_IT_ENABLE       (0x00000001u)

#define CSL_UART_SCR_RX_CTS_DSR_WAKE_UP_ENABLE_MASK (0x00000010u)
#define CSL_UART_SCR_RX_CTS_DSR_WAKE_UP_ENABLE_SHIFT (0x00000004u)
#define CSL_UART_SCR_RX_CTS_DSR_WAKE_UP_ENABLE_RESETVAL (0x00000000u)
#define CSL_UART_SCR_RX_CTS_DSR_WAKE_UP_ENABLE_DISABLE (0x00000000u)
#define CSL_UART_SCR_RX_CTS_DSR_WAKE_UP_ENABLE_ENABLE (0x00000001u)

#define CSL_UART_SCR_TX_EMPTY_CTL_IT_MASK (0x00000008u)
#define CSL_UART_SCR_TX_EMPTY_CTL_IT_SHIFT (0x00000003u)
#define CSL_UART_SCR_TX_EMPTY_CTL_IT_RESETVAL (0x00000000u)
#define CSL_UART_SCR_TX_EMPTY_CTL_IT_DISABLE (0x00000000u)
#define CSL_UART_SCR_TX_EMPTY_CTL_IT_ENABLE (0x00000001u)

#define CSL_UART_SCR_DMA_MODE_2_MASK     (0x00000006u)
#define CSL_UART_SCR_DMA_MODE_2_SHIFT    (0x00000001u)
#define CSL_UART_SCR_DMA_MODE_2_RESETVAL (0x00000000u)
#define CSL_UART_SCR_DMA_MODE_2_00       (0x00000000u)
#define CSL_UART_SCR_DMA_MODE_2_01       (0x00000001u)
#define CSL_UART_SCR_DMA_MODE_2_02       (0x00000002u)
#define CSL_UART_SCR_DMA_MODE_2_03       (0x00000003u)

#define CSL_UART_SCR_DMA_MODE_CTL_MASK   (0x00000001u)
#define CSL_UART_SCR_DMA_MODE_CTL_SHIFT  (0x00000000u)
#define CSL_UART_SCR_DMA_MODE_CTL_RESETVAL (0x00000000u)
#define CSL_UART_SCR_DMA_MODE_CTL_DISABLE (0x00000000u)
#define CSL_UART_SCR_DMA_MODE_CTL_ENABLE (0x00000001u)

#define CSL_UART_SCR_RESETVAL            (0x00000000u)

/* SSR */

#define CSL_UART_SSR_RX_CTS_DSR_WAKE_UP_STS_MASK (0x00000002u)
#define CSL_UART_SSR_RX_CTS_DSR_WAKE_UP_STS_SHIFT (0x00000001u)
#define CSL_UART_SSR_RX_CTS_DSR_WAKE_UP_STS_RESETVAL (0x00000000u)

#define CSL_UART_SSR_TX_FIFO_FULL_MASK   (0x00000001u)
#define CSL_UART_SSR_TX_FIFO_FULL_SHIFT  (0x00000000u)
#define CSL_UART_SSR_TX_FIFO_FULL_RESETVAL (0x00000000u)

#define CSL_UART_SSR_RESETVAL            (0x00000000u)

/* MVR */

#define CSL_UART_MVR_MAJOR_REV_MASK      (0x000000F0u)
#define CSL_UART_MVR_MAJOR_REV_SHIFT     (0x00000004u)
#define CSL_UART_MVR_MAJOR_REV_RESETVAL  (0x00000001u)

#define CSL_UART_MVR_MINOR_REV_MASK      (0x0000000Fu)
#define CSL_UART_MVR_MINOR_REV_SHIFT     (0x00000000u)
#define CSL_UART_MVR_MINOR_REV_RESETVAL  (0x00000001u)

#define CSL_UART_MVR_RESETVAL            (0x00000011u)

/* SYSC */

#define CSL_UART_SYSC_IDLEMODE_MASK      (0x00000018u)
#define CSL_UART_SYSC_IDLEMODE_SHIFT     (0x00000003u)
#define CSL_UART_SYSC_IDLEMODE_RESETVAL  (0x00000000u)
#define CSL_UART_SYSC_IDLEMODE_FORCE     (0x00000000u)
#define CSL_UART_SYSC_IDLEMODE_NONE      (0x00000001u)
#define CSL_UART_SYSC_IDLEMODE_SMART     (0x00000002u)

#define CSL_UART_SYSC_ENAWAKEUP_MASK     (0x00000004u)
#define CSL_UART_SYSC_ENAWAKEUP_SHIFT    (0x00000002u)
#define CSL_UART_SYSC_ENAWAKEUP_RESETVAL (0x00000000u)
#define CSL_UART_SYSC_ENAWAKEUP_DISABLE  (0x00000000u)
#define CSL_UART_SYSC_ENAWAKEUP_ENABLE   (0x00000001u)

#define CSL_UART_SYSC_SOFTRESET_MASK     (0x00000002u)
#define CSL_UART_SYSC_SOFTRESET_SHIFT    (0x00000001u)
#define CSL_UART_SYSC_SOFTRESET_RESETVAL (0x00000000u)
#define CSL_UART_SYSC_SOFTRESET_DISABLE  (0x00000000u)
#define CSL_UART_SYSC_SOFTRESET_ENABLE   (0x00000001u)

#define CSL_UART_SYSC_AUTOIDLE_MASK      (0x00000001u)
#define CSL_UART_SYSC_AUTOIDLE_SHIFT     (0x00000000u)
#define CSL_UART_SYSC_AUTOIDLE_RESETVAL  (0x00000000u)
#define CSL_UART_SYSC_AUTOIDLE_CLKRUN    (0x00000000u)
#define CSL_UART_SYSC_AUTOIDLE_AUTO_OCP  (0x00000001u)

#define CSL_UART_SYSC_RESETVAL           (0x00000000u)

/* SYSS */

#define CSL_UART_SYSS_RESETDONE_MASK     (0x00000001u)
#define CSL_UART_SYSS_RESETDONE_SHIFT    (0x00000000u)
#define CSL_UART_SYSS_RESETDONE_RESETVAL (0x00000000u)

#define CSL_UART_SYSS_RESETVAL           (0x00000000u)

/* WER */

#define CSL_UART_WER_RLS_MASK            (0x00000040u)
#define CSL_UART_WER_RLS_SHIFT           (0x00000006u)
#define CSL_UART_WER_RLS_RESETVAL        (0x00000001u)

#define CSL_UART_WER_RHR_MASK            (0x00000020u)
#define CSL_UART_WER_RHR_SHIFT           (0x00000005u)
#define CSL_UART_WER_RHR_RESETVAL        (0x00000001u)

#define CSL_UART_WER_RXIR_MASK           (0x00000010u)
#define CSL_UART_WER_RXIR_SHIFT          (0x00000004u)
#define CSL_UART_WER_RXIR_RESETVAL       (0x00000001u)

#define CSL_UART_WER_DCD_MASK            (0x00000008u)
#define CSL_UART_WER_DCD_SHIFT           (0x00000003u)
#define CSL_UART_WER_DCD_RESETVAL        (0x00000001u)

#define CSL_UART_WER_RI_MASK             (0x00000004u)
#define CSL_UART_WER_RI_SHIFT            (0x00000002u)
#define CSL_UART_WER_RI_RESETVAL         (0x00000001u)

#define CSL_UART_WER_DSR_MASK            (0x00000002u)
#define CSL_UART_WER_DSR_SHIFT           (0x00000001u)
#define CSL_UART_WER_DSR_RESETVAL        (0x00000001u)

#define CSL_UART_WER_CTS_MASK            (0x00000001u)
#define CSL_UART_WER_CTS_SHIFT           (0x00000000u)
#define CSL_UART_WER_CTS_RESETVAL        (0x00000001u)

#define CSL_UART_WER_RESETVAL           (0x0000007Fu)

#endif