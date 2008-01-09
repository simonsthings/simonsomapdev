/*
 * Set up the DSP DMA to service McBSP1 and McBSP3
 *
 *
 */


#include <csl.h>
#include <csl_error.h>
#include <csl_intc.h>
#include <csl_chip.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <soc.h>

//#define N 0x20
#define N 256


/** Source location for DMA transfer */
extern int src_ping1[N*2];
extern int src_pong1[N*2];
/** Destination location for DMA transfer */
extern int dst_ping1[N*2];
extern int dst_pong1[N*2];

/** Source location for DMA transfer */
extern int src_ping3[N*2];
extern int src_pong3[N*2];
/** Destination location for DMA transfer */
extern int dst_ping3[N*2];
extern int dst_pong3[N*2];

int dma_setup (CSL_DmaHandle * hDma_xmit1, CSL_DmaHandle * hDma_rcv1, 
				CSL_DmaHandle * hDma_xmit3, CSL_DmaHandle * hDma_rcv3)
{
	/*	Setup for a DMA channel */
	CSL_DmaHwSetup hwSetup_xmit1, hwSetup_rcv1; 
	CSL_DmaHwSetup hwSetup_xmit3, hwSetup_rcv3;

	/* McBSP1 Transmit Section */
	//Setup for source port
	CSL_DmaHwSetupSrcPort srcObj_xmit1 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_DARAM, 
											CSL_DMA_AMODE_POSTINC, 
											0, 
											0, 
											0} ;
	//Setup for destination port
	CSL_DmaHwSetupDstPort dstObj_xmit1 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_TIPB, 
											CSL_DMA_AMODE_CONST, 
											0, 
											0, 
											0} ;
	//Setup for channel parameters
	CSL_DmaHwSetupChannel channelObj_xmit1 = {CSL_DMA_DATASIZE_16BIT, 
											(N*2)*2, 	// size = ping + pong
											1, 
											CSL_DMA_PRIORITY_HI, 
											CSL_DMA_SYNC_MCBSP1TX, 
											CSL_DMA_FRAMESYNC_DISABLE, 
											CSL_DMA_ONOFF_OFF, 	// end_prog
											CSL_DMA_ONOFF_ON, 	// repeat
											CSL_DMA_ONOFF_ON};	// auto-init
	//Setup for interrupts
	CSL_DmaHwSetupIntr intrObj_xmit1 = { 	CSL_DMA_ONOFF_ON, 	// end of block
											CSL_DMA_ONOFF_OFF, 	// last
											CSL_DMA_ONOFF_OFF, 	// frame
											CSL_DMA_ONOFF_ON, 	// half
											CSL_DMA_ONOFF_ON, 	// drop
											CSL_DMA_ONOFF_OFF};	// timeout



	/* McBSP1 Receive Section */
	//Setup for source port
	CSL_DmaHwSetupSrcPort srcObj_rcv1 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_TIPB, 
											CSL_DMA_AMODE_CONST, 
											0, 
											0, 
											0} ;
	//Setup for destination port
	CSL_DmaHwSetupDstPort dstObj_rcv1 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_DARAM, 
											CSL_DMA_AMODE_POSTINC, 
											0, 
											0, 
											0} ;
	//Setup for channel parameters
	CSL_DmaHwSetupChannel channelObj_rcv1 = {CSL_DMA_DATASIZE_16BIT, 
											(N*2)*2, 	// size = ping + pong
											1, 
											CSL_DMA_PRIORITY_HI, 
											CSL_DMA_SYNC_MCBSP1RX, 
											CSL_DMA_FRAMESYNC_DISABLE, 
											CSL_DMA_ONOFF_OFF, 	// end_prog
											CSL_DMA_ONOFF_ON, 	// repeat
											CSL_DMA_ONOFF_ON};	// auto-init
	//Setup for interrupts
	CSL_DmaHwSetupIntr intrObj_rcv1 = {		CSL_DMA_ONOFF_ON, 	// end of block
											CSL_DMA_ONOFF_OFF, 	// last
											CSL_DMA_ONOFF_OFF, 	// frame
											CSL_DMA_ONOFF_ON, 	// half 
											CSL_DMA_ONOFF_ON, 	// drop
											CSL_DMA_ONOFF_OFF};	// timeout


	/* McBSP3 Transmit Section */
	//Setup for source port
	CSL_DmaHwSetupSrcPort srcObj_xmit3 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_DARAM, 
											CSL_DMA_AMODE_POSTINC, 
											0, 
											0, 
											0} ;
	//Setup for destination port
	CSL_DmaHwSetupDstPort dstObj_xmit3 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_TIPB, 
											CSL_DMA_AMODE_CONST, 
											0, 
											0, 
											0} ;
	//Setup for channel parameters
	CSL_DmaHwSetupChannel channelObj_xmit3 = {CSL_DMA_DATASIZE_16BIT, 
											N*2*2, 	// size = ping + pong
											1, 
											CSL_DMA_PRIORITY_HI, 
											CSL_DMA_SYNC_MCBSP3TX, 
											CSL_DMA_FRAMESYNC_DISABLE, 
											CSL_DMA_ONOFF_OFF, 	// end_prog
											CSL_DMA_ONOFF_ON, 	// repeat
											CSL_DMA_ONOFF_ON};	// auto-init
	//Setup for interrupts
	CSL_DmaHwSetupIntr intrObj_xmit3 = { 	CSL_DMA_ONOFF_ON, 	// end of block
											CSL_DMA_ONOFF_OFF, 	// last
											CSL_DMA_ONOFF_OFF, 	// frame
											CSL_DMA_ONOFF_ON, 	// half
											CSL_DMA_ONOFF_ON, 	// drop
											CSL_DMA_ONOFF_OFF};	// timeout



	/* McBSP3 Receive Section */
	//Setup for source port
	CSL_DmaHwSetupSrcPort srcObj_rcv3 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_TIPB, 
											CSL_DMA_AMODE_CONST, 
											0, 
											0, 
											0} ;
	//Setup for destination port
	CSL_DmaHwSetupDstPort dstObj_rcv3 = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_DARAM, 
											CSL_DMA_AMODE_POSTINC, 
											0, 
											0, 
											0} ;
	//Setup for channel parameters
	CSL_DmaHwSetupChannel channelObj_rcv3 = {CSL_DMA_DATASIZE_16BIT, 
											N*2*2, 	// size = ping + pong
											1, 
											CSL_DMA_PRIORITY_HI, 
											CSL_DMA_SYNC_MCBSP3RX, 
											CSL_DMA_FRAMESYNC_DISABLE, 
											CSL_DMA_ONOFF_OFF, 	// end_prog
											CSL_DMA_ONOFF_ON, 	// repeat
											CSL_DMA_ONOFF_ON};	// auto-init
	//Setup for interrupts
	CSL_DmaHwSetupIntr intrObj_rcv3 = {		CSL_DMA_ONOFF_ON, 	// end of block
											CSL_DMA_ONOFF_OFF, 	// last
											CSL_DMA_ONOFF_OFF, 	// frame
											CSL_DMA_ONOFF_ON, 	// half 
											CSL_DMA_ONOFF_ON, 	// drop
											CSL_DMA_ONOFF_OFF};	// timeout


	/* DMA Initialization */
	CSL_dmaInit(NULL);

    
	/* Construct DMA setup object */
	/* McBSP 1 */
	/* Transmit Section */
	//Source address - location from which data is to be transferred
	srcObj_xmit1.staddr = (Uint32)src_ping1;	// start with ping
	//Destination address - location to which data is to be transferred
	dstObj_xmit1.staddr = (Uint32)0x8c03 ;  // DXR1

	/* McBSP 1 */
	/* Receive Section */
	//Source address - location from which data is to be transferred
	srcObj_rcv1.staddr = (Uint32)0x8C01 ; // DRR1
	//Destination address - location to which data is to be transferred
	dstObj_rcv1.staddr = (Uint32)dst_ping1;  // start with ping
	

	/* McBSP 3 */
	/* Transmit Section */
	//Source address - location from which data is to be transferred
	srcObj_xmit3.staddr = (Uint32)src_ping3;	// start with ping
	//Destination address - location to which data is to be transferred
	dstObj_xmit3.staddr = (Uint32)0xB803 ;  // DXR1

	/* McBSP 3 */
	/* Receive Section */
	//Source address - location from which data is to be transferred
	srcObj_rcv3.staddr = (Uint32)0xB801 ; // DRR1
	//Destination address - location to which data is to be transferred
	dstObj_rcv3.staddr = (Uint32)dst_ping3;  // start with ping

	
	/* McBSP 1 */
	//Construct complete setup object for a channel
	hwSetup_xmit1.srcStruct 		= &srcObj_xmit1; 
	hwSetup_xmit1.dstStruct 		= &dstObj_xmit1;
	hwSetup_xmit1.channelStruct	= &channelObj_xmit1; 
	hwSetup_xmit1.intrStruct 	= &intrObj_xmit1 ;       
	hwSetup_xmit1.globalStruct	= NULL; //Global setup is default(reset values).

	hwSetup_rcv1.srcStruct 		= &srcObj_rcv1; 
	hwSetup_rcv1.dstStruct 		= &dstObj_rcv1;
	hwSetup_rcv1.channelStruct	= &channelObj_rcv1; 
	hwSetup_rcv1.intrStruct 		= &intrObj_rcv1 ;       
	hwSetup_rcv1.globalStruct	= NULL; //Global setup is default(reset values).
	
	/* McBSP3 */
	//Construct complete setup object for a channel
	hwSetup_xmit3.srcStruct 		= &srcObj_xmit3; 
	hwSetup_xmit3.dstStruct 		= &dstObj_xmit3;
	hwSetup_xmit3.channelStruct	= &channelObj_xmit3; 
	hwSetup_xmit3.intrStruct 	= &intrObj_xmit3 ;       
	hwSetup_xmit3.globalStruct	= NULL; //Global setup is default(reset values).

	hwSetup_rcv3.srcStruct 		= &srcObj_rcv3; 
	hwSetup_rcv3.dstStruct 		= &dstObj_rcv3;
	hwSetup_rcv3.channelStruct	= &channelObj_rcv3; 
	hwSetup_rcv3.intrStruct 		= &intrObj_rcv3 ;       
	hwSetup_rcv3.globalStruct	= NULL; //Global setup is default(reset values).

		if(hDma_xmit1 != NULL)
		{
			//Setup the channel
			CSL_dmaHwSetup((CSL_DmaHandle)*hDma_xmit1, &hwSetup_xmit1);
		}
		else {
			return 0;
		}
		if (hDma_rcv1 != NULL)
		{
			//Setup the channel
			CSL_dmaHwSetup((CSL_DmaHandle)*hDma_rcv1, &hwSetup_rcv1);
		}
		else {
			return 0;
		}

		if(hDma_xmit3 != NULL)
		{
			//Setup the channel
			CSL_dmaHwSetup((CSL_DmaHandle)*hDma_xmit3, &hwSetup_xmit3);
		}
		else {
			return 0;
		}
		if (hDma_rcv3 != NULL)
		{
			//Setup the channel
			CSL_dmaHwSetup((CSL_DmaHandle)*hDma_rcv3, &hwSetup_rcv3);
		}
		else {
			return 0;
		}

	return 1;
}
