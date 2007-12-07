/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSL_MCBSP_H_
#define _CSL_MCBSP_H_

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_mcbsp.h>

/**************************************************************************\
* MCBSP global macro declarations
\**************************************************************************/

/* IO pin Selection to read/write */
 #define CSL_MCBSP_IO_CLKX                             (1)
 /* I/O Pin Input/Output configuration for CLKX Pin              */
 #define CSL_MCBSP_IO_FSX                              (2)
 /* I/O Pin Input/Output configuration for FSX  Pin              */
 #define CSL_MCBSP_IO_DX                               (4)
 /* Not Configurable. Always Output.                             */
 #define CSL_MCBSP_IO_CLKR                             (8)
 /* I/O Pin Input/Output configuration for CLKR Pin              */
 #define CSL_MCBSP_IO_FSR                              (16)
 /* I/O Pin Input/Output configuration for FSR Pin               */
 #define CSL_MCBSP_IO_DR                               (32)
 /* Not Configurable. Always Input.                              */
 #define CSL_MCBSP_IO_CLKS                             (64)
 /* Not Configurable. Always Input.                              */

/* Options for Mcbsp reset control */
 #define CSL_MCBSP_CTRL_RX_ENABLE                      (1)
 /* To enable Receiver in resetControl Function                  */
 #define CSL_MCBSP_CTRL_TX_ENABLE                      (2)
 /* To enable Transmitter in resetControl Function               */
 #define CSL_MCBSP_CTRL_RX_DISABLE                     (4)
 /* To disable Receiver in resetControl Function                 */
 #define CSL_MCBSP_CTRL_TX_DISABLE                     (8)
 /* To disable Transmitter in resetControl Function              */
 #define CSL_MCBSP_CTRL_FSYNC_ENABLE                   (16)
 /* To enable Frame Sync Generation in resetControl Function     */
 #define CSL_MCBSP_CTRL_SRG_ENABLE                     (32)
 /* To enable Sample Rate Generator in resetControl Function     */
 #define CSL_MCBSP_CTRL_FSYNC_DISABLE                  (64)
 /* To disable Frame Sync Generation in resetControl Function    */
 #define CSL_MCBSP_CTRL_SRG_DISABLE                    (128)
 /* To disable Sample Rate Generator in resetControl Function    */

/* Error codes for McBSP */
#define CSL_EMCBSP_INVCNTLCMD                         (CSL_EMCBSP_FIRST - 0)
#define CSL_EMCBSP_INVQUERY                           (CSL_EMCBSP_FIRST - 1)
#define CSL_EMCBSP_INVPARAMS                          (CSL_EMCBSP_FIRST - 2)
#define CSL_EMCBSP_INVSIZE                            (CSL_EMCBSP_FIRST - 3)
#define CSL_EMCBSP_NOTEXIST                           (CSL_EMCBSP_FIRST - 4)

#define CSL_MCBSP_HWSETUP_DEFAULTS {    \
    NULL,                               \
    NULL,                               \
    NULL,                               \
    NULL,                               \
    NULL,                               \
    (CSL_McbspEmulation)( CSL_MCBSP_SPCR2_FREE_RESETVAL << 1 | CSL_MCBSP_SPCR2_SOFT_RESETVAL),  \
    NULL        }

#define CSL_MCBSP_GLOBALSETUP_DEFAULTS {                \
    (CSL_McbspIOEnable)                                 \
        (CSL_MCBSP_PCR_XIOEN_RESETVAL << 1  |  CSL_MCBSP_PCR_RIOEN_RESETVAL), \
    CSL_MCBSP_SPCR1_DLB_RESETVAL,                                 \
    (CSL_McbspClkStop)CSL_MCBSP_SPCR1_CLKSTP_RESETVAL                               \
}

