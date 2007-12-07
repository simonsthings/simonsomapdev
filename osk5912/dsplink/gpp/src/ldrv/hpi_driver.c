/** ============================================================================
 *  @file   hpi_driver.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implementation of the HPI link driver between 64x DSP on DM642
 *          and ARM on DM310.
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
#include <intobject.h>
#include <bitops.h>
#include <safe.h>

#include <cfgdefs.h>
#include <dspdefs.h>
#include <linkdefs.h>

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
#include <dpc.h>
#include <mem_os.h>
#include <mem.h>
#include <cfg.h>
#include <isr.h>
#include <sync.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <list.h>

/*  ----------------------------------- Link Driver                 */
#if defined (MSGQ_COMPONENT)
#include <ldrv_mqa.h>
#include <ldrv_mqt.h>
#include <ldrv_msgq.h>
#endif /* if defined (MSGQ_COMPONENT) */

#include <ldrv.h>
#include <ldrv_chnl.h>
#include <ldrv_io.h>
#include <dsp.h>

#include <hpi_driver.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_HPI_DRV

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  GEN_SetReason (status, FID_C_LDRV_HPI_DRV, __LINE__)

/** ============================================================================
 *  @const  CHNLID_INVALID
 *
 *  @desc   Indicates invalid channel id.
 *  ============================================================================
 */
#define CHNLID_INVALID  0xFFFF

/** ============================================================================
 *  @const  DM642_MAUSIZE
 *
 *  @desc   Size of the minimum addressable unit on DM642 (in bytes).
 *  ============================================================================
 */
#define DM642_MAUSIZE 1

/*  ============================================================================
 *  @const  DM642_INTERRUPT
 *
 *  @desc   Interrupt number corresponding to DSPINT interrupt on HPI
 *          control register.
 *          (Also defined in dsp.c)
 *  ============================================================================
 */
#define DM642_INTERRUPT   92

#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @const  MAX_POLL_COUNT
 *
 *  @desc   For debug mode, indicates the maximum count to wait for handshake
 *          to complete before timing out.
 *  ============================================================================
 */
#define MAX_POLL_COUNT  100000000
#endif /* defined (DDSP_DEBUG) */

/** ============================================================================
 *  @const  GPP_HANDSHAKE
 *
 *  @desc   Value written by GPP for synchronizing the link driver.
 *  ============================================================================
 */
#define GPP_HANDSHAKE               0xC0C0

/** ============================================================================
 *  @const  DSP_HANDSHAKE
 *
 *  @desc   Value written by DSP for synchronizing the link driver.
 *  ============================================================================
 */
#define DSP_HANDSHAKE               0xBABA

/** ============================================================================
 *  @const  GPP_DMASK_OFFSET
 *
 *  @desc   Offset of the GPP data mask bits in the HPI link-driver
 *          control structure (GPP control structure).
 *  ============================================================================
 */
#define GPP_DATAMASK_OFFSET         4

/** ============================================================================
 *  @const  DSP_DATAMASK_OFFSET
 *
 *  @desc   Offset of the DSP data mask bits in the HPI link-driver
 *          control structure (DSP control structure).
 *  ============================================================================
 */
#define DSP_DATAMASK_OFFSET         4

/** ============================================================================
 *  @const  GPP_MSGMASK_OFFSET
 *
 *  @desc   Offset of the GPP MSG mask bits in the HPI link-driver
 *          control structure (GPP control structure).
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define GPP_MSGQMASK_OFFSET          8
#else  /* if defined (MSGQ_COMPONENT) */
#define GPP_MSGQMASK_OFFSET          0
#endif /* if defined (MSGQ_COMPONENT) */

/** ============================================================================
 *  @const  DSP_MSGQMASK_OFFSET
 *
 *  @desc   Offset of the DSP MSG mask bits in the HPI link-driver
 *          control structure (DSP control structure).
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define DSP_MSGQMASK_OFFSET          8
#else  /* if defined (MSGQ_COMPONENT) */
#define DSP_MSGQMASK_OFFSET          0
#endif /* if defined (MSGQ_COMPONENT) */

/** ============================================================================
 *  @const  CTRL_PADLENGTH
 *
 *  @desc   Padding required for 128-byte alignment (for L2 cache) for control
 *          structure.
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define CTRL_PADLENGTH              0x1D
#else  /* if defined (MSGQ_COMPONENT) */
#define CTRL_PADLENGTH              0x1E
#endif /* if defined (MSGQ_COMPONENT) */

/** ============================================================================
 *  @const  CHNLINFO_PADLENGTH
 *
 *  @desc   Padding required for 128-byte alignment (for L2 cache) for channel
 *          information structure.
 *  ============================================================================
 */
#define CHNLINFO_PADLENGTH          0x1A


/** ============================================================================
 *  @const  NUM_DATA_CHNLS
 *
 *  @desc   Maximum number of channels supported by the HPI link driver.
 *  ============================================================================
 */
#define NUM_DATA_CHNLS        MAX_CHANNELS

/** ============================================================================
 *  @const  NUM_MSGQ_CHNLS
 *
 *  @desc   Number of channels used for messaging.
 *          Note: The current implementation assumes only 2 channels.
 *  ============================================================================
 */
#if defined (MSGQ_COMPONENT)
#define NUM_MSGQ_CHNLS               2
#else  /* if defined (MSGQ_COMPONENT) */
#define NUM_MSGQ_CHNLS               0
#endif /* if defined (MSGQ_COMPONENT) */

/** ============================================================================
 *  @macro  IS_VALID_HPI_CHNLID
 *
 *  @desc   Check if the specific channel id is a valid channel id for the link
 *          driver.
 *  ============================================================================
 */
#define IS_VALID_HPI_CHNLID(id)     ((id) < (NUM_DATA_CHNLS + NUM_MSGQ_CHNLS))

/** ============================================================================
 *  @macro  IS_VALID_HPI_MSGCHNLID
 *
 *  @desc   Check if the specific channel id is a valid messaging channel id
 *          for the link driver.
 *  ============================================================================
 */
#define IS_VALID_HPI_MSGCHNLID(id)  (   ((id) == NUM_DATA_CHNLS)        \
                                     || ((id) == NUM_DATA_CHNLS + 1))


/** ============================================================================
 *  @name   HpiCmd
 *
 *  @desc   The commands used by the HPI link driver for
 *          carrying out data transfer on a specific channel.
 *
 *  @field  HpiCmd_Invalid
 *              Indicates no command is pending on the channel.
 *  @field  HpiCmd_Read
 *              Indicates that processing for a Read is pending.
 *  @field  HpiCmd_Write
 *              Indicates that processing for a Write is pending.
 *  @field  HpiCmd_ReadAck
 *              Indicates that processing for a ReadAck is pending.
 *  @field  HpiCmd_WriteAck
 *              Indicates that processing for a WriteAck is pending.
 *  @field  HpiCmd_ReadMore
 *              Indicates that processing for a ReadMore is pending.
 *  @field  HpiCmd_WriteMore
 *              Indicates that processing for a WriteMore is pending.
 *  @field  HpiCmd_ReadComplete
 *              Indicates that processing for a ReadComplete is pending.
 *  @field  HpiCmd_WriteComplete
 *              Indicates that processing for a WriteComplete is pending.
 *  ============================================================================
 */
typedef enum {
    HpiCmd_Invalid       = 0,
    HpiCmd_Read          = 1,
    HpiCmd_Write         = 2,
    HpiCmd_ReadAck       = 3,
    HpiCmd_WriteAck      = 4,
    HpiCmd_ReadMore      = 5,
    HpiCmd_WriteMore     = 6,
    HpiCmd_ReadComplete  = 7,
    HpiCmd_WriteComplete = 8
} HpiCmd ;

/*  ============================================================================
 *  @name   ChannelCtrlCmd
 *
 *  @desc   Command indicating action required on the bit corresponding
 *          to a channel in a bitmask.
 *
 *  @field  ChannelCtrlCmd_Reset
 *              Indicates resetting the bit in respective bit-mask for
 *              corresponding channel Id.
 *  @field  ChannelCtrlCmd_Set
 *              Indicates setting the bit in respective bit-mask for
 *              corresponding channel Id.
 *  ============================================================================
 */
typedef enum {
    ChannelCtrlCmd_Reset = 0,
    ChannelCtrlCmd_Set   = 1
} ChannelCtrlCmd ;


/** ============================================================================
 *  @name   HpiGppControl
 *
 *  @desc   Defines the control structure used by GPP side
 *          HPI Link Driver.
 *
 *  @field  gppHandshake
 *              Handshake field to be updated by GPP.
 *  @field  dspDataCmdMask
 *              Mask indicating data channels on which DSP needs to take action.
 *              (written by GPP/read by DSP)
 *  @field  dspMsgqCmdMask
 *              Mask indicating messaging channels on which DSP needs to take
 *              action.
 *              (written by GPP/read by DSP)
 *  @field  padding
 *              Alignment required for 128 byte cache-line.
 *  ============================================================================
 */
