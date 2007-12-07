/** ============================================================================
 *  @file   signature.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Defines the file and object signatures used in DSP/BIOS Link
 *          These signatures are used in object validation and error reporting.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
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
/* API */
#define FID_BASE_API            0x0200
#define FID_C_API_PROC          (FID_BASE_API  + 0)
#define FID_C_API_CHNL          (FID_BASE_API  + 1)
#define FID_C_API_MSGQ          (FID_BASE_API  + 2)

/* Platform Manager */
#define FID_BASE_PMGR           0x0300
#define FID_C_PMGR_PROC         (FID_BASE_PMGR + 0)
#define FID_C_PMGR_CHNL         (FID_BASE_PMGR + 1)
#define FID_C_PMGR_PMGR         (FID_BASE_PMGR + 2)
#define FID_C_PMGR_MSGQ         (FID_BASE_PMGR + 5)

/* Link Driver */
#define FID_BASE_LDRV           0x0400
#define FID_C_LDRV_CHNL         (FID_BASE_LDRV + 0)
#define FID_C_LDRV_PROC         (FID_BASE_LDRV + 1)
#define FID_C_LDRV_IO           (FID_BASE_LDRV + 2)
#define FID_C_LDRV_ISR          (FID_BASE_LDRV + 3)
#define FID_C_DSP               (FID_BASE_LDRV + 4)
#define FID_C_LDRV_LDRV         (FID_BASE_LDRV + 5)
#define FID_C_LDRV_SHM          (FID_BASE_LDRV + 6)
#define FID_C_LDRV_MSGQ         (FID_BASE_LDRV + 7)
#define FID_C_LDRV_MQABUF       (FID_BASE_LDRV + 8)
#define FID_C_LDRV_LMQT         (FID_BASE_LDRV + 9)
#define FID_C_LDRV_RMQT         (FID_BASE_LDRV + 10)
#define FID_C_LDRV_HPI_DRV      (FID_BASE_LDRV + 11)

/* OS Adaptation Layer */
#define FID_BASE_OSAL           0x0500
#define FID_C_OSAL_OSAL         (FID_BASE_OSAL + 0)
#define FID_C_OSAL_CFG          (FID_BASE_OSAL + 1)
#define FID_C_OSAL_TRC          (FID_BASE_OSAL + 2)
#define FID_C_OSAL_DPC          (FID_BASE_OSAL + 3)
#define FID_C_OSAL_ISR          (FID_BASE_OSAL + 4)
#define FID_C_OSAL_KFILE        (FID_BASE_OSAL + 5)
#define FID_C_OSAL_MEM          (FID_BASE_OSAL + 6)
#define FID_C_OSAL_PRCS         (FID_BASE_OSAL + 7)
#define FID_C_OSAL_SYNC         (FID_BASE_OSAL + 8)
#define FID_C_OSAL_DRV_PMGR     (FID_BASE_OSAL + 9)
#define FID_C_OSAL_DRV_API      (FID_BASE_OSAL + 10)

/* Generic components */
#define FID_BASE_GEN            0x0600
#define FID_C_GEN_GEN           (FID_BASE_GEN  + 0)
#define FID_C_GEN_LIST          (FID_BASE_GEN  + 1)
#define FID_C_GEN_COFF          (FID_BASE_GEN  + 2)
#define FID_C_GEN_COFF_55x      (FID_BASE_GEN  + 3)
#define FID_C_GEN_COFF_64x      (FID_BASE_GEN  + 4)
#define FID_C_GEN_BUF           (FID_BASE_GEN  + 5)

/* HAL components */
#define FID_BASE_HAL            0x0700
#define FID_C_HAL_API           (FID_BASE_HAL  + 0)
#define FID_C_HAL_CLKRST        (FID_BASE_HAL  + 1)
#define FID_C_HAL_IO            (FID_BASE_HAL  + 2)
#define FID_C_HAL_MAILBOX       (FID_BASE_HAL  + 3)
#define FID_C_HAL_MMU           (FID_BASE_HAL  + 4)
#define FID_C_HAL_OBJECT        (FID_BASE_HAL  + 5)
#define FID_C_HAL_HPI           (FID_BASE_HAL  + 6)

/*  ============================================================================
 *  @const  Object signatures
 *
 *  @desc   Object signatures used to validate objects.
 *  ============================================================================
 */
#define SIGN_NULL                0x00000000      /* NULL signature */

/*  Processor Manager            hex value       String (in reverse) */
#define SIGN_CHNL                0x4C4E4843      /* CHNL */
#define SIGN_PROC                0x434F5250      /* PROC */
#define SIGN_CODE                0x45444F43      /* CODE */
#define SIGN_PROC_SETUP          0x50535450      /* PSTP */

/*  Link Driver                  hex value       String (in reverse) */
#define SIGN_DSP                 0x5F505344      /* DSP_ */
#define SIGN_LDRV_IO             0x5F4F494C      /* LIO_ */
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
#define ID_API       0x00010000
#define ID_PMGR      0x00020000
#define ID_LDRV      0x00040000
#define ID_GEN       0x00080000
#define ID_OSAL      0x00100000
#define ID_HAL       0x00200000

