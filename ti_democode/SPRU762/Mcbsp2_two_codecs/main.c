/*
 * Copyright (C) 2005 Texas Instruments Incorporated
 * All Rights Reserved
 */
/** McBSP2 interfaced to two codecs
*
* McBSP2 is a master for transmission and a slave for reception.  
* The implementation has a software check
* routine in the ISRs, which should be replaced by the actual 
* application software.
*
* N represents the number of audio samples per buffer.  The
* array size is twice the number of samples to account for left
* and right channels for a given audio sample.
*
* Each channel is 16-bits, and each frame is 4 channels long to accomodate
* the two stereo codecs.
*
* Once the McBSP transmitter and receiver are taken out of reset,
* the System DMA services the event requests using the following buffering
* scheme.
*
* The implementation uses a ping-pong buffering scheme that 
* takes advantage of the half-frame and end of block DMA interrupts.
* For this implementation, the DMA autoinitializes after a
* block as been transferred.  Ping and pong are placed in 
* consecutive locations in memory so that the DMA can be programmed
* to transfer data for a total of ping + pong samples, hence
* the half-frame interrupt indicating that ping has been transferred
* and the end of block interrupt to indicate that pong has 
* been transferred:
* 
* Memory contents:		DMA interrupt
* src_ping[1]
* src_ping[2]
* src_ping[3]
* ...
* src_ping[N*2-2]
* src_ping[N*2-1]		half-frame
* src_pong[1]
* src_pong[2]
* ...
* src_pong[N*2-2]
* src_pong[N*2-1]		end of block
* 
* This example runs with CSL 3.0.
*/

 
#include <csl.h>
#include <csl_error.h>
#include <csl_intc.h>
#include <csl_chip.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <csl_ssw.h>
#include <soc.h>

#define N 128
#define LOOPBACK 1		// audio loopback enabled.  User can set digital loopback mode
						// to test software as well, but needs to modify McBSP DLB setting
						// since it's default to OFF (CSL_MCBSP_DLBMODE_OFF)

/*-------Macro that gives 2 CLK delay cycles---------*/
/* The delay depends on the DSP frequency and on the CLKX/R rate */
#define WAIT_FOR_2_CLK  do {                                   \
                               volatile int delayCnt = 20;     \
                               while(delayCnt > 0) --delayCnt; \
                           } while (0)
                           
                           

/*---------Global data definition---------*/
CSL_McbspGlobalSetup gblCfg = {
    CSL_MCBSP_IOMODE_TXDIS_RXDIS,		// Set pins to work as McBSP pins
    CSL_MCBSP_DLBMODE_OFF,				// Disable digital loopback mode
    CSL_MCBSP_CLKSTP_DISABLE			// Disable clock stop mode
};

CSL_McbspDataSetup rcvDataCfg = {
    CSL_MCBSP_PHASE_SINGLE,				// single phase
    CSL_MCBSP_WORDLEN_16,				// 16-bit words
    4, /* FRMLEN1 */					// 4 words per frame
    CSL_MCBSP_WORDLEN_8,				// default
    1, /* FRMLEN2 */					// default
    CSL_MCBSP_FRMSYNC_DETECT,			// default
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,	// MSB first
    CSL_MCBSP_DATADELAY_2_BITS,			// 2-bit data delay
    CSL_MCBSP_RJUSTDXENA_RJUST_RZF,		// default
    CSL_MCBSP_INTMODE_ON_READY			// default
};

CSL_McbspDataSetup xmtDataCfg = {
    CSL_MCBSP_PHASE_SINGLE,				// single phase
    CSL_MCBSP_WORDLEN_16,				// 16-bit words
    4, /* FRMLEN1 */					// 4 words per frame
    CSL_MCBSP_WORDLEN_8, 				// default
    1, /* FRMLEN2 */					// default
    CSL_MCBSP_FRMSYNC_DETECT,			// default
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,	// MSB first
    CSL_MCBSP_DATADELAY_2_BITS,			// 2-bit data delay
    CSL_MCBSP_RJUSTDXENA_DXENA_OFF,		// dxena not supported
    CSL_MCBSP_INTMODE_ON_READY			// default
};

