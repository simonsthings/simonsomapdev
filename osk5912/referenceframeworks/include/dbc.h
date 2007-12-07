/** ============================================================================
 *  @file   dbc.h
 *
 *  @path   $(DSPLINK)\dsp\src\inc
 *
 *  @desc   DSP-side Design By Contract support.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef _DBC_
#define _DBC_

#ifdef __cplusplus
extern "C" {
#endif

#if defined (_DEBUG)
#include <sys.h>

#if (defined(USE_CCS_BREAKPOINT) && defined( _55_ ))

#define CCS_BREAKPOINT	      \
	asm(" .if (.MNEMONIC)\n" \
        " ESTOP_1\n"     \
        " .else\n"           \
        " ESTOP_1()\n"   \
        " .endif\n"          \
        " NOP")

/** ============================================================================
 *  @macro  DBC_assert
 *
 *  @desc   Asserts a condition.
 *          If USE_CCS_BREAKPOINT is defined the breakpoint instruction
 *          specific to CCS is used.
 *  ============================================================================
 */
#define DBC_assert(expr)    for (; !( expr ); )  \
                                CCS_BREAKPOINT ;

#else /* (defined(USE_CCS_BREAKPOINT) && defined( _55_ )) */

#define DBC_assert(c) \
    if (!(c)) { \
        SYS_abort("Assertion Failed: file=%s, line=%d.\n", __FILE__, \
        __LINE__); }

#endif /* (defined(USE_CCS_BREAKPOINT) && defined( _55_ )) */


/** ============================================================================
 *  @macro  DBC_require
 *
 *  @desc   Requirement assertion.
 *  ============================================================================
 */
#define DBC_require DBC_assert

/** ============================================================================
 *  @macro  DBC_ensure
 *
 *  @desc   Ensure assertion.
 *  ============================================================================
 */
#define DBC_ensure  DBC_assert

#else /* if defined (_DEBUG) */

/** ============================================================================
 *  @macro  DBC_xxx
 *
 *  @desc   Compiled out in case of DBC_ASSERTS is not set.
 *  ============================================================================
 */
#define DBC_assert(c)
#define DBC_require(c)
#define DBC_ensure(c)


#endif /* if defined (_DEBUG) */


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* DBC_ */

