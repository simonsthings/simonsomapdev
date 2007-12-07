/** ============================================================================
 *  @file   main.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework\Linux
 *
 *  @desc   The driver function of testsuite.
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
#include <errbase.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   main
 *
 *  @desc   The driver function of testsuite.
 *
 *  @modif  None.
 *  ============================================================================
 */
int
main (int argc, char ** argv)
{
    if ((argc != 5) && (argc != 3)) {
        printf ("\n=======================================================") ;
        printf ("\nThis program invokes the test suite for DSP/BIOS(tm) LINK."
                "\nUSAGE:"
                "\n To invoke a single test only:"
                "\n%s single <testsuite> <testcase> <absolute path of datafile>"
                "\nAlternately, to invoke a multiple tests through a script:"
                "\n%s script <absolute path of script file>"
                "\nTo view the format of the script and datafiles, refer to"
                "\nthe user reference manual"
                "\nand Test Suite design Document [LNK 015 DES]."
                ,argv[0],argv[0]) ;
        printf ("\n=======================================================") ;
    }
    else {
        /*
         * Invocation of the testsuite with the parameters given by the user
         */
        TST_Main (argc, (Pstr *) argv) ;
    }
    return 0 ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
