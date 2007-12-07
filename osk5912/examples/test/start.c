/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== start.c ========
 *  Inline assembly code processed before any C Code.  The reason is for
 *      set up important settings on the ARM side that requires assembly
 *      instructions, and does not interfere with the normal operations
 *      of the C Code.
 */

#include <osk5912.h>

/* ------------------------------------------------------------------------ *
 *  Entry Points                                                            *
 * ------------------------------------------------------------------------ */
asm("           .global _c_int00");         // Entry point into C Code
asm("           .global _start");           // Entry point into Assembly Code

/* ------------------------------------------------------------------------ *
 *  Reset Vectors                                                           *
 *      At locatation [0x20000000, 0x20000020] - SRAM on internal mem (CS6) *
 *                                                                          *
 *  NOTE:                                                                   *
 *      Must occupy this exact memory space b/c MMU is set to jump to this  *
 *      address in SRAM for the RESET VECTORS.                              *
 * ------------------------------------------------------------------------ */
asm("           .sect \".vector\" ");
    asm("           .space 0x20");          // Reserve Space

/* ------------------------------------------------------------------------ *
 *  Setup Stack                                                             *
 *      Settings Stacks for the different Modes on the ARM processor        *
 *                                                                          *
 *  Note:                                                                   *
 *      To modify the stack size, change to value reserved in the .space    *
 *      command.  Example:                                                  *
 *                                                                          *
 *       SVCSTK     .space 100 // Reserves 100 bytes                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
    /* Stacks Memory - Change .space [value] to modify size*/
    asm("SVCSTK     .space 0");             // Supervisor Stack Space
    asm("IRQSTK     .space 256");           // IRQ Stack Space
    asm("FIQSTK     .space 256");           // FIQ Stack Space
    asm("ABORTSTK   .space 0");             // Abort Data/Prefetch Stack Space
    asm("ENDSTK     .space 0");             // End of Stack Space ( always 0 )
    /* Stack Pointers */
    asm("SVCPTR     .long SVCSTK");         // Pointer to SVC Stack
    asm("IRQPTR     .long IRQSTK");         // Pointer to IRQ Stack
    asm("FIQPTR     .long FIQSTK");         // Pointer to FIQ Stack
    asm("ABTPTR     .long ABORTSTK");       // Pointer to ABORT Stack
    asm("ENDPTR     .long ENDSTK");         // Pointer to END of Stack
asm("_start:");
    /* Setup Stacks */
    asm("           MRS R5, CPSR");         // R5 = CPSR
    asm("           BIC R4, R5, #0x1f");    // R4 = R5 & ~0x1f Clear Bits
    /* Set SVC STACK */
    asm("           LDR R2, SVCPTR");       // R2 = SVCPTR
    asm("           LDR R1, IRQPTR");       // R1 = IRQPTR
    asm("           SUB R0, R1, R2");       // R0 = IRQPTR - SVCPTR
    asm("           ORR R3, R4, #0x13");    // R3 = R4 | 0x13 ( SVC mode )
    asm("           MSR CPSR, R3");         // CPSR = R3 ( Switch modes )
    asm("           ADD R2, R2, R0");       // R2 = STK_PTR + STK_SIZE
    asm("           MOV SP, R2");           // SP = R2 ( Set stack pointer )
    /* Set IRQ STACK */
    asm("           LDR R2, IRQPTR");       // R2 = IRQPTR
    asm("           LDR R1, FIQPTR");       // R1 = FIQPTR
    asm("           SUB R0, R1, R2");       // R0 = FIQPTR - IRQPTR
    asm("           ORR R3, R4, #0x12");    // R3 = R4 | 0x12 ( IRQ mode )
    asm("           MSR CPSR, R3");         // CPSR = R3 ( Switch modes )
    asm("           ADD R2, R2, R0");       // R2 = STK_PTR + STK_SIZE
    asm("           MOV SP, R2");           // SP = R2 ( Set stack pointer )
    /* Set FIQ STACK */
    asm("           LDR R2, FIQPTR");       // R2 = FIQPTR
    asm("           LDR R1, ABTPTR");       // R1 = ABTPTR
    asm("           SUB R0, R1, R2");       // R0 = ABTPTR - FIQPTR
    asm("           ORR R3, R4, #0x11");    // R3 = R4 | 0x11 ( FIQ mode )
    asm("           MSR CPSR, R3");         // CPSR = R3 ( Switch modes )
    asm("           ADD R2, R2, R0");       // R2 = STK_PTR + STK_SIZE
    asm("           MOV SP, R2");           // SP = R2 ( Set stack pointer )
    /* Set ABORT STACK */
    asm("           LDR R2, ABTPTR");       // R2 = ABTPTR
    asm("           LDR R1, ENDPTR");       // R1 = ENDPTR
    asm("           SUB R0, R1, R2");       // R0 = ENDPTR - ABTPTR
    asm("           ORR R3, R4, #0x17");    // R3 = R4 | 0x17 ( ABORT mode )
    asm("           MSR CPSR, R3");         // CPSR = R3 ( Switch modes )
    asm("           ADD R2, R2, R0");       // R2 = STK_PTR + STK_SIZE
    asm("           MOV SP, R2");           // SP = R2 ( Set stack pointer )
    /* Restore to original mode */
    asm("           MSR CPSR, R5");         // CPSR = R5 ( Restore CPSR )
    asm("           B   SET_MMU");          // Branch to MMU Code

