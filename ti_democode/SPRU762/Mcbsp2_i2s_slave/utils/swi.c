/* #include <csl_chip.h> */
#include "swi.h"

void swiSysMode(void)
{
  asm(" .state32");
  asm(" mrs r0, spsr");
  asm(" orr  r0, r0, #0x1f ; SYS mode");
  asm(" msr spsr, r0");
}

int swiService_hello(void)
{
    printf("hello!\n");
    return 0xdeadbeef;
}

int
    SWI_dispatcher (
        int serviceNum,
        int arg0,
        int arg1,
        int arg2
)
{
    /* printf("SWI #0x%.8X\n", serviceNum); */
    switch (serviceNum) {
    case SWI_SYSMODE:
        swiSysMode();
        break;
    
    case SWI_READSPSR:
    	/* return CSL_chipReadReg(CSL_CHIP_REG_SPSR); */
    	return 0;
    case SWI_WRITESPSR:
        /* return CSL_chipWriteReg(CSL_CHIP_REG_SPSR, arg0); */
        return 0;        
    case SWI_HELLO:
        return swiService_hello();
    default:
        break;
    }
    return -1;
}