#define CSL_MCBSP_RXDATASETUP_DEFAULTS{                      \
    CSL_MCBSP_RCR2_RPHASE_RESETVAL,                      \
    (CSL_McbspWordLength) CSL_MCBSP_RCR1_RWDLEN1_RESETVAL, \
    CSL_MCBSP_RCR1_RFRLEN1_RESETVAL,                     \
    (CSL_McbspWordLength) CSL_MCBSP_RCR2_RWDLEN2_RESETVAL, \
    CSL_MCBSP_RCR2_RFRLEN2_RESETVAL,                     \
    CSL_MCBSP_RCR2_RFIG_RESETVAL,                        \
    (CSL_McbspCompand) CSL_MCBSP_RCR2_RCOMPAND_RESETVAL, \
    (CSL_McbspDataDelay) CSL_MCBSP_RCR2_RDATDLY_RESETVAL,\
    CSL_MCBSP_SPCR1_RJUST_RESETVAL,                      \
    (CSL_McbspIntEvent) CSL_MCBSP_SPCR1_RINTM_RESETVAL   \
}

#define CSL_MCBSP_TXDATASETUP_DEFAULTS{              \
    CSL_MCBSP_XCR2_XPHASE_RESETVAL,                      \
    (CSL_McbspWordLength) CSL_MCBSP_XCR1_XWDLEN1_RESETVAL, \
    CSL_MCBSP_XCR1_XFRLEN1_RESETVAL,                     \
    (CSL_McbspWordLength) CSL_MCBSP_XCR2_XWDLEN2_RESETVAL, \
    CSL_MCBSP_XCR2_XFRLEN2_RESETVAL,                     \
    CSL_MCBSP_XCR2_XFIG_RESETVAL,                        \
    (CSL_McbspCompand) CSL_MCBSP_XCR2_XCOMPAND_RESETVAL, \
    (CSL_McbspDataDelay) CSL_MCBSP_XCR2_XDATDLY_RESETVAL,\
    CSL_MCBSP_SPCR1_DXENA_RESETVAL,                      \
    (CSL_McbspIntEvent) CSL_MCBSP_SPCR2_XINTM_RESETVAL   \
}

#define CSL_MCBSP_CLKSETUP_DEFAULTS{  \
    (CSL_McbspClkMode) CSL_MCBSP_PCR_FSRM_RESETVAL,  \
    (CSL_McbspClkMode) CSL_MCBSP_PCR_FSXM_RESETVAL,  \
    (CSL_McbspClkMode) CSL_MCBSP_PCR_CLKXM_RESETVAL, \
    (CSL_McbspClkMode) CSL_MCBSP_PCR_CLKRM_RESETVAL, \
    (CSL_McbspClkPolarity) CSL_MCBSP_PCR_FSXP_RESETVAL, \
    (CSL_McbspClkPolarity) CSL_MCBSP_PCR_FSRP_RESETVAL, \
    (CSL_McbspClkPolarity) CSL_MCBSP_PCR_CLKXP_RESETVAL, \
    (CSL_McbspClkPolarity) CSL_MCBSP_PCR_CLKRP_RESETVAL, \
    (Uint16) CSL_MCBSP_SRGR1_FWID_RESETVAL, \
    (Uint16) CSL_MCBSP_SRGR2_FPER_RESETVAL, /* srgFrmPeriod */ \
    (Uint16) CSL_MCBSP_SRGR1_CLKGDV_RESETVAL, /* srgClkDivide */ \
    (CSL_McbspSrgInputClk)                                       \
        ( CSL_MCBSP_PCR_SCLKME_RESETVAL << 1 | CSL_MCBSP_SRGR2_CLKSM_RESETVAL),  \
    (CSL_McbspClkPolarity) CSL_MCBSP_SRGR2_CLKSP_RESETVAL, \
    (CSL_McbspSrgFrmSyncGen) CSL_MCBSP_SRGR2_FSGM_RESETVAL, \
    (Uint16) CSL_MCBSP_SRGR2_GSYNC_RESETVAL \
}

