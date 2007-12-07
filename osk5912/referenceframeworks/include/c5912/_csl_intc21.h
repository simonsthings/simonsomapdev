/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef __CSL_INTC21_H_
#define __CSL_INTC21_H_

#include <cslr.h>
#include <csl_error.h>
#include <_csl_resource.h>
#include <csl_types.h>
#include <cslr_intc21.h>

/**************************************************************************\
 * INTC21 global macro declarations
 \**************************************************************************/

/* Count of the number of input interrupt lines to the level 2.1 interrupt handler. */
#define CSL_INTC21_EVENT_CNT                          (64)

/* Mask of the bits in the input event id used to identify the  */
/* section to which the input interrupt belongs                 */
#define CSL_INTC21_SECTION_MASK                       (48)

/* Position of the start of SECTION MASK bit for identifying    */
/* bits indicating section                                      */
#define CSL_INTC21_SECTION_SHIFT                      (4)

#define CSL_INTC21_SECTION_EVT_ID(x)   CSL_FMKR(3, 0, (x))

/**************************************************************************\
 * INTC21 global typedef declarations
 \**************************************************************************/
/* Default priority level                        */
typedef enum {
    CSL_INTC21_PRIORITY_DEFAULT    =                  0
} CSL_Intc21Priority;

/* Interrupt routing                             */
typedef enum {
    CSL_INTC21_IRQ                 =                  0,
    CSL_INTC21_FIQ                 =                  1
} CSL_Intc21Type;

/* Interrupt trigger                             */
typedef enum {
    CSL_INTC21_FALL_EDGE           =                  0,
    CSL_INTC21_ACTIVE_LOW          =                  1,
    CSL_INTC21_RAISE_EDGE,
    CSL_INTC21_ACTIVE_HIGH
} CSL_Intc21Sense;

typedef    int CSL_Intc21EventId;
typedef    int CSL_Intc21VectId;
typedef Uint32 CSL_Intc21EventState;
typedef void (* CSL_Intc21EventHandler)(void *);

typedef struct  {
    CSL_Intc21EventHandler    handler; // user C function to be called
    void                    *arg;       // parameter passed to handler
} CSL_Intc21EventHandlerRecord;

typedef Uint32    CSL_intc21GblEnableState;
typedef Uint32    CSL_Intc21EventEnableStatus;

// ISR address type
typedef void (*CSL_Intc21IsrPtr)(void);

/* INTC Level 2.1 setup structure                       */
typedef struct CSL_Intc21Setup {
    CSL_Intc21Priority      priority;
    CSL_Intc21Type          type;
    CSL_Intc21Sense         sense;
} CSL_Intc21Setup;

