/** ============================================================================
 *  @file   TST_Framework.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Implemetation of entry point to FrameWork function.
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
#include <TST_Initialize.h>
#include <TST_FileOperation.h>
#include <TST_PrintFuncs.h>
#include <TST_RunTestSuite.h>
#include <TST_Helper.h>
#include <TST_GetArgs.h>
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_Main
 *
 *  @desc   Main FrameWork function.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TST_Main (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS status                      = DSP_SOK ;
    Bool       check                       = FALSE   ;
    Pstr       modeForRunning                        ;
    Char8      modeUppercase [MAX_BUFSIZE]           ;
    Int32      cmpResult                             ;
    Result     testResult                            ;
    TST_Attrs  attrs                                 ;
    TST_State  testState                             ;

    TST_PrnDebug ("Entered TST_Main ()") ;

    DBC_Require (argv != NULL) ;

    --argc ;
    ++argv ;

    testResult.testCount = 0 ;
    testResult.testRun   = 0 ;
    testResult.passCount = 0 ;
    testResult.failCount = 0 ;

    testState = TST_RunAll ;

    /* Consume the mode */
    modeForRunning = *argv ;
    status = TST_Initialize () ;
    if (DSP_SUCCEEDED (status)) {
        /* Advance to after mode to filename or the testsuit name */
        --argc ;
        ++argv ;

        DBC_Assert (modeForRunning != NULL) ;
        status = TST_ToUpper (modeForRunning, modeUppercase) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Error in TST_ToUpper. Status = [0x%x]", status) ;
        }
    }

    /*  This is the case when the single option was used */
    if ((DSP_SUCCEEDED (status)) && (check == FALSE)) {
        TST_Strcmp (modeUppercase, "SINGLE", &cmpResult) ;
        if (cmpResult == 0) {
            check = TRUE ;
            status = TST_RunTestSuite (argc,
                                       argv,
                                       &attrs,
                                       testState,
                                       &testResult) ;
        }
    }

    /*  This is the case when the script option was used */
    if ((DSP_SUCCEEDED (status)) && (check == FALSE)) {
       TST_Strcmp (modeUppercase, "SCRIPT", &cmpResult) ;
       if (cmpResult == 0) {
           check = TRUE ;
           status = TST_ExecuteScript (argc,
                                       argv,
                                       &attrs,
                                       testState,
                                       &testResult) ;
       }
    }

    if (DSP_SUCCEEDED (status)) {
        if (check == FALSE) {
            TST_PrnInfo ("Option Should Be Single Or Script.") ;
        }
    }

    TST_PrnSummary (&testResult) ;

    status = TST_Finalize () ;
    if (DSP_FAILED (status)) {
        TST_PrnError ("Error in TST_Finalize. Status = [0x%x]", status) ;
    }
    TST_PrnDebug ("Leaving TST_Main ()") ;
}


/** ============================================================================
 *  @func   TST_ExecuteScript
 *
 *  @desc   This function is invoked with the script argument and it also
 *          invokes the desired testcase.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ExecuteScript (IN  Uint32       argc,
                   IN  Char8     ** argv,
                   IN  TST_Attrs *  attrs,
                   IN  TST_State    testState,
                   OUT Result    *  frameworkResult)
{
    DSP_STATUS    status                               = DSP_SOK ;
    Void        * pointer                              = NULL    ;
    Void        * filePtr                              = NULL    ;
    Uint32        i                                    = 0       ;
    Bool          isTestsuiteCommented                 = FALSE   ;
    ArrayPtr      arguments                                      ;
    Char8       * arrPtr [MAX_BUFSIZE]                           ;
    Char8       * scriptName                                     ;

    TST_PrnDebug ("Entered TST_ExecuteScript ()") ;

    DBC_Require (argv [0] != NULL) ;

    scriptName = argv [0] ;

    if (DSP_SUCCEEDED (status)) {
        status  = TST_Alloc (&pointer, MAX_BUFFERS * MAX_BUFSIZE) ;
        arguments = (ArrayPtr) pointer ;
    }

    if (argv [0] == NULL) {
        status = DSP_EINVALIDARG ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = TST_OpenFile (scriptName, &filePtr) ;
        if (DSP_FAILED (status)) {
            TST_PrnError ("Error Opening Script File. Status = [0x%x]",
                          status) ;
        }
    }

    while ((DSP_SUCCEEDED (status)) && (argc > 0) ) {
        testState = TST_RunAll ;
        status = TST_GetArgs (filePtr,
                              &argc,
                              *arguments,
                              &isTestsuiteCommented) ;
        if ((DSP_SUCCEEDED (status)) && (argc > 0)) {
            if (isTestsuiteCommented == TRUE ) {
                testState = TST_IgnoreTestCase ;
            }
            for (i = 0 ; i < argc ; i++) {
                arrPtr [i] = (*arguments) [i] ;
            }
            status = TST_RunTestSuite (argc,
                                       arrPtr,
                                       attrs,
                                       testState,
                                       frameworkResult) ;
        }
    }

    TST_CloseFile (&filePtr) ;

    TST_Free (&pointer) ;

    TST_PrnDebug ("Leaving TST_ExecuteScript ()") ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
