/*
 * Set up the interrupts to service the DSP DMA requests
 * and the McBSP DSP interrupts.
 *
 *
 */


#include <csl_error.h>
#include <csl_intc.h>
#include <csl_chip.h>
#include <csl_mcbsp.h>
#include <csl_dma.h>
#include <soc.h>

CSL_IntcObj intcObj_xmit1, intcObj_rcv1, intcObj_xmit3, intcObj_rcv3;
CSL_IntcEventId	myDmaEventId_xmit1, myDmaEventId_rcv1, myDmaEventId_xmit3, myDmaEventId_rcv3;
CSL_IntcGlobalEnableState state;
CSL_IntcHandle hIntc_xmit1, hIntc_rcv1, hIntc_xmit3, hIntc_rcv3;
/* McBSP DSP interrupts */
CSL_IntcObj intcObj_rx3, intcObj_tx3;
CSL_IntcHandle hIntc_rx3, hIntc_tx3;

CSL_IntcHwSetup           inthSetup_txrx;
CSL_IntcGlobalEnableState state_txrx;
CSL_IntcEventHandlerRecord isrRec_tx, isrRec_rx;
CSL_IntcEventId 		dmaEventId_tx, dmaEventId_rx;

extern void isr_tx3(void *arg);
extern void isr_rx3(void *arg);
	
/* DMA DSP interrupts */
CSL_IntcContext context = CSL_INTC_CONTEXT_INITVAL;
CSL_IntcDispatcherContext dispatchContext = CSL_INTC_DISPATCHERCONTEXT_INITVAL;

extern void isr_xmit1(void *arg);
extern void isr_xmit3(void *arg);

extern void isr_rcv1(void *arg);
extern void isr_rcv3(void *arg);

void configIntHandler1(CSL_IntcHandle hIntc_xmit, CSL_IntcHandle hIntc_rcv );
void configIntHandler3(CSL_IntcHandle hIntc_xmit, CSL_IntcHandle hIntc_rcv );

/* Sets up the DSP DMA interrupts to service the McBSPs */
void int_setup (CSL_DmaHandle * hDma_xmit1, CSL_DmaHandle * hDma_rcv1, 
				CSL_DmaHandle * hDma_xmit3, CSL_DmaHandle * hDma_rcv3)
{
		/* Interrupt Initialization */
    	CSL_intcInit(&context); 

    	CSL_intcDispatcherInit(&dispatchContext);	// Hooks up and sets up teh CSL interrupt dispatchers
    	CSL_intcGlobalEnable(&state);		// returns previous state?


		CSL_dmaGetChipCtxt((CSL_DmaHandle)*hDma_xmit1, CSL_DMA_CHIPCTXTQUERY_EVENTID, &myDmaEventId_xmit1);
		CSL_dmaGetChipCtxt((CSL_DmaHandle)*hDma_rcv1, CSL_DMA_CHIPCTXTQUERY_EVENTID, &myDmaEventId_rcv1);

		CSL_dmaGetChipCtxt((CSL_DmaHandle)*hDma_xmit3, CSL_DMA_CHIPCTXTQUERY_EVENTID, &myDmaEventId_xmit3);
		CSL_dmaGetChipCtxt((CSL_DmaHandle)*hDma_rcv3, CSL_DMA_CHIPCTXTQUERY_EVENTID, &myDmaEventId_rcv3);
		
		/* Obtain a handle to the Interrupt module correspoding to DMA channel 0*/
    	hIntc_xmit1 = CSL_intcOpen(&intcObj_xmit1, myDmaEventId_xmit1, NULL, NULL);
    	if (hIntc_xmit1 == NULL) {
	    	exit();
		}
		/* Obtain a handle to the Interrupt module correspoding to DMA channel 1*/
    	hIntc_rcv1 = CSL_intcOpen(&intcObj_rcv1, myDmaEventId_rcv1, NULL, NULL);
    	if (hIntc_rcv1 == NULL) {
	    	exit();
		}
		
		/* Obtain a handle to the Interrupt module correspoding to DMA channel 2*/
    	hIntc_xmit3 = CSL_intcOpen(&intcObj_xmit3, myDmaEventId_xmit3, NULL, NULL);
    	if (hIntc_xmit3 == NULL) {
	    	exit();
		}
		/* Obtain a handle to the Interrupt module correspoding to DMA channel 3*/
    	hIntc_rcv3 = CSL_intcOpen(&intcObj_rcv3, myDmaEventId_rcv3, NULL, NULL);
    	if (hIntc_rcv3 == NULL) {
	    	exit();
		}
		
		/* Configure the Interrupt Controller module. */
    	configIntHandler1(hIntc_xmit1, hIntc_rcv1);
    	configIntHandler3(hIntc_xmit3, hIntc_rcv3);

#if 1 	// the following code sets up the McBSP interrupts, if required by the system

    	hIntc_rx3 = CSL_intcOpen(&intcObj_rx3, CSL_INTC_EVENTID_MCBSP3_RX, NULL, NULL);
    	if (hIntc_rx3 == NULL) {
	    	exit();
		}
    	hIntc_tx3 = CSL_intcOpen(&intcObj_tx3, CSL_INTC_EVENTID_MCBSP3_TX, NULL, NULL);
    	if (hIntc_tx3 == NULL) {
	    	exit();
		}

	/* Hardware settings for the Interrupt Handler module */
	inthSetup_txrx.priority = CSL_INTC_PRIORITY_DEFAULT; /* Use the default priority */
	inthSetup_txrx.sense    = CSL_INTC_SENSE_FALL_EDGE; /* Triggers on falling edge */
	inthSetup_txrx.type     = CSL_INTC_TYPE_FIQ; /* In OMAP59120, FIQ is the only type of routing supported. */

	/* Set up interrupt controller for receive and transmit side */
	CSL_intcHwSetup (hIntc_tx3, &inthSetup_txrx);
	CSL_intcHwSetup (hIntc_rx3, &inthSetup_txrx);

    /* Function Address w/ Argument for the Interrupt Handler */
	isrRec_tx.handler = isr_tx3;
	isrRec_rx.handler = isr_rx3;

	/* Argument to be used in the Interrupt Service Routine */
	dmaEventId_tx = hIntc_tx3->eventId ;
	isrRec_tx.arg = (CSL_IntcEventId *) &dmaEventId_tx;
	CSL_intcPlugEventHandler(hIntc_tx3, &isrRec_tx);
	CSL_intcEventEnable(dmaEventId_tx, &state_txrx);

	dmaEventId_rx = hIntc_rx3->eventId ;
	isrRec_rx.arg = (CSL_IntcEventId *) &dmaEventId_rx;
	CSL_intcPlugEventHandler(hIntc_rx3, &isrRec_rx);
	CSL_intcEventEnable(dmaEventId_rx, &state_txrx);
#endif


}



