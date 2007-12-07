/** ============================================================================
 *  @file   failure.c
 *
 *  @path   $(DSPLINK)\dsp\src\gen
 *
 *  @desc   This file implements failure handler functions. They also provide
 *          support for debugging the DSP side application.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <failure.h>

#if defined(_DEBUG)

/*  ============================================================================
 *  @name   CCS_BREAKPOINT
 *
 *  @desc   If USE_CCS_BREAKPOINT is defined the breakpoint instruction
 *          specific to CCS is used.
 *  ============================================================================
 */
#if (defined(USE_CCS_BREAKPOINT) && defined( _55_ ))

#define CCS_BREAKPOINT  \
    asm(" .if (.MNEMONIC)\n" \
        " ESTOP_1\n"     \
        " .else\n"           \
        " ESTOP_1()\n"   \
        " .endif\n"          \
        " NOP")

#endif /* (defined(USE_CCS_BREAKPOINT) && defined( _55_ )) */


/** ============================================================================
 *  @name   FAIL_XXX
 *
 *  @desc   Contains information regaring first failure that is set otherwise
 *          if no failure is set all varibles should be zero.
 *  ============================================================================
 */
Int   FAIL_Set   = 0 ;
Int   FAIL_File;
Int   FAIL_Line;
Int   FAIL_Code;

/** ============================================================================
 *  @func   SetReason
 *
 *  @desc   Failure handler function for failures in LINK.
 *
 *  @modif  None
 *  ============================================================================
 */
Void SetReason (Int fileId, Int lineNo, Int failureCode)
{
    if (FAIL_Set == 0)
    {
        FAIL_Set  = 1;
        FAIL_File = fileId;
        FAIL_Line = lineNo;
        FAIL_Code = failureCode;
    }

#if (defined(USE_CCS_BREAKPOINT) && defined( _55_ ))
    CCS_BREAKPOINT ;
#else
    /* A forever while loop.
     * It is not simple while (1) to sneak through the compiler optimizations.
     */
    {
        volatile Int i = 1 ;
        while (i) ;
    }
#endif /* (defined(USE_CCS_BREAKPOINT) && defined( _55_ )) */
}
#endif /* if defined(_DEBUG) */
