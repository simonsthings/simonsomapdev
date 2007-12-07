/** ============================================================================
 *  @file   TST_TestInputs.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Implemetation of TST_TestInputs Function.
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
#include <TST_Framework.h>
#include <TST_FileOperation.h>
#include <TST_TestInputs.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_TestInputs
 *
 *  @desc   It  parses the data file and fetches the inputs to execute a
 *          test case.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_TestInputs (IN  Void *   filePtr,
                OUT Uint32 * argc,
                OUT Char8    arguments [][MAX_BUFSIZE],
                OUT Bool *   isTestCommented)
{
    DSP_STATUS status = DSP_SOK ;

    status = TST_ReadLine (filePtr, argc, arguments, isTestCommented) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