#define CSL_MCBSP_MULCHSETUP_DEFAULTS{                      \
    (CSL_McbspPartitionType) CSL_MCBSP_MCR1_RMCME_RESETVAL, \
    (CSL_McbspPartitionType) CSL_MCBSP_MCR2_XMCME_RESETVAL, \
    CSL_MCBSP_MCR1_RMCM_RESETVAL,                           \
    CSL_MCBSP_MCR2_XMCM_RESETVAL,                           \
    (CSL_McbspPartABlk) CSL_MCBSP_MCR1_RPABLK_RESETVAL,     \
    (CSL_McbspPartBBlk) CSL_MCBSP_MCR1_RPBBLK_RESETVAL,     \
    (CSL_McbspPartABlk) CSL_MCBSP_MCR2_XPABLK_RESETVAL,     \
    (CSL_McbspPartBBlk) CSL_MCBSP_MCR2_XPBBLK_RESETVAL      \
}

/* MCBSP global typedef declarations
\**************************************************************************/
/* Mcbsp Instances */
typedef enum {
    CSL_MCBSP_ANY                  =                 -1,
    CSL_MCBSP_1                    =                  0,
    CSL_MCBSP_3                    =                  1
} CSL_McbspNum;

/* This enum should be to set Word Length in each Phase in each Frame */
typedef enum {
    CSL_MCBSP_WORD_LENGTH_8        =                  0,
    CSL_MCBSP_WORD_LENGTH_12       =                  1,
    CSL_MCBSP_WORD_LENGTH_16       =                  2,
    CSL_MCBSP_WORD_LENGTH_20       =                  3,
    CSL_MCBSP_WORD_LENGTH_24       =                  4,
    CSL_MCBSP_WORD_LENGTH_32       =                  5
} CSL_McbspWordLength;

/* This enum should be used to Enable/Disable Companding */
typedef enum {
    CSL_MCBSP_NO_COMPAND_MSB_FIRST =                  0,
    CSL_MCBSP_NO_COMPAND_LSB_FIRST =                  1,
    CSL_MCBSP_MULAW_COMPAND        =                  2,
    CSL_MCBSP_ALAW_COMPAND         =                  3
} CSL_McbspCompand;

/* This enum should be used to set Transmitter & Receiver Data Delay Parameter */
typedef enum {
    CSL_MCBSP_BIT_DELAY_0          =                  0,
    CSL_MCBSP_BIT_DELAY_1          =                  1,
    CSL_MCBSP_BIT_DELAY_2          =                  2
} CSL_McbspDataDelay;

/* This enum should be used to Set Interrupt Event Type (i.e. Source of Interrupt Generation) */
typedef enum {
    CSL_MCBSP_INT_ON_READY         =                  0,
    CSL_MCBSP_INT_ON_EOB           =                  1,
    CSL_MCBSP_INT_ON_FSYNC         =                  2,
    CSL_MCBSP_INT_ON_SYNCERR       =                  3
} CSL_McbspIntEvent;

/* This enum should be used to set Clock Source as Internal/External */
typedef enum {
    CSL_MCBSP_CLK_MODE_EXTERNAL    =                  0,
    CSL_MCBSP_CLK_MODE_INTERNAL    =                  1
} CSL_McbspClkMode;

/* This enum should be used to set Clock Polarity */
typedef enum {
    CSL_MCBSP_POL_ACTIVE_HIGH      =                  0,
    CSL_MCBSP_POL_ACTIVE_LOW       =                  1
} CSL_McbspClkPolarity;

/* This enum should be used to Select Input Clock Source for Sample Rate Generator */
typedef enum {
    CSL_MCBSP_CLKS                 =                  0,
    CSL_MCBSP_CLKCPU               =                  1,
    CSL_MCBSP_CLKR                 =                  2,
    CSL_MCBSP_CLKX                 =                  3
} CSL_McbspSrgInputClk;

