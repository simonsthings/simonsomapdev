/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5912PG1_0] (2003-10-15)  */

#ifndef _CSLR_MCBSP_001_H_
#define _CSLR_MCBSP_001_H_

#include <cslr.h>
#include <csl_types.h>

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/


typedef struct  {
    volatile Uint16 DRR2;
    volatile Uint16 DRR1;
    volatile Uint16 DXR2;
    volatile Uint16 DXR1;
    volatile Uint16 SPCR2;
    volatile Uint16 SPCR1;
    volatile Uint16 RCR2;
    volatile Uint16 RCR1;
    volatile Uint16 XCR2;
    volatile Uint16 XCR1;
    volatile Uint16 SRGR2;
    volatile Uint16 SRGR1;
    volatile Uint16 MCR2;
    volatile Uint16 MCR1;
    volatile Uint16 RCERA;
    volatile Uint16 RCERB;
    volatile Uint16 XCERA;
    volatile Uint16 XCERB;
    volatile Uint16 PCR;
    volatile Uint16 RCERC;
    volatile Uint16 RCERD;
    volatile Uint16 XCERC;
    volatile Uint16 XCERD;
    volatile Uint16 RCERE;
    volatile Uint16 RCERF;
    volatile Uint16 XCERE;
    volatile Uint16 XCERF;
    volatile Uint16 RCERG;
    volatile Uint16 RCERH;
    volatile Uint16 XCERG;
    volatile Uint16 XCERH;
} CSL_McbspRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/
  /* CSL_MCBSP_DRR2 */
#define CSL_MCBSP_DRR2_DRR2_MASK      (0xFFFFu)
#define CSL_MCBSP_DRR2_DRR2_SHIFT     (0x0000u)
#define CSL_MCBSP_DRR2_DRR2_RESETVAL  (0x0000u)

#define CSL_MCBSP_DRR2_RESETVAL       (0x0000u)


  /* CSL_MCBSP_DRR1 */
#define CSL_MCBSP_DRR1_DRR1_MASK      (0xFFFFu)
#define CSL_MCBSP_DRR1_DRR1_SHIFT     (0x0000u)
#define CSL_MCBSP_DRR1_DRR1_RESETVAL  (0x0000u)

#define CSL_MCBSP_DRR1_RESETVAL       (0x0000u)


  /* CSL_MCBSP_DXR2 */
#define CSL_MCBSP_DXR2_DXR2_MASK      (0xFFFFu)
#define CSL_MCBSP_DXR2_DXR2_SHIFT     (0x0000u)
#define CSL_MCBSP_DXR2_DXR2_RESETVAL  (0x0000u)

#define CSL_MCBSP_DXR2_RESETVAL       (0x0000u)


  /* CSL_MCBSP_DXR1 */
#define CSL_MCBSP_DXR1_DXR1_MASK      (0xFFFFu)
#define CSL_MCBSP_DXR1_DXR1_SHIFT     (0x0000u)
#define CSL_MCBSP_DXR1_DXR1_RESETVAL  (0x0000u)

#define CSL_MCBSP_DXR1_RESETVAL       (0x0000u)


  /* CSL_MCBSP_SPCR2 */
#define CSL_MCBSP_SPCR2_FREE_MASK     (0x0200u)
#define CSL_MCBSP_SPCR2_FREE_SHIFT    (0x0009u)
#define CSL_MCBSP_SPCR2_FREE_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR2_SOFT_MASK     (0x0100u)
#define CSL_MCBSP_SPCR2_SOFT_SHIFT    (0x0008u)
#define CSL_MCBSP_SPCR2_SOFT_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR2_FRST_MASK     (0x0080u)
#define CSL_MCBSP_SPCR2_FRST_SHIFT    (0x0007u)
#define CSL_MCBSP_SPCR2_FRST_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR2_GRST_MASK     (0x0040u)
#define CSL_MCBSP_SPCR2_GRST_SHIFT    (0x0006u)
#define CSL_MCBSP_SPCR2_GRST_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR2_XINTM_MASK    (0x0030u)
#define CSL_MCBSP_SPCR2_XINTM_SHIFT   (0x0004u)
#define CSL_MCBSP_SPCR2_XINTM_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_SPCR2_XSYNCERR_MASK (0x0008u)
#define CSL_MCBSP_SPCR2_XSYNCERR_SHIFT                                    \
          (0x0003u)
#define CSL_MCBSP_SPCR2_XSYNCERR_RESETVAL                                 \
          (0x0000u)
#define CSL_MCBSP_SPCR2_XSYNCERR_CLEAR                                    \
          (0x0000u)

#define CSL_MCBSP_SPCR2_XEMPTY_MASK   (0x0004u)
#define CSL_MCBSP_SPCR2_XEMPTY_SHIFT  (0x0002u)
#define CSL_MCBSP_SPCR2_XEMPTY_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_SPCR2_XRDY_MASK     (0x0002u)
#define CSL_MCBSP_SPCR2_XRDY_SHIFT    (0x0001u)
#define CSL_MCBSP_SPCR2_XRDY_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR2_XRST_MASK     (0x0001u)
#define CSL_MCBSP_SPCR2_XRST_SHIFT    (0x0000u)
#define CSL_MCBSP_SPCR2_XRST_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR2_RESETVAL      (0x0000u)


  /* CSL_MCBSP_SPCR1 */
#define CSL_MCBSP_SPCR1_DLB_MASK      (0x8000u)
#define CSL_MCBSP_SPCR1_DLB_SHIFT     (0x000Fu)
#define CSL_MCBSP_SPCR1_DLB_RESETVAL  (0x0000u)

#define CSL_MCBSP_SPCR1_RJUST_MASK    (0x6000u)
#define CSL_MCBSP_SPCR1_RJUST_SHIFT   (0x000Du)
#define CSL_MCBSP_SPCR1_RJUST_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_SPCR1_CLKSTP_MASK   (0x1800u)
#define CSL_MCBSP_SPCR1_CLKSTP_SHIFT  (0x000Bu)
#define CSL_MCBSP_SPCR1_CLKSTP_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_SPCR1_DXENA_MASK    (0x0080u)
#define CSL_MCBSP_SPCR1_DXENA_SHIFT   (0x0007u)
#define CSL_MCBSP_SPCR1_DXENA_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_SPCR1_RINTM_MASK    (0x0030u)
#define CSL_MCBSP_SPCR1_RINTM_SHIFT   (0x0004u)
#define CSL_MCBSP_SPCR1_RINTM_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_SPCR1_RSYNCERR_MASK (0x0008u)
#define CSL_MCBSP_SPCR1_RSYNCERR_SHIFT                                    \
          (0x0003u)
#define CSL_MCBSP_SPCR1_RSYNCERR_RESETVAL                                 \
          (0x0000u)
#define CSL_MCBSP_SPCR1_RSYNCERR_CLEAR                                    \
          (0x0000u)

#define CSL_MCBSP_SPCR1_RFULL_MASK    (0x0004u)
#define CSL_MCBSP_SPCR1_RFULL_SHIFT   (0x0002u)
#define CSL_MCBSP_SPCR1_RFULL_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_SPCR1_RRDY_MASK     (0x0002u)
#define CSL_MCBSP_SPCR1_RRDY_SHIFT    (0x0001u)
#define CSL_MCBSP_SPCR1_RRDY_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR1_RRST_MASK     (0x0001u)
#define CSL_MCBSP_SPCR1_RRST_SHIFT    (0x0000u)
#define CSL_MCBSP_SPCR1_RRST_RESETVAL (0x0000u)

