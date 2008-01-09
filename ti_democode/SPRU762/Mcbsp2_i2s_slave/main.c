/*
 * Copyright (C) 2005 Texas Instruments Incorporated
 * All Rights Reserved
 */
/** McBSP2 interfaced to Two Codecs with Independent External
* Transmit and Receive Rates
*
* McBSP2 is a slave for both reception and transmission, and
* operates in I2S mode.  The implementation has a software check
* routine in the ISRs, which should be replaced by the actual 
* application software.
*
* N represents the number of audio samples per buffer.  The
* array size is twice the number of samples to account for left
* and right channels for a given audio sample.
*
* McBSP2 needs to be released from reset during the inactive 
* state of the FSX and FSR signal to prevent data misalignment.
* This is accomplished by detecting the first frame sync using
* the McBSP MPU interrupt, and then using the GPIO capability
* of the McBSP to detect the inactive state.  In this case, since
* FSX/RP=1, the inactive state of FSX/R is high.
*
* Once the McBSP transmitter and receiver are taken out of reset,
* the DMA services the event requests using the following buffering
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
* srcBufPing[1]
* srcBufPing[2]
* srcBufPing[3]
* ...
* srcBufPing[N*2-2]
* srcBufPing[N*2-1]		half-frame
* srcBufPong[1]
* srcBufPong[2]
* ...
* srcBufPong[N*2-2]
* srcBufPong[N*2-1]		end of block
* 
* This example runs with CSL 3.0.
*/

#define AUDIO_LOOP	1
 

#include <csl.h>
#include <csl_error.h>
#include <csl_intc.h>
#include <csl_chip.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <soc.h>
#include "swi.h"

#define N 256
#define PI 3.141592654
#define PCR 	*((short *) 0xFFFB1024)
#define SPCR1 	*((short *) 0xFFFB100A)
#define SPCR2	*((short *) 0xFFFB1008)

/*-------Macro that gives 2 CLKX/R delay cycles---------*/
/* this delay is dependent on the actual MPU core speed vs. CLKX/R frequency */
#define WAIT_FOR_2_CLK  do {                                   \
                               volatile int delayCnt = 20;     \
                               while(delayCnt > 0) --delayCnt; \
                           } while (0)
                           
CSL_DmaObj sdmaObjXmit;                      /* Global DMA Object @a CSL_DmaObj */
CSL_DmaObj sdmaObjRcv;                       /* Global DMA Object @a CSL_DmaObj */
CSL_IntcObj intcObjXmit;                     /* Global Interrupt Controller Object */
CSL_IntcObj intcObjRcv;                      /* Global Interrupt Controller Object */
                           


/*---------Global data definition---------*/
CSL_McbspGlobalSetup gblCfg = {
    CSL_MCBSP_IOMODE_TXDIS_RXDIS,	// pins used as McBSP pin (not GPIO)
    CSL_MCBSP_DLBMODE_OFF,			// DLB disabled
    CSL_MCBSP_CLKSTP_DISABLE		// CLKSTP mode disabled
};

CSL_McbspDataSetup rcvDataCfg = {
    CSL_MCBSP_PHASE_SINGLE,			// single phase
    CSL_MCBSP_WORDLEN_16,			// 16-bit words
    2, /* FRMLEN1 */  				// 2 words per frame
    CSL_MCBSP_WORDLEN_8, 			// default
    1, 								// default
    CSL_MCBSP_FRMSYNC_DETECT,		// detect frame syncs
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// MSB first
    CSL_MCBSP_DATADELAY_1_BIT,		// 1-bit data delay
    CSL_MCBSP_RJUSTDXENA_RJUST_RZF,	// right justify
    CSL_MCBSP_INTMODE_ON_FSYNC		// interrupt on frame synch
};

CSL_McbspDataSetup xmtDataCfg = {
    CSL_MCBSP_PHASE_SINGLE,			// single phase
    CSL_MCBSP_WORDLEN_16,			// 16-bit words
    2, /* FRMLEN1 */ 				// 2 words per frame
    CSL_MCBSP_WORDLEN_8, 			// default
    1, /* FRMLEN2 */				// default
    CSL_MCBSP_FRMSYNC_DETECT,		// detect frame syncs
    CSL_MCBSP_COMPAND_OFF_MSB_FIRST,// MSB first
    CSL_MCBSP_DATADELAY_1_BIT,		// 1-bit data delay
    CSL_MCBSP_RJUSTDXENA_DXENA_OFF,	// disable dxena
    CSL_MCBSP_INTMODE_ON_FSYNC		// interrupt on frame synch
};