/* This enum should be used to set How to generate Internal Transmit Frame Sync */
typedef enum {
    CSL_MCBSP_SRG_FRMSYNC_DXRCOPY  =                  0,
    CSL_MCBSP_SRG_FRMSYNC_SRG      =                  1
} CSL_McbspSrgFrmSyncGen;

/* This enum should be used to Enable/Disable IO Mode for Transmitter and Receiver */
typedef enum {
    CSL_MCBSP_IO_TXDIS_RXDIS       =                  0,
    CSL_MCBSP_IO_TXDIS_RXEN        =                  1,
    CSL_MCBSP_IO_TXEN_RXDIS        =                  2,
    CSL_MCBSP_IO_TXEN_RXEN         =                  3
} CSL_McbspIOEnable;

/* This enum should be used to Enable/Disable Clock Stop Mode */
typedef enum {
    CSL_MCBSP_CLKSTP_DISABLE       =                  1,
    CSL_MCBSP_CLKSTP_WITHOUT_DELAY =                  2,
    CSL_MCBSP_CLKSTP_WITH_DELAY    =                  3
} CSL_McbspClkStop;

/* Select the partition type in multichannel selection mode */
typedef enum {
    CSL_MCBSP_PARTITION2           =                  0,
    CSL_MCBSP_PARTITION8           =                  1
} CSL_McbspPartitionType;

/* This enum should be used to assign Blocks to Partition-A */
typedef enum {
    CSL_MCBSP_PABLK_0              =                  0,
    CSL_MCBSP_PABLK_2              =                  1,
    CSL_MCBSP_PABLK_4              =                  2,
    CSL_MCBSP_PABLK_6              =                  3
} CSL_McbspPartABlk;

/* This enum should be used to assign Blocks to Partition-B */
typedef enum {
    CSL_MCBSP_PBBLK_1              =                  0,
    CSL_MCBSP_PBBLK_3              =                  1,
    CSL_MCBSP_PBBLK_5              =                  2,
    CSL_MCBSP_PBBLK_7              =                  3
} CSL_McbspPartBBlk;

/* This enum should be used for setting Emulation Mode Parameters */
typedef enum {
    CSL_MCBSP_EMU_STOP             =                  0,
    CSL_MCBSP_EMU_TX_STOP          =                  1,
    CSL_MCBSP_EMU_CONT             =                  2
} CSL_McbspEmulation;

/* Select the Partition for a assigning a block to */
typedef enum {
    CSL_MCBSP_PARTITION_ATX        =                  0,
    CSL_MCBSP_PARTITION_ARX        =                  1,
    CSL_MCBSP_PARTITION_BTX        =                  2,
    CSL_MCBSP_PARTITION_BRX        =                  3
} CSL_McbspPartition;

/* Select the block on which the operation is to be performed */
typedef enum {
    CSL_MCBSP_BLOCK0               =                  0,
    CSL_MCBSP_BLOCK1               =                  1,
    CSL_MCBSP_BLOCK2               =                  2,
    CSL_MCBSP_BLOCK3               =                  3,
    CSL_MCBSP_BLOCK4               =                  4,
    CSL_MCBSP_BLOCK5               =                  5,
    CSL_MCBSP_BLOCK6               =                  6,
    CSL_MCBSP_BLOCK7               =                  7
} CSL_McbspBlock;

/* Select the block on which the operation is to be performed */
typedef enum {
    CSL_MCBSP_CHAN_TX_ENABLE       =                  0,
    CSL_MCBSP_CHAN_TX_DISABLE      =                  1,
    CSL_MCBSP_CHAN_RX_ENABLE       =                  2,
    CSL_MCBSP_CHAN_RX_DISABLE      =                  3
} CSL_McbspChanOperation;

/* This enum is used to select the type of channel for Mcbsp */
typedef enum {
    CSL_MCBSP_RX                   =                  1,
    CSL_MCBSP_TX                   =                  2,
    CSL_MCBSP_TXRX                 =                  4
} CSL_McbspChanType;

