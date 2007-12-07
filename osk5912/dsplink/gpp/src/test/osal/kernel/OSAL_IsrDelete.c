/** ============================================================================
 *  @file   OSAL_IsrDelete.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of ISR_Delete  API test case.
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
#include <OSAL_PostProcess.h>
#include <OSAL_IsrDelete.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   ISR_TEST_COUNT
 *
 *  @desc   Maximum no of ISR's which will be created
 *  ============================================================================
 */
#define ISR_TEST_COUNT 300


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
 *  @func   OSAL_IsrDelete
 *
 *  @desc   Tests the ISR_Delete API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_IsrDelete (IN Uint32 argc, IN Char8 ** argv)
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
        TST_PrnError ("Incorrect usage of OSAL_IsrDelete. Status = [0x%x]",
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

        if (DSP_SUCCEEDED (statusOfSetup)) {
            for (i = 0 ;   (i < noOfIsr)
                        && (DSP_SUCCEEDED (statusOfSetup))
                        && (i < ISR_TEST_COUNT) ; i++) {
                vector.intId = i ;
                statusOfSetup = ISR_Create (DummyIsrFunct,
                                           (Pvoid) noOfIsr,
                                           &vector,
                                           &isrObj [i]) ;
            }
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests ISR_Delete.
     *  --------------------------------------------------------------------
     */

    if (DSP_SUCCEEDED (status)) {
        if (noOfIsr < 0) {
            status = ISR_Delete (NULL) ;
        }
        else {
            for (i = 0 ;   (i < noOfIsr)
                        && (i < ISR_TEST_COUNT)
                        && (DSP_SUCCEEDED (status)) ; i++) {
                status = ISR_Delete (isrObj [i]) ;
            }
        }
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    ISR_Finalize () ;
    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
