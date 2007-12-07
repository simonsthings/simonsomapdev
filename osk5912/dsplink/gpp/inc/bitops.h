/** ============================================================================
 *  @file   bitops.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Defines generic macros for bitwise opeartions.
 *
 *  @ver    01.10.01
 *
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (BITOPS_H)
#define BITOPS_H


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  SET_BITS
 *
 *  @desc   Sets bits contained in given mask.
 *          This macro is independent of operand width. User must ensure
 *          correctness.
 *  ============================================================================
 */
#define SET_BITS(num,mask)          ((num) |= (mask))

/** ============================================================================
 *  @macro  CLEAR_BITS
 *
 *  @desc   Clears bits contained in given mask.
 *          This macro is independent of operand width. User must ensure
 *          correctness.
 *  ============================================================================
 */
#define CLEAR_BITS(num,mask)        ((num) &= ~(mask))

/** ============================================================================
 *  @macro  SET_BIT
 *
 *  @desc   Sets bit at given position.
 *          This macro is independent of operand width. User must ensure
 *          correctness.
 *  ============================================================================
 */
#define SET_BIT(num,pos)            ((num) |= (1u << (pos)))

/** ============================================================================
 *  @macro  CLEAR_BIT
 *
 *  @desc   Clears bit at given position.
 *          This macro is independent of operand width. User must ensure
 *          correctness.
 *  ============================================================================
 */
#define CLEAR_BIT(num,pos)          ((num) &= ~(1u << (pos)))


/** ============================================================================
 *  @macro  TEST_BIT
 *
 *  @desc   Tests if bit at given position is set.
 *          This macro is independent of operand width. User must ensure
 *          correctness.
 *  ============================================================================
 */
#define TEST_BIT(num,pos)           !(!((num) & (1u << (pos))))


/** ============================================================================
 *  @macro  GET_NBITS8
 *
 *  @desc   Gets numeric value of specified bits from an 8 bit value.
 *  ============================================================================
 */
#define GET_NBITS8(num,pos,width)   \
            (((num) & ((0xFF >> (8 - (width))) << (pos))) >> (pos))

/** ============================================================================
 *  @macro  GET_NBITS16
 *
 *  @desc   Gets numeric value of specified bits from an 16 bit value.
 *  ============================================================================
 */
#define GET_NBITS16(num,pos,width)  \
            (((num) & ((0xFFFF >> (16 - (width))) << (pos))) >>  (pos))

/** ============================================================================
 *  @macro  GET_BITS32
 *
 *  @desc   Gets numeric value of specified bits from an 32 bit value.
 *  ============================================================================
 */
#define GET_NBITS32(num,pos,width)  \
            (((num) & ((0xFFFFFFFF >> (32 - (width))) << (pos))) >>  (pos))

/** ============================================================================
 *  @macro  SET_NBITS8
 *
 *  @desc   Sets specified bits in a 8 bit entity to given value.
 *  ============================================================================
 */
#define SET_NBITS8(num,pos,width,value)     \
            (num) &= ~((0xFF >> (8 - (width))) << (pos)) ;                  \
            (num) |= (((value) & (0xFF >> (8 - (width)))) << (pos)) ;

/** ============================================================================
 *  @macro  SET_NBITS16
 *
 *  @desc   Sets specified bits in a 16 bit entity to given value.
 *  ============================================================================
 */
#define SET_NBITS16(num,pos,width,value)    \
            (num) &= ~((0xFFFF >> (16 - (width))) << (pos)) ;               \
            (num) |= (((value) & (0xFFFF >> (16 - (width)))) << (pos)) ;

/** ============================================================================
 *  @macro  SET_NBITS32
 *
 *  @desc   Sets specified bits in a 32 bit entity to given value.
 *  ============================================================================
 */
#define SET_NBITS32(num,pos,width,value)   \
            (num) &= ~((0xFFFFFFFF >> (32 - (width))) << (pos)) ;           \
            (num) |= (((value) & (0xFFFFFFFF >> (32 - (width)))) << (pos)) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif  /* !defined (BITOPS_H) */
