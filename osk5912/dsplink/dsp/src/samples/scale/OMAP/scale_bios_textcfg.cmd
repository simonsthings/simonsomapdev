/*   Do *not* directly modify this file.  It was    */
/*   generated by the Configuration Tool; any  */
/*   changes risk being overwritten.                */

/* INPUT scale_bios_text.cdb */

/* MODULE PARAMETERS */
GBL_USERINITFXN = _FXN_F_nop;
GBL_BIOS_BOOTFXN = _FXN_F_nop;

MEM_SEGZERO = DARAM;
MEM_MALLOCSEG = DARAM;

CLK_TIMEFXN = CLK_F_getshtime32;
CLK_HOOKFXN = CLK_F_run;

PRD_THOOKFXN = KNL_tick_stub;

RTDX_DATAMEMSEG = DARAM;

HST_DSMBUFSEG = DARAM;

SWI_EHOOKFXN = GBL_NULL;
SWI_IHOOKFXN = GBL_NULL;
SWI_EXECFXN = SWI_F_exec;
SWI_RUNFXN = SWI_F_run;

TSK_STACKSEG = DARAM;
TSK_VCREATEFXN = _FXN_F_nop;
TSK_VDELETEFXN = _FXN_F_nop;
TSK_VEXITFXN = _FXN_F_nop;

IDL_CALIBRFXN = GBL_NULL;

SYS_ABORTFXN = _UTL_doAbort;
SYS_ERRORFXN = _UTL_doError;
SYS_EXITFXN = _UTL_halt;
SYS_PUTCFXN = _UTL_doPutc;

GIO_CREATEFXN = _FXN_F_nop;
GIO_DELETEFXN = _FXN_F_nop;
GIO_PENDFXN = _FXN_F_nop;
GIO_POSTFXN = _FXN_F_nop;

/* OBJECT ALIASES */
_DARAM = DARAM;
_SARAM = SARAM;
_PDROM = PDROM;
_VECT = VECT;
_RESET_VECTOR = RESET_VECTOR;
_SHMMEM = SHMMEM;
_EXTMEM = EXTMEM;
_PRD_clock = PRD_clock;
_HWI_RESET = HWI_RESET;
_HWI_NMI = HWI_NMI;
_HWI_INT2 = HWI_INT2;
_HWI_INT3 = HWI_INT3;
_HWI_INT4 = HWI_INT4;
_HWI_INT5 = HWI_INT5;
_HWI_INT6 = HWI_INT6;
_HWI_INT7 = HWI_INT7;
_HWI_INT8 = HWI_INT8;
_HWI_INT9 = HWI_INT9;
_HWI_INT10 = HWI_INT10;
_HWI_INT11 = HWI_INT11;
_HWI_INT12 = HWI_INT12;
_HWI_INT13 = HWI_INT13;
_HWI_INT14 = HWI_INT14;
_HWI_INT15 = HWI_INT15;
_HWI_INT16 = HWI_INT16;
_HWI_INT17 = HWI_INT17;
_HWI_INT18 = HWI_INT18;
_HWI_INT19 = HWI_INT19;
_HWI_INT20 = HWI_INT20;
_HWI_INT21 = HWI_INT21;
_HWI_INT22 = HWI_INT22;
_HWI_INT23 = HWI_INT23;
_HWI_BERR = HWI_BERR;
_HWI_DLOG = HWI_DLOG;
_HWI_RTOS = HWI_RTOS;
_HWI_SINT27 = HWI_SINT27;
_HWI_SINT28 = HWI_SINT28;
_HWI_SINT29 = HWI_SINT29;
_HWI_SINT30 = HWI_SINT30;
_HWI_SINT31 = HWI_SINT31;
_KNL_swi = KNL_swi;
_TSK_idle = TSK_idle;
_LOG_system = LOG_system;
_trace = trace;

/* MODULE GBL */

SECTIONS {
   .vers block(0x20000) (COPY): {} /* version information */
}

