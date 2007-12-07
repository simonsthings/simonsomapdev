#ifndef _CSL_CLKRSTGETHWSETUP_H_
#define _CSL_CLKRSTGETHWSETUP_H_

#include <csl_clkrst.h>

inline void _CSL_clkrstGetClkIdleEntry(
                CSL_ClkrstHandle        hClkrst,
                Uint16                  *ckIdleEntry
                )
{

        int i;
        Uint16 _dsp_idlect1;

        Uint16 clkSrcToIdleEntryLookup[] = { CSL_FMKT(CLKRST_DSP_IDLECT1_IDLTIM_DSP, IDLE_STOP),
                                             CSL_FMKT(CLKRST_DSP_IDLECT1_IDLPER_DSP, IDLE_STOP),
                                             CSL_FMKT(CLKRST_DSP_IDLECT1_IDLXORP_DSP, IDLE_STOP),
                                             CSL_FMKT(CLKRST_DSP_IDLECT1_IDLWDT_DSP, IDLE_STOP)
                                           };

        _dsp_idlect1 = hClkrst->regs->DSP_IDLECT1;
        *ckIdleEntry = 0;

        for (i = 0; i < 4; i++) {
                if (_dsp_idlect1 & clkSrcToIdleEntryLookup[i]) {
                        *ckIdleEntry |= (1 << i);
                }
        }


}

inline void _CSL_clkrstGetClkSource(
                CSL_ClkrstHandle        hClkrst,
                Uint16                  *clkSource
                )
{

        *clkSource = hClkrst->regs->DSP_CKCTL &
                     (CSL_CLKRST_DSP_CKCTL_TIMXO_MASK);

}

inline void _CSL_clkrstGetClkDiv(
                CSL_ClkrstHandle  hClkrst,
                CSL_ClkrstClkDiv  *clkDivPtr
                )
{

        clkDivPtr->perDiv = CSL_FEXT(hClkrst->regs->DSP_CKCTL, CLKRST_DSP_CKCTL_PERDIV);

}

#endif
