/** ============================================================================
 *  @file   safe.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Contains safe programming macros
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (SAFE_H)
#define SAFE_H


USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  IS_OBJECT_VALID
 *
 *  @desc   Checks validity of object by comparing against it's signature.
 *  ============================================================================
 */
#define IS_OBJECT_VALID(obj, sign)                                  \
    (((obj != NULL) && ((obj)->signature == sign)) ? TRUE : FALSE)


/** ============================================================================
 *  @macro  IS_RANGE_VALID
 *
 *  @desc   Checks if a value lies in given range.
 *  ============================================================================
 */
#define IS_RANGE_VALID(x,min,max) (((x) < (max)) && ((x) >= (min)))

/** ============================================================================
 *  @macro  MIN
 *
 *  @desc   Returns minumum of the two arguments
 *  ============================================================================
 */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/** ============================================================================
 *  @macro  FREE_PTR
 *
 *  @desc   Frees memory pointed to by ptr and sets it to NULL. Also returns
 *          status of MEM_Free function call
 *  ============================================================================
 */
#if defined (OS_LINUX)
#define FREE_PTR(ptr)  ({ DSP_STATUS status = DSP_SOK ; \
                          status = MEM_Free (ptr, MEM_DEFAULT) ; \
                          ptr = NULL ; \
                          status ; })

#endif

#if defined (OS_NUCLEUS)
#define FREE_PTR(ptr)  (MEM_Free (ptr, MEM_DEFAULT) | (Uint32) (ptr = NULL))
#endif


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (SAFE_H) */
