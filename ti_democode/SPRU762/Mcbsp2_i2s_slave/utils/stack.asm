
    .text
    .state32

    .global _setupStack

CPSR_MODE_MASK     .equ     0x0000001F  ; the MODE bits [4:0] in PSR
CPSR_IRQ_MASK      .equ     0x00000080  ; the IRQ bit   [7]   in PSR
CPSR_FIQ_MASK      .equ     0x00000040  ; the FIQ bit   [6]   in PSR

****************************************************************************
* void  setupStack(
*     ARM_Mode                  mode,
*     void                      *stack_pointer
* )
****************************************************************************

_setupStack:
    mrs  r2, cpsr
    mov  r3, r2
    bic  r2, r2, #CPSR_MODE_MASK  ; clear the mode bits
    orr  r2, r2, r0              ; modify the mode bits to
    msr  cpsr, r2                ; switch to specified mode
    mov  sp, r1                  ; setup stack pointer
    msr  cpsr, r3                ; restore CPSR
    bx   lr

    .state16
    .global $setupStack
$setupStack:
    bx  pc
    nop
    .state32
    b   _setupStack

