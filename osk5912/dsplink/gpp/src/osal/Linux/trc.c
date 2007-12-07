/** ============================================================================
 *  @file   trc.c
 *
 *  @path   $(DSPLINK)\gpp\src\osal\Linux
 *
 *  @desc   Implemetation of sub-component TRC.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if defined (TRACE_ENABLE)


#if defined (TRACE_KERNEL)

/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <bitops.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>

/*  ----------------------------------- OSAL Headers                */
#include <print.h>
#include <trc.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_OSAL_TRC, __LINE__)


/** ============================================================================
 *  @const  COMP_BITS_NUM
 *
 *  @desc   Number of bits comprising component bitmask.
 *  ============================================================================
 */
#define COMP_BITS_NUM           16

/** ============================================================================
 *  @const  COMP_BITS_POS
 *
 *  @desc   Position of component bitmask.
 *  ============================================================================
 */
#define COMP_BITS_POS           16

/** ============================================================================
 *  @const  SUBCOMP_BITS_NUM
 *
 *  @desc   Number of bits comprising subcomponent bitmask.
 *  ============================================================================
 */
#define SUBCOMP_BITS_NUM        16

/** ============================================================================
 *  @const  SUBCOMP_BITS_POS
 *
 *  @desc   Position of subcomponent bitmask.
 *  ============================================================================
 */
#define SUBCOMP_BITS_POS        0


/** ============================================================================
 *  @macro  TRC_MATCH
 *
 *  @desc   Matches the specified component, subcomponent and severity with
 *          the global trace object.
 *  ============================================================================
 */
#define TRC_MATCH(trcObj,comp,subcomp,idxSubcomp,sev)                   \
        (    (((trcObj).subcomponents [idxSubcomp] & (subcomp)) != 0)   \
          && (((trcObj).components & (comp)) != 0)                      \
          && ((trcObj).level <= (sev)))


/** ============================================================================
 *  @name   TRC_ObjectData
 *
 *  @desc   Global TRC Object.
 *  ============================================================================
 */
STATIC TrcObject TRC_ObjectData ;


