/** ============================================================================
 *  @file   TST_Initialize.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework\Linux
 *
 *  @desc   Implemetation of TST_Initialize and TST_Finalize function.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Test framework                */
#include <TST_Initialize.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_Initialize
 *
 *  @desc   Initialization function to Testsuite.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_Initialize ()
{
    DSP_STATUS status = DSP_SOK ;

    return status ;
}


/** ============================================================================
 *  @func   TST_Finalize
 *
 *  @desc   Finalization function to Testsuite.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_Finalize ()
{
    DSP_STATUS status = DSP_SOK ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
