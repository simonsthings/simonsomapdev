/** ============================================================================
 *  @file   testsuiteANA.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\analysis
 *
 *  @desc   Implemetation of TestSuite_Analysis Function.
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
#include <TST_RunTest.h>
#include <TST_PrintFuncs.h>

/*  ----------------------------------- Analysis Tests                */
#include <testsuiteANA.h>
#include <ANA_AsyncChnlIssueReclaim.h>
#include <ANA_ProcLoad.h>
#include <ANA_Loop.h>
#include <ANA_SyncChnlIssueReclaim.h>
#include <ANA_MsgqPutGet.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   TestcaseObj
 *
 *  @desc   The buffer structure to be used for messaging.
 *
 *  @field  name
 *              The name of the test case.
 *  @field  function
 *              Pointer to the test function.
 *  ============================================================================
 */
typedef struct TestcaseObj_tag {
    Char8 *       name     ;
    PtrToTestCase function ;
} TestcaseObj ;


/** ----------------------------------------------------------------------------
 *  @name   TestList
 *
 *  @desc   Contains the list of testcase name and the associated functions.
 *  ----------------------------------------------------------------------------
 */
STATIC TestcaseObj TestList [] = {
    {"ANA_ASYNCCHNLISSUERECLAIM",   ANA_AsyncChnlIssueReclaim       },
    {"ANA_PROCLOAD",                ANA_ProcLoad                    },
    {"ANA_LOOP",                    ANA_Loop                        },
    {"ANA_SYNCCHNLISSUERECLAIM",    ANA_SyncChnlIssueReclaim        },
#if defined (MSGQ_COMPONENT)
    {"ANA_MSGQPUTGET",              ANA_MsgqPutGet                  },
#endif /* if defined (MSGQ_COMPONENT) */
    {NULL,                         NULL                             }
    /* NULL to mark end of list */
} ;


/** ============================================================================
 *  @func   TestSuite_Analysis
 *
 *  @desc   This function decides which Analysis testcase to run.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TestSuite_Analysis (IN  Int32        argc,
                    IN  Char8 **     argv,
                    IN  TST_Attrs *  attrs,
                    IN  TST_State    testState,
                    OUT Result *     frameworkResult)
{
    DSP_STATUS    status                          = DSP_SOK ;
    Bool          check                           = FALSE   ;
    PtrToTestCase testCase                        = NULL    ;
    Uint16        i                               = 0       ;
    Int32         cmpResult                       = 0       ;
    Char8 *       testName                                  ;
    Char8         testCaseUppercase [MAX_BUFSIZE]           ;
    TestInfo      testInfo                                  ;

    TST_PrnDebug ("Entered TestSuiteAnalysis ()") ;

    DBC_Require (argc >= 1) ;
    DBC_Require (argv [0] != NULL) ;

    testName = argv [0] ;

    --argc ;
    ++argv ;

    testInfo.testSuite = "LinkAnalysisTest" ;
            testInfo.testCase = testName ;
        TST_ToUpper (testName, testCaseUppercase) ;

    while (   DSP_SUCCEEDED (status)
           && (check == FALSE)
           && (TestList [i].name != NULL)) {

        status = TST_Strcmp (testCaseUppercase,
                             TestList [i].name,
                             &cmpResult) ;

        if (DSP_FAILED (status)) {
            TST_PrnError ("TST_Strcmp failed. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testCase = TestList [i].function ;
        }
        i++ ;
    }

    if (check == FALSE)  {
         status = DSP_EFAIL ;
         TST_PrnError ("TEST NOT AVAILABLE. Status = [0x%x]", status) ;
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

    TST_PrnDebug ("Leaving TestSuiteAnalysis") ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