/* ------------------------------------------------------------------------ *
 *  Setup MMU                                                               *
 *      Setup the Translation Table needed for the MMU.  The page will be   *
 *      0x1000 by 32-bit entries long and will reside in SDRAM at location  *
 *      [0x11FFC000 - 0x12000000].  Each entry maps a 1MB region of Virtual *
 *      Memory to a 1MB Physical Memory except for the RESET Vectors which  *
 *      are mapped to Interal SRAM.                                         *
 * ------------------------------------------------------------------------ */
    /* Translation Table Parameters */
    asm("TT_BASE    .long 0x11FFC000");     // Translation Table Base Addr
    asm("TT_ENTRY   .long 0x00000C12");     // Trans Table Entry allows for:
                                            //     Page Sections ( 1MB )
                                            //     Full Access permission
                                            //     NON  Caching
    asm("RESETVEC   .long 0x20000C12");     // Reset Vector TT Entry
    asm("SDRAM_ET   .long 0x10000C1E");     // SDRAM is most effective at using Caching.
    asm("PERMISSN   .long 0xFFFFFFFF");     // MMU Domain Permissions
    asm("CNTLMASK   .long 0x0001");         // CP15 Cntl Reg Bit Mask
    asm("CNTLREG    .long 0x1300");         // CP15 Cntl Reg Value
                                            // 0x2000 enables Alt. Int Vector
                                            // 0x1000 enables ICache
                                            // 0x0200 enables ROM Protect
                                            // 0x0100 enables System Protect
                                            // 0x0008 enables Write buffer
                                            // 0x0004 enables DCache
                                            // 0x0002 enables Alignment check
                                            // 0x0001 enables MMU