#define CSL_MCBSP_SPCR1_RESETVAL      (0x0000u)


  /* CSL_MCBSP_RCR2 */
#define CSL_MCBSP_RCR2_RPHASE_MASK    (0x8000u)
#define CSL_MCBSP_RCR2_RPHASE_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCR2_RPHASE_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCR2_RFRLEN2_MASK   (0x7F00u)
#define CSL_MCBSP_RCR2_RFRLEN2_SHIFT  (0x0008u)
#define CSL_MCBSP_RCR2_RFRLEN2_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCR2_RWDLEN2_MASK   (0x00E0u)
#define CSL_MCBSP_RCR2_RWDLEN2_SHIFT  (0x0005u)
#define CSL_MCBSP_RCR2_RWDLEN2_RESETVAL                                   \
          (0x0000u)
#define CSL_MCBSP_RCR2_RWDLEN2_8BITS  (0x0000u)
#define CSL_MCBSP_RCR2_RWDLEN2_12BITS (0x0001u)
#define CSL_MCBSP_RCR2_RWDLEN2_16BITS (0x0002u)
#define CSL_MCBSP_RCR2_RWDLEN2_20BITS (0x0003u)
#define CSL_MCBSP_RCR2_RWDLEN2_24BITS (0x0004u)
#define CSL_MCBSP_RCR2_RWDLEN2_32BITS (0x0005u)

#define CSL_MCBSP_RCR2_RCOMPAND_MASK  (0x0018u)
#define CSL_MCBSP_RCR2_RCOMPAND_SHIFT (0x0003u)
#define CSL_MCBSP_RCR2_RCOMPAND_RESETVAL                                  \
          (0x0000u)

#define CSL_MCBSP_RCR2_RFIG_MASK      (0x0004u)
#define CSL_MCBSP_RCR2_RFIG_SHIFT     (0x0002u)
#define CSL_MCBSP_RCR2_RFIG_RESETVAL  (0x0000u)

#define CSL_MCBSP_RCR2_RDATDLY_MASK   (0x0003u)
#define CSL_MCBSP_RCR2_RDATDLY_SHIFT  (0x0000u)
#define CSL_MCBSP_RCR2_RDATDLY_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCR2_RESETVAL       (0x0000u)


  /* CSL_MCBSP_RCR1 */
#define CSL_MCBSP_RCR1_RFRLEN1_MASK   (0x7F00u)
#define CSL_MCBSP_RCR1_RFRLEN1_SHIFT  (0x0008u)
#define CSL_MCBSP_RCR1_RFRLEN1_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCR1_RWDLEN1_MASK   (0x00E0u)
#define CSL_MCBSP_RCR1_RWDLEN1_SHIFT  (0x0005u)
#define CSL_MCBSP_RCR1_RWDLEN1_RESETVAL                                   \
          (0x0000u)
#define CSL_MCBSP_RCR1_RWDLEN1_8BITS  (0x0000u)
#define CSL_MCBSP_RCR1_RWDLEN1_12BITS (0x0001u)
#define CSL_MCBSP_RCR1_RWDLEN1_16BITS (0x0002u)
#define CSL_MCBSP_RCR1_RWDLEN1_20BITS (0x0003u)
#define CSL_MCBSP_RCR1_RWDLEN1_24BITS (0x0004u)
#define CSL_MCBSP_RCR1_RWDLEN1_32BITS (0x0005u)

#define CSL_MCBSP_RCR1_RESETVAL       (0x0000u)


  /* CSL_MCBSP_XCR2 */
#define CSL_MCBSP_XCR2_XPHASE_MASK    (0x8000u)
#define CSL_MCBSP_XCR2_XPHASE_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCR2_XPHASE_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCR2_XFRLEN2_MASK   (0x7F00u)
#define CSL_MCBSP_XCR2_XFRLEN2_SHIFT  (0x0008u)
#define CSL_MCBSP_XCR2_XFRLEN2_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCR2_XWDLEN2_MASK   (0x00E0u)
#define CSL_MCBSP_XCR2_XWDLEN2_SHIFT  (0x0005u)
#define CSL_MCBSP_XCR2_XWDLEN2_RESETVAL                                   \
          (0x0000u)
#define CSL_MCBSP_XCR2_XWDLEN2_8BITS  (0x0000u)
#define CSL_MCBSP_XCR2_XWDLEN2_12BITS (0x0001u)
#define CSL_MCBSP_XCR2_XWDLEN2_16BITS (0x0002u)
#define CSL_MCBSP_XCR2_XWDLEN2_20BITS (0x0003u)
#define CSL_MCBSP_XCR2_XWDLEN2_24BITS (0x0004u)
#define CSL_MCBSP_XCR2_XWDLEN2_32BITS (0x0005u)

#define CSL_MCBSP_XCR2_XCOMPAND_MASK  (0x0018u)
#define CSL_MCBSP_XCR2_XCOMPAND_SHIFT (0x0003u)
#define CSL_MCBSP_XCR2_XCOMPAND_RESETVAL                                  \
          (0x0000u)

#define CSL_MCBSP_XCR2_XFIG_MASK      (0x0004u)
#define CSL_MCBSP_XCR2_XFIG_SHIFT     (0x0002u)
#define CSL_MCBSP_XCR2_XFIG_RESETVAL  (0x0000u)

#define CSL_MCBSP_XCR2_XDATDLY_MASK   (0x0003u)
#define CSL_MCBSP_XCR2_XDATDLY_SHIFT  (0x0000u)
#define CSL_MCBSP_XCR2_XDATDLY_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCR2_RESETVAL       (0x0000u)


  /* CSL_MCBSP_XCR1 */
#define CSL_MCBSP_XCR1_XFRLEN1_MASK   (0x7F00u)
#define CSL_MCBSP_XCR1_XFRLEN1_SHIFT  (0x0008u)
#define CSL_MCBSP_XCR1_XFRLEN1_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCR1_XWDLEN1_MASK   (0x00E0u)
#define CSL_MCBSP_XCR1_XWDLEN1_SHIFT  (0x0005u)
#define CSL_MCBSP_XCR1_XWDLEN1_RESETVAL                                   \
          (0x0000u)
#define CSL_MCBSP_XCR1_XWDLEN1_8BITS  (0x0000u)
#define CSL_MCBSP_XCR1_XWDLEN1_12BITS (0x0001u)
#define CSL_MCBSP_XCR1_XWDLEN1_16BITS (0x0002u)
#define CSL_MCBSP_XCR1_XWDLEN1_20BITS (0x0003u)
#define CSL_MCBSP_XCR1_XWDLEN1_24BITS (0x0004u)
#define CSL_MCBSP_XCR1_XWDLEN1_32BITS (0x0005u)

#define CSL_MCBSP_XCR1_RESETVAL       (0x0000u)


  /* CSL_MCBSP_SRGR2 */
#define CSL_MCBSP_SRGR2_GSYNC_MASK    (0x8000u)
#define CSL_MCBSP_SRGR2_GSYNC_SHIFT   (0x000Fu)
#define CSL_MCBSP_SRGR2_GSYNC_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_SRGR2_CLKSP_MASK    (0x4000u)
#define CSL_MCBSP_SRGR2_CLKSP_SHIFT   (0x000Eu)
#define CSL_MCBSP_SRGR2_CLKSP_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_SRGR2_CLKSM_MASK    (0x2000u)
#define CSL_MCBSP_SRGR2_CLKSM_SHIFT   (0x000Du)
#define CSL_MCBSP_SRGR2_CLKSM_RESETVAL                                    \
          (0x0001u)

