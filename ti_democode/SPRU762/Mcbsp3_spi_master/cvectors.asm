; CVECTORS.ASM SOLUTION
; Eric Wilbur
; 1/5/2000 
 
		.def _rsv
		.ref _c_int00
		
		.sect ".vectors"
		
_rsv:	.ivec	_c_int00

NMI:    .ivec  1
	
INT0:   .ivec  2

INT2:   .ivec  3   		;fir ISR

TINT0:  .ivec  4

RINT0:  .ivec  5

RINT1:  .ivec  6

XINT1:  .ivec  7

LCKINT: .ivec  8

DMAC1:  .ivec  9

DSPINT: .ivec 10

INT3:   .ivec 11

UART:   .ivec 12 ;_UART_ISRHandler

XINT2:  .ivec 13

DMAC4:  .ivec 14

DMAC5:  .ivec 15

INT1:   .ivec 16

XINT0:  .ivec 17

DMAC0:  .ivec 18

INT4:   .ivec 19

DMAC2:  .ivec 20

DMAC3:  .ivec 21

TINT1:  .ivec 22

INT5:   .ivec 23

