#include <csl_dma.h>

#define N 256

extern Uint32 srcBufPing[];
extern Uint32 dstBufPing[];

/** This function setups the ARM DMA(System DMA)
 *
 *  The function does the following steps :-
 *  -# Initializes the parameters of the @a #dmaHwSetup structure
 *     with the following setttings :-
 *     -# OMAP32 compatibilty mode
 *     -# Block interrupts for the channel is enabled
 */
int sdmaSetup(CSL_DmaHandle sdmaHndlXmit, CSL_DmaHandle sdmaHndlRcv)
{
   Uint32 *srcBufptr=srcBufPing;
   Uint32 *dstBufptr=dstBufPing;

   /* Transmit Channel */
   CSL_DmaSetupGlobal dmaSetupGlobalXmit=CSL_DMA_SETUPGLOBAL_DEFAULTS;
   CSL_DmaSetupChannel dmaSetupChannelXmit=CSL_DMA_SETUPCHANNEL_DEFAULTS;
   		CSL_DmaSetupChaCtrl dmaSetupChaCtrlXmit=CSL_DMA_SETUPCHACTRL_DEFAULTS;
   CSL_DmaSetupSrcPort dmaSetupSrcPortXmit=CSL_DMA_SETUPSRCPORT_DEFAULTS;
   		CSL_DmaSetupPortCtrl dmaSetupSrcPortCtrlXmit=CSL_DMA_SETUPPORTCTRL_DEFAULTS;
   CSL_DmaSetupDstPort dmaSetupDstPortXmit=CSL_DMA_SETUPDSTPORT_DEFAULTS;
		CSL_DmaSetupPortCtrl dmaSetupDstPortCtrlXmit=CSL_DMA_SETUPPORTCTRL_DEFAULTS;
   CSL_DmaHwSetup dmaHwSetupXmit={NULL,NULL,NULL,NULL};

   /* Receive Channel */
   CSL_DmaSetupGlobal dmaSetupGlobalRcv=CSL_DMA_SETUPGLOBAL_DEFAULTS;
   CSL_DmaSetupChannel dmaSetupChannelRcv=CSL_DMA_SETUPCHANNEL_DEFAULTS;
   		CSL_DmaSetupChaCtrl dmaSetupChaCtrlRcv=CSL_DMA_SETUPCHACTRL_DEFAULTS;
   CSL_DmaSetupSrcPort dmaSetupSrcPortRcv=CSL_DMA_SETUPSRCPORT_DEFAULTS;
   		CSL_DmaSetupPortCtrl dmaSetupSrcPortCtrlRcv=CSL_DMA_SETUPPORTCTRL_DEFAULTS;
   CSL_DmaSetupDstPort dmaSetupDstPortRcv=CSL_DMA_SETUPDSTPORT_DEFAULTS;
		CSL_DmaSetupPortCtrl dmaSetupDstPortCtrlRcv=CSL_DMA_SETUPPORTCTRL_DEFAULTS;
   CSL_DmaHwSetup dmaHwSetupRcv={NULL,NULL,NULL,NULL};

	/* DMA Initialization */
	CSL_dmaInit(NULL);

   /* Global DMA Setup */
   /* Transmit */
   dmaSetupGlobalXmit.omapCompatMode = CSL_DMA_OMAP32_COMPAT_MODE;
   /* Receive */
   dmaSetupGlobalRcv.omapCompatMode = CSL_DMA_OMAP32_COMPAT_MODE;

   /* Source Port Setup */
   /* Transmit */
   dmaSetupSrcPortXmit.portCtrlSetup = &dmaSetupSrcPortCtrlXmit; // set to default, modify some below
	   dmaSetupSrcPortXmit.portCtrlSetup->port = CSL_DMA_PORT_L3_OCP_T1;
	   dmaSetupSrcPortXmit.amode = CSL_DMA_AMODE_POST_INCREMENT;
	   dmaSetupSrcPortXmit.startAddr = (Uint32)srcBufptr;
 /* Receive */
   dmaSetupSrcPortRcv.portCtrlSetup = &dmaSetupSrcPortCtrlRcv; // set to default, modify some below
	   dmaSetupSrcPortRcv.portCtrlSetup->port = CSL_DMA_PORT_TIPB;
	   dmaSetupSrcPortRcv.amode = CSL_DMA_AMODE_CONSTANT;
	   dmaSetupSrcPortRcv.startAddr = (Uint32)0xFFFB1002; // DRR1
	   
   /* Destination Port Setup */
   /* Transmit */
   dmaSetupDstPortXmit.portCtrlSetup = &dmaSetupDstPortCtrlXmit;
	   dmaSetupDstPortXmit.portCtrlSetup->port = CSL_DMA_PORT_TIPB;   
	   dmaSetupDstPortXmit.amode = CSL_DMA_AMODE_CONSTANT;
	   dmaSetupDstPortXmit.startAddr = (Uint32)0xFFFB1006; // DXR1

 /* Receive */
   dmaSetupDstPortRcv.portCtrlSetup = &dmaSetupDstPortCtrlRcv;
	   dmaSetupDstPortRcv.portCtrlSetup->port = CSL_DMA_PORT_L3_OCP_T1;
	   dmaSetupDstPortRcv.amode = CSL_DMA_AMODE_POST_INCREMENT;
	   dmaSetupDstPortRcv.startAddr = (Uint32)dstBufptr;

   /* Channel Setup */
   /* Transmit */
   dmaSetupChannelXmit.datatype=CSL_DMA_DATASIZE_16BIT;
   dmaSetupChannelXmit.interruptMask=	CSL_DMA_HALF_FRAME_EVENT | 
   										CSL_DMA_END_BLOCK_EVENT | 
   										CSL_DMA_DROP_EVENT;
   dmaSetupChannelXmit.elementCount=(N*2)*2; // size = ping + pong
   dmaSetupChannelXmit.frameCount=1;
   dmaSetupChannelXmit.chaCtrlSetup=&dmaSetupChaCtrlXmit; // set to default, modify some below
	   dmaSetupChannelXmit.chaCtrlSetup->sync = CSL_DMA_MCBSP2_TX;
	   dmaSetupChannelXmit.chaCtrlSetup->autoinit = CSL_DMA_AUTOINIT_ENABLE;
	   dmaSetupChannelXmit.chaCtrlSetup->endprog = CSL_DMA_ENDPROG_DISABLE; 
	   dmaSetupChannelXmit.chaCtrlSetup->repeat = CSL_DMA_REPEAT_ENABLE; 

   /* Receive */
   dmaSetupChannelRcv.datatype=CSL_DMA_DATASIZE_16BIT;
   dmaSetupChannelRcv.interruptMask=	CSL_DMA_HALF_FRAME_EVENT |
   										CSL_DMA_END_BLOCK_EVENT |
   										CSL_DMA_DROP_EVENT;
   dmaSetupChannelRcv.elementCount=(N*2)*2;	// size = ping + pong
   dmaSetupChannelRcv.frameCount=1;
   dmaSetupChannelRcv.chaCtrlSetup=&dmaSetupChaCtrlRcv; // set to default, modify some below
	   dmaSetupChannelRcv.chaCtrlSetup->sync = CSL_DMA_MCBSP2_RX;
	   dmaSetupChannelRcv.chaCtrlSetup->autoinit = CSL_DMA_AUTOINIT_ENABLE;
	   dmaSetupChannelRcv.chaCtrlSetup->endprog = CSL_DMA_ENDPROG_DISABLE; 
	   dmaSetupChannelRcv.chaCtrlSetup->repeat = CSL_DMA_REPEAT_ENABLE; 

   /* DMA HW Setup */
   /* Transmit */
   dmaHwSetupXmit.channelSetup=&dmaSetupChannelXmit;
   dmaHwSetupXmit.srcPortSetup=&dmaSetupSrcPortXmit;
   dmaHwSetupXmit.dstPortSetup=&dmaSetupDstPortXmit;
   dmaHwSetupXmit.globalSetup=&dmaSetupGlobalXmit;
   /* Receive */
   dmaHwSetupRcv.channelSetup=&dmaSetupChannelRcv;
   dmaHwSetupRcv.srcPortSetup=&dmaSetupSrcPortRcv;
   dmaHwSetupRcv.dstPortSetup=&dmaSetupDstPortRcv;
   dmaHwSetupRcv.globalSetup=&dmaSetupGlobalRcv;

   CSL_dmaHwSetup(sdmaHndlXmit,&dmaHwSetupXmit);
   CSL_dmaHwSetup(sdmaHndlRcv,&dmaHwSetupRcv);


   return 1;
}
