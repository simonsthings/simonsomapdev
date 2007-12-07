/** ============================================================================
 *  @file   testsuiteAPP.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\application
 *
 *  @desc   Implementation of TestSuite_Application function.
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

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <TST_RunTest.h>
#include <strmcopy.h>
#include <testsuiteAPP.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TestSuite_Application
 *
 *  @desc   This function takes the name of the function as an argument in
 *          string form and returns the function pointer to that function
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TestSuite_Application (IN  Int32        argc,
                       IN  Pstr *       argv,
                       IN  TST_Attrs *  attrs,
                       IN  TST_State    testState,
                       OUT Result *     frameworkResult)
{
    DSP_STATUS    status                          = DSP_SOK ;
    Bool          check                           = FALSE   ;
    PtrToTestCase testCase                        = NULL    ;
    Int32         cmpResult                                 ;
    Char8 *       testName                                  ;
    Char8         testCaseUppercase [MAX_BUFSIZE]           ;
    TestInfo      testInfo                                  ;


    TST_PrnDebug ("Entered TestSuite_Application ()") ;

    DBC_Require (argc >= 1) ;
    DBC_Require (argv [0] != NULL) ;

    testName = argv [0] ;
    --argc ;
    ++argv ;

    testInfo.testSuite = "LinkApplicationTest" ;
    testInfo.testCase = testName ;

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "STRMCOPY",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testCase = StrmCopy ;
        }
    }

    /* HERE THE  COMPARISONS WILL BE MADE FOR THE
       OTHER TESTS */
    if (check == FALSE)  {
         status= DSP_EFAIL ;
         TST_PrnError ("Test not available. Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status) && check == TRUE) {
        status = TST_RunTest (argc,
                              argv,
                              testCase,
                              &testInfo,
                              attrs,
                              testState,
                              frameworkResult) ;
    }

    TST_PrnDebug ("Leaving TestSuite_Application ()") ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
