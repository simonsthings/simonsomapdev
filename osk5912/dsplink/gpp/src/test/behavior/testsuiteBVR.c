/** ============================================================================
 *  @file   testsuiteBVR.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implemetation of TestSuite_Behavior function.
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

/*  ----------------------------------- Behavior Tests                */
#include <testsuiteBVR.h>
#include <BVR_StateTransition.h>
#include <BVR_SeqOpenClose.h>
#include <BVR_DoubleLoadImage.h>
#include <BVR_DataWrite.h>
#include <BVR_DataRead.h>
#include <BVR_Flush.h>
#include <BVR_Idle.h>
#include <BVR_EndOfStream.h>
#include <BVR_XferAfterIdle.h>
#include <BVR_ProcStart.h>
#include <BVR_MultiProcessAttach.h>
#include <BVR_MultiProcessOpen.h>
#include <BVR_MultiThreadOpen.h>
#include <BVR_ReclaimTimeout.h>
#include <BVR_DspSioDelete.h>
#include <BVR_MsgqDeleteOwner.h>
#include <BVR_MsgqGetTimeout.h>
#include <BVR_MsgqLocateTimeout.h>
#include <BVR_MsgqPriority.h>
#include <BVR_MsgqReceive.h>
#include <BVR_MsgqReceiveMultiThread.h>
#include <BVR_MsgqSend.h>
#include <BVR_MsgqSendMultiThread.h>
#include <BVR_MsgqStateTest.h>

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
    {"BVR_STATETRANSITION",        BVR_StateTransition       },
    {"BVR_SEQOPENCLOSE",           BVR_SeqOpenClose          },
    {"BVR_DOUBLELOADIMAGE",        BVR_DoubleLoadImage       },
    {"BVR_DATAWRITE",              BVR_DataWrite             },
    {"BVR_DATAREAD",               BVR_DataRead              },
    {"BVR_FLUSH",                  BVR_Flush                 },
    {"BVR_IDLE",                   BVR_Idle                  },
    {"BVR_MULTIPROCESSOPEN",       BVR_MultiProcessOpen      },
    {"BVR_PROCSTART",              BVR_ProcStart             },
    {"BVR_XFERAFTERIDLE",          BVR_XferAfterIdle         },
    {"BVR_ENDOFSTREAM",            BVR_EndOfStream           },
    {"BVR_MULTITHREADOPEN",        BVR_MultiThreadOpen       },
    {"BVR_MULTIPROCESSATTACH",     BVR_MultiProcessAttach    },
    {"BVR_RECLAIMTIMEOUT",         BVR_ReclaimTimeout        },
    {"BVR_DSPSIODELETE",           BVR_DspSioDelete          },
#if defined (MSGQ_COMPONENT)
    {"BVR_MSGQDELETEOWNER",        BVR_MsgqDeleteOwner       },
    {"BVR_MSGQGETTIMEOUT",         BVR_MsgqGetTimeout        },
    {"BVR_MSGQLOCATETIMEOUT",      BVR_MsgqLocateTimeout     },
    {"BVR_MSGQPRIORITY",           BVR_MsgqPriority          },
    {"BVR_MSGQRECEIVE",            BVR_MsgqReceive           },
    {"BVR_MSGQRECEIVEMULTITHREAD", BVR_MsgqReceiveMultiThread},
    {"BVR_MSGQSEND",               BVR_MsgqSend              },
    {"BVR_MSGQSENDMULTITHREAD",    BVR_MsgqSendMultiThread   },
    {"BVR_MSGQSTATETEST",          BVR_MsgqStateTest         },
#endif /* if defined (MSGQ_COMPONENT) */
    {NULL,                         NULL                      }
    /* NULL to mark end of list */
} ;


/** ============================================================================
 *  @func   TestSuite_Behavior
 *
 *  @desc   This function takes the name of the function as an argument in
 *          string form and returns the function pointer to that function
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TestSuite_Behavior (IN  Int32        argc,
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

    TST_PrnDebug ("Entered TestSuite_Behavior ()") ;

    DBC_Require (argc >= 1) ;
    DBC_Require (argv [0] != NULL) ;

    testName = argv [0] ;

    --argc ;
    ++argv ;

    testInfo.testSuite = "LinkBehaviorTest" ;
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

    TST_PrnDebug ("Leaving TestSuite_Behavior ()") ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
