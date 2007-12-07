/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== innov5910_dma_aic23_cslv3.c ========
 *
 *  DMA interrupt-driven low-level streaming device driver for TI
 *  OMAP 5910 Innovator. Uses Chip Support Library Version 3.x.
 *
 *  DSP/BIOS configuration:
 *    DMA channel 4 RX ISR plugged to INNOV5910_DMA_AIC23_CSLV3_isr with arg = 0
 *    DMA channel 5 TX ISR plugged to INNOV5910_DMA_AIC23_CSLV3_isr with arg = 1
 */

#include <std.h>

#include <stdplus.h>

#include <csl.h>
#include <csl_dma.h>
#include <csl_mcbsp.h>

#include <iom.h>

#include <c55xx_dma_mcbsp_cslv3.h>
#include <innov5910_dma_aic23_cslv3.h>

/*
 * Forward declaration of IOM interface functions.
 */
static Int mdBindDev(Ptr *hDevice, Int devid, Ptr devParams);
static Int mdCreateChan(Ptr *chanp, Ptr devp, String name, Int mode,
        Ptr chanParams, IOM_TiomCallback cbFxn, Ptr cbArg);

/*
 * Public IOM interface table.
 */
IOM_Fxns INNOV5910_DMA_AIC23_CSLV3_FXNS;

/* MCBSP Config */
static const CSL_McbspDataSetup mcbspRxSetup = {
    0,                             // (Uint16)              phaseNum
    CSL_MCBSP_WORD_LENGTH_16,      // (CSL_McbspWordLength) wordLength1
    2,                             // (Uint16)              frmLength1
    CSL_MCBSP_WORD_LENGTH_8,       // (CSL_McbspWordLength) wordLength2
    1,                             // (Uint16)              frmLength2
    1,                             // (Uint16)              frmSyncIgn
    CSL_MCBSP_NO_COMPAND_MSB_FIRST,// (CSL_McbspCompand)    compand
    CSL_MCBSP_BIT_DELAY_0,         // (CSL_McbspDataDelay)  dataDelay
    0,                             // (Uint16)              rjust_dxenable
    CSL_MCBSP_INT_ON_READY,        // (CSL_McbspIntEvent)   IntEvent
};

static const CSL_McbspDataSetup mcbspTxSetup = {
    0,                             // (Uint16)              phaseNum
    CSL_MCBSP_WORD_LENGTH_16,      // (CSL_McbspWordLength) wordLength1
    2,                             // (Uint16)              frmLength1
    CSL_MCBSP_WORD_LENGTH_8,       // (CSL_McbspWordLength) wordLength2
    1,                             // (Uint16)              frmLength2
    1,                             // (Uint16)              frmSyncIgn
    CSL_MCBSP_NO_COMPAND_MSB_FIRST,// (CSL_McbspCompand)    compand
    CSL_MCBSP_BIT_DELAY_0,         // (CSL_McbspDataDelay)  dataDelay
    0,                             // (Uint16)              rjust_dxenable
    CSL_MCBSP_INT_ON_READY,        // (CSL_McbspIntEvent)   IntEvent
};

static const CSL_McbspClkSetup mcbspClkSetup = {
    CSL_MCBSP_CLK_MODE_EXTERNAL,   // (CSL_McbspClkMode)     frmSyncTxMode
    CSL_MCBSP_CLK_MODE_EXTERNAL,   // (CSL_McbspClkMode)     frmSyncRxMode
    CSL_MCBSP_CLK_MODE_EXTERNAL,   // (CSL_McbspClkMode)     clkTxMode
    CSL_MCBSP_CLK_MODE_EXTERNAL,   // (CSL_McbspClkMode)     clkRxMode
    CSL_MCBSP_POL_ACTIVE_HIGH,     // (CSL_McbspClkPolarity) frmSyncTxPolarity
    CSL_MCBSP_POL_ACTIVE_HIGH,     // (CSL_McbspClkPolarity) frmSyncRxPolarity
    CSL_MCBSP_POL_ACTIVE_LOW,      // (CSL_McbspClkPolarity) clkTxPolarity
    CSL_MCBSP_POL_ACTIVE_LOW,      // (CSL_McbspClkPolarity) clkRxPolarity
    0x0,                           // (Uint16)               srgFrmPulseWidth
    0x0,                           // (Uint16)               srgFrmPeriod
    0x0,                           // (Uint16)               srgClkDivide
    CSL_MCBSP_CLKS,                // (CSL_McbspSrgInputClk) srgInputClkMode
    CSL_MCBSP_POL_ACTIVE_HIGH,     // (CSL_McbspClkPolarity) srgClkPolarity
    CSL_MCBSP_SRG_FRMSYNC_DXRCOPY, // (CSL_McbspSrgFrmSyncGen) srgTxFrmSyncMode
    0,                             // (Uint16)               srgClkSync
};

