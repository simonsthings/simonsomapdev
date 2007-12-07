/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5912PG1_0] (2003-11-12)  */

#ifndef _CSLR_DMA_001_H_
#define _CSLR_DMA_001_H_

#include <cslr.h>
#include <csl_types.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    struct CHA  {
    CSL_Reg16 CSDP;
    CSL_Reg16 CCR;
    CSL_Reg16 CICR;
    CSL_Reg16 CSR;
    CSL_Reg16 CSSAL;
    CSL_Reg16 CSSAU;
    CSL_Reg16 CDSAL;
    CSL_Reg16 CDSAU;
    CSL_Reg16 CEN;
    CSL_Reg16 CFN;
    CSL_Reg16 CSFI;
    CSL_Reg16 CSEI;
    CSL_Reg16 CSAC;
    CSL_Reg16 CDAC;
    CSL_Reg16 CDFI;
    CSL_Reg16 CDEI;
    const char RSVD0[16];
    }CHA[6];
    const CSL_Reg16 RSVD1[320];
    struct GLOBAL  {
    CSL_Reg16 GCR;
    CSL_Reg16 GTCR;
    CSL_Reg16 GSCR;
    }GLOBAL;
} CSL_DmaRegs;

typedef struct CSL_DmaChaRegs  {
    CSL_Reg16 CSDP;
    CSL_Reg16 CCR;
    CSL_Reg16 CICR;
    CSL_Reg16 CSR;
    CSL_Reg16 CSSAL;
    CSL_Reg16 CSSAU;
    CSL_Reg16 CDSAL;
    CSL_Reg16 CDSAU;
    CSL_Reg16 CEN;
    CSL_Reg16 CFN;
    CSL_Reg16 CSFI;
    CSL_Reg16 CSEI;
    CSL_Reg16 CSAC;
    CSL_Reg16 CDAC;
    CSL_Reg16 CDFI;
    CSL_Reg16 CDEI;
    const char RSVD0[16];
}CSL_DmaChaRegs;

typedef struct CSL_DmaGlobalRegs  {
    CSL_Reg16 GCR;
    CSL_Reg16 GTCR;
    CSL_Reg16 GSCR;
}CSL_DmaGlobalRegs;

/**************************************************************************\
* Resource counts
\**************************************************************************/
#define CSL_DMA_CHA_CNT   6
#define CSL_DMA_GLOBAL_CNT   1

/**************************************************************************\
* Register Id's
\**************************************************************************/
typedef enum  {
   CSL_DMA_CSDP = 0x0000u,
   CSL_DMA_CCR = 0x0001u,
   CSL_DMA_CICR = 0x0002u,
   CSL_DMA_CSR = 0x0003u,
   CSL_DMA_CSSAL = 0x0004u,
   CSL_DMA_CSSAU = 0x0005u,
   CSL_DMA_CDSAL = 0x0006u,
   CSL_DMA_CDSAU = 0x0007u,
   CSL_DMA_CEN = 0x0008u,
   CSL_DMA_CFN = 0x0009u,
   CSL_DMA_CSFI = 0x000Au,
   CSL_DMA_CSEI = 0x000Bu,
   CSL_DMA_CSAC = 0x000Cu,
   CSL_DMA_CDAC = 0x000Du,
   CSL_DMA_CDFI = 0x000Eu,
   CSL_DMA_CDEI = 0x000Fu,
   CSL_DMA_GCR = 0x0000u,
   CSL_DMA_GTCR = 0x0001u,
   CSL_DMA_GSCR = 0x0002u
} CSL_DmaRegIds;
/**************************************************************************\
* Field Definition Macros
\**************************************************************************/
  /* CSL_DMA_CSDP */
#define CSL_DMA_CSDP_DSTBEN_MASK      (0xC000u)
#define CSL_DMA_CSDP_DSTBEN_SHIFT     (0x000Eu)
#define CSL_DMA_CSDP_DSTBEN_RESETVAL  (0x0000u)

#define CSL_DMA_CSDP_DSTPACK_MASK     (0x2000u)
#define CSL_DMA_CSDP_DSTPACK_SHIFT    (0x000Du)
#define CSL_DMA_CSDP_DSTPACK_RESETVAL (0x0000u)

#define CSL_DMA_CSDP_DST_MASK         (0x1E00u)
#define CSL_DMA_CSDP_DST_SHIFT        (0x0009u)
#define CSL_DMA_CSDP_DST_RESETVAL     (0x0000u)

#define CSL_DMA_CSDP_SRCBEN_MASK      (0x0180u)
#define CSL_DMA_CSDP_SRCBEN_SHIFT     (0x0007u)
#define CSL_DMA_CSDP_SRCBEN_RESETVAL  (0x0000u)