typedef struct HpiGppControl_tag {
    volatile Uint32  gppHandshake   ;
    volatile Uint32  dspDataCmdMask ;
#if defined (MSGQ_COMPONENT)
    volatile Uint32  dspMsgqCmdMask ;
#endif /* if defined (MSGQ_COMPONENT) */
    volatile Uint32  padding [CTRL_PADLENGTH] ;
} HpiGppControl ;

/** ============================================================================
 *  @name   HpiDspControl
 *
 *  @desc   Defines the control structure used by DSP side
 *          HPI Link Driver.
 *
 *  @field  dspHandshake
 *              Handshake field to be updated by DSP.
 *  @field  gppDataCmdMask
 *              Mask indicating data channels on which GPP needs to take action.
 *              (written by DSP/read by GPP)
 *  @field  gppMsgqCmdMask
 *              Mask indicating messaging channels on which GPP needs to take
 *              action.
 *              (written by DSP/read by GPP)
 *  @field  padding
 *              Alignment required for 128 byte cache-line.
 *  ============================================================================
 */
typedef struct HpiDspControl_tag {
    volatile Uint32  dspHandshake   ;
    volatile Uint32  gppDataCmdMask ;
#if defined (MSGQ_COMPONENT)
    volatile Uint32  gppMsgqCmdMask  ;
#endif /* if defined (MSGQ_COMPONENT) */
    volatile Uint32  padding [CTRL_PADLENGTH] ;
} HpiDspControl ;

/** ============================================================================
 *  @name   HpiControl
 *
 *  @desc   Defines the control structure used by data transfer protocol
 *          built over the HPI Link Driver.
 *
 *  @field  gppControl
 *              GPP side link driver control information.
 *  @field  dspControl
 *              DSP side link driver control information.
 *  ============================================================================
 */
typedef struct HpiControl_tag {
    HpiGppControl   gppControl ;
    HpiDspControl   dspControl ;
} HpiControl ;

/** ============================================================================
 *  @name   HpiChnlInfo
 *
 *  @desc   This structure contains details regarding the current state of a
 *          channel.
 *
 *  @field  command
 *              Indicates the command pending on the channel.
 *  @field  isValid
 *              Boolean flag to indicate that address and size values are valid.
 *  @field  sharedBufPtr
 *              Address of the memory area exposed by DSP used for transferring
 *              data on the channel.
 *  @field  sharedBufSize
 *              Size of the memory area exposed by DSP used for transferring
 *              data on the channel.
 *  @field  clientBufPtr
 *              Address of the client buffer. This field stores the address of
 *              the client buffer on the DSP side for which the operation is
 *              pending.
 *  @field  clientBufSize
 *              Size of the client buffer on the user size on which the
 *              operation is pending.
 *  @field  padding
 *              Alignment required for 128 byte cache-line.
 *  ============================================================================
 */
typedef struct HpiChnlInfo_tag {
    volatile Uint32  command       ;
    volatile Uint32  isValid       ;
    volatile Uint32  sharedBufPtr  ;
    volatile Uint32  sharedBufSize ;
    volatile Uint32  clientBufPtr  ;
    volatile Uint32  clientBufSize ;
    volatile Uint32  padding [CHNLINFO_PADLENGTH] ;
} HpiChnlInfo ;

/** ============================================================================
 *  @name   HpiDriverObj
 *
 *  @desc   Defines the driver information object for the HPI
 *          Link Driver.
 *
 *  @field  dspId
 *              Processor Identifier.
 *  @field  linkId
 *              Link Identifier.
 *  @field  baseChnlId
 *              Base channel identifier for HPI link driver.
 *  @field  numChannels
 *              Number of channels supported by this link driver.
 *  @field  intObj
 *              Interrupt object used by the HPI link driver.
 *  @field  isrObj
 *              ISR object for the the HPI link driver.
 *  @field  dpcObj
 *              DPC object for the the HPI link driver.
 *  @field  addrGppControl
 *              Address of the GPP control information in the DSP memory space.
 *  @field  addrDspControl
 *              Address of the DSP control information in the DSP memory space.
 *  @field  addrChannels
 *              Starting address of the Channel information structures in the
 *              DSP memory space.
 *  @field  ctrlInfo
 *              Structure for storing an image of the control information as
 *              maintained on the DSP side.
 *  @field  chnlInfo
 *              Structure of the channel specific control information. This is
 *              maintained locally as a image of the control information kept
 *              on the DSP side.
 *  @field  chirp
 *              Current IO Request packet..
 *  @field  sizeTransferred
 *              Size of data copied across the physical link as part of a buffer
 *              transfer.
 *  ============================================================================
 */
typedef struct HpiDriverObj_tag {
    Uint32          dspId       ;
    Uint32          linkId      ;
    Uint16          baseChnlId  ;
    Uint16          numChannels ;

    InterruptObject intObj ;
    IsrObject *     isrObj ;
    DpcObject *     dpcObj ;

    Uint32          addrGppControl ;
    Uint32          addrDspControl ;
    Uint32          addrChannels   ;

    HpiControl      ctrlInfo ;
    HpiChnlInfo     chnlInfo [NUM_DATA_CHNLS + NUM_MSGQ_CHNLS] ;

    Uint32          sizeTransferred ;
} HpiDriverObj  ;


/** ============================================================================
 *  @name   LDRV_Obj
 *
 *  @desc   Extern declaration of object containing ldrv level information
 *          defined in ldrv.c.
 *  ============================================================================
 */
EXTERN LDRV_Object LDRV_Obj ;


/** ----------------------------------------------------------------------------
 *  @name   HPI_DriverObject
 *
 *  @desc   Array of HPI driver info structure.
 *          It assumes only one HPI link exists between the GPP and the DSP.
 *  ----------------------------------------------------------------------------
 */
STATIC HpiDriverObj   HPI_DriverObject [MAX_PROCESSORS] ;


/** ============================================================================
 *  @name   HPI_Interface
 *
 *  @desc   HPI Driver interface.
 *  ============================================================================
 */
LinkInterface HPI_Interface = {
    &HPI_Initialize,
    &HPI_Finalize,
    &HPI_OpenChannel,
    &HPI_CloseChannel,
    &HPI_CancelIO,
    &HPI_IO_Request,
    &HPI_ScheduleDPC,
    &HPI_HandshakeSetup,
    &HPI_HandshakeStart,
    &HPI_HandshakeComplete
} ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_InterruptDsp
 *
 *  @desc   This function sends the interrupt to DSP.
 *
 *  @arg    dspId
 *              Processor ID.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    HPI_ClearDspInterrupt, DSP_Interrupt
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_InterruptDsp (IN ProcessorId dspId) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_DoDataTransfer
 *
 *  @desc   This function drives input and output operations on channels after
 *          checking pending commands on the channels.
 *
 *  @arg    dspId
 *              Processor ID.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    HPI_DoChnlProcessing
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_DoDataTransfer (IN ProcessorId dspId) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_ClearDspInterrupt
 *
 *  @desc   This function clears the DSP Interrupt.
 *
 *  @arg    dspId
 *              Processor ID.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EINVALIDARG
 *              dspId is invalid
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    HPI_InterruptDsp, DSP_ClearInterrupt
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_ClearDspInterrupt (IN ProcessorId dspId) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_DoChnlProcessing
 *
 *  @desc   This function is called by the DPC for processing on a channel.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    hpiChnlId
 *              Channel Id.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    HPI_Initialize, HPI_OpenChannel
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_DoChnlProcessing (IN     ProcessorId   dspId,
                      IN     ChannelId     hpiChnlId) ;



