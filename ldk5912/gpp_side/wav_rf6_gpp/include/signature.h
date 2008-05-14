/** ============================================================================
 *  @file   signature.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines the file and object signatures used in DSP/BIOS Link
 *          These signatures are used in object validation and error reporting.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (SIGNATURE_H)
#define SIGNATURE_H


USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
 *  @const  File identifiers
 *
 *  @desc   File identifiers used in GEN_SetReason ()
 *  ============================================================================
 */
/*  ============================================================================
 *  API File identifiers
 *  ============================================================================
 */
#define FID_BASE_API            0x0200
#define FID_C_API_PROC          (FID_BASE_API  + 0)
#define FID_C_API_CHNL          (FID_BASE_API  + 1)
#define FID_C_API_MSGQ          (FID_BASE_API  + 2)
#define FID_C_API_POOL          (FID_BASE_API  + 3)

/*  ============================================================================
 *  PMGR File identifiers
 *  ============================================================================
 */
#define FID_BASE_PMGR           0x0300
#define FID_C_PMGR_PROC         (FID_BASE_PMGR + 0)
#define FID_C_PMGR_CHNL         (FID_BASE_PMGR + 1)
#define FID_C_PMGR_MSGQ         (FID_BASE_PMGR + 2)

/*  ============================================================================
 *  GEN File identifiers
 *  ============================================================================
 */
#define FID_BASE_GEN            0x0400
#define FID_C_GEN_UTILS         (FID_BASE_GEN  + 0)
#define FID_C_GEN_LIST          (FID_BASE_GEN  + 1)
#define FID_C_GEN_COFF          (FID_BASE_GEN  + 2)
#define FID_C_GEN_COFF_INT      (FID_BASE_GEN  + 3)
#define FID_C_GEN_COFF_55x      (FID_BASE_GEN  + 4)
#define FID_C_GEN_COFF_64x      (FID_BASE_GEN  + 5)
#define FID_C_GEN_MPCS          (FID_BASE_GEN  + 6)
#define FID_C_GEN_STATICLOADER  (FID_BASE_GEN  + 7)
#define FID_C_GEN_BINLOADER     (FID_BASE_GEN  + 8)

/*  ============================================================================
 *  OSAL File identifiers
 *  ============================================================================
 */
#define FID_BASE_OSAL           0x0500
#define FID_C_OSAL              (FID_BASE_OSAL + 0x0)
#define FID_C_OSAL_DPC          (FID_BASE_OSAL + 0x1)
#define FID_C_OSAL_ISR          (FID_BASE_OSAL + 0x2)
#define FID_C_OSAL_KFILE        (FID_BASE_OSAL + 0x3)
#define FID_C_OSAL_MEM          (FID_BASE_OSAL + 0x4)
#define FID_C_OSAL_PRCS         (FID_BASE_OSAL + 0x5)
#define FID_C_OSAL_SYNC         (FID_BASE_OSAL + 0x6)
#define FID_C_OSAL_TRC          (FID_BASE_OSAL + 0x7)
#define FID_C_OSAL_CFG          (FID_BASE_OSAL + 0x8)
#define FID_C_OSAL_PRINT        (FID_BASE_OSAL + 0x9)
#define FID_C_OSAL_DRV_PMGR     (FID_BASE_OSAL + 0xA)
#define FID_C_OSAL_DRV_POOL     (FID_BASE_OSAL + 0xB)
#define FID_C_OSAL_DRV_API      (FID_BASE_OSAL + 0xC)

/*  ============================================================================
 *  PROBE File identifiers
 *  ============================================================================
 */
#define FID_BASE_PROBE          0x0600
#define FID_C_PROBE_KNL         (FID_BASE_PROBE + 0x0)
#define FID_C_PROBE_USR         (FID_BASE_PROBE + 0x1)

/*  ============================================================================
 *  LDRV File identifiers
 *  ============================================================================
 */
#define FID_BASE_LDRV           0x0700
#define FID_C_LDRV              (FID_BASE_LDRV + 0x0)
#define FID_C_LDRV_PROC         (FID_BASE_LDRV + 0x1)
#define FID_C_LDRV_CHNL         (FID_BASE_LDRV + 0x2)
#define FID_C_LDRV_MSGQ         (FID_BASE_LDRV + 0x3)
#define FID_C_LDRV_CHIRPS       (FID_BASE_LDRV + 0x4)

/* LDRV DATA */
#define FID_C_LDRV_DATA         (FID_BASE_LDRV + 0x5)
#define FID_C_LDRV_DATA_PCPY    (FID_BASE_LDRV + 0x6)
#define FID_C_LDRV_DATA_ZCPY    (FID_BASE_LDRV + 0x7)