#define CSL_DMA_CSDP_SRCPACK_MASK     (0x0040u)
#define CSL_DMA_CSDP_SRCPACK_SHIFT    (0x0006u)
#define CSL_DMA_CSDP_SRCPACK_RESETVAL (0x0000u)

#define CSL_DMA_CSDP_SRC_MASK         (0x003Cu)
#define CSL_DMA_CSDP_SRC_SHIFT        (0x0002u)
#define CSL_DMA_CSDP_SRC_RESETVAL     (0x0000u)

#define CSL_DMA_CSDP_DATATYPE_MASK    (0x0003u)
#define CSL_DMA_CSDP_DATATYPE_SHIFT   (0x0000u)
#define CSL_DMA_CSDP_DATATYPE_RESETVAL                                    \
          (0x0000u)

#define CSL_DMA_CSDP_RESETVAL         (0x0000u)


  /* CSL_DMA_CCR */
#define CSL_DMA_CCR_DSTAMODE_MASK     (0xC000u)
#define CSL_DMA_CCR_DSTAMODE_SHIFT    (0x000Eu)
#define CSL_DMA_CCR_DSTAMODE_RESETVAL (0x0000u)

#define CSL_DMA_CCR_SRCAMODE_MASK     (0x3000u)
#define CSL_DMA_CCR_SRCAMODE_SHIFT    (0x000Cu)
#define CSL_DMA_CCR_SRCAMODE_RESETVAL (0x0000u)

#define CSL_DMA_CCR_ENDPROG_MASK      (0x0800u)
#define CSL_DMA_CCR_ENDPROG_SHIFT     (0x000Bu)
#define CSL_DMA_CCR_ENDPROG_RESETVAL  (0x0000u)

#define CSL_DMA_CCR_REPEAT_MASK       (0x0200u)
#define CSL_DMA_CCR_REPEAT_SHIFT      (0x0009u)
#define CSL_DMA_CCR_REPEAT_RESETVAL   (0x0000u)

#define CSL_DMA_CCR_AUTOINIT_MASK     (0x0100u)
#define CSL_DMA_CCR_AUTOINIT_SHIFT    (0x0008u)
#define CSL_DMA_CCR_AUTOINIT_RESETVAL (0x0000u)

#define CSL_DMA_CCR_EN_MASK           (0x0080u)
#define CSL_DMA_CCR_EN_SHIFT          (0x0007u)
#define CSL_DMA_CCR_EN_RESETVAL       (0x0000u)

#define CSL_DMA_CCR_PRIO_MASK         (0x0040u)
#define CSL_DMA_CCR_PRIO_SHIFT        (0x0006u)
#define CSL_DMA_CCR_PRIO_RESETVAL     (0x0000u)

#define CSL_DMA_CCR_FS_MASK           (0x0020u)
#define CSL_DMA_CCR_FS_SHIFT          (0x0005u)
#define CSL_DMA_CCR_FS_RESETVAL       (0x0000u)

#define CSL_DMA_CCR_SYNC_MASK         (0x001Fu)
#define CSL_DMA_CCR_SYNC_SHIFT        (0x0000u)
#define CSL_DMA_CCR_SYNC_RESETVAL     (0x0000u)

#define CSL_DMA_CCR_RESETVAL          (0x0000u)


  /* CSL_DMA_CICR */
#define CSL_DMA_CICR_BLOCKIE_MASK     (0x0020u)
#define CSL_DMA_CICR_BLOCKIE_SHIFT    (0x0005u)
#define CSL_DMA_CICR_BLOCKIE_RESETVAL (0x0000u)

#define CSL_DMA_CICR_LASTIE_MASK      (0x0010u)
#define CSL_DMA_CICR_LASTIE_SHIFT     (0x0004u)
#define CSL_DMA_CICR_LASTIE_RESETVAL  (0x0000u)

#define CSL_DMA_CICR_FRAMEIE_MASK     (0x0008u)
#define CSL_DMA_CICR_FRAMEIE_SHIFT    (0x0003u)
#define CSL_DMA_CICR_FRAMEIE_RESETVAL (0x0000u)

#define CSL_DMA_CICR_FIRSTHALFIE_MASK (0x0004u)
#define CSL_DMA_CICR_FIRSTHALFIE_SHIFT                                    \
          (0x0002u)
#define CSL_DMA_CICR_FIRSTHALFIE_RESETVAL                                 \
          (0x0000u)