static const CSL_McbspGlobalSetup mcbspGlobalSetup = {
    CSL_MCBSP_IO_TXDIS_RXDIS,      // (CSL_McbspIOEnable) IOEnableMode
    0,                             // (Uint16)            dlbMode
    CSL_MCBSP_CLKSTP_DISABLE,      // (CSL_McbspClkStop)  clkStopMode
};


static const CSL_McbspMulChSetup mcbspMulChSetup = {
    CSL_MCBSP_PARTITION2,          // (CSL_McbspPartitionType) rxPartition
    CSL_MCBSP_PARTITION2,          // (CSL_McbspPartitionType) txPartition
    0,                             // (Uint16)                 rxMulChSel
    0,                             // (Uint16)                 txMulChSel
    CSL_MCBSP_PABLK_0,             // (CSL_McbspPartABlk)      rxPartABlk
    CSL_MCBSP_PBBLK_1,             // (CSL_McbspPartBBlk)      rxPartBBlk
    CSL_MCBSP_PABLK_0,             // (CSL_McbspPartABlk)      txPartABlk
    CSL_MCBSP_PBBLK_1,             // (CSL_McbspPartBBlk)      txPartBBlk
};

static const CSL_McbspHwSetup mcbspCfg1 = {
    (CSL_McbspGlobalSetup *)&mcbspGlobalSetup,             // global
    (CSL_McbspDataSetup *)  &mcbspRxSetup,                 // rxdataset
    (CSL_McbspDataSetup *)  &mcbspTxSetup,                 // txdataset
    (CSL_McbspClkSetup *)   &mcbspClkSetup,                // clkset
    (CSL_McbspMulChSetup *) &mcbspMulChSetup,              // mulCh
    (CSL_McbspEmulation)    CSL_MCBSP_EMU_STOP,            // emumode
    (void *)                NULL,                          // extendSetup
};

/* DMA - RX */
static CSL_DmaHwSetupDstPort dmaRxDstSetup = {
    CSL_DMA_BURSTEN_DISABLE,       // (CSL_DmaBurstEn) burstEn
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)   packing
    CSL_DMA_PORT_DARAM,            // (CSL_DmaPort)    port
    CSL_DMA_AMODE_POSTINC,         // (CSL_DmaAmode)   amode
    0x00000000,                    // (Uint32)         staddr
    0x0,                           // (Int16)          frmIndex
    0x0,                           // (Int16)          elmIndex
};

static const CSL_DmaHwSetupSrcPort dmaRxSrcSetup = {
    CSL_DMA_BURSTEN_DISABLE,       // (CSL_DmaBurstEn) burstEn
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)   packing
    CSL_DMA_PORT_RHEA,             // (CSL_DmaPort)    port
    CSL_DMA_AMODE_CONST,           // (CSL_DmaAmode)   amode
    ( ((Uint32) &(CSL_MCBSP_0_REGS->DRR1)) ), // (Uint32) staddr
    0x0,                           // (Int16)          frmIndex
    0x0,                           // (Int16)          elmIndex
};

static const CSL_DmaHwSetupChannel dmaRxChannelSetup = {
    CSL_DMA_DATASIZE_16BIT,        // (CSL_DmaDataSize)  datatype
    0,                             // (Uint16)           elmNum
    1,                             // (Uint16)           frmNum
    CSL_DMA_PRIORITY_HI,           // (CSL_DmaPriority)  prio
    CSL_DMA_SYNC_MCBSP1RX,         // (CSL_DmaSync)      sync
    CSL_DMA_FRAMESYNC_DISABLE,     // (CSL_DmaFrameSync) fs
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)     endprog
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)     repeat
    CSL_DMA_ONOFF_ON,              // (CSL_DmaOnOff)     autoinit
};

static const CSL_DmaHwSetupIntr dmaRxIntrSetup = {
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) blockie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) lastie
    CSL_DMA_ONOFF_ON,              // (CSL_DmaOnOff) frameie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) firsthalfie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) dropie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) timeoutie
};