/* LDRV DRV */
#define FID_C_LDRV_DRV          (FID_BASE_LDRV + 0x8)
#define FID_C_LDRV_DRV_SHM      (FID_BASE_LDRV + 0x9)

/* LDRV DSP */
#define FID_C_LDRV_DSP          (FID_BASE_LDRV + 0xA)

/* LDRV HAL */
#define FID_C_LDRV_HAL_CLKRST   (FID_BASE_LDRV + 0xB)
#define FID_C_LDRV_HAL_IO       (FID_BASE_LDRV + 0xC)
#define FID_C_LDRV_HAL_MAILBOX  (FID_BASE_LDRV + 0xD)
#define FID_C_LDRV_HAL_MMU      (FID_BASE_LDRV + 0xE)
#define FID_C_LDRV_HAL_MPUI     (FID_BASE_LDRV + 0xF)
#define FID_C_LDRV_HAL_OBJECT   (FID_BASE_LDRV + 0x10)
#define FID_C_LDRV_HAL_RTC      (FID_BASE_LDRV + 0x11)
#define FID_C_LDRV_HAL_HPI      (FID_BASE_LDRV + 0x12)
#define FID_C_LDRV_HAL_INTGEN   (FID_BASE_LDRV + 0x13)
#define FID_C_LDRV_HAL_PCI      (FID_BASE_LDRV + 0x14)

/* LDRV IPS */
#define FID_C_LDRV_IPS_SHM      (FID_BASE_LDRV + 0x15)

/* LDRV MQT */
#define FID_C_LDRV_MQT_PCPY     (FID_BASE_LDRV + 0x16)
#define FID_C_LDRV_MQT_ZCPY     (FID_BASE_LDRV + 0x17)

/* LDRV POOL */
#define FID_C_LDRV_POOL         (FID_BASE_LDRV + 0x18)
#define FID_C_LDRV_POOL_BUF     (FID_BASE_LDRV + 0x19)
#define FID_C_LDRV_POOL_MPBUF   (FID_BASE_LDRV + 0x1A)
#define FID_C_LDRV_POOL_SMA     (FID_BASE_LDRV + 0x1B)


/*  ============================================================================
 *  @const  Object signatures
 *
 *  @desc   Object signatures used to validate objects.
 *  ============================================================================
 */
#define SIGN_NULL                0x00000000      /* NULL signature */

/*  Processor Manager            hex value       String (in reverse) */
#define SIGN_PROC                0x434F5250      /* PROC */
#define SIGN_CHNL                0x4C4E4843      /* CHNL */
#define SIGN_CODE                0x45444F43      /* CODE */
#define SIGN_PROC_SETUP          0x50535450      /* PSTP */

/*  Link Driver                  hex value       String (in reverse) */
#define SIGN_DSP                 0x5F505344      /* DSP_ */
#define SIGN_LDRV_DATA           0x5341444C      /* LDAT */
#define SIGN_LDRV_ISR            0x5253494C      /* LISR */
#define SIGN_LDRV_PROC           0x4352504C      /* LPRC */
#define SIGN_LDRV_CHNL           0x484E434C      /* LCHN */

/*  OS Adaptation Layer          hex value       String (in reverse) */
#define SIGN_CFG                 0x5F474643      /* CFG_ */
#define SIGN_TRC                 0x5F435254      /* TRC_ */
#define SIGN_DPC                 0x5F435044      /* DPC_ */
#define SIGN_ISR                 0x5F525349      /* ISR_ */
#define SIGN_KFILE               0x4C49464B      /* KFIL */
#define SIGN_MEM                 0x5F504550      /* MEM_ */
#define SIGN_PRCS                0x53435250      /* PRCS */
#define SIGN_SYNC                0x434E5953      /* SYNC */
#define SIGN_DRV                 0x5F4B5244      /* DRV_ */

/*  Generic components           hex value       String (in reverse) */
#define SIGN_GEN                 0x5F4E4547      /* GEN_ */
#define SIGN_LIST                0x5453494C      /* LIST */

/* HAL                           hex value       String (in reverse) */
#define SIGN_HAL                 0x484C414E      /* HAL_ */


/*  ============================================================================
 *  @const  MAXIMUM_COMPONENTS
 *
 *  @desc   Maximum number of components
 *  ============================================================================
 */
#define MAXIMUM_COMPONENTS         16

/*  ============================================================================
 *  @const  Component IDs
 *
 *  @desc   Component Identifiers. These must match corresponding bit
 *          position in component map
 *  ============================================================================
 */