#define CSL_MCBSP_SRGR2_FSGM_MASK     (0x1000u)
#define CSL_MCBSP_SRGR2_FSGM_SHIFT    (0x000Cu)
#define CSL_MCBSP_SRGR2_FSGM_RESETVAL (0x0000u)

#define CSL_MCBSP_SRGR2_FPER_MASK     (0x0FFFu)
#define CSL_MCBSP_SRGR2_FPER_SHIFT    (0x0000u)
#define CSL_MCBSP_SRGR2_FPER_RESETVAL (0x0000u)

#define CSL_MCBSP_SRGR2_RESETVAL      (0x2000u)


  /* CSL_MCBSP_SRGR1 */
#define CSL_MCBSP_SRGR1_FWID_MASK     (0xFF00u)
#define CSL_MCBSP_SRGR1_FWID_SHIFT    (0x0008u)
#define CSL_MCBSP_SRGR1_FWID_RESETVAL (0x0000u)

#define CSL_MCBSP_SRGR1_CLKGDV_MASK   (0x00FFu)
#define CSL_MCBSP_SRGR1_CLKGDV_SHIFT  (0x0000u)
#define CSL_MCBSP_SRGR1_CLKGDV_RESETVAL                                   \
          (0x0001u)

#define CSL_MCBSP_SRGR1_RESETVAL      (0x0001u)


  /* CSL_MCBSP_MCR2 */
#define CSL_MCBSP_MCR2_XMCME_MASK     (0x0200u)
#define CSL_MCBSP_MCR2_XMCME_SHIFT    (0x0009u)
#define CSL_MCBSP_MCR2_XMCME_RESETVAL (0x0000u)
#define CSL_MCBSP_MCR2_XMCME_PARTITIONS2                                  \
          (0x0000u)
#define CSL_MCBSP_MCR2_XMCME_PARTITIONS8                                  \
          (0x0001u)

#define CSL_MCBSP_MCR2_XPBBLK_MASK    (0x0180u)
#define CSL_MCBSP_MCR2_XPBBLK_SHIFT   (0x0007u)
#define CSL_MCBSP_MCR2_XPBBLK_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_MCR2_XPABLK_MASK    (0x0060u)
#define CSL_MCBSP_MCR2_XPABLK_SHIFT   (0x0005u)
#define CSL_MCBSP_MCR2_XPABLK_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_MCR2_XCBLK_MASK     (0x001Cu)
#define CSL_MCBSP_MCR2_XCBLK_SHIFT    (0x0002u)
#define CSL_MCBSP_MCR2_XCBLK_RESETVAL (0x0000u)

#define CSL_MCBSP_MCR2_XMCM_MASK      (0x0003u)
#define CSL_MCBSP_MCR2_XMCM_SHIFT     (0x0000u)
#define CSL_MCBSP_MCR2_XMCM_RESETVAL  (0x0000u)
#define CSL_MCBSP_MCR2_XMCM_ENABLE    (0x0000u)
#define CSL_MCBSP_MCR2_XMCM_DISABLE   (0x0001u)
#define CSL_MCBSP_MCR2_XMCM_ENABLE_MASK                                   \
          (0x0002u)
#define CSL_MCBSP_MCR2_XMCM_SYMMETRIC (0x0003u)

#define CSL_MCBSP_MCR2_RESETVAL       (0x0000u)


  /* CSL_MCBSP_MCR1 */
#define CSL_MCBSP_MCR1_RMCME_MASK     (0x0200u)
#define CSL_MCBSP_MCR1_RMCME_SHIFT    (0x0009u)
#define CSL_MCBSP_MCR1_RMCME_RESETVAL (0x0000u)
#define CSL_MCBSP_MCR1_RMCME_PARTITIONS2                                  \
          (0x0000u)
#define CSL_MCBSP_MCR1_RMCME_PARTITIONS8                                  \
          (0x0001u)

#define CSL_MCBSP_MCR1_RPBBLK_MASK    (0x0180u)
#define CSL_MCBSP_MCR1_RPBBLK_SHIFT   (0x0007u)
#define CSL_MCBSP_MCR1_RPBBLK_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_MCR1_RPABLK_MASK    (0x0060u)
#define CSL_MCBSP_MCR1_RPABLK_SHIFT   (0x0005u)
#define CSL_MCBSP_MCR1_RPABLK_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_MCR1_RCBLK_MASK     (0x001Cu)
#define CSL_MCBSP_MCR1_RCBLK_SHIFT    (0x0002u)
#define CSL_MCBSP_MCR1_RCBLK_RESETVAL (0x0000u)

#define CSL_MCBSP_MCR1_RMCM_MASK      (0x0001u)
#define CSL_MCBSP_MCR1_RMCM_SHIFT     (0x0000u)
#define CSL_MCBSP_MCR1_RMCM_RESETVAL  (0x0000u)
#define CSL_MCBSP_MCR1_RMCM_ENABLE    (0x0000u)
#define CSL_MCBSP_MCR1_RMCM_DISABLE   (0x0001u)

#define CSL_MCBSP_MCR1_RESETVAL       (0x0000u)


  /* CSL_MCBSP_RCERA */
#define CSL_MCBSP_RCERA_RCEA0_MASK    (0x8000u)
#define CSL_MCBSP_RCERA_RCEA0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERA_RCEA0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA1_MASK    (0x4000u)
#define CSL_MCBSP_RCERA_RCEA1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERA_RCEA1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA2_MASK    (0x2000u)
#define CSL_MCBSP_RCERA_RCEA2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERA_RCEA2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA3_MASK    (0x1000u)
#define CSL_MCBSP_RCERA_RCEA3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERA_RCEA3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA4_MASK    (0x0800u)
#define CSL_MCBSP_RCERA_RCEA4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERA_RCEA4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA5_MASK    (0x0400u)
#define CSL_MCBSP_RCERA_RCEA5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERA_RCEA5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA6_MASK    (0x0200u)
#define CSL_MCBSP_RCERA_RCEA6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERA_RCEA6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA7_MASK    (0x0100u)
#define CSL_MCBSP_RCERA_RCEA7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERA_RCEA7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA8_MASK    (0x0080u)
#define CSL_MCBSP_RCERA_RCEA8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERA_RCEA8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA9_MASK    (0x0040u)
#define CSL_MCBSP_RCERA_RCEA9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERA_RCEA9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA10_MASK   (0x0020u)
#define CSL_MCBSP_RCERA_RCEA10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERA_RCEA10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA11_MASK   (0x0010u)
#define CSL_MCBSP_RCERA_RCEA11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERA_RCEA11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA12_MASK   (0x0008u)
#define CSL_MCBSP_RCERA_RCEA12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERA_RCEA12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA13_MASK   (0x0004u)
#define CSL_MCBSP_RCERA_RCEA13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERA_RCEA13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA14_MASK   (0x0002u)
#define CSL_MCBSP_RCERA_RCEA14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERA_RCEA14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERA_RCEA15_MASK   (0x0001u)
#define CSL_MCBSP_RCERA_RCEA15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERA_RCEA15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERA_RESETVAL      (0x0000u)


  /* CSL_MCBSP_RCERB */
