/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef _CSLR_TIMER_H_
#define _CSLR_TIMER_H_

#include <cslr.h>
#include <cslr_timer_004.h>
#include <5912.h>

/**************************************************************************\
* Overlay structure typedef definition
\**************************************************************************/
typedef volatile ioport CSL_TimerRegs  * CSL_TimerRegsOvly;


inline CSL_TimerRegsOvly _CSL_timerGetBaseAddr(Uint16 timerNum)
{
    return((CSL_TimerRegsOvly)( 0x00002800u + timerNum * 0x00000400u ));
}


#endif  /* CSLR_TIMER_H_ */
