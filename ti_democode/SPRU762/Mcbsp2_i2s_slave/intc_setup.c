
#include <soc.h>
#include <csl_intc.h>
#include "swi.h"

/** This function setups the Interrupt Controller
 *
 *  The function does the following steps :-
 *  -# Initializes the parameters of the @a #intcHwSetup structure
 *  -# Calls the Interrupt Controller Setup which sets up the
 *     interrupt controller for the Channel0 System DMA interrupt
 *  -# Plugs the interrupt handler for the System DMA channel 0
 *     into the dispatcher
 *  -# Enable the System DMA channel 0 interrupt
 *  -# Switches the ARM Core to System Mode so that the
 *     the ARM's IRQ can be enabled.
 *  -# Enables the ARM IRQ interrupt
 *
 */
int intcSetup(CSL_IntcHandle intcHndl,CSL_IntcEventHandlerRecord *intcEventHandlerRecordptr)
{

   CSL_IntcHwSetup intcHwSetup;
   CSL_IntcEventEnableState intcEventEnableState;
   CSL_IntcGlobalEnableState intcGlobalEnableState;

   intcHwSetup.priority=CSL_INTC_PRIORITY_DEFAULT;
   intcHwSetup.type=CSL_INTC_TYPE_IRQ;
   intcHwSetup.sense=CSL_INTC_SENSE_ACTIVE_LOW;

   /* Interrupt Controller Setup */
   CSL_intcHwSetup(intcHndl,&intcHwSetup);

   /* Plug an interrupt handler into the dispatcher */
   CSL_intcPlugEventHandler(intcHndl,intcEventHandlerRecordptr);

   /* Enable the Channel0 Interrupt */
   CSL_intcEventEnable(CSL_INTC_EVENTID_DMACH0,&intcEventEnableState);
   /* Enable the Channel1 Interrupt */
   CSL_intcEventEnable(CSL_INTC_EVENTID_DMACH1,&intcEventEnableState);

   /* Switching to system mode */
   SWI_sysMode();

   /* Enable the ARM IRQ interrupt */
   CSL_intcGlobalEnable(&intcGlobalEnableState);

   return 1;
}