#define CSL_MCBSP_RCERB_RCEB0_MASK    (0x8000u)
#define CSL_MCBSP_RCERB_RCEB0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERB_RCEB0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB1_MASK    (0x4000u)
#define CSL_MCBSP_RCERB_RCEB1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERB_RCEB1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB2_MASK    (0x2000u)
#define CSL_MCBSP_RCERB_RCEB2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERB_RCEB2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB3_MASK    (0x1000u)
#define CSL_MCBSP_RCERB_RCEB3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERB_RCEB3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB4_MASK    (0x0800u)
#define CSL_MCBSP_RCERB_RCEB4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERB_RCEB4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB5_MASK    (0x0400u)
#define CSL_MCBSP_RCERB_RCEB5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERB_RCEB5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB6_MASK    (0x0200u)
#define CSL_MCBSP_RCERB_RCEB6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERB_RCEB6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB7_MASK    (0x0100u)
#define CSL_MCBSP_RCERB_RCEB7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERB_RCEB7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB8_MASK    (0x0080u)
#define CSL_MCBSP_RCERB_RCEB8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERB_RCEB8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB9_MASK    (0x0040u)
#define CSL_MCBSP_RCERB_RCEB9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERB_RCEB9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB10_MASK   (0x0020u)
#define CSL_MCBSP_RCERB_RCEB10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERB_RCEB10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB11_MASK   (0x0010u)
#define CSL_MCBSP_RCERB_RCEB11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERB_RCEB11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB12_MASK   (0x0008u)
#define CSL_MCBSP_RCERB_RCEB12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERB_RCEB12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB13_MASK   (0x0004u)
#define CSL_MCBSP_RCERB_RCEB13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERB_RCEB13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB14_MASK   (0x0002u)
#define CSL_MCBSP_RCERB_RCEB14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERB_RCEB14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERB_RCEB15_MASK   (0x0001u)
#define CSL_MCBSP_RCERB_RCEB15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERB_RCEB15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERB_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERA */
#define CSL_MCBSP_XCERA_XCEA0_MASK    (0x8000u)
#define CSL_MCBSP_XCERA_XCEA0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERA_XCEA0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA1_MASK    (0x4000u)
#define CSL_MCBSP_XCERA_XCEA1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERA_XCEA1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA2_MASK    (0x2000u)
#define CSL_MCBSP_XCERA_XCEA2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERA_XCEA2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA3_MASK    (0x1000u)
#define CSL_MCBSP_XCERA_XCEA3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERA_XCEA3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA4_MASK    (0x0800u)
#define CSL_MCBSP_XCERA_XCEA4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERA_XCEA4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA5_MASK    (0x0400u)
#define CSL_MCBSP_XCERA_XCEA5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERA_XCEA5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA6_MASK    (0x0200u)
#define CSL_MCBSP_XCERA_XCEA6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERA_XCEA6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA7_MASK    (0x0100u)
#define CSL_MCBSP_XCERA_XCEA7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERA_XCEA7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA8_MASK    (0x0080u)
#define CSL_MCBSP_XCERA_XCEA8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERA_XCEA8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA9_MASK    (0x0040u)
#define CSL_MCBSP_XCERA_XCEA9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERA_XCEA9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA10_MASK   (0x0020u)
#define CSL_MCBSP_XCERA_XCEA10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERA_XCEA10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA11_MASK   (0x0010u)
#define CSL_MCBSP_XCERA_XCEA11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERA_XCEA11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA12_MASK   (0x0008u)
#define CSL_MCBSP_XCERA_XCEA12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERA_XCEA12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA13_MASK   (0x0004u)
#define CSL_MCBSP_XCERA_XCEA13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERA_XCEA13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA14_MASK   (0x0002u)
#define CSL_MCBSP_XCERA_XCEA14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERA_XCEA14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERA_XCEA15_MASK   (0x0001u)
#define CSL_MCBSP_XCERA_XCEA15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERA_XCEA15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERA_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERB */
#define CSL_MCBSP_XCERB_XCEB0_MASK    (0x8000u)
#define CSL_MCBSP_XCERB_XCEB0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERB_XCEB0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB1_MASK    (0x4000u)
#define CSL_MCBSP_XCERB_XCEB1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERB_XCEB1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB2_MASK    (0x2000u)
#define CSL_MCBSP_XCERB_XCEB2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERB_XCEB2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB3_MASK    (0x1000u)
#define CSL_MCBSP_XCERB_XCEB3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERB_XCEB3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB4_MASK    (0x0800u)
#define CSL_MCBSP_XCERB_XCEB4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERB_XCEB4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB5_MASK    (0x0400u)
#define CSL_MCBSP_XCERB_XCEB5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERB_XCEB5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB6_MASK    (0x0200u)
#define CSL_MCBSP_XCERB_XCEB6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERB_XCEB6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB7_MASK    (0x0100u)
#define CSL_MCBSP_XCERB_XCEB7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERB_XCEB7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB8_MASK    (0x0080u)
#define CSL_MCBSP_XCERB_XCEB8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERB_XCEB8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB9_MASK    (0x0040u)
#define CSL_MCBSP_XCERB_XCEB9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERB_XCEB9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB10_MASK   (0x0020u)
#define CSL_MCBSP_XCERB_XCEB10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERB_XCEB10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB11_MASK   (0x0010u)
#define CSL_MCBSP_XCERB_XCEB11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERB_XCEB11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB12_MASK   (0x0008u)
#define CSL_MCBSP_XCERB_XCEB12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERB_XCEB12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB13_MASK   (0x0004u)
#define CSL_MCBSP_XCERB_XCEB13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERB_XCEB13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB14_MASK   (0x0002u)
#define CSL_MCBSP_XCERB_XCEB14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERB_XCEB14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERB_XCEB15_MASK   (0x0001u)
#define CSL_MCBSP_XCERB_XCEB15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERB_XCEB15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERB_RESETVAL      (0x0000u)


  /* CSL_MCBSP_PCR */
#define CSL_MCBSP_PCR_IDLEN_MASK      (0x4000u)
#define CSL_MCBSP_PCR_IDLEN_SHIFT     (0x000Eu)
#define CSL_MCBSP_PCR_IDLEN_RESETVAL  (0x0000u)

#define CSL_MCBSP_PCR_XIOEN_MASK      (0x2000u)
#define CSL_MCBSP_PCR_XIOEN_SHIFT     (0x000Du)
#define CSL_MCBSP_PCR_XIOEN_RESETVAL  (0x0000u)

#define CSL_MCBSP_PCR_RIOEN_MASK      (0x1000u)
#define CSL_MCBSP_PCR_RIOEN_SHIFT     (0x000Cu)
#define CSL_MCBSP_PCR_RIOEN_RESETVAL  (0x0000u)

#define CSL_MCBSP_PCR_FSXM_MASK       (0x0800u)
#define CSL_MCBSP_PCR_FSXM_SHIFT      (0x000Bu)
#define CSL_MCBSP_PCR_FSXM_RESETVAL   (0x0000u)
#define CSL_MCBSP_PCR_FSXM_INPUT      (0x0000u)
#define CSL_MCBSP_PCR_FSXM_OUTPUT     (0x0001u)

#define CSL_MCBSP_PCR_FSRM_MASK       (0x0400u)
#define CSL_MCBSP_PCR_FSRM_SHIFT      (0x000Au)
#define CSL_MCBSP_PCR_FSRM_RESETVAL   (0x0000u)
#define CSL_MCBSP_PCR_FSRM_INPUT      (0x0000u)
#define CSL_MCBSP_PCR_FSRM_OUTPUT     (0x0001u)

