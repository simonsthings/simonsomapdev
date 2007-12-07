#ifndef _CSLR_I2C_002_H_
#define _CSLR_I2C_002_H_

#include <cslr.h>

#include <tistdtypes.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 REV;
    const char RSVD0[1];
    volatile Uint16 IE;
    const char RSVD1[1];
    volatile Uint16 STAT;
    const char RSVD2[3];
    volatile Uint16 SYSS;
    const char RSVD3[1];
    volatile Uint16 BUF;
    const char RSVD4[1];
    volatile Uint16 CNT;
    const char RSVD5[1];
    volatile Uint16 DATA;
    const char RSVD6[1];
    volatile Uint16 SYSC;
    const char RSVD7[1];
    volatile Uint16 CON;
    const char RSVD8[1];
    volatile Uint16 OA;
    const char RSVD9[1];
    volatile Uint16 SA;
    const char RSVD10[1];
    volatile Uint16 PSC;
    const char RSVD11[1];
    volatile Uint16 SCLL;
    const char RSVD12[1];
    volatile Uint16 SCLH;
    const char RSVD13[1];
    volatile Uint16 SYSTEST;
} CSL_I2cRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* REV */

#define CSL_I2C_REV_REV_MASK             (0x000000FFu)
#define CSL_I2C_REV_REV_SHIFT            (0x00000000u)
#define CSL_I2C_REV_REV_RESETVAL         (0x00000022u)

#define CSL_I2C_REV_RESETVAL             (0x00000022u)

/* IE */

#define CSL_I2C_IE_GC_IE_MASK            (0x00000020u)
#define CSL_I2C_IE_GC_IE_SHIFT           (0x00000005u)
#define CSL_I2C_IE_GC_IE_RESETVAL        (0x00000000u)

#define CSL_I2C_IE_XRDY_IE_MASK          (0x00000010u)
#define CSL_I2C_IE_XRDY_IE_SHIFT         (0x00000004u)
#define CSL_I2C_IE_XRDY_IE_RESETVAL      (0x00000000u)

#define CSL_I2C_IE_RRDY_IE_MASK          (0x00000008u)
#define CSL_I2C_IE_RRDY_IE_SHIFT         (0x00000003u)
#define CSL_I2C_IE_RRDY_IE_RESETVAL      (0x00000000u)

#define CSL_I2C_IE_ARDY_IE_MASK          (0x00000004u)
#define CSL_I2C_IE_ARDY_IE_SHIFT         (0x00000002u)
#define CSL_I2C_IE_ARDY_IE_RESETVAL      (0x00000000u)

#define CSL_I2C_IE_NACK_IE_MASK          (0x00000002u)
#define CSL_I2C_IE_NACK_IE_SHIFT         (0x00000001u)
#define CSL_I2C_IE_NACK_IE_RESETVAL      (0x00000000u)

#define CSL_I2C_IE_AL_IE_MASK            (0x00000001u)
#define CSL_I2C_IE_AL_IE_SHIFT           (0x00000000u)
#define CSL_I2C_IE_AL_IE_RESETVAL        (0x00000000u)

#define CSL_I2C_IE_RESETVAL              (0x00000000u)

/* STAT */

#define CSL_I2C_STAT_SBD_MASK            (0x00008000u)
#define CSL_I2C_STAT_SBD_SHIFT           (0x0000000Fu)
#define CSL_I2C_STAT_SBD_RESETVAL        (0x00000000u)

#define CSL_I2C_STAT_BB_MASK             (0x00001000u)
#define CSL_I2C_STAT_BB_SHIFT            (0x0000000Cu)
#define CSL_I2C_STAT_BB_RESETVAL         (0x00000000u)

#define CSL_I2C_STAT_ROVR_MASK           (0x00000800u)
#define CSL_I2C_STAT_ROVR_SHIFT          (0x0000000Bu)
#define CSL_I2C_STAT_ROVR_RESETVAL       (0x00000000u)

#define CSL_I2C_STAT_XUDF_MASK           (0x00000400u)
#define CSL_I2C_STAT_XUDF_SHIFT          (0x0000000Au)
#define CSL_I2C_STAT_XUDF_RESETVAL       (0x00000000u)

#define CSL_I2C_STAT_AAS_MASK            (0x00000200u)
#define CSL_I2C_STAT_AAS_SHIFT           (0x00000009u)
#define CSL_I2C_STAT_AAS_RESETVAL        (0x00000000u)

#define CSL_I2C_STAT_GC_MASK             (0x00000020u)
#define CSL_I2C_STAT_GC_SHIFT            (0x00000005u)
#define CSL_I2C_STAT_GC_RESETVAL         (0x00000000u)

