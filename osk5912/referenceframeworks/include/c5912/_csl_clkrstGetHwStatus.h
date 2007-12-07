#ifndef __CSL_CLKRSTGETHWSTATUS_H_
#define __CSL_CLKRSTGETHWSTATUS_H_

#include <csl_clkrst.h>

inline void _CSL_clkrstGetCkEn(
                CSL_ClkrstHandle        hClkrst,
                Uint16                  *ckEnable
                )
{

        int i;
        Uint16 _dsp_idlect2 = 0;
        Uint16 clkEnToClkSrcLookup[] = { CSL_FMKT(CLKRST_DSP_IDLECT2_EN_TIMCK, ACTIVE),
                                         CSL_FMKT(CLKRST_DSP_IDLECT2_EN_PERCK, ACTIVE),
                                         CSL_FMKT(CLKRST_DSP_IDLECT2_EN_XORPCK, ACTIVE),
                                         CSL_FMKT(CLKRST_DSP_IDLECT2_EN_WDTCK, ACTIVE)
                                       };

        _dsp_idlect2 = hClkrst->regs->DSP_IDLECT2;
        *ckEnable = 0;

        for (i = 0; i < 4; i++) {
                if (_dsp_idlect2 & clkEnToClkSrcLookup[i]) {
                        *ckEnable |= (1 << i);
                }
        }

}

inline void _CSL_clkrstGetStatus(
                CSL_ClkrstHandle        hClkrst,
                CSL_ClkrstStatus        *status
                )
{

        Uint16 _dsp_sysst;

        _dsp_sysst = hClkrst->regs->DSP_SYSST;
        hClkrst->regs->DSP_SYSST = 0;

        status->clkScheme = (CSL_ClkrstClkScheme)CSL_FEXT(_dsp_sysst, CLKRST_DSP_SYSST_CLOCK_SELECT);
        status->status    = _dsp_sysst & 0x3F;
        status->mpuStatus = (CSL_ClkrstMpuStatus)CSL_FEXT(_dsp_sysst, CLKRST_DSP_SYSST_IDLE_ARM);

}

#endif
