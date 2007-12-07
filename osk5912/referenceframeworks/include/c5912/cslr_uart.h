/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#ifndef _CSLR_UART_H_
#define _CSLR_UART_H_

#include <cslr.h>
#include <cslr_uart_002.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_UartRegs  * CSL_UartRegsOvly;



static inline CSL_UartRegsOvly _CSL_uartGetBaseAddr(Uint16 uartNum)
{
    return (CSL_UartRegsOvly)_CSL_uartlookup[uartNum];
}

#endif  /* CSLR_UART_H_ */
