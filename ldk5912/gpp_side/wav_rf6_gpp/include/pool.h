/** ============================================================================
 *  @file   pool.h
 *
 *  @path   $(DSPLINK)/gpp/src/api/
 *
 *  @desc   Defines signature of POOL API Functions.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (POOL_H)
#define POOL_H


USES (gpptypes.h)
USES (dsplink.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @func   POOL_Open
 *
 *  @desc   This function opens a specific pool referenced by the pool Id
 *          provided.
 *
 *  @arg    poolId
 *              Pool Identification number.
 *  @arg    params
 *              POOL Open specific parameters.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  Pool ID must be less than maximum allowed value.
 *          params must be valid.
 *
 *  @leave  None.
 *
 *  @see    POOL_Close
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
POOL_Open (IN Uint16 poolId, IN Pvoid params) ;


/** ============================================================================
 *  @func   POOL_Close
 *
 *  @desc   This function closes a specific pool whose pool id is provided.
 *
 *  @arg    poolId
 *              Pool Identification number.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EINVALIDARG
 *              Invalid argument.
 *          DSP_EMEMORY
 *              Memory error.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  Pool ID must be less than maximum allowed value.
 *
 *  @leave  None.
 *
 *  @see    POOL_Open
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
POOL_Close (IN Uint16 poolId) ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (POOL_H) */