#define CSL_I2C_STAT_XRDY_MASK           (0x00000010u)
#define CSL_I2C_STAT_XRDY_SHIFT          (0x00000004u)
#define CSL_I2C_STAT_XRDY_RESETVAL       (0x00000000u)

#define CSL_I2C_STAT_RRDY_MASK           (0x00000008u)
#define CSL_I2C_STAT_RRDY_SHIFT          (0x00000003u)
#define CSL_I2C_STAT_RRDY_RESETVAL       (0x00000000u)

#define CSL_I2C_STAT_ARDY_MASK           (0x00000004u)
#define CSL_I2C_STAT_ARDY_SHIFT          (0x00000002u)
#define CSL_I2C_STAT_ARDY_RESETVAL       (0x00000000u)

#define CSL_I2C_STAT_NACK_MASK           (0x00000002u)
#define CSL_I2C_STAT_NACK_SHIFT          (0x00000001u)
#define CSL_I2C_STAT_NACK_RESETVAL       (0x00000000u)

#define CSL_I2C_STAT_AL_MASK             (0x00000001u)
#define CSL_I2C_STAT_AL_SHIFT            (0x00000000u)
#define CSL_I2C_STAT_AL_RESETVAL         (0x00000000u)

#define CSL_I2C_STAT_RESETVAL            (0x00000000u)

/* SYSS */

#define CSL_I2C_SYSS_RDONE_MASK          (0x00000001u)
#define CSL_I2C_SYSS_RDONE_SHIFT         (0x00000000u)
#define CSL_I2C_SYSS_RDONE_RESETVAL      (0x00000000u)

#define CSL_I2C_SYSS_RESETVAL            (0x00000000u)

/* BUF */

#define CSL_I2C_BUF_RDMA_EN_MASK         (0x00008000u)
#define CSL_I2C_BUF_RDMA_EN_SHIFT        (0x0000000Fu)
#define CSL_I2C_BUF_RDMA_EN_RESETVAL     (0x00000000u)

#define CSL_I2C_BUF_XDMA_EN_MASK         (0x00000080u)
#define CSL_I2C_BUF_XDMA_EN_SHIFT        (0x00000007u)
#define CSL_I2C_BUF_XDMA_EN_RESETVAL     (0x00000000u)

#define CSL_I2C_BUF_RESETVAL             (0x00000000u)

/* CNT */

#define CSL_I2C_CNT_DCOUNT_MASK          (0x0000FFFFu)
#define CSL_I2C_CNT_DCOUNT_SHIFT         (0x00000000u)
#define CSL_I2C_CNT_DCOUNT_RESETVAL      (0x00000000u)

#define CSL_I2C_CNT_RESETVAL             (0x00000000u)

/* DATA */

#define CSL_I2C_DATA_DATA_MASK           (0x0000FFFFu)
#define CSL_I2C_DATA_DATA_SHIFT          (0x00000000u)
#define CSL_I2C_DATA_DATA_RESETVAL       (0x00000000u)

#define CSL_I2C_DATA_RESETVAL            (0x00000000u)

/* SYSC */

#define CSL_I2C_SYSC_SRST_MASK           (0x00000002u)
#define CSL_I2C_SYSC_SRST_SHIFT          (0x00000001u)
#define CSL_I2C_SYSC_SRST_RESETVAL       (0x00000000u)

#define CSL_I2C_SYSC_RESETVAL            (0x00000000u)

/* CON */

#define CSL_I2C_CON_I2C_EN_MASK          (0x00008000u)
#define CSL_I2C_CON_I2C_EN_SHIFT         (0x0000000Fu)
#define CSL_I2C_CON_I2C_EN_RESETVAL      (0x00000000u)

#define CSL_I2C_CON_BE_MASK              (0x00004000u)
#define CSL_I2C_CON_BE_SHIFT             (0x0000000Eu)
#define CSL_I2C_CON_BE_RESETVAL          (0x00000000u)

#define CSL_I2C_CON_STB_MASK             (0x00000800u)
#define CSL_I2C_CON_STB_SHIFT            (0x0000000Bu)
#define CSL_I2C_CON_STB_RESETVAL         (0x00000000u)

#define CSL_I2C_CON_MST_MASK             (0x00000400u)
#define CSL_I2C_CON_MST_SHIFT            (0x0000000Au)
#define CSL_I2C_CON_MST_RESETVAL         (0x00000000u)

#define CSL_I2C_CON_TRX_MASK             (0x00000200u)
#define CSL_I2C_CON_TRX_SHIFT            (0x00000009u)
#define CSL_I2C_CON_TRX_RESETVAL         (0x00000000u)

#define CSL_I2C_CON_XA_MASK              (0x00000100u)
#define CSL_I2C_CON_XA_SHIFT             (0x00000008u)
#define CSL_I2C_CON_XA_RESETVAL          (0x00000000u)

