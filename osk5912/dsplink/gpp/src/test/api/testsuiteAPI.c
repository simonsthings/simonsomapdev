/** ============================================================================
 *  @file   testsuiteAPI.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of TestSuite_API function.
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
#include <testsuiteAPI.h>
#include <TST_RunTest.h>
#include <API_ChnlAllocateBuffer.h>
#include <API_ProcAttach.h>
#include <API_ProcControl.h>
#include <API_ProcLoad.h>
#include <API_ProcStart.h>
#include <API_ProcStop.h>
#include <API_ProcDetach.h>
#include <API_ProcGetstate.h>
#include <API_ProcInstrument.h>
#include <API_ChnlAllocateBuffer.h>
#include <API_ChnlFlush.h>
#include <API_ChnlCreate.h>
#include <API_ChnlControl.h>
#include <API_ChnlFreeBuffer.h>
#include <API_ChnlDelete.h>
#include <API_ChnlIssue.h>
#include <API_ChnlReclaim.h>
#include <API_ChnlIdle.h>
#include <API_MsgqAllocatorOpen.h>
#include <API_MsgqAllocatorClose.h>
#include <API_MsgqAlloc.h>
#include <API_MsgqFree.h>
#include <API_MsgqTransportOpen.h>
#include <API_MsgqTransportClose.h>
#include <API_MsgqCreate.h>
#include <API_MsgqDelete.h>
#include <API_MsgqLocate.h>
#include <API_MsgqRelease.h>
#include <API_MsgqPut.h>
#include <API_MsgqGet.h>
#include <API_MsgqGetReplyId.h>


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
    {"API_CHNLIDLE",           API_ChnlIdle          },
    {"API_CHNLRECLAIM",        API_ChnlReclaim       },
    {"API_CHNLISSUE",          API_ChnlIssue         },
    {"API_CHNLDELETE",         API_ChnlDelete        },
    {"API_CHNLFREEBUFFER",     API_ChnlFreeBuffer    },
    {"API_CHNLCONTROL",        API_ChnlControl       },
    {"API_CHNLCREATE",         API_ChnlCreate        },
    {"API_CHNLFLUSH",          API_ChnlFlush         },
    {"API_PROCGETSTATE",       API_ProcGetstate      },
    {"API_PROCDETACH",         API_ProcDetach        },
    {"API_PROCSTOP",           API_ProcStop          },
    {"API_PROCSTART",          API_ProcStart         },
    {"API_PROCLOAD",           API_ProcLoad          },
    {"API_PROCCONTROL",        API_ProcControl       },
    {"API_PROCATTACH",         API_ProcAttach        },
    {"API_CHNLALLOCATEBUFFER", API_ChnlAllocateBuffer},
    {"API_PROCINSTRUMENT",     API_ProcInstrument    },
#if defined (MSGQ_COMPONENT)
    {"API_MSGQALLOCATOROPEN",  API_MsgqAllocatorOpen },
    {"API_MSGQALLOCATORCLOSE", API_MsgqAllocatorClose},
    {"API_MSGQALLOC",          API_MsgqAlloc         },
    {"API_MSGQFREE",           API_MsgqFree          },
    {"API_MSGQTRANSPORTOPEN",  API_MsgqTransportOpen },
    {"API_MSGQTRANSPORTCLOSE", API_MsgqTransportClose},
    {"API_MSGQCREATE",         API_MsgqCreate        },
    {"API_MSGQDELETE",         API_MsgqDelete        },
    {"API_MSGQLOCATE",         API_MsgqLocate        },
    {"API_MSGQRELEASE",        API_MsgqRelease       },
    {"API_MSGQPUT",            API_MsgqPut           },
    {"API_MSGQGET",            API_MsgqGet           },
    {"API_MSGQGETREPLYID",     API_MsgqGetReplyId    },
#endif /* if defined (MSGQ_COMPONENT) */
    {NULL,                     NULL                  }
    /* NULL to mark end of list */
} ;


/** ============================================================================
 *  @func   TestSuite_Api
 *
 *  @desc   This function decides which API testcase to run.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TestSuite_Api (IN  Int32        argc,
               IN  Char8 **     argv,
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

    TST_PrnDebug ("Entered TestSuiteApi ()") ;

    DBC_Require (argc >= 1) ;
    DBC_Require (argv [0] != NULL) ;

    testName = argv [0] ;

    --argc ;
    ++argv ;

    testInfo.testSuite = "LinkApiTest" ;
    testInfo.testCase = testName ;
    TST_ToUpper (testName, testCaseUppercase) ;

    while (   DSP_SUCCEEDED (status)
           && (check == FALSE)
           && (TestList [i].name != NULL)) {

        status = TST_Strcmp (testCaseUppercase,
                             TestList [i].name,
                             &cmpResult) ;

        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testCase = TestList [i].function ;
        }
        i++ ;
    }

    if (check == FALSE)  {
         status = DSP_EFAIL ;
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

    TST_PrnDebug ("Leaving TestSuiteApi ()") ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
