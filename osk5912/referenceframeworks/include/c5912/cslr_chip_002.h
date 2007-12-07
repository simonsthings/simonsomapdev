/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef _CSLR_CHIP_002_H_
#define _CSLR_CHIP_002_H_

#include <cslr.h>
#include <csl_types.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/


typedef struct  {
    CSL_Reg16 IER0;
    CSL_Reg16 IFR0;
    CSL_Reg16 ST0_55;
    CSL_Reg16 ST1_55;
    CSL_Reg16 ST3_55;
    const CSL_Reg16 PAD0[42];
    CSL_Reg16 PDP;
    const CSL_Reg16 PAD1[21];
    CSL_Reg16 IER1;
    CSL_Reg16 IFR1;
    CSL_Reg16 DBIER0;
    CSL_Reg16 DBIER1;
    CSL_Reg16 IVPD;
    CSL_Reg16 IVPH;
    CSL_Reg16 ST2_55;
} CSL_ChipRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/
  /* CSL_CHIP_IER0 */
#define CSL_CHIP_IER0_IE15_MASK       (0x8000u)
#define CSL_CHIP_IER0_IE15_SHIFT      (0x000Fu)
#define CSL_CHIP_IER0_IE15_RESETVAL   (0x0000u)
#define CSL_CHIP_IER0_IE15_DISABLE    (0x0000u)
#define CSL_CHIP_IER0_IE15_ENABLE     (0x0001u)

#define CSL_CHIP_IER0_IE14_MASK       (0x4000u)
#define CSL_CHIP_IER0_IE14_SHIFT      (0x000Eu)
#define CSL_CHIP_IER0_IE14_RESETVAL   (0x0000u)
#define CSL_CHIP_IER0_IE14_DISABLE    (0x0000u)
#define CSL_CHIP_IER0_IE14_ENABLE     (0x0001u)

#define CSL_CHIP_IER0_IE13_MASK       (0x2000u)
#define CSL_CHIP_IER0_IE13_SHIFT      (0x000Du)
#define CSL_CHIP_IER0_IE13_RESETVAL   (0x0000u)
#define CSL_CHIP_IER0_IE13_DISABLE    (0x0000u)
#define CSL_CHIP_IER0_IE13_ENABLE     (0x0001u)

#define CSL_CHIP_IER0_IE12_MASK       (0x1000u)
#define CSL_CHIP_IER0_IE12_SHIFT      (0x000Cu)
#define CSL_CHIP_IER0_IE12_RESETVAL   (0x0000u)
#define CSL_CHIP_IER0_IE12_DISABLE    (0x0000u)
#define CSL_CHIP_IER0_IE12_ENABLE     (0x0001u)

#define CSL_CHIP_IER0_IE11_MASK       (0x0800u)
#define CSL_CHIP_IER0_IE11_SHIFT      (0x000Bu)
#define CSL_CHIP_IER0_IE11_RESETVAL   (0x0000u)
#define CSL_CHIP_IER0_IE11_DISABLE    (0x0000u)
#define CSL_CHIP_IER0_IE11_ENABLE     (0x0001u)

#define CSL_CHIP_IER0_IE10_MASK       (0x0400u)
#define CSL_CHIP_IER0_IE10_SHIFT      (0x000Au)
#define CSL_CHIP_IER0_IE10_RESETVAL   (0x0000u)
#define CSL_CHIP_IER0_IE10_DISABLE    (0x0000u)
#define CSL_CHIP_IER0_IE10_ENABLE     (0x0001u)