#define CSL_MCBSP_PCR_CLKXM_MASK      (0x0200u)
#define CSL_MCBSP_PCR_CLKXM_SHIFT     (0x0009u)
#define CSL_MCBSP_PCR_CLKXM_RESETVAL  (0x0000u)
#define CSL_MCBSP_PCR_CLKXM_INPUT     (0x0000u)
#define CSL_MCBSP_PCR_CLKXM_OUTPUT    (0x0001u)

#define CSL_MCBSP_PCR_CLKRM_MASK      (0x0100u)
#define CSL_MCBSP_PCR_CLKRM_SHIFT     (0x0008u)
#define CSL_MCBSP_PCR_CLKRM_RESETVAL  (0x0000u)
#define CSL_MCBSP_PCR_CLKRM_INPUT     (0x0000u)
#define CSL_MCBSP_PCR_CLKRM_OUTPUT    (0x0001u)

#define CSL_MCBSP_PCR_SCLKME_MASK     (0x0080u)
#define CSL_MCBSP_PCR_SCLKME_SHIFT    (0x0007u)
#define CSL_MCBSP_PCR_SCLKME_RESETVAL (0x0000u)

#define CSL_MCBSP_PCR_CLKSSTAT_MASK   (0x0040u)
#define CSL_MCBSP_PCR_CLKSSTAT_SHIFT  (0x0006u)
#define CSL_MCBSP_PCR_CLKSSTAT_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_PCR_DXSTAT_MASK     (0x0020u)
#define CSL_MCBSP_PCR_DXSTAT_SHIFT    (0x0005u)
#define CSL_MCBSP_PCR_DXSTAT_RESETVAL (0x0000u)

#define CSL_MCBSP_PCR_DRSTAT_MASK     (0x0010u)
#define CSL_MCBSP_PCR_DRSTAT_SHIFT    (0x0004u)
#define CSL_MCBSP_PCR_DRSTAT_RESETVAL (0x0000u)

#define CSL_MCBSP_PCR_FSXP_MASK       (0x0008u)
#define CSL_MCBSP_PCR_FSXP_SHIFT      (0x0003u)
#define CSL_MCBSP_PCR_FSXP_RESETVAL   (0x0000u)

#define CSL_MCBSP_PCR_FSRP_MASK       (0x0004u)
#define CSL_MCBSP_PCR_FSRP_SHIFT      (0x0002u)
#define CSL_MCBSP_PCR_FSRP_RESETVAL   (0x0000u)

#define CSL_MCBSP_PCR_CLKXP_MASK      (0x0002u)
#define CSL_MCBSP_PCR_CLKXP_SHIFT     (0x0001u)
#define CSL_MCBSP_PCR_CLKXP_RESETVAL  (0x0000u)

#define CSL_MCBSP_PCR_CLKRP_MASK      (0x0001u)
#define CSL_MCBSP_PCR_CLKRP_SHIFT     (0x0000u)
#define CSL_MCBSP_PCR_CLKRP_RESETVAL  (0x0000u)

#define CSL_MCBSP_PCR_RESETVAL        (0x0000u)


  /* CSL_MCBSP_RCERC */
#define CSL_MCBSP_RCERC_RCEC0_MASK    (0x8000u)
#define CSL_MCBSP_RCERC_RCEC0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERC_RCEC0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC1_MASK    (0x4000u)
#define CSL_MCBSP_RCERC_RCEC1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERC_RCEC1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC2_MASK    (0x2000u)
#define CSL_MCBSP_RCERC_RCEC2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERC_RCEC2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC3_MASK    (0x1000u)
#define CSL_MCBSP_RCERC_RCEC3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERC_RCEC3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC4_MASK    (0x0800u)
#define CSL_MCBSP_RCERC_RCEC4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERC_RCEC4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC5_MASK    (0x0400u)
#define CSL_MCBSP_RCERC_RCEC5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERC_RCEC5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC6_MASK    (0x0200u)
#define CSL_MCBSP_RCERC_RCEC6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERC_RCEC6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC7_MASK    (0x0100u)
#define CSL_MCBSP_RCERC_RCEC7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERC_RCEC7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC8_MASK    (0x0080u)
#define CSL_MCBSP_RCERC_RCEC8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERC_RCEC8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC9_MASK    (0x0040u)
#define CSL_MCBSP_RCERC_RCEC9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERC_RCEC9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC10_MASK   (0x0020u)
#define CSL_MCBSP_RCERC_RCEC10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERC_RCEC10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC11_MASK   (0x0010u)
#define CSL_MCBSP_RCERC_RCEC11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERC_RCEC11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC12_MASK   (0x0008u)
#define CSL_MCBSP_RCERC_RCEC12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERC_RCEC12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC13_MASK   (0x0004u)
#define CSL_MCBSP_RCERC_RCEC13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERC_RCEC13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC14_MASK   (0x0002u)
#define CSL_MCBSP_RCERC_RCEC14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERC_RCEC14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERC_RCEC15_MASK   (0x0001u)
#define CSL_MCBSP_RCERC_RCEC15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERC_RCEC15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERC_RESETVAL      (0x0000u)


  /* CSL_MCBSP_RCERD */
