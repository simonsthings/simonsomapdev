/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)\gpp\src\samples\loop\Linux
 *
 *  @desc   Linux specific implementation of loop sample application's driver.
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
#include <stdlib.h>

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
 *  @func   main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  None
 *  ============================================================================
 */
int main (int argc, char ** argv)
{
    Char8 * dspExecutable    = NULL ;
    Char8 * strBufferSize    = NULL ;
    Char8 * strNumIterations = NULL ;

    if (argc != 4) {
        printf ("Usage : %s <absolute path of DSP executable> "
                "<Buffer Size> <number of transfers>\n",
                argv [0]) ;
    }
    else {
        dspExecutable    = argv [1] ;
        strBufferSize    = argv [2] ;
        strNumIterations = argv [3] ;

        LOOP_Main (dspExecutable, strBufferSize, strNumIterations) ;
    }

    return 0 ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