/** ============================================================================
 *  @func   TRC_0Print
 *
 *  @desc   Prints a null terminated character string based on its severity,
 *          the subcomponent and component it is associated with.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TRC_0Print (IN  Uint32   componentMap,
            IN  Uint16   severity,
            IN  Char8 *  debugString)
{
    Uint16 component    = 0 ;
    Uint16 subcomponent = 0 ;
    Uint16 idxSubcomp   = 0 ;

    DBC_Require (debugString != NULL ) ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    while (TEST_BIT (component, idxSubcomp) != TRUE) {
        idxSubcomp++ ;
    }

    if (TRC_MATCH (TRC_ObjectData,
                   component,
                   subcomponent,
                   idxSubcomp,
                   severity)) {
        PRINT_Printf ((Char8 *) debugString) ;
    }
}


/** ============================================================================
 *  @func   TRC_1Print
 *
 *  @desc   Prints a null terminated character string and an integer argument
 *          based on its severity, the subcomponent and component it is
 *          associated  with.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TRC_1Print (IN  Uint32   componentMap,
            IN  Uint16   severity,
            IN  Char8 *  debugString,
            IN  Uint32   argument1)
{
    Uint16 component    = 0 ;
    Uint16 subcomponent = 0 ;
    Uint16 idxSubcomp   = 0 ;

    DBC_Require (debugString != NULL ) ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    while (TEST_BIT (component, idxSubcomp) != TRUE) {
        idxSubcomp++ ;
    }

    if (TRC_MATCH (TRC_ObjectData,
                   component,
                   subcomponent,
                   idxSubcomp,
                   severity)) {
        PRINT_Printf ((Char8 *) debugString, argument1) ;
    }
}


/** ============================================================================
 *  @func   TRC_2Print
 *
 *  @desc   Prints a null terminated character string and two integer arguments
 *          based on its severity, the subcomponent and component it is
 *          associated  with.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TRC_2Print (IN  Uint32   componentMap,
            IN  Uint16   severity,
            IN  Char8 *  debugString,
            IN  Uint32   argument1,
            IN  Uint32   argument2)
{
    Uint16 component    = 0 ;
    Uint16 subcomponent = 0 ;
    Uint16 idxSubcomp   = 0 ;

    DBC_Require (debugString != NULL ) ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    while (TEST_BIT (component, idxSubcomp) != TRUE) {
        idxSubcomp++ ;
    }

    if (TRC_MATCH (TRC_ObjectData,
                   component,
                   subcomponent,
                   idxSubcomp,
                   severity)) {
        PRINT_Printf ((Char8 *) debugString, argument1, argument2) ;
    }
}


/** ============================================================================
 *  @func   TRC_3Print
 *
 *  @desc   Prints a null terminated character string and three integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TRC_3Print (IN  Uint32   componentMap,
            IN  Uint16   severity,
            IN  Char8 *  debugString,
            IN  Uint32   argument1,
            IN  Uint32   argument2,
            IN  Uint32   argument3)
{
    Uint16 component    = 0 ;
    Uint16 subcomponent = 0 ;
    Uint16 idxSubcomp   = 0 ;

    DBC_Require (debugString != NULL ) ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    while (TEST_BIT (component, idxSubcomp) != TRUE) {
        idxSubcomp++ ;
    }

    if (TRC_MATCH (TRC_ObjectData,
                   component,
                   subcomponent,
                   idxSubcomp,
                   severity)) {
        PRINT_Printf ((Char8 *) debugString, argument1, argument2, argument3) ;
    }
}


/** ============================================================================
 *  @func   TRC_4Print
 *
 *  @desc   Prints a null terminated character string and four integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TRC_4Print (IN  Uint32   componentMap,
            IN  Uint16   severity,
            IN  Char8 *  debugString,
            IN  Uint32   argument1,
            IN  Uint32   argument2,
            IN  Uint32   argument3,
            IN  Uint32   argument4)
{
    Uint16 component    = 0 ;
    Uint16 subcomponent = 0 ;
    Uint16 idxSubcomp   = 0 ;

    DBC_Require (debugString != NULL ) ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    while (TEST_BIT (component, idxSubcomp) != TRUE) {
        idxSubcomp++ ;
    }

    if (TRC_MATCH (TRC_ObjectData,
                   component,
                   subcomponent,
                   idxSubcomp,
                   severity)) {
        PRINT_Printf ((Char8 *) debugString,
                      argument1,
                      argument2,
                      argument3,
                      argument4) ;
    }
}


/** ============================================================================
 *  @func   TRC_5Print
 *
 *  @desc   Prints a null terminated character string and five integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TRC_5Print (IN  Uint32   componentMap,
            IN  Uint16   severity,
            IN  Char8 *  debugString,
            IN  Uint32   argument1,
            IN  Uint32   argument2,
            IN  Uint32   argument3,
            IN  Uint32   argument4,
            IN  Uint32   argument5)
{
    Uint16 component    = 0 ;
    Uint16 subcomponent = 0 ;
    Uint16 idxSubcomp   = 0 ;

    DBC_Require (debugString != NULL ) ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    while (TEST_BIT (component, idxSubcomp) != TRUE) {
        idxSubcomp++ ;
    }

    if (TRC_MATCH (TRC_ObjectData,
                   component,
                   subcomponent,
                   idxSubcomp,
                   severity)) {
        PRINT_Printf ((Char8 *) debugString,
                      argument1,
                      argument2,
                      argument3,
                      argument4,
                      argument5) ;
    }
}


/** ============================================================================
 *  @func   TRC_6Print
 *
 *  @desc   Prints a null terminated character string and six integer
 *          arguments based on its severity, the subcomponent and component it
 *          is associated  with.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TRC_6Print (IN  Uint32   componentMap,
            IN  Uint16   severity,
            IN  Char8 *  debugString,
            IN  Uint32   argument1,
            IN  Uint32   argument2,
            IN  Uint32   argument3,
            IN  Uint32   argument4,
            IN  Uint32   argument5,
            IN  Uint32   argument6)
{
    Uint16 component    = 0 ;
    Uint16 subcomponent = 0 ;
    Uint16 idxSubcomp   = 0 ;

    DBC_Require (debugString != NULL ) ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    while (TEST_BIT (component, idxSubcomp) != TRUE) {
        idxSubcomp++ ;
    }

    if (TRC_MATCH (TRC_ObjectData,
                   component,
                   subcomponent,
                   idxSubcomp,
                   severity)) {
        PRINT_Printf ((Char8 *) debugString,
                      argument1,
                      argument2,
                      argument3,
                      argument4,
                      argument5,
                      argument6) ;
    }
}


/** ============================================================================
 *  @func   TRC_Enable
 *
 *  @desc   Enables debug printing.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TRC_Enable (IN  Uint32   componentMap)
{
    DSP_STATUS status       = DSP_SOK ;
    Uint16     component    = 0 ;
    Uint16     subcomponent = 0 ;
    Uint16     idxSubcomp   = 0       ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    if ((component <= MAX_COMPONENT) && (component >= MIN_COMPONENT)) {
        SET_BITS (TRC_ObjectData.components, component) ;

        if (subcomponent != 0) {
            while (TEST_BIT (component, idxSubcomp) != TRUE) {
                idxSubcomp++ ;
            }
            SET_BITS (TRC_ObjectData.subcomponents [idxSubcomp], subcomponent) ;
        }
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    return status ;
}


/** ============================================================================
 *  @func   TRC_Disable
 *
 *  @desc   Disables debug printing.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TRC_Disable (IN  Uint32   componentMap)
{
    DSP_STATUS status       = DSP_SOK ;
    Uint16     component    = 0 ;
    Uint16     subcomponent = 0 ;
    Uint16     idxSubcomp   = 0 ;

    subcomponent = (Uint16) GET_NBITS32 (componentMap,
                                         SUBCOMP_BITS_POS,
                                         SUBCOMP_BITS_NUM) ;
    component    = (Uint16) GET_NBITS32 (componentMap,
                                         COMP_BITS_POS,
                                         COMP_BITS_NUM) ;

    if ((component <= MAX_COMPONENT) && (component >= MIN_COMPONENT)) {

        if (subcomponent != 0) {
            while (TEST_BIT (component, idxSubcomp) != TRUE) {
                idxSubcomp++ ;
            }
            CLEAR_BITS (TRC_ObjectData.subcomponents [idxSubcomp], subcomponent) ;
        }
        else {
            CLEAR_BITS (TRC_ObjectData.components, component) ;
        }
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    return status ;
}


/** ============================================================================
 *  @func   TRC_SetSeverity
 *
 *  @desc   Sets the severity level of the required debug prints.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TRC_SetSeverity (IN Uint16   level)
{
    DSP_STATUS status = DSP_SOK ;

    if ((level <= TRC_LEVEL7) && (level >= TRC_ENTER)) {
        TRC_ObjectData.level = level ;
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* defined (TRACE_KERNEL) */


#endif /* defined (TRACE_ENABLE) */