#define CSL_MCBSP_RCERD_RCED0_MASK    (0x8000u)
#define CSL_MCBSP_RCERD_RCED0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERD_RCED0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED1_MASK    (0x4000u)
#define CSL_MCBSP_RCERD_RCED1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERD_RCED1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED2_MASK    (0x2000u)
#define CSL_MCBSP_RCERD_RCED2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERD_RCED2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED3_MASK    (0x1000u)
#define CSL_MCBSP_RCERD_RCED3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERD_RCED3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED4_MASK    (0x0800u)
#define CSL_MCBSP_RCERD_RCED4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERD_RCED4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED5_MASK    (0x0400u)
#define CSL_MCBSP_RCERD_RCED5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERD_RCED5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED6_MASK    (0x0200u)
#define CSL_MCBSP_RCERD_RCED6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERD_RCED6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED7_MASK    (0x0100u)
#define CSL_MCBSP_RCERD_RCED7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERD_RCED7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED8_MASK    (0x0080u)
#define CSL_MCBSP_RCERD_RCED8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERD_RCED8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED9_MASK    (0x0040u)
#define CSL_MCBSP_RCERD_RCED9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERD_RCED9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED10_MASK   (0x0020u)
#define CSL_MCBSP_RCERD_RCED10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERD_RCED10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED11_MASK   (0x0010u)
#define CSL_MCBSP_RCERD_RCED11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERD_RCED11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED12_MASK   (0x0008u)
#define CSL_MCBSP_RCERD_RCED12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERD_RCED12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED13_MASK   (0x0004u)
#define CSL_MCBSP_RCERD_RCED13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERD_RCED13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED14_MASK   (0x0002u)
#define CSL_MCBSP_RCERD_RCED14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERD_RCED14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERD_RCED15_MASK   (0x0001u)
#define CSL_MCBSP_RCERD_RCED15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERD_RCED15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERD_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERC */
#define CSL_MCBSP_XCERC_XCEC0_MASK    (0x8000u)
#define CSL_MCBSP_XCERC_XCEC0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERC_XCEC0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC1_MASK    (0x4000u)
#define CSL_MCBSP_XCERC_XCEC1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERC_XCEC1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC2_MASK    (0x2000u)
#define CSL_MCBSP_XCERC_XCEC2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERC_XCEC2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC3_MASK    (0x1000u)
#define CSL_MCBSP_XCERC_XCEC3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERC_XCEC3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC4_MASK    (0x0800u)
#define CSL_MCBSP_XCERC_XCEC4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERC_XCEC4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC5_MASK    (0x0400u)
#define CSL_MCBSP_XCERC_XCEC5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERC_XCEC5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC6_MASK    (0x0200u)
#define CSL_MCBSP_XCERC_XCEC6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERC_XCEC6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC7_MASK    (0x0100u)
#define CSL_MCBSP_XCERC_XCEC7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERC_XCEC7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC8_MASK    (0x0080u)
#define CSL_MCBSP_XCERC_XCEC8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERC_XCEC8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC9_MASK    (0x0040u)
#define CSL_MCBSP_XCERC_XCEC9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERC_XCEC9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC10_MASK   (0x0020u)
#define CSL_MCBSP_XCERC_XCEC10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERC_XCEC10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC11_MASK   (0x0010u)
#define CSL_MCBSP_XCERC_XCEC11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERC_XCEC11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC12_MASK   (0x0008u)
#define CSL_MCBSP_XCERC_XCEC12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERC_XCEC12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC13_MASK   (0x0004u)
#define CSL_MCBSP_XCERC_XCEC13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERC_XCEC13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC14_MASK   (0x0002u)
#define CSL_MCBSP_XCERC_XCEC14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERC_XCEC14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERC_XCEC15_MASK   (0x0001u)
#define CSL_MCBSP_XCERC_XCEC15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERC_XCEC15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERC_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERD */
#define CSL_MCBSP_XCERD_XCED0_MASK    (0x8000u)
#define CSL_MCBSP_XCERD_XCED0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERD_XCED0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED1_MASK    (0x4000u)
#define CSL_MCBSP_XCERD_XCED1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERD_XCED1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED2_MASK    (0x2000u)
#define CSL_MCBSP_XCERD_XCED2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERD_XCED2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED3_MASK    (0x1000u)
#define CSL_MCBSP_XCERD_XCED3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERD_XCED3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED4_MASK    (0x0800u)
#define CSL_MCBSP_XCERD_XCED4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERD_XCED4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED5_MASK    (0x0400u)
#define CSL_MCBSP_XCERD_XCED5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERD_XCED5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED6_MASK    (0x0200u)
#define CSL_MCBSP_XCERD_XCED6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERD_XCED6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED7_MASK    (0x0100u)
#define CSL_MCBSP_XCERD_XCED7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERD_XCED7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED8_MASK    (0x0080u)
#define CSL_MCBSP_XCERD_XCED8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERD_XCED8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED9_MASK    (0x0040u)
#define CSL_MCBSP_XCERD_XCED9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERD_XCED9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED10_MASK   (0x0020u)
#define CSL_MCBSP_XCERD_XCED10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERD_XCED10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED11_MASK   (0x0010u)
#define CSL_MCBSP_XCERD_XCED11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERD_XCED11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED12_MASK   (0x0008u)
#define CSL_MCBSP_XCERD_XCED12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERD_XCED12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED13_MASK   (0x0004u)
#define CSL_MCBSP_XCERD_XCED13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERD_XCED13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED14_MASK   (0x0002u)
#define CSL_MCBSP_XCERD_XCED14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERD_XCED14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERD_XCED15_MASK   (0x0001u)
#define CSL_MCBSP_XCERD_XCED15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERD_XCED15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERD_RESETVAL      (0x0000u)


  /* CSL_MCBSP_RCERE */
#define CSL_MCBSP_RCERE_RCEE0_MASK    (0x8000u)
#define CSL_MCBSP_RCERE_RCEE0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERE_RCEE0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE1_MASK    (0x4000u)
#define CSL_MCBSP_RCERE_RCEE1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERE_RCEE1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE2_MASK    (0x2000u)
#define CSL_MCBSP_RCERE_RCEE2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERE_RCEE2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE3_MASK    (0x1000u)
#define CSL_MCBSP_RCERE_RCEE3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERE_RCEE3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE4_MASK    (0x0800u)
#define CSL_MCBSP_RCERE_RCEE4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERE_RCEE4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE5_MASK    (0x0400u)
#define CSL_MCBSP_RCERE_RCEE5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERE_RCEE5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE6_MASK    (0x0200u)
#define CSL_MCBSP_RCERE_RCEE6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERE_RCEE6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE7_MASK    (0x0100u)
#define CSL_MCBSP_RCERE_RCEE7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERE_RCEE7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE8_MASK    (0x0080u)
#define CSL_MCBSP_RCERE_RCEE8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERE_RCEE8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE9_MASK    (0x0040u)
#define CSL_MCBSP_RCERE_RCEE9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERE_RCEE9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE10_MASK   (0x0020u)
#define CSL_MCBSP_RCERE_RCEE10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERE_RCEE10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE11_MASK   (0x0010u)
#define CSL_MCBSP_RCERE_RCEE11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERE_RCEE11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE12_MASK   (0x0008u)
#define CSL_MCBSP_RCERE_RCEE12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERE_RCEE12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE13_MASK   (0x0004u)
#define CSL_MCBSP_RCERE_RCEE13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERE_RCEE13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE14_MASK   (0x0002u)
#define CSL_MCBSP_RCERE_RCEE14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERE_RCEE14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERE_RCEE15_MASK   (0x0001u)
#define CSL_MCBSP_RCERE_RCEE15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERE_RCEE15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERE_RESETVAL      (0x0000u)


  /* CSL_MCBSP_RCERF */
