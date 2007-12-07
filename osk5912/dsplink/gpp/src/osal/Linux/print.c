/** ============================================================================
 *  @file   print.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implementation of OS printf abstraction.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <asm/div64.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- OSAL Headers                */
#include <print.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   PRINT_IsInitialized
 *
 *  @desc   Flag to track initialization of the sub-component.
 *  ============================================================================
 */
STATIC Uint32  PRINT_IsInitialized  = FALSE   ;


/** ============================================================================
 *  @func   PRINT_Initialize
 *
 *  @desc   Initializes the PRINT sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRINT_Initialize ()
{
    DSP_STATUS status = DSP_SOK ;

    PRINT_IsInitialized = TRUE ;

    return status ;
}


/** ============================================================================
 *  @func   PRINT_Finalize
 *
 *  @desc   Releases resources used by this sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PRINT_Finalize ()
{
    DSP_STATUS status = DSP_SOK ;

    PRINT_IsInitialized = FALSE ;

    return status ;
}


/** ============================================================================
 *  @func   PRINT_Printf
 *
 *  @desc   Provides standard printf functionality abstraction.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
PRINT_Printf (Pstr format, ...)
{
    va_list args ;
    char buffer [512];

    buffer[0] = '<';
    buffer[1] = '1';
    buffer[2] = '>';

    va_start (args, format) ;
    vsprintf (buffer+3, format, args);
    va_end   (args) ;

    printk (buffer) ;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