#define CSL_CHIP_IER0_IE9_MASK        (0x0200u)
#define CSL_CHIP_IER0_IE9_SHIFT       (0x0009u)
#define CSL_CHIP_IER0_IE9_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE9_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE9_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_IE8_MASK        (0x0100u)
#define CSL_CHIP_IER0_IE8_SHIFT       (0x0008u)
#define CSL_CHIP_IER0_IE8_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE8_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE8_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_IE7_MASK        (0x0080u)
#define CSL_CHIP_IER0_IE7_SHIFT       (0x0007u)
#define CSL_CHIP_IER0_IE7_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE7_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE7_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_IE6_MASK        (0x0040u)
#define CSL_CHIP_IER0_IE6_SHIFT       (0x0006u)
#define CSL_CHIP_IER0_IE6_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE6_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE6_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_IE5_MASK        (0x0020u)
#define CSL_CHIP_IER0_IE5_SHIFT       (0x0005u)
#define CSL_CHIP_IER0_IE5_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE5_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE5_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_IE4_MASK        (0x0010u)
#define CSL_CHIP_IER0_IE4_SHIFT       (0x0004u)
#define CSL_CHIP_IER0_IE4_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE4_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE4_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_IE3_MASK        (0x0008u)
#define CSL_CHIP_IER0_IE3_SHIFT       (0x0003u)
#define CSL_CHIP_IER0_IE3_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE3_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE3_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_IE2_MASK        (0x0004u)
#define CSL_CHIP_IER0_IE2_SHIFT       (0x0002u)
#define CSL_CHIP_IER0_IE2_RESETVAL    (0x0000u)
#define CSL_CHIP_IER0_IE2_DISABLE     (0x0000u)
#define CSL_CHIP_IER0_IE2_ENABLE      (0x0001u)

#define CSL_CHIP_IER0_RESETVAL        (0x0000u)


  /* CSL_CHIP_IFR0 */
#define CSL_CHIP_IFR0_IF15_MASK       (0x8000u)
#define CSL_CHIP_IFR0_IF15_SHIFT      (0x000Fu)
#define CSL_CHIP_IFR0_IF15_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR0_IF15_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR0_IF15_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR0_IF14_MASK       (0x4000u)
#define CSL_CHIP_IFR0_IF14_SHIFT      (0x000Eu)
#define CSL_CHIP_IFR0_IF14_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR0_IF14_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR0_IF14_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR0_IF13_MASK       (0x2000u)
#define CSL_CHIP_IFR0_IF13_SHIFT      (0x000Du)
#define CSL_CHIP_IFR0_IF13_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR0_IF13_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR0_IF13_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR0_IF12_MASK       (0x1000u)
#define CSL_CHIP_IFR0_IF12_SHIFT      (0x000Cu)
#define CSL_CHIP_IFR0_IF12_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR0_IF12_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR0_IF12_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR0_IF11_MASK       (0x0800u)
#define CSL_CHIP_IFR0_IF11_SHIFT      (0x000Bu)
#define CSL_CHIP_IFR0_IF11_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR0_IF11_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR0_IF11_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR0_IF10_MASK       (0x0400u)
#define CSL_CHIP_IFR0_IF10_SHIFT      (0x000Au)
#define CSL_CHIP_IFR0_IF10_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR0_IF10_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR0_IF10_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR0_IF9_MASK        (0x0200u)
#define CSL_CHIP_IFR0_IF9_SHIFT       (0x0009u)
#define CSL_CHIP_IFR0_IF9_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF9_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF9_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_IF8_MASK        (0x0100u)
#define CSL_CHIP_IFR0_IF8_SHIFT       (0x0008u)
#define CSL_CHIP_IFR0_IF8_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF8_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF8_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_IF7_MASK        (0x0080u)
#define CSL_CHIP_IFR0_IF7_SHIFT       (0x0007u)
#define CSL_CHIP_IFR0_IF7_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF7_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF7_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_IF6_MASK        (0x0040u)
#define CSL_CHIP_IFR0_IF6_SHIFT       (0x0006u)
#define CSL_CHIP_IFR0_IF6_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF6_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF6_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_IF5_MASK        (0x0020u)
#define CSL_CHIP_IFR0_IF5_SHIFT       (0x0005u)
#define CSL_CHIP_IFR0_IF5_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF5_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF5_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_IF4_MASK        (0x0010u)
#define CSL_CHIP_IFR0_IF4_SHIFT       (0x0004u)
#define CSL_CHIP_IFR0_IF4_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF4_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF4_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_IF3_MASK        (0x0008u)
#define CSL_CHIP_IFR0_IF3_SHIFT       (0x0003u)
#define CSL_CHIP_IFR0_IF3_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF3_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF3_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_IF2_MASK        (0x0004u)
#define CSL_CHIP_IFR0_IF2_SHIFT       (0x0002u)
#define CSL_CHIP_IFR0_IF2_RESETVAL    (0x0000u)
#define CSL_CHIP_IFR0_IF2_FLAGCLR     (0x0000u)
#define CSL_CHIP_IFR0_IF2_FLAGSET     (0x0001u)

#define CSL_CHIP_IFR0_RESETVAL        (0x0000u)


  /* CSL_CHIP_ST0_55 */
