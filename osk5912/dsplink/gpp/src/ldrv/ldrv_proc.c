/** ============================================================================
 *  @file   ldrv_proc.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Functionality implementation of the PROC sub-component in LDRV.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ------------------------------------- DSP/BIOS Link             */
#include <gpptypes.h>
#include <dsplink.h>
#include <intobject.h>
#include <errbase.h>
#include <safe.h>

#include <dspdefs.h>
#include <linkdefs.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ------------------------------------- Trace & Debug             */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ------------------------------------- OSAL Headers              */
#include <mem.h>
#include <prcs.h>
#include <sync.h>
#include <print.h>

/*  ------------------------------------- Generic Functions         */
#include <gen_utils.h>
#include <list.h>

/*  ------------------------------------- Link Driver               */
#include <ldrv.h>
#include <ldrv_proc.h>
#if defined (CHNL_COMPONENT)
#include <ldrv_chnl.h>
#endif /* if defined (CHNL_COMPONENT) */


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent ID.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_PROC


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_PROC, __LINE__)


/** ============================================================================
 *  @name   procState
 *
 *  @desc   Array of current processor states.
 *  ============================================================================
 */
STATIC ProcState  procStates [MAX_PROCESSORS] ;


/** ============================================================================
 *  @macro  LDRV_Obj
 *
 *  @desc   Global defined in ldrv.c.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;


/** ============================================================================
 *  @func   LDRV_PROC_Initialize
 *
 *  @desc   Allocates resources at GPP side that are required for using DSP.
 *          It also sets up the connection to DSP from the GPP and other
 *          associated peripheral hardware.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Initialize (IN ProcessorId dspId)
{
    DSP_STATUS      status   = DSP_SOK ;
    DspInterface *    dspIntf  = NULL    ;
    DspObject *     dspObj   = NULL    ;

    TRC_1ENTER ("LDRV_PROC_Initialize", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    status = (*(dspIntf->setup)) (dspId, dspObj) ;
    if (DSP_SUCCEEDED (status)) {
        status = (*(dspIntf->initialize)) (dspId, dspObj) ;
        if (DSP_SUCCEEDED (status)) {
            procStates [dspId] = ProcState_Reset ;
        }
    }

    if (DSP_FAILED (status)) {
        procStates [dspId] = ProcState_Unknown ;
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        /* Increment the count of active links */
        (LDRV_Obj.procStats.procData[dspId].activeLinks)++ ;
    }
