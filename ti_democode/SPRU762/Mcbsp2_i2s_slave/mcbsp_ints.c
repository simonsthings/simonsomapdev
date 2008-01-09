
#include <csl_intc.h>
#include <soc.h>
#include "swi.h"

extern void FsrIsr(void *arg);
extern void FsxIsr(void *arg);

	/* FSR interrupt */
	CSL_Status intcStatusFsr;
	CSL_IntcContext	intcContextFsr = CSL_INTC_CONTEXT_INITVAL;
	CSL_IntcDispatcherContext intcDispatcherContextFsr = CSL_INTC_DISPATCHERCONTEXT_INITVAL;
	CSL_IntcObj intcObjFsr;
    CSL_IntcHwSetup intcHwSetupFsr;
    CSL_IntcEventEnableState intcEventEnableStateFsr;
    CSL_IntcGlobalEnableState intcGlobalEnableStateFsr;
    CSL_IntcEventHandlerRecord intcEventHandlerRecordFsr;
    CSL_IntcHandle intcHndl_Fsr=NULL;
    int fsr_temp = 5;
    
	/* FSX interrupt */
	CSL_Status intcStatusFsx;
	CSL_IntcContext	intcContextFsx = CSL_INTC_CONTEXT_INITVAL;
	CSL_IntcDispatcherContext intcDispatcherContextFsx = CSL_INTC_DISPATCHERCONTEXT_INITVAL;
	CSL_IntcObj intcObjFsx;
    CSL_IntcHwSetup intcHwSetupFsx;
    CSL_IntcEventEnableState intcEventEnableStateFsx;
    CSL_IntcGlobalEnableState intcGlobalEnableStateFsx;
    CSL_IntcEventHandlerRecord intcEventHandlerRecordFsx;
    CSL_IntcHandle intcHndl_Fsx=NULL;
    int fsx_temp = 7;

void mcbsp_ints(){

//////////////// FSR INTERRUPT SETUP //////////////////
	CSL_intcInit(&intcContextFsr);
	CSL_intcDispatcherInit(&intcDispatcherContextFsr);
	intcHndl_Fsr = CSL_intcOpen(&intcObjFsr,
								CSL_INTC_EVENTID_MCBSP2RX,
								NULL,&intcStatusFsr);
	

   intcEventHandlerRecordFsr.handler = FsrIsr;
   intcEventHandlerRecordFsr.arg = (void *)&fsr_temp;

   intcHwSetupFsr.priority=CSL_INTC_PRIORITY_DEFAULT;
   intcHwSetupFsr.type=CSL_INTC_TYPE_IRQ;
   intcHwSetupFsr.sense=CSL_INTC_SENSE_FALL_EDGE;
	
   /* Interrupt Controller Setup */
   CSL_intcHwSetup(intcHndl_Fsr,&intcHwSetupFsr);

   /* Plug an interrupt handler into the dispatcher */
   CSL_intcPlugEventHandler(intcHndl_Fsr,&intcEventHandlerRecordFsr);

   /* Enable the FSR Interrupt */
   CSL_intcEventEnable(CSL_INTC_EVENTID_MCBSP2RX,
   						&intcEventEnableStateFsr);

   /* Switching to system mode */
   SWI_sysMode();

   /* Enable the ARM IRQ interrupt */
   CSL_intcGlobalEnable(&intcGlobalEnableStateFsr);

////////////////////////////////////////////////////////////

//////////////// FSX INTERRUPT SETUP //////////////////
	CSL_intcInit(&intcContextFsx);
	CSL_intcDispatcherInit(&intcDispatcherContextFsx);
	intcHndl_Fsx = CSL_intcOpen(&intcObjFsx,
								CSL_INTC_EVENTID_MCBSP2TX,
								NULL,&intcStatusFsx);
	

   intcEventHandlerRecordFsx.handler = FsxIsr;
   intcEventHandlerRecordFsx.arg = (void *)&fsx_temp;

   intcHwSetupFsx.priority=CSL_INTC_PRIORITY_DEFAULT;
   intcHwSetupFsx.type=CSL_INTC_TYPE_IRQ;
   intcHwSetupFsx.sense=CSL_INTC_SENSE_FALL_EDGE;
	
   /* Interrupt Controller Setup */
   CSL_intcHwSetup(intcHndl_Fsx,&intcHwSetupFsx);

   /* Plug an interrupt handler into the dispatcher */
   CSL_intcPlugEventHandler(intcHndl_Fsx,&intcEventHandlerRecordFsx);

   /* Enable the FSX Interrupt */
   CSL_intcEventEnable(CSL_INTC_EVENTID_MCBSP2TX,
   						&intcEventEnableStateFsx);

   /* Switching to system mode */
   SWI_sysMode();

   /* Enable the ARM IRQ interrupt */
   CSL_intcGlobalEnable(&intcGlobalEnableStateFsx);
////////////////////////////////////////////////////////////
}