/* This enum should be used for selecting the operation in HwControl function */
typedef enum {
    CSL_MCBSP_CMD_ASSIGN_BLOCK     =                  0,
    CSL_MCBSP_CMD_CHANNEL_CONTROL  =                  1,
    CSL_MCBSP_CMD_CLEAR_FRAMESYNC =                  2,
    CSL_MCBSP_CMD_IOMODE_CONTROL  =                  3,
    CSL_MCBSP_CMD_REG_RESET        =                  4,
    CSL_MCBSP_CMD_RESET_CONTROL    =                  5
} CSL_McbspControlCmd;

/* This enum should be used for selecting the get status param */
typedef enum {
    CSL_MCBSP_QUERY_GLOBAL_SETUP =                  0,
    CSL_MCBSP_QUERY_RX_DATA_SETUP =                  1,
    CSL_MCBSP_QUERY_TX_DATA_SETUP =                  2,
    CSL_MCBSP_QUERY_CLK_SETUP  =                  3,
    CSL_MCBSP_QUERY_MULTICHAN_SETUP =                  4,
    CSL_MCBSP_QUERY_EMULATION_MODE =                  5,
    CSL_MCBSP_QUERY_CUR_TX_BLK =                  6,
    CSL_MCBSP_QUERY_CUR_RX_BLK =                  7,
    CSL_MCBSP_QUERY_DEV_STATUS =                  8,
    CSL_MCBSP_QUERY_TX_RST_STAT =                  9,
    CSL_MCBSP_QUERY_RX_RST_STAT =                 10
} CSL_McbspHwStatusQuery;


/* This type is used for block assignment(CSL_mcbspHwControl) */
typedef struct CSL_McbspBlkAssign {
    CSL_McbspPartition      partition;
    CSL_McbspBlock          block;
} CSL_McbspBlkAssign;

/* This is used to for channel control operations(en/disable Rx/Tx) */
typedef struct CSL_McbspChanControl {
    Uint16              channelNo;
    CSL_McbspChanOperation  operation;
} CSL_McbspChanControl;

/* This type is used for IO control (selecting the io pins) */
typedef struct CSL_McbspIoControl {
    CSL_BitMask16           outputsel;
    CSL_BitMask16           inputsel;
} CSL_McbspIoControl;

/* Structure(Internal) used during resource allocation */
typedef struct CSL_McbspResAttrs {
    CSL_Xio                 xio;
    CSL_Uid                 uid;
    CSL_McbspRegsOvly       *regs;
    Int16               perNum;
} CSL_McbspResAttrs;

/* This is the Setup Structure for Configuring Receiver and Transmitter in Setup Function */
typedef struct CSL_McbspDataSetup {
    Uint16              phaseNum;
    CSL_McbspWordLength     wordLength1;
    Uint16              frmLength1;
    CSL_McbspWordLength     wordLength2;
    Uint16              frmLength2;
    Uint16              frmSyncIgn;
    CSL_McbspCompand        compand;
    CSL_McbspDataDelay      dataDelay;
    Uint16              rjust_dxenable;
    CSL_McbspIntEvent       IntEvent;
} CSL_McbspDataSetup;

/* This is the Setup Structure for Configuring Clock and Frame Sync Generation in Setup Function */
typedef struct CSL_McbspClkSetup {
    CSL_McbspClkMode        frmSyncTxMode;
    CSL_McbspClkMode        frmSyncRxMode;
    CSL_McbspClkMode        clkTxMode;
    CSL_McbspClkMode        clkRxMode;
    CSL_McbspClkPolarity    frmSyncTxPolarity;
    CSL_McbspClkPolarity    frmSyncRxPolarity;
    CSL_McbspClkPolarity    clkTxPolarity;
    CSL_McbspClkPolarity    clkRxPolarity;
    Uint16              srgFrmPulseWidth;
    Uint16              srgFrmPeriod;
    Uint16              srgClkDivide;
    CSL_McbspSrgInputClk    srgInputClkMode;
    CSL_McbspClkPolarity    srgClkPolarity;
    CSL_McbspSrgFrmSyncGen  srgTxFrmSyncMode;
    Uint16              srgClkSync;
} CSL_McbspClkSetup;