-priority
-llnkrtdx.a55l
-ldrivers.a55l         /* device drivers support */
-lsioboth.a55l         /* supports both SIO models */
-lbios.a55l            /* DSP/BIOS support */
-lrtdxx.lib            /* RTDX JTAG support */
-lrts55x.lib           /* C and C++ run-time library support */
-lclk.a55l

/* MODULE MEM */
-stack 0x800
-sysstack 0x200

SECTIONS { .sysstack : block(0x20000) fill = 0xfeeb {
           GBL_sysstackbeg = .;
           *(.sysstack)
           GBL_sysstackend = GBL_sysstackbeg + 0x200 -1 ;
            _HWI_SYSSTKTOP = GBL_sysstackbeg; 
            _HWI_SYSSTKBOTTOM =
(GBL_sysstackend+1);
       } > DARAM}

MEMORY {
   DARAM:      origin = 0x200,         len = 0xfe00
   SARAM:      origin = 0x101fe,       len = 0x17e00
   PDROM:      origin = 0xff8000,      len = 0x8000
   VECT:       origin = 0x100,         len = 0x100
   RESET_VECTOR: origin = 0x10000,     len = 0x1fe
   SHMMEM:     origin = 0x400000,      len = 0x100000
   EXTMEM:     origin = 0x600000,      len = 0x100000
}
/* MODULE CLK */
SECTIONS {
   .clk: block(0x20000){
        _CLK_start = FXN_F_nop;
        CLK_F_gethtime = CLK_F_getshtime32;
        CLK_A_TABBEG = .;
        *(.clk)
        CLK_A_TABEND = .;
        CLK_A_TABLEN = (. - CLK_A_TABBEG) / 2;
   } > DARAM 
}
_CLK_PRD = CLK_PRD;
_CLK_COUNTSPMS = CLK_COUNTSPMS;
_CLK_REGS = CLK_REGS;
_CLK_USETIMER = CLK_USETIMER;
_CLK_TIMERNUM = CLK_TIMERNUM;
_CLK_TCR = CLK_TCR;
_CLK_TDDR = CLK_TDDR;

/* MODULE PRD */
SECTIONS {
   .prd: block(0x20000){
        PRD_A_TABBEG = .;
        /* no PRD objects */
        PRD_A_TABEND = .;
        PRD_A_TABLEN = (. - PRD_A_TABBEG) / 20;
   } > DARAM
}

/* MODULE RTDX */
_RTDX_interrupt_mask = 0x0;

/* MODULE HWI */
HWI_TINT = HWI_INT23;
_HWI_TINT = HWI_TINT;
/* MODULE SWI */
SECTIONS {
   .swi: block(0x20000){
        SWI_A_TABBEG = .;
        *(.swi)
        SWI_A_TABEND = .;
        SWI_A_TABLEN = (. - SWI_A_TABBEG) / 22;
   } > DARAM
}

/* MODULE IDL */
SECTIONS {
   .idl: block(0x20000){
        IDL_A_TABBEG = .;
        *(.idl)
        IDL_A_TABEND = .;
        IDL_A_TABLEN = (. - IDL_A_TABBEG) / 4;
        IDL_A_CALBEG = .;
        *(.idlcal)
        IDL_A_CALEND = .;
        IDL_A_CALLEN = (. - IDL_A_CALBEG) / 4;
   } > DARAM
}