#endif

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (procStates [dspId] == ProcState_Reset))
                || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("LDRV_PROC_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_Finalize
 *
 *  @desc   Releases all the resources created on GPP side for specified DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Finalize (IN ProcessorId dspId)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspInterface * dspIntf = NULL    ;
    DspObject *          dspObj   = NULL    ;

    TRC_1ENTER ("LDRV_PROC_Finalize", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (procStates [dspId] != ProcState_Unknown) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    if (procStates [dspId] != ProcState_Unknown) {
        status = (*(dspIntf->finalize)) (dspId, dspObj) ;
        if (DSP_FAILED (status)) {
            procStates [dspId] = ProcState_Unknown ;
            SET_FAILURE_REASON ;
        }
        else {
            procStates [dspId] = ProcState_Idle ;
        }
    }
    else {
        status = DSP_EWRONGSTATE ;
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (procStates [dspId] == ProcState_Idle))
                || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("LDRV_PROC_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_Start
 *
 *  @desc   Starts execution of DSP from specified location.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Start (IN ProcessorId dspId, IN Uint32 dspAddr)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspInterface *       dspIntf  = NULL    ;
    DspObject *          dspObj   = NULL    ;

    TRC_2ENTER ("LDRV_PROC_Start", dspId, dspAddr) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (procStates [dspId] == ProcState_Loaded)
                 || (procStates [dspId] == ProcState_Stopped)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    if (   (procStates [dspId] == ProcState_Loaded)
        || (procStates [dspId] == ProcState_Stopped)) {

#if defined (CHNL_COMPONENT)
        status = LDRV_CHNL_HandshakeSetup (dspId) ;
#endif  /* if defined (CHNL_COMPONENT) */

        if (DSP_SUCCEEDED (status)) {
            status = (*(dspIntf->start)) (dspId, dspObj, dspAddr) ;
        }
        else {
            SET_FAILURE_REASON ;
        }

#if defined (CHNL_COMPONENT)
        if (DSP_SUCCEEDED (status)) {
            status = LDRV_CHNL_Handshake (dspId) ;
        }
        else {
            SET_FAILURE_REASON ;
        }
#endif  /* if defined (CHNL_COMPONENT) */

        if (DSP_SUCCEEDED (status)) {
            procStates [dspId] = ProcState_Started ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = DSP_EWRONGSTATE ;
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (procStates [dspId] == ProcState_Started))
                || (DSP_FAILED (status))) ;


    TRC_1LEAVE ("LDRV_PROC_Start", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_Stop
 *
 *  @desc   Stops the execution of DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Stop (IN ProcessorId dspId)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspInterface * dspIntf = NULL    ;
    DspObject *          dspObj   = NULL    ;

    TRC_1ENTER ("LDRV_PROC_Stop", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (procStates [dspId] == ProcState_Started)
                 || (procStates [dspId] == ProcState_Stopped)
                 || (procStates [dspId] == ProcState_Idle)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    if (   (procStates [dspId] == ProcState_Started)
        || (procStates [dspId] == ProcState_Stopped)
        || (procStates [dspId] == ProcState_Idle)) {
        status = (*(dspIntf->stop)) (dspId, dspObj) ;
        if (DSP_FAILED (status)) {
            procStates [dspId] = ProcState_Unknown ;
            SET_FAILURE_REASON ;
        }
        else {
            procStates [dspId] = ProcState_Stopped ;
        }
    }
    else {
        status = DSP_EWRONGSTATE ;
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (procStates [dspId] == ProcState_Stopped))
                || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("LDRV_PROC_Stop", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_Idle
 *
 *  @desc   Puts the processor in idle mode, which means that read and write
 *          can be done to the processor.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Idle (IN ProcessorId dspId)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspInterface * dspIntf = NULL    ;
    DspObject *          dspObj   = NULL    ;

    TRC_1ENTER ("LDRV_ProcState_Idle", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (procStates [dspId] != ProcState_Unknown) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    if (procStates [dspId] != ProcState_Unknown) {
        status = (*(dspIntf->idle)) (dspId, dspObj) ;
        if (DSP_FAILED (status)) {
            procStates [dspId] = ProcState_Unknown ;
            SET_FAILURE_REASON ;
        }
        else {
            procStates [dspId] = ProcState_Idle ;
        }
    }
    else {
        status = DSP_EWRONGSTATE ;
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (procStates [dspId] == ProcState_Idle))
                || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("LDRV_ProcState_Idle", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_Read
 *
 *  @desc   Reads from the DSP's memory space.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Read (IN     ProcessorId   dspId,
                IN     Uint32        dspAddr,
                IN     Endianism     endianInfo,
                IN OUT Uint32 *      numBytes,
                OUT    Uint8 *       buffer)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspInterface * dspIntf = NULL    ;
    DspObject *          dspObj   = NULL    ;

    TRC_5ENTER ("LDRV_PROC_Read",
                dspId,
                dspAddr,
                endianInfo,
                numBytes,
                buffer) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   (procStates [dspId] != ProcState_Unknown)
                 && (procStates [dspId] != ProcState_Reset)) ;
    DBC_Require (buffer != NULL) ;
    DBC_Require (numBytes != NULL) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    if (   (procStates [dspId] != ProcState_Unknown)
        && (procStates [dspId] != ProcState_Reset)) {
        status = (*(dspIntf->read)) (dspId,
                                      dspObj,
                                      dspAddr,
                                      endianInfo,
                                      numBytes,
                                      buffer) ;
        if (DSP_FAILED (status)) {
            procStates [dspId] = ProcState_Unknown ;
            SET_FAILURE_REASON ;
        }
#if defined (DDSP_PROFILE)
        if (DSP_SUCCEEDED (status)) {
            LDRV_Obj.procStats.procData [dspId].dataFromDsp += (*numBytes) ;
        }
#endif /* defined (DDSP_PROFILE) */

    }
    else {
        status = DSP_EWRONGSTATE ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("LDRV_PROC_Read", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_Write
 *
 *  @desc   Writes to DSP's memory space.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Write (IN  ProcessorId    dspId,
                 IN  Uint32         dspAddr,
                 IN  Endianism      endianInfo,
                 IN  Uint32         numBytes,
                 IN  Uint8 *        buffer)
{
    DSP_STATUS           status   = DSP_SOK ;
    DspInterface * dspIntf = NULL    ;
    DspObject *          dspObj   = NULL    ;

    TRC_5ENTER ("LDRV_PROC_Write",
                dspId,
                dspAddr,
                endianInfo,
                numBytes,
                buffer) ;


    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (buffer != NULL) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;

    if (procStates [dspId] != ProcState_Unknown) {
        status = (*(dspIntf->write)) (dspId,
                                       dspObj,
                                       dspAddr,
                                       endianInfo,
                                       numBytes,
                                       buffer) ;
        if (DSP_FAILED (status)) {
            procStates [dspId] = ProcState_Unknown ;
            SET_FAILURE_REASON ;
        }
        else {
#if defined (DDSP_PROFILE)
            LDRV_Obj.procStats.procData [dspId].dataToDsp += numBytes ;
#endif /* defined (DDSP_PROFILE) */
            procStates [dspId] = ProcState_Loaded ;
        }
    }
    else {
        status = DSP_EWRONGSTATE ;
        SET_FAILURE_REASON ;
    }

    DBC_Ensure (   (   (DSP_SUCCEEDED (status))
                    && (procStates [dspId] == ProcState_Loaded))
                || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("LDRV_PROC_Write", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_GetState
 *
 *  @desc   Returns the current processor state.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_GetState (IN ProcessorId dspId, OUT ProcState * procState)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("LDRV_PROC_GetState", dspId, procState) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (procState != NULL) ;

    *procState = procStates [dspId] ;

    TRC_1LEAVE ("LDRV_PROC_GetState", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_SetState
 *
 *  @desc   Sets the current state of processor to the specified state.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_SetState (IN ProcessorId dspId, IN ProcState procState)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("LDRV_PROC_SetState", dspId, procState) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    procStates [dspId] = procState ;

    TRC_1LEAVE ("LDRV_PROC_SetState", status) ;

    return status ;
}


/** ============================================================================
 *  @func   LDRV_PROC_Control
 *
 *  @desc   Provides a hook to perform device dependent control operations.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Control (IN  ProcessorId dspId,
                   IN  Int32       cmd,
                   OPT Pvoid       arg)
{
    DSP_STATUS status  = DSP_SOK ;

    TRC_3ENTER ("LDRV_PROC_Control", dspId, cmd, arg) ;

    status = DSP_ENOTIMPL ;

    TRC_1LEAVE ("LDRV_PROC_Control", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   LDRV_PROC_Instrument
 *
 *  @desc   Gets the instrumentation information related to the specified
 *          DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LDRV_PROC_Instrument (IN ProcessorId procId, OUT ProcInstrument * retVal)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("LDRV_PROC_Instrument", retVal) ;

    DBC_Require (IS_VALID_PROCID (procId)) ;
    DBC_Require (retVal != NULL) ;

    *retVal = LDRV_Obj.procStats.procData [procId] ;

    TRC_1LEAVE ("LDRV_PROC_Instrument", status) ;

    return status ;
}
#endif


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   LDRV_PROC_Debug
 *
 *  @desc   Prints out debug information of LDRV_PROC module.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LDRV_PROC_Debug (IN ProcessorId procId)
{
    TRC_1ENTER ("LDRV_PROC_Debug", procId) ;

    TRC_2PRINT (TRC_LEVEL1,
                "Processor Number %d, Current State = %d\n",
                procId, procStates [procId]) ;

    TRC_0LEAVE ("LDRV_PROC_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