#define CSL_CHIP_ST0_55_ACOV2_MASK    (0x8000u)
#define CSL_CHIP_ST0_55_ACOV2_SHIFT   (0x000Fu)
#define CSL_CHIP_ST0_55_ACOV2_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST0_55_AC0V3_MASK    (0x4000u)
#define CSL_CHIP_ST0_55_AC0V3_SHIFT   (0x000Eu)
#define CSL_CHIP_ST0_55_AC0V3_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST0_55_TC1_MASK      (0x2000u)
#define CSL_CHIP_ST0_55_TC1_SHIFT     (0x000Du)
#define CSL_CHIP_ST0_55_TC1_RESETVAL  (0x0001u)

#define CSL_CHIP_ST0_55_TC2_MASK      (0x1000u)
#define CSL_CHIP_ST0_55_TC2_SHIFT     (0x000Cu)
#define CSL_CHIP_ST0_55_TC2_RESETVAL  (0x0001u)

#define CSL_CHIP_ST0_55_CARRY_MASK    (0x0800u)
#define CSL_CHIP_ST0_55_CARRY_SHIFT   (0x000Bu)
#define CSL_CHIP_ST0_55_CARRY_RESETVAL                                    \
          (0x0001u)

#define CSL_CHIP_ST0_55_ACOV0_MASK    (0x0400u)
#define CSL_CHIP_ST0_55_ACOV0_SHIFT   (0x000Au)
#define CSL_CHIP_ST0_55_ACOV0_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST0_55_AC0V1_MASK    (0x0200u)
#define CSL_CHIP_ST0_55_AC0V1_SHIFT   (0x0009u)
#define CSL_CHIP_ST0_55_AC0V1_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST0_55_DP_MASK       (0x01FFu)
#define CSL_CHIP_ST0_55_DP_SHIFT      (0x0000u)
#define CSL_CHIP_ST0_55_DP_RESETVAL   (0x0000u)

#define CSL_CHIP_ST0_55_RESETVAL      (0x3800u)


  /* CSL_CHIP_ST1_55 */
#define CSL_CHIP_ST1_55_BRAF_MASK     (0x8000u)
#define CSL_CHIP_ST1_55_BRAF_SHIFT    (0x000Fu)
#define CSL_CHIP_ST1_55_BRAF_RESETVAL (0x0000u)
#define CSL_CHIP_ST1_55_BRAF_BRDISABLE                                    \
          (0x0000u)
#define CSL_CHIP_ST1_55_BRAF_BRENABLE (0x0001u)

#define CSL_CHIP_ST1_55_CPL_MASK      (0x4000u)
#define CSL_CHIP_ST1_55_CPL_SHIFT     (0x000Eu)
#define CSL_CHIP_ST1_55_CPL_RESETVAL  (0x0000u)
#define CSL_CHIP_ST1_55_CPL_DPADDRESSMODE                                 \
          (0x0000u)
#define CSL_CHIP_ST1_55_CPL_SPADDRESSMODE                                 \
          (0x0001u)

#define CSL_CHIP_ST1_55_XF_MASK       (0x2000u)
#define CSL_CHIP_ST1_55_XF_SHIFT      (0x000Du)
#define CSL_CHIP_ST1_55_XF_RESETVAL   (0x0001u)

#define CSL_CHIP_ST1_55_HM_MASK       (0x1000u)
#define CSL_CHIP_ST1_55_HM_SHIFT      (0x000Cu)
#define CSL_CHIP_ST1_55_HM_RESETVAL   (0x0000u)

#define CSL_CHIP_ST1_55_INTM_MASK     (0x0800u)
#define CSL_CHIP_ST1_55_INTM_SHIFT    (0x000Bu)
#define CSL_CHIP_ST1_55_INTM_RESETVAL (0x0001u)

#define CSL_CHIP_ST1_55_M40_MASK      (0x0400u)
#define CSL_CHIP_ST1_55_M40_SHIFT     (0x000Au)
#define CSL_CHIP_ST1_55_M40_RESETVAL  (0x0000u)

#define CSL_CHIP_ST1_55_SATD_MASK     (0x0200u)
#define CSL_CHIP_ST1_55_SATD_SHIFT    (0x0009u)
#define CSL_CHIP_ST1_55_SATD_RESETVAL (0x0000u)

