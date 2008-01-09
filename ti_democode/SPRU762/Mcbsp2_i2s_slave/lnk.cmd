
-e _reset                                  /* entry-point = _reset */
-c                                         /* LINK USING C CONVENTIONS      */
-stack  0x1000                             /* SOFTWARE STACK SIZE           */
-heap   0x1000                             /* HEAP AREA SIZE                */
-l rts32e.lib                              /* GET RUN-TIME SUPPORT          */

/* SPECIFY THE SYSTEM MEMORY MAP */

MEMORY
{
    BOOT_MEM : org = 0x00000000   len = 0x00001000  /* map over exception vectors */
    P_MEM    : org = 0x00001000   len = 0x0002F000  /* PROGRAM MEMORY (ROM) */
    D_MEM    : org = 0x00030000   len = 0x00050000  /* DATA MEMORY    (RAM) */
            INT_D_MEM : org = 0x2000b000   len = 0x00001000  /* DATA MEMORY    (IRAM) */
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */
SECTIONS
{
    .boot   : {} > BOOT_MEM            /* Exception VECTORS start at 0x00000000  */
    .bss     : {} > D_MEM              /* GLOBAL & STATIC VARS              */
    .sysmem  : {} > D_MEM              /* DYNAMIC MEMORY ALLOCATION AREA    */
    .stack   : {} > D_MEM              /* SOFTWARE SYSTEM STACK             */
    .text    : {} > P_MEM              /* CODE                              */
    .cinit   : {} > P_MEM              /* INITIALIZATION TABLES             */
    .const   : {} > P_MEM              /* CONSTANT DATA                     */
    .pinit   : {} > P_MEM              /* TEMPLATE INSTANTIATION TABLES     */
    mydata         : {} > INT_D_MEM

}

