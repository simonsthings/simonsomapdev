/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef __CSL_INTC20_H_
#define __CSL_INTC20_H_

#include <cslr.h>
#include <csl_error.h>
#include <csl_types.h>
#include <_csl_resource.h>
#include <cslr_intc20.h>

/**************************************************************************\
 * INTC20 global macro declarations
 \**************************************************************************/

/* Count of the number of input interrupt lines to the level 2.0 interrupt handler. */
#define CSL_INTC20_EVENT_CNT                          (16)

/* Mask of the bits in the input event id used to identify the  */
/* section to which the input interrupt belongs.                */
#define CSL_INTC20_SECTION_MASK                       (96)

/* Position of the start of SECTION MASK bit for identifying    */
/* bits indicating section.                                     */
#define CSL_INTC20_SECTION_SHIFT                      (5)

/**************************************************************************\
 * INTC20 typedef declarations
 \**************************************************************************/
/* Default priority level.                       */
typedef enum {
    CSL_INTC20_PRIORITY_DEFAULT    =                  0
} CSL_Intc20Priority;

/* Interrupt routing.                            */
typedef enum {
    CSL_INTC20_IRQ                 =                  0,
    CSL_INTC20_FIQ                 =                  1
} CSL_Intc20Type;

/* Interrupt trigger.                            */
typedef enum {
    CSL_INTC20_FALL_EDGE           =                  0,
    CSL_INTC20_ACTIVE_LOW          =                  1,
    CSL_INTC20_RAISE_EDGE,
    CSL_INTC20_ACTIVE_HIGH
} CSL_Intc20Sense;

typedef int CSL_Intc20EventId;
typedef int CSL_Intc20VectId;
typedef Uint32 CSL_Intc20EventState;
typedef void (* CSL_Intc20EventHandler)(void *);
typedef void (*CSL_Intc20IsrPtr)(void);

typedef struct  {
    CSL_Intc20EventHandler    handler; // user C function to be called
    void                    *arg;       // parameter passed to handler
} CSL_Intc20EventHandlerRecord;

typedef Uint32    CSL_intc20GblEnableState;
typedef Uint32    CSL_Intc20EventEnableStatus;

/* INTC2.0 setup structure                       */
typedef struct CSL_Intc20Setup {
    CSL_Intc20Priority      priority;
    CSL_Intc20Type          type;
    CSL_Intc20Sense         sense;
} CSL_Intc20Setup;

/**************************************************************************\
 * INTC20 function declarations
 \**************************************************************************/
/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20EventClear(
        Uint16                evt_id
        )
{
    CSL_FINSR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ITR,evt_id,evt_id,0);
    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20EventDisable(
        Uint16                evt_id,
        CSL_Intc20EventState      *oldState
        )
{
    *oldState = CSL_FEXTR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->MIR,evt_id,evt_id);
    CSL_FINSR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->MIR,evt_id,evt_id,1);
    return (CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20EventEnable(
        Uint16                evt_id,
        CSL_Intc20EventState      *oldState
        )
{
    *oldState = CSL_FEXTR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->MIR,evt_id,evt_id);
    CSL_FINSR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->MIR,evt_id,evt_id,0);
    return (CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20EventRestore(
        Uint16                evt_id,
        Uint16                ieState
        )
{
    if (ieState)
        CSL_FINSR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->MIR,evt_id,evt_id,1);
    else
        CSL_FINSR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->MIR,evt_id,evt_id,0);
    return (CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20EventSet (
Uint16 evt_id
)
{
    CSL_FINSR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ISR,evt_id,evt_id,1);
    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline Bool  _CSL_intc20EventStatus(
        Uint16                evt_id
        )
{
    return (Bool)(CSL_FEXTR(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ITR,evt_id,evt_id));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Intc20Priority  _CSL_intc20GetPriority(
        Uint16                evt_id
        )
{
    return(CSL_Intc20Priority) (CSL_FEXT(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ILR[2*evt_id],INTC20_ILR_PRIORITY));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Intc20Sense  _CSL_intc20GetSense(
        Uint16                evt_id
        )
{
    return(CSL_Intc20Sense)(CSL_FEXT(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ILR[2*evt_id],INTC20_ILR_SENSE_EDGE));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Intc20Type _CSL_intc20GetType(
        Uint16                evt_id
        )
{
    return(CSL_Intc20Type)(CSL_FEXT(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ILR[2*evt_id],INTC20_ILR_FIQ));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

inline Uint16  _CSL_intc20GlobalDisable(
        void
        )
{
    Uint16 ieState;
    ieState = CSL_FEXT(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ICR,INTC20_ICR_GLOBAL_MASK);
    CSL_FINS(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ICR,INTC20_ICR_GLOBAL_MASK,1);
    return ieState;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void  _CSL_intc20GlobalEnable(
        void
        )
{
    CSL_FINS(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ICR,INTC20_ICR_GLOBAL_MASK,0);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void  _CSL_intc20GlobalRestore(
        Uint16                gie
        )
{
    /* the corresponding bit in "Control Register" is restored to the 'gie' value */
    if (gie)
        CSL_FINS(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ICR,INTC20_ICR_GLOBAL_MASK,1);
    else
        CSL_FINS(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ICR,INTC20_ICR_GLOBAL_MASK,0);

}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20SetPriority(
        Uint16                evt_id,
        CSL_Intc20Priority                priority
        )
{
    ((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ILR[2*evt_id] = CSL_FMK(INTC20_ILR_PRIORITY,priority) ;
    return (CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20SetSense(
        Uint16                evt_id,
        CSL_Intc20Sense                sense
        )
{
    ((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ILR[2*evt_id] = CSL_FMK(INTC20_ILR_SENSE_EDGE,sense) ;
    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20SetType(
        Uint16                evt_id,
        CSL_Intc20Type                route_IRQ_or_FIQ
        )
{
    ((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ILR[2*evt_id] = CSL_FMK(INTC20_ILR_FIQ,route_IRQ_or_FIQ) ;
    return (CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc20Setup(
        Uint16                evt_id,
        CSL_Intc20Setup           *params
        )
{
    ((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ILR[2*evt_id] =(
            CSL_FMK(INTC20_ILR_PRIORITY,params->priority) | \
            CSL_FMK(INTC20_ILR_FIQ,CSL_INTC20_FIQ)   | \
            CSL_FMK(INTC20_ILR_SENSE_EDGE,params->sense)
            );
    return ( CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void  _CSL_intc20AckFIQ (void)
{
    CSL_FINS(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->ICR, INTC20_ICR_NEW_FIQ_AGR, 1);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc20EventId  _CSL_intc20GetActiveFIQ(
        void
        )
{
    return (CSL_Intc20EventId)CSL_FEXT(((CSL_Intc20RegsOvly)CSL_INTC20_0_REGS)->SFR, INTC20_SFR_IRQ);
}
/*------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
interrupt void _CSL_intc20DispatchFIQ(void);
/*----------------------------------------------------------------------------*/

#endif