asm("SET_MMU:");
/* ------------------------------------------------------------------------ *
 *  Pusedo C Code for building Translation Table (TT):                      *
 *                                                                          *
 *  R0 = TT_BASE;           <- R0 = TT_BASE                                 *
 *  R1 = TT_ENTRY;          <- R1 = TT_ENTRY = ( 0x00000C1E )               *
 *  R2 = TT_BASE + 0x4000;  <- R2 = TT_BASE + ( 0x1000 x 32-bit TT entries )*
 *                                                                          *
 *  do {                                                                    *
 *      *R0 = R1;           <- *TT_BASE = TT_ENTRY                          *
 *      R0 += 4;            <- TT_BASE++                                    *
 *      R1 += 0x00100000;   <- TT_ENTRY += 0x00100000                       *
 *  } while ( R0 != R2 );   <- while ( TT_BASE != ( TT_BASE + 0x4000 ) )    *
 *                                                                          *
 *                                                                          *
 *                                                                          *
 *  R0 = TT_BASE;           <- R0 = TT_BASE                                 *
 *  R0 = R0 + ( 0x400 );    <- R0 = SDRAM_TT_ENTRY                          *
 *  R1 = 0x00000C12;        <- R1 = SDRAM_ET                                *
 *  R2 = R0 + ( 0x40 );     <- R2 = SDRAM_TT_ENTRY + 0x40                   *
 *                                                                          *
 *  do {                                                                    *
 *      *R0 = R1;           <- *TT_BASE = TT_ENTRY                          *
 *      R0 += 4;            <- TT_BASE++                                    *
 *      R1 += 0x00100000;   <- TT_ENTRY += 0x00100000                       *
 *  } while ( R0 != R2 );   <- while ( TT_BASE != ( TT_BASE + 0x440 ) )     *
 *                                                                          *
 *                                                                          *
 *                                                                          *
 *  R0 = TT_BASE;           <- TT_BASE                                      *
 *  R0 = R0 + ( 0x0000 );   <- R0 = CS0_TT_ENTRY                            *
 *  R1 = RESETVEC;          <- RESETVEC  = 0x20000C1E;                      *
 *  R2 = R0 + ( 0x10 );     <- R2 = CS0_TT_ENTRY + 0x10                     *
 *                                                                          *
 *  do {                                                                    *
 *      *R0 = R1;           <- *TT_BASE = TT_ENTRY                          *
 *      R0 += 4;            <- TT_BASE++                                    *
 *      R1 += 0x00100000;   <- TT_ENTRY += 0x00100000                       *
 *  } while ( R0 != R2 );   <- while ( TT_BASE != ( TT_BASE + 0x440 ) )     *
 * ------------------------------------------------------------------------ */
	/* Fetch Parameters */
    asm("           LDR R0, TT_BASE");      // R0 = TT_BASE
    asm("           LDR R1, TT_ENTRY");     // R1 = TT_ENTRY
    asm("           ADD R2, R0, #0x4000");  // R2 = TT_BASE + 0x4000
    /* Set all TT entries */
    asm("LOOP       STR R1, [R0]");         // *R0 = R1
    asm("           ADD R0, R0, #0x4");     // R0 += 4
    asm("           ADD R1, R1, #0x100000");// R1 += 0x00100000
    asm("           CMP R0, R2");           // R0 =? R2
    asm("           BLT LOOP");             // Run until R0 == R2
    /* Fetch Special Parameters for SDRAM */
    asm("           LDR R0, TT_BASE");      // R0 = TT_BASE
    asm("           ADD R0, R0, #0x400");   // R0 = TT_BASE + 0x400 ( SDRAM )
    asm("           LDR R1, SDRAM_ET");     // R1 = SDRAM_ET
    asm("           ADD R2, R0, #0x40");    // R2 = TT_BASE + 0x400 + 0x40
    /* Set SDRAM TT entries */
    asm("LOOP2      STR R1, [R0]");         // *R0 = R1
    asm("           ADD R0, R0, #0x4");     // R0 += 4
    asm("           ADD R1, R1, #0x100000");// R1 += 0x00100000
    asm("           CMP R0, R2");           // R0 =? R2
    asm("           BLT LOOP2");            // Run until R0 == R2
    /* Fetch Special Parameters for RESET Vector */
    asm("           LDR R0, TT_BASE");      // R0 = TT_BASE
    asm("           ADD R0, R0, #0");       // R0 = TT_BASE + 0 ( CS0 )
    asm("           LDR R1, RESETVEC");     // R1 = RESETVEC
    asm("           ADD R2, R0, #0x10");    // R2 = TT_BASE + 0 + 0x10
    /* Set RESET Vector TT entries */
    asm("LOOP3      STR R1, [R0]");         // *R0 = R1
    asm("           ADD R0, R0, #0x4");     // R0 += 4
    asm("           ADD R1, R1, #0x100000");// R1 += 0x00100000
    asm("           CMP R0, R2");           // R0 =? R2
    asm("           BLT LOOP3");            // Run until R0 == R2
    /* Set Translation Table & Permissions */
    asm("           LDR R0, TT_BASE");          // R0 = TT_BASE
    asm("           LDR R1, PERMISSN");         // R1 = PERMISSN
    asm("           MCR p15, #0, R0, C2, C0");  // Set Translation Base Address
    asm("           MCR p15, #0, R1, C3, C0");  // Set Domain Permissions
    asm("           NOP");
    /* Enable MMU */
    asm("           LDR R1, CNTLMASK");         // R1 = CNTLMASK
    asm("           LDR R2, CNTLREG");          // R2 = CNTLREG
    /* Read, Clear, then Set the Control Register */
    asm("           MRC p15, #0, R0, C1, C0");  // Read Control register.
    asm("           BIC R0, R0, R1");           // R0 &= ~CNTLMASK
    asm("           ORR R0, R0, R2");           // R0 |= CNTLREG
    asm("           MCR p15, #0, R0, C1, C0");  // Write Control register.
    asm("           NOP");
    asm("           NOP");
    asm("           B   SET_INT");

