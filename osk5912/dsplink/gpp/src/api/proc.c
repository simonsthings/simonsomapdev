/** ============================================================================
 *  @file   proc.c
 *
 *  @path   $(DSPLINK)\gpp\src\api
 *
 *  @desc   Implementation of API sub-component PROC.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ----------------------------------- OSAL Headers                */
#include <drv_pmgr.h>
#include <drv_api.h>
#include <print.h>

/*  ----------------------------------- User API                    */
#include <proc.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent ID.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_API_PROC

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason
 *  ============================================================================
 */
#define SET_FAILURE_REASON  \
            TRC_3PRINT (TRC_LEVEL7, \
                        "\nFailure: Status:[0x%x] File:[0x%x] Line:[%d]\n", \
                        status, FID_C_API_PROC, __LINE__)


/** ============================================================================
 *  @name   DRV_Handle
 *
 *  @desc   OS specific Link Driver object
 *  ============================================================================
 */
DRV_Object   * DRV_Handle = NULL ;


/** ============================================================================
 *  @func   PROC_Setup
 *
 *  @desc   Sets up the necessary data structures for the PROC sub-component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Setup ()
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;
    CMD_Args   args                ;

    TRC_0ENTER ("PROC_Setup") ;

    status = DRV_INITIALIZE (&DRV_Handle) ;
    if (DSP_SUCCEEDED (status)) {
        status = DRV_INVOKE (DRV_Handle, CMD_PROC_SETUP, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }

        tmpStatus = DRV_FINALIZE (DRV_Handle) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
        }
        DRV_Handle = NULL ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PROC_Setup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_Destroy
 *
 *  @desc   Destroys the data structures for the PROC component,
 *          allocated earlier by a call to PROC_Setup ().
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Destroy ()
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;
    CMD_Args   args                ;

    TRC_0ENTER ("PROC_Destroy") ;

    DBC_Assert (DRV_Handle == NULL) ;

    if (DRV_Handle == NULL) {
        status = DRV_INITIALIZE (&DRV_Handle) ;
    }

    if (DSP_SUCCEEDED (status)) {
        status = DRV_INVOKE (DRV_Handle, CMD_PROC_DESTROY, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }

        tmpStatus = DRV_FINALIZE (DRV_Handle) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
        DRV_Handle = NULL ;
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("PROC_Destroy", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_Attach
 *
 *  @desc   Attaches the client to the specified DSP and also
 *          initializes the DSP (if required).
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Attach (IN  ProcessorId    procId,
             OPT ProcAttr *     attr)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_2ENTER ("PROC_Attach", procId, attr) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (IS_VALID_PROCID (procId) == FALSE) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procAttachArgs.procId = procId ;
        args.apiArgs.procAttachArgs.attr   = attr   ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_ATTACH, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_Attach", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_Detach
 *
 *  @desc   Detaches the client from specified processor.
 *          If the caller is the owner of the processor, this function releases
 *          all the resources that this component uses and puts the DSP in an
 *          unusable state (from application perspective).
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Detach (IN  ProcessorId   procId)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_1ENTER ("PROC_Detach", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (IS_VALID_PROCID (procId) == FALSE) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procDetachArgs.procId = procId ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_DETACH, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_Detach", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_GetState
 *
 *  @desc   Gets the current status of DSP by querying the Link Driver.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_GetState (IN   ProcessorId procId,
               OUT  ProcState * procState)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_2ENTER ("PROC_GetState", procId, status) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (procState != NULL ) ;

    if ((IS_VALID_PROCID (procId) == FALSE) || (procState == NULL )) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procGetStateArgs.procId    = procId ;
        args.apiArgs.procGetStateArgs.procState = procState ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_GETSTATE, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_GetState", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_Load
 *
 *  @desc   Loads the specified DSP executable on the target DSP.
 *          It ensures that the caller owns the DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Load (IN   ProcessorId  procId,
           IN   Char8 *      imagePath,
           IN   Uint32       argc,
           IN   Char8 **     argv)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_4ENTER ("PROC_Load", procId, imagePath, argc, argv) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (imagePath != NULL ) ;
    DBC_Require (   ((argc != 0) && (argv != NULL))
                 || ((argc == 0) && (argv == NULL))) ;

    if (   (IS_VALID_PROCID (procId) == FALSE)
        || (imagePath == NULL)
        || (((argc == 0) && (argv != NULL)))
        || (((argc != 0) && (argv == NULL)))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procLoadArgs.procId    = procId    ;
        args.apiArgs.procLoadArgs.imagePath = imagePath ;
        args.apiArgs.procLoadArgs.argc      = argc      ;
        args.apiArgs.procLoadArgs.argv      = argv      ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_LOAD, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_Load", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_LoadSection
 *
 *  @desc   Loads the specified section of DSP executable onto the target DSP.
 *          It ensures that the client owns the DSP.
 *          Currently this API is not implemented.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_LoadSection (IN    ProcessorId  procId,
                  IN    Char8 *      imagePath,
                  IN    Uint32       sectID)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("PROC_LoadSection", procId, imagePath, sectID) ;

    status = DSP_ENOTIMPL ;
    SET_FAILURE_REASON ;

    TRC_1LEAVE ("PROC_LoadSection", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_Start
 *
 *  @desc   Starts execution of the loaded code on DSP from the starting
 *          point specified in the DSP executable loaded earlier by call to
 *          PROC_Load ().
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Start (IN  ProcessorId  procId)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_1ENTER ("PROC_Start", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (IS_VALID_PROCID (procId) == FALSE) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procStartArgs.procId = procId ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_START, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_Start", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_Stop
 *
 *  @desc   Stops the DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Stop (IN  ProcessorId  procId)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_1ENTER ("PROC_Stop", procId) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (IS_VALID_PROCID (procId) == FALSE) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procStopArgs.procId = procId ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_STOP, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_Stop", status) ;

    return status ;
}


/** ============================================================================
 *  @func   PROC_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations on
 *          the DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Control (IN  ProcessorId procId,
              IN  Int32       cmd,
              OPT Pvoid       arg)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_3ENTER ("PROC_Control", procId, cmd, arg) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (IS_VALID_PROCID (procId) == FALSE) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procControlArgs.procId = procId ;
        args.apiArgs.procControlArgs.cmd    = cmd ;
        args.apiArgs.procControlArgs.arg    = arg ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_CONTROL, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_Control", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   PROC_Instrument
 *
 *  @desc   Gets the instrumentation data associated with PROC sub-component.
 *
 *  @modif  None.
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
PROC_Instrument (IN ProcessorId procId, OUT ProcInstrument * retVal)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_2ENTER ("PROC_Instrument", procId, retVal) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (retVal != NULL) ;

    if ((IS_VALID_PROCID (procId) == FALSE) || (retVal == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        args.apiArgs.procInstrumentArgs.procId    = procId ;
        args.apiArgs.procInstrumentArgs.procStats = retVal ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_INSTRUMENT, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("PROC_Instrument", status) ;

    return status ;
}
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   PROC_Debug
 *
 *  @desc   Prints the debug information summarizing the current status
 *          of the PROC component.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
PROC_Debug (IN ProcessorId procId)
{
    DSP_STATUS status = DSP_SOK ;
    CMD_Args   args             ;

    TRC_0ENTER ("PROC_Debug") ;

    DBC_Require (IS_VALID_PROCID (procId)) ;

    if (IS_VALID_PROCID (procId)) {
        args.apiArgs.procDebugArgs.procId = procId ;

        status = DRV_INVOKE (DRV_Handle, CMD_PROC_DEBUG, &args) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_0LEAVE ("PROC_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