#define CSL_DMA_CICR_DROPIE_MASK      (0x0002u)
#define CSL_DMA_CICR_DROPIE_SHIFT     (0x0001u)
#define CSL_DMA_CICR_DROPIE_RESETVAL  (0x0001u)

#define CSL_DMA_CICR_TIMEOUTIE_MASK   (0x0001u)
#define CSL_DMA_CICR_TIMEOUTIE_SHIFT  (0x0000u)
#define CSL_DMA_CICR_TIMEOUTIE_RESETVAL                                   \
          (0x0001u)

#define CSL_DMA_CICR_RESETVAL         (0x0003u)


  /* CSL_DMA_CSR */
#define CSL_DMA_CSR_SYNC_MASK         (0x0040u)
#define CSL_DMA_CSR_SYNC_SHIFT        (0x0006u)
#define CSL_DMA_CSR_SYNC_RESETVAL     (0x0000u)

#define CSL_DMA_CSR_BLOCK_MASK        (0x0020u)
#define CSL_DMA_CSR_BLOCK_SHIFT       (0x0005u)
#define CSL_DMA_CSR_BLOCK_RESETVAL    (0x0000u)

#define CSL_DMA_CSR_LAST_MASK         (0x0010u)
#define CSL_DMA_CSR_LAST_SHIFT        (0x0004u)
#define CSL_DMA_CSR_LAST_RESETVAL     (0x0000u)

#define CSL_DMA_CSR_FRAME_MASK        (0x0008u)
#define CSL_DMA_CSR_FRAME_SHIFT       (0x0003u)
#define CSL_DMA_CSR_FRAME_RESETVAL    (0x0000u)

#define CSL_DMA_CSR_FIRSTHALF_MASK    (0x0004u)
#define CSL_DMA_CSR_FIRSTHALF_SHIFT   (0x0002u)
#define CSL_DMA_CSR_FIRSTHALF_RESETVAL                                    \
          (0x0000u)

#define CSL_DMA_CSR_DROP_MASK         (0x0002u)
#define CSL_DMA_CSR_DROP_SHIFT        (0x0001u)
#define CSL_DMA_CSR_DROP_RESETVAL     (0x0000u)

#define CSL_DMA_CSR_TIMEOUT_MASK      (0x0001u)
#define CSL_DMA_CSR_TIMEOUT_SHIFT     (0x0000u)
#define CSL_DMA_CSR_TIMEOUT_RESETVAL  (0x0000u)

#define CSL_DMA_CSR_RESETVAL          (0x0000u)


  /* CSL_DMA_CSSAL */
#define CSL_DMA_CSSAL_CSSAL_MASK      (0xFFFFu)
#define CSL_DMA_CSSAL_CSSAL_SHIFT     (0x0000u)
#define CSL_DMA_CSSAL_CSSAL_RESETVAL  (0x0000u)

#define CSL_DMA_CSSAL_RESETVAL        (0x0000u)


  /* CSL_DMA_CSSAU */
#define CSL_DMA_CSSAU_CSSAU_MASK      (0xFFFFu)
#define CSL_DMA_CSSAU_CSSAU_SHIFT     (0x0000u)
#define CSL_DMA_CSSAU_CSSAU_RESETVAL  (0x0000u)

#define CSL_DMA_CSSAU_RESETVAL        (0x0000u)


  /* CSL_DMA_CDSAL */
#define CSL_DMA_CDSAL_CDSAL_MASK      (0xFFFFu)
#define CSL_DMA_CDSAL_CDSAL_SHIFT     (0x0000u)
#define CSL_DMA_CDSAL_CDSAL_RESETVAL  (0x0000u)

#define CSL_DMA_CDSAL_RESETVAL        (0x0000u)


  /* CSL_DMA_CDSAU */
#define CSL_DMA_CDSAU_CDSAU_MASK      (0xFFFFu)
#define CSL_DMA_CDSAU_CDSAU_SHIFT     (0x0000u)
#define CSL_DMA_CDSAU_CDSAU_RESETVAL  (0x0000u)

#define CSL_DMA_CDSAU_RESETVAL        (0x0000u)


  /* CSL_DMA_CEN */
#define CSL_DMA_CEN_CEN_MASK          (0xFFFFu)
#define CSL_DMA_CEN_CEN_SHIFT         (0x0000u)
#define CSL_DMA_CEN_CEN_RESETVAL      (0x0000u)

#define CSL_DMA_CEN_RESETVAL          (0x0000u)


  /* CSL_DMA_CFN */
#define CSL_DMA_CFN_CFN_MASK          (0xFFFFu)
#define CSL_DMA_CFN_CFN_SHIFT         (0x0000u)
#define CSL_DMA_CFN_CFN_RESETVAL      (0x0000u)

