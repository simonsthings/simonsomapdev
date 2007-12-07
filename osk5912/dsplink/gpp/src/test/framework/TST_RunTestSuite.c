/** ============================================================================
 *  @file   TST_RunTestSuite.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Implemetation of TST_RunTestSuite function.
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
#include <TST_RunTestSuite.h>
#include <TST_PrintFuncs.h>
#include <TST_Helper.h>
#include <testsuiteANA.h>
#include <testsuiteAPI.h>
#include <testsuiteSTS.h>
#include <testsuiteBVR.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_RunTestSuite
 *
 *  @desc   This function invokes the desired testsuite.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_RunTestSuite (IN Int32        argc,
                  IN Char8 **     argv,
                  IN TST_Attrs *  attrs,
                  IN TST_State    testState,
                  IN Result *     frameworkResult)
{
    DSP_STATUS     status                           = DSP_SOK ;
    Bool           done                             = FALSE   ;
    WhichTestSuite whichTestSuite                   = NoTest  ;
    Pstr           testSuite                                  ;
    Char8          testSuiteUppercase [MAX_BUFSIZE]           ;
    Int32          cmpResult                                  ;

    TST_PrnDebug ("Entered TST_RunTestSuite ()") ;

    DBC_Require (argv != NULL) ;
    DBC_Require (argc >= 2) ;

    testSuite = *argv ;

    status = TST_ToUpper (testSuite, testSuiteUppercase) ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("TST_ToUpper. Status = [0x%x]", status) ;
    }

    DBC_Assert (testSuite != NULL) ;

    if (done == FALSE) {
        TST_Strcmp ("LINKAPITEST", testSuiteUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            done = TRUE ;
            whichTestSuite = Test_Api ;
        }
    }
    if (done == FALSE) {
        TST_Strcmp ("LINKANALYSISTEST", testSuiteUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            done = TRUE ;
            whichTestSuite = Test_Analysis ;
        }
    }
    if (done == FALSE) {
        TST_Strcmp ("LINKBEHAVIORTEST", testSuiteUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            done = TRUE ;
            whichTestSuite = Test_Behavior ;
        }
    }
    if (done == FALSE) {
        TST_Strcmp ("LINKSTRESSTEST", testSuiteUppercase, &cmpResult) ;
        if (cmpResult == 0) {
            done = TRUE ;
            whichTestSuite = Test_Stress ;
        }
    }

    if (done == FALSE) {
        whichTestSuite = NoTest ;
    }

    --argc ;
    ++argv ;

    switch (whichTestSuite) {
    case Test_Api:
        status = TestSuite_Api (argc,
                                argv,
                                attrs,
                                testState,
                                frameworkResult) ;
        break ;

    case Test_Analysis:
        status = TestSuite_Analysis (argc,
                                     argv,
                                     attrs,
                                     testState,
                                     frameworkResult) ;
        break ;

    case Test_Behavior:
        status = TestSuite_Behavior (argc,
                                     argv,
                                     attrs,
                                     testState,
                                     frameworkResult) ;
        break ;
    case Test_Stress:
        status = TestSuite_Stress (argc,
                                   argv,
                                   attrs,
                                   testState,
                                   frameworkResult) ;
        break ;
    default:
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Test suite not available. Status = [0x%x]", status) ;
        break ;
    }

    TST_PrnDebug ("Leaving TST_RunTestSuite ()") ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
