/** ============================================================================
 *  @file   probedefs.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines data types and structures used by DSP/BIOS(TM) LINK for
 *          probe
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PROBEDEFS_H)
#define PROBEDEFS_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)
USES (errbase.h)


/** ============================================================================
 *  @const  NUM_PROBES
 *
 *  @desc   Defines the number of probe points to be used for calibration of the
 *          PROBE component.
 *  ============================================================================
 */
#define NUM_PROBES 1000

/** ============================================================================
 *  @const  NUM_PROBE_POINTS
 *
 *  @desc   Defines the number of probe points used.
 *  ============================================================================
 */
#define NUM_PROBE_POINTS 24

/** ============================================================================
 *  @const  PROBE_MAX_ROWS
 *
 *  @desc   Defines the maximum number of memory locations allocated for storing
 *          PROBE points.
 *  ============================================================================
 */
#define PROBE_MAX_ROWS   1000


/** ============================================================================
 *  @name   PROBE_Event
 *
 *  @desc   This enumeration defines the types of events associated with
 *          gathering analysis data when data transfer / message exchange is
 *          happening between the GPP and the DSP.
 *
 *  @field  ProbeEventUIssueEntry
 *              Indicates the CHNL_Issue entry point from the User API.
 *  @field  ProbeEventUIssueLeave
 *              Indicates the CHNL_Issue exit  point from the User API.
 *  @field  ProbeEventUPutEntry
 *              Indicates the MSGQ_Put entry point from the User API.
 *  @field  ProbeEventUPutLeave
 *              Indicates the MSGQ_Put exit  point from the User API.
 *  @field  ProbeEventUReclaimEntry
 *              Indicates the CHNL_Reclaim entry point from the User API.
 *  @field  ProbeEventUReclaimLeave
 *              Indicates the CHNL_Reclaim exit  point from the User API.
 *  @field  ProbeEventUGetEntry
 *              Indicates the MSGQ_Get entry point from the User API.
 *  @field  ProbeEventUGetLeave
 *              Indicates the MSGQ_Get exit  point from the User API.
 *  @field  ProbeEventKIssueEntry
 *              Indicates the CHNL_Issue entry point in the kernel context.
 *  @field  ProbeEventKIssueLeave
 *              Indicates the CHNL_Issue exit  point in the kernel context.
 *  @field  ProbeEventKPutEntry
 *              Indicates the MSGQ_Put entry point in the kernel context.
 *  @field  ProbeEventKPutLeave
 *              Indicates the MSGQ_Put exit  point in the kernel context.
 *  @field  ProbeEventKReclaim1Entry
 *              Indicates the CHNL_Reclaim1 entry point in the kernel context.
 *              (PROBE value recorded when CHNL_Reclaim call enters the kernel
 *              context)
 *  @field  ProbeEventKReclaim1Leave
 *              Indicates the CHNL_Reclaim1 exit  point in the kernel context.
 *              (PROBE value recorded when CHNL_Reclaim call blocks)
 *  @field  ProbeEventKReclaim2Entry
 *              Indicates the CHNL_Reclaim2 entry point in the kernel context.
 *              (PROBE value recorded when CHNL_Reclaim call unblocks)
 *  @field  ProbeEventKReclaim2Leave
 *              Indicates the CHNL_Reclaim2 exit  point in the kernel context.
 *              (PROBE value recorded when CHNL_Reclaim returns from the kernel
 *              context)
 *  @field  ProbeEventKGet1Entry
 *              Indicates the MSGQ_Get1 entry point in the kernel context.
 *              (PROBE value recorded when MSGQ_Get call enters the kernel
 *              context)
 *  @field  ProbeEventKGet1Leave
 *              Indicates the MSGQ_Get1 exit  point in the kernel context.
 *              (PROBE value recorded when MSGQ_Get call blocks)
 *  @field  ProbeEventKGet2Entry
 *              Indicates the MSGQ_Get2 entry point in the kernel context.
 *              (PROBE value recorded when MSGQ_Get call unblocks)
 *  @field  ProbeEventKGet2Leave
 *              Indicates the MSGQ_Get2 exit  point in the kernel context.
 *              (PROBE value recorded when MSGQ_Get returns from the kernel
 *              context)
 *  @field  ProbeEventKDpcEntry
 *              Indicates the ISR entry point.
 *  @field  ProbeEventKDpcLeave
 *              Indicates the ISR exit point.
 *  @field  ProbeEventKIsrEntry
 *              Indicates the DPC entry point.
 *  @field  ProbeEventKIsrLeave
 *              Indicates the DPC exit point.
 *
 *  @see    None
 *  ============================================================================
 */