#define CSL_DMA_CFN_RESETVAL          (0x0000u)


  /* CSL_DMA_CSFI */
#define CSL_DMA_CSFI_CSFI_MASK        (0xFFFFu)
#define CSL_DMA_CSFI_CSFI_SHIFT       (0x0000u)
#define CSL_DMA_CSFI_CSFI_RESETVAL    (0x0000u)

#define CSL_DMA_CSFI_RESETVAL         (0x0000u)


  /* CSL_DMA_CSEI */
#define CSL_DMA_CSEI_CSEI_MASK        (0xFFFFu)
#define CSL_DMA_CSEI_CSEI_SHIFT       (0x0000u)
#define CSL_DMA_CSEI_CSEI_RESETVAL    (0x0000u)

#define CSL_DMA_CSEI_RESETVAL         (0x0000u)


  /* CSL_DMA_CSAC */
#define CSL_DMA_CSAC_CSAC_MASK        (0xFFFFu)
#define CSL_DMA_CSAC_CSAC_SHIFT       (0x0000u)
#define CSL_DMA_CSAC_CSAC_RESETVAL    (0x0000u)

#define CSL_DMA_CSAC_RESETVAL         (0x0000u)


  /* CSL_DMA_CDAC */
#define CSL_DMA_CDAC_CDAC_MASK        (0xFFFFu)
#define CSL_DMA_CDAC_CDAC_SHIFT       (0x0000u)
#define CSL_DMA_CDAC_CDAC_RESETVAL    (0x0000u)

#define CSL_DMA_CDAC_RESETVAL         (0x0000u)


  /* CSL_DMA_CDFI */
#define CSL_DMA_CDFI_CDFI_MASK        (0xFFFFu)
#define CSL_DMA_CDFI_CDFI_SHIFT       (0x0000u)
#define CSL_DMA_CDFI_CDFI_RESETVAL    (0x0000u)

#define CSL_DMA_CDFI_RESETVAL         (0x0000u)


  /* CSL_DMA_CDEI */
#define CSL_DMA_CDEI_CDEI_MASK        (0xFFFFu)
#define CSL_DMA_CDEI_CDEI_SHIFT       (0x0000u)
#define CSL_DMA_CDEI_CDEI_RESETVAL    (0x0000u)

#define CSL_DMA_CDEI_RESETVAL         (0x0000u)


  /* CSL_DMA_GCR */
#define CSL_DMA_GCR_AUTOGATINGON_MASK (0x0008u)
#define CSL_DMA_GCR_AUTOGATINGON_SHIFT                                    \
          (0x0003u)
#define CSL_DMA_GCR_AUTOGATINGON_RESETVAL                                 \
          (0x0001u)

#define CSL_DMA_GCR_FREE_MASK         (0x0004u)
#define CSL_DMA_GCR_FREE_SHIFT        (0x0002u)
#define CSL_DMA_GCR_FREE_RESETVAL     (0x0000u)

#define CSL_DMA_GCR_APIEXCL_MASK      (0x0002u)
#define CSL_DMA_GCR_APIEXCL_SHIFT     (0x0001u)
#define CSL_DMA_GCR_APIEXCL_RESETVAL  (0x0000u)

#define CSL_DMA_GCR_APIPRIO_MASK      (0x0001u)
#define CSL_DMA_GCR_APIPRIO_SHIFT     (0x0000u)
#define CSL_DMA_GCR_APIPRIO_RESETVAL  (0x0000u)

#define CSL_DMA_GCR_RESETVAL          (0x0008u)


  /* CSL_DMA_GTCR */
#define CSL_DMA_GTCR_DARAMTCE_MASK    (0x0002u)
#define CSL_DMA_GTCR_DARAMTCE_SHIFT   (0x0001u)
#define CSL_DMA_GTCR_DARAMTCE_RESETVAL                                    \
          (0x0000u)

#define CSL_DMA_GTCR_SARAMTCE_MASK    (0x0001u)
#define CSL_DMA_GTCR_SARAMTCE_SHIFT   (0x0000u)
#define CSL_DMA_GTCR_SARAMTCE_RESETVAL                                    \
          (0x0000u)

#define CSL_DMA_GTCR_RESETVAL         (0x0000u)


  /* CSL_DMA_GSCR */
#define CSL_DMA_GSCR_INDEXMODE_MASK   (0x0001u)
#define CSL_DMA_GSCR_INDEXMODE_SHIFT  (0x0000u)
#define CSL_DMA_GSCR_INDEXMODE_RESETVAL                                   \
          (0x0000u)

#define CSL_DMA_GSCR_RESETVAL         (0x0000u)




#endif
