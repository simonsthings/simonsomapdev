/** ============================================================================
 *  @file   dsplink.h
 *
 *  @path   $(DSPLINK)/gpp/inc/
 *
 *  @desc   Defines data types and structures used by DSP/BIOS(tm) Link.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DSPLINK_H)
#define DSPLINK_H


USES (gpptypes.h)

/*  ============================================================================
 *  Include the generated link configuration file.
 *  This is directly included here since it completes the definitions
 *  generically available to users.
 *  ============================================================================
 */
#include <linkcfg.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @name   MAX_SMABUFENTRIES
 *
 *  @desc   The maximum number of buffer pools that can be configured for SMA.
 *  ============================================================================
 */
#define MAX_SMABUFENTRIES   16

/** ============================================================================
 *  @name   MAX_POOLENTRIES
 *
 *  @desc   The maximum number of pools that can be configured.
 *  ============================================================================
 */
#define MAX_POOLENTRIES   4

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

/** ============================================================================
 *  @const  DSP_MAX_STRLEN
 *
 *  @desc   Maximum length of string.
 *  ============================================================================
 */
#define DSP_MAX_STRLEN      32

/** ============================================================================
 *  @const  POOL_INVALIDID
 *
 *  @desc   Reserved for denoting an invalid pool Id.
 *  ============================================================================
 */
#define POOL_INVALIDID         (Uint16) -1


/** ============================================================================
 *  @macro  IS_GPPID
 *
 *  @desc   Is the GPP ID valid.
 *  ============================================================================
 */
#define IS_GPPID(id)        (id == ID_GPP)

/** ============================================================================
 *  @macro  IS_VALID_PROCID
 *
 *  @desc   Is the proc ID valid.
 *  ============================================================================
 */
#define IS_VALID_PROCID(id)   (id < MAX_DSPS)


#if defined (CHNL_COMPONENT)
/** ============================================================================
 *  @macro  IS_VALID_CHNLID
 *
 *  @desc   Is the chnl ID valid.
 *  ============================================================================
 */
#define IS_VALID_CHNLID(dspId, chnlId)                                      \
                                (    IS_VALID_PROCID (dspId)                \
                                 &&  (chnlId < MAX_CHANNELS))
#else /* if defined (CHNL_COMPONENT) */
#define IS_VALID_CHNLID(dspId, chnlId)  FALSE
#endif /* if defined (CHNL_COMPONENT) */


/** ============================================================================
 *  @macro  IS_VALID_POOLID
 *
 *  @desc   Is the pool ID valid.
 *  ============================================================================
 */
#define IS_VALID_POOLID(id)   (id < MAX_POOLENTRIES)


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


#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
#if defined (PCPY_LINK)
/** ============================================================================
 *  @name   BufPoolAttrs
 *
 *  @desc   This structure defines the attributes required for initialization of
 *          the buffer pool.
 *
 *  @field  numBufPools
 *              Number of entries in the arrays of buffer sizes and number of
 *              buffers in the params structure.
 *  @field  bufSizes
 *              Array of sizes of the buffers in the buffer pools.
 *              This array is of size numBufObjs.
 *  @field  numBuffers
 *              Array of number of buffers in all buffer pools.
 *              This array is of size numBufObjs.
 *  ============================================================================
 */
typedef struct BufPoolAttrs_tag {
    Uint32     numBufPools ;
    Uint32 *   bufSizes ;
    Uint32 *   numBuffers ;
} BufPoolAttrs ;
#endif /* if defined (PCPY_LINK) */

#if defined (ZCPY_LINK)
/** ============================================================================
 *  @name   SmaPoolAttrs
 *
 *  @desc   This structure defines the attributes of the SMA POOL.
 *
 *  @field  numBufPools
 *              Number of buffer pools.
 *  @field  bufSizes
 *              Array of sizes of the buffers in each buffer pools.
 *  @field  numBuffers
 *              Array of number of buffers in each buffer pools.
 *  @field  exactMatchReq
 *              Flag indicating whether requested size is to be rounded to
 *              nearest available size in Pools or exact match has to be
 *              performed.
 *  ============================================================================
 */
typedef struct SmaPoolAttrs_tag {
    Uint32        numBufPools ;
    Uint32  *     bufSizes    ;
    Uint32  *     numBuffers  ;
    Bool          exactMatchReq ;
} SmaPoolAttrs ;
#endif /* if defined (ZCPY_LINK) */
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */


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