CSL_McbspClkSetup clkCfg = {
    CSL_MCBSP_FSCLKMODE_INTERNAL, 		// FSX is a master
    CSL_MCBSP_FSCLKMODE_EXTERNAL, 		// FSR is a slave
    CSL_MCBSP_TXRXCLKMODE_INTERNAL,		// CLKX is a master
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL, 	// CLKR is a slave
    CSL_MCBSP_FSPOL_ACTIVE_HIGH,   		// FSX is active high
    CSL_MCBSP_FSPOL_ACTIVE_HIGH,   		// FSR is active high
    CSL_MCBSP_CLKPOL_TX_RISING_EDGE,   	// transmit on rising edge
    CSL_MCBSP_CLKPOL_RX_FALLING_EDGE,   // receive on falling edge
    1,  								// frame width
    63, 								// Frame-sync pulse period = 64 bits
    30,  								// CLKGDIV
    CSL_MCBSP_SRGCLK_CLKCPU, 			// SCLKME = 0, CLKSM = 1, internal source
    CSL_MCBSP_CLKPOL_SRG_RISING_EDGE, 	// default
    CSL_MCBSP_TXFSMODE_SRG, 			// Frame-sync generated periodically
    CSL_MCBSP_CLKGSYNCMODE_OFF 			// default
};

CSL_McbspMulChSetup mulChCfg = CSL_MCBSP_MULTICHAN_DEFAULTS;

CSL_McbspHwSetup myHwCfg = {
    &gblCfg,
    &rcvDataCfg,
    &xmtDataCfg,
    &clkCfg,
    &mulChCfg,
    CSL_MCBSP_EMU_STOP,
    NULL
};


CSL_Status status;
CSL_McbspHandle hMcbsp;
CSL_McbspObj myMcbspObj;

CSL_SswObj sswObj;
CSL_SswHandle hSsw ;
CSL_SswSharedPer sharedPer;

/** Source location for DMA transfer */
int src_ping[N*2];
int src_pong[N*2];
/** Destination location for DMA transfer */
int dst_ping[N*2];
int dst_pong[N*2];

volatile int isIsrHit_xmit, isIsrHit_rcv;
/* optional flags */
int ping_xmit=1; // start with ping
int ping_rcv=1; // start with ping
int end_prog_status;

int isrHitCount = 0;
int error_count = 0;

/** DMA event ID */
CSL_IntcEventId dmaEventId_xmit, dmaEventId_rcv ;
//DMA handle.   
CSL_DmaHandle	hDma_xmit, hDma_rcv ;

void configIntHandler(CSL_IntcHandle hIntc_xmit, CSL_IntcHandle hIntc_rcv );
void ssw_acquire(void);

CSL_IntcContext context = CSL_INTC_CONTEXT_INITVAL;
CSL_IntcDispatcherContext dispatchContext = CSL_INTC_DISPATCHERCONTEXT_INITVAL;

