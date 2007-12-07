/** ============================================================================
 *  @file   TST_dbc.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Design by Contract
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (TST_DBC_H)
#define TST_DBC_H


USES (gpptypes.h)
USES (tst_print.h)

#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  DBC_PRINTF
 *
 *  @desc   printf routine.
 *  ============================================================================
 */
#define  DBC_PRINTF     TST_PRINT


#if defined (TST_DDSP_DEBUG)

/** ============================================================================
 *  @macro  DBC_Assert
 *
 *  @desc   Assert on expression.
 *  ============================================================================
 */
#define DBC_Assert(exp)                                                     \
        if (!(exp)) {                                                       \
            DBC_PRINTF ("Assertion failed ("#exp"). File : %s Line : %d\n", \
                            __FILE__, __LINE__) ;                           \
        }                                                                   \

/** ============================================================================
 *  @macro  DBC_Require
 *
 *  @desc   Function Precondition.
 *  ============================================================================
 */
#define DBC_Require    DBC_Assert

/** ============================================================================
 *  @macro  DBC_Ensure
 *
 *  @desc   Function Postcondition.
 *  ============================================================================
 */
#define DBC_Ensure     DBC_Assert

#else /* defined (TST_DDSP_DEBUG) */

/** ============================================================================
 *  @macro  DBC_Assert/DBC_Require/DBC_Ensure
 *
 *  @desc   Asserts defined out.
 *  ============================================================================
 */
#define DBC_Assert(exp)
#define DBC_Require(exp)
#define DBC_Ensure(exp)

#endif /* defined (TST_DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (TST_DBC_H) */
