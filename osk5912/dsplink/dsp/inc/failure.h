/** ============================================================================
 *  @file   failure.h
 *
 *  @path   $(DSPLINK)\dsp\inc
 *
 *  @desc   Header file for failure handler module.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef FAILURE_
#define FAILURE_

/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>


#ifdef __cplusplus
extern "C" {
#endif

#if defined(_DEBUG)
/** ============================================================================
 *  @name   FID_XXX
 *
 *  @desc   File ids for files in LINK IOM driver.
 *  ============================================================================
 */
#define FID_BASE                (0x100)
#define FID_DRIVER_C            (FID_BASE + 1)
#define FID_SHM_C               (FID_BASE + 2)
#define FID_HAL_C               (FID_BASE + 3)
#define FID_EMIF_C              (FID_BASE + 4)
#define FID_INTERRRUT_C         (FID_BASE + 5)
#define FID_APP_C               (FID_BASE + 6)
#define FID_HPIDRIVER_C         (FID_BASE + 7)

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets the failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON(code)  SetReason (FILEID, __LINE__, code)


/** ============================================================================
 *  @func   SetReason
 *
 *  @desc   Sets the reason of failure.
 *          The reason is set for first failure only.
 *
 *  @arg    fileId
 *              An ID representing a file.
 *          lineNo
 *              Line number from where this function is called.
 *          failureCode
 *              Failure code.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Void SetReason (int fileId, int lineNo, int failureCode);

#else  /* if defined (_DEBUG) */

#define SET_FAILURE_REASON(code)

#endif /* if defined (_DEBUG) */

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* FAILURE_ */
