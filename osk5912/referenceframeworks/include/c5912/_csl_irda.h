/** =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   priovided
*   ==========================================================
*/
#define CSL_IRDA_LCR_STORE(val)                         val = hIrda->regs->LCR
#define CSL_IRDA_LCR_RESTORE(val)                       hIrda->regs->LCR = val
#define CSL_IRDA_SETLCR_BF                      hIrda->regs->LCR = 0xBF
#define CSL_IRDA_SETLCRBIT7_0                   CSL_FINSR(hIrda->regs->LCR,7,7,0)
#define CSL_IRDA_SETLCRBIT7_1                   CSL_FINSR(hIrda->regs->LCR,7,7,1)
