#ifndef _ARM_H_
#define _ARM_H_

/* ARM processor modes */
typedef enum {
    ARM_MODE_USR  = 16, /* user mode */
    ARM_MODE_FIQ  = 17, /* FIQ mode */
    ARM_MODE_IRQ  = 18, /* IRQ mode */
    ARM_MODE_SVC  = 19, /* supervisor mode */
    ARM_MODE_ABT  = 23, /* abort mode */
    ARM_MODE_UND  = 27, /* undefined mode */
    ARM_MODE_SYS  = 31 /* system mode */
} ARM_Mode;

typedef enum {
    ARM_EXCP_RESET      = 0,
    ARM_EXCP_UNDEF      = 4,
    ARM_EXCP_SWI        = 8,
    ARM_EXCP_PREABT     = 12,
    ARM_EXCP_DATABT     = 16,
    ARM_EXCP_IRQ        = 24,
    ARM_EXCP_FIQ        = 28
} ARM_Excp;

#endif