CSL_McbspClkSetup clkCfg = {
    CSL_MCBSP_FSCLKMODE_EXTERNAL, 	// FSX is external
    CSL_MCBSP_FSCLKMODE_EXTERNAL, 	// FSR is external
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL, // CLKX is external
    CSL_MCBSP_TXRXCLKMODE_EXTERNAL, // CLKP is external
    CSL_MCBSP_FSPOL_ACTIVE_LOW,		// FSXP = 1 (active low)
    CSL_MCBSP_FSPOL_ACTIVE_LOW,		// FSRP = 1 (active low)
    CSL_MCBSP_CLKPOL_TX_FALLING_EDGE,// CLKRP=1 (output on falling edge)
    CSL_MCBSP_CLKPOL_RX_RISING_EDGE,// CLKRP=1 (sample on rising edge)
    15,  							// don't care, SRG not used
    31,								// don't care, SRG not used
    9,								// don't care, SRG not used
    CSL_MCBSP_SRGCLK_CLKCPU, 		// default, SRG not used
    CSL_MCBSP_CLKPOL_SRG_RISING_EDGE,// default, not used
    CSL_MCBSP_TXFSMODE_SRG,			// default, not used
    CSL_MCBSP_CLKGSYNCMODE_OFF 		// GSYNC off
};


CSL_McbspMulChSetup mulChCfg = CSL_MCBSP_MULTICHAN_DEFAULTS;

CSL_McbspHwSetup myHwCfg = {
    &gblCfg,
    &rcvDataCfg,
    &xmtDataCfg,
    &clkCfg,
    &mulChCfg,
    CSL_MCBSP_EMU_STOP,
//	CSL_MCBSP_EMU_FREERUN,
    NULL
};


/* Create data buffers in consecutive locations in memory for transfer */
#pragma DATA_SECTION(srcBufPing, "mydata")
short srcBufPing[N*2];
#pragma DATA_SECTION(srcBufPong, "mydata")
short srcBufPong[N*2];

#pragma DATA_SECTION(dstBufPing, "mydata")
short dstBufPing[N*2];
#pragma DATA_SECTION(dstBufPong, "mydata")
short dstBufPong[N*2];

int ping_rcv = 1;
int ping_xmit = 1;
int error_count = 0;

CSL_Status status;
CSL_McbspHandle hMcbsp;
CSL_McbspObj myMcbspObj;

/* Variable to communicate data to ISR */
typedef struct sdmaIsrInfo {
   CSL_DmaHandle sdmaHndl;  /**< Handle to dma channel */
   Bool blockXferComplete;  /**< Status flag which indicates whether the block Xfer is complete */
} sdmaIsrInfo;
typedef struct sdmaIsrInfo *sdmaIsrInfoPtr;

/* DMA handles */
CSL_DmaHandle sdmaHndl_xmit=NULL;
CSL_DmaHandle sdmaHndl_rcv=NULL;

volatile int isIsrHit_xmit, isIsrHit_rcv;        

/* Optional flags */
int isrHitCount = 0;
int eventDropReceive = 0;
int eventDropTransmit = 0;
int isrReceiveHitCount = 0;
int isrTransmitHitCount = 0;
int errorReceiveCount = 0;

/** DMA event ID */
CSL_IntcEventId dmaEventId_xmit, dmaEventId_rcv ;

/* Function definitions */
void configIntHandler(CSL_IntcHandle hIntc_xmit, CSL_IntcHandle hIntc_rcv );
int intcInit(CSL_IntcHandle *intcHndlptrXmit, CSL_IntcHandle *intcHndlptrRcv);
int intcSetup(CSL_IntcHandle intcHndl,CSL_IntcEventHandlerRecord *intcEventHandlerRecordptr);
int sdmaInit(CSL_DmaHandle *sdmaHndlXmitptr, CSL_DmaHandle *sdmaHndlRcvptr);
int sdmaSetup(CSL_DmaHandle sdmaHndlXmit, CSL_DmaHandle sdmaHndlRcv);

CSL_IntcContext context = CSL_INTC_CONTEXT_INITVAL;
CSL_IntcDispatcherContext dispatchContext = CSL_INTC_DISPATCHERCONTEXT_INITVAL;

/* Interrupts */
extern CSL_IntcEventEnableState intcEventEnableStateFsr;
extern CSL_IntcEventEnableState intcEventEnableStateFsx;

/** This is the Interrupt Service Routine of the System DMA
 *  which is called by the Interrupt Dispatcher
 *
 *  Each Interrupt Service Routine has a void * argument
 *  which can be passed to the ISR.  In this test the
 *  void * argument is cast to @a #sdmaIsrInfoPtr which
 *  is a pointer to the structure of type @a #sdmaIsrInfo
 *
 *  This function gets the reason why the System DMA interrupt
 *  was triggered.
 *
 */
   
