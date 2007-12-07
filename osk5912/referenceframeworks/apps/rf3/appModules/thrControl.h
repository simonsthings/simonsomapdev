/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrControl.h ========
 *
 *  Declaration of resources for the control thread
 *
 *  The purpose of the control thread is to detect hardware events such
 *  as user's presses on device's buttons and other controls, and apply
 *  them to data processing. (For example, changing the volume, modifying
 *  filter parameters, cancelling a channel etc.)
 *  
 *  The mechanism is the following: a hardware event such as button press
 *  trigers an interrupt, serviced by thrControlIsr() procedure below.
 *  That procedure quickly reads the hardware parameters, interprets them,
 *  stores the result of the interpretation in thrControl thread's data
 *  structure, posts the swiControl thread and exits. It does not modify
 *  any of the data processing parameters itself.
 *  
 *  In our example the host writes into an area of memory that simulates
 *  I/O area where the buttons and other controls are, and we have 
 *  a clkControl object that runs thrControlIsr() function; that is a
 *  timer interrupt which simulates a device control interrupt -- this
 *  we use if we develop and test applications on a board such as
 *  DSK5402, where no buttons are connected to any interrupt lines.
 */
#ifndef THRCONTROL_
#define THRCONTROL_

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Control thread's data structure: information about hardware events
 *  that caused the thread to be posted. Function thrControlIsr() writes
 *  interpreted I/O data into this structure, and thrControlRun() takes
 *  action based on the information in this structure. 
 *  There is no need for this data to be grouped in a structure except
 *  for increased readability, in case the ISR and the thread function
 *  are stored in separate files. There's no overhead to such grouping.
 */
typedef struct ThrControl {
    /* number of the channel to be used for output */
    Int  outputChannel;     
    
    /* modification of original volume for each channel */
    Int outputVolume[ NUMCHANNELS ];  /* Default: 100, meaning 100% of normal */
} ThrControl;

/*
 *  Declaration of thread data object
 */
extern ThrControl thrControl;

/*
 *  Declaration of "public" thread functions 
 */
/* init function, called from thrInit() */
extern Void thrControlInit( Void );
 
/* SWI run function, called by the swiControl SWI object */
extern Void thrControlRun( Void );

/* HWI interrupt function (in this app called by the clkControl CLK object) */
extern Void thrControlIsr( Void );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* THRCONTROL_ */


