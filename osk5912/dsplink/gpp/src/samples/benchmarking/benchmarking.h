/** ============================================================================
 *  @file   Benchmarking.h
 *
 *  @path   $(PROJROOT)\src\samples\benchmarking
 *
 *  @desc   Defines the configurable parameters for the benchmarking test which
 *          sends a buffer across the DSP processor and measures the
 *          performance of the transfer. In this application the transfer is
 *          done using only one buffer and no queueing of buffers is done.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  This software is copyright to Texas Instruments Incorporated.
 *  Its usage is governed by the terms and conditions stipulated in
 *  the license agreement under which this software has been supplied.
 *
 *  The recipient of this software implicitly accepts the terms of
 *  the license.
 *  ============================================================================
 */


#if !defined (BENCHMARKING_H)
#define BENCHMARKING_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  ID_PROCESSOR
 *
 *  @desc   The processor id of the processor being used.
 *  ============================================================================
 */
#define ID_PROCESSOR       0


/** ============================================================================
 *  @const  CHNL_ID_OUTPUT
 *
 *  @desc   ID of channel used to send data to DSP.
 *  ============================================================================
 */
#define CHNL_ID_OUTPUT     0


/** ============================================================================
 *  @name   MAX_ARGUMENTS
 *
 *  @desc   The maximum no of arguments which are being passed to DSP.
 *  ============================================================================
 */
#define MAX_ARGUMENTS      2


/** ============================================================================
 *  @func   Benchmarking_Create
 *
 *  @desc   This function allocates nesessary resources.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Resource allocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    Benchmarking_Delete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
Benchmarking_Create () ;


/** ============================================================================
 *  @func   Benchmarking_Execute
 *
 *  @desc   This function executes the Benchmarking application.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Benchmarking application execution failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    Benchmarking_Delete , Benchmarking_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
Benchmarking_Execute () ;


/** ============================================================================
 *  @func   Benchmarking_Delete
 *
 *  @desc   This function deallocates resources allocated earlier by call to
 *          Benchmarking_Create (). During cleanup, the allocated resources are
 *          being freed unconditionally. Actual applications may require
 *          stricter check against return values for robustness.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Resource deallocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    Benchmarking_Create
 *  ============================================================================
 */
NORMAL_API
Void
Benchmarking_Delete () ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (BENCHMARKING_H) */
