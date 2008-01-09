
    .state32


    .sect ".boot"
    .global _reset

_reset:
    ldr     pc,     addr_onReset
    ldr     pc,     addr_onUndefined
    ldr     pc,     addr_onSWI
    ldr     pc,     addr_onPrefetchAbort
    ldr     pc,     addr_onDataAbort
    ldr     pc,     addr_onUndefined           ; unassigned vector location
    ldr     pc,     addr_onIRQ
    ldr     pc,     addr_onFIQ

    .global   _onReset
    .global   _onUndefined
    .global   _onSWI
    .global   _onPrefetchAbort
    .global   _onDataAbort
    .global   _onIRQ
    .global   _onFIQ
    
    .global   _svcStack
    .global   _svcStackSize
    
    .global   _init

addr_onReset           .word   _onReset
addr_onUndefined       .word   _onUndefined
addr_onSWI             .word   _onSWI
addr_onPrefetchAbort   .word   _onPrefetchAbort
addr_onDataAbort       .word   _onDataAbort
addr_onIRQ             .word   _onIRQ
addr_onFIQ             .word   _onFIQ

addr_init              .word   _init

addr_svcStack          .word   _svcStack
addr_svcStackSize      .word   _svcStackSize

_onReset:
    mrs     r0, cpsr
    bic     r0, r0, #0x1F
    orr     r0, r0, #0x13
    msr     cpsr, r0
    ldr     sp, addr_svcStack
    ldr     r0, addr_svcStackSize     ; size of the stack
    add     sp, sp, r0
    ldr     pc, addr_init


;_onReset:
;    mrs     r0, cpsr
;    bic     r0, r0, #0x1F
;    orr     r0, r0, #0x13
;    msr     cpsr, r0
;    ldr     sp, addr_svcStack
;    ldr     r0, addr_svcStackSize     ; size of the stack
;    add     sp, sp, r0
;    ldr     pc, addr_init
