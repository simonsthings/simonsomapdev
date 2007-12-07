/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef __CSLR_INTC20_005_H_
#define __CSLR_INTC20_005_H_

#include <cslr.h>
#include <csl_types.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    CSL_Reg16 ITR;
    const CSL_Reg16 RSVD0[1];
    CSL_Reg16 MIR;
    const CSL_Reg16 RSVD1[1];
    CSL_Reg16 SIR;
    const CSL_Reg16 RSVD2[1];
    CSL_Reg16 SFR;
    const CSL_Reg16 RSVD3[1];
    CSL_Reg16 ICR;
    const CSL_Reg16 RSVD4[1];
    CSL_Reg16 ISR;
    const CSL_Reg16 RSVD5[1];
    CSL_Reg16 ILR[16];
} CSL_Intc20Regs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/
  /* CSL_INTC20_ITR */
#define CSL_INTC20_ITR_IRQ0_MASK      (0x8000u)
#define CSL_INTC20_ITR_IRQ0_SHIFT     (0x000Fu)
#define CSL_INTC20_ITR_IRQ0_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ1_MASK      (0x4000u)
#define CSL_INTC20_ITR_IRQ1_SHIFT     (0x000Eu)
#define CSL_INTC20_ITR_IRQ1_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ2_MASK      (0x2000u)
#define CSL_INTC20_ITR_IRQ2_SHIFT     (0x000Du)
#define CSL_INTC20_ITR_IRQ2_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ3_MASK      (0x1000u)
#define CSL_INTC20_ITR_IRQ3_SHIFT     (0x000Cu)
#define CSL_INTC20_ITR_IRQ3_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ4_MASK      (0x0800u)
#define CSL_INTC20_ITR_IRQ4_SHIFT     (0x000Bu)
#define CSL_INTC20_ITR_IRQ4_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ5_MASK      (0x0400u)
#define CSL_INTC20_ITR_IRQ5_SHIFT     (0x000Au)
#define CSL_INTC20_ITR_IRQ5_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ6_MASK      (0x0200u)
#define CSL_INTC20_ITR_IRQ6_SHIFT     (0x0009u)
#define CSL_INTC20_ITR_IRQ6_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ7_MASK      (0x0100u)
#define CSL_INTC20_ITR_IRQ7_SHIFT     (0x0008u)
#define CSL_INTC20_ITR_IRQ7_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ8_MASK      (0x0080u)
#define CSL_INTC20_ITR_IRQ8_SHIFT     (0x0007u)
#define CSL_INTC20_ITR_IRQ8_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ9_MASK      (0x0040u)
#define CSL_INTC20_ITR_IRQ9_SHIFT     (0x0006u)
#define CSL_INTC20_ITR_IRQ9_RESETVAL  (0x0000u)

#define CSL_INTC20_ITR_IRQ10_MASK     (0x0020u)
#define CSL_INTC20_ITR_IRQ10_SHIFT    (0x0005u)
#define CSL_INTC20_ITR_IRQ10_RESETVAL (0x0000u)

#define CSL_INTC20_ITR_IRQ11_MASK     (0x0010u)
#define CSL_INTC20_ITR_IRQ11_SHIFT    (0x0004u)
#define CSL_INTC20_ITR_IRQ11_RESETVAL (0x0000u)

#define CSL_INTC20_ITR_IRQ12_MASK     (0x0008u)
#define CSL_INTC20_ITR_IRQ12_SHIFT    (0x0003u)
#define CSL_INTC20_ITR_IRQ12_RESETVAL (0x0000u)

#define CSL_INTC20_ITR_IRQ13_MASK     (0x0004u)
#define CSL_INTC20_ITR_IRQ13_SHIFT    (0x0002u)
#define CSL_INTC20_ITR_IRQ13_RESETVAL (0x0000u)

#define CSL_INTC20_ITR_IRQ14_MASK     (0x0002u)
#define CSL_INTC20_ITR_IRQ14_SHIFT    (0x0001u)
#define CSL_INTC20_ITR_IRQ14_RESETVAL (0x0000u)

#define CSL_INTC20_ITR_IRQ15_MASK     (0x0001u)
#define CSL_INTC20_ITR_IRQ15_SHIFT    (0x0000u)
#define CSL_INTC20_ITR_IRQ15_RESETVAL (0x0000u)

#define CSL_INTC20_ITR_RESETVAL       (0x0000u)


  /* CSL_INTC20_MIR */
