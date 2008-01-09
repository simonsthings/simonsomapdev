#include <csl_intc.h>
#include <soc.h>

extern CSL_IntcObj intcObjXmit;
extern CSL_IntcObj intcObjRcv;

CSL_IntcContext			intcContextXmit = CSL_INTC_CONTEXT_INITVAL;
CSL_IntcContext			intcContextRcv = CSL_INTC_CONTEXT_INITVAL;
CSL_IntcDispatcherContext 	intcDispatcherContextXmit = CSL_INTC_DISPATCHERCONTEXT_INITVAL;
CSL_IntcDispatcherContext 	intcDispatcherContextRcv = CSL_INTC_DISPATCHERCONTEXT_INITVAL;

/** This function initialiazes the ARM interrupt controller
 *
 *  The function does the following steps :-
 *  -# Initializes the interrupt controller module
 *  -# Initializes the interrupt controller dispatcher
 *
 *  @return returns an int: always 1
 */
int intcInit(CSL_IntcHandle *intcHndlptrXmit, CSL_IntcHandle *intcHndlptrRcv)
{
   CSL_Status intcStatusXmit;
   CSL_Status intcStatusRcv;

   /* Interrupt Controller Init */
   CSL_intcInit(&intcContextXmit);
   CSL_intcInit(&intcContextRcv);

   /* Interrupt Controller Dispatcher Init */
   CSL_intcDispatcherInit(&intcDispatcherContextXmit);
   CSL_intcDispatcherInit(&intcDispatcherContextRcv);

   /* Interrupt Controller Init which sets up the properties for the DMA Channel 0 interrupt pin */
   *intcHndlptrXmit = CSL_intcOpen(&intcObjXmit,CSL_INTC_EVENTID_DMACH0,NULL,&intcStatusXmit);
   
   /* Interrupt Controller Init which sets up the properties for the DMA Channel 1 interrupt pin */
   *intcHndlptrRcv = CSL_intcOpen(&intcObjRcv,CSL_INTC_EVENTID_DMACH1,NULL,&intcStatusRcv);

   return 1;
}