/*  ----------------------------------------------------------------------------
 *  @func   HPI_UpdateChnlInfo
 *
 *  @desc   This function updates the channel information structure with
 *          after completion of the data transfer.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    hpiChnlId
 *              Channel Id.
 *  @arg    chnlInfo
 *              Pointer to the channel information structure.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    HPI_Initialize, HPI_OpenChannel
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_UpdateChnlInfo (IN ProcessorId    dspId,
                    IN ChannelId      hpiChnlId,
                    IN HpiChnlInfo *  chnlInfo) ;


/*  ----------------------------------------------------------------------------
 *  @func   HPI_InvokeCallback
 *
 *  @desc   This function invokes the callback functions required.
 *          - AddIOCompletion to indicate that the data transfer is complete.
 *          - chirp->callback () if it is non-NULL.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    hpiChnlId
 *              Channel Id.
 *  @arg    chirp
 *              Pointer to channel IO request packet.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General error.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    HPI_Initialize, HPI_OpenChannel
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_InvokeCallback (IN ProcessorId   dspId,
                    IN ChannelId     hpiChnlId,
                    IN LDRVChnlIRP * chirp) ;


/*  ----------------------------------------------------------------------------
 *  @func   HPI_GetChannelId
 *
 *  @desc   This function finds the next channel on which a command is pending.
 *
 *  @arg    chnlMask
 *              Mask used for searching the pending channel.
 *  @arg    maxChannels
 *              Maximum number of channels supported by the link driver.
 *
 *  @ret    CHNLID_INVALID
 *              Invalid channel id.
 *          id >= 0, id < maxChannels
 *              Valid channel id.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    HPI_DoDataTransfer
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
ChannelId
HPI_GetChannelId (IN  Uint16 chnlMask, IN  Uint16 maxChannels) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_ChannelCtrl
 *
 *  @desc   This function sets / resets a channel by updating the data
 *          structures accordingly.
 *          Specifically it performs the following actions:
 *          - Sets / Resets the bit corresponding to the channel id in the
 *            dspDataMask.
 *
 *  @arg    dspId
 *              Processor Identifier
 *  @arg    hpiChnlId
 *              Channel Identifier
 *  @arg    cmd
 *              Command to be performed on the channel.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Generic failure.
 *
 *  @enter  dspId must be valid.
 *          hpiChnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_OpenChannel, HPI_CloseChannel, HPI_CancelIO
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_ChannelCtrl (IN ProcessorId     dspId,
                 IN ChannelId       hpiChnlId,
                 IN ChannelCtrlCmd  cmd) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_ReadChannelInfo
 *
 *  @desc   This function reads the HPI link control information pertaining to
 *          the channel.
 *
 *  @arg    dspId
 *              Processor Identifier
 *  @arg    hpiChnlId
 *              Channel Identifier
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Generic failure.
 *
 *  @enter  dspId must be valid.
 *          hpiChnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_DoChnlProcessing, HPI_IO_Request
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_ReadChannelInfo (IN ProcessorId dspId, IN ChannelId hpiChnlId) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_WriteChannelInfo
 *
 *  @desc   This function writes the HPI link control information pertaining to
 *          the channel.
 *
 *  @arg    dspId
 *              Processor Identifier
 *  @arg    hpiChnlId
 *              Channel Identifier
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Generic failure.
 *
 *  @enter  dspId must be valid.
 *          hpiChnlId must be valid.
 *
 *  @leave  None
 *
 *  @see    HPI_DoChnlProcessing, HPI_IO_Request
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_WriteChannelInfo (IN ProcessorId    dspId,
                      IN ChannelId      hpiChnlId) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_DspRead
 *
 *  @desc   Reads data from DSP through HPI interface.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              DSP address to read from.
 *  @arg    endianInfo
 *              endianness of data - indicates whether swap is required or not.
 *  @arg    numBytes
 *              Number of bytes to read.
 *  @arg    buffer
 *              Buffer to hold the read data.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
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
DSP_STATUS
HPI_DspRead (IN     ProcessorId  dspId,
             IN     DspObject *  dspObj,
             IN     Uint32       dspAddr,
             IN     Endianism    endianInfo,
             IN OUT Uint32 *     numBytes,
             OUT    Uint8 *      buffer) ;


/** ----------------------------------------------------------------------------
 *  @func   HPI_DspWrite
 *
 *  @desc   Writes data to DSP through HPI interface.
 *
 *  @arg    dspId
 *              Processor ID.
 *  @arg    dspObj
 *              Pointer to object containing context information for DSP.
 *  @arg    dspAddr
 *              DSP address to write to.
 *  @arg    endianInfo
 *              endianness of data - indicates whether swap is required or not.
 *  @arg    numBytes
 *              Number of bytes to write.
 *  @arg    buffer
 *              Buffer containing data to be written.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General Failure.
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
DSP_STATUS
HPI_DspWrite (IN     ProcessorId  dspId,
              IN     DspObject *  dspObj,
              IN     Uint32       dspAddr,
              IN     Endianism    endianInfo,
              IN OUT Uint32       numBytes,
              OUT    Uint8 *      buffer) ;


/** ============================================================================
 *  @func   HPI_Initialize
 *
 *  @desc   This function allocates and initializes resources used by the
 *          HPI link Driver.
 *
 *  @modif  HPI_DriverObject
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_Initialize (IN ProcessorId dspId, IN LinkId lnkId)
{
    DSP_STATUS     status      = DSP_SOK ;
    HpiDriverObj * hpiInfo     = NULL    ;
    DspInterface * dspIntf     = NULL    ;
    DspObject *    dspObj      = NULL    ;
    LinkAttrs *    linkAttr    = NULL    ;
    Uint32         i                     ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    TRC_2ENTER("HPI_Initialize",dspId,lnkId) ;

    dspObj   = &(LDRV_Obj.dspObjects [dspId]) ;
    linkAttr = LDRV_Obj.linkTables [lnkId]    ;
    hpiInfo  = &(HPI_DriverObject [dspId])    ;
    dspIntf = dspObj->interface               ;

    /*  ------------------------------------------------------------------------
     *  Initialize the HPI Driver information fields
     *  ------------------------------------------------------------------------
     */
    hpiInfo->dspId       = dspId ;
    hpiInfo->linkId      = lnkId ;
    hpiInfo->baseChnlId  = linkAttr->baseChnlId  ;
    hpiInfo->numChannels = linkAttr->numChannels ;

    hpiInfo->intObj.intId = DM642_INTERRUPT ;
    hpiInfo->dpcObj       = NULL ;
    hpiInfo->isrObj       = NULL ;

    hpiInfo->addrGppControl = linkAttr->argument1 ;
    hpiInfo->addrDspControl = hpiInfo->addrGppControl + sizeof (HpiGppControl) ;
    hpiInfo->addrChannels   = hpiInfo->addrDspControl + sizeof (HpiDspControl) ;

    hpiInfo->sizeTransferred = 0    ;

    /*  ------------------------------------------------------------------------
     *  Initialize the link-driver control structure in DSP memory space
     *  ------------------------------------------------------------------------
     */
    hpiInfo->ctrlInfo.gppControl.gppHandshake   = 0x0 ;
    hpiInfo->ctrlInfo.gppControl.dspDataCmdMask = 0x0 ;