typedef enum {
    ProbeEventUIssueEntry         = 0,
    ProbeEventUIssueLeave         = 1,
    ProbeEventUPutEntry           = 2,
    ProbeEventUPutLeave           = 3,
    ProbeEventUReclaimEntry       = 4,
    ProbeEventUReclaimLeave       = 5,
    ProbeEventUGetEntry           = 6,
    ProbeEventUGetLeave           = 7,
    ProbeEventKIssueEntry         = 8,
    ProbeEventKIssueLeave         = 9,
    ProbeEventKPutEntry           = 10,
    ProbeEventKPutLeave           = 11,
    ProbeEventKReclaim1Entry      = 12,
    ProbeEventKReclaim1Leave      = 13,
    ProbeEventKReclaim2Entry      = 14,
    ProbeEventKReclaim2Leave      = 15,
    ProbeEventKGet1Entry          = 16,
    ProbeEventKGet1Leave          = 17,
    ProbeEventKGet2Entry          = 18,
    ProbeEventKGet2Leave          = 19,
    ProbeEventKDpcEntry           = 20,
    ProbeEventKDpcLeave           = 21,
    ProbeEventKIsrEntry           = 22,
    ProbeEventKIsrLeave           = 23
} PROBE_Event ;


/** ============================================================================
 *  @name   ProbeEntry
 *
 *  @desc   This structure defines the PROBE entry for each probe point.
 *
 *  @field  numOverflow
 *              The number of timers RTC has overflowed after being started.
 *  @field  rtcVal
 *              The value returned by the RTC.
 *
 *  @see    None
 *  ============================================================================
 */
typedef struct ProbeEntry_tag {
    Uint32 numOverflow ;
    Uint32 rtcVal      ;
} ProbeEntry ;


/** ============================================================================
 *  @name   ProbeData
 *
 *  @desc   This structure defines the PROBE data structure for the probe
 *          analysis.
 *
 *  @field  bufSize
 *              The size of the buffer being transferred by the application.
 *  @field  numBufs
 *              The number of buffers being transferred by the application.
 *  @field  selfPhysAddr
 *              The start address of the memory location allocated to store the
 *              PROBE data.
 *  @field  appStartTime
 *              The start PROBE value of the application.
 *  @field  appEndTime
 *              The end PROBE value of the application.
 *  @field  userProbeTimeStart
 *              The entry recording the start RTC values for the user-side probe.
 *              (The user probe time records the time taken to store the PROBE
 *              values from user side for a specified number of times.)
 *  @field  userProbeTimeEnd
 *              The entry recording the end RTC values for the user-side probe.
 *              (The user probe time records the time taken to store the PROBE
 *              values from user side for a specified number of times.)
 *  @field  numOverflow
 *              Number of times the RTC has overflowed since it was started.
 *  @field  ctxtSwitchTimeStart
 *              The entry recording the start RTC values for the context switch
 *              time probe.
 *              (The kernel probe time records the time taken to store the PROBE
 *              values from kernel side for a specified number of times.)
 *  @field  ctxtSwitchTimeEnd
 *              The entry recording the end RTC values for the context switch
 *              time probe.
 *              (The time taken to make a context switch from user side for a
 *              specified number of times.)
 *  @field  kernProbeTimeStart
 *              The entry recording the start RTC values for the kernel probe.
 *              (The kernel probe time records the time taken to store the PROBE
 *              values from kernel side for a specified number of times.)
 *  @field  kernProbeTimeEnd
 *              The entry recording the end RTC values for the kernel probe.
 *              (The kernel probe time records the time taken to store the PROBE
 *              values from kernel side for a specified number of times.)
 *  @field  index
 *              The offset to the next available location.
 *  @field  rtcValue
 *              A two-dimensional array of locations to store PROBE values.
 *              PROBE_MAX_ROWS values are stored for each probe point to be
 *              tracked.
 *
 *  @see    None
 *  ============================================================================
 */
typedef struct ProbeData_tag {
    Uint32          bufSize ;
    Uint32          numBufs ;

    Uint32          selfPhysAddr ;

    Uint32          appStartTime ;
    Uint32          appEndTime ;
    ProbeEntry      userProbeTimeStart ;
    ProbeEntry      userProbeTimeEnd ;
    Uint32          numOverflow ;
    ProbeEntry      ctxtSwitchTimeStart ;
    ProbeEntry      ctxtSwitchTimeEnd ;
    ProbeEntry      kernProbeTimeStart ;
    ProbeEntry      kernProbeTimeEnd ;

    Uint32          index ;
    ProbeEntry      rtcValue [NUM_PROBE_POINTS] [PROBE_MAX_ROWS] ;
} ProbeData ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !define (PROBEDEFS_H) */