/* ------------------------------------------------------------------------ *
 *  Setup Interrupts                                                        *
 *      Clear any pending Interrupts and Enables IRQ/FIQ                    *
 * ------------------------------------------------------------------------ */
    /* Interrupt Handler 1 Registers */
    asm("INT1_ITR   .long 0xFFFECB00");
    asm("INT1_MIR   .long 0xFFFECB04");
    asm("INT1_CTL   .long 0xFFFECB18");
    /* Interrupt Handler 2 Registers */
    asm("INT2_ITR   .long 0xFFFE0000");
    asm("INT2_MIR   .long 0xFFFE0004");
    asm("INT2_CTL   .long 0xFFFE0018");
    /* Interrupt Handler Values */
    asm("ITR_REG    .long 0x00000000");
    asm("MIR_REG    .long 0xFFFFFFFF");
    asm("CTL_REG    .long 0x00000003");     // [2: Request FIQ][1: Request IRQ]

asm("SET_INT:");
    /* Clear any pending Interrupts before enabling both IRQs and FIQs.
     * This will ensure that no interrupts will occur when running any user
     * code. */
    /* Set MIR to MASK any future interrupts from occuring */
    asm("           LDR R1, MIR_REG");
    asm("           LDR R0, INT1_MIR");
    asm("           STR R1, [R0]");         // *INT1_MIR   = 0xFFFFFFFF
    asm("           LDR R0, INT2_MIR");
    asm("           STR R1, [R0, #0]");     // INT2_MIR[0] = 0xFFFFFFFF
    asm("           STR R1, [R0, #0x0100]");// INT2_MIR[1] = 0xFFFFFFFF
    asm("           STR R1, [R0, #0x0200]");// INT2_MIR[2] = 0xFFFFFFFF
    asm("           STR R1, [R0, #0x0300]");// INT2_MIR[3] = 0xFFFFFFFF
    /* Clear ITR of old interrupts that have tiggered */
    asm("           LDR R1, ITR_REG");
    asm("           LDR R0, INT1_ITR");
    asm("           STR R1, [R0]");         // *INT1_ITR   = 0
    asm("           LDR R0, INT2_ITR");
    asm("           STR R1, [R0, #0]");     // INT2_ITR[0] = 0
    asm("           STR R1, [R0, #0x0100]");// INT2_ITR[1] = 0
    asm("           STR R1, [R0, #0x0200]");// INT2_ITR[2] = 0
    asm("           STR R1, [R0, #0x0300]");// INT2_ITR[3] = 0
    /* Clear current interrupts that are being serviced */
    asm("           LDR R1, CTL_REG");
    asm("           LDR R0, INT1_CTL");
    asm("           STR R1, [R0]");         // *INT1_CTL   = 3
    asm("           LDR R0, INT2_CTL");
    asm("           STR R1, [R0, #0]");     // INT2_CTL[0] = 3
    asm("           STR R1, [R0, #0x0100]");// INT2_CTL[1] = 3
    asm("           STR R1, [R0, #0x0200]");// INT2_CTL[2] = 3
    asm("           STR R1, [R0, #0x0300]");// INT2_CTL[3] = 3
    /* Enable Interrupts by setting CPSR */
    asm("           MRS R0, CPSR");
    asm("           BIC R0, R0, #0xC0");
    asm("           ORR R0, R0, #0");
    asm("           MSR CPSR, R0");

/* ------------------------------------------------------------------------ *
 *  Branch to _c_int00 ( Start of C Code )                                  *
 * ------------------------------------------------------------------------ */
asm("           B     _c_int00");           // Branch to ( C code )