#define ID_API_BASE        0x00010000
#define ID_PMGR_BASE       0x00020000
#define ID_GEN_BASE        0x00040000
#define ID_OSAL_BASE       0x00080000
#define ID_PROBE_BASE      0x00100000
#define ID_LDRV_BASE       0x00200000
#define ID_LDRV_DATA_BASE  0x00400000
#define ID_LDRV_DRV_BASE   0x00800000
#define ID_LDRV_DSP_BASE   0x01000000
#define ID_LDRV_HAL_BASE   0x02000000
#define ID_LDRV_IPS_BASE   0x04000000
#define ID_LDRV_MQT_BASE   0x08000000
#define ID_LDRV_POOL_BASE  0x10000000

#define MIN_COMPONENT  ID_API_BASE        >> 16
#define MAX_COMPONENT  ID_LDRV_POOL_BASE  >> 16

/*  ============================================================================
 *  API Subcomponent map
 *  ============================================================================
 */
#define ID_API_PROC        ID_API_BASE  | 0x0001
#define ID_API_CHNL        ID_API_BASE  | 0x0002
#define ID_API_MSGQ        ID_API_BASE  | 0x0004
#define ID_API_POOL        ID_API_BASE  | 0x0008
#define ID_API_ALL         ID_API_PROC | ID_API_CHNL | ID_API_MSGQ | ID_API_POOL

/*  ============================================================================
 *  PMGR Subcomponent map
 *  ============================================================================
 */
#define ID_PMGR_PROC       ID_PMGR_BASE | 0x0001
#define ID_PMGR_CHNL       ID_PMGR_BASE | 0x0002
#define ID_PMGR_MSGQ       ID_PMGR_BASE | 0x0004
#define ID_PMGR_ALL        ID_PMGR_PROC | ID_PMGR_CHNL | ID_PMGR_MSGQ

/*  ============================================================================
 *  GEN subcomponent map
 *  ============================================================================
 */
#define ID_GEN_UTILS        ID_GEN_BASE | 0x0001
#define ID_GEN_LIST         ID_GEN_BASE | 0x0002
#define ID_GEN_COFF         ID_GEN_BASE | 0x0004
#define ID_GEN_COFF_55x     ID_GEN_BASE | 0x0008
#define ID_GEN_COFF_64x     ID_GEN_BASE | 0x0010
#define ID_GEN_MPCS         ID_GEN_BASE | 0x0020
#define ID_GEN_STATICLOADER ID_GEN_BASE | 0x0040
#define ID_GEN_BINLOADER    ID_GEN_BASE | 0x0080
#define ID_GEN_ALL          ID_GEN_UTILS     | ID_GEN_LIST     | ID_GEN_COFF  \
                         |  ID_GEN_COFF_55x  | ID_GEN_COFF_64x | ID_GEN_MPCS  \
                         |  ID_GEN_STATICLOADER | ID_GEN_BINLOADER

/*  ============================================================================
 *  OSAL Subcomponent map
 *  ============================================================================
 */
#define ID_OSAL            ID_OSAL_BASE | 0x0001
#define ID_OSAL_DPC        ID_OSAL_BASE | 0x0002
#define ID_OSAL_ISR        ID_OSAL_BASE | 0x0004
#define ID_OSAL_KFILE      ID_OSAL_BASE | 0x0008
#define ID_OSAL_MEM        ID_OSAL_BASE | 0x0010
#define ID_OSAL_PRCS       ID_OSAL_BASE | 0x0020
#define ID_OSAL_SYNC       ID_OSAL_BASE | 0x0040
#define ID_OSAL_TRC        ID_OSAL_BASE | 0x0080
#define ID_OSAL_CFG        ID_OSAL_BASE | 0x0100
#define ID_OSAL_PRINT      ID_OSAL_BASE | 0x0200
#define ID_OSAL_DRV        ID_OSAL_BASE | 0x0400
#define ID_OSAL_ALL        ID_OSAL      | ID_OSAL_DPC  | ID_OSAL_ISR      \
                         | ID_OSAL_KFILE| ID_OSAL_MEM  | ID_OSAL_PRCS     \
                         | ID_OSAL_SYNC | ID_OSAL_TRC  | ID_OSAL_CFG      \
                         | ID_OSAL_PRINT| ID_OSAL_DRV

/*  ============================================================================
 *  PROBE subcomponent map
 *  ============================================================================
 */
#define ID_PROBE_KNL       ID_PROBE_BASE | 0x0001
#define ID_PROBE_USR       ID_PROBE_BASE | 0x0002
#define ID_PROBE_ALL       ID_PROBE_KNL | ID_PROBE_USR