#define CSL_CHIP_ST1_55_SXMD_MASK     (0x0100u)
#define CSL_CHIP_ST1_55_SXMD_SHIFT    (0x0008u)
#define CSL_CHIP_ST1_55_SXMD_RESETVAL (0x0001u)

#define CSL_CHIP_ST1_55_C16_MASK      (0x0080u)
#define CSL_CHIP_ST1_55_C16_SHIFT     (0x0007u)
#define CSL_CHIP_ST1_55_C16_RESETVAL  (0x0000u)

#define CSL_CHIP_ST1_55_FRCT_MASK     (0x0040u)
#define CSL_CHIP_ST1_55_FRCT_SHIFT    (0x0006u)
#define CSL_CHIP_ST1_55_FRCT_RESETVAL (0x0000u)

#define CSL_CHIP_ST1_55_C54CM_MASK    (0x0020u)
#define CSL_CHIP_ST1_55_C54CM_SHIFT   (0x0005u)
#define CSL_CHIP_ST1_55_C54CM_RESETVAL                                    \
          (0x0001u)

#define CSL_CHIP_ST1_55_ASM_MASK      (0x001Fu)
#define CSL_CHIP_ST1_55_ASM_SHIFT     (0x0000u)
#define CSL_CHIP_ST1_55_ASM_RESETVAL  (0x0000u)

#define CSL_CHIP_ST1_55_RESETVAL      (0x2920u)


  /* CSL_CHIP_ST3_55 */
#define CSL_CHIP_ST3_55_CAFRZ_MASK    (0x8000u)
#define CSL_CHIP_ST3_55_CAFRZ_SHIFT   (0x000Fu)
#define CSL_CHIP_ST3_55_CAFRZ_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST3_55_CAEN_MASK     (0x4000u)
#define CSL_CHIP_ST3_55_CAEN_SHIFT    (0x000Eu)
#define CSL_CHIP_ST3_55_CAEN_RESETVAL (0x0000u)

#define CSL_CHIP_ST3_55_CACLR_MASK    (0x2000u)
#define CSL_CHIP_ST3_55_CACLR_SHIFT   (0x000Du)
#define CSL_CHIP_ST3_55_CACLR_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST3_55_HINT_MASK     (0x1000u)
#define CSL_CHIP_ST3_55_HINT_SHIFT    (0x000Cu)
#define CSL_CHIP_ST3_55_HINT_RESETVAL (0x0001u)

#define CSL_CHIP_ST3_55_CBERR_MASK    (0x0080u)
#define CSL_CHIP_ST3_55_CBERR_SHIFT   (0x0007u)
#define CSL_CHIP_ST3_55_CBERR_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST3_55_MPNMC_MASK    (0x0040u)
#define CSL_CHIP_ST3_55_MPNMC_SHIFT   (0x0006u)
#define CSL_CHIP_ST3_55_MPNMC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST3_55_SATA_MASK     (0x0020u)
#define CSL_CHIP_ST3_55_SATA_SHIFT    (0x0005u)
#define CSL_CHIP_ST3_55_SATA_RESETVAL (0x0000u)

#define CSL_CHIP_ST3_55_CLKOFF_MASK   (0x0004u)
#define CSL_CHIP_ST3_55_CLKOFF_SHIFT  (0x0002u)
#define CSL_CHIP_ST3_55_CLKOFF_RESETVAL                                   \
          (0x0000u)

#define CSL_CHIP_ST3_55_SMUL_MASK     (0x0002u)
#define CSL_CHIP_ST3_55_SMUL_SHIFT    (0x0001u)
#define CSL_CHIP_ST3_55_SMUL_RESETVAL (0x0000u)

#define CSL_CHIP_ST3_55_SST_MASK      (0x0001u)
#define CSL_CHIP_ST3_55_SST_SHIFT     (0x0000u)
#define CSL_CHIP_ST3_55_SST_RESETVAL  (0x0000u)

#define CSL_CHIP_ST3_55_RESETVAL      (0x1000u)


  /* CSL_CHIP_PDP */
#define CSL_CHIP_PDP_PDP_MASK         (0x01FFu)
#define CSL_CHIP_PDP_PDP_SHIFT        (0x0000u)
#define CSL_CHIP_PDP_PDP_RESETVAL     (0x0000u)

#define CSL_CHIP_PDP_RESETVAL         (0x0000u)


  /* CSL_CHIP_IER1 */
