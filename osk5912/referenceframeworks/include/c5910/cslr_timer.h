/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSLR_TIMER_H_
#define _CSLR_TIMER_H_

/***************5910PG1_0***************/
#ifdef  CSL_CHIP_5910PG1_0

#include <cslr.h>
#include <cslr_timer_004.h>

/**************************************************************************\
* Peripheral Instance count
\**************************************************************************/
#define CSL_TIMER_CNT                 3
/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_TimerRegs  * CSL_TimerRegsOvly;
/**************************************************************************\
* Peripheral Base Address
\**************************************************************************/
#define CSL_TIMER_1_REGS              ((CSL_TimerRegsOvly) 0x00002800u)
#define CSL_TIMER_2_REGS              ((CSL_TimerRegsOvly) 0x00002C00u)
#define CSL_TIMER_3_REGS              ((CSL_TimerRegsOvly) 0x00003000u)

inline CSL_TimerRegsOvly _CSL_timerGetBaseAddr(Uint16 timerNum)
{
    return((CSL_TimerRegsOvly)( 0x00002800u + timerNum * 0x00000400u ));
}

#else
#error "CHIP symbol not defined"

#endif /* CSL_CHIP_5910PG1_0 */

#endif  /* CSLR_TIMER_H_ */
