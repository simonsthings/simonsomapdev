
    .state32

    .text

    .global _onSWI
    .global _SWI_dispatcher

_onSWI:
    ;stmfd   sp!,    {r0-r3, r12, lr}
    stmfd   sp!,    {lr}

    ; shift the parameters to the right
    ; the first parameter would be the SWI number
    mov     r3,     r2
    mov     r2,     r1
    mov     r1,     r0

    mrs     r0,     spsr
    tst     r0,     #0x20
    ldrneh  r0,     [lr, #-2]
    bicne   r0,     r0,     #0xff00
    ldreq   r0,     [lr, #-4]
    biceq   r0,     r0,     #0xff000000
    bl      _SWI_dispatcher

    ;ldmfd   sp!,    {r0-r3, r12, pc}^
    ldmfd   sp!,    {pc}^
