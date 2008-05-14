/** ============================================================================
 *  @file   probe.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines probe-related types, data structures and APIs.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PROBE_H)
#define PROBE_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)
USES (errbase.h)


/** ============================================================================
 *  @name   ProbeUserAppData
 *
 *  @desc   This structure specifies the probe data to be provided by
 *          the application for the probe analysis.
 *
 *  @field  bufSize
 *              The size of the buffer being transferred by the application.
 *  @field  numBufs
 *              The number of buffers being transferred by the application.
 *  @field  appStartTime
 *              The start time of the application as seen from user side.
 *  @field  appEndTime
 *              The end time of the application as seen from user side.
 *
 *  @see    None
 *  ============================================================================
 */
typedef struct ProbeUserAppData_tag {
    Uint32  bufSize ;
    Uint32  numBufs ;

    Real32  appStartTime ;
    Real32  appEndTime ;
} ProbeUserAppData ;


/** ============================================================================
 *  @func   PROBE_UpdateAppData
 *
 *  @desc   This function allows the user to set up application-specific fields
 *          required for probe.
 *
 *  @arg    userAppData
 *              pointer to the structure containing data provided by the user
 *              application.
 *
 *  @ret    None.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
Void
PROBE_UpdateAppData (IN  ProbeUserAppData * userAppData) ;


/** ============================================================================
 *  @func   PROBE_Start
 *
 *  @desc   This function enables logging the probe values.
 *
 *  @arg    None.
 *
 *  @ret    None.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
Void
PROBE_Start (Void) ;


/** ============================================================================
 *  @func   PROBE_Stop
 *
 *  @desc   This function disables logging the probe values.
 *
 *  @arg    None.
 *
 *  @ret    None.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ============================================================================
 */
EXPORT_API
Void
PROBE_Stop (Void) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !define (PROBE_H) */