#define CSL_MCBSP_RCERF_RCEF0_MASK    (0x8000u)
#define CSL_MCBSP_RCERF_RCEF0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERF_RCEF0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF1_MASK    (0x4000u)
#define CSL_MCBSP_RCERF_RCEF1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERF_RCEF1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF2_MASK    (0x2000u)
#define CSL_MCBSP_RCERF_RCEF2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERF_RCEF2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF3_MASK    (0x1000u)
#define CSL_MCBSP_RCERF_RCEF3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERF_RCEF3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF4_MASK    (0x0800u)
#define CSL_MCBSP_RCERF_RCEF4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERF_RCEF4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF5_MASK    (0x0400u)
#define CSL_MCBSP_RCERF_RCEF5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERF_RCEF5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF6_MASK    (0x0200u)
#define CSL_MCBSP_RCERF_RCEF6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERF_RCEF6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF7_MASK    (0x0100u)
#define CSL_MCBSP_RCERF_RCEF7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERF_RCEF7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF8_MASK    (0x0080u)
#define CSL_MCBSP_RCERF_RCEF8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERF_RCEF8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF9_MASK    (0x0040u)
#define CSL_MCBSP_RCERF_RCEF9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERF_RCEF9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF10_MASK   (0x0020u)
#define CSL_MCBSP_RCERF_RCEF10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERF_RCEF10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF11_MASK   (0x0010u)
#define CSL_MCBSP_RCERF_RCEF11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERF_RCEF11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF12_MASK   (0x0008u)
#define CSL_MCBSP_RCERF_RCEF12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERF_RCEF12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF13_MASK   (0x0004u)
#define CSL_MCBSP_RCERF_RCEF13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERF_RCEF13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF14_MASK   (0x0002u)
#define CSL_MCBSP_RCERF_RCEF14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERF_RCEF14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERF_RCEF15_MASK   (0x0001u)
#define CSL_MCBSP_RCERF_RCEF15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERF_RCEF15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERF_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERE */
#define CSL_MCBSP_XCERE_XCEE0_MASK    (0x8000u)
#define CSL_MCBSP_XCERE_XCEE0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERE_XCEE0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE1_MASK    (0x4000u)
#define CSL_MCBSP_XCERE_XCEE1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERE_XCEE1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE2_MASK    (0x2000u)
#define CSL_MCBSP_XCERE_XCEE2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERE_XCEE2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE3_MASK    (0x1000u)
#define CSL_MCBSP_XCERE_XCEE3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERE_XCEE3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE4_MASK    (0x0800u)
#define CSL_MCBSP_XCERE_XCEE4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERE_XCEE4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE5_MASK    (0x0400u)
#define CSL_MCBSP_XCERE_XCEE5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERE_XCEE5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE6_MASK    (0x0200u)
#define CSL_MCBSP_XCERE_XCEE6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERE_XCEE6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE7_MASK    (0x0100u)
#define CSL_MCBSP_XCERE_XCEE7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERE_XCEE7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE8_MASK    (0x0080u)
#define CSL_MCBSP_XCERE_XCEE8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERE_XCEE8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE9_MASK    (0x0040u)
#define CSL_MCBSP_XCERE_XCEE9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERE_XCEE9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE10_MASK   (0x0020u)
#define CSL_MCBSP_XCERE_XCEE10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERE_XCEE10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE11_MASK   (0x0010u)
#define CSL_MCBSP_XCERE_XCEE11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERE_XCEE11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE12_MASK   (0x0008u)
#define CSL_MCBSP_XCERE_XCEE12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERE_XCEE12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE13_MASK   (0x0004u)
#define CSL_MCBSP_XCERE_XCEE13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERE_XCEE13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE14_MASK   (0x0002u)
#define CSL_MCBSP_XCERE_XCEE14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERE_XCEE14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERE_XCEE15_MASK   (0x0001u)
#define CSL_MCBSP_XCERE_XCEE15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERE_XCEE15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERE_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERF */
#define CSL_MCBSP_XCERF_XCEF0_MASK    (0x8000u)
#define CSL_MCBSP_XCERF_XCEF0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERF_XCEF0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF1_MASK    (0x4000u)
#define CSL_MCBSP_XCERF_XCEF1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERF_XCEF1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF2_MASK    (0x2000u)
#define CSL_MCBSP_XCERF_XCEF2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERF_XCEF2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF3_MASK    (0x1000u)
#define CSL_MCBSP_XCERF_XCEF3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERF_XCEF3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF4_MASK    (0x0800u)
#define CSL_MCBSP_XCERF_XCEF4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERF_XCEF4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF5_MASK    (0x0400u)
#define CSL_MCBSP_XCERF_XCEF5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERF_XCEF5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF6_MASK    (0x0200u)
#define CSL_MCBSP_XCERF_XCEF6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERF_XCEF6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF7_MASK    (0x0100u)
#define CSL_MCBSP_XCERF_XCEF7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERF_XCEF7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF8_MASK    (0x0080u)
#define CSL_MCBSP_XCERF_XCEF8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERF_XCEF8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF9_MASK    (0x0040u)
#define CSL_MCBSP_XCERF_XCEF9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERF_XCEF9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF10_MASK   (0x0020u)
#define CSL_MCBSP_XCERF_XCEF10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERF_XCEF10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF11_MASK   (0x0010u)
#define CSL_MCBSP_XCERF_XCEF11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERF_XCEF11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF12_MASK   (0x0008u)
#define CSL_MCBSP_XCERF_XCEF12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERF_XCEF12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF13_MASK   (0x0004u)
#define CSL_MCBSP_XCERF_XCEF13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERF_XCEF13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF14_MASK   (0x0002u)
#define CSL_MCBSP_XCERF_XCEF14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERF_XCEF14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERF_XCEF15_MASK   (0x0001u)
#define CSL_MCBSP_XCERF_XCEF15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERF_XCEF15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERF_RESETVAL      (0x0000u)


  /* CSL_MCBSP_RCERG */
#define CSL_MCBSP_RCERG_RCEG0_MASK    (0x8000u)
#define CSL_MCBSP_RCERG_RCEG0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERG_RCEG0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG1_MASK    (0x4000u)
#define CSL_MCBSP_RCERG_RCEG1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERG_RCEG1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG2_MASK    (0x2000u)
#define CSL_MCBSP_RCERG_RCEG2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERG_RCEG2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG3_MASK    (0x1000u)
#define CSL_MCBSP_RCERG_RCEG3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERG_RCEG3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG4_MASK    (0x0800u)
#define CSL_MCBSP_RCERG_RCEG4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERG_RCEG4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG5_MASK    (0x0400u)
#define CSL_MCBSP_RCERG_RCEG5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERG_RCEG5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG6_MASK    (0x0200u)
#define CSL_MCBSP_RCERG_RCEG6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERG_RCEG6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG7_MASK    (0x0100u)
#define CSL_MCBSP_RCERG_RCEG7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERG_RCEG7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG8_MASK    (0x0080u)
#define CSL_MCBSP_RCERG_RCEG8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERG_RCEG8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG9_MASK    (0x0040u)
#define CSL_MCBSP_RCERG_RCEG9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERG_RCEG9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG10_MASK   (0x0020u)
#define CSL_MCBSP_RCERG_RCEG10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERG_RCEG10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG11_MASK   (0x0010u)
#define CSL_MCBSP_RCERG_RCEG11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERG_RCEG11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG12_MASK   (0x0008u)
#define CSL_MCBSP_RCERG_RCEG12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERG_RCEG12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG13_MASK   (0x0004u)
#define CSL_MCBSP_RCERG_RCEG13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERG_RCEG13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG14_MASK   (0x0002u)
#define CSL_MCBSP_RCERG_RCEG14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERG_RCEG14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERG_RCEG15_MASK   (0x0001u)
#define CSL_MCBSP_RCERG_RCEG15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERG_RCEG15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERG_RESETVAL      (0x0000u)


  /* CSL_MCBSP_RCERH */
