/** ============================================================================
 *  @file   OSAL_IsrInstall.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\osal\kernel
 *
 *  @desc   Implemetation of ISR_Install API test case.
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
#include <OSAL_IsrInstall.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


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
 *  @func   OSAL_IsrInstall
 *
 *  @desc   Tests the ISR_Install API.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
OSAL_IsrInstall (IN Uint32 argc, IN Char8 ** argv)
{
    DSP_STATUS      statusOfSetup    = DSP_SOK   ;
    DSP_STATUS      status           = DSP_SOK   ;
    IsrObject   *   isrObj           = NULL      ;
    InterruptObject vector                       ;
    Int32           intId                        ;

    DBC_Require (argc == 3) ;
    DBC_Require (argv != NULL) ;

    if ((argc != 3) || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
        TST_PrnError ("Incorrect usage of OSAL_IsrInstall. Status = [0x%x]",
                      status) ;
    }

    if (DSP_SUCCEEDED (status)) {

        /*  --------------------------------------------------------------------
         *  Get arguments.
         *  --------------------------------------------------------------------
         */

        intId = TST_StringToInt (argv [2]) ;

        /*  --------------------------------------------------------------------
         *  Initialize.
         *  --------------------------------------------------------------------
         */

        statusOfSetup = ISR_Initialize () ;

        if (DSP_SUCCEEDED (statusOfSetup)) {
            vector.intId  = intId ;
            statusOfSetup = ISR_Create (DummyIsrFunct,
                                        (Pvoid) &intId,
                                        &vector,
                                        &isrObj) ;
        }
    }

    /*  --------------------------------------------------------------------
     *  Tests ISR_Install.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        if (intId < 0) {
            status = ISR_Install (NULL, NULL) ;
        }
        else {
            status = ISR_Install (NULL, isrObj) ;
        }
    }

    status = OSAL_PostProcess (status, argv [1], statusOfSetup, argv [0]) ;

    ISR_Uninstall (isrObj) ;
    ISR_Delete (isrObj) ;
    ISR_Finalize () ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
