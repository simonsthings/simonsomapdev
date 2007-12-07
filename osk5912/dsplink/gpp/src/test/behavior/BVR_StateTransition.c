/** ============================================================================
 *  @file   BVR_StateTransition.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\behavior
 *
 *  @desc   Implmementation of behavior test to check state transition of DSP.
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
#include <BVR_StateTransition.h>
#include <BVR_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @func   BVR_StateTransition
 *
 *  @desc   Perform the state transition test.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
BVR_StateTransition (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS  status           = DSP_SOK ;
    DSP_STATUS  statusOfSetup    = DSP_SOK ;
    ProcessorId dspId            = 0       ;
    Pstr        transitionString = NULL    ;
    Pstr        fileName         = NULL    ;
    Char8       transition                 ;
    Uint32      position                   ;
    Uint32      count                      ;

    /*  --------------------------------------------------------------------
     *  Validate number of arguments
     *  --------------------------------------------------------------------
     */
    DBC_Require (argc == 5) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 5) && (argv != NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of BVR_StateTransition. Status = [0x%x]",
                      status) ;
    }

    /*  --------------------------------------------------------------------
     *  Get arguments and perform the pre-operations for test.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        dspId            = TST_StringToInt (argv [2]) ;
        fileName         = argv [3]                   ;
        transitionString = argv [4]                   ;
        statusOfSetup    = PROC_Setup ()              ;
    }

    /*  --------------------------------------------------------------------
     *  Test BVR_StateTransition.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        position = 0 ;
        count    = 0 ;
        while (   (transitionString [position] != '\0')
               && DSP_SUCCEEDED (status)) {

            transition = transitionString [position++] ;

            /* valid transitions: 0 - 9, a - z, A - Z and escape character '\'
             * On presence of other characters - error is flagged. '\' is used
             * to skip testing of return value and resultant state for the
             * transition following it.
             */
            if (   ((transition >= '0') && (transition <= '9'))
                || ((transition >= 'a') && (transition <= 'z'))
                || ((transition >= 'A') && (transition <= 'Z'))
                || (transition == '\\') ) {
                count++ ;

                switch (transition)
                {
                case '1': /* PROC_RESET */
                    status = PROC_Attach (dspId, NULL) ;
                    break ;
                case '2': /* PROC_IDLE */
                    status = PROC_Detach (dspId) ;
                    break ;
                case '3': /* PROC_LOADED */
                    status = PROC_Load (dspId, fileName, 0, NULL) ;
                    break ;
                case '4': /* PROC_STARTED */
                    status = PROC_Start (dspId) ;
                    break ;
                case '5': /* PROC_STOPPED */
                    status = PROC_Stop (dspId) ;
                    break ;
                default:
                    break ;
                }
            }
        }

        status = BVR_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

        PROC_Stop (dspId) ; /* Ignore the return code */
        PROC_Detach (dspId) ; /* Ignore the return code */
    }

    PROC_Destroy () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
