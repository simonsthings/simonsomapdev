/** ============================================================================
 *  @file   TST_Print.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils\Linux
 *
 *  @desc   Implemetation of TST_Print function.
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
#include <stdarg.h>
#include <stdio.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Print.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */

#if !defined (UTILS_KERNEL)

/** ============================================================================
 *  @func   TST_Print
 *
 *  @desc   This function provides standard printf functionality abstraction.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_Print (Char8 * format, ...)
{
    Uint32     arg          ;
    va_list    va           ;
    Char8      buffer [512] ;
    Uint32     numChar      ;

    va_start (va, format) ;

    arg = va_arg (va, int) ;

    va_end (va) ;

    numChar = sprintf (&buffer [0], format, arg) ;
    if (numChar > 0) {
        printf (buffer) ;
        fflush (stdout) ;
    }
    else {
        printf ("TST Framework: Error in printing\n") ;
    }
}

#endif /* ifndef (UTILS_KERNEL)*/

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
