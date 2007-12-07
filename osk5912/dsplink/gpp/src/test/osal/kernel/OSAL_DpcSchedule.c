/** ============================================================================
 *  @file   OSAL_DpcSchedule.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of DPC_Schedule API test case.
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

/*  ----------------------------------- OSAL Headers                  */
#include <dpc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <OSAL_PostProcess.h>
#include <OSAL_DpcSchedule.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   DPC_TEST_COUNT
 *
 *  @desc   Maximum no of DPC's which will be created
 *  ============================================================================
 */
#define DPC_TEST_COUNT 100

/** ----------------------------------------------------------------------------
 *  @func   DummyDpcFunct
 *
 *  @desc   This is a dummy function which is used while creating the DPC's.
 *
 *  @arg    arg
 *              Dummy argument which is passed to this dummy function
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
DummyDpcFunct (Pvoid dummy) ;


/** ----------------------------------------------------------------------------
 *  @func   DummyDpcFunct
 *
 *  @desc   This is a dummy function which is used while creating the DPC's.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
DummyDpcFunct (Pvoid dummy)
{
    *(Uint32 *) dummy = 0 ;
}


/** ============================================================================
 *  @func   OSAL_DpcSchedule
 *
 *  @desc   Tests the DPC_Schedule API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_DpcSchedule (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS    statusOfSetup    = DSP_SOK   ;
    DSP_STATUS    status           = DSP_SOK   ;
    Uint32        dpcVal           = 0         ;
    Int32         noOfDpc          = 0         ;
    DpcObject   * dpcObj [DPC_TEST_COUNT]      ;
    Int32         i                            ;

    DBC_Require (argc == 3) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 3) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_DpcSchedule. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        noOfDpc = TST_StringToInt (argv [2]) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = DPC_Initialize () ;

        for (i = 0 ; i < DPC_TEST_COUNT ; i++) {
            dpcObj [i] = NULL ;
        }

        if (DSP_SUCCEEDED (statusOfSetup)) {
            for (i = 0 ;   (i < noOfDpc)
                        && (DSP_SUCCEEDED (statusOfSetup))
                        && (i < DPC_TEST_COUNT) ; i++) {
                dpcVal = i ;
                statusOfSetup = DPC_Create (DummyDpcFunct,
                                           (Pvoid) &dpcVal,
                                           &dpcObj [i]) ;
            }

        }
    }

    /*  --------------------------------------------------------------------
     *  Tests DPC_Schedule.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        if (noOfDpc < 0) {
            status = DPC_Schedule (NULL) ;
        }
        else {
            for (i = 0 ;   (i < noOfDpc)
                        && (DSP_SUCCEEDED (status))
                        && (i < DPC_TEST_COUNT) ; i++) {
                status = DPC_Schedule (dpcObj [i]) ;
            }
        }
    }


    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;



    for (i = 0 ;   (i < noOfDpc)
                 && (i < DPC_TEST_COUNT) ; i++) {
        DPC_Delete (dpcObj [i]) ;
    }

    DPC_Finalize () ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
