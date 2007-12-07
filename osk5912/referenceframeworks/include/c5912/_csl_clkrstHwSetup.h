#ifndef _CSL_CLKRSTHWSETUP_H_
#define _CSL_CLKRSTHWSETUP_H_

#include <csl_clkrst.h>

inline void _CSL_clkrstClkIdleEntry(
                CSL_ClkrstHandle        hClkrst,
                Uint16                  ckIdleEntry
                )
{

        int i;
        Uint16 _dsp_idlect1 = 0;
        Uint16 clkSrcToIdleEntryLookup[] = { CSL_FMKT(CLKRST_DSP_IDLECT1_IDLTIM_DSP, IDLE_STOP),
                                             CSL_FMKT(CLKRST_DSP_IDLECT1_IDLPER_DSP, IDLE_STOP),
                                             CSL_FMKT(CLKRST_DSP_IDLECT1_IDLXORP_DSP, IDLE_STOP),
                                             CSL_FMKT(CLKRST_DSP_IDLECT1_IDLWDT_DSP, IDLE_STOP)
                                           };

        for (i = 0; i < 4; i++) {
                if (ckIdleEntry & (1 << i)) {
                        _dsp_idlect1 |= clkSrcToIdleEntryLookup[i];
                }
        }

        hClkrst->regs->DSP_IDLECT1 = _dsp_idlect1;

}

inline void _CSL_clkrstClkSource(
                CSL_ClkrstHandle        hClkrst,
                Uint16                  clkSource
                )
{

        CSL_FINS(hClkrst->regs->DSP_CKCTL, CLKRST_DSP_CKCTL_TIMXO, CSL_FEXT(clkSource, CLKRST_DSP_CKCTL_TIMXO));

}

inline void _CSL_clkrstClkDiv(
                CSL_ClkrstHandle  hClkrst,
                CSL_ClkrstClkDiv  *clkDivPtr
                )
{

        CSL_FINS(hClkrst->regs->DSP_CKCTL, CLKRST_DSP_CKCTL_PERDIV, clkDivPtr->perDiv);

}

#endif
