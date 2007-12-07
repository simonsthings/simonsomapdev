/** ============================================================================
 *  @file   TST_PRINTFuncs.c
 *
 *  @path   $(DSPLINK)\gpp\src\utils\Linux
 *
 *  @desc   Implemetation of Various print functions.
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

#if defined (UTILS_KERNEL)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#endif

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Print.h>
#include <TST_PrintFuncs.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_PrnError
 *
 *  @desc   This function prints error strings.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnError (IN Char8 * fmt, IN DSP_STATUS status)
{
    TST_PRINT ("\n\tERROR : ") ;
    TST_PRINT (fmt, status) ;
    TST_PRINT ("\n") ;
}


/** ============================================================================
 *  @func   TST_PrnDebug
 *
 *  @desc   This function prints debug strings.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnDebug (IN Char8 * fmt)
{
#if defined (DEBUG_FRAMEWORK)
    TST_PRINT ("\tDEBUG : \n") ;
    TST_PRINT (fmt) ;
#endif  /* defined (DEBUG_FRAMEWORK) */
}


/** ============================================================================
 *  @func   TST_PrnArgs
 *
 *  @desc   This function prints arguments used by testcase.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnArgs (IN Uint32 argc, IN Char8 ** argv)
{
    Uint32 i ;

    for (i = 0 ; i < argc ; i++) {
        TST_PRINT (" %s", argv [i]) ;
    }
    TST_PRINT ("\n") ;
}


/** ============================================================================
 *  @func   TST_PrnInfo
 *
 *  @desc   This function prints information strings.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnInfo (IN Char8 * fmt, ...)
{
    Int32 arg1 ;
    va_list va ;

    va_start (va, fmt) ;

    arg1 = va_arg (va, Int32) ;

    va_end (va) ;

    TST_PRINT ("\n\tINFO : ") ;
    TST_PRINT (fmt, arg1) ;
    TST_PRINT ("\n") ;
}


/** ============================================================================
 *  @func   TST_PrnStatus
 *
 *  @desc   This function prints status of the tests.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnStatus (IN DSP_STATUS statusToPrint)
{
    if (DSP_SUCCEEDED (statusToPrint)) {
        TST_PRINT ("Status : Passed\n") ;
    }
    else {
        TST_PRINT ("Status : Failed 0x%lx\n", statusToPrint) ;
    }
}


/** ============================================================================
 *  @func   TST_PrnTestCase
 *
 *  @desc   This function Testsuite and testcase name.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnTestCase (IN TestInfo * testInfo)
{
    TST_PRINT ("\n") ;
    TST_PRINT ("Suite  : ") ;
    TST_PRINT (testInfo -> testSuite) ;
    TST_PRINT ("\n") ;
    TST_PRINT ("Test   : ") ;
    TST_PRINT (testInfo -> testCase) ;
}


/** ============================================================================
 *  @func   TST_PrnSummary
 *
 *  @desc   This function prints summary of the tests run.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
Void
TST_PrnSummary (IN Result * finalResult)
{
    Uint32 i ;

    TST_PRINT ("\n") ;
    for (i =0 ; i < 40 ; i++) {
        TST_PRINT ("=") ;
    }
    TST_PRINT ("\n") ;

    TST_PRINT ("Test Results Summary\n") ;
    TST_PRINT ("Total Tests  : %ld\n", finalResult -> testCount)    ;
    TST_PRINT ("Tests Run    : %ld\n", finalResult -> testRun)      ;
    TST_PRINT ("Tests Passed : %ld\n", finalResult -> passCount)    ;
    TST_PRINT ("Tests Failed : %ld\n", finalResult -> failCount)    ;

    TST_PRINT ("\n") ;

    for (i =0 ; i < 40 ; i++) {
        TST_PRINT ("=") ;
    }

    TST_PRINT ("\n") ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
