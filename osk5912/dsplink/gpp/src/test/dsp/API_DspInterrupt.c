/** ============================================================================
 *  @file   API_DspInterrupt.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\dsp
 *
 *  @desc   Implemetation of DSP_Interrupt API test case.
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
#include <dspdefs.h>
#include <linkdefs.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Link Driver                   */
#include <dsp.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <API_PostProcess.h>
#include <API_DspInterrupt.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   API_DspInterrupt
 *
 *  @desc   This test case tests the DSP_Interrupt  API.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_DspInterrupt (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS statusOfSetup    = DSP_SOK   ;
    DSP_STATUS status           = DSP_SOK   ;
    Uint32     processorId                  ;
    Uint16     intNum                       ;
    DspObject  dspObj                       ;
    Uint32     argForInterrupt              ;

    DBC_Require (argc == 4) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 4) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_DspInterrupt. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        processorId = TST_StringToInt (argv [2]) ;
        intNum = (Uint16) TST_StringToInt (argv [3]) ;

        dspObj.dspName [0]   = 'D'  ;
        dspObj.dspName [1]   = 'S'  ;
        dspObj.dspName [2]   = 'P'  ;
        dspObj.dspName [3]   = '_'  ;
        dspObj.dspName [4]   = '1'  ;
        dspObj.wordSize      = 2    ;
        dspObj.endian        = 1    ;
        dspObj.mmuFlag       = 1    ;
        dspObj.numMmuEntries = 0    ;
        dspObj.mmuTable      = NULL ;

        statusOfSetup = DSP_Setup (processorId, &dspObj) ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = DSP_Initialize (processorId, &dspObj) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Test DSP_Interrupt.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        status = DSP_Interrupt (processorId,
                                &dspObj,
                                intNum,
                                (Void *) &argForInterrupt) ;
    }

    status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
