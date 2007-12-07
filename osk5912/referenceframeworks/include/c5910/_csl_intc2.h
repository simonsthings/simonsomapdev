/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef __CSL_INTC2_H_
#define __CSL_INTC2_H_

#include <csl_types.h>
#include <csl_error.h>

#include <cslr.h>
#include <_cslr_intc2.h>

/**************************************************************************\
* INTC2 global macro declarations
\**************************************************************************/

#define CSL_INTC2_EVENT_CNT        (16)
#define CSL_INTC2_EVT_INVALID      (-1)
#define CSL_INTC2_VECT_INVALID     (-1)
#define CSL_INTC2_BADHANDLE        {CSL_INTC2_VECT_INVALID, CSL_INTC2_EVT_INVALID}

/**************************************************************************\
* INTC2 global typedef declarations
\**************************************************************************/
/* Default priority level                        */
typedef enum {
    CSL_INTC2_PRIORITY_DEFAULT      =                  0
} CSL_Intc2Priority;

/* Interrupt routing                             */
typedef enum {
    CSL_INTC2_ROUTE_IRQ             =                  0,
    CSL_INTC2_ROUTE_FIQ             =                  1
} CSL_Intc2Route;

/* Interrupt trigger                             */
typedef enum {
    CSL_INTC2_SENSE_FALLINGEDGE     =                  0,
    CSL_INTC2_SENSE_LOWLEVEL        =                  1
} CSL_Intc2Sense;

typedef int CSL_Intc2EventId;
typedef int CSL_Intc2VectId;
typedef Uint32 CSL_Intc2EventState;
typedef void (* CSL_Intc2EventHandler)(void *);
typedef int CSL_intcGblEnableState;
typedef int CSL_Intc2EventEnableState;
typedef struct CSL_Intc2Obj *CSL_Intc2Handle;

typedef struct  {
    CSL_Intc2EventHandler    handler; // user C function to be called
    void                    *arg;       // parameter passed to handler
 } CSL_Intc2EventHandlerRecord;

typedef void (*CSL_Intc2CSL_Intc2IsrPtr) ( void );

/* INTC2 setup structure */
typedef struct CSL_Intc2Setup {
    CSL_Intc2Priority        priority;
    CSL_Intc2Route           route;
    CSL_Intc2Sense           sense;
} CSL_Intc2Setup;

/* CSL_IntcObj structure definition.             */
typedef struct CSL_Intc2Obj {
    CSL_Intc2EventId         eventId;
    CSL_Intc2VectId          vectId;
    void                    *reserved;
} CSL_Intc2Obj;

/**************************************************************************\
* INTC2 function defintions
\**************************************************************************/
inline CSL_Status _CSL_intc2Clear( Uint16  evt_id )
{
    CSL_FINSR(CSL_INTC2_0_REGS->ITR,evt_id,evt_id,0);
    return(CSL_SOK);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Bool _CSL_intc2Test( Uint16 evt_id )
{
    return (CSL_Bool)CSL_FEXTR(CSL_INTC2_0_REGS->ITR, evt_id, evt_id);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc2EventEnableState
        _CSL_intc2EventDisable (
        CSL_Intc2EventId        evtId
)
{
    CSL_Intc2EventEnableState en;

    en = CSL_FEXTR(CSL_INTC2_0_REGS->MIR, evtId, evtId);
    CSL_FINSR(CSL_INTC2_0_REGS->MIR, evtId, evtId, 1 /* 1 -> disable */);
    return en;
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc2EventEnableState
        _CSL_intc2EventEnable(
        CSL_Intc2EventId        evtId
)
{
    CSL_Intc2EventEnableState en;

    en = CSL_FEXTR(CSL_INTC2_0_REGS->MIR, evtId, evtId);
    CSL_FINSR(CSL_INTC2_0_REGS->MIR, evtId, evtId, 0 /* 0 -> enable */);
    return en;
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Status _CSL_intc2EventRestore(
                CSL_Intc2EventId                        evtId,
        CSL_Intc2EventEnableState       ieState
)
{
        CSL_FINSR(CSL_INTC2_0_REGS->MIR, evtId, evtId, ieState);
        return(CSL_SOK);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc2Priority  _CSL_intc2GetPriority(
    Uint16                evt_id
)
{
    return (CSL_Intc2Priority)CSL_FEXT(CSL_INTC2_0_REGS->ILR[2*evt_id], INTC2_ILR_PRIORITY);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc2Route  _CSL_intc2GetRoute(
    Uint16                evt_id
)
{
    return (CSL_Intc2Route)CSL_FEXT(CSL_INTC2_0_REGS->ILR[2*evt_id],INTC2_ILR_FIQ);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc2Sense  _CSL_intc2GetSense(
    Uint16                evt_id
)
{
    return (CSL_Intc2Sense)CSL_FEXT(CSL_INTC2_0_REGS->ILR[2*evt_id],INTC2_ILR_SENSE_EDGE);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void  _CSL_intc2Reset ( Uint16 evt_id )
{
    CSL_FINSR(CSL_INTC2_0_REGS->MIR,evt_id,evt_id,1);
    CSL_FINSR(CSL_INTC2_0_REGS->ITR,evt_id,evt_id,0);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Status _CSL_intc2Set(
    Uint16                evt_id
)
{
    /* set the bit in the ISR corresponding to the 'evt_id' */
    CSL_FINSR(CSL_INTC2_0_REGS->ISR,evt_id,evt_id,1);
    return(CSL_SOK);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void _CSL_intc2SetPriority(
    Uint16                evt_id,
    CSL_Intc2Priority          priority
)
{
    CSL_INTC2_0_REGS->ILR[2*evt_id] = CSL_FMK(INTC2_ILR_PRIORITY, priority);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void _CSL_intc2SetRoute(
    Uint16                evt_id,
    CSL_Intc2Route             route_IRQ_or_FIQ
)
{
    CSL_INTC2_0_REGS->ILR[2*evt_id] = CSL_FMK(INTC2_ILR_FIQ, route_IRQ_or_FIQ);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void _CSL_intc2SetSense(
    Uint16                evt_id,
    CSL_Intc2Sense             sense
)
{
    CSL_INTC2_0_REGS->ILR[2*evt_id] = CSL_FMK(INTC2_ILR_SENSE_EDGE,sense);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void _CSL_intc2Setup(
    Uint16                evt_id,
    CSL_Intc2Setup             *setup
)
{
    CSL_INTC2_0_REGS->ILR[2*evt_id] =
        CSL_FMK(INTC2_ILR_PRIORITY, setup->priority) |
        CSL_FMK(INTC2_ILR_SENSE_EDGE, setup->sense) |
        CSL_FMK(INTC2_ILR_FIQ, setup->route);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void  _CSL_intc2AckIRQ (void)
{
    CSL_FINS(CSL_INTC2_0_REGS->ICR, INTC2_ICR_NEW_IRQ_AGR, 1);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc2EventId _CSL_intc2GetActiveFIQ( void )
{
    return (CSL_Intc2EventId)CSL_FEXT(CSL_INTC2_0_REGS->SFR, INTC2_SFR_IRQ);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc2EventId  _CSL_intc2GetActiveIRQ(
    void
)
{
    return (CSL_Intc2EventId)CSL_FEXT(CSL_INTC2_0_REGS->SIR, INTC2_SIR_IRQ);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void _CSL_intc2AckFIQ( void )
{
    CSL_FINS(CSL_INTC2_0_REGS->ICR, INTC2_ICR_NEW_FIQ_AGR, 1);
}
/*------------------------------------------------------------------------*/

#endif
