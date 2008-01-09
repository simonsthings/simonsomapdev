/*
 * Copyright (C) 2005 Texas Instruments Incorporated
 * All Rights Reserved
 */
/** McBSP1 in a TDM data highway in multichannel mode
*
* McBSP1 is a master for transmission and a slave for reception.  
* The implementation has a software check
* routine in the ISRs, which should be replaced by the actual 
* application software.  McBSP1 transmits on channels 0 and 1, and
* receives on channels 3 and 4.
*
* N represents the number of audio samples per buffer.  The
* array size is twice the number of samples to account for left
* and right channels for a given audio sample.
*
* This particular example also uses McBSP3 as a multichannel device
* on the same TDM data highway.  McBSP3 is a slave transmitter and
* receiver.  It has transmits channels 3 and 4 and receives channels
* 0 and 1.
*
* Neither McBSP transfers on channels 2 and 5 to prevent data
* contention between multichannel devices.
*
* McBSP3 needs to be released from reset during the inactive 
* state of the FSX signal to prevent data misalignment.
* This is accomplished by detecting the first frame sync using
* the McBSP DSP interrupt.  In this case, since
* FSX/RP=0, the inactive state of FSX/R is low.  This can be
* accomplished since the frame sync width is 1-bit so the duty cycle
* can be enough to release the transmitter in the inactive state.
*
* Each channel is 16-bits, and each frame is 6 channels long to accomodate
* the 4 transmission channels and 2 dummy channels.
*
* Once the McBSP transmitter and receiver are taken out of reset,
* the DSP DMA services the event requests using the following buffering
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
* This buffering scheme is implemented for both McBSP1 and McBSP3.
*
* This example runs with CSL 3.0.
*/

 
#define LOOPBACK 0
#define TEST 1

#include <stdio.h>
#include <csl.h>
#include <csl_error.h>
#include <csl_intc.h>
#include <csl_chip.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <soc.h>

//#define N 0x20
#define N 256

#define MCBSP2_SPCR2 0x0B804

void mcbsp_setup (CSL_McbspHandle *hMcbsp1, CSL_McbspHandle *hMcbsp3);
void int_setup (CSL_DmaHandle *hDma_xmit1, CSL_DmaHandle *hDma_rcv1, CSL_DmaHandle *hDma_xmit3, CSL_DmaHandle *hDma_rcv3);
int dma_setup(CSL_DmaHandle *hDma_xmit1, CSL_DmaHandle *hDma_rcv1, CSL_DmaHandle *hDma_xmit3, CSL_DmaHandle *hDma_rcv3);

/*-------Macro that gives 2 CLK delay cycles---------*/
/* The actual delay depends on the DSP frequency and the CLKX/R rate */
#define WAIT_FOR_2_CLK  do {                                   \
                               volatile int delayCnt = 20;      \
                               while(delayCnt > 0) --delayCnt; \
                           } while (0)

#define	IO_REG(a)	*(ioport volatile unsigned int *)(a)

CSL_Status status1, status3;
CSL_McbspHandle hMcbsp1,hMcbsp3;
CSL_McbspObj myMcbspObj1, myMcbspObj3;

/** Source location for DMA transfer */
int src_ping1[N*2];
int src_pong1[N*2];
/** Destination location for DMA transfer */
int dst_ping1[N*2];
int dst_pong1[N*2];

/** Source location for DMA transfer */
int src_ping3[N*2];
int src_pong3[N*2];
/** Destination location for DMA transfer */
int dst_ping3[N*2];
int dst_pong3[N*2];

volatile int isIsrHit_xmit1, isIsrHit_rcv1, isIsrHit_xmit3, isIsrHit_rcv3;
/* optional completion/error flags */
int ping_xmit1 = 1; // start with ping
int ping_rcv1 = 1; // start with ping
int ping_xmit3 = 1; // start with ping
int ping_rcv3 = 1; // start with ping

int isrHitCount = 0;
int isrHitCountRecord = 0;
int error_previous = 0;
int iRecord = 0x1234;
int error_count = 0;
int drop_count = 0;
int mcbsp3_xempty = 0;
int mcbsp3_rfull = 0;
int mcbsp3_xsyncerr = 0;
int mcbsp3_rsyncerr = 0;

CSL_BitMask16 ctrlMasktx= CSL_MCBSP_CTRL_TX_ENABLE;
int first_FSX = 1;

