/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef __CSL_INTC_H_
#define __CSL_INTC_H_

#include <csl_intc.h>

#define _CSL_INTC_EVTHANDLER_UNPLUGGED  ((CSL_IntcEventHandler) 0)

void _CSL_intcIsr0(void);
void _CSL_intcIsr1(void);

interrupt void _CSL_intcDispatcher (void);

extern CSL_IntcEventHandlerRecord   _CSL_intcEventHandlerRecord[ ];
extern CSL_BitMask32                _CSL_intcAllocMask[ ];

#endif /* __CSL_INTC_H_ */

