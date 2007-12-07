/** ============================================================================
 *  @file   testsuiteOSAL.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\usr\Linux
 *
 *  @desc   Implemetation of TestSuite_Osal function.
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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Mem.h>
#include <TST_GetArgs.h>
#include <TST_FileOperation.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <testsuiteOSAL.h>
#include <OsalDriver.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TestSuite_Osal
 *
 *  @desc   This function decides which OSAL testcase to run.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TestSuite_Osal (IN  Int32        argc,
                IN  Char8 **     argv,
                IN  TST_Attrs *  attrs,
                IN  TST_State    testState,
                OUT Result *     frameworkResult)
{
    DSP_STATUS status                          = DSP_SOK ;
    DSP_STATUS statusFrmTest                   = DSP_SOK ;
    Bool       check                           = FALSE   ;
    Void     * pointer                         = NULL    ;
    int        osStatus                        = 0       ;
    int        testId                          = 0       ;
    int        fd                              = 0       ;
    ArrayPtr   args                            = NULL    ;
    Void  *    filePtr                         = NULL    ;
    Uint32     numArgs                         = 0       ;
    Uint32     i                               = 0       ;
    Bool       isScenarioIgnored               = FALSE   ;
    Char8 *    arguments [MAX_BUFSIZE]                   ;
    TST_Args   argToIoctl                                ;
    Int32      cmpResult                                 ;
    Char8 *    testName                                  ;
    Char8      testCaseUppercase [MAX_BUFSIZE]           ;
    TestInfo   testInfo                                  ;


    TST_PrnDebug ("Entered TestSuiteOSAL ()") ;

    DBC_Require (argc     > = 1)    ;
    DBC_Require (argv [0] ! = NULL) ;

    testName = argv [0] ;
    --argc              ;
    ++argv              ;

    testInfo.testSuite = "LinkOsalTest" ;
    testInfo.testCase  = testName       ;

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCLEAVECS",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_LeaveCS_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCENTERCS",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_EnterCS_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCDELETECS",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_DeleteCS_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCCREATECS",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_CreateCS_ID ;
        }
    }


    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCCLOSEEVENT",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_CloseEvent_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCWAITONEVENT",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_WaitOnEvent_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCRESETEVENT",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_ResetEvent_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCSETEVENT",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_SetEvent_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCOPENEVENT",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_OpenEvent_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCFINALIZE",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_Finalize_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_SYNCINITIALIZE",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = SYNC_Initialize_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase, "OSAL_DPCCREATE", &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = DPC_Create_ID ;
        }
    }

    if (DSP_SUCCEEDED (status)  && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase, "OSAL_DPCDELETE", &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = DPC_Delete_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_DPCFINALIZE",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = DPC_Finalize_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_DPCINITIALIZE",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = DPC_Initialize_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_DPCSCHEDULE",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = DPC_Schedule_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase, "OSAL_ISRCREATE", &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = ISR_Create_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase, "OSAL_ISRDELETE", &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = ISR_Delete_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_ISRINITIALIZE",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = ISR_Initialize_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_ISRFINALIZE",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = ISR_Finalize_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase, "OSAL_ISRINSTALL", &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = ISR_Install_ID ;
        }
    }

    if (DSP_SUCCEEDED (status) && check == FALSE) {
        TST_ToUpper (testName, testCaseUppercase) ;
        status = TST_Strcmp (testCaseUppercase,
                             "OSAL_ISRUNINSTALL",
                             &cmpResult) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Genstrcmp. Status = [0x%x]", status) ;
        }
        else if (cmpResult == 0) {
            check = TRUE ;
            testId = ISR_Uninstall_ID ;
        }
    }

    if (check == FALSE)  {
         status = DSP_EFAIL ;
         TST_PrnError ("Test not available. Status = [0x%x]", status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = TST_Alloc (&pointer, MAX_BUFFERS * MAX_BUFSIZE) ;
        args = (ArrayPtr) pointer ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = TST_OpenFile (argv [0], &filePtr) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Error Opening DataFile. Status = [0x%x]", status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        fd = open ("/dev/test_driver", O_RDONLY) ;
        if (fd < 0) {
            status = DSP_EFAIL ;
            TST_PrnError ("Driver open failed. Status = [0x%x]", status) ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        do {
            status = TST_GetArgs (filePtr,
                                  &numArgs,
                                  *args,
                                  &isScenarioIgnored) ;
            if ((DSP_SUCCEEDED (status)) && (numArgs > 0)) {
                for (i = 0 ; i < numArgs ; i++) {
                    arguments [i] = (*args) [i] ;
                }
                argToIoctl.argc = numArgs ;
                argToIoctl.argv = arguments ;
                TST_PrnTestCase (&testInfo) ;
                TST_PrnArgs (numArgs, arguments) ;
                frameworkResult->testCount++ ;
                if (   (testState == TST_RunAll)
                    && (isScenarioIgnored  == FALSE)) {

                    osStatus = ioctl (fd, testId, &argToIoctl) ;
                    statusFrmTest = argToIoctl.apiStatus ;
                    frameworkResult->testRun++  ;
                }
                else {
                    statusFrmTest = DSP_EFAIL ;
                    TST_PrnInfo ("This testcase is commented out") ;
                }
                TST_PrnStatus (statusFrmTest) ;
                if (DSP_SUCCEEDED (statusFrmTest)&& (osStatus == 0)) {
                    frameworkResult->passCount++ ;
                }
                else {
                    frameworkResult->failCount++ ;
                }
             }
        } while ((DSP_SUCCEEDED (status)) && (numArgs > 0) && (!osStatus)) ;
    }

    osStatus = close (fd) ;
    if (osStatus < 0) {
        status = DSP_EFAIL ;
        TST_PrnError ("Closing descriptor failed. Status = [0x%x]", status) ;
    }
    TST_Free (&pointer) ;


    TST_PrnDebug ("Leaving TestSuiteOsal ()") ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