#define CSL_CHIP_IER1_RTOSINTE_MASK   (0x0400u)
#define CSL_CHIP_IER1_RTOSINTE_SHIFT  (0x000Au)
#define CSL_CHIP_IER1_RTOSINTE_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_IER1_RTOSINTE_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_IER1_RTOSINTE_ENABLE (0x0001u)

#define CSL_CHIP_IER1_DLOGINTE_MASK   (0x0200u)
#define CSL_CHIP_IER1_DLOGINTE_SHIFT  (0x0009u)
#define CSL_CHIP_IER1_DLOGINTE_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_IER1_DLOGINTE_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_IER1_DLOGINTE_ENABLE (0x0001u)

#define CSL_CHIP_IER1_BERRINTE_MASK   (0x0100u)
#define CSL_CHIP_IER1_BERRINTE_SHIFT  (0x0008u)
#define CSL_CHIP_IER1_BERRINTE_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_IER1_BERRINTE_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_IER1_BERRINTE_ENABLE (0x0001u)

#define CSL_CHIP_IER1_IE23_MASK       (0x0080u)
#define CSL_CHIP_IER1_IE23_SHIFT      (0x0007u)
#define CSL_CHIP_IER1_IE23_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE23_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE23_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_IE22_MASK       (0x0040u)
#define CSL_CHIP_IER1_IE22_SHIFT      (0x0006u)
#define CSL_CHIP_IER1_IE22_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE22_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE22_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_IE21_MASK       (0x0020u)
#define CSL_CHIP_IER1_IE21_SHIFT      (0x0005u)
#define CSL_CHIP_IER1_IE21_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE21_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE21_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_IE20_MASK       (0x0010u)
#define CSL_CHIP_IER1_IE20_SHIFT      (0x0004u)
#define CSL_CHIP_IER1_IE20_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE20_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE20_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_IE19_MASK       (0x0008u)
#define CSL_CHIP_IER1_IE19_SHIFT      (0x0003u)
#define CSL_CHIP_IER1_IE19_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE19_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE19_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_IE18_MASK       (0x0004u)
#define CSL_CHIP_IER1_IE18_SHIFT      (0x0002u)
#define CSL_CHIP_IER1_IE18_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE18_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE18_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_IE17_MASK       (0x0002u)
#define CSL_CHIP_IER1_IE17_SHIFT      (0x0001u)
#define CSL_CHIP_IER1_IE17_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE17_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE17_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_IE16_MASK       (0x0001u)
#define CSL_CHIP_IER1_IE16_SHIFT      (0x0000u)
#define CSL_CHIP_IER1_IE16_RESETVAL   (0x0000u)
#define CSL_CHIP_IER1_IE16_DISABLE    (0x0000u)
#define CSL_CHIP_IER1_IE16_ENABLE     (0x0001u)

#define CSL_CHIP_IER1_RESETVAL        (0x0000u)


  /* CSL_CHIP_IFR1 */
#define CSL_CHIP_IFR1_RTOSINTF_MASK   (0x0400u)
#define CSL_CHIP_IFR1_RTOSINTF_SHIFT  (0x000Au)
#define CSL_CHIP_IFR1_RTOSINTF_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_IFR1_RTOSINTF_FLAGCLR                                    \
          (0x0000u)
#define CSL_CHIP_IFR1_RTOSINTF_FLAGSET                                    \
          (0x0001u)

#define CSL_CHIP_IFR1_DLOGINTF_MASK   (0x0200u)
#define CSL_CHIP_IFR1_DLOGINTF_SHIFT  (0x0009u)
#define CSL_CHIP_IFR1_DLOGINTF_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_IFR1_DLOGINTF_FLAGCLR                                    \
          (0x0000u)
#define CSL_CHIP_IFR1_DLOGINTF_FLAGSET                                    \
          (0x0001u)

#define CSL_CHIP_IFR1_BERRINTF_MASK   (0x0100u)
#define CSL_CHIP_IFR1_BERRINTF_SHIFT  (0x0008u)
#define CSL_CHIP_IFR1_BERRINTF_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_IFR1_BERRINTF_FLAGCLR                                    \
          (0x0000u)
#define CSL_CHIP_IFR1_BERRINTF_FLAGSET                                    \
          (0x0001u)

