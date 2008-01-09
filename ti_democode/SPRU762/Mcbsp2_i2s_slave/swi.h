#ifndef _SWI_H_
#define _SWI_H_

#include <csl_chip.h>

#define SWI_SYSMODE (0xBEEF)
#define SWI_HELLO   (0x07734)

#define SWI_READSPSR  (0xAAAA)
#define SWI_WRITESPSR (0xBBBB)

#pragma SWI_ALIAS(SWI_sysMode, SWI_SYSMODE)
int SWI_sysMode (void);

#pragma SWI_ALIAS(SWI_hello, SWI_HELLO)
int SWI_hello(void);

#pragma SWI_ALIAS(SWI_chipReadSPSR, SWI_READSPSR)
int SWI_chipReadSPSR(void);

#pragma SWI_ALIAS(SWI_chipWriteSPSR, SWI_WRITESPSR)
int SWI_chipWriteSPSR(Uint32);

#endif
