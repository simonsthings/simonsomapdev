/** ============================================================================
 *  @file   testsuiteSTS.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\stress
 *
 *  @desc   Implementation of TestSuite_Stress function.
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
#include <testsuiteSTS.h>
#include <STS_MultiThread.h>
#include <STS_SingleThread.h>
#include <STS_MsgqLocate.h>
#include <STS_MsgqMemory.h>
#include <STS_MsgqMemoryLeakAllocator.h>
#include <STS_MsgqMemoryLeakMsgq.h>
#include <STS_MsgqMemoryLeakTransport.h>

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
    {"STS_MULTITHREAD",             STS_MultiThread             },
    {"STS_SINGLETHREAD",            STS_SingleThread            },
#if defined (MSGQ_COMPONENT)
    {"STS_MSGQLOCATE",              STS_MsgqLocate              },
    {"STS_MSGQMEMORY",              STS_MsgqMemory              },
    {"STS_MSGQMEMORYLEAKALLOCATOR", STS_MsgqMemoryLeakAllocator },
    {"STS_MSGQMEMORYLEAKMSGQ",      STS_MsgqMemoryLeakMsgq      },
    {"STS_MSGQMEMORYLEAKTRANSPORT", STS_MsgqMemoryLeakTransport },
#endif /* if defined (MSGQ_COMPONENT) */
    {NULL,                          NULL                        }
    /* NULL to mark end of list */
} ;


/** ============================================================================
 *  @func   TestSuite_Stress
 *
 *  @desc   This function takes the name of the function as an argument in
 *          string form and returns the function pointer to that function
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TestSuite_Stress (IN  Int32        argc,
                  IN  Pstr *       argv,
                  IN  TST_Attrs *  attrs,
                  IN  TST_State    testState,
                  OUT Result *     frameworkResult)
{
    DSP_STATUS    status                          = DSP_SOK ;
    Bool          check                           = FALSE   ;
    PtrToTestCase testCase                        = NULL    ;
    Uint16        i                               = 0       ;
    Int32         cmpResult                                 ;
    Char8 *       testName                                  ;
    Char8         testCaseUppercase [MAX_BUFSIZE]           ;
    TestInfo      testInfo                                  ;


    TST_PrnDebug ("Entered TestSuiteStress ()") ;

    DBC_Require (argc >= 1) ;
    DBC_Require (argv [0] != NULL) ;

    testName = argv [0] ;
    --argc ;
    ++argv ;

    testInfo.testSuite = "LinkStressTest" ;
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

    TST_PrnDebug ("Leaving TestSuiteStress ()") ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