#define CSL_CHIP_IFR1_IF23_MASK       (0x0080u)
#define CSL_CHIP_IFR1_IF23_SHIFT      (0x0007u)
#define CSL_CHIP_IFR1_IF23_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF23_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF23_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_IF22_MASK       (0x0040u)
#define CSL_CHIP_IFR1_IF22_SHIFT      (0x0006u)
#define CSL_CHIP_IFR1_IF22_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF22_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF22_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_IF21_MASK       (0x0020u)
#define CSL_CHIP_IFR1_IF21_SHIFT      (0x0005u)
#define CSL_CHIP_IFR1_IF21_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF21_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF21_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_IF20_MASK       (0x0010u)
#define CSL_CHIP_IFR1_IF20_SHIFT      (0x0004u)
#define CSL_CHIP_IFR1_IF20_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF20_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF20_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_IF19_MASK       (0x0008u)
#define CSL_CHIP_IFR1_IF19_SHIFT      (0x0003u)
#define CSL_CHIP_IFR1_IF19_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF19_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF19_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_IF18_MASK       (0x0004u)
#define CSL_CHIP_IFR1_IF18_SHIFT      (0x0002u)
#define CSL_CHIP_IFR1_IF18_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF18_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF18_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_IF17_MASK       (0x0002u)
#define CSL_CHIP_IFR1_IF17_SHIFT      (0x0001u)
#define CSL_CHIP_IFR1_IF17_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF17_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF17_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_IF16_MASK       (0x0001u)
#define CSL_CHIP_IFR1_IF16_SHIFT      (0x0000u)
#define CSL_CHIP_IFR1_IF16_RESETVAL   (0x0000u)
#define CSL_CHIP_IFR1_IF16_FLAGCLR    (0x0000u)
#define CSL_CHIP_IFR1_IF16_FLAGSET    (0x0001u)

#define CSL_CHIP_IFR1_RESETVAL        (0x0000u)


  /* CSL_CHIP_DBIER0 */
#define CSL_CHIP_DBIER0_DBIE15_MASK   (0x8000u)
#define CSL_CHIP_DBIER0_DBIE15_SHIFT  (0x000Fu)
#define CSL_CHIP_DBIER0_DBIE15_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE15_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE15_ENABLE (0x0001u)

#define CSL_CHIP_DBIER0_DBIE14_MASK   (0x4000u)
#define CSL_CHIP_DBIER0_DBIE14_SHIFT  (0x000Eu)
#define CSL_CHIP_DBIER0_DBIE14_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE14_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE14_ENABLE (0x0001u)

#define CSL_CHIP_DBIER0_DBIE13_MASK   (0x2000u)
#define CSL_CHIP_DBIER0_DBIE13_SHIFT  (0x000Du)
#define CSL_CHIP_DBIER0_DBIE13_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE13_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE13_ENABLE (0x0001u)

#define CSL_CHIP_DBIER0_DBIE12_MASK   (0x1000u)
#define CSL_CHIP_DBIER0_DBIE12_SHIFT  (0x000Cu)
#define CSL_CHIP_DBIER0_DBIE12_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE12_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE12_ENABLE (0x0001u)

#define CSL_CHIP_DBIER0_DBIE11_MASK   (0x0800u)
#define CSL_CHIP_DBIER0_DBIE11_SHIFT  (0x000Bu)
#define CSL_CHIP_DBIER0_DBIE11_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE11_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE11_ENABLE (0x0001u)

#define CSL_CHIP_DBIER0_DBIE10_MASK   (0x0400u)
#define CSL_CHIP_DBIER0_DBIE10_SHIFT  (0x000Au)
#define CSL_CHIP_DBIER0_DBIE10_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE10_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE10_ENABLE (0x0001u)

