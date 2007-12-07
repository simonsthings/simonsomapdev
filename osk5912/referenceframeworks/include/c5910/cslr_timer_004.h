/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSLR_TIMER_004_H_
#define _CSLR_TIMER_004_H_

#include <cslr.h>
#include <csl_types.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/


typedef struct  {
    CSL_Reg16 CNTL;
    const CSL_Reg16 PAD0[1];
    CSL_Reg16 LOADHI;
    CSL_Reg16 LOADLO;
    CSL_Reg16 READHI;
    CSL_Reg16 READLO;
} CSL_TimerRegs;
/**************************************************************************\
* Register Id's
\**************************************************************************/
typedef enum  {
   CSL_TIMER_CNTL = 0x0000u,
   CSL_TIMER_LOADHI = 0x0002u,
   CSL_TIMER_LOADLO = 0x0003u,
   CSL_TIMER_READHI = 0x0004u,
   CSL_TIMER_READLO = 0x0005u
} CSL_TimerRegIds;
/**************************************************************************\
* Field Definition Macros
\**************************************************************************/
  /* CSL_TIMER_CNTL */
#define CSL_TIMER_CNTL_FREE_MASK      (0x0040u)
#define CSL_TIMER_CNTL_FREE_SHIFT     (0x0006u)
#define CSL_TIMER_CNTL_FREE_RESETVAL  (0x0000u)
#define CSL_TIMER_CNTL_FREE_SUSPEND   (0x0000u)
#define CSL_TIMER_CNTL_FREE_FREEMODE  (0x0001u)

#define CSL_TIMER_CNTL_CLKEN_MASK     (0x0020u)
#define CSL_TIMER_CNTL_CLKEN_SHIFT    (0x0005u)
#define CSL_TIMER_CNTL_CLKEN_RESETVAL (0x0000u)
#define CSL_TIMER_CNTL_CLKEN_DISABLE  (0x0000u)
#define CSL_TIMER_CNTL_CLKEN_ENABLE   (0x0001u)

#define CSL_TIMER_CNTL_PTV_MASK       (0x001Cu)
#define CSL_TIMER_CNTL_PTV_SHIFT      (0x0002u)
#define CSL_TIMER_CNTL_PTV_RESETVAL   (0x0000u)
#define CSL_TIMER_CNTL_PTV_CLKBY2     (0x0000u)
#define CSL_TIMER_CNTL_PTV_CLKBY4     (0x0001u)
#define CSL_TIMER_CNTL_PTV_CLKBY8     (0x0002u)
#define CSL_TIMER_CNTL_PTV_CLKBY16    (0x0003u)
#define CSL_TIMER_CNTL_PTV_CLKBY32    (0x0004u)
#define CSL_TIMER_CNTL_PTV_CLKBY64    (0x0005u)
#define CSL_TIMER_CNTL_PTV_CLKBY128   (0x0006u)
#define CSL_TIMER_CNTL_PTV_CLKBY256   (0x0007u)

#define CSL_TIMER_CNTL_AR_MASK        (0x0002u)
#define CSL_TIMER_CNTL_AR_SHIFT       (0x0001u)
#define CSL_TIMER_CNTL_AR_RESETVAL    (0x0000u)
#define CSL_TIMER_CNTL_AR_ONESHOT     (0x0000u)
#define CSL_TIMER_CNTL_AR_RELOAD      (0x0001u)

#define CSL_TIMER_CNTL_ST_MASK        (0x0001u)
#define CSL_TIMER_CNTL_ST_SHIFT       (0x0000u)
#define CSL_TIMER_CNTL_ST_RESETVAL    (0x0000u)
#define CSL_TIMER_CNTL_ST_START       (0x0001u)
#define CSL_TIMER_CNTL_ST_STOP        (0x0000u)

#define CSL_TIMER_CNTL_RESETVAL       (0x0000u)


  /* CSL_TIMER_LOADHI */
#define CSL_TIMER_LOADHI_PRD_MASK     (0xFFFFu)
#define CSL_TIMER_LOADHI_PRD_SHIFT    (0x0000u)
#define CSL_TIMER_LOADHI_PRD_RESETVAL (0x0000u)

#define CSL_TIMER_LOADHI_RESETVAL     (0x0000u)


  /* CSL_TIMER_LOADLO */
#define CSL_TIMER_LOADLO_PRD_MASK     (0xFFFFu)
#define CSL_TIMER_LOADLO_PRD_SHIFT    (0x0000u)
#define CSL_TIMER_LOADLO_PRD_RESETVAL (0x0000u)

#define CSL_TIMER_LOADLO_RESETVAL     (0x0000u)


  /* CSL_TIMER_READHI */
#define CSL_TIMER_READHI_CNT_MASK     (0xFFFFu)
#define CSL_TIMER_READHI_CNT_SHIFT    (0x0000u)
#define CSL_TIMER_READHI_CNT_RESETVAL (0x0000u)

#define CSL_TIMER_READHI_RESETVAL     (0x0000u)


  /* CSL_TIMER_READLO */
#define CSL_TIMER_READLO_CNT_MASK     (0xFFFFu)
#define CSL_TIMER_READLO_CNT_SHIFT    (0x0000u)
#define CSL_TIMER_READLO_CNT_RESETVAL (0x0000u)

#define CSL_TIMER_READLO_RESETVAL     (0x0000u)




#endif
