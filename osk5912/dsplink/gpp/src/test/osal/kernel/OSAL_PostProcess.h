/** ============================================================================
 *  @file   OSAL_PostProcess.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Defines the interface of OSAL_PostProcess function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */



#if !defined (OSAL_POSTPROCESS_H)
#define OSAL_POSTPROCESS_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   OSAL_PostProcess
 *
 *  @desc   This function compares the status of setup with expected status of
 *          setup and status of test with expected status of test.
 *
 *  @arg    statusOfTest
 *              Actual status of the test.
 *  @arg    expStatusTest
 *              Expected status of the test.
 *  @arg    statusOfSetup
 *              Actual status of the setup.
 *  @arg    expStatusSetup
 *              Expected status of the setup.
 *
 *  @ret    DSP_SOK
 *              Test has passed.
 *          DSP_EFAIL
 *              Test has failed.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_PostProcess (IN DSP_STATUS statusOfTest,
                 IN Char8 * expStatusTest,
                 IN DSP_STATUS statusOfSetup,
                 IN Char8 * expStatusSetup) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !defined (OSAL_POSTPROCESS_H) */