#define CSL_CHIP_DBIER0_DBIE9_MASK    (0x0200u)
#define CSL_CHIP_DBIER0_DBIE9_SHIFT   (0x0009u)
#define CSL_CHIP_DBIER0_DBIE9_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE9_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE9_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_DBIE8_MASK    (0x0100u)
#define CSL_CHIP_DBIER0_DBIE8_SHIFT   (0x0008u)
#define CSL_CHIP_DBIER0_DBIE8_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE8_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE8_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_DBIE7_MASK    (0x0080u)
#define CSL_CHIP_DBIER0_DBIE7_SHIFT   (0x0007u)
#define CSL_CHIP_DBIER0_DBIE7_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE7_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE7_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_DBIE6_MASK    (0x0040u)
#define CSL_CHIP_DBIER0_DBIE6_SHIFT   (0x0006u)
#define CSL_CHIP_DBIER0_DBIE6_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE6_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE6_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_DBIE5_MASK    (0x0020u)
#define CSL_CHIP_DBIER0_DBIE5_SHIFT   (0x0005u)
#define CSL_CHIP_DBIER0_DBIE5_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE5_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE5_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_DBIE4_MASK    (0x0010u)
#define CSL_CHIP_DBIER0_DBIE4_SHIFT   (0x0004u)
#define CSL_CHIP_DBIER0_DBIE4_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE4_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE4_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_DBIE3_MASK    (0x0008u)
#define CSL_CHIP_DBIER0_DBIE3_SHIFT   (0x0003u)
#define CSL_CHIP_DBIER0_DBIE3_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE3_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE3_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_DBIE2_MASK    (0x0004u)
#define CSL_CHIP_DBIER0_DBIE2_SHIFT   (0x0002u)
#define CSL_CHIP_DBIER0_DBIE2_RESETVAL                                    \
          (0x0000u)
#define CSL_CHIP_DBIER0_DBIE2_DISABLE (0x0000u)
#define CSL_CHIP_DBIER0_DBIE2_ENABLE  (0x0001u)

#define CSL_CHIP_DBIER0_RESETVAL      (0x0000u)


  /* CSL_CHIP_DBIER1 */
#define CSL_CHIP_DBIER1_RTOSINTD_MASK (0x0400u)
#define CSL_CHIP_DBIER1_RTOSINTD_SHIFT                                    \
          (0x000Au)
#define CSL_CHIP_DBIER1_RTOSINTD_RESETVAL                                 \
          (0x0000u)
#define CSL_CHIP_DBIER1_RTOSINTD_DISABLE                                  \
          (0x0000u)
#define CSL_CHIP_DBIER1_RTOSINTD_ENABLE                                   \
          (0x0001u)

#define CSL_CHIP_DBIER1_DLOGINTD_MASK (0x0200u)
#define CSL_CHIP_DBIER1_DLOGINTD_SHIFT                                    \
          (0x0009u)
#define CSL_CHIP_DBIER1_DLOGINTD_RESETVAL                                 \
          (0x0000u)
#define CSL_CHIP_DBIER1_DLOGINTD_DISABLE                                  \
          (0x0000u)
#define CSL_CHIP_DBIER1_DLOGINTD_ENABLE                                   \
          (0x0001u)

#define CSL_CHIP_DBIER1_BERRINTD_MASK (0x0100u)
#define CSL_CHIP_DBIER1_BERRINTD_SHIFT                                    \
          (0x0008u)
#define CSL_CHIP_DBIER1_BERRINTD_RESETVAL                                 \
          (0x0000u)
#define CSL_CHIP_DBIER1_BERRINTD_DISABLE                                  \
          (0x0000u)
#define CSL_CHIP_DBIER1_BERRINTD_ENABLE                                   \
          (0x0001u)

#define CSL_CHIP_DBIER1_DBIE23_MASK   (0x0080u)
#define CSL_CHIP_DBIER1_DBIE23_SHIFT  (0x0007u)
#define CSL_CHIP_DBIER1_DBIE23_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE23_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE23_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_DBIE22_MASK   (0x0040u)
#define CSL_CHIP_DBIER1_DBIE22_SHIFT  (0x0006u)
#define CSL_CHIP_DBIER1_DBIE22_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE22_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE22_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_DBIE21_MASK   (0x0020u)
#define CSL_CHIP_DBIER1_DBIE21_SHIFT  (0x0005u)
#define CSL_CHIP_DBIER1_DBIE21_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE21_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE21_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_DBIE20_MASK   (0x0010u)
#define CSL_CHIP_DBIER1_DBIE20_SHIFT  (0x0004u)
#define CSL_CHIP_DBIER1_DBIE20_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE20_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE20_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_DBIE19_MASK   (0x0008u)
#define CSL_CHIP_DBIER1_DBIE19_SHIFT  (0x0003u)
#define CSL_CHIP_DBIER1_DBIE19_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE19_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE19_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_DBIE18_MASK   (0x0004u)
#define CSL_CHIP_DBIER1_DBIE18_SHIFT  (0x0002u)
#define CSL_CHIP_DBIER1_DBIE18_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE18_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE18_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_DBIE17_MASK   (0x0002u)
#define CSL_CHIP_DBIER1_DBIE17_SHIFT  (0x0001u)
#define CSL_CHIP_DBIER1_DBIE17_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE17_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE17_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_DBIE16_MASK   (0x0001u)
#define CSL_CHIP_DBIER1_DBIE16_SHIFT  (0x0000u)
#define CSL_CHIP_DBIER1_DBIE16_RESETVAL                                   \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE16_DISABLE                                    \
          (0x0000u)
