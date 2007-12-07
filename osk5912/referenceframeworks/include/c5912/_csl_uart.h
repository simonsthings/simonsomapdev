/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#define CSL_UART_LCR_STORE(val)                         val = hUart->regs->LCR
#define CSL_UART_LCR_RESTORE(val)                       hUart->regs->LCR = val
#define CSL_UART_SETLCR_BF                      hUart->regs->LCR = 0xBF
#define CSL_UART_SETLCRBIT7_0                   CSL_FINSR(hUart->regs->LCR,7,7,0)
#define CSL_UART_SETLCRBIT7_1                   CSL_FINSR(hUart->regs->LCR,7,7,1)