/* DMA global structure used for both DMA channels */
static const CSL_DmaHwSetupGlobal dmaGlobalSetup = {
    CSL_DMA_ONOFF_ON,              // (CSL_DmaOnOff)     autogating
    CSL_FALSE,                     // (CSL_Bool)         free
    CSL_FALSE,                     // (CSL_Bool)         apiexcl
    CSL_DMA_PRIORITY_LO,           // (CSL_DmaPriority)  apiprio
    CSL_FALSE,                     // (CSL_Bool)         stce
    CSL_FALSE,                     // (CSL_Bool)         dtce
    CSL_DMA_INDEXMODE_SAME,        // (CSL_DmaIndexMode) indexMode
};


static const CSL_DmaHwSetup dmaRxCfg = {
    (CSL_DmaHwSetupSrcPort *) &dmaRxSrcSetup,               // srcStruct
    (CSL_DmaHwSetupDstPort *) &dmaRxDstSetup,               // dstStruct
    (CSL_DmaHwSetupChannel *) &dmaRxChannelSetup,           // channelStruct
    (CSL_DmaHwSetupIntr *)    &dmaRxIntrSetup,              // intrStruct
    (CSL_DmaHwSetupGlobal *)  &dmaGlobalSetup,              // globalStruct
};

/* DMA - TX */
static const CSL_DmaHwSetupDstPort dmaTxDstSetup = {
    CSL_DMA_BURSTEN_DISABLE,       // (CSL_DmaBurstEn) burstEn
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)   packing
    CSL_DMA_PORT_RHEA,             // (CSL_DmaPort)    port
    CSL_DMA_AMODE_CONST,           // (CSL_DmaAmode)   amode
    ( ((Uint32) &(CSL_MCBSP_0_REGS->DXR1)) ), // (Uint32) staddr
    0x0,                           // (Int16)          frmIndex
    0x0,                           // (Int16)          elmIndex
};

static CSL_DmaHwSetupSrcPort dmaTxSrcSetup = {
    CSL_DMA_BURSTEN_DISABLE,       // (CSL_DmaBurstEn) burstEn
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)   packing
    CSL_DMA_PORT_DARAM,            // (CSL_DmaPort)    port
    CSL_DMA_AMODE_POSTINC,         // (CSL_DmaAmode)   amode
    0x00000000,                    // (Uint32)         staddr
    0x0,                           // (Int16)          frmIndex
    0x0,                           // (Int16)          elmIndex
};

static const CSL_DmaHwSetupChannel dmaTxChannelSetup = {
    CSL_DMA_DATASIZE_16BIT,        // (CSL_DmaDataSize)  datatype
    0,                             // (Uint16)           elmNum
    1,                             // (Uint16)           frmNum
    CSL_DMA_PRIORITY_HI,           // (CSL_DmaPriority)  prio
    CSL_DMA_SYNC_MCBSP1TX,         // (CSL_DmaSync)      sync
    CSL_DMA_FRAMESYNC_DISABLE,     // (CSL_DmaFrameSync) fs
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)     endprog
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff)     repeat
    CSL_DMA_ONOFF_ON,              // (CSL_DmaOnOff)     autoinit
};

static const CSL_DmaHwSetupIntr dmaTxIntrSetup = {
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) blockie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) lastie
    CSL_DMA_ONOFF_ON,              // (CSL_DmaOnOff) frameie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) firsthalfie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) dropie
    CSL_DMA_ONOFF_OFF,             // (CSL_DmaOnOff) timeoutie
};

static const CSL_DmaHwSetup dmaTxCfg = {
    (CSL_DmaHwSetupSrcPort *) &dmaTxSrcSetup,              // srcStruct
    (CSL_DmaHwSetupDstPort *) &dmaTxDstSetup,              // dstStruct
    (CSL_DmaHwSetupChannel *) &dmaTxChannelSetup,          // channelStruct
    (CSL_DmaHwSetupIntr *)    &dmaTxIntrSetup,             // intrStruct
    (CSL_DmaHwSetupGlobal *)  &dmaGlobalSetup,             // globalStruct
};

/*
 * These arrays are used to initialized csdp value for different DMA port
 * type. This is done by mdBindDev using params->dmaPortType
 */
static const Uns portValue[3] = {
    CSL_DMA_PORT_DARAM,         /* INNOV5910_DMA_AIC23_CSLV3_PORTTYPE_DARAM */
    CSL_DMA_PORT_SARAM,         /* INNOV5910_DMA_AIC23_CSLV3_PORTTYPE_SARAM */
    CSL_DMA_PORT_EMIF           /* INNOV5910_DMA_AIC23_CSLV3_PORTTYPE_EMIF */
};

