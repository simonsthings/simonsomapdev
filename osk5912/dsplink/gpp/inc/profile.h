/** ============================================================================
 *  @file   profile.h
 *
 *  @path   $(DSPLINK)\gpp\inc
 *
 *  @desc   Defines instrumentation structures for profiling DSP/BIOS LINK.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PROFILE_H)
#define PROFILE_H


USES (gpptypes.h)
USES (dsplink.h)
USES (msgqdefs.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


#if defined (DDSP_PROFILE)


/** ============================================================================
 *  @const  DATA_SIZE
 *
 *  @desc   Number of bytes stored per buffer for recording the history of
 *          data transfer on a channel.
 *  ============================================================================
 */
#define DATA_LENGTH   8

/** ============================================================================
 *  @name   HIST_LENGTH
 *
 *  @desc   Number of buffers to be recorded as history.
 *  ============================================================================
 */
#define HIST_LENGTH   8

/** ============================================================================
 *  @name   HistoryData
 *
 *  @desc   Array to hold history data for a channel.
 *  ============================================================================
 */
typedef Char8    HistoryData [DATA_LENGTH] ;

/** ============================================================================
 *  @name   NumberOfBytes
 *
 *  @desc   Number of bytes transferred.
 *  ============================================================================
 */
typedef Uint32   NumberOfBytes ;

/** ============================================================================
 *  @name   NumberOfInterrupts
 *
 *  @desc   Number of interrupts sent to/received from DSP.
 *  ============================================================================
 */
typedef Uint32   NumberOfInterrupts ;


/** ============================================================================
 *  @name   ProcInstrument
 *
 *  @desc   Instrumentation data for a DSP processor.
 *
 *  @field  procId
 *              Processor identifier.
 *  @field  active
 *              Indicates if the DSP is in use.
 *  @field  dataToDsp
 *              Number of bytes sent to DSP.
 *  @field  dataFromDsp
 *              Number of bytes received from DSP.
 *  @field  intsToDsp
 *              Number of interrupts sent to DSP.
 *  @field  intsFromDsp
 *              Number of interrupts received from DSP.
 *  @field  activeLinks
 *              Number of links currently active.
 *  ============================================================================
 */
typedef struct ProcInstrument_tag {
    ProcessorId        procId      ;
    Bool               active      ;
    NumberOfBytes      dataToDsp   ;
    NumberOfBytes      dataFromDsp ;
    NumberOfInterrupts intsToDsp   ;
    NumberOfInterrupts intsFromDsp ;
    Uint32             activeLinks ;
} ProcInstrument ;


/** ============================================================================
 *  @name   ProcStats
 *
 *  @desc   Instrumentation data for the processors.
 *
 *  @field  procData
 *              Instrumentation data per dsp processor.
 *  ============================================================================
 */
typedef struct ProcStats_tag {
    ProcInstrument  procData [MAX_PROCESSORS] ;
} ProcStats ;


#if defined (CHNL_COMPONENT)
/** ============================================================================
 *  @name   ChnlShared
 *
 *  @desc   Place holder for instrumentation data common to multiple channels.
 *
 *  @field  nameLinkDriver
 *              Name of the link driver.
 *  ============================================================================
 */
typedef struct ChnlShared_tag {
    Pstr    nameLinkDriver ;
} ChnlShared ;


/** ============================================================================
 *  @name   ChnlInstrument
 *
 *  @desc   Instrumentation data for a channel.
 *
 *  @field  procId
 *              Processor identifier.
 *  @field  chnlId
 *              Channel identifier.
 *  @field  active
 *              Indicates if the channel is in use.
 *  @field  mode
 *              Indicates if the channel is input or output.
 *  @field  chnlShared
 *              Pointer to the shared information across multiple channels.
 *  @field  transferred
 *              Number of bytes transferred on channel.
 *  @field  numBufsQueued
 *              Number of currently queued buffers.
 *  @field  archive
 *              History of data sent on channel.
 *  ============================================================================
 */
typedef struct ChnlInstrument_tag {
    ProcessorId      procId        ;
    ChannelId        chnlId        ;
    Bool             active        ;
    ChannelMode      mode          ;
    ChnlShared *     chnlShared    ;
    NumberOfBytes    transferred   ;
    Uint32           numBufsQueued ;
#if defined (DDSP_PROFILE_DETAILED)
    Uint32           archIndex     ;
    HistoryData      archive [HIST_LENGTH] ;
#endif /* defined (DDSP_PROFILE_DETAILED) */
} ChnlInstrument ;


/** ============================================================================
 *  @name   ChnlStats
 *
 *  @desc   Instrumentation data for channels.
 *
 *  @field  chnlId
 *              Instrumentation data per channel.
 *  ============================================================================
 */
typedef struct ChnlStats_tag {
    ChnlInstrument  chnlData [MAX_PROCESSORS][MAX_CHANNELS] ;
} ChnlStats ;
#endif  /* if defined (CHNL_COMPONENT) */


/** ============================================================================
 *  @name   DspStats_tag
 *
 *  @desc   Instrumentation data per DSP.
 *
 *  @field  dataGppToDsp
 *              Number of bytes transferred to DSP.
 *  @field  dataDspToGpp
 *              Number of bytes transferred from DSP.
 *  @field  intsGppToDsp
 *              Number of interrupts to DSP.
 *  @field  intsDspToGpp
 *              Number of interrupts from DSP.
 *  ============================================================================
 */
struct DspStats_tag {
    NumberOfBytes       dataGppToDsp ;
    NumberOfBytes       dataDspToGpp ;
    NumberOfInterrupts  intsGppToDsp ;
    NumberOfInterrupts  intsDspToGpp ;
} ;


#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   MsgqInstrument
 *
 *  @desc   This structure defines the instrumentation data for a message queue.
 *
 *  @field  procId
 *              Processor identifier.
 *  @field  msgqId
 *              Message queue identifier.
 *  @field  isValid
 *              Indicates if the message queue is valid.
 *  @field  transferred
 *              Number of messages transferred on this MSGQ.
 *  @field  queued
 *              Number of messages currently queued on this MSGQ, pending calls
 *              to get them.
 *  ============================================================================
 */
typedef struct MsgqInstrument_tag {
    ProcessorId      procId ;
    MsgQueueId       msgqId ;
    Bool             isValid ;
    Uint32           transferred ;
    Uint32           queued ;
} MsgqInstrument ;

/** ============================================================================
 *  @name   MsgqStats
 *
 *  @desc   This structure defines the instrumentation data for MSGQs on all
 *          processors in the system.
 *
 *  @field  localMsgqData
 *              Instrumentation data for the local MSGQs.
 *  @field  msgqData
 *              Instrumentation data for the remote MSGQs.
 *  ============================================================================
 */
typedef struct MsgqStats_tag {
    MsgqInstrument   localMsgqData [MAX_MSGQS] ;
    MsgqInstrument   msgqData [MAX_PROCESSORS] [MAX_MSGQS] ;
} MsgqStats ;

#endif /* defined (MSGQ_COMPONENT) */
#endif  /* if defined (DDSP_PROFILE) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* defined (PROFILE_H) */