/* This is the Setup Structure for Configuring Global Behaviour in Setup Function */
typedef struct CSL_McbspGlobalSetup {
    CSL_McbspIOEnable       IOEnableMode;
    Uint16              dlbMode;
    CSL_McbspClkStop        clkStopMode;
} CSL_McbspGlobalSetup;

/* This is the Setup Structure for Configuring Multi Channel Mode in Setup Function */
typedef struct CSL_McbspMulChSetup {
    CSL_McbspPartitionType  rxPartition;
    CSL_McbspPartitionType  txPartition;
    Uint16              rxMulChSel;
    Uint16              txMulChSel;
    CSL_McbspPartABlk       rxPartABlk;
    CSL_McbspPartBBlk       rxPartBBlk;
    CSL_McbspPartABlk       txPartABlk;
    CSL_McbspPartBBlk       txPartBBlk;
} CSL_McbspMulChSetup;

/* This is the Setup Structure for Configuring MCBSP in Setup Function */
typedef struct CSL_McbspHwSetup {
    CSL_McbspGlobalSetup    *global;
    CSL_McbspDataSetup      *rxdataset;
    CSL_McbspDataSetup      *txdataset;
    CSL_McbspClkSetup       *clkset;
    CSL_McbspMulChSetup     *mulCh;
    CSL_McbspEmulation      emumode;
    void                    *extendSetup;
} CSL_McbspHwSetup;

/* MCBSP Object */
typedef struct CSL_McbspObj {
    CSL_OpenMode            openMode;   /* Always the first member of <PERI>Obj */
    CSL_Uid                 uid;        /* Always the second member of <PERI>Obj */
    CSL_Xio                 xio;        /* Always the third member of <PERI>Obj */
    CSL_McbspRegsOvly       regs;
    Int16               perNum;
} CSL_McbspObj;

typedef struct CSL_McbspObj *CSL_McbspHandle;

/**************************************************************************\
* MCBSP global function declarations
\**************************************************************************/
/*
 * ======================================================
 *   @func   csl_mcbspInit.c
 *   @desc   Intialization function
 *
 *
 *   @ret  CSL_Status
 *
 *
 *   @eg
 *
 * ======================================================
*/

CSL_Status  CSL_mcbspInit(
    void
);

/*
 * ======================================================
 *   @func   csl_mcbspOpen.c
 *   @desc
 *         The Function opens the MCBSP Obj, for the specified MCBSP NUMBER.
 *         If the hwsetup is not NULL, then the H/W registers are configured,
 *         with the values passed by the hwsetup parameter.
 *
 *   @arg  hMcbspObj
 *              Handle to the MCBSP Obj
 *   @arg  mcbspNum
 *              The Mcbsp to opened, CSL_MCBSP_ANY symbolizes for ANY of the McBSPs
 *   @arg  openMode
 *              Pin Mode: EXL or SHARED
 *   @arg  hwSetup
 *              H/W setup parameters, if NULL, h/w setup is ignored.
 *   @arg  status
 *              Return Status
 *   @ret  Handle
 *
 *          Valid Mcbsp handle will be returned
 *          if status value is equal to CSL_SOK.
 *          if status returned is CSL_EMCSP_NOTEXIST : mcbspNum doesnt correspond to an
 *                               available McBSP with the device.
 *          if status returned is CSL_ESYS_OVFL  : No McBSP is currently avaialable
 *
 *   @eg
 *     hMcbsp = CSL_mcbspOpen(&hMcbspObj, CSL_MCBSP_ANY, CSL_EXCLUSIVE, &hwSetup, &status);
 *
 *
 * ======================================================
*/