#if defined (MSGQ_COMPONENT)
    hpiInfo->ctrlInfo.gppControl.dspMsgqCmdMask = 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

    status = (*(dspIntf->write)) (dspId,
                                  dspObj,
                                  hpiInfo->addrGppControl,
                                  Endianism_Default,
                                  sizeof (HpiGppControl),
                                  (Uint8 *) &(hpiInfo->ctrlInfo.gppControl)) ;

    if (DSP_SUCCEEDED (status)) {
        hpiInfo->ctrlInfo.dspControl.dspHandshake   = 0x0 ;
        hpiInfo->ctrlInfo.dspControl.gppDataCmdMask = 0x0 ;

#if defined (MSGQ_COMPONENT)
        hpiInfo->ctrlInfo.dspControl.gppMsgqCmdMask = 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

        status = (*(dspIntf->write)) (
                               dspId,
                               dspObj,
                               hpiInfo->addrDspControl,
                               Endianism_Default,
                               sizeof (HpiDspControl),
                               (Uint8 *) &(hpiInfo->ctrlInfo.dspControl)) ;
        if (DSP_FAILED (status)) {
            TRC_0PRINT (TRC_LEVEL7,
                        "Error in HPI control structure initialization\n") ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    /*  --------------------------------------------------------------------
     *  Initialize channel control structures in DSP memory space.
     *  --------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        for (i = 0 ; i < (hpiInfo->numChannels + NUM_MSGQ_CHNLS) ; i++) {
            hpiInfo->chnlInfo [i].command       = HpiCmd_Invalid ;
            hpiInfo->chnlInfo [i].isValid       = FALSE          ;
            hpiInfo->chnlInfo [i].clientBufPtr  = 0              ;
            hpiInfo->chnlInfo [i].clientBufSize = 0              ;
            hpiInfo->chnlInfo [i].sharedBufPtr  = 0              ;
            hpiInfo->chnlInfo [i].sharedBufSize = 0              ;
        }

        status = (*(dspIntf->write)) (dspId, dspObj,
                                      hpiInfo->addrChannels,
                                      Endianism_Default,
                                      (  sizeof (HpiChnlInfo)
                                       * (  hpiInfo->numChannels
                                          + NUM_MSGQ_CHNLS)),
                                      (Uint8 *) &(hpiInfo->chnlInfo [0])) ;
        if (DSP_FAILED (status)) {
            TRC_0PRINT (TRC_LEVEL7,
                        "Error in HPI channel info initialization\n") ;
            SET_FAILURE_REASON ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Get link specific DPC function & create the DPC Object.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = DPC_Create (HPI_DPC,
                             hpiInfo,
                             &(hpiInfo->dpcObj)) ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Get link specific interrupt information & create the ISR Object.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = ISR_Create (HPI_ISR,
                             hpiInfo,
                             &(hpiInfo->intObj),
                             &(hpiInfo->isrObj)) ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    /*  ------------------------------------------------------------------------
     *  Install the ISR (it is enabled automatically).
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = ISR_Install (NULL, hpiInfo->isrObj) ;
        if (DSP_SUCCEEDED (status)) {
            status  = (*(dspObj->interface->enableInterrupt)) (
                                                    dspId,
                                                    dspObj,
                                                    &(hpiInfo->intObj)) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("HPI_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_Finalize
 *
 *  @desc   This function releases the resources allocated earlier by this
 *          link driver in call to HPI_Initialize (). It disables & uninstalls
 *          the Interrupt Service Routine and Delayed Procedure Call.
 *
 *  @modif  HPI_DriverObject
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_Finalize (IN ProcessorId dspId, IN LinkId lnkId)
{
    DSP_STATUS      status    = DSP_SOK ;
    DSP_STATUS      tmpStatus = DSP_SOK ;
    HpiDriverObj *  hpiInfo   = NULL    ;
    DspObject    *  dspObj    = NULL    ;
    DspInterface *  dspIntf   = NULL    ;
    Uint32          i                   ;

    TRC_2ENTER ("HPI_Finalize", dspId, lnkId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId])    ;
    dspIntf = dspObj->interface              ;

    /*  ------------------------------------------------------------------------
     *  Disable the interrupt, uninstall it and delete the ISR Object.
     *  ------------------------------------------------------------------------
     */
    status = (*(dspObj->interface->disableInterrupt)) (dspId,
                                                       dspObj,
                                                       &(hpiInfo->intObj)) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    tmpStatus = ISR_Uninstall (hpiInfo->isrObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    tmpStatus = ISR_Delete (hpiInfo->isrObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    hpiInfo->isrObj = NULL ;

    /*  ------------------------------------------------------------------------
     * Cancel the DPC and Free the DPC.
     *  ------------------------------------------------------------------------
    */
    tmpStatus = DPC_Cancel (hpiInfo->dpcObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    tmpStatus = DPC_Delete (hpiInfo->dpcObj) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    hpiInfo->dpcObj = NULL ;

    /*  --------------------------------------------------------------------
     *  Clear the link-driver control structure in DSP memory space
     *  --------------------------------------------------------------------
     */
    hpiInfo->ctrlInfo.gppControl.gppHandshake   = 0x0 ;
    hpiInfo->ctrlInfo.gppControl.dspDataCmdMask = 0x0 ;

#if defined (MSGQ_COMPONENT)
    hpiInfo->ctrlInfo.gppControl.dspMsgqCmdMask = 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

    tmpStatus = (*(dspIntf->write)) (
                                dspId,
                                dspObj,
                                hpiInfo->addrGppControl,
                                Endianism_Default,
                                sizeof (HpiGppControl),
                                (Uint8 *) &(hpiInfo->ctrlInfo.gppControl)) ;
    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    hpiInfo->ctrlInfo.dspControl.dspHandshake   = 0x0 ;
    hpiInfo->ctrlInfo.dspControl.gppDataCmdMask = 0x0 ;

#if defined (MSGQ_COMPONENT)
    hpiInfo->ctrlInfo.dspControl.gppMsgqCmdMask = 0x0 ;
#endif /* if defined (MSGQ_COMPONENT) */

   tmpStatus = (*(dspIntf->write)) (
                                dspId,
                                dspObj,
                                hpiInfo->addrDspControl,
                                Endianism_Default,
                                sizeof (HpiDspControl),
                                (Uint8 *) &(hpiInfo->ctrlInfo.dspControl)) ;

    /*  ------------------------------------------------------------------------
     *  Clear the control structure for all channels in DSP memory space
     *  ------------------------------------------------------------------------
     */
    for (i = 0 ; i < (hpiInfo->numChannels + NUM_MSGQ_CHNLS) ; i++) {
        hpiInfo->chnlInfo [i].command       = HpiCmd_Invalid ;
        hpiInfo->chnlInfo [i].isValid       = FALSE          ;
        hpiInfo->chnlInfo [i].clientBufPtr  = 0              ;
        hpiInfo->chnlInfo [i].clientBufSize = 0              ;
        hpiInfo->chnlInfo [i].sharedBufPtr  = 0              ;
        hpiInfo->chnlInfo [i].sharedBufSize = 0              ;
    }

    tmpStatus = (*(dspIntf->write)) (dspId,
                                     dspObj,
                                     hpiInfo->addrChannels,
                                     Endianism_Default,
                                     (  (sizeof (HpiChnlInfo))
                                      * (  hpiInfo->numChannels
                                         + NUM_MSGQ_CHNLS)),
                                     (Uint8 *) &(hpiInfo->chnlInfo [0])) ;

    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    /*  ------------------------------------------------------------------------
     *  Un-initialize the HPI driver information
     *  ------------------------------------------------------------------------
     */
    hpiInfo->sizeTransferred = 0    ;
    hpiInfo->addrGppControl  = 0    ;
    hpiInfo->addrDspControl  = 0    ;
    hpiInfo->addrChannels    = 0    ;

    TRC_1LEAVE ("HPI_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_OpenChannel
 *
 *  @desc   This function opens a channel to be used for data transfer.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_OpenChannel (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status  = DSP_SOK ;
    HpiDriverObj *  hpiInfo = NULL    ;

    TRC_2ENTER ("HPI_OpenChannel", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   IS_VALID_CHNLID    (dspId, chnlId)
                 || IS_VALID_MSGCHNLID (chnlId)) ;

    hpiInfo = &(HPI_DriverObject [dspId]) ;

    /*  Adjust the channel id to the actual channel id.  */
    if (IS_VALID_MSGCHNLID (chnlId)) {
        chnlId = chnlId - MAX_CHANNELS + hpiInfo->numChannels ;
    }
    else {
        chnlId = chnlId - hpiInfo->baseChnlId ;
    }

    status = HPI_ChannelCtrl (dspId, chnlId, ChannelCtrlCmd_Reset) ;

    if (DSP_FAILED (status)) {
        TRC_0PRINT (TRC_LEVEL7,
                    "Error in resetting the channel control info\n") ;
        SET_FAILURE_REASON ;
    }
    else {
         hpiInfo->chnlInfo [chnlId].command       = HpiCmd_Invalid ;
         hpiInfo->chnlInfo [chnlId].isValid       = FALSE          ;
         hpiInfo->chnlInfo [chnlId].clientBufPtr  = 0              ;
         hpiInfo->chnlInfo [chnlId].clientBufSize = 0              ;
         hpiInfo->chnlInfo [chnlId].sharedBufPtr  = 0              ;
         hpiInfo->chnlInfo [chnlId].sharedBufSize = 0              ;

         status = HPI_WriteChannelInfo (dspId, chnlId) ;
        if (DSP_FAILED (status)) {
            TRC_0PRINT (TRC_LEVEL7,
                        "Error in resetting the channel information"
                         "structure \n") ;
            SET_FAILURE_REASON ;
        }
    }
    TRC_1LEAVE ("HPI_OpenChannel", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_CloseChannel
 *
 *  @desc   This function closes the physical channel corresponding to the
 *          specified logical channel.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_CloseChannel (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status  = DSP_SOK ;
    HpiDriverObj *  hpiInfo = NULL    ;

    TRC_2ENTER ("HPI_CloseChannel", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   IS_VALID_CHNLID    (dspId, chnlId)
                 || IS_VALID_MSGCHNLID (chnlId)) ;

    hpiInfo = &(HPI_DriverObject [dspId]) ;

    /*  Adjust the channel id to the actual channel id.  */
    if (IS_VALID_MSGCHNLID (chnlId)) {
        chnlId = chnlId - MAX_CHANNELS + hpiInfo->numChannels ;
    }
    else {
        chnlId = chnlId - hpiInfo->baseChnlId ;
    }

    status = HPI_ChannelCtrl (dspId, chnlId, ChannelCtrlCmd_Reset) ;

    if (DSP_FAILED (status)) {
        TRC_0PRINT (TRC_LEVEL7,
                    "Error in resetting the channel control info\n") ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_CloseChannel", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_CancelIO
 *
 *  @desc   This function cancels IO on a channel.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_CancelIO (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS      status  = DSP_SOK ;
    HpiDriverObj *  hpiInfo = NULL    ;

    TRC_2ENTER ("HPI_CancelIO", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   IS_VALID_CHNLID    (dspId, chnlId)
                 || IS_VALID_MSGCHNLID (chnlId)) ;

    hpiInfo = &(HPI_DriverObject [dspId]) ;

    /*  Adjust the channel id to the actual channel id.  */
    if (IS_VALID_MSGCHNLID (chnlId)) {
        chnlId = chnlId - MAX_CHANNELS + hpiInfo->numChannels ;
    }
    else {
        chnlId = chnlId - hpiInfo->baseChnlId ;
    }

    status = HPI_ChannelCtrl (dspId, chnlId, ChannelCtrlCmd_Reset) ;

    if (DSP_FAILED (status)) {
        TRC_0PRINT (TRC_LEVEL7,
                    "Error in resetting the channel control info\n") ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_CancelIO", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_ISR
 *
 *  @desc   This function is the interrupt service routine for interrupt used
 *          for data transfer on the physical link between GPP and target DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
HPI_ISR (Pvoid refData)
{
    DSP_STATUS     status   = DSP_SOK                  ;
    HpiDriverObj * hpiInfo  = (HpiDriverObj *) refData ;

    /*  ------------------------------------------------------------------------
     *  Uncomment the statement below, if prints are allowed in interrupt
     *  context.
     *  ------------------------------------------------------------------------
     */
    /* DBC_Require (refData != NULL) ; */


    if (hpiInfo != NULL) {
        status = HPI_ClearDspInterrupt (hpiInfo->dspId) ;

        if (DSP_SUCCEEDED (status)) {
            status = DPC_Schedule (hpiInfo->dpcObj) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
#if defined (DDSP_PROFILE)
            (LDRV_Obj.procStats.procData [hpiInfo->dspId].intsFromDsp)++ ;
#endif /* defined (DDSP_PROFILE) */
        }
        else {
            SET_FAILURE_REASON ;
        }
    }
}


/** ============================================================================
 *  @func   HPI_DPC
 *
 *  @desc   This function is the delayed procedure for performing actual data
 *          transfer on the physical link between GPP and target DSP.
 *          It runs at a priority level between interrupt service routines and
 *          normal tasks/processes/threads.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HPI_DPC (IN Pvoid refData)
{
    DSP_STATUS     status  = DSP_SOK                  ;
    HpiDriverObj * hpiInfo = (HpiDriverObj *) refData ;

    TRC_1ENTER ("HPI_DPC", refData) ;

    DBC_Require (refData != NULL) ;

    if (hpiInfo != NULL) {
        status = HPI_DoDataTransfer (hpiInfo->dspId) ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
            TRC_1PRINT (TRC_LEVEL7, "HPI_DoDataTransfer failed. [0x%x]\n",
                        status) ;
        }
    }

    TRC_1LEAVE ("HPI_DPC", status) ;
}


/** ============================================================================
 *  @func   HPI_ScheduleDPC
 *
 *  @desc   This function schedules the DPC to perform IO operation.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_ScheduleDPC (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS  status  = DSP_SOK ;

    TRC_2ENTER ("HPI_ScheduleDPC", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   IS_VALID_CHNLID    (dspId, chnlId)
                 || IS_VALID_MSGCHNLID (chnlId)) ;

    status = DPC_Schedule (HPI_DriverObject [dspId].dpcObj) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
    TRC_1LEAVE ("HPI_ScheduleDPC", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_IO_Request
 *
 *  @desc   This function submits an IO request to the DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_IO_Request (IN ProcessorId dspId, IN ChannelId chnlId)
{
    DSP_STATUS     status   = DSP_SOK ;
    HpiDriverObj * hpiInfo  = NULL    ;
    HpiChnlInfo *  chnlInfo = NULL    ;
    ChannelMode    chnlMode           ;
    ChannelId      hpiChnlId          ;

    TRC_2ENTER ("HPI_IO_Request", dspId, chnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (   IS_VALID_CHNLID    (dspId, chnlId)
                 || IS_VALID_MSGCHNLID (chnlId)) ;

    hpiInfo  = &(HPI_DriverObject [dspId])   ;

    /*  Adjust the channel id to the actual channel id.  */
    if (IS_VALID_MSGCHNLID (chnlId)) {
        hpiChnlId = chnlId - MAX_CHANNELS + hpiInfo->numChannels ;
    }
    else {
        hpiChnlId = chnlId - hpiInfo->baseChnlId ;
    }

    /*  ------------------------------------------------------------------------
     *  Set command on the channel if no transfer is currently in progress.
     *  ------------------------------------------------------------------------
     */
    status = HPI_ReadChannelInfo (dspId, hpiChnlId) ;

    if (DSP_SUCCEEDED (status)) {
        chnlInfo = &(hpiInfo->chnlInfo [hpiChnlId]) ;
        if (chnlInfo->command == HpiCmd_Invalid) {
            chnlMode = LDRV_CHNL_GetChannelMode (dspId, chnlId) ;

            chnlInfo->isValid = 0x0 ;
            chnlInfo->command =   (chnlMode == ChannelMode_Input)
                                ? HpiCmd_Write
                                : HpiCmd_Read ;

            status = HPI_WriteChannelInfo (dspId, hpiChnlId) ;

            if (DSP_SUCCEEDED (status)) {
                /*  ------------------------------------------------------------
                 *  Set bit corresponding to channel to indicate that GPP is
                 *  ready.
                 *  ------------------------------------------------------------
                 */
                status = HPI_ChannelCtrl (dspId,
                                          hpiChnlId,
                                          ChannelCtrlCmd_Set) ;
                if (DSP_FAILED (status)) {
                    TRC_0PRINT (TRC_LEVEL7,
                                "Error in setting bitmask for the channel\n") ;
                    SET_FAILURE_REASON ;
                }
            }
            else {
                TRC_0PRINT (TRC_LEVEL7,
                            "Error in writing channel control information\n") ;
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        TRC_0PRINT (TRC_LEVEL7, "Error in reading channel information\n") ;
        SET_FAILURE_REASON ;
    }
    /*  ------------------------------------------------------------------------
     *  Send an interrupt to the DSP.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = HPI_InterruptDsp (dspId) ;
        if (DSP_FAILED (status)) {
            TRC_0PRINT (TRC_LEVEL7, "Error in sending interrupt to DSP\n") ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("HPI_IO_Request", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_HandshakeSetup
 *
 *  @desc   This function initializes the memory area used synchronizing
 *          the DSP with GPP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_HandshakeSetup (IN ProcessorId dspId)
{
    DSP_STATUS     status  = DSP_SOK ;
    HpiDriverObj * hpiInfo = NULL    ;
    DspObject *    dspObj  = NULL    ;
    DspInterface * dspIntf = NULL    ;

    TRC_1ENTER ("HPI_IO_HandshakeSetup", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId]) ;
    dspIntf = dspObj->interface ;

    /*  ------------------------------------------------------------------------
     *  Clear the GPP handshake value.
     *  ------------------------------------------------------------------------
     */
    hpiInfo->ctrlInfo.gppControl.gppHandshake = 0x0 ;
    status = (*(dspIntf->write)) (
                    dspId,
                    dspObj,
                    hpiInfo->addrGppControl,
                    Endianism_Default,
                    sizeof (hpiInfo->ctrlInfo.gppControl.gppHandshake),
                    (Uint8 *) &(hpiInfo->ctrlInfo.gppControl.gppHandshake)) ;

    /*  ------------------------------------------------------------------------
     *  Clear the DSP handshake value.
     *  ------------------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        hpiInfo->ctrlInfo.dspControl.dspHandshake = 0x0 ;
        status = (*(dspIntf->write)) (dspId,
                   dspObj,
                   hpiInfo->addrDspControl,
                   Endianism_Default,
                   sizeof (hpiInfo->ctrlInfo.dspControl.dspHandshake),
                   (Uint8 *) &(hpiInfo->ctrlInfo.dspControl.dspHandshake)) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_IO_HandshakeSetup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_HandshakeStart
 *
 *  @desc   This function initiates the handshake on the physical link with
 *          the target DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_HandshakeStart (IN ProcessorId dspId)
{
    DSP_STATUS     status  = DSP_SOK ;
    HpiDriverObj * hpiInfo = NULL    ;
    DspObject *    dspObj  = NULL    ;
    DspInterface * dspIntf = NULL    ;

    TRC_1ENTER ("HPI_HandshakeStart", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId])    ;
    dspIntf = dspObj->interface ;

    /*  ------------------------------------------------------------------------
     *  Write the GPP handshake value.
     *  ------------------------------------------------------------------------
     */
    hpiInfo->ctrlInfo.gppControl.gppHandshake = GPP_HANDSHAKE ;
    status = (*(dspIntf->write)) (dspId,
                    dspObj,
                    hpiInfo->addrGppControl,
                    Endianism_Default,
                    sizeof (hpiInfo->ctrlInfo.gppControl.gppHandshake),
                    (Uint8 *) &(hpiInfo->ctrlInfo.gppControl.gppHandshake)) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_HandshakeStart", status) ;

    return status ;
}


/** ============================================================================
 *  @func   HPI_HandshakeComplete
 *
 *  @desc   This function completes the handshake on the physical link with
 *          the target DSP. This function block until handshake completes.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
HPI_HandshakeComplete (IN ProcessorId dspId)
{
    DSP_STATUS      status  = DSP_SOK ;
    HpiDriverObj *  hpiInfo = NULL    ;
    DspObject *     dspObj  = NULL    ;
    DspInterface *  dspIntf = NULL    ;
    volatile Uint32 dspHandshake      ;
    Uint32          size              ;

#if defined (DDSP_DEBUG)
    Uint32 i = 0 ;
#endif /* defined (DDSP_DEBUG) */

    TRC_1ENTER ("HPI_HandshakeComplete", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId])    ;
    dspIntf = dspObj->interface ;

    do {
        /*  --------------------------------------------------------------------
         *  Read the DSP handshake value.
         *  --------------------------------------------------------------------
         */
        size = sizeof (hpiInfo->ctrlInfo.dspControl.dspHandshake) ;
        status = (*(dspIntf->read)) (dspId,
                                     dspObj,
                                     hpiInfo->addrDspControl,
                                     Endianism_Default,
                                     &size,
                                     (Uint8 *) &(dspHandshake)) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
#if defined (DDSP_DEBUG)
        /*  --------------------------------------------------------------------
         *  Set the failure after a pre-specified count when DSP has been unable
         *  to write the handshake value.
         *  --------------------------------------------------------------------
         */
        i++ ;
        if (i == MAX_POLL_COUNT) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
#endif /* defined (DDSP_DEBUG) */
    } while ((dspHandshake != DSP_HANDSHAKE) && DSP_SUCCEEDED (status)) ;

#if defined (DDSP_DEBUG)
    /*  Update the local structure to indicate that handshake is complete  */
    hpiInfo->ctrlInfo.dspControl.dspHandshake = dspHandshake ;
#endif /* defined (DDSP_DEBUG) */

    TRC_1LEAVE ("HPI_HandshakeComplete", status) ;

    return status ;
}


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   HPI_Debug
 *
 *  @desc   Prints information related to HPI Link Driver.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Void
HPI_Debug (ProcessorId dspId)
{
    HpiDriverObj *  hpiInfo = NULL ;

    TRC_1ENTER ("HPI_Debug", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    hpiInfo = &(HPI_DriverObject [dspId]) ;

    TRC_0PRINT (TRC_LEVEL4, "HPI Driver Info\n") ;

    TRC_1PRINT (TRC_LEVEL4, "    dspId = 0x%x\n", hpiInfo->dspId) ;
    TRC_1PRINT (TRC_LEVEL4, "    linkId = 0x%x\n", hpiInfo->linkId) ;
    TRC_1PRINT (TRC_LEVEL4, "    baseChnlId = 0x%x\n", hpiInfo->baseChnlId) ;
    TRC_1PRINT (TRC_LEVEL4, "    numChannels = 0x%x\n", hpiInfo->numChannels) ;
    TRC_1PRINT (TRC_LEVEL4, "    intObj.intId = 0x%x\n",
                                            hpiInfo->intObj.intId) ;
    TRC_1PRINT (TRC_LEVEL4, "    isrObj = 0x%x\n", hpiInfo->isrObj) ;
    TRC_1PRINT (TRC_LEVEL4, "    dpcObj = 0x%x\n", hpiInfo->dpcObj) ;
    TRC_1PRINT (TRC_LEVEL4, "    addrGppControl = 0x%x\n",
                                            hpiInfo->addrGppControl) ;
    TRC_1PRINT (TRC_LEVEL4, "    addrDspControl = 0x%x\n",
                                            hpiInfo->addrDspControl) ;
    TRC_1PRINT (TRC_LEVEL4, "    addrChannels = 0x%x\n",
                                            hpiInfo->addrChannels) ;
    TRC_1PRINT (TRC_LEVEL4, "    sizeTransferred = 0x%x\n",
                                            hpiInfo->sizeTransferred) ;

    TRC_0PRINT (TRC_LEVEL4, "HPI GPP Control Area") ;
    TRC_1PRINT (TRC_LEVEL4, "    gppHandshake = 0x%x\n",
                                hpiInfo->ctrlInfo.gppControl.gppHandshake) ;
    TRC_1PRINT (TRC_LEVEL4, "    dspDataCmdMask  = 0x%x\n",
                                hpiInfo->ctrlInfo.gppControl.dspDataCmdMask) ;

#if defined (MSGQ_COMPONENT)
    TRC_1PRINT (TRC_LEVEL4, "    dspMsgqCmdMask  = 0x%x\n",
                                hpiInfo->ctrlInfo.gppControl.dspMsgqCmdMask) ;
#endif /* if defined (MSGQ_COMPONENT) */

    TRC_0PRINT (TRC_LEVEL4, "HPI DSP Control Area") ;
    TRC_1PRINT (TRC_LEVEL4, "    gppHandshake = 0x%x\n",
                                hpiInfo->ctrlInfo.dspControl.dspHandshake) ;
    TRC_1PRINT (TRC_LEVEL4, "    gppDataCmdMask  = 0x%x\n",
                                hpiInfo->ctrlInfo.dspControl.gppDataCmdMask) ;

#if defined (MSGQ_COMPONENT)
    TRC_1PRINT (TRC_LEVEL4, "    gppMsgqCmdMask  = 0x%x\n",
                                hpiInfo->ctrlInfo.dspControl.gppMsgqCmdMask) ;
#endif /* if defined (MSGQ_COMPONENT) */

    TRC_0LEAVE ("HPI_Debug") ;
}
#endif /* defined (DDSP_DEBUG) */


/** ----------------------------------------------------------------------------
 *  @func   HPI_ClearDspInterrupt
 *
 *  @desc   This function clears the DSP interrupt.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_ClearDspInterrupt (ProcessorId dspId)
{
    DSP_STATUS       status  = DSP_SOK ;
    DspObject *      dspObj  = NULL    ;
    DspInterface *   dspIntf = NULL    ;
    HpiDriverObj *   hpiInfo = NULL    ;
    Uint32           intVal            ;
    InterruptObject  intObj            ;

    /*  ------------------------------------------------------------------------
     *  Uncomment the statement below, if prints are allowed in interrupt
     *  context.
     *  ------------------------------------------------------------------------
     */
    /* TRC_1ENTER ("HPI_ClearDspInterrupt", dspId) ; */
    /* DBC_Require (IS_VALID_PROCID (dspId)) ; */

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;
    hpiInfo = &(HPI_DriverObject [dspId]) ;

    intObj.intId = hpiInfo->intObj.intId ;

    status = (*(dspIntf->clearInterrupt)) (dspId,
                                           dspObj,
                                           &intObj,
                                           (Pvoid) &intVal) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    /*  ------------------------------------------------------------------------
     *  Uncomment the statement below, if prints are allowed in interrupt
     *  context.
     *  ------------------------------------------------------------------------
     */
    /* TRC_1LEAVE ("HPI_ClearDspInterrupt", status) ; */

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   HPI_InterruptDsp
 *
 *  @desc   This function sends the interrupt to DSP.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_InterruptDsp (IN ProcessorId dspId)
{
    DSP_STATUS       status  = DSP_SOK ;
    DspObject *      dspObj  = NULL    ;
    DspInterface *   dspIntf = NULL    ;
    HpiDriverObj *   hpiInfo = NULL    ;
    InterruptObject  intObj            ;
    Uint32           intVal            ;

    TRC_1ENTER ("HPI_InterruptDsp", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    dspIntf = dspObj->interface ;
    hpiInfo  = &(HPI_DriverObject [dspId]) ;

    intObj.intId = hpiInfo->intObj.intId ;

    status = (*(dspIntf->interrupt)) (dspId, dspObj, &intObj, (Pvoid) &intVal) ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

#if defined (DDSP_PROFILE)
    if (DSP_SUCCEEDED (status)) {
        (LDRV_Obj.procStats.procData [dspId].intsToDsp)++ ;
    }
#endif /* defined (DDSP_PROFILE) */

    TRC_1LEAVE ("HPI_InterruptDsp", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   HPI_DoDataTransfer
 *
 *  @desc   This function drives input and output operations on channels after
 *          checking pending commands on the channels.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_DoDataTransfer (IN ProcessorId dspId)
{
    DSP_STATUS      status    = DSP_SOK        ;
    DspObject *     dspObj    = NULL           ;
    HpiDriverObj *  hpiInfo   = NULL           ;
    Uint32          size      = 0              ;
    ChannelId       hpiChnlId = CHNLID_INVALID ;

    TRC_1ENTER ("HPI_DoDataTransfer", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId]) ;

    /*  ------------------------------------------------------------------------
     *  Read the control information from the DSP memory space.
     *  ------------------------------------------------------------------------
     */

#if defined (MSGQ_COMPONENT)
    size = sizeof (hpiInfo->ctrlInfo.dspControl.gppMsgqCmdMask) ;
    status = HPI_DspRead (
                 dspId,
                 dspObj,
                 hpiInfo->addrDspControl + GPP_MSGQMASK_OFFSET,
                 Endianism_Default,
                 &size,
                 (Uint8 *) &(hpiInfo->ctrlInfo.dspControl.gppMsgqCmdMask)) ;

    if (DSP_SUCCEEDED (status)) {
        if (hpiInfo->ctrlInfo.dspControl.gppMsgqCmdMask == 1) {
            hpiChnlId = ID_MSGCHNL_TO_DSP ;
        }
        else if (hpiInfo->ctrlInfo.dspControl.gppMsgqCmdMask == 2) {
            hpiChnlId = ID_MSGCHNL_FM_DSP ;
        }
    }
    else {
        SET_FAILURE_REASON ;
    }
#endif /* if defined (MSGQ_COMPONENT) */

    if (hpiChnlId == CHNLID_INVALID) {
        size = sizeof (hpiInfo->ctrlInfo.dspControl.gppDataCmdMask) ;
        status = HPI_DspRead (
                     dspId,
                     dspObj,
                     hpiInfo->addrDspControl + GPP_DATAMASK_OFFSET,
                     Endianism_Default,
                     &size,
                     (Uint8 *) &(hpiInfo->ctrlInfo.dspControl.gppDataCmdMask)) ;

        if (DSP_SUCCEEDED (status)) {
            /*  ----------------------------------------------------------------
             *  Find the channel on which GPP has to take some action.
             *  ----------------------------------------------------------------
             */
            hpiChnlId = HPI_GetChannelId (
                                hpiInfo->ctrlInfo.dspControl.gppDataCmdMask,
                                hpiInfo->numChannels) ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    if (hpiChnlId != CHNLID_INVALID) {
        status = HPI_DoChnlProcessing (dspId, hpiChnlId) ;
    }

    TRC_1LEAVE ("HPI_DoDataTransfer", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   HPI_DoChnlProcessing
 *
 *  @desc   This function is called by the DPC for processing on a channel.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_DoChnlProcessing (IN     ProcessorId   dspId,
                      IN     ChannelId     hpiChnlId)
{
    DSP_STATUS      status      = DSP_SOK            ;
    DspObject *     dspObj      = NULL               ;
    HpiDriverObj *  hpiInfo     = NULL               ;
    ChannelState    chnlState   = ChannelState_Ready ;
    HpiChnlInfo *   chnlInfo    = NULL               ;
    LDRVChnlIRP *   chirp       = NULL               ;
    Bool            fUpdateChnl = FALSE              ;
    ChannelId       chnlId      ;
    ChannelMode     chnlMode    ;
    Uint32          size        ;

    TRC_2ENTER ("HPI_DoChnlProcessing", dspId, hpiChnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (IS_VALID_HPI_CHNLID (hpiChnlId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId])  ;
    hpiInfo = &(HPI_DriverObject [dspId])     ;
    chnlId  = hpiChnlId + hpiInfo->baseChnlId ;

    chnlState = LDRV_CHNL_GetChannelState (dspId, chnlId) ;
    chnlInfo = &(hpiInfo->chnlInfo [hpiChnlId]) ;

    if ((chnlState & ~ChannelState_EOS) == ChannelState_Ready) {
        status = HPI_ReadChannelInfo (dspId, hpiChnlId) ;
        if (DSP_SUCCEEDED (status)) {
            if (   (chnlInfo->command != HpiCmd_Invalid)
                && (chnlInfo->isValid == TRUE)) {
                chirp = LDRV_CHNL_GetRequestChirp (dspId, hpiChnlId) ;

                if (chirp == NULL) {
                    /*  --------------------------------------------------------
                     *  DSP sent a buffer transfer request based on an earlier
                     *  request from GPP but the GPP side channel has either
                     *  closed or been cancelled.
                     *  --------------------------------------------------------
                     */
                    fUpdateChnl = TRUE ;
                    chnlInfo->isValid       = FALSE          ;
                    chnlInfo->command       = HpiCmd_Invalid ;
                    chnlInfo->sharedBufSize = 0              ;
                    status = HPI_UpdateChnlInfo (dspId, chnlId, chnlInfo) ;
                }
            }
            else {
                status = DSP_EMULINST ;
            }

            if (chirp != NULL) {
                chnlMode  = LDRV_CHNL_GetChannelMode  (dspId, chnlId) ;
                switch (chnlInfo->command) {
                case HpiCmd_Read:
                    if (chnlMode == ChannelMode_Input) {
                        /*  ----------------------------------------------------
                         *  Read data buffer.
                         *  ----------------------------------------------------
                         */
                        size = MIN (chnlInfo->sharedBufSize * DM642_MAUSIZE,
                                    chirp->size) ;
                        if (size == 0) {
                            /*  ------------------------------------------------
                             *  Buffer length of zero bytes indicates EOS.
                             *  This Asertion fails if DSP send EOS more than
                             *  once on a given channel.
                             *  ------------------------------------------------
                             */
                            DBC_Assert ((chnlState & ChannelState_EOS) == 0) ;

                            /*  ------------------------------------------------
                             *  Update channel state & IO Completion status
                             *  ------------------------------------------------
                             */
                            chnlState = ChannelState_EOS ;
                            chirp->iocStatus = LDRV_CHNL_IOCSTATE_EOS ;

                            LDRV_CHNL_SetChannelState (dspId,
                                                       chnlId,
                                                       chnlState) ;
                        }
                        else {
                            status = HPI_DspRead (dspId,
                                                  dspObj,
                                                  chnlInfo->sharedBufPtr,
                                                  Endianism_Default,
                                                  &size,
                                                  chirp->buffer) ;
                            if (DSP_SUCCEEDED (status)) {
                                chnlInfo->sharedBufSize = size / DM642_MAUSIZE ;
                                chirp->size       = size ;
                                chirp->iocStatus |=
                                               LDRV_CHNL_IOCSTATE_COMPLETE ;
                            }
                            else {
                                SET_FAILURE_REASON ;
                            }
                        }

                        if (DSP_SUCCEEDED (status)) {
                            fUpdateChnl = TRUE ;
                            chnlInfo->isValid       = FALSE        ;
                            chnlInfo->command       = HpiCmd_Write ;

                            status = HPI_UpdateChnlInfo (dspId,
                                                         chnlId,
                                                         chnlInfo) ;
                        }
                    }
                    break ;

                case HpiCmd_Write:
                    if (chnlMode == ChannelMode_Output) {
                        /*  ----------------------------------------------------
                         *  Write data buffer.
                         *  ----------------------------------------------------
                         */
                        size = MIN (chnlInfo->sharedBufSize * DM642_MAUSIZE,
                                    chirp->size) ;
                        status = HPI_DspWrite (dspId,
                                               dspObj,
                                               chnlInfo->sharedBufPtr,
                                               Endianism_Default,
                                               size,
                                               chirp->buffer) ;
                        if (DSP_SUCCEEDED (status)) {
                            fUpdateChnl = TRUE ;
                            chnlInfo->isValid       = FALSE                ;
                            chnlInfo->command       = HpiCmd_Read          ;
                            chnlInfo->sharedBufSize = size / DM642_MAUSIZE ;

                            chirp->size       = size ;
                            chirp->iocStatus |= LDRV_CHNL_IOCSTATE_COMPLETE ;
                            status = HPI_UpdateChnlInfo (dspId,
                                                         chnlId,
                                                         chnlInfo) ;
                        }
                        else {
                            SET_FAILURE_REASON ;
                        }
                    }
                    break ;
                } /* switch */
            } /* (chirp != NULL) */
        } /* HPI_ReadChannelInfo (dspId, hpiChnlId) is successfull */
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    } /* if ((chnlState & ~ChannelState_EOS) == ChannelState_Ready) */
    else {
        fUpdateChnl = TRUE ;
        chnlInfo->isValid       = FALSE          ;
        chnlInfo->command       = HpiCmd_Invalid ;
        chnlInfo->sharedBufSize = 0              ;
    }

    if ((fUpdateChnl == TRUE) && DSP_SUCCEEDED (status)) {
        status = HPI_WriteChannelInfo (dspId, hpiChnlId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
#if defined (DDSP_PROFILE)
        LDRV_Obj.chnlStats.chnlData [dspId][hpiChnlId].transferred += size ;
#endif /* defined (DDSP_PROFILE) */
    }

    /*  ----------------------------------------------------------------
     *  Send an interrupt to the DSP to indicate completion of transfer.
     *  ----------------------------------------------------------------
     */
    if (DSP_SUCCEEDED (status)) {
        status = HPI_InterruptDsp (dspId) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    if ((chirp != NULL) && DSP_SUCCEEDED (status)) {
        status = HPI_InvokeCallback (dspId, hpiChnlId, chirp) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    /*  --------------------------------------------------------------------
     *  DSP_EMULINST indicates that no processing was done in this call.
     *  --------------------------------------------------------------------
     */
    if (status == DSP_EMULINST) {
        status = DSP_SOK ;
    }

    TRC_1LEAVE ("HPI_DoChnlProcessing", status) ;

    return status ;
}



/*  ----------------------------------------------------------------------------
 *  @func   HPI_UpdateChnlInfo
 *
 *  @desc   This function updates the channel information structure with
 *          after completion of the data transfer.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_UpdateChnlInfo (IN ProcessorId    dspId,
                    IN ChannelId      hpiChnlId,
                    IN HpiChnlInfo *  chnlInfo)
{
    DSP_STATUS   status  = DSP_SOK ;

    TRC_3ENTER ("HPI_UpdateChnlInfo", dspId, hpiChnlId, chnlInfo) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (chnlInfo != NULL) ;
    DBC_Require (IS_VALID_HPI_CHNLID (hpiChnlId)) ;

    if (LDRV_CHNL_ChannelHasMoreChirps (dspId, hpiChnlId) == FALSE) {
        /*  --------------------------------------------------------------------
         *  Clear the bit mask for channel if no more
         *  pending IO on channel.
         *  --------------------------------------------------------------------
         */
        status = HPI_ChannelCtrl (dspId, hpiChnlId, ChannelCtrlCmd_Reset) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }

        /*  Overwrite the channel command  */
        chnlInfo->command = HpiCmd_Invalid ;
    }

    TRC_1LEAVE ("HPI_UpdateChnlInfo", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   HPI_InvokeCallback
 *
 *  @desc   This function invokes the callback functions required.
 *          - AddIOCompletion to indicate that the data transfer is complete.
 *          - chirp->callback () if it is non-NULL.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_InvokeCallback (IN ProcessorId   dspId,
                    IN ChannelId     hpiChnlId,
                    IN LDRVChnlIRP * chirp)
{
    DSP_STATUS      status    = DSP_SOK ;

    TRC_3ENTER ("HPI_InvokeCallback", dspId, hpiChnlId, chirp) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (IS_VALID_HPI_CHNLID (hpiChnlId)) ;
    DBC_Require (chirp != NULL) ;

    status = LDRV_CHNL_AddIOCompletion (dspId, hpiChnlId, chirp) ;
    if (DSP_SUCCEEDED (status)) {
        /* Call the callback function if it is specified */
        if (chirp->callback != NULL) {
            status = chirp->callback (dspId,
                                      status,
                                      chirp->buffer,
                                      chirp->size,
                                      (Pvoid) (chirp->arg)) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
        }
    }
    else {
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_InvokeCallback", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   HPI_GetChannelId
 *
 *  @desc   This function finds the next channel on which a command is pending.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
ChannelId
HPI_GetChannelId (IN  Uint16 chnlMask,
                  IN  Uint16 maxChannels)
{
    ChannelId chnlId = 0     ;
    Bool      found  = FALSE ;

    TRC_2ENTER ("HPI_GetChannelId", chnlMask, maxChannels) ;

    if (chnlMask != 0) {
        do {
            if ((chnlMask & (1 << chnlId)) != 0) {
                found = TRUE ;
            }
            else {
                chnlId = chnlId + 1 ;
            }
        } while ((chnlId != maxChannels) && (found == FALSE)) ;
    }

    if (found == FALSE) {
        chnlId = CHNLID_INVALID ;
    }

    TRC_1LEAVE ("HPI_GetChannelId", chnlId) ;

    return chnlId ;
}


/** ----------------------------------------------------------------------------
 *  @func   HPI_ChannelCtrl
 *
 *  @desc   This function resets a channel by updating the data
 *          structures accordingly.
 *          Specifically it performs the following actions:
 *          - Resets the bit corresponding to the channel id in the dspDataMask.
 *
 *  @modif  HpiControl, HpiDriverObj
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_ChannelCtrl (IN ProcessorId     dspId,
                 IN ChannelId       hpiChnlId,
                 IN ChannelCtrlCmd  cmd)
{
    DSP_STATUS      status        = DSP_SOK ;
    HpiDriverObj  * hpiInfo       = NULL    ;
    DspObject     * dspObj        = NULL    ;
    Uint32          size                    ;
    Uint32          value                   ;
    Uint32          offset                  ;

    TRC_3ENTER ("HPI_ChannelCtrl", dspId, hpiChnlId, cmd) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (IS_VALID_HPI_CHNLID (hpiChnlId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId]) ;

    /*  ------------------------------------------------------------------------
     *  Read the current HPI driver control structure, set the required bits
     *  and write it back.
     *  Also update the local bitmasks depending on the mode.
     *  ------------------------------------------------------------------------
     */
    size = sizeof (Uint32) ;
    if (IS_VALID_HPI_MSGCHNLID (hpiChnlId)) {
        offset = DSP_MSGQMASK_OFFSET ;
    }
    else {
        offset = DSP_DATAMASK_OFFSET ;
    }


    status = HPI_DspRead (dspId,
                          dspObj,
                          hpiInfo->addrGppControl + offset,
                          Endianism_Default,
                          &size,
                          (Uint8 *) &value) ;

    if (DSP_SUCCEEDED (status)) {
        if (cmd == ChannelCtrlCmd_Reset) {
            if (IS_VALID_HPI_MSGCHNLID (hpiChnlId)) {
                CLEAR_BIT (value, hpiChnlId - NUM_DATA_CHNLS) ;
            }
            else {
                CLEAR_BIT (value, hpiChnlId) ;
            }
        }
        else {
            if (IS_VALID_HPI_MSGCHNLID (hpiChnlId)) {
                SET_BIT (value, hpiChnlId - NUM_DATA_CHNLS) ;
            }
            else {
                SET_BIT (value, hpiChnlId) ;
            }
        }

        status = HPI_DspWrite (dspId,
                               dspObj,
                               hpiInfo->addrGppControl + offset,
                               Endianism_Default,
                               size,
                               (Uint8 *) &(value)) ;

        if (DSP_FAILED (status)) {
            TRC_0PRINT (TRC_LEVEL7, "Error in updating control info\n") ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        TRC_0PRINT (TRC_LEVEL7, "Error in getting control info\n") ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_ChannelCtrl", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   HPI_ReadChannelInfo
 *
 *  @desc   This function reads the HPI link control information pertaining to
 *          the channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_ReadChannelInfo (IN  ProcessorId dspId, IN  ChannelId hpiChnlId)
{
    DSP_STATUS     status   = DSP_SOK ;
    HpiDriverObj * hpiInfo  = NULL    ;
    DspObject    * dspObj   = NULL    ;
    Uint32         size               ;

    TRC_2ENTER ("HPI_ReadChannelInfo", dspId, hpiChnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (IS_VALID_HPI_CHNLID (hpiChnlId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId]) ;

    size = sizeof (HpiChnlInfo) ;
    status = HPI_DspRead (dspId,
                          dspObj,
                          hpiInfo->addrChannels + (hpiChnlId * size),
                          Endianism_Default,
                          &size,
                          (Uint8 *) &(hpiInfo->chnlInfo [hpiChnlId])) ;
    if (DSP_FAILED (status)) {
        TRC_0PRINT (TRC_LEVEL7,
                    "Error in reading channel control information.\n") ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_ReadChannelInfo", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   HPI_WriteChannelInfo
 *
 *  @desc   This function writes the HPI link control information pertaining to
 *          the channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_WriteChannelInfo (IN ProcessorId    dspId,
                      IN ChannelId      hpiChnlId)
{
    DSP_STATUS     status   = DSP_SOK ;
    HpiDriverObj * hpiInfo  = NULL    ;
    DspObject    * dspObj   = NULL    ;
    Uint32         size               ;

    TRC_2ENTER ("HPI_WriteChannelInfo", dspId, hpiChnlId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (IS_VALID_HPI_CHNLID (hpiChnlId)) ;

    dspObj  = &(LDRV_Obj.dspObjects [dspId]) ;
    hpiInfo = &(HPI_DriverObject [dspId]) ;

    size = sizeof (HpiChnlInfo) ;
    status = HPI_DspWrite (dspId,
                           dspObj,
                           hpiInfo->addrChannels + (hpiChnlId * size),
                           Endianism_Default,
                           size,
                           (Uint8 *) &(hpiInfo->chnlInfo [hpiChnlId])) ;
    if (DSP_FAILED (status)) {
        TRC_0PRINT (TRC_LEVEL7,
                    "Error in writing channel control information.\n") ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("HPI_WriteChannelInfo", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   HPI_DspRead
 *
 *  @desc   Reads data from DSP through HPI interface.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_DspRead (IN     ProcessorId  dspId,
             IN     DspObject *  dspObj,
             IN     Uint32       dspAddr,
             IN     Endianism    endianInfo,
             IN OUT Uint32 *     numBytes,
             OUT    Uint8 *      buffer)
{
    DSP_STATUS     status    = DSP_SOK ;
    DSP_STATUS     tmpStatus = DSP_SOK ;
    HpiDriverObj * hpiInfo   = NULL    ;
    DspInterface * dspIntf   = NULL    ;

    hpiInfo = &(HPI_DriverObject [dspId]) ;
    dspIntf = dspObj->interface ;

    /*  ------------------------------------------------------------------------
     *  Disable the ISR.
     *  ------------------------------------------------------------------------
     */
    tmpStatus = ISR_Disable (hpiInfo->isrObj) ;

    if (DSP_SUCCEEDED (tmpStatus)) {
        /*  --------------------------------------------------------------------
         *  Read data from DSP memory.
         *  --------------------------------------------------------------------
         */
        status = (*(dspIntf->read)) (dspId, dspObj, dspAddr, endianInfo,
                                     numBytes, buffer) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        /*  --------------------------------------------------------------------
         *  Enable ISR if it was disabled.
         *  --------------------------------------------------------------------
         */
        tmpStatus = ISR_Enable (hpiInfo->isrObj) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   HPI_DspWrite
 *
 *  @desc   Writes data to DSP through HPI interface.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
HPI_DspWrite (IN     ProcessorId  dspId,
              IN     DspObject *  dspObj,
              IN     Uint32       dspAddr,
              IN     Endianism    endianInfo,
              IN OUT Uint32       numBytes,
              OUT    Uint8 *      buffer)
{
    DSP_STATUS     status    = DSP_SOK ;
    DSP_STATUS     tmpStatus = DSP_SOK ;
    HpiDriverObj * hpiInfo   = NULL    ;
    DspInterface * dspIntf   = NULL    ;

    hpiInfo = &(HPI_DriverObject [dspId]) ;
    dspIntf = dspObj->interface ;

    /*  ------------------------------------------------------------------------
     *  Disable the ISR.
     *  ------------------------------------------------------------------------
     */
    tmpStatus = ISR_Disable (hpiInfo->isrObj) ;

    if (DSP_SUCCEEDED (tmpStatus)) {
        /*  --------------------------------------------------------------------
         *  Write data to DSP memory.
         *  --------------------------------------------------------------------
         */
        status = (*(dspIntf->write)) (dspId, dspObj, dspAddr, endianInfo,
                                      numBytes, buffer) ;
        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
        /*  --------------------------------------------------------------------
         *  Enable ISR if it was disabled.
         *  --------------------------------------------------------------------
         */
        tmpStatus = ISR_Enable (hpiInfo->isrObj) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }
    else {
        status = tmpStatus ;
        SET_FAILURE_REASON ;
    }

    return status ;
}


#if defined (__cplusplus)
}
#endif /* if defined (__cplusplus) */
