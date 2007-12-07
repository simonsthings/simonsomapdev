/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef __CSL_INTC0_H_
#define __CSL_INTC0_H_

#include <cslr.h>
#include <csl_types.h>
#include <csl_sysData.h>

typedef Uint16  CSL_Intc0EventId;

typedef enum {
        CSL_INTC0_STATE_ENABLE  = 1,
        CSL_INTC0_STATE_DISABLE = 0
} CSL_Intc0EventEnableState;

typedef enum {
        CSL_INTC0_GBLSTATE_ENABLE       = 0,
        CSL_INTC0_GBLSTATE_DISABLE = 1
} CSL_Intc0GlobalEnableState;

/**************************************************************************\
 * INTC0 function definitions
\**************************************************************************/

inline
CSL_Intc0GlobalEnableState
        _CSL_intc0GlobalDisable(
                void
)
{
        CSL_Intc0GlobalEnableState    prevState;

        prevState = ( CSL_Intc0GlobalEnableState) CSL_FEXT( CSL_sysDataHandle->chipRegs->ST1_55, 
        CHIP_ST1_55_INTM);
        asm(" BSET INTM");

        return prevState;
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
inline
CSL_Intc0GlobalEnableState
        _CSL_intc0GlobalEnable(
                void
)
{
        CSL_Intc0GlobalEnableState    prevState;

        prevState = ( CSL_Intc0GlobalEnableState) CSL_FEXT(CSL_sysDataHandle->chipRegs->ST1_55, 
        CHIP_ST1_55_INTM);
        asm(" BCLR INTM");

    return prevState;
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
inline
void
        _CSL_intc0GlobalRestore(
                CSL_Intc0GlobalEnableState      prevState
)
{
        if (prevState == CSL_INTC0_GBLSTATE_ENABLE) {
                asm(" BCLR INTM");
        } else {
                asm(" BSET INTM");
        }
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
inline
void
        _CSL_intc0Clear (
                CSL_Intc0EventId        evtId
)
{
        if (evtId <= CSL_INTC_IVPDINTEND)  {
                CSL_sysDataHandle->chipRegs->IFR0 = CSL_FMKR(evtId, evtId, 1 /* 1 to clear */);
        } else {
                CSL_sysDataHandle->chipRegs->IFR1 = CSL_FMKR(evtId - 16, evtId - 16, 1 /* 1 to clear */);
        }
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
inline
CSL_Intc0EventEnableState
        _CSL_intc0Disable (
                CSL_Intc0EventId        evtId
)
{
        CSL_Intc0EventEnableState       en;

        if ( evtId <= CSL_INTC_IVPDINTEND) {
                en = ( CSL_Intc0EventEnableState) CSL_FEXTR(CSL_sysDataHandle->chipRegs->IER0, evtId, evtId);
                CSL_FINSR(CSL_sysDataHandle->chipRegs->IER0, evtId, evtId, CSL_INTC0_STATE_DISABLE);
        } else {
                en = ( CSL_Intc0EventEnableState) CSL_FEXTR(CSL_sysDataHandle->chipRegs->IER0, evtId - 16, evtId - 16);
                CSL_FINSR(CSL_sysDataHandle->chipRegs->IER0, evtId - 16, evtId - 16, CSL_INTC0_STATE_DISABLE);
        }

        return en;
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
inline
CSL_Intc0EventEnableState
        _CSL_intc0Enable (
                CSL_Intc0EventId        evtId
)
{
        CSL_Intc0EventEnableState       en;

        if ( evtId <= CSL_INTC_IVPDINTEND) {
                en = ( CSL_Intc0EventEnableState) CSL_FEXTR(CSL_sysDataHandle->chipRegs->IER0, evtId, evtId);
                CSL_FINSR(CSL_sysDataHandle->chipRegs->IER0, evtId, evtId, CSL_INTC0_STATE_ENABLE);
        } else {
                en = ( CSL_Intc0EventEnableState) CSL_FEXTR( CSL_sysDataHandle->chipRegs->IER1, evtId - 16, evtId - 16);
                CSL_FINSR(CSL_sysDataHandle->chipRegs->IER1, evtId - 16, evtId - 16, CSL_INTC0_STATE_ENABLE);
        }

        return en;
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
inline
CSL_Status
        _CSL_intc0Restore (
                CSL_Intc0EventId                evtId,
                CSL_Intc0EventEnableState       prevState
)
{
        CSL_FINSR(CSL_sysDataHandle->chipRegs->IER0, evtId, evtId, prevState);
        return CSL_SOK;
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
inline
void
        _CSL_intc0Set (
                CSL_Intc0EventId        evtId
)
{
        switch (evtId) {
        case 0:
                asm (" intr #0");
                break;
        case 1:
                asm (" intr #1");
                break;
        case 2:
                asm (" intr #2");
                break;
        case 3:
                asm (" intr #3");
                break;
        case 4:
                asm (" intr #4");
                break;
        case 5:
                asm (" intr #5");
                break;
        case 6:
                asm (" intr #6");
                break;
        case 7:
                asm (" intr #7");
                break;
        case 8:
                asm (" intr #8");
                break;
        case 9:
                asm (" intr #9");
                break;
        case 10:
                asm (" intr #10");
                break;
        case 11:
                asm (" intr #11");
                break;
        case 12:
                asm (" intr #12");
                break;
        case 13:
                asm (" intr #13");
                break;
        case 14:
                asm (" intr #14");
                break;
        case 15:
                asm (" intr #15");
                break;
        case 16:
                asm (" intr #16");
                break;
        case 17:
                asm (" intr #17");
                break;
        case 18:
                asm (" intr #18");
                break;
        case 19:
                asm (" intr #19");
                break;
        case 20:
                asm (" intr #20");
                break;
        case 21:
                asm (" intr #21");
                break;
        case 22:
                asm (" intr #22");
                break;
        case 23:
                asm (" intr #23");
                break;
        case 24:
                asm (" intr #24");
                break;
        case 25:
                asm (" intr #25");
                break;
        case 26:
                asm (" intr #26");
                break;
        case 27:
                asm (" intr #27");
                break;
        case 28:
                asm (" intr #28");
                break;
        case 29:
                asm (" intr #29");
                break;
        case 30:
                asm (" intr #30");
                break;
        case 31:
                asm (" intr #31");
                break;
        }
}
/*-------------------------------------------------------------------------------*/

#endif