#define CSL_INTC20_MIR_IRQ0_MASK      (0x8000u)
#define CSL_INTC20_MIR_IRQ0_SHIFT     (0x000Fu)
#define CSL_INTC20_MIR_IRQ0_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ1_MASK      (0x4000u)
#define CSL_INTC20_MIR_IRQ1_SHIFT     (0x000Eu)
#define CSL_INTC20_MIR_IRQ1_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ2_MASK      (0x2000u)
#define CSL_INTC20_MIR_IRQ2_SHIFT     (0x000Du)
#define CSL_INTC20_MIR_IRQ2_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ3_MASK      (0x1000u)
#define CSL_INTC20_MIR_IRQ3_SHIFT     (0x000Cu)
#define CSL_INTC20_MIR_IRQ3_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ4_MASK      (0x0800u)
#define CSL_INTC20_MIR_IRQ4_SHIFT     (0x000Bu)
#define CSL_INTC20_MIR_IRQ4_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ5_MASK      (0x0400u)
#define CSL_INTC20_MIR_IRQ5_SHIFT     (0x000Au)
#define CSL_INTC20_MIR_IRQ5_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ6_MASK      (0x0200u)
#define CSL_INTC20_MIR_IRQ6_SHIFT     (0x0009u)
#define CSL_INTC20_MIR_IRQ6_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ7_MASK      (0x0100u)
#define CSL_INTC20_MIR_IRQ7_SHIFT     (0x0008u)
#define CSL_INTC20_MIR_IRQ7_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ8_MASK      (0x0080u)
#define CSL_INTC20_MIR_IRQ8_SHIFT     (0x0007u)
#define CSL_INTC20_MIR_IRQ8_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ9_MASK      (0x0040u)
#define CSL_INTC20_MIR_IRQ9_SHIFT     (0x0006u)
#define CSL_INTC20_MIR_IRQ9_RESETVAL  (0x0001u)

#define CSL_INTC20_MIR_IRQ10_MASK     (0x0020u)
#define CSL_INTC20_MIR_IRQ10_SHIFT    (0x0005u)
#define CSL_INTC20_MIR_IRQ10_RESETVAL (0x0001u)

#define CSL_INTC20_MIR_IRQ11_MASK     (0x0010u)
#define CSL_INTC20_MIR_IRQ11_SHIFT    (0x0004u)
#define CSL_INTC20_MIR_IRQ11_RESETVAL (0x0001u)

#define CSL_INTC20_MIR_IRQ12_MASK     (0x0008u)
#define CSL_INTC20_MIR_IRQ12_SHIFT    (0x0003u)
#define CSL_INTC20_MIR_IRQ12_RESETVAL (0x0001u)

#define CSL_INTC20_MIR_IRQ13_MASK     (0x0004u)
#define CSL_INTC20_MIR_IRQ13_SHIFT    (0x0002u)
#define CSL_INTC20_MIR_IRQ13_RESETVAL (0x0001u)

#define CSL_INTC20_MIR_IRQ14_MASK     (0x0002u)
#define CSL_INTC20_MIR_IRQ14_SHIFT    (0x0001u)
#define CSL_INTC20_MIR_IRQ14_RESETVAL (0x0001u)

#define CSL_INTC20_MIR_IRQ15_MASK     (0x0001u)
#define CSL_INTC20_MIR_IRQ15_SHIFT    (0x0000u)
#define CSL_INTC20_MIR_IRQ15_RESETVAL (0x0001u)

#define CSL_INTC20_MIR_RESETVAL       (0x8000u)


  /* CSL_INTC20_SIR */
#define CSL_INTC20_SIR_IRQ_MASK       (0x000Fu)
#define CSL_INTC20_SIR_IRQ_SHIFT      (0x0000u)
#define CSL_INTC20_SIR_IRQ_RESETVAL   (0x0000u)

#define CSL_INTC20_SIR_RESETVAL       (0x0000u)


  /* CSL_INTC20_SFR */
#define CSL_INTC20_SFR_IRQ_MASK       (0x000Fu)
#define CSL_INTC20_SFR_IRQ_SHIFT      (0x0000u)
#define CSL_INTC20_SFR_IRQ_RESETVAL   (0x0000u)

#define CSL_INTC20_SFR_RESETVAL       (0x0000u)


  /* CSL_INTC20_ICR */
#define CSL_INTC20_ICR_GLOBAL_MASK_MASK                                   \
          (0x0004u)
#define CSL_INTC20_ICR_GLOBAL_MASK_SHIFT                                  \
          (0x0002u)
#define CSL_INTC20_ICR_GLOBAL_MASK_RESETVAL                               \
          (0x0000u)

#define CSL_INTC20_ICR_NEW_FIQ_AGR_MASK                                   \
          (0x0002u)
#define CSL_INTC20_ICR_NEW_FIQ_AGR_SHIFT                                  \
          (0x0001u)
#define CSL_INTC20_ICR_NEW_FIQ_AGR_RESETVAL                               \
          (0x0000u)

#define CSL_INTC20_ICR_NEW_IRQ_AGR_MASK                                   \
          (0x0001u)