SECTIONS {
        .bss: block(0x20000)    {} > DARAM
        
        .hwi_disp_sec:     {} > DARAM
        
        .far:     {} > DARAM
        
        .sysdata: block(0x20000) {} > DARAM
        
        .gio: block(0x20000)    {} > DARAM
        
        .sysregs: {} > DARAM
        
        .udev: block(0x20000){} > DARAM
        
        .mem: 	  {} > DARAM
        
        .devtable: block(0x20000){} > DARAM
        
        .trace: block(0x20000) fill = 0x0 {
           _SYS_PUTCBEG = .;
           . += 0x200;
           _SYS_PUTCEND = . - 1;
        } > DARAM
        
        .sys: block(0x20000)    {} > DARAM
        
        .data: block(0x20000)  {} > DARAM
        
        .const:    {} > DARAM
        
        .sts: {
             STS_A_TABBEG = .;
            _STS_A_TABBEG = .;
            /* no STS objects */
            STS_A_TABEND = .;
            _STS_A_TABEND = .;
             STS_A_TABLEN = (. - _STS_A_TABBEG) / 16;
            _STS_A_TABLEN = (. - _STS_A_TABBEG) / 16;
        } > DARAM
        
        .cio:     {} > DARAM
        
        .pip: {
             PIP_A_TABBEG = .;
            _PIP_A_TABBEG = .;
            /* no PIP objects */
            PIP_A_TABEND = .;
            _PIP_A_TABEND = .;
             PIP_A_TABLEN = (. - _PIP_A_TABBEG) / 100;
            _PIP_A_TABLEN = (. - _PIP_A_TABBEG) / 100;
        } > DARAM
        
        /* trace buffer */
        .trace$buf: block(0x20000) align = 0x40 fill = 0xffff {} > DARAM
        
        /* LOG_system buffer */
        .LOG_system$buf: block(0x20000) align = 0x80 fill = 0xffff {} > DARAM
        
        .stack: block(0x20000) fill=0xbeef {
            GBL_stackbeg = .;
            *(.stack)
            GBL_stackend = (GBL_stackbeg + 0x800 - 1) ;
            _HWI_STKBOTTOM = (GBL_stackend+1);
            _HWI_STKTOP = (GBL_stackbeg);
        } > DARAM
        
        .printf (COPY): {} > DARAM
        
        .rtdx_data: block(0x20000){}  > DARAM
        
        .log: {
             LOG_A_TABBEG = .;
            _LOG_A_TABBEG = .;
            *(.log)
            LOG_A_TABEND = .;
            _LOG_A_TABEND = .;
             LOG_A_TABLEN = (. - _LOG_A_TABBEG) / 20;
            _LOG_A_TABLEN = (. - _LOG_A_TABBEG) / 20;
        } > DARAM
        
        .hst: {
             HST_A_TABBEG = .;
            _HST_A_TABBEG = .;
            /* no HST objects */
            HST_A_TABEND = .;
            _HST_A_TABEND = .;
             HST_A_TABLEN = (. - _HST_A_TABBEG) / 8;
            _HST_A_TABLEN = (. - _HST_A_TABBEG) / 8;
        } > DARAM
        
        .dsm: block(0x20000) {} > DARAM
        
        .TSK_idle$stk: block(0x20000){
            *(.TSK_idle$stk)
        } > DARAM
        
        .tsk: block(0x20000){
            *(.tsk)
        } > DARAM
        
        .args: align = 0x4  fill=0 block(0x20000) {
            *(.args)
            . += 0x64;
        } > DARAM
        
        .dio: {} > DARAM
        
        .DARAM$heap: {
            DARAM$B = .;
            _DARAM_base = .;
            DARAM$L = 0xa800;
            _DARAM_length = 0xa800;
            . += 0xa800;
        } > DARAM
        
        frt:    {} > EXTMEM
        
        .bios:    {} > EXTMEM
        
        .text:    {} > EXTMEM
        
        .rtdx_text: {}  > SARAM
        
        .hwi:  {}  > SARAM
        
        .switch:    {} > SARAM
        
        .pinit: block(0x20000)    {} > SARAM
        
        .cinit: block(0x20000)    {} > SARAM
        
        .sysinit:    {} > SARAM
        
        .gblinit: block(0x20000)   {} > SARAM
        
        .trcdata:    {} > SARAM
        
        .hwi_vec:  align = 0x100{
            HWI_A_VECS = .;
           _HWI_A_VECS = . ; 
           *(.hwi_vec)
        } > VECT
        
}