void dmaXmitIsr(void *arg)
{
    Uint16 intStatus;
    sdmaIsrInfoPtr infoPtr = (sdmaIsrInfoPtr)arg;

   CSL_dmaGetHwStatus(infoPtr->sdmaHndl,CSL_DMA_QUERY_INTERRUPT_STATUS,&intStatus);

   if(intStatus & CSL_DMA_HALF_FRAME_EVENT){
      isrTransmitHitCount++;
	  ping_xmit = 0;		// ping has compleated at half frame
	}

   else if (intStatus & CSL_DMA_END_BLOCK_EVENT) {
      infoPtr->blockXferComplete=TRUE;
      isrTransmitHitCount++;
      /* call routine for full-frame processing */
	  ping_xmit = 1;		// pong as completed at end of block
   }
	
	else if(intStatus & CSL_DMA_DROP_EVENT){
		eventDropTransmit++;
	}
}

void dmaRcvIsr(void *arg)
{
    Uint16 intStatus, i;
    sdmaIsrInfoPtr infoPtr = (sdmaIsrInfoPtr)arg;
	
   CSL_dmaGetHwStatus(infoPtr->sdmaHndl,CSL_DMA_QUERY_INTERRUPT_STATUS,&intStatus);

   if (intStatus & CSL_DMA_HALF_FRAME_EVENT) {
	    	ping_rcv = 0;	// ping has completed at half frame
	    	isrReceiveHitCount++;
			/* Insert actual processing routine here */
			   	// check data
				for(i=0; i<(N*2); i++){
					#if AUDIO_LOOP
					srcBufPing[i] = dstBufPing[i];
					#else
					if(dstBufPing[i+44] != srcBufPing[i]){	// tuned for 142.9KHz Fs
						errorReceiveCount++;
						}
					srcBufPing[i] = rand();
					#endif
				}	// end error for(;;)
	}

   if (intStatus & CSL_DMA_END_BLOCK_EVENT) {
      infoPtr->blockXferComplete=TRUE;
	   ping_rcv = 1;	// pong has completed at end of block
	   isrReceiveHitCount++;
	   /* Insert actual processing routine here */
       /* call routine for full-frame processing */
			   	// check data
				for(i=0; i<(N*2); i++){
					#if AUDIO_LOOP
					srcBufPong[i] = dstBufPong[i];
					#else
					if(dstBufPing[(i+44+N*2)%(N*4)] != srcBufPong[i]){	// tuned for 142.9KHz Fs
						errorReceiveCount++;
						}
					srcBufPong[i] = ~(rand());
					#endif
				}	// end error for(;;)
   }

	if (intStatus & CSL_DMA_DROP_EVENT){
		eventDropReceive++;
		}
}

void FsrIsr(void *arg)
{
		/* this sets FSR as a GPIO input and polls for it to be high before */
		/* releasing the transmitter from reset */
		PCR = PCR | 0x1000; 		// PCR[12] = RIOEN = 1
		/* this ensures we start at the beginning of FSR high */
		while( !(PCR & 0x0004) );	// PCR[2] = FSRP, if 1 then proceed
		while(  (PCR & 0x0004) );	// PCR[2] = FSRP, if 0 then proceed
		while( !(PCR & 0x0004) );	// PCR[2] = FSRP, if 1 then make RRST=0
		PCR = 0x000F; 				// PCR[12] = RIOEN = 0
									// restore original values

		SPCR1 = 0x0821 /*| 0x0030*/; 	// SPCR1[0] = \RRST (value=0x0820 when in reset)

	   /* Disable the FSR Interrupt */
	   CSL_intcEventDisable(CSL_INTC_EVENTID_MCBSP2RX, &intcEventEnableStateFsr);
}

void FsxIsr(void *arg)
{
		/* this sets FSR as a GPIO input and polls for it to be high before */
		/* releasing the receiver from reset */
		PCR = PCR | 0x2000; 			// PCR[13] = XIOEN = 1
		/* this ensures we start at the beginning of FSX high */
		while( !(PCR & 0x0008) );		// PCR[3] = FSXP, if 1 proceed
		while(  (PCR & 0x0008) );		// PCR[3] = FSXP, if 0, proceed
		while( !(PCR & 0x0008) );		// PCR[3] = FSXP, if 1 then make XRST=0
		PCR = 0x000F; 					// PCR[13] = XIOEN = 0
										// restore original values

		SPCR2 = 0x00e1 /*| 0x0030*/; // SPCR2[0] = \XRST (value=0x00e0 when in reset)

		/* Disable FSX interrupt on MIR */  
		CSL_intcEventDisable(CSL_INTC_EVENTID_MCBSP2TX, &intcEventEnableStateFsx);
}	



