/** ============================================================================
 *  @file   BVR_SeqOpenClose.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implementation of behavior test to check CHNL behavior with
 *          sequence of multiple opens and closes.
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
#include <chnl.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <BVR_SeqOpenClose.h>
#include <BVR_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   DoSeqOpenCloseTest
 *
 *  @desc   Function to test the behavior when CHNL_Open and CHNL_Close is
 *          closed multiple times.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_SeqOpenClose (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  status        = DSP_SOK ;
    DSP_STATUS  statusOfSetup = DSP_SOK ;
    ProcessorId dspId         = 0       ;
    Int32       numOpens      = 0       ;
    Int32       i                       ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 3) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 3) && (argv != NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_SeqOpenClose. Status = [0x%x]",
                      status) ;
    }
    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        dspId    = TST_StringToInt (argv [1]) ;
        numOpens = TST_StringToInt (argv [2]) ;
        statusOfSetup = PROC_Setup () ;
    }
    /*  --------------------------------------------------------------------
     *  Test BVR_SeqOpenClose.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; DSP_SUCCEEDED (status) && (i < numOpens) ; i++) {
            status = PROC_Attach (dspId, NULL) ;

            if (DSP_SUCCEEDED (status)) {
               status = PROC_Detach (dspId) ;
            }
            else {
                TST_PrnError ("Couldn't perform PROC_Attach. Status = [0x%x]",
                              status) ;
            }

            if (DSP_FAILED (status)) {
                TST_PrnError ("Couldn't perform PROC_Detach. Status = [0x%x]",
                              status) ;
            }
        }
    }

    status = BVR_PostProcess (status, argv [0], statusOfSetup, "DSP_SOK") ;

    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
