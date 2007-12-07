/** ============================================================================
 *  @file   ANA_ProcLoad.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\analysis
 *
 *  @desc   Implements a test case which tests the Proc load performance.
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
#include <TST_Analysis.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <ANA_ProcLoad.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   ANA_ProcLoad
 *
 *  @desc   Tests the Proc load performance.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
ANA_ProcLoad (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      status            = DSP_SOK ;
    Uint32          processorId       = 0       ;
    Uint32          stopTime          = 0       ;
    Uint32          iterations        = 0       ;
    Uint32          count             = 0       ;
    Uint32          sizeOfFile        = 0       ;
    Real32          throughput        = 0       ;
    Real32          timeToLoad        = 0       ;

    DBC_Require (argc != 3) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 3) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of ANA_ProcLoad. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {
        processorId     = TST_StringToInt (argv [0]) ;
        iterations      = TST_StringToInt (argv [2]) ;
        status = PROC_Setup () ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (processorId, NULL) ;
    }


    if (DSP_SUCCEEDED (status)) {

        TST_ZeroTime () ;

        for (count = 0 ;   (count < iterations)
                       && (DSP_SUCCEEDED (status)); count++) {
            status = PROC_Load (processorId, argv [1], 0, NULL) ;
        }
        if (DSP_FAILED (status)) {
            TST_PrnError ("Proc Load Failed. Status = [0x%x]", status) ;
        }

        stopTime = TST_GetTime () ;
    }

    if (DSP_SUCCEEDED (status)) {
        sizeOfFile = TST_GetFileSize (argv [1], &sizeOfFile) ;
        timeToLoad = (stopTime / TICS_PER_SEC) ;
        TST_PrnInfo ("Time taken to load file = %.2f sec(s)", timeToLoad) ;

        if (stopTime != 0) {
            throughput =    (Real32) (sizeOfFile * TICS_PER_SEC * iterations)
                          / stopTime ;
            TST_PrnInfo ("Throughput = %.2f Bytes/Sec", throughput) ;
        }
        else {
            TST_PrnInfo ("Negligible data transfer time.") ;
        }
    }

    PROC_Detach (processorId) ;
    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
