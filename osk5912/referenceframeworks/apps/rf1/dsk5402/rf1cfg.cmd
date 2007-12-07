/*   Do *not* directly modify this file.  It was    */
/*   generated by the Configuration Tool; any  */
/*   changes risk being overwritten.                */

/* INPUT rf1.cdb */

/* MODULE PARAMETERS */
GBL_USERINITFXN = _FXN_F_nop;

MEM_SEGZERO = MEM_NULL;
MEM_MALLOCSEG = MEM_NULL;

CLK_TIMEFXN = CLK_F_getshtime;
CLK_HOOKFXN = CLK_F_rete;

PRD_THOOKFXN = FXN_F_nop;

RTDX_DATAMEMSEG = IDATA;

HST_DSMBUFSEG = IDATA;

SWI_EHOOKFXN = GBL_NULL;
SWI_IHOOKFXN = GBL_NULL;
SWI_EXECFXN = FXN_F_nop;
SWI_RUNFXN = FXN_F_nop;

TSK_STACKSEG = MEM_NULL;
TSK_VCREATEFXN = _FXN_F_nop;
TSK_VDELETEFXN = _FXN_F_nop;
TSK_VEXITFXN = _FXN_F_nop;

IDL_CALIBRFXN = GBL_NULL;

SYS_ABORTFXN = _UTL_halt;
SYS_ERRORFXN = _UTL_halt;
SYS_EXITFXN = _UTL_halt;
SYS_PUTCFXN = _FXN_F_nop;

GIO_CREATEFXN = _FXN_F_nop;
GIO_DELETEFXN = _FXN_F_nop;
GIO_PENDFXN = _FXN_F_nop;
GIO_POSTFXN = _FXN_F_nop;

/* OBJECT ALIASES */
_USERREGS = USERREGS;
_BIOSREGS = BIOSREGS;
_CSLREGS = CSLREGS;
_VECT = VECT;
_IDATA = IDATA;
_IPROG = IPROG;
_EPROG = EPROG;
_EDATA = EDATA;
_IDATAOVL = IDATAOVL;
_IPROGOVL = IPROGOVL;
_PRGDUMMY = PRGDUMMY;
_HWI_RS = HWI_RS;
_HWI_NMI = HWI_NMI;
_HWI_SINT17 = HWI_SINT17;
_HWI_SINT18 = HWI_SINT18;
_HWI_SINT19 = HWI_SINT19;
_HWI_SINT20 = HWI_SINT20;
_HWI_SINT21 = HWI_SINT21;
_HWI_SINT22 = HWI_SINT22;
_HWI_SINT23 = HWI_SINT23;
_HWI_SINT24 = HWI_SINT24;
_HWI_SINT25 = HWI_SINT25;
_HWI_SINT26 = HWI_SINT26;
_HWI_SINT27 = HWI_SINT27;
_HWI_SINT28 = HWI_SINT28;
_HWI_SINT29 = HWI_SINT29;
_HWI_SINT30 = HWI_SINT30;
_HWI_INT0 = HWI_INT0;
_HWI_INT1 = HWI_INT1;
_HWI_INT2 = HWI_INT2;
_HWI_TINT = HWI_TINT;
_HWI_SINT4 = HWI_SINT4;
_HWI_SINT5 = HWI_SINT5;
_HWI_SINT6 = HWI_SINT6;
_HWI_SINT7 = HWI_SINT7;
_HWI_SINT8 = HWI_SINT8;
_HWI_SINT9 = HWI_SINT9;
_HWI_SINT10 = HWI_SINT10;
_HWI_SINT11 = HWI_SINT11;
_HWI_SINT12 = HWI_SINT12;
_HWI_SINT13 = HWI_SINT13;
_HWI_SINT14 = HWI_SINT14;
_HWI_SINT15 = HWI_SINT15;
_audioIdl = audioIdl;
_LOG_system = LOG_system;
_trace = trace;
_pipRx = pipRx;
_pipTx = pipTx;
_audioProcessExecTime = audioProcessExecTime;

/* MODULE GBL */

SECTIONS {
   .vers (COPY): {} /* version information */
}

-priority
-llnknone.a54
-ldrivers.a54          /* device drivers support */
-lsioboth.a54          /* supports both SIO models */
-lbios.a54             /* DSP/BIOS support */
-lrtdx.lib             /* RTDX support */
-lcsl5402.lib
-lrts.lib              /* C and C++ run-time library support */

/* MODULE MEM */
-stack 0x240
MEMORY {
   PAGE 1:   USERREGS:   origin = 0x60,          len = 0x1a
   PAGE 1:   BIOSREGS:   origin = 0x7c,          len = 0x4
   PAGE 1:   CSLREGS:    origin = 0x7a,          len = 0x2
   PAGE 0:   VECT:       origin = 0xff80,        len = 0x80
   PAGE 1:   IDATA:      origin = 0x80,          len = 0xf80
   PAGE 0:   IPROG:      origin = 0x2000,        len = 0x2000
   PAGE 0:   EPROG:      origin = 0x8000,        len = 0x7f80
   PAGE 1:   EDATA:      origin = 0x8000,        len = 0x8000
   PAGE 1:   IDATAOVL:   origin = 0x1000,        len = 0x1000
   PAGE 0:   IPROGOVL:   origin = 0x1000,        len = 0x1000
   PAGE 0:   PRGDUMMY:   origin = 0x4000,        len = 0x1
}
/* MODULE CLK */
SECTIONS {
   .clk: {
        _CLK_start = _CLK_startNOPRSC;
        CLK_F_isr = CLK_F_isr54x;
        CLK_F_gethtime = CLK_F_getshtime;
        CLK_A_TABBEG = .;
        /* no CLK objs */
        CLK_A_TABEND = .;
        CLK_A_TABLEN = (. - CLK_A_TABBEG) / 1;
   } > IDATA PAGE 1
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
   .prd: {
        PRD_A_TABBEG = .;
        /* no PRD objects */
        PRD_A_TABEND = .;
        PRD_A_TABLEN = (. - PRD_A_TABBEG) / 8;
   } > IDATA PAGE 1
}