/* Delcare a McBsp Object to pass to DMA mover */
static CSL_McbspObj     mcbspObj1;

/* Declare 2 DMA object, for Input and Output channels */
static CSL_DmaObj       dmaRxObj;
static CSL_DmaObj       dmaTxObj;

/*
 *  ======== mdBindDev ========
 */
#pragma CODE_SECTION(mdBindDev, ".text:init")
static Int mdBindDev(Ptr *devp, Int devid, Ptr devParams)
{
    INNOV5910_DMA_AIC23_CSLV3_DevParams *params =
        (INNOV5910_DMA_AIC23_CSLV3_DevParams *)devParams;
    C55XX_DMA_MCBSP_CSLV3_DevParams genericDevParams;
    INNOV5910_DMA_AIC23_CSLV3_DevParams defaultParams =
                        INNOV5910_DMA_AIC23_CSLV3_DEVPARAMS_DEFAULT;

    /* use default parameters if none are given */
    if (params == NULL) {
        params = &defaultParams;
    }

    /* Check the version number */
    if (params->versionId != INNOV5910_DMA_AIC23_CSLV3_VERSION_2){
        /* Unsupported version */
        return(IOM_EBADARGS);
    }
    
    /* set dmacsdp register to the right dma port type */
    dmaRxDstSetup.port = (CSL_DmaPort) portValue[params->dmaPortType];
    dmaTxSrcSetup.port = (CSL_DmaPort) portValue[params->dmaPortType];

    genericDevParams.versionId = C55XX_DMA_MCBSP_CSLV3_VERSION_2;
    genericDevParams.rxDmaId = params->rxDmaId;
    genericDevParams.txDmaId = params->txDmaId;
    genericDevParams.mcbspObj = &mcbspObj1;
    genericDevParams.mcbspCfg = (CSL_McbspHwSetup *)&mcbspCfg1;
    genericDevParams.rxIerMask[0] = params->rxIerMask[0];
    genericDevParams.rxIerMask[1] = params->rxIerMask[1];
    genericDevParams.txIerMask[0] = params->txIerMask[0];
    genericDevParams.txIerMask[1] = params->txIerMask[1];

    return (C55XX_DMA_MCBSP_CSLV3_FXNS.mdBindDev(devp, CSL_MCBSP_1,
            &genericDevParams));
}

/*
 *  ======== mdCreateChan ========
 */
static Int mdCreateChan(Ptr *chanp, Ptr devp, String name, Int mode,
                Ptr chanParams, IOM_TiomCallback cbFxn, Ptr cbArg)
{
    C55XX_DMA_MCBSP_CSLV3_ChanParams genericChanParams;

    if (mode == IOM_INPUT) {
        genericChanParams.dmaObj = &dmaRxObj;
        genericChanParams.dmaCfg = (CSL_DmaHwSetup *)&dmaRxCfg;
    }
    else if (mode == IOM_OUTPUT) {
        genericChanParams.dmaObj = &dmaTxObj;
        genericChanParams.dmaCfg = (CSL_DmaHwSetup *)&dmaTxCfg;
    }
    else {
        return (IOM_EBADMODE);
    }

    return (C55XX_DMA_MCBSP_CSLV3_FXNS.mdCreateChan(chanp, devp, name, mode,
        &genericChanParams, cbFxn, cbArg));
}

/*
 *  ======== INNOV5910_DMA_AIC23_CSLV3_init ========
 *
 *  Controller initialization function
 */
#pragma CODE_SECTION(INNOV5910_DMA_AIC23_CSLV3_init, ".text:init")
Void INNOV5910_DMA_AIC23_CSLV3_init(Void)
{
    /*
     * Use C55XX_DMA_MCBSP_CSLV3_FXNS functions for the heart of the
     * controller.  This is common DMA/MCBSP code that works for
     * many DMA/MCBSP/codec combinations.
     */
    C55XX_DMA_MCBSP_CSLV3_init();

    INNOV5910_DMA_AIC23_CSLV3_FXNS = C55XX_DMA_MCBSP_CSLV3_FXNS;
    INNOV5910_DMA_AIC23_CSLV3_FXNS.mdBindDev = mdBindDev;
    INNOV5910_DMA_AIC23_CSLV3_FXNS.mdCreateChan = mdCreateChan;
}