/** Configures the interrupt handler module to handle DMA interrupt. 
 *  Configures channel interrupt by getting event ID using API CSL_dmaGetChipCtxt(...) 
 */  
void configIntHandler1(CSL_IntcHandle hIntc_xmit, CSL_IntcHandle hIntc_rcv ) {

	CSL_IntcHwSetup           inthSetup;
	CSL_IntcGlobalEnableState state;
	CSL_IntcEventHandlerRecord isrRec_xmit, isrRec_rcv;
	CSL_IntcEventId 		dmaEventId_xmit, dmaEventId_rcv ;

	/* Hardware settings for the Interrupt Handler module */
	inthSetup.priority = CSL_INTC_PRIORITY_DEFAULT; /* Use the default priority */
	inthSetup.sense    = CSL_INTC_SENSE_FALL_EDGE; /* Triggers on falling edge */
	inthSetup.type     = CSL_INTC_TYPE_FIQ; /* In OMAP59120, FIQ is the only type of routing supported. */

	/* Set up interrupt controller for receive and transmit side */
	CSL_intcHwSetup (hIntc_xmit, &inthSetup);
	CSL_intcHwSetup (hIntc_rcv, &inthSetup);

    /* Function Address w/ Argument for the Interrupt Handler */
	isrRec_xmit.handler = isr_xmit1;
	isrRec_rcv.handler = isr_rcv1;

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

void configIntHandler3(CSL_IntcHandle hIntc_xmit, CSL_IntcHandle hIntc_rcv ) {

	CSL_IntcHwSetup           inthSetup;
	CSL_IntcGlobalEnableState state;
	CSL_IntcEventHandlerRecord isrRec_xmit, isrRec_rcv;
	CSL_IntcEventId 		dmaEventId_xmit, dmaEventId_rcv ;

	/* Hardware settings for the Interrupt Handler module */
	inthSetup.priority = CSL_INTC_PRIORITY_DEFAULT; /* Use the default priority */
	inthSetup.sense    = CSL_INTC_SENSE_FALL_EDGE; /* Triggers on falling edge */
	inthSetup.type     = CSL_INTC_TYPE_FIQ; /* In OMAP59120, FIQ is the only type of routing supported. */

	/* Set up interrupt controller for receive and transmit side */
	CSL_intcHwSetup (hIntc_xmit, &inthSetup);
	CSL_intcHwSetup (hIntc_rcv, &inthSetup);

    /* Function Address w/ Argument for the Interrupt Handler */
	isrRec_xmit.handler = isr_xmit3;
	isrRec_rcv.handler = isr_rcv3;

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

