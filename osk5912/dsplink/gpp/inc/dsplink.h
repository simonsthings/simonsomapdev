/** ============================================================================
 *  @file   dsplink.h
 *
 *  @path   $(DSPLINK)\gpp\inc\Linux
 *
 *  @desc   Defines data types and structures used by DSP/BIOS(tm) Link.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DSPLINK_H)
#define DSPLINK_H


USES (gpptypes.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @const  MAX_PROCESSORS
 *
 *  @desc   Maximum number of DSPs supported by DSP/BIOS Link.
 *  ============================================================================
 */
#define MAX_PROCESSORS   1

/** ============================================================================
 *  @const  MAX_CHANNELS
 *
 *  @desc   Maximum number of channels per link supported by DSP/BIOS Link.
 *  ============================================================================
 */
#define MAX_CHANNELS    16

/** ============================================================================
 *  @const  MAX_ALLOC_BUFFERS
 *
 *  @desc   Maximum number of buffers that can be allocated
 *          through CHNL_AllocateBuffer.
 *  ============================================================================
 */
#define MAX_ALLOC_BUFFERS      1000

/** ============================================================================
 *  @const  WAIT_FOREVER
 *
 *  @desc   Wait indefinitely.
 *  ============================================================================
 */
#define WAIT_FOREVER           (~((Uint32) 0))

/** ============================================================================
 *  @const  WAIT_NONE
 *
 *  @desc   Do not wait.
 *  ============================================================================
 */
#define WAIT_NONE              ((Uint32) 0)

#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @const  NUM_MSGQ_CHANNELS
 *
 *  @desc   Number of channels being used for messaging.
 *  ============================================================================
 */
#define NUM_MSGQ_CHANNELS    2
#else /* if defined (MSGQ_COMPONENT) */
#define NUM_MSGQ_CHANNELS    0
#endif /* if defined (MSGQ_COMPONENT) */

#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @macro  IS_VALID_MSGCHNLID
 *
 *  @desc   Is the messaging chnl id valid.
 *  ============================================================================
 */
#define IS_VALID_MSGCHNLID(chnlId)                          \
                (    (chnlId >= MAX_CHANNELS)               \
                 &&  (chnlId <= (MAX_CHANNELS + NUM_MSGQ_CHANNELS - 1)))
#else /* if defined (MSGQ_COMPONENT) */
/** ============================================================================
 *  @macro  IS_VALID_MSGCHNLID
 *
 *  @desc   Is the messaging chnl id valid.
 *  ============================================================================
 */
#define IS_VALID_MSGCHNLID(chnlId)     FALSE
#endif /* if defined (MSGQ_COMPONENT) */

/** ============================================================================
 *  @macro  IS_VALID_PROCID
 *
 *  @desc   Is the proc id valid.
 *  ============================================================================
 */
#define IS_VALID_PROCID(id)        (id < MAX_PROCESSORS)

/** ============================================================================
 *  @macro  IS_VALID_CHNLID
 *
 *  @desc   Is the chnl id valid.
 *  ============================================================================
 */
#define IS_VALID_CHNLID(dspId, chnlId)                      \
                (    IS_VALID_PROCID (dspId)                \
                 &&  (chnlId < MAX_CHANNELS))

/** ============================================================================
 *  @const  DSP_MAX_STRLEN
 *
 *  @desc   Maximum length of string.
 *  ============================================================================
 */
#define DSP_MAX_STRLEN      32


/** ============================================================================
 *  @name   Endianism
 *
 *  @desc   Enumeration of data endianism.
 *
 *  @field  Endianism_Default
 *              Default endianism - no conversion required.
 *  @field  Endianism_Big
 *              Big endian.
 *  @field  Endianism_Little
 *              Little endian.
 *  ============================================================================
 */
typedef enum {
    Endianism_Default = 1,
    Endianism_Big     = 2,
    Endianism_Little  = 3
} Endianism ;

/** ============================================================================
 *  @name   ProcState
 *
 *  @desc   Enumerations to indicate processor states.
 *
 *  @field  ProcState_Unknown
 *              Unknown (possibly error) processor state.
 *  @field  ProcState_Loaded
 *              Indicates the DSP is loaded.
 *  @field  ProcState_Started
 *              Indicates the DSP is started.
 *  @field  ProcState_Stopped
 *              Indicates the DSP is stopped.
 *  @field  ProcState_Idle
 *              Indicates the DSP is idle.
 *  @field  ProcState_Reset
 *              Indicates the DSP is reset.
 *  ============================================================================
 */