/**************************************************************************\
 * INTC21 function declarations
 \**************************************************************************/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21EventClear(
        Uint16                evt_id
)
{
    Uint16 sect_evt_id;
    Uint16 section;

    /* get the section event id i.e evt id offset within the section*/
    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);

    /* get the section number */
    section=CSL_FEXT(evt_id,INTC21_SECTION);

    /* clear the bit in the IT_R corresponding to the 'eventId' */
    CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ITR,sect_evt_id,sect_evt_id,0);

    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21EventDisable(
        Uint16                evt_id,
        CSL_Intc21EventState      *oldState
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    *oldState = CSL_FEXTR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).MIR,sect_evt_id,sect_evt_id);
    CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).MIR,sect_evt_id,sect_evt_id,1);

    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21EventEnable(
        Uint16                evt_id,
        CSL_Intc21EventState      *oldState
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    *oldState = CSL_FEXTR(((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section].MIR,evt_id,evt_id);
    CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).MIR,sect_evt_id,sect_evt_id,0);

    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21EventRestore(
        Uint16                evt_id,
        Uint16                ieState
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    if (ieState)
        CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).MIR,sect_evt_id,sect_evt_id,1);
    else
        CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).MIR,sect_evt_id,sect_evt_id,0);


    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21EventSet(
        Uint16                evt_id
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ISR,sect_evt_id,sect_evt_id,1);

    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline Uint16  _CSL_intc21GetAutoIdle(
        void
        )
{
    return(CSL_FEXT((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).OCP_CFG,INTC21_OCP_CFG_AUTOIDLE));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline Uint16  _CSL_intc21GetMode(
        void
        )
{
    return(CSL_FEXT((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).OCP_CFG,INTC21_OCP_CFG_IDLE_MODE));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Intc21Priority  _CSL_intc21GetPriority(
        Uint16                evt_id
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    sect_evt_id = 2 * sect_evt_id;
    return(CSL_Intc21Priority)(CSL_FEXT((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ILR[sect_evt_id],INTC21_ILR_PRIORITY));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline Uint16  _CSL_intc21GetRstStat(
        void
        )
{
    return(CSL_FEXT((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).STATUS,INTC21_STATUS_RST));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Intc21Sense  _CSL_intc21GetSense(
        Uint16                evt_id
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    sect_evt_id = 2 * sect_evt_id;

    return(CSL_Intc21Sense)CSL_FEXT((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ILR[sect_evt_id],INTC21_ILR_SENSE_EDGE);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Intc21Type  _CSL_intc21GetType(
        Uint16                evt_id
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    sect_evt_id = 2 * sect_evt_id;

    return(CSL_Intc21Type)(CSL_FEXT((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ILR[sect_evt_id],INTC21_ILR_FIQ));
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline Uint16  _CSL_intc21GlobalDisable(
        void
        )
{
    Uint16 ieState;
    ieState = CSL_FEXT((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).ICR,INTC21_ICR_GLOBAL_MASK);
    CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).ICR,INTC21_ICR_GLOBAL_MASK,1);
    return ieState;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void _CSL_intc21GlobalEnable(
        void
        )
{
    CSL_FINS(((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0].ICR,INTC21_ICR_GLOBAL_MASK,0);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void  _CSL_intc21GlobalRestore(
        Uint16                gie
        )
{
    /* the corresponding bit in "Control Register" (MASK_IT_REG -> MI_R)
       is restored to the 'gie' value */
    if (gie)
        CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).ICR,INTC21_ICR_GLOBAL_MASK,1);
    else
        CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).ICR,INTC21_ICR_GLOBAL_MASK,0);

}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void  _CSL_intc21Reset(
        Uint16                evt_id
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).MIR,sect_evt_id,sect_evt_id,1);
    CSL_FINSR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ITR,sect_evt_id,sect_evt_id,0);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void  _CSL_intc21SetAutoIdle(
        Uint16                autoidle
        )
{
    CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).OCP_CFG,INTC21_OCP_CFG_AUTOIDLE,autoidle);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void  _CSL_intc21SetIdle(
        Uint16                mode
        )
{
    CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).OCP_CFG,INTC21_OCP_CFG_IDLE_MODE,mode);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21SetPriority(
        Uint16                evt_id,
        CSL_Intc21Priority                priority
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    sect_evt_id = 2 * sect_evt_id;
    (((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ILR[sect_evt_id] = CSL_FMK(INTC21_ILR_PRIORITY,priority) ;
    return (CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline void  _CSL_intc21SetReset(
        Uint16                reset
        )
{
    CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).OCP_CFG,INTC21_OCP_CFG_SOFTRST,reset);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21SetSense(
        Uint16                evt_id,
        CSL_Intc21Sense               sense
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    sect_evt_id = 2 * sect_evt_id;
    (((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ILR[sect_evt_id] = CSL_FMK(INTC21_ILR_SENSE_EDGE,sense) ;
    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21SetType(
        Uint16                evt_id,
        CSL_Intc21Type                route_IRQ_or_FIQ
        )
{
    Uint16 sect_evt_id;
    Uint16 section;

    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    sect_evt_id = 2 * sect_evt_id;
    (((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ILR[sect_evt_id] = CSL_FMK(INTC21_ILR_FIQ,route_IRQ_or_FIQ) ;
    return (CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline CSL_Status  _CSL_intc21Setup(
        Uint16                evt_id,
        CSL_Intc21Setup           *params
        )
{
    Uint16 sect_evt_id;
    Uint16 section;
    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    sect_evt_id = 2 * sect_evt_id;
    ((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section].ILR[sect_evt_id] =(
            CSL_FMK(INTC21_ILR_PRIORITY,params->priority) | \
            CSL_FMK(INTC21_ILR_FIQ,params->type) |\
            CSL_FMK(INTC21_ILR_SENSE_EDGE,params->sense)
            );
    return(CSL_SOK);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
inline Bool  _CSL_intc21Test(
        Uint16                evt_id
        )
{
    Uint16 sect_evt_id;
    Uint16 section;
    sect_evt_id=CSL_INTC21_SECTION_EVT_ID(evt_id);
    section=CSL_FEXT(evt_id,INTC21_SECTION);
    return ((Bool) CSL_FEXTR((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[section]).ITR,sect_evt_id,sect_evt_id));
}
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void  _CSL_intc21AckFIQ (void)
{
    CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).ICR, INTC21_ICR_NEW_FIQ_AGR, 1);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline void  _CSL_intc21AckIRQ (void)
{
    CSL_FINS((((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).ICR, INTC21_ICR_NEW_IRQ_AGR, 1);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc21EventId _CSL_intc21GetActiveFIQ(
        void
        )
{
    return (CSL_Intc21EventId)CSL_FEXT( (((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).SFR, INTC21_SFR_IRQ);
}
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
inline CSL_Intc21EventId _CSL_intc21GetActiveIRQ(
        void
        )
{
    return (CSL_Intc21EventId) CSL_FEXT( (((CSL_Intc21RegsOvly)CSL_INTC21_0_REGS)->SECT21[0]).SIR, INTC21_SIR_IRQ);
}
/*------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
interrupt void _CSL_intc21DispatchFIQ(void);
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
interrupt void _CSL_intc21DispatchIRQ(void);
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#endif
