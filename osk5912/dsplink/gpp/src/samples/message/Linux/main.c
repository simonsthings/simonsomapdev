/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)\gpp\src\samples\message\Linux
 *
 *  @desc   Linux specific implementation of message sample application's driver.
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
#include <message.h>


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
    Char8 * strNumIterations = NULL ;

    if (argc != 3) {
        printf ("Usage : %s <absolute path of DSP executable> "
                "<number of transfers>\n",
                argv [0]) ;
    }
    else {
        dspExecutable    = argv [1] ;        
        strNumIterations = argv [2] ;

        MESSAGE_Main (dspExecutable, strNumIterations) ;
    }

    return 0 ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