/* MODULE HWI */
SECTIONS {
   .hwi_vec: {
        GBL_F_chip = GBL_F_chip54x;
   } > VECT PAGE 0
   .hwi: {
       /* no HWI stubs are necessary */
   } > IPROG PAGE 0

}

/* MODULE SWI */
SECTIONS {
   .swi: {
        SWI_A_TABBEG = .;
        /* no SWI objects */
        SWI_A_TABEND = .;
        SWI_A_TABLEN = (. - SWI_A_TABBEG) / 11;
   } > IDATA PAGE 1
}

/* MODULE TSK */
_KNL_swi = 0;
/* MODULE IDL */
SECTIONS {
   .idl: {
        IDL_A_TABBEG = .;
        *(.idl)
        IDL_A_TABEND = .;
        IDL_A_TABLEN = (. - IDL_A_TABBEG) / 2;
        IDL_A_CALBEG = .;
        *(.idlcal)
        IDL_A_CALEND = .;
        IDL_A_CALLEN = (. - IDL_A_CALBEG) / 2;
   } > IDATA PAGE 1
}



SECTIONS {
        .sysregs: {} > BIOSREGS PAGE 1
        
        .bss:     {} > IDATA PAGE 1
        
        .far:     {} > IDATA PAGE 1
        
        .sysdata: align = 128 {
            GBL_A_SYSPAGE = .;
            GBL_A_SYSDP = GBL_A_SYSPAGE >> 7;
             } > IDATA PAGE 1
        
        .mem: 	  {} > IDATA PAGE 1
        
        .sys:     {} > IDATA PAGE 1
        
        .args: fill=0 {
            *(.args)
            . += 0x4;
        } > IDATA PAGE 1
        
        .gio:     {} > IDATA PAGE 1
        
        .devtable: {} > IDATA PAGE 1
        
        .csldata: {
            *(.csldata)
        } > IDATA PAGE 1
        
        .sts: {
             STS_A_TABBEG = .;
            _STS_A_TABBEG = .;
            *(.sts)
            STS_A_TABEND = .;
            _STS_A_TABEND = .;
             STS_A_TABLEN = (. - _STS_A_TABBEG) / 8;
            _STS_A_TABLEN = (. - _STS_A_TABBEG) / 8;
        } > IDATA PAGE 1
        
        .pip: {
             PIP_A_TABBEG = .;
            _PIP_A_TABBEG = .;
            *(.pip)
            PIP_A_TABEND = .;
            _PIP_A_TABEND = .;
             PIP_A_TABLEN = (. - _PIP_A_TABBEG) / 25;
            _PIP_A_TABLEN = (. - _PIP_A_TABBEG) / 25;
        } > IDATA PAGE 1
        
        .data:    {} > IDATA PAGE 1
        
        .const:    {} > IDATA PAGE 1
        
        /* trace buffer */
        .trace$buf: align = 0x20 fill = 0xffff {} > IDATA PAGE 1
        
        .cio:     {} > IDATA PAGE 1
        
        /* LOG_system buffer */
        .LOG_system$buf: align = 0x20 fill = 0xffff {} > IDATA PAGE 1
        
        .udev: {} > IDATA PAGE 1
        
        .printf (COPY): {} > IDATA PAGE 1
        
        .stack: fill=0xbeef {
            GBL_stackbeg = .;
            *(.stack)
            GBL_stackend = ((GBL_stackbeg + 0x240 - 1) & 0xfffe) ;
        _HWI_STKBOTTOM = GBL_stackend;
            _HWI_STKTOP = GBL_stackbeg;
        } > IDATA PAGE 1
        
        .hst: {
             HST_A_TABBEG = .;
            _HST_A_TABBEG = .;
            /* no HST objects */
            HST_A_TABEND = .;
            _HST_A_TABEND = .;
             HST_A_TABLEN = (. - _HST_A_TABBEG) / 5;
            _HST_A_TABLEN = (. - _HST_A_TABBEG) / 5;
        } > IDATA PAGE 1
        
        .dsm: {} > IDATA PAGE 1
        
        .log: {
             LOG_A_TABBEG = .;
            _LOG_A_TABBEG = .;
            *(.log)
            LOG_A_TABEND = .;
            _LOG_A_TABEND = .;
             LOG_A_TABLEN = (. - _LOG_A_TABBEG) / 6;
            _LOG_A_TABLEN = (. - _LOG_A_TABBEG) / 6;
        } > IDATA PAGE 1
        
        /* pipRx buffer */
        .pip0: align = 0x2 {} > IDATAOVL PAGE 1
        
        /* pipTx buffer */
        .pip1: align = 0x2 {} > IDATAOVL PAGE 1
        
        .pinit:   {} > IPROG PAGE 0
        
        .switch:    {} > IPROG PAGE 0
        
        .text:    {} > IPROG PAGE 0
        
        .bios:    {} > IPROG PAGE 0
        
        .cinit:    {} > IPROG PAGE 0
        
        .sysinit:    {} > IPROG PAGE 0
        
        .gblinit:    {} > IPROG PAGE 0
        
        .trcdata:    {} > IPROG PAGE 0
        
        frt:    {} > IPROG PAGE 0
        
}