#define CSL_MCBSP_RCERH_RCEH0_MASK    (0x8000u)
#define CSL_MCBSP_RCERH_RCEH0_SHIFT   (0x000Fu)
#define CSL_MCBSP_RCERH_RCEH0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH1_MASK    (0x4000u)
#define CSL_MCBSP_RCERH_RCEH1_SHIFT   (0x000Eu)
#define CSL_MCBSP_RCERH_RCEH1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH2_MASK    (0x2000u)
#define CSL_MCBSP_RCERH_RCEH2_SHIFT   (0x000Du)
#define CSL_MCBSP_RCERH_RCEH2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH3_MASK    (0x1000u)
#define CSL_MCBSP_RCERH_RCEH3_SHIFT   (0x000Cu)
#define CSL_MCBSP_RCERH_RCEH3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH4_MASK    (0x0800u)
#define CSL_MCBSP_RCERH_RCEH4_SHIFT   (0x000Bu)
#define CSL_MCBSP_RCERH_RCEH4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH5_MASK    (0x0400u)
#define CSL_MCBSP_RCERH_RCEH5_SHIFT   (0x000Au)
#define CSL_MCBSP_RCERH_RCEH5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH6_MASK    (0x0200u)
#define CSL_MCBSP_RCERH_RCEH6_SHIFT   (0x0009u)
#define CSL_MCBSP_RCERH_RCEH6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH7_MASK    (0x0100u)
#define CSL_MCBSP_RCERH_RCEH7_SHIFT   (0x0008u)
#define CSL_MCBSP_RCERH_RCEH7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH8_MASK    (0x0080u)
#define CSL_MCBSP_RCERH_RCEH8_SHIFT   (0x0007u)
#define CSL_MCBSP_RCERH_RCEH8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH9_MASK    (0x0040u)
#define CSL_MCBSP_RCERH_RCEH9_SHIFT   (0x0006u)
#define CSL_MCBSP_RCERH_RCEH9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH10_MASK   (0x0020u)
#define CSL_MCBSP_RCERH_RCEH10_SHIFT  (0x0005u)
#define CSL_MCBSP_RCERH_RCEH10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH11_MASK   (0x0010u)
#define CSL_MCBSP_RCERH_RCEH11_SHIFT  (0x0004u)
#define CSL_MCBSP_RCERH_RCEH11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH12_MASK   (0x0008u)
#define CSL_MCBSP_RCERH_RCEH12_SHIFT  (0x0003u)
#define CSL_MCBSP_RCERH_RCEH12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH13_MASK   (0x0004u)
#define CSL_MCBSP_RCERH_RCEH13_SHIFT  (0x0002u)
#define CSL_MCBSP_RCERH_RCEH13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH14_MASK   (0x0002u)
#define CSL_MCBSP_RCERH_RCEH14_SHIFT  (0x0001u)
#define CSL_MCBSP_RCERH_RCEH14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERH_RCEH15_MASK   (0x0001u)
#define CSL_MCBSP_RCERH_RCEH15_SHIFT  (0x0000u)
#define CSL_MCBSP_RCERH_RCEH15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_RCERH_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERG */
#define CSL_MCBSP_XCERG_XCEG0_MASK    (0x8000u)
#define CSL_MCBSP_XCERG_XCEG0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERG_XCEG0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG1_MASK    (0x4000u)
#define CSL_MCBSP_XCERG_XCEG1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERG_XCEG1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG2_MASK    (0x2000u)
#define CSL_MCBSP_XCERG_XCEG2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERG_XCEG2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG3_MASK    (0x1000u)
#define CSL_MCBSP_XCERG_XCEG3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERG_XCEG3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG4_MASK    (0x0800u)
#define CSL_MCBSP_XCERG_XCEG4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERG_XCEG4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG5_MASK    (0x0400u)
#define CSL_MCBSP_XCERG_XCEG5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERG_XCEG5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG6_MASK    (0x0200u)
#define CSL_MCBSP_XCERG_XCEG6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERG_XCEG6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG7_MASK    (0x0100u)
#define CSL_MCBSP_XCERG_XCEG7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERG_XCEG7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG8_MASK    (0x0080u)
#define CSL_MCBSP_XCERG_XCEG8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERG_XCEG8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG9_MASK    (0x0040u)
#define CSL_MCBSP_XCERG_XCEG9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERG_XCEG9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG10_MASK   (0x0020u)
#define CSL_MCBSP_XCERG_XCEG10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERG_XCEG10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG11_MASK   (0x0010u)
#define CSL_MCBSP_XCERG_XCEG11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERG_XCEG11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG12_MASK   (0x0008u)
#define CSL_MCBSP_XCERG_XCEG12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERG_XCEG12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG13_MASK   (0x0004u)
#define CSL_MCBSP_XCERG_XCEG13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERG_XCEG13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG14_MASK   (0x0002u)
#define CSL_MCBSP_XCERG_XCEG14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERG_XCEG14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERG_XCEG15_MASK   (0x0001u)
#define CSL_MCBSP_XCERG_XCEG15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERG_XCEG15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERG_RESETVAL      (0x0000u)


  /* CSL_MCBSP_XCERH */
#define CSL_MCBSP_XCERH_XCEH0_MASK    (0x8000u)
#define CSL_MCBSP_XCERH_XCEH0_SHIFT   (0x000Fu)
#define CSL_MCBSP_XCERH_XCEH0_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH1_MASK    (0x4000u)
#define CSL_MCBSP_XCERH_XCEH1_SHIFT   (0x000Eu)
#define CSL_MCBSP_XCERH_XCEH1_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH2_MASK    (0x2000u)
#define CSL_MCBSP_XCERH_XCEH2_SHIFT   (0x000Du)
#define CSL_MCBSP_XCERH_XCEH2_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH3_MASK    (0x1000u)
#define CSL_MCBSP_XCERH_XCEH3_SHIFT   (0x000Cu)
#define CSL_MCBSP_XCERH_XCEH3_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH4_MASK    (0x0800u)
#define CSL_MCBSP_XCERH_XCEH4_SHIFT   (0x000Bu)
#define CSL_MCBSP_XCERH_XCEH4_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH5_MASK    (0x0400u)
#define CSL_MCBSP_XCERH_XCEH5_SHIFT   (0x000Au)
#define CSL_MCBSP_XCERH_XCEH5_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH6_MASK    (0x0200u)
#define CSL_MCBSP_XCERH_XCEH6_SHIFT   (0x0009u)
#define CSL_MCBSP_XCERH_XCEH6_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH7_MASK    (0x0100u)
#define CSL_MCBSP_XCERH_XCEH7_SHIFT   (0x0008u)
#define CSL_MCBSP_XCERH_XCEH7_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH8_MASK    (0x0080u)
#define CSL_MCBSP_XCERH_XCEH8_SHIFT   (0x0007u)
#define CSL_MCBSP_XCERH_XCEH8_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH9_MASK    (0x0040u)
#define CSL_MCBSP_XCERH_XCEH9_SHIFT   (0x0006u)
#define CSL_MCBSP_XCERH_XCEH9_RESETVAL                                    \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH10_MASK   (0x0020u)
#define CSL_MCBSP_XCERH_XCEH10_SHIFT  (0x0005u)
#define CSL_MCBSP_XCERH_XCEH10_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH11_MASK   (0x0010u)
#define CSL_MCBSP_XCERH_XCEH11_SHIFT  (0x0004u)
#define CSL_MCBSP_XCERH_XCEH11_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH12_MASK   (0x0008u)
#define CSL_MCBSP_XCERH_XCEH12_SHIFT  (0x0003u)
#define CSL_MCBSP_XCERH_XCEH12_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH13_MASK   (0x0004u)
#define CSL_MCBSP_XCERH_XCEH13_SHIFT  (0x0002u)
#define CSL_MCBSP_XCERH_XCEH13_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH14_MASK   (0x0002u)
#define CSL_MCBSP_XCERH_XCEH14_SHIFT  (0x0001u)
#define CSL_MCBSP_XCERH_XCEH14_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERH_XCEH15_MASK   (0x0001u)
#define CSL_MCBSP_XCERH_XCEH15_SHIFT  (0x0000u)
#define CSL_MCBSP_XCERH_XCEH15_RESETVAL                                   \
          (0x0000u)

#define CSL_MCBSP_XCERH_RESETVAL      (0x0000u)




#endif
