/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef __CSL_INTC_H_
#define __CSL_INTC_H_

#include <csl_intc.h>
#include <csl_types.h>

#define _CSL_INTC_EVTHANDLER_UNPLUGGED  ((CSL_IntcEventHandler) 0)

extern CSL_IntcEventHandlerRecord    _CSL_intcEventHandlerRecord[ ];
extern CSL_BitMask32    _CSL_intcAllocMask[ ];

/* These declarations are meant for computing the ISR jump location. */
void _CSL_intcIsr0(void);
void _CSL_intcIsr1(void);

interrupt
        void
                _CSL_intcDispatcher (
                        void
);

#endif /* __CSL_INTC_H_ */