CSL_McbspHandle  CSL_mcbspOpen(
    CSL_McbspObj                            *hMcbspObj,
    CSL_McbspNum                            mcbspNum,
    CSL_OpenMode                            openMode,
    CSL_McbspHwSetup                        *hwSetup,
    CSL_Status                              *status
);

/*
 * ======================================================
 *   @func   csl_mcbspClose.c
 *   @desc   This function closes the MCBSP handle obtained using
 *                the open function call.
 *
 *
 *   @arg  hMcbsp
 *              Handle of the McBsp to close
 *   @ret  CSL_Status
 *
 *          returns CSL_SOK, if close funtion succeeds.
 *
 *
 *
 *   @eg
 *     status = CSL_mcbspClose(hMcbsp);
 *
 *
 * ======================================================
*/

CSL_Status  CSL_mcbspClose(
    CSL_McbspHandle                         hMcbsp
);

/*
 * ======================================================
 *   @func   csl_mcbspHwSetup.c
 *   @desc   Function to Setup MCBSP registers Initially
 *
 *
 *   @arg  hMcbsp
 *              Device Handle;
 *   @arg  setup
 *              Pointer to Initilaization structure
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful completion of the operation
 *          CSL_ESYS_INVPARAMS: HwSetup pointer is invalid
 *
 *
 *   @eg
 *     CSL_McbspHandle hMcbsp;
 *     hMcbsp = CSL_mcbspOpen(hMcbsp, CSL_MCBSP_0, CSL_EXCLUSIVE, &status, NULL);
 *     CSL_mcbspSetup( hMcbsp, &mysetup);
 *
 *
 * ======================================================
*/

CSL_Status  CSL_mcbspHwSetup(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspHwSetup                        *setup
);

/*
 * ======================================================
 *   @func   csl_mcbspRead.c
 *   @desc
 *         Function reads the data from the MCBSP peripheral specified by the Hanldle.
 *         Operation done is based on the the size of the data unit specified by eSize.
 *
 *
 *   @arg  hMcbsp
 *              Handle of McBSP to read from
 *   @arg  wordLen
 *              Size of the data element to be read
 *   @arg  data
 *              Container for the data read
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful, desired operation is done.
 *          CSL_EMCBSP_INVSIZE : word size, is not supported
 *
 *   @eg
 *     Uint16 data;
 *     status = CSL_mcbspRead(hMcbsp, 16, &data);
 *
 *
 * ======================================================
*/

CSL_Status  CSL_mcbspRead(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspWordLength                     wordLen,
    void                                    *data
);

/*
 * ======================================================
 *   @func   csl_mcbspWrite.c
 *   @desc
 *         Function writes the data to the MCBSP peripheral specified by the Hanldle.
 *         Operation done is based on the the size of the data unit specified by eSize.
 *
 *
 *   @arg  hMcbsp
 *              Handle of McBSP, to write to
 *   @arg  wordLen
 *              Size of the data element
 *   @arg  data
 *              Container of the data
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful, desired operation is done.
 *          CSL_EMCBSP_INVSIZE : word size, is not supported
 *
 *   @eg
 *     Uint16 data;
 *     status = CSL_mcbspWrite(hMcbsp, 16, &data);
 *
 *
 * ======================================================
*/

CSL_Status  CSL_mcbspWrite(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspWordLength                     wordLen,
    void                                    *data
);

/*
 * ======================================================
 *   @func   csl_mcbspIoWrite.c
 *   @desc   Send bit data to pins (I/O mode pins).
 *
 *
 *   @arg  hMcbsp
 *              Device Handle;
 *   @arg  outputSel
 *              Select pin to send data on. Bitwise OR of CSL_MCBSP_IO_XXXX
 *   @arg  outputData
 *              Set CSL_MCBSP_IO_XXXX to data(0 or 1) to be sent outThis value is bitwise OR of all CSL_MCBSP_IO_XXXX for which the data to be sent out is 1. The data will be shifted outonly on pin selected by outputSel and set to output.
 *   @ret  void
 *
 *
 *   @eg
 *     CSL_McbspHandle hMcbsp;
 *     hMcbsp = CSL_mcbspOpen(0);
 *     CSL_mcbspIoWrite( hMcbsp,  CSL_MCBSP_IO_CLKX | CSL_MCBSP_IO_CLKR,
 *                                         CSL_MCBSP_IO_CLX );
 *
 * ======================================================
*/