int main (void)
{
   CSL_IntcHandle intcHndl_xmit=NULL;
   CSL_IntcHandle intcHndl_rcv=NULL;
   CSL_IntcEventHandlerRecord  intcEventHandlerRecordXmit;
   CSL_IntcEventHandlerRecord  intcEventHandlerRecordRcv;
   sdmaIsrInfo mysdmaXmitIsrInfo, mysdmaRcvIsrInfo;
   
	CSL_BitMask16 ctrlMask;

  	Uint16 i;

	/* ARM_IDLECT2 */
	*((int *) 0xFFFECE08) = *((int *) 0xFFFECE08) & 0xFFFFFEFF; // DMACK_REQ=0

	SWI_sysMode(); 

   /* Initialization : All opens DMA/Interrupt opens happen here */
   sdmaInit(&sdmaHndl_xmit, &sdmaHndl_rcv);
   intcInit(&intcHndl_xmit, &intcHndl_rcv);

   mysdmaXmitIsrInfo.sdmaHndl = sdmaHndl_xmit;
   mysdmaXmitIsrInfo.blockXferComplete = FALSE;
   
   mysdmaRcvIsrInfo.sdmaHndl = sdmaHndl_rcv;
   mysdmaRcvIsrInfo.blockXferComplete = FALSE;

   intcEventHandlerRecordXmit.handler = dmaXmitIsr;
   intcEventHandlerRecordXmit.arg = (void *)&mysdmaXmitIsrInfo;
   
   intcEventHandlerRecordRcv.handler = dmaRcvIsr;
   intcEventHandlerRecordRcv.arg = (void *)&mysdmaRcvIsrInfo;

   /* Set up DMA interrupts with the appropriate information */
   intcSetup(intcHndl_xmit,&intcEventHandlerRecordXmit);
   intcSetup(intcHndl_rcv,&intcEventHandlerRecordRcv);
   
   /* Initialize CSL library, this step is required */
   sdmaSetup(sdmaHndl_xmit, sdmaHndl_rcv);

    CSL_mcbspInit(NULL);
    
    /* Initilize data buffers  */
    for(i=0;i<=(N)-1;i++) {
	    srcBufPing[2*i] = 0;
	    srcBufPing[2*i+1] = 0;
	    srcBufPong[2*i] = 0;
	    srcBufPong[2*i+1] = 0;
	    
	    dstBufPing[2*i] = 0xDEAD;
	    dstBufPing[2*i+1] = 0xDEAD;
	    dstBufPong[2*i] = 0xDEAD;
	    dstBufPong[2*i+1] = 0xDEAD;
	}

  /* Open MCBSP Port, this will return a MCBSP handle that will */
  /* be used in calls to other CSL functions                    */
  hMcbsp = CSL_mcbspOpen(&myMcbspObj, CSL_MCBSP_2,
                         NULL, &status);

  if ((hMcbsp == NULL) || (status != CSL_SOK)) {
      printf ("\nError opening CSL_MCBSP_2");
      exit(1);
  }

  /* Step 1: Put SRG, Frame-sync, XMT and RCV in reset */
  ctrlMask =   CSL_MCBSP_CTRL_SRG_DISABLE
             | CSL_MCBSP_CTRL_FSYNC_DISABLE
             | CSL_MCBSP_CTRL_TX_DISABLE
             | CSL_MCBSP_CTRL_RX_DISABLE;
  CSL_mcbspHwControl(hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);

  /* Step 2: Program MCBSP control registers */
  CSL_mcbspHwSetup(hMcbsp, &myHwCfg);

  /* Step 3: Wait for two CLKX/R cycles */
  WAIT_FOR_2_CLK;

  /* Step 5: Clear any potential XSYNCERR */
	  /* 5.a: Enable the transmitter */
	  ctrlMask = CSL_MCBSP_CTRL_TX_ENABLE;
	  CSL_mcbspHwControl(hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
	  /* 5.b: Wait for any unexpected frame sync error to occur */
	  WAIT_FOR_2_CLK;
	  /* 5.c: Disable the transmitter */
	  ctrlMask = CSL_MCBSP_CTRL_TX_DISABLE;
	  CSL_mcbspHwControl(hMcbsp, CSL_MCBSP_CMD_RESET_CONTROL, &ctrlMask);
  
  /* Step 6: Enable data acquisition */
		isIsrHit_xmit = 0;
		isIsrHit_rcv = 0;

		//Start data receive
		CSL_dmaHwControl(sdmaHndl_rcv,CSL_DMA_CMD_ENABLE_CHANNEL,NULL);
		
		//Start data transfer
		CSL_dmaHwControl(sdmaHndl_xmit,CSL_DMA_CMD_ENABLE_CHANNEL,NULL);
  
  /* set up and enable McBSP->MPU interrupts */
  mcbsp_ints();
  
  while(1);	// wait for interrupts

} // end main