#define CSL_CHIP_DBIER1_DBIE16_ENABLE (0x0001u)

#define CSL_CHIP_DBIER1_RESETVAL      (0x0000u)


  /* CSL_CHIP_IVPD */
#define CSL_CHIP_IVPD_IVPD_MASK       (0xFFFFu)
#define CSL_CHIP_IVPD_IVPD_SHIFT      (0x0000u)
#define CSL_CHIP_IVPD_IVPD_RESETVAL   (0xFFFFu)

#define CSL_CHIP_IVPD_RESETVAL        (0xFFFFu)


  /* CSL_CHIP_IVPH */
#define CSL_CHIP_IVPH_IVPH_MASK       (0xFFFFu)
#define CSL_CHIP_IVPH_IVPH_SHIFT      (0x0000u)
#define CSL_CHIP_IVPH_IVPH_RESETVAL   (0xFFFFu)

#define CSL_CHIP_IVPH_RESETVAL        (0xFFFFu)


  /* CSL_CHIP_ST2_55 */
#define CSL_CHIP_ST2_55_ARMS_MASK     (0x8000u)
#define CSL_CHIP_ST2_55_ARMS_SHIFT    (0x000Fu)
#define CSL_CHIP_ST2_55_ARMS_RESETVAL (0x0000u)

#define CSL_CHIP_ST2_55_DBGM_MASK     (0x1000u)
#define CSL_CHIP_ST2_55_DBGM_SHIFT    (0x000Cu)
#define CSL_CHIP_ST2_55_DBGM_RESETVAL (0x0000u)

#define CSL_CHIP_ST2_55_EALLOW_MASK   (0x0800u)
#define CSL_CHIP_ST2_55_EALLOW_SHIFT  (0x000Bu)
#define CSL_CHIP_ST2_55_EALLOW_RESETVAL                                   \
          (0x0000u)

#define CSL_CHIP_ST2_55_RDM_MASK      (0x0400u)
#define CSL_CHIP_ST2_55_RDM_SHIFT     (0x000Au)
#define CSL_CHIP_ST2_55_RDM_RESETVAL  (0x0000u)

#define CSL_CHIP_ST2_55_CDPLC_MASK    (0x0100u)
#define CSL_CHIP_ST2_55_CDPLC_SHIFT   (0x0008u)
#define CSL_CHIP_ST2_55_CDPLC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR7LC_MASK    (0x0080u)
#define CSL_CHIP_ST2_55_AR7LC_SHIFT   (0x0007u)
#define CSL_CHIP_ST2_55_AR7LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR6LC_MASK    (0x0040u)
#define CSL_CHIP_ST2_55_AR6LC_SHIFT   (0x0006u)
#define CSL_CHIP_ST2_55_AR6LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR5LC_MASK    (0x0020u)
#define CSL_CHIP_ST2_55_AR5LC_SHIFT   (0x0005u)
#define CSL_CHIP_ST2_55_AR5LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR4LC_MASK    (0x0010u)
#define CSL_CHIP_ST2_55_AR4LC_SHIFT   (0x0004u)
#define CSL_CHIP_ST2_55_AR4LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR3LC_MASK    (0x0008u)
#define CSL_CHIP_ST2_55_AR3LC_SHIFT   (0x0003u)
#define CSL_CHIP_ST2_55_AR3LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR2LC_MASK    (0x0004u)
#define CSL_CHIP_ST2_55_AR2LC_SHIFT   (0x0002u)
#define CSL_CHIP_ST2_55_AR2LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR1LC_MASK    (0x0002u)
#define CSL_CHIP_ST2_55_AR1LC_SHIFT   (0x0001u)
#define CSL_CHIP_ST2_55_AR1LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_AR0LC_MASK    (0x0001u)
#define CSL_CHIP_ST2_55_AR0LC_SHIFT   (0x0000u)
#define CSL_CHIP_ST2_55_AR0LC_RESETVAL                                    \
          (0x0000u)

#define CSL_CHIP_ST2_55_RESETVAL      (0x0000u)




#endif
