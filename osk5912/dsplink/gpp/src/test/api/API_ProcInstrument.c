/** ============================================================================
 *  @file   API_ProcInstrument.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\api
 *
 *  @desc   Implemetation of PROC_Instrument API test case.
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
#include <dsplink.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- Processor Manager             */
#include <proc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <API_PostProcess.h>
#include <API_ProcInstrument.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   API_ProcInstrument
 *
 *  @desc   Tests the PROC_Instrument API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
API_ProcInstrument (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS     statusOfSetup    = DSP_SOK   ;
    DSP_STATUS     status           = DSP_SOK   ;
    Uint32         processorId      = 0         ;

#if defined (DDSP_PROFILE)
    Int32          nullTest         = 0         ;
    ProcInstrument instrumentation              ;
#endif


    DBC_Require (argc == 4) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 3) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of API_ProcInstrument. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        processorId = TST_StringToInt (argv [2]) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */
        statusOfSetup = PROC_Setup () ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            statusOfSetup = PROC_Attach (processorId, NULL) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests PROC_Instrument.
     *  --------------------------------------------------------------------
     */

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        if (nullTest == 1) {
            status = PROC_Instrument (processorId, NULL) ;
        }
        else {
            status = PROC_Instrument (processorId, &instrumentation) ;
        }
    }
#endif

    status = API_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    PROC_Detach  (processorId) ;
    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