#define MIN_COMPONENT  ID_API >> 16
#define MAX_COMPONENT  ID_HAL >> 16

/*  ============================================================================
 *  API Subcomponent map
 *  ============================================================================
 */
#define ID_API_PROC     ID_API  | 0x0001
#define ID_API_CHNL     ID_API  | 0x0002
#define ID_API_MSGQ     ID_API  | 0x0003
#define ID_API_ALL      ID_API_PROC | ID_API_CHNL | ID_API_MSGQ

/*  ============================================================================
 *  PMGR Subcomponent map
 *  ============================================================================
 */
#define ID_PMGR_CHNL    ID_PMGR | 0x0001
#define ID_PMGR_PROC    ID_PMGR | 0x0002
#define ID_PMGR_MSGQ    ID_PMGR | 0x0004
#define ID_PMGR_ALL     ID_PMGR_CHNL | ID_PMGR_PROC | ID_PMGR_MSGQ

/*  ============================================================================
 *  LDRV Subcomponent map
 *  ============================================================================
 */
#define ID_LDRV_DSP     ID_LDRV | 0x0001
#define ID_LDRV_IO      ID_LDRV | 0x0002
#define ID_LDRV_ISR     ID_LDRV | 0x0004
#define ID_LDRV_CHNL    ID_LDRV | 0x0008
#define ID_LDRV_PROC    ID_LDRV | 0x0010
#define ID_LDRV_LDRV    ID_LDRV | 0x0020
#define ID_LDRV_SHM     ID_LDRV | 0x0040
#define ID_LDRV_MSGQ    ID_LDRV | 0x0080
#define ID_LDRV_MQABUF  ID_LDRV | 0x0100
#define ID_LDRV_LMQT    ID_LDRV | 0x0200
#define ID_LDRV_RMQT    ID_LDRV | 0x0400
#define ID_LDRV_HPI_DRV ID_LDRV | 0x0800
#define ID_LDRV_ALL     ID_LDRV_DSP | ID_LDRV_IO | ID_LDRV_CHNL | ID_LDRV_PROC \
                      | ID_LDRV_LDRV | ID_LDRV_SHM | ID_LDRV_MSGQ              \
                      | ID_LDRV_MQABUF | ID_LDRV_LMQT | ID_LDRV_RMQT           \
                      | ID_LDRV_HPI_DRV

/*  ============================================================================
 *  Gen subcomponent map
 *  ============================================================================
 */
#define ID_GEN_GEN        ID_GEN | 0x0001
#define ID_GEN_LST        ID_GEN | 0x0002
#define ID_GEN_COFF       ID_GEN | 0x0004
#define ID_GEN_COFF_55x   ID_GEN | 0x0008
#define ID_GEN_COFF_64x   ID_GEN | 0x0010
#define ID_GEN_BUF        ID_GEN | 0x0020
#define ID_GEN_ALL        ID_GEN_GEN | ID_GEN_LST | ID_GEN_COFF               \
                        | ID_GEN_COFF_55x | ID_GEN_COFF_64x | ID_GEN_BUF

/*  ============================================================================
 *  OSAL Subcomponent map
 *  ============================================================================
 */
#define ID_OSAL_DPC       ID_OSAL | 0x0001
#define ID_OSAL_ISR       ID_OSAL | 0x0002
#define ID_OSAL_KFILE     ID_OSAL | 0x0004
#define ID_OSAL_MEM       ID_OSAL | 0x0008
#define ID_OSAL_PRCS      ID_OSAL | 0x0010
#define ID_OSAL_SYNC      ID_OSAL | 0x0020
#define ID_OSAL_TRC       ID_OSAL | 0x0040
#define ID_OSAL_OSAL      ID_OSAL | 0x0080
#define ID_OSAL_CFG       ID_OSAL | 0x0100
#define ID_OSAL_DRIVER    ID_OSAL | 0x0200
#define ID_OSAL_ALL       ID_OSAL_DPC  | ID_OSAL_ISR  | ID_OSAL_KFILE    \
                        | ID_OSAL_MEM  | ID_OSAL_PRCS | ID_OSAL_SYNC     \
                        | ID_OSAL_TRC  | ID_OSAL_OSAL | ID_OSAL_CFG      \
                        | ID_OSAL_DRIVER

/*  ============================================================================
 *  HAL subcomponent map
 *  ============================================================================
 */
#define ID_HAL_API      ID_HAL | 0x0001
#define ID_HAL_CLKRST   ID_HAL | 0x0002
#define ID_HAL_IO       ID_HAL | 0x0004
#define ID_HAL_MAILBOX  ID_HAL | 0x0008
#define ID_HAL_MMU      ID_HAL | 0x0010
#define ID_HAL_OBJECT   ID_HAL | 0x0020
#define ID_HAL_HPI      ID_HAL | 0x0040
#define ID_HAL_ALL      ID_HAL_API     | ID_HAL_CLKRST | ID_HAL_IO          \
                      | ID_HAL_MAILBOX | ID_HAL_MMU    | ID_HAL_OBJECT      \
                      | ID_HAL_HPI

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif  /* !defined (SIGNATURE_H) */