//DMA handle.   
CSL_DmaHandle	hDma_xmit1, hDma_rcv1 ;
CSL_DmaHandle	hDma_xmit3, hDma_rcv3 ;

// This variable allocates memory for DMA object. Used as parameter 
// to CSL_dmaOpen function
CSL_DmaObj 	dmaObj_xmit1, dmaObj_rcv1;
CSL_DmaObj 	dmaObj_xmit3, dmaObj_rcv3;

//DMA module specific parameters
CSL_DmaParam	dmaParam_xmit1, dmaParam_rcv1;
CSL_DmaParam	dmaParam_xmit3, dmaParam_rcv3;

/*---------main routine---------*/
int main (void)
{
    CSL_Status st = CSL_SOK;
	
	int k ;
	Uint16 response1, response3;
	
		
	CSL_BitMask16 ctrlMask;

	/* Setup the CPU's IVPD & IVPH registers. */                         
   	CSL_chipWriteReg( CSL_CHIP_REG_IVPD, 0xD0);
   	CSL_chipWriteReg( CSL_CHIP_REG_IVPH, 0xD0);


	// set up initial data
	        for(k=0; k < N*2; k++)   
	        {    
	        	src_ping1[k] = 0; 
	        	src_pong1[k] = 0; 
	        	
	        	src_ping3[k] = 0; 
	        	src_pong3[k] = 0; 

#if TEST
	        	src_ping1[k] = (k+1); 
	        	src_pong1[k] = -(k+1); 
	        	
	        	src_ping3[k] = (k+1); 
	        	src_pong3[k] = -(k+1); 
#endif
	        }
    
  /* Open MCBSP Port, this will return a MCBSP handle that will */
  /* be used in calls to other CSL functions                    */
  hMcbsp1 = CSL_mcbspOpen(&myMcbspObj1, CSL_MCBSP_1,
                         NULL, &status1);
  hMcbsp3 = CSL_mcbspOpen(&myMcbspObj3, CSL_MCBSP_3,
                         NULL, &status3);

  if ((hMcbsp1 == NULL) || (status1 != CSL_SOK)) {
      exit(1);
  }
  if ((hMcbsp3 == NULL) || (status3 != CSL_SOK)) {
      exit(1);
  }

  /* Step 1: Ensure McBSP is in reset */
  ctrlMask =   CSL_MCBSP_CTRL_SRG_DISABLE
             | CSL_MCBSP_CTRL_FSYNC_DISABLE
             | CSL_MCBSP_CTRL_TX_DISABLE
             | CSL_MCBSP_CTRL_RX_DISABLE;
  CSL_mcbspHwControl(hMcbsp1, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  CSL_mcbspHwControl(hMcbsp3, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);

  /* Step 2: Program MCBSP control registers, with McBSP3 generating */ 
  /* interrupts upon FSX detection */
  mcbsp_setup(&hMcbsp1, &hMcbsp3);
  
  /* Step 3: Wait for at least 2 CLKX cycles */
  WAIT_FOR_2_CLK;

  /* Step 4: Only applies for McBSP1 */
  /* Start Sample Rate Generator and wait for 2 clock cycles */
  ctrlMask = CSL_MCBSP_CTRL_SRG_ENABLE;
  CSL_mcbspHwControl(hMcbsp1, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  WAIT_FOR_2_CLK;

  /* Step 5: Eliminate possible XSYNCERR */
  /* Step 5a: Set XRST to 1 to enable the transmitter */
  ctrlMask = CSL_MCBSP_CTRL_TX_ENABLE;
  CSL_mcbspHwControl(hMcbsp1, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  CSL_mcbspHwControl(hMcbsp3, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  /* Step 5b: Wait for sync error */
  WAIT_FOR_2_CLK;
  /* Step 5c: Disable the transmitter */
  ctrlMask = CSL_MCBSP_CTRL_TX_DISABLE;
  CSL_mcbspHwControl(hMcbsp1, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  CSL_mcbspHwControl(hMcbsp3, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  
  /* Step 6: Setup data acquisition */
  /* Transmit DMA Channel */  
  //Select Channel 0 (arbitrary)
  dmaParam_xmit1.chaNum = CSL_DMA_CHA0 ;
  dmaParam_xmit3.chaNum = CSL_DMA_CHA2 ;

  /* Receive DMA Channel */  
  //Select Channel 1 (arbitrary)
  dmaParam_rcv1.chaNum = CSL_DMA_CHA1 ;
  dmaParam_rcv3.chaNum = CSL_DMA_CHA3 ;

  //Open Transmit DMA channel for use
  hDma_xmit1 = CSL_dmaOpen(&dmaObj_xmit1, CSL_DMA, &dmaParam_xmit1, &st); 	
  hDma_xmit3 = CSL_dmaOpen(&dmaObj_xmit3, CSL_DMA, &dmaParam_xmit3, &st); 
	
  //Open Receive DMA channel for use
  hDma_rcv1 = CSL_dmaOpen(&dmaObj_rcv1, CSL_DMA, &dmaParam_rcv1, &st); 	
  hDma_rcv3 = CSL_dmaOpen(&dmaObj_rcv3, CSL_DMA, &dmaParam_rcv3, &st); 	

  dma_setup(&hDma_xmit1, &hDma_rcv1, &hDma_xmit3, &hDma_rcv3);

  /* Globally enable the McBSP TX interrupt in this step as well */
  int_setup(&hDma_xmit1, &hDma_rcv1, &hDma_xmit3, &hDma_rcv3);

		isIsrHit_xmit1 = 0;
		isIsrHit_rcv1 = 0;
		
		isIsrHit_xmit3 = 0;
		isIsrHit_rcv3 = 0;

		//Start data receive
		CSL_dmaHwControl(hDma_rcv1, CSL_DMA_CMD_START, NULL);
		CSL_dmaHwControl(hDma_rcv3, CSL_DMA_CMD_START, NULL);

		//Start data transfer
		CSL_dmaHwControl(hDma_xmit1, CSL_DMA_CMD_START, NULL);
		CSL_dmaHwControl(hDma_xmit3, CSL_DMA_CMD_START, NULL);

	/* Step 7: Enable MCBSP1 transmit and receive */
	ctrlMask = CSL_MCBSP_CTRL_TX_ENABLE | CSL_MCBSP_CTRL_RX_ENABLE;
	CSL_mcbspHwControl(hMcbsp1, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
	WAIT_FOR_2_CLK;

	/* Enable MCBSP3 receive only for now */
	ctrlMask = CSL_MCBSP_CTRL_RX_ENABLE;
	CSL_mcbspHwControl(hMcbsp3, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
	WAIT_FOR_2_CLK;
	
	/* Step 8: Skip */
	
	/* Step 9: Check for XEMPTY = 1*/
	status1 = CSL_mcbspGetHwStatus(hMcbsp1, CSL_MCBSP_QUERY_DEV_STATUS, &response1);
	status3 = CSL_mcbspGetHwStatus(hMcbsp3, CSL_MCBSP_QUERY_DEV_STATUS, &response3);
	if (!(response1 & CSL_MCBSP_XEMPTY)){
		// check if problem in DMA setup
	}
	if (!(response3 & CSL_MCBSP_XEMPTY)){
		// check if problem in DMA setup
	}

	/* Step 10: Start FSX Generator and wait for 2 clock cycles */
	ctrlMask = CSL_MCBSP_CTRL_FSYNC_ENABLE;
	CSL_mcbspHwControl(hMcbsp1, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
	WAIT_FOR_2_CLK;
		
		/* Wait for DMA interrupt generation. */
	for(;;){
		if (error_count){
		  while(1);
		}
		
	}	// for(;;);
}

/* Interrupt Service Routine for transmit DMA interrupt. */
void isr_xmit1(void *arg)
{
	CSL_DmaStatus dmaStatus;

			CSL_dmaGetHwStatus(hDma_xmit1, CSL_DMA_QUERY_STATUS, &dmaStatus); // read DMA_CSR
			
			isIsrHit_xmit1 = 1;
		    isrHitCount++ ;
		
			if (dmaStatus.halfframe == 1){	// finished transmitting half frame
			    	ping_xmit1 = 0;			// ping has finished transferring
			}
			
			if (dmaStatus.block == 1){		// finished transmitting block
		    		ping_xmit1 = 1;			// pong has finished transferring
			}
			
			if (dmaStatus.eventDrop == 1){
					drop_count++;
			}
    
}
/** Interrupt Service Routine for receive DMA interrupt. This sets isIsrHit_rcv to 1 
 * indicating DMA triggered an interrupt 
*/
void isr_rcv1(void *arg)
{
	int i;
	CSL_DmaStatus dmaStatus;

	isIsrHit_rcv1 = 1;
    isrHitCount++ ;

			CSL_dmaGetHwStatus(hDma_rcv1, CSL_DMA_QUERY_STATUS, &dmaStatus); // read DMA_CSR  

		    if (dmaStatus.halfframe == 1){ // check if ping was received
		    	ping_rcv1 = 0;
		    	//peform copy for data loopback, data processing routine
		    	for(i=0; i<N*2; i++){
					if (src_ping3[i] != dst_ping1[i+2]){
						error_count++;
						isrHitCountRecord = isrHitCount;
						i = iRecord;
						}
					src_ping3[i] = rand();	// change data for next transfer
		    	}
		    }
		    if (dmaStatus.block == 1){  // pong was received
		    	ping_rcv1 = 1;
		    	//perform copy for data loopback, data processing routine
		    	for(i=0; i<N*2; i++){	// check half block only
					if (src_pong3[i] != dst_ping1[(i+2+N*2)%(N*4)]){	// check half block only
						error_count++;
						isrHitCountRecord = isrHitCount;
						i = iRecord;
						}
					src_pong3[i] = ~(rand());	// change data for next transfer
		    	}
		    }
			
			if (dmaStatus.eventDrop == 1){
					drop_count++;
			}

}

void isr_xmit3(void *arg)
{
	CSL_DmaStatus dmaStatus;

			CSL_dmaGetHwStatus(hDma_xmit3, CSL_DMA_QUERY_STATUS, &dmaStatus); // read DMA_CSR
			
			isIsrHit_xmit3 = 1;
		    isrHitCount++ ;
		
			if (dmaStatus.halfframe == 1){	// finished transmitting half frame
			    	ping_xmit3 = 0;			// ping has finished transferring
			}
			
			if (dmaStatus.block == 1){		// finished transmitting block
		    		ping_xmit3 = 1;			// pong has finished transferring
			}
			
			if (dmaStatus.eventDrop == 1){
					drop_count++;
			}
    
}
/* Interrupt Service Routine for receive DMA interrupt. */
void isr_rcv3(void *arg)
{
	int i;
	CSL_DmaStatus dmaStatus;

	isIsrHit_rcv3 = 1;
    isrHitCount++ ;

			CSL_dmaGetHwStatus(hDma_rcv3, CSL_DMA_QUERY_STATUS, &dmaStatus); // read DMA_CSR  

		    if (dmaStatus.halfframe == 1){ // check if ping was received
		    	ping_rcv3 = 0;
		    	//peform copy for data loopback, data processing routine
		    	for(i=0; i<N*2; i++){
					if (src_ping1[i] != dst_ping3[i]){	
						error_count++;
						isrHitCountRecord = isrHitCount;
						i = iRecord;
						}
					src_ping1[i] = rand();	// change data for next transfer
		    	}
		    }
		    if (dmaStatus.block == 1){  // pong was received
		    	ping_rcv3 = 1;
		    	//perform copy for data loopback, data processing routine
		    	for(i=0; i<N*2; i++){
					if (src_pong1[i] != dst_pong3[i]){	// for external LB
						error_count++;
						isrHitCountRecord = isrHitCount;
						i = iRecord;
						}
					src_pong1[i] = ~(rand());	// change data for next transfer
		    	}
		    }
		    
			if (dmaStatus.eventDrop == 1){
					drop_count++;
			}

}

/* Detect first frame sync pulse and then check for unexpected frame sync pulses */
void isr_tx3(void *arg)
{
	if (first_FSX){
	hMcbsp3->regs->SPCR2 = hMcbsp3->regs->SPCR2 | 0x0030;	// set XINTM = 11b
	IO_REG(MCBSP2_SPCR2) = IO_REG(MCBSP2_SPCR2) | 0x0001; 	// SPCR2[0] = \XRST = 1, transmitter out
															// of reset after first FSX is detected

	// from this point on, interrupt if unexpected FSX is detected (normal interrupt operation)
	first_FSX = 0;
	}
	else{
	mcbsp3_xsyncerr++;
	}
    
}

/* Check for unexpected frame sync pulses */
void isr_rx3(void *arg)
{
	mcbsp3_rsyncerr++;

}




