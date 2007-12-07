#ifndef __CSL_CLKRSTHWCONTROL_H_
#define __CSL_CLKRSTHWCONTROL_H_

#include <csl_clkrst.h>

inline void _CSL_clkrstCkEn(
                CSL_ClkrstHandle        hClkrst,
                CSL_ClkrstHwControlCmd  cmd,
                Uint16                  ckEnable
                )
{

        int i;
        Uint16 _dsp_idlect2 = 0;
        Uint16 clkSrcToClkEnLookup[] = { CSL_FMKT(CLKRST_DSP_IDLECT2_EN_TIMCK, ACTIVE),
                                         CSL_FMKT(CLKRST_DSP_IDLECT2_EN_PERCK, ACTIVE),
                                         CSL_FMKT(CLKRST_DSP_IDLECT2_EN_XORPCK, ACTIVE),
                                         CSL_FMKT(CLKRST_DSP_IDLECT2_EN_WDTCK, ACTIVE)
                                       };

        for (i = 0; i < 4; i++) {
                if (ckEnable & (1 << i)) {
                        _dsp_idlect2 |= clkSrcToClkEnLookup[i];
                }
        }

        if (CSL_CLKRST_CMD_CLK_ENABLE == cmd) {
                hClkrst->regs->DSP_IDLECT2 |= _dsp_idlect2;
        } else if (CSL_CLKRST_CMD_CLK_DISABLE == cmd) {
                hClkrst->regs->DSP_IDLECT2 &= ~_dsp_idlect2;
        }

}


inline void _CSL_clkrstReset(
                CSL_ClkrstHandle        hClkrst,
                CSL_ClkrstHwControlCmd  cmd,
                Uint16                  resetSrc
                )
{

        Uint16 resetSrcToResetIn[] = { CSL_CLKRST_DSP_RSTCT2_PER_EN_RESET,
                                       CSL_CLKRST_DSP_RSTCT2_WD_PER_EN_RESET };

        Uint16 resetSrcToResetOut[] = { CSL_CLKRST_DSP_RSTCT2_PER_EN_ENABLED,
                                        CSL_CLKRST_DSP_RSTCT2_WD_PER_EN_ENABLED};
        
        Uint16 *lookupTbl;
        Uint16 _dsp_rstct2;
        int i;
        
        if (CSL_CLKRST_CMD_RESET_IN == cmd) {
                lookupTbl = resetSrcToResetIn;
        } else if (CSL_CLKRST_CMD_RESET_OUT == cmd) {
                lookupTbl = resetSrcToResetOut;
        } else {
                return;
        }

        _dsp_rstct2 = hClkrst->regs->DSP_RSTCT2;
        for (i = 0; i < 2; i++) {
                if (resetSrc & (1 << i)) {
                        CSL_FINSR(_dsp_rstct2, i, i, lookupTbl[i]);
                }
        };
        hClkrst->regs->DSP_RSTCT2 = _dsp_rstct2;

}

#endif