void  CSL_mcbspIoWrite(
    CSL_McbspHandle                         hMcbsp,
    CSL_BitMask16                           outputSel,
    Uint16                              outputData
);

/*
 * ======================================================
 *   @func   csl_mcbspIoRead.c
 *   @desc   Receive bit data from pins (I/O mode pins).
 *
 *
 *   @arg  hMcbsp
 *              Device Handle;
 *   @arg  inputSel
 *              Select pin to receive data from. Bitwise OR of CSL_MCBSP_IO_XXXX
 *   @ret  Uint16
 *
 *          This value is bitwise OR of all corresponding CSL_MCBSP_IO_XXXX for which
 *          the data received is 1. Data Received on all other Pins is 0. To get data for a
 *          particular pin, use !(!(return_value & CSL_MCBSP_IO_XXXX ))
 *
 *
 *   @eg
 *     MCBSP_Handle hMcbsp;
 *     hMcbsp = MCBSP_open(0);
 *     Uint16 data;
 *     data = CSL_mcbspIoRead( hMcbsp,  CSL_MCBSP_IO_CLKX | CSL_MCBSP_IO_CLKR );
 *
 *
 * ======================================================
*/

Uint16  CSL_mcbspIoRead(
    CSL_McbspHandle                         hMcbsp,
    CSL_BitMask16                           inputSel
);

/*
 * ======================================================
 *   @func   csl_mcbspHwControl.c
 *   @desc
 *         Function controls the MCBSP peripheral specified by the Hanldle.
 *         Operation done is based on the "cmd".
 *
 *
 *   @arg  hMcbsp
 *              Mcbsp Handle Object
 *   @arg  cmd
 *              Command that specifies the operation to be performed
 *   @arg  arg
 *              Contains the data passed to be function
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful, desired operation is done.
 *          CSL_ESYS_INVCNTLCMD : Control Command is not supported.
 *
 *
 *   @eg
 *     status = CSL_msbspHwControl(hMcbsp, CSL_MCBSP_ASSIGN_BLOCK, &block);
 *     This will Assign the block to the MCBSP represented by the HANDLE - hMcbsp.
 *
 *
 * ======================================================
*/

CSL_Status  CSL_mcbspHwControl(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspControlCmd                     cmd,
    void                                    *arg
);

/*
 * ======================================================
 *   @func   csl_mcbspGetHwStatus.c
 *   @desc
 *         Function query's the H/W configuration of the Mcbsp specified by the Handle,
 *         command specifies the type of data required
 *
 *
 *   @arg  hMcbsp
 *              MbBSP Handle Object
 *   @arg  myQuery
 *              The type of operation to be performed
 *   @arg  response
 *              The pointer that would hold the data to be returned.
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful Retreive, (* Ptr) will have the desired value.
 *          CSL_ESYS_INVQUERY : Query Command is not supported.
 *
 *   @eg
 *     status = CSL_mcbspGetHwStatus(hMcbsp, CSL_MCBSP_GET_GLOBAL_SETUP, &global);
 *
 *
 * ======================================================
*/

CSL_Status  CSL_mcbspGetHwStatus(
    CSL_McbspHandle                         hMcbsp,
    CSL_McbspHwStatusQuery                  myQuery,
    void                                    *response
);

/**************************************************************************\
* MCBSP inline function declarations
\**************************************************************************/
/**************************************************************************\
* MCBSP inline function definitions
\**************************************************************************/


#endif

