/** ============================================================================
 *  @file   loop_os.c
 *
 *  @path   $(DSPLINK)\gpp\src\samples\loop\Linux
 *
 *  @desc   OS specific implementation of functions used by the loop
 *          sample application.
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
#include <stdio.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>

/*  ----------------------------------- Application Header            */
#include <loop.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */



/** ============================================================================
 *  @func   LOOP_0Print
 *
 *  @desc   Print a message without any arguments.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_0Print (Char8 * str)
{
    printf (str) ;
}


/** ============================================================================
 *  @func   LOOP_1Print
 *
 *  @desc   Print a message with one arguments.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_1Print (Char8 * str, Uint32 arg)
{
    printf (str, arg) ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