#define CSL_INTC20_ICR_NEW_IRQ_AGR_SHIFT                                  \
          (0x0000u)
#define CSL_INTC20_ICR_NEW_IRQ_AGR_RESETVAL                               \
          (0x0000u)

#define CSL_INTC20_ICR_RESETVAL       (0x0000u)


  /* CSL_INTC20_ISR */
#define CSL_INTC20_ISR_ISR0_MASK      (0x8000u)
#define CSL_INTC20_ISR_ISR0_SHIFT     (0x000Fu)
#define CSL_INTC20_ISR_ISR0_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR1_MASK      (0x4000u)
#define CSL_INTC20_ISR_ISR1_SHIFT     (0x000Eu)
#define CSL_INTC20_ISR_ISR1_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR2_MASK      (0x2000u)
#define CSL_INTC20_ISR_ISR2_SHIFT     (0x000Du)
#define CSL_INTC20_ISR_ISR2_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR3_MASK      (0x1000u)
#define CSL_INTC20_ISR_ISR3_SHIFT     (0x000Cu)
#define CSL_INTC20_ISR_ISR3_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR4_MASK      (0x0800u)
#define CSL_INTC20_ISR_ISR4_SHIFT     (0x000Bu)
#define CSL_INTC20_ISR_ISR4_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR5_MASK      (0x0400u)
#define CSL_INTC20_ISR_ISR5_SHIFT     (0x000Au)
#define CSL_INTC20_ISR_ISR5_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR6_MASK      (0x0200u)
#define CSL_INTC20_ISR_ISR6_SHIFT     (0x0009u)
#define CSL_INTC20_ISR_ISR6_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR7_MASK      (0x0100u)
#define CSL_INTC20_ISR_ISR7_SHIFT     (0x0008u)
#define CSL_INTC20_ISR_ISR7_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR8_MASK      (0x0080u)
#define CSL_INTC20_ISR_ISR8_SHIFT     (0x0007u)
#define CSL_INTC20_ISR_ISR8_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR9_MASK      (0x0040u)
#define CSL_INTC20_ISR_ISR9_SHIFT     (0x0006u)
#define CSL_INTC20_ISR_ISR9_RESETVAL  (0x0000u)

#define CSL_INTC20_ISR_ISR10_MASK     (0x0020u)
#define CSL_INTC20_ISR_ISR10_SHIFT    (0x0005u)
#define CSL_INTC20_ISR_ISR10_RESETVAL (0x0000u)

#define CSL_INTC20_ISR_ISR11_MASK     (0x0010u)
#define CSL_INTC20_ISR_ISR11_SHIFT    (0x0004u)
#define CSL_INTC20_ISR_ISR11_RESETVAL (0x0000u)

#define CSL_INTC20_ISR_ISR12_MASK     (0x0008u)
#define CSL_INTC20_ISR_ISR12_SHIFT    (0x0003u)
#define CSL_INTC20_ISR_ISR12_RESETVAL (0x0000u)

#define CSL_INTC20_ISR_ISR13_MASK     (0x0004u)
#define CSL_INTC20_ISR_ISR13_SHIFT    (0x0002u)
#define CSL_INTC20_ISR_ISR13_RESETVAL (0x0000u)

#define CSL_INTC20_ISR_ISR14_MASK     (0x0002u)
#define CSL_INTC20_ISR_ISR14_SHIFT    (0x0001u)
#define CSL_INTC20_ISR_ISR14_RESETVAL (0x0000u)

#define CSL_INTC20_ISR_ISR15_MASK     (0x0001u)
#define CSL_INTC20_ISR_ISR15_SHIFT    (0x0000u)
#define CSL_INTC20_ISR_ISR15_RESETVAL (0x0000u)

#define CSL_INTC20_ISR_RESETVAL       (0x0000u)


  /* CSL_INTC20_ILR */
#define CSL_INTC20_ILR_PRIORITY_MASK  (0x003Cu)
#define CSL_INTC20_ILR_PRIORITY_SHIFT (0x0002u)
#define CSL_INTC20_ILR_PRIORITY_RESETVAL                                  \
          (0x0000u)

#define CSL_INTC20_ILR_SENSE_EDGE_MASK                                    \
          (0x0002u)
#define CSL_INTC20_ILR_SENSE_EDGE_SHIFT                                   \
          (0x0001u)
#define CSL_INTC20_ILR_SENSE_EDGE_RESETVAL                                \
          (0x0000u)

#define CSL_INTC20_ILR_FIQ_MASK       (0x0001u)
#define CSL_INTC20_ILR_FIQ_SHIFT      (0x0000u)
#define CSL_INTC20_ILR_FIQ_RESETVAL   (0x0000u)

#define CSL_INTC20_ILR_RESETVAL       (0x0000u)




 #endif
