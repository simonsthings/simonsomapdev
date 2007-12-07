/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrControl.c ========
 *
 *  Procedures for the control ISR and the control thread
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
 *  Thread swiControl is a SWI like the processing SWIs, and has exactly
 *  the same priority; it executes thread function thrControlRun(). 
 *  Function thrControlRun(), based on the data presented by thrControlIsr(),
 *  modified processing thread's data and/or XDAIS algorithms' parameters.
 *
 *  The reason we encapsulate processing parameters modification logic 
 *  in a separate SWI thread is twofold:
 *  
 *  1) We want to keep the time spent in a hardware interrupt at a 
 *     minimum;
 *  2) thrControlIsr() most likely interrupted a data processing thread;
 *     we don't want to modify processing parameters in the middle of
 *     processing activity.
 *     
 *  By having swiControl have the same priority as the processing threads,
 *  we ensure that it does not prevent them, nor gets starved by them,
 *  so it has a low latency. Thread swiControl should thus have the 
 *  priority of the lowest-priority thread whose data parameters it
 *  modifies.
 *  
 *  An alternative approach may be to check the device controls every
 *  certain period (for example, 20 ms), if there's no interrupt that
 *  would inform us about the hardware event. This choice depends
 *  on the application and the device.
 *  
 *  In our example the host writes into an area of memory that simulates
 *  I/O area where the buttons and other controls are, and we have 
 *  a clkControl object that runs thrControlIsr() function; that is a
 *  timer interrupt which simulates a device control interrupt -- this
 *  we use if we develop and test applications on a board such as
 *  DSK5402, where no buttons are connected to any interrupt lines.
 */
#include <std.h>
#include <hwi.h>
#include <swi.h>

#include <utl.h>            /* debug/diagnostics utility functions */

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */
#include "thrAudioproc.h"   /* definition of thrAudioproc thread data */
#include "thrControl.h"     /* definition of thrControl thread data */
#include "thrTxJoin.h"      /* definition of thrTxJoin thread data */

/*
 *  Above we included header files with data structure definitions
 *  of all the threads whose data we may modify. This is the only 
 *  procedure in the system that should attempt to write to other
 *  threads' private data.
 */

/*
 *  Static initialization
 */

/*
 *  We simulate reading the device's controls by having an area
 *  in memory that we will read whenever this function is posted to
 *  run, and the host will write to it using a GEL script. This simulates
 *  an I/O area where values of buttons and sliders can be read.
 *  The map of the area is the following:
 *  
 *  Word 0: flag indicating whether new values have been written by the
 *          user. The GEL script, when changing anything, sets this to
 *          TRUE (1), and the control ISR, if finds this to be TRUE,
 *          reads all the other values and then sets this flag to FALSE;
 *          otherwise (if it is already FALSE) it knows no new user
 *          input has been made and therefore takes no action
 *  Word 1: number of the channel whose output should be sent to the codec
 *  Word 2: ch#0 volume gain; 0-200; 100 normal 
 *  Word 3: ch#1 0:filter disabled, 1:enabled 
 *  etc. Host's GEL file writes to this array directly.
 *
 *  Note: this array and the first if() clause in thrControlIsr() are
 *  the only portions of code that are here due to the fact that in this
 *  application we simulate device buttons from a GEL file on the host.
 */
Int deviceControlsIOArea[] = {
    FALSE,   /* initially, no user action */
    0,       /* default active channel */
    100,     /* default volume for channel #0 */
    100,     /* default volume for channel #1 */
};

/* 
 * Control thread's data structure. (We don't have to initialize it since
 * control ISR will write to it before posting the thread. However, if it
 * had some other, persistent data, we could initialize it here.)
 */
ThrControl thrControl;

/*
 *  ========= thrControlIsr ========
 *  Interrupt Service Routine for the Control thread
 * 
 *  reads the values of buttons/knobs from the device, stores the
 *  interpreted information in control thread's data structure and
 *  posts the thread
 */
Void thrControlIsr( Void ) {
    Int i;
    static Uns activationCount = 0;
    
    /* 
     *  We are really called from the CLK object upon every timer interrupt
     *  whereas user's action would occur after relatively long intervals,
     *  so we try to simulate that, too. Since interrupts occur every
     *  1ms (based on the BIOS config), we arbitrarily decide to actually
     *  do anything in this procedure every 20 interrupts, i.e. every 20 ms. 
     *  If the if() clause below is removed, then the response to the user's
     *  action could happen in one millisecond. That would be the case with
     *  control ISR activated by a separate interrupt line. Such ISR would 
     *  not need the if() clause below.
     */
    if (++activationCount < 20) {   /* 20 ms */
        return;
    } else {
        activationCount = 0;
    }
    
    /* now proceed with regular "I/O memory" reading actions */
    
    /* check if there has been any unread user input */
    if (deviceControlsIOArea[0] == FALSE) {
        return;     /* there has not; return */
    }

    /* read "channel selection" switch */
    thrControl.outputChannel = deviceControlsIOArea[1];
    
    /* 
     *  Read "volume" value for all channels and store
     *  the information in control thread's data structure. Interpretation
     *  is trivial in this case, we just copy host's data to control thread's
     *  data structure.
     */
    for (i = 0; i < NUMCHANNELS; i++) {
        /* read "slider" for volume */
        thrControl.outputVolume[i] = deviceControlsIOArea[ 2 + i ];
    }
    
    /* now post the control thread */
    SWI_post( &swiControl );
    
    /* and clear the "user input" flag */
    deviceControlsIOArea[0] = FALSE;
}


/*
 *  ========= thrControlInit ========
 *  Initialization of data structures for the thread, called from 
 *  appThreads.c:thrInit() at init time.
 */
Void thrControlInit( Void ) 
{
}
    
/*
 *  ========= thrControlRun ========
 *  The body of the control thread (SWI).
 * 
 *  Reads the channel selection/filter enable/volume values 
 *  and applies them to data processing 
 */
Void thrControlRun( Void ) 
{
    Int i;
    Uns interruptState;
    /* a local copy of all thrControlIsr-writable variables: */
    Int outputChannel;
    Int outputVolume[ NUMCHANNELS ];
    
    /* 
     *  Disable the control ISR, make a local copy of all the variables
     *  that the conrol ISR can write to, and restore interrupts. After
     *  that we will use local copies only; that is to prevent the ISR
     *  partially overwriting our current set of values (i.e. to prevent
     *  race conditions).
     */
    interruptState = HWI_disable();
    outputChannel = thrControl.outputChannel;
    for (i = 0; i < NUMCHANNELS; i++) {
        outputVolume[i] = thrControl.outputVolume[i];
    }
    HWI_restore( interruptState );
     
    /* now use the local copies to change threads' parameters */

    /* Instruct thrTxJoin thread which channel to output */
    thrTxJoinSetOutputChannel( outputChannel );
    
    /* set the new volume value for all the channels */
    for (i = 0; i < NUMCHANNELS; ++i) {
        thrAudioprocSetOutputVolume( i, outputVolume[i] );
    }
}

