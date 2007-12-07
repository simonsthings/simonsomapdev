/** ============================================================================
 *  @file   TST_Mem.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils\Linux
 *
 *  @desc   Implemetation of memory allocation and deallocation Functions used
 *          by framework and test suites.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers           */
#include <stdlib.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_Alloc
 *
 *  @desc   This function allocates memory for test cases and framework.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_Alloc (Void ** pointer, Uint32 size)
{
    DSP_STATUS  status = DSP_SOK ;
    *pointer = malloc (size) ;
    if (*pointer == NULL) {
        status = DSP_EMEMORY ;
    }
    return status ;
}


/** ============================================================================
 *  @func   TST_Print
 *
 *  @desc   This function deallocates memory allocated by test cases and
 *          framework.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_Free (Void ** pointer)
{
    free (*pointer) ;
    *pointer = NULL ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