/*  ============================================================================
 *  LDRV Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV            ID_LDRV_BASE | 0x0001
#define ID_LDRV_PROC       ID_LDRV_BASE | 0x0002
#define ID_LDRV_CHNL       ID_LDRV_BASE | 0x0004
#define ID_LDRV_MSGQ       ID_LDRV_BASE | 0x0008
#define ID_LDRV_CHIRPS     ID_LDRV_BASE | 0x0010
#define ID_LDRV_ALL        ID_LDRV      | ID_LDRV_PROC | ID_LDRV_CHNL    \
                         | ID_LDRV_MSGQ | ID_LDRV_CHIRPS

/*  ============================================================================
 *  LDRV_DATA Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_DATA       ID_LDRV_DATA_BASE | 0x0001
#define ID_LDRV_DATA_PCPY  ID_LDRV_DATA_BASE | 0x0002
#define ID_LDRV_DATA_ZCPY  ID_LDRV_DATA_BASE | 0x0004
#define ID_LDRV_DATA_ALL   ID_LDRV_DATA | ID_LDRV_DATA_PCPY    \
                         | ID_LDRV_DATA_ZCPY

/*  ============================================================================
 *  LDRV_DRV Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_DRV        ID_LDRV_DRV_BASE | 0x0001
#define ID_LDRV_DRV_SHM    ID_LDRV_DRV_BASE | 0x0002
#define ID_LDRV_DRV_ALL    ID_LDRV_DRV | ID_LDRV_DRV_SHM

/*  ============================================================================
 *  LDRV_DRV Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_DSP        ID_LDRV_DSP_BASE | 0x0001
#define ID_LDRV_DSP_ALL    ID_LDRV_DSP

/*  ============================================================================
 *  LDRV_HAL subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_HAL_CLKRST   ID_LDRV_HAL_BASE | 0x0001
#define ID_LDRV_HAL_IO       ID_LDRV_HAL_BASE | 0x0002
#define ID_LDRV_HAL_MAILBOX  ID_LDRV_HAL_BASE | 0x0004
#define ID_LDRV_HAL_MMU      ID_LDRV_HAL_BASE | 0x0008
#define ID_LDRV_HAL_MPUI     ID_LDRV_HAL_BASE | 0x0010
#define ID_LDRV_HAL_OBJECT   ID_LDRV_HAL_BASE | 0x0020
#define ID_LDRV_HAL_RTC      ID_LDRV_HAL_BASE | 0x0040
#define ID_LDRV_HAL_HPI      ID_LDRV_HAL_BASE | 0x0080
#define ID_LDRV_HAL_INTGEN   ID_LDRV_HAL_BASE | 0x0100
#define ID_LDRV_HAL_PCI      ID_LDRV_HAL_BASE | 0x0200
#define ID_LDRV_HAL_ALL      ID_LDRV_HAL_CLKRST  | ID_LDRV_HAL_IO          \
                           | ID_LDRV_HAL_MAILBOX | ID_LDRV_HAL_MMU         \
                           | ID_LDRV_HAL_MPUI    | ID_LDRV_HAL_OBJECT      \
                           | ID_LDRV_HAL_RTC     | ID_LDRV_HAL_HPI         \
                           | ID_LDRV_HAL_INTGEN  | ID_LDRV_HAL_PCI

/*  ============================================================================
 *  LDRV_IPS Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_IPS_SHM    ID_LDRV_IPS_BASE | 0x0001
#define ID_LDRV_IPS_ALL    ID_LDRV_IPS_SHM

/*  ============================================================================
 *  LDRV_MQT Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_MQT_PCPY   ID_LDRV_MQT_BASE | 0x0001
#define ID_LDRV_MQT_ZCPY   ID_LDRV_MQT_BASE | 0x0002
#define ID_LDRV_MQT_ALL    ID_LDRV_MQT_PCPY | ID_LDRV_MQT_ZCPY

/*  ============================================================================
 *  LDRV_POOL Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_POOL       ID_LDRV_POOL_BASE | 0x0001
#define ID_LDRV_POOL_BUF   ID_LDRV_POOL_BASE | 0x0002
#define ID_LDRV_POOL_MPBUF ID_LDRV_POOL_BASE | 0x0004
#define ID_LDRV_POOL_SMA   ID_LDRV_POOL_BASE | 0x0008
#define ID_LDRV_POOL_ALL   ID_LDRV_POOL       | ID_LDRV_POOL_BUF       \
                         | ID_LDRV_POOL_MPBUF | ID_LDRV_POOL_SMA


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (SIGNATURE_H) */