#define CSL_I2C_CON_STP_MASK             (0x00000002u)
#define CSL_I2C_CON_STP_SHIFT            (0x00000001u)
#define CSL_I2C_CON_STP_RESETVAL         (0x00000000u)

#define CSL_I2C_CON_STT_MASK             (0x00000001u)
#define CSL_I2C_CON_STT_SHIFT            (0x00000000u)
#define CSL_I2C_CON_STT_RESETVAL         (0x00000000u)

#define CSL_I2C_CON_RESETVAL             (0x00000000u)

/* OA */

#define CSL_I2C_OA_OA_MASK               (0x000003FFu)
#define CSL_I2C_OA_OA_SHIFT              (0x00000000u)
#define CSL_I2C_OA_OA_RESETVAL           (0x00000000u)

#define CSL_I2C_OA_RESETVAL              (0x00000000u)

/* SA */

#define CSL_I2C_SA_SA_MASK               (0x000003FFu)
#define CSL_I2C_SA_SA_SHIFT              (0x00000000u)
#define CSL_I2C_SA_SA_RESETVAL           (0x000003FFu)

#define CSL_I2C_SA_RESETVAL              (0x000003FFu)

/* PSC */

#define CSL_I2C_PSC_PSC_MASK             (0x000000FFu)
#define CSL_I2C_PSC_PSC_SHIFT            (0x00000000u)
#define CSL_I2C_PSC_PSC_RESETVAL         (0x00000000u)

#define CSL_I2C_PSC_RESETVAL             (0x00000000u)

/* SCLL */

#define CSL_I2C_SCLL_SCLL_MASK           (0x000000FFu)
#define CSL_I2C_SCLL_SCLL_SHIFT          (0x00000000u)
#define CSL_I2C_SCLL_SCLL_RESETVAL       (0x00000000u)

#define CSL_I2C_SCLL_RESETVAL            (0x00000000u)

/* SCLH */

#define CSL_I2C_SCLH_SCLH_MASK           (0x000000FFu)
#define CSL_I2C_SCLH_SCLH_SHIFT          (0x00000000u)
#define CSL_I2C_SCLH_SCLH_RESETVAL       (0x00000000u)

#define CSL_I2C_SCLH_RESETVAL            (0x00000000u)

/* SYSTEST */

#define CSL_I2C_SYSTEST_ST_EN_MASK       (0x00008000u)
#define CSL_I2C_SYSTEST_ST_EN_SHIFT      (0x0000000Fu)
#define CSL_I2C_SYSTEST_ST_EN_RESETVAL   (0x00000000u)

#define CSL_I2C_SYSTEST_FREE_MASK        (0x00004000u)
#define CSL_I2C_SYSTEST_FREE_SHIFT       (0x0000000Eu)
#define CSL_I2C_SYSTEST_FREE_RESETVAL    (0x00000000u)

#define CSL_I2C_SYSTEST_TMODE_MASK       (0x00003000u)
#define CSL_I2C_SYSTEST_TMODE_SHIFT      (0x0000000Cu)
#define CSL_I2C_SYSTEST_TMODE_RESETVAL   (0x00000000u)

#define CSL_I2C_SYSTEST_SSB_MASK         (0x00000800u)
#define CSL_I2C_SYSTEST_SSB_SHIFT        (0x0000000Bu)
#define CSL_I2C_SYSTEST_SSB_RESETVAL     (0x00000000u)

#define CSL_I2C_SYSTEST_SCL_I_MASK       (0x00000008u)
#define CSL_I2C_SYSTEST_SCL_I_SHIFT      (0x00000003u)
#define CSL_I2C_SYSTEST_SCL_I_RESETVAL   (0x00000000u)

#define CSL_I2C_SYSTEST_SCL_O_MASK       (0x00000004u)
#define CSL_I2C_SYSTEST_SCL_O_SHIFT      (0x00000002u)
#define CSL_I2C_SYSTEST_SCL_O_RESETVAL   (0x00000000u)

#define CSL_I2C_SYSTEST_SDA_I_MASK       (0x00000002u)
#define CSL_I2C_SYSTEST_SDA_I_SHIFT      (0x00000001u)
#define CSL_I2C_SYSTEST_SDA_I_RESETVAL   (0x00000000u)

#define CSL_I2C_SYSTEST_SDA_O_MASK       (0x00000001u)
#define CSL_I2C_SYSTEST_SDA_O_SHIFT      (0x00000000u)
#define CSL_I2C_SYSTEST_SDA_O_RESETVAL   (0x00000000u)

#define CSL_I2C_SYSTEST_RESETVAL        (0x00000000u)

#endif