typedef enum {
    ProcState_Unknown = 0,
    ProcState_Loaded  = 1,
    ProcState_Started = 2,
    ProcState_Stopped = 3,
    ProcState_Idle    = 4,
    ProcState_Reset   = 5
} ProcState ;

/** ============================================================================
 *  @name   DspArch
 *
 *  @desc   Enumerates the various architectures of DSP supported by
 *          DSP/BIOS LINK.
 *
 *  @field  DspArch_Unknown
 *              It indicates that the architecture is not supported.
 *  @field  DspArch_C55x
 *              It indicates that the architecture is C55x.
 *  @field  DspArch_C64x
 *              It indicates that the architecture is C64x.
 *  ============================================================================
 */
typedef enum {
    DspArch_Unknown  = 0,
    DspArch_C55x     = 1,
    DspArch_C64x     = 2
} DspArch ;

#if defined (CHNL_COMPONENT)
/** ============================================================================
 *  @name   ChannelMode
 *
 *  @desc   Mode of a channel.
 *
 *  @field  ChannelMode_Input
 *              Indicates the channel as an Input channel (from DSP to GPP).
 *  @field  ChannelMode_Output
 *              Indicates the channel as an Output channel (from GPP to DSP).
 *  ============================================================================
 */
typedef enum {
    ChannelMode_Input  = 0x1,
    ChannelMode_Output = 0x2
} ChannelMode ;

/** ============================================================================
 *  @name   ChannelDataSize
 *
 *  @desc   Width of data being sent on channel.
 *
 *  @field  ChannelDataSize_16bits
 *              Indicates the data to be transferred through the channel
 *              as 16 bit data.
 *  @field  ChannelDataSize_32bits
 *              Indicates the data to be transferred through the channel
 *              as 32 bit data.
 *  ============================================================================
 */
typedef enum {
    ChannelDataSize_16bits = 1,
    ChannelDataSize_32bits = 2
} ChannelDataSize ;

/** ============================================================================
 *  @name   ChannelAttrs
 *
 *  @desc   Channel Attributes.
 *
 *  @field  endianism
 *              Endiannism information currently not used.
 *  @field  mode
 *              Mode of channel (Input or output).
 *  @field  size
 *              Size of data sent on channel (16 bits or 32 bits).
 *  ============================================================================
 */
typedef struct ChannelAttrs_tag {
    Endianism       endianism ;
    ChannelMode     mode      ;
    ChannelDataSize size      ;
} ChannelAttrs ;

/** ============================================================================
 *  @name   ChannelIOInfo
 *
 *  @desc   Information for adding or reclaiming a IO request.
 *
 *  @field  buffer
 *              Buffer pointer.
 *  @field  size
 *              Size of buffer.
 *  @field  arg
 *              Argument to receive or send.
 *  ============================================================================
 */
typedef struct ChannelIOInfo_tag {
    Char8 *   buffer ;
    Uint32    size ;
    Uint32    arg ;
} ChannelIOInfo ;
#endif /* if defined (CHNL_COMPONENT) */

/** ============================================================================
 *  @name   LoaderObject
 *
 *  @desc   This object is used to pass arguments to Loader component.
 *
 *  @field  baseImage
 *              DSP executable file name.
 *  @field  dspArch
 *              Architecture of the dsp.
 *  @field  endian
 *              Endianism of the processor.
 *  @field  wordSize
 *              Word size on the dsp.
 *
 *  @see    None
 *  ============================================================================
 */
typedef struct LoaderObject_tag {
    Pstr            baseImage ;
    DspArch         dspArch   ;
    Endianism       endian    ;
    Uint32          wordSize  ;
} LoaderObject ;


/** ============================================================================
 *  @name   ProcAttr
 *
 *  @desc   A placeholder for processor information.
 *
 *  @field  timeout
 *              Time out associated with a DSP.
 *  ============================================================================
 */
typedef struct ProcAttr_tag {
    Uint32   timeout ;
} ProcAttr ;


#if defined (__cplusplus)
}
#endif


#endif /* !defined (DSPLINK_H) */