/*---------main routine---------*/
int main (void)
{
	CSL_IntcObj intcObj_xmit, intcObj_rcv;
	CSL_IntcEventId	myDmaEventId_xmit, myDmaEventId_rcv ;
	CSL_IntcGlobalEnableState state;
	CSL_IntcHandle hIntc_xmit, hIntc_rcv;
	
    CSL_Status st = CSL_SOK;
	
	int k ;
	
		
	CSL_BitMask16 ctrlMask;

	// This variable allocates memory for DMA object. Used as parameter 
	// to CSL_dmaOpen function
	CSL_DmaObj 	dmaObj_xmit, dmaObj_rcv ;
	
	//DMA module specific parameters
	CSL_DmaParam	dmaParam_xmit, dmaParam_rcv;

	/*	Setup for a DMA channel */
	CSL_DmaHwSetup hwSetup_xmit, hwSetup_rcv ; 

	/* McBSP Transmit Section */
	//Setup for source port
	CSL_DmaHwSetupSrcPort srcObj_xmit = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_DARAM, 
											CSL_DMA_AMODE_POSTINC, 
											0, 
											0, 
											0} ;
	//Setup for destination port
	CSL_DmaHwSetupDstPort dstObj_xmit = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_TIPB,
											CSL_DMA_AMODE_CONST, 
											0, 
											0, 
											0} ;
	//Setup for channel parameters
	CSL_DmaHwSetupChannel channelObj_xmit = {CSL_DMA_DATASIZE_16BIT, 
											(N*2)*2, // size = ping + pong
											1, 
											CSL_DMA_PRIORITY_HI, 
											CSL_DMA_SYNC_EVT18, // mapped McBSP2TX to REQ18
																// default was UART3 Receive
											CSL_DMA_FRAMESYNC_DISABLE, 
											CSL_DMA_ONOFF_OFF, 	// end_prog
											CSL_DMA_ONOFF_ON, 	// repeat
											CSL_DMA_ONOFF_ON};	// auto-init
	//Setup for interrupts
	CSL_DmaHwSetupIntr intrObj_xmit = { 	CSL_DMA_ONOFF_ON, 	// block interrupt (pong)
											CSL_DMA_ONOFF_OFF, 	// last
											CSL_DMA_ONOFF_OFF, 	// frame
											CSL_DMA_ONOFF_ON, 	// half (ping)
											CSL_DMA_ONOFF_OFF, 	// dropped
											CSL_DMA_ONOFF_OFF};	// time out



	/* McBSP Receive Section */
	//Setup for source port
	CSL_DmaHwSetupSrcPort srcObj_rcv = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_TIPB,
											CSL_DMA_AMODE_CONST, 
											0, 
											0, 
											0} ;
	//Setup for destination port
	CSL_DmaHwSetupDstPort dstObj_rcv = {	CSL_DMA_BURSTEN_DISABLE, 
											CSL_DMA_ONOFF_OFF, 
											CSL_DMA_PORT_DARAM, 
											CSL_DMA_AMODE_POSTINC, 
											0, 
											0, 
											0} ;
	//Setup for channel parameters
	CSL_DmaHwSetupChannel channelObj_rcv = {CSL_DMA_DATASIZE_16BIT, 
											(N*2)*2, 	// * size = ping + pong
											1, 
											CSL_DMA_PRIORITY_HI, 
											CSL_DMA_SYNC_EVT7,	// mapped McBSP2RX to REQ7
											CSL_DMA_FRAMESYNC_DISABLE, 
											CSL_DMA_ONOFF_OFF, 	// end_prog
											CSL_DMA_ONOFF_ON, 	// repeat
											CSL_DMA_ONOFF_ON};	// auto-init
	//Setup for interrupts
	CSL_DmaHwSetupIntr intrObj_rcv = { 		CSL_DMA_ONOFF_ON, 	// block interrupt (pong)
											CSL_DMA_ONOFF_OFF, 	// last
											CSL_DMA_ONOFF_OFF, 	// frame
											CSL_DMA_ONOFF_ON, 	// half (ping)
											CSL_DMA_ONOFF_OFF, 	// dropped
											CSL_DMA_ONOFF_OFF};	// time out

	/* DMA Initialization */
	CSL_dmaInit(NULL);
	/* Interrupt Initialization */
    CSL_intcInit(&context); 
    /* Initialize CSL library, this step is required */
    CSL_mcbspInit(NULL);
    
	/* Setup the CPU's IVPD & IVPH registers. */                         
   	CSL_chipWriteReg( CSL_CHIP_REG_IVPD, 0xD0);
   	CSL_chipWriteReg( CSL_CHIP_REG_IVPH, 0xD0);

	ssw_acquire();	// acquire McBSP2 from MPU (release by MPU located in MPU gel file)
    
    CSL_intcDispatcherInit(&dispatchContext);	// Hooks up and sets up teh CSL interrupt dispatchers
    CSL_intcGlobalEnable(&state);		// returns previous state?
    
	/* Construct DMA setup object */

	/* Transmit Section */
	//Source address - location from which data is to be transferred
	srcObj_xmit.staddr = (Uint32)src_ping;	// start with ping
	//Destination address - location to which data is to be transferred
	dstObj_xmit.staddr = (Uint32)0x8803 ;  // DXR1
	
	/* Receive Section */
	//Source address - location from which data is to be transferred
	srcObj_rcv.staddr = (Uint32)0x8801 ; // DRR1
	//Destination address - location to which data is to be transferred
	dstObj_rcv.staddr = (Uint32)dst_ping;  // start with ping
	
	
	//Construct complete setup object for a channel
	hwSetup_xmit.srcStruct 		= &srcObj_xmit; 
	hwSetup_xmit.dstStruct 		= &dstObj_xmit;
	hwSetup_xmit.channelStruct	= &channelObj_xmit; 
	hwSetup_xmit.intrStruct 	= &intrObj_xmit ;       
	hwSetup_xmit.globalStruct	= NULL; //Global setup is default(reset values).

	hwSetup_rcv.srcStruct 		= &srcObj_rcv; 
	hwSetup_rcv.dstStruct 		= &dstObj_rcv;
	hwSetup_rcv.channelStruct	= &channelObj_rcv; 
	hwSetup_rcv.intrStruct 		= &intrObj_rcv ;       
	hwSetup_rcv.globalStruct	= NULL; //Global setup is default(reset values).


  /* Open MCBSP Port, this will return a MCBSP handle that will */
  /* be used in calls to other CSL functions                    */
  hMcbsp = CSL_mcbspOpen(&myMcbspObj, CSL_MCBSP_2,
                         NULL, &status);

  if ((hMcbsp == NULL) || (status != CSL_SOK)) {
      exit(1);
  }

  /* Put SRG, Frame-sync, XMT and RCV in reset */
  ctrlMask =   CSL_MCBSP_CTRL_SRG_DISABLE
             | CSL_MCBSP_CTRL_FSYNC_DISABLE
             | CSL_MCBSP_CTRL_TX_DISABLE
             | CSL_MCBSP_CTRL_RX_DISABLE;
  CSL_mcbspHwControl(hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);

  /* Program MCBSP control registers and wait for 2 clock cycles */
  CSL_mcbspHwSetup(hMcbsp, &myHwCfg);
  WAIT_FOR_2_CLK;

  /* Start Sample Rate Generator and wait for 2 clock cycles */
  ctrlMask = CSL_MCBSP_CTRL_SRG_ENABLE | CSL_MCBSP_CTRL_FSYNC_ENABLE;
  CSL_mcbspHwControl(hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  WAIT_FOR_2_CLK;

  /* Transmit DMA Channel */  
  //Select Channel 0 (arbitrary)
  dmaParam_xmit.chaNum = CSL_DMA_CHA0 ;

  /* Receive DMA Channel */  
  //Select Channel 1 (arbitrary)
  dmaParam_rcv.chaNum = CSL_DMA_CHA1 ;
  
		//Open Transmit DMA channel for use
		hDma_xmit = CSL_dmaOpen(&dmaObj_xmit, CSL_DMA, &dmaParam_xmit, &st); 	
		
		//Open Receive DMA channel for use
		hDma_rcv = CSL_dmaOpen(&dmaObj_rcv, CSL_DMA, &dmaParam_rcv, &st); 	
		
  /* Initialize data */
	        for(k=0; k < N*2; k++)   
	        {    
	        	src_ping[k] = 0; 	 	// used for external loopback
	        	src_pong[k] = 0;  		// used for external loopback
	        	dst_ping[k] = 0xDEAD;
	        	dst_pong[k] = 0xDEAD;
	        }

		if(hDma_xmit != NULL)
		{
			//Setup the channel
			CSL_dmaHwSetup(hDma_xmit, &hwSetup_xmit);
		}
		else {
			return 0;
		}
		if (hDma_rcv != NULL)
		{
			//Setup the channel
			CSL_dmaHwSetup(hDma_rcv, &hwSetup_rcv);
		}
		else {
			return 0;
		}
		
		CSL_dmaGetChipCtxt(hDma_xmit, CSL_DMA_CHIPCTXTQUERY_EVENTID, &myDmaEventId_xmit);
		CSL_dmaGetChipCtxt(hDma_rcv, CSL_DMA_CHIPCTXTQUERY_EVENTID, &myDmaEventId_rcv);
		
		/* Obtain a handle to the Interrupt module correspoding to DMA channel 0*/
    	hIntc_xmit = CSL_intcOpen(&intcObj_xmit, myDmaEventId_xmit, NULL, NULL);
    	if (hIntc_xmit == NULL) {
	    	exit();
		}
		/* Obtain a handle to the Interrupt module correspoding to DMA channel 1*/
    	hIntc_rcv = CSL_intcOpen(&intcObj_rcv, myDmaEventId_rcv, NULL, NULL);
    	if (hIntc_rcv == NULL) {
	    	exit();
		}
		/* Configure the Interrupt Controller module. */
    	configIntHandler(hIntc_xmit, hIntc_rcv);


		isIsrHit_xmit = 0;
		isIsrHit_rcv = 0;

		//Start data receive
		CSL_dmaHwControl(hDma_rcv, CSL_DMA_CMD_START, NULL);

		//Start data transfer
		CSL_dmaHwControl(hDma_xmit, CSL_DMA_CMD_START, NULL);
		
  		/* Enable MCBSP transmit and receive */
  		ctrlMask = CSL_MCBSP_CTRL_TX_ENABLE | CSL_MCBSP_CTRL_RX_ENABLE;
  		CSL_mcbspHwControl(hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
		
		/* Wait for DMA interrupt generation. */
		while(!(isIsrHit_xmit && isIsrHit_rcv));
	while(1);	

}

/** Interrupt Service Routine for transmit DMA interrupt */
void isr_xmit(void *arg)
{
	CSL_DmaStatus dmaStatus;
	CSL_dmaGetHwStatus(hDma_xmit, CSL_DMA_QUERY_STATUS, &dmaStatus); // read DMA_CSR	

	isIsrHit_xmit = 1;
    isrHitCount++ ;
    
    if (dmaStatus.halfframe == 1){ // check if ping was transferred
    	ping_xmit = 0;
		/* peform processing */
    	
    }
    else if (dmaStatus.block == 1){  // pong was transferred
    	ping_xmit = 1;
    	/* perform processing */
    }
    else if (dmaStatus.eventDrop == 1){	// DMA dropped event
		/* perform processing, re-initialization */
    }
    
}
/** Interrupt Service Routine for receive DMA interrupt */
void isr_rcv(void *arg)
{
	int i;
	
	CSL_DmaStatus dmaStatus;
	CSL_dmaGetHwStatus(hDma_rcv, CSL_DMA_QUERY_STATUS, &dmaStatus); // read DMA_CSR    

	isIsrHit_rcv = 1;
    isrHitCount++ ;
    
    if (dmaStatus.halfframe == 1){ // check if ping was received
    	ping_rcv = 0;
    	//peform copy for data loopback, data processing routine
    	for(i=0; i<N*2; i++){
		#if LOOPBACK
    	src_ping[i] = dst_ping[i];	// used for external loopback
    	#else
			if (src_ping[i] != dst_ping[i]){
				error_count++;
				}
			src_ping[i] = rand();	// used to update sample
		#endif
    	}
    }
    else if (dmaStatus.block == 1){  // pong was received
    	ping_rcv = 1;
    	//perform copy for data loopback
    	for(i=0; i<N*2; i++){
    	#if LOOPBACK
    	src_pong[i] = dst_pong[i];	// used for external loopback
		#else
			if (src_pong[i] != dst_pong[i]){
				error_count++;
				}
			src_pong[i] = ~(rand());	// used to update sample
		#endif
    	}
    }
    else if (dmaStatus.eventDrop == 1){	// DMA dropped event
    	/* perform processing, re-initialization */
    }

}

/** Configures the interrupt handler module to handle DMA interrupt. 
 *  Configures channel interrupt by getting event ID using API CSL_dmaGetChipCtxt(...) 
 */  
void configIntHandler(CSL_IntcHandle hIntc_xmit, CSL_IntcHandle hIntc_rcv ) {

	CSL_IntcHwSetup           inthSetup;
	CSL_IntcGlobalEnableState state;
	CSL_IntcEventHandlerRecord isrRec_xmit, isrRec_rcv;

	/* Hardware settings for the Interrupt Handler module */
	inthSetup.priority = CSL_INTC_PRIORITY_DEFAULT; /* Use the default priority */
	inthSetup.sense    = CSL_INTC_SENSE_FALL_EDGE; /* Triggers on falling edge */
	inthSetup.type     = CSL_INTC_TYPE_FIQ; /* In OMAP59120, FIQ is the only type of routing supported. */

	/* Set up interrupt controller for receive and transmit side */
	CSL_intcHwSetup (hIntc_xmit, &inthSetup);
	CSL_intcHwSetup (hIntc_rcv, &inthSetup);

    /* Function Address w/ Argument for the Interrupt Handler */
	isrRec_xmit.handler = isr_xmit;
	isrRec_rcv.handler = isr_rcv;

	/* Argument to be used in the Interrupt Service Routine */
	dmaEventId_xmit = hIntc_xmit->eventId ;
	isrRec_xmit.arg = (CSL_IntcEventId *) &dmaEventId_xmit;
	CSL_intcPlugEventHandler(hIntc_xmit, &isrRec_xmit);
	CSL_intcEventEnable(dmaEventId_xmit, &state);

	dmaEventId_rcv = hIntc_rcv->eventId ;
	isrRec_rcv.arg = (CSL_IntcEventId *) &dmaEventId_rcv;
	CSL_intcPlugEventHandler(hIntc_rcv, &isrRec_rcv);
	CSL_intcEventEnable(dmaEventId_rcv, &state);
	

}

void ssw_acquire(void)
{
	/* Obtain handle to SSW object */
	hSsw = CSL_sswOpen(&sswObj, CSL_SSW, NULL, &status);

    if ((hSsw == NULL) || (status != CSL_SOK)) {
      exit(1);
    }

    /* After MCBSP2 is released from MPU side (in gel), acquire it from DSP side */
	sharedPer = CSL_SSW_SHAREDPER_MCBSP2;
	CSL_sswHwControl(hSsw, CSL_SSW_CMD_ACQUIRE, &sharedPer);

}
