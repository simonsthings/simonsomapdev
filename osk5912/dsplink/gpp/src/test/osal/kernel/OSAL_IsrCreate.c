/** ============================================================================
 *  @file   OSAL_IsrCreate.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of ISR_Create  API test case.
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
#include <intobject.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- OSAL Headers                  */
#include <isr.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Helper.h>
#include <TST_PrintFuncs.h>
#include <OSAL_IsrCreate.h>
#include <OSAL_PostProcess.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   ISR_TEST_COUNT
 *
 *  @desc   Maximum no of ISR's which will be created
 *  ============================================================================
 */
#define ISR_TEST_COUNT 500


/** ----------------------------------------------------------------------------
 *  @func   DummyIsrFunct
 *
 *  @desc   This is a dummy function which is used while creating the ISR's.
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
Void DummyIsrFunct (Pvoid dummy) ;


/** ----------------------------------------------------------------------------
 *  @func   DummyIsrFunct
 *
 *  @desc   This is a dummy function which is used while creating the ISR's.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void DummyIsrFunct (Pvoid dummy)
{
    *(Uint32 *)dummy = 0 ;
}


/** ============================================================================
 *  @func   OSAL_IsrCreate
 *
 *  @desc   Tests the ISR_Create API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_IsrCreate (IN Uint32 argc, IN Char8 ** argv )
{
    DSP_STATUS    statusOfSetup    = DSP_SOK   ;
    DSP_STATUS    status           = DSP_SOK   ;
    Int32         noOfIsr          = 0         ;
    InterruptObject vector                       ;
    IsrObject   *   isrObj [ISR_TEST_COUNT]      ;
    Int32         i                            ;

    DBC_Require (argc == 3) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 3) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_IsrCreate. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        noOfIsr = TST_StringToInt (argv [2]) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = ISR_Initialize () ;

        for (i = 0 ; i < ISR_TEST_COUNT ; i++) {
            isrObj [i] = NULL ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests ISR_Create.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        if (noOfIsr < 0) {
            status = ISR_Create (NULL, NULL, NULL, NULL) ;
        }
        else {
            for (i = 0 ;   (i < noOfIsr)
                        && (DSP_SUCCEEDED (status))
                        && (i < ISR_TEST_COUNT) ; i++) {
                vector.intId = i ;
                status = ISR_Create (DummyIsrFunct,
                                    (Pvoid) noOfIsr,
                                    &vector,
                                    &isrObj [i]) ;
            }
        }
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    for (i = 0 ;   (i < noOfIsr)
                 && (i < ISR_TEST_COUNT) ; i++) {
        ISR_Delete (isrObj [i]) ;
    }

    ISR_Finalize () ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
