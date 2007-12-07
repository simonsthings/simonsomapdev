/** ============================================================================
 *  @file   TST_RunTest.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\framework
 *
 *  @desc   Implemetation of TST_RunTest Function.
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
#include <TST_GetArgs.h>
#include <TST_RunTest.h>
#include <TST_PrintFuncs.h>
#include <TST_FileOperation.h>
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   TST_RunTest
 *
 *  @desc   This function runs the testcase.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_RunTest (IN Uint32         argc,
             IN Char8 **       argv,
             IN PtrToTestCase  funcPtr,
             IN TestInfo *     testName,
             IN TST_Attrs *    attrs,
             IN TST_State      testState,
             IN Result *       frameworkResult)
{
    DSP_STATUS status                          = DSP_SOK ;
    DSP_STATUS statusFrmTest                   = DSP_SOK ;
    ArrayPtr   args                            = NULL    ;
    Void     * pointer                         = NULL    ;
    Bool       isScenarioIgnored               = FALSE   ;
    Uint32     numArgs                                   ;
    Char8 *    arguments [MAX_BUFSIZE]                   ;
    Void  *    filePtr                                   ;
    Uint32     i                                         ;

    DBC_Require (argc == 1) ;
    DBC_Require (argv != NULL) ;
    DBC_Require (funcPtr != NULL) ;
    DBC_Require (testName != NULL) ;
    DBC_Require (frameworkResult != NULL) ;

    if (   (argc != 1)
        || (argv == NULL)
        || (funcPtr == NULL)
        || (testName == NULL)
        || (frameworkResult == NULL)) {
        status = DSP_EINVALIDARG ;
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
        do {
            status = TST_GetArgs (filePtr,
                                  &numArgs,
                                  *args,
                                  &isScenarioIgnored) ;
            if ((DSP_SUCCEEDED (status)) && (numArgs > 0)) {
                TST_PrnTestCase (testName) ;
                for (i = 0 ; i < numArgs ; i++) {
                    arguments [i] = (*args) [i] ;
                }
                TST_PrnArgs (numArgs, arguments) ;
                if (   (testState == TST_RunAll)
                    && (isScenarioIgnored  == FALSE)) {
                    statusFrmTest = funcPtr (numArgs, arguments) ;
                    frameworkResult->testRun++  ;
                }
                else {
                    statusFrmTest = DSP_EFAIL ;
                    TST_PrnInfo ("This testcase is commented out") ;
                }
                TST_PrnStatus (statusFrmTest) ;
                frameworkResult->testCount++ ;
                if (DSP_SUCCEEDED (statusFrmTest)) {
                    frameworkResult->passCount++ ;
                }
                else {
                    frameworkResult->failCount++ ;
                }
            }
        } while ((DSP_SUCCEEDED (status)) && (numArgs > 0)) ;
    }

    TST_Free (&pointer) ;
    TST_CloseFile (&filePtr) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
