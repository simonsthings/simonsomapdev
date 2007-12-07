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
 *  This file shows an example of sending messages from one thread to the
 *  other using mailbox. 
 */
#include <std.h>

// DSP/BIOS includes
#include <mbx.h>
#include <tsk.h>

// RF5 library includes
#include <utl.h>

// application includes
#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */
#include "thrControl.h"

#include "process/thrProcess.h"     /* contains thread specific information */

/* global structure to which the host writes control values (via GEL script) */
typedef struct ExternalControl {
    Int volGain [ NUMCHANNELS ];
    Int firCoeff[ NUMCHANNELS ];
} ExternalControl;

ExternalControl externalControl;        // GEL script writes here
ExternalControl externalControlPrev;    // this is a local copy 

/*
 *  ======== thrControlInit ========
 *
 */
Void thrControlInit()
{
    Int chanNum;
    
    // set the default gain to 100%
    for( chanNum = 0; chanNum < NUMCHANNELS; chanNum++ ) {
        externalControl.volGain[ chanNum ] = 100;
        externalControl.firCoeff[ chanNum ] = 0;
    }
    
    // Set the previous state to the initial values.
    externalControlPrev = externalControl;
}

/*
 *  ======== thrControlStartup ========
 *
 */
Void thrControlStartup()
{    
    Int chanNum;
    CtrlMsg txMsg;
    
    // send message to initialize the volume & fir for all channels
    for( chanNum = 0; chanNum < NUMCHANNELS; chanNum++) {
        
        // Set-up initial values for Volume
        txMsg.cmd  = MSGNEWVOL;
        txMsg.arg1 = chanNum;
        txMsg.arg2 = externalControl.volGain[ chanNum ];
        
        // Send the request.
        MBX_post( mbxProcess, &txMsg, 0 );

        // Set-up initial filter coefficients
        txMsg.cmd  = MSGNEWCOEFF;
        txMsg.arg1 = chanNum;
        txMsg.arg2 = externalControl.firCoeff[ chanNum ];
        
        // Send the request.
        MBX_post( mbxProcess, &txMsg, 0 );
    }
}

/*
 *  ======== thrControlRun ========
 *
 *  Main function of Control task.
 */
Void thrControlRun()
{
    Int chanNum;
    CtrlMsg txMsg;
    
    // Main loop
    while (TRUE) {
    
        // check for changes
        for (chanNum = 0; chanNum < NUMCHANNELS; chanNum++) {

            // See if user requested a volume change.
            if( externalControl.volGain[ chanNum ] != 
                externalControlPrev.volGain[ chanNum ] ) {
                    
                // new value entered
                externalControlPrev.volGain[ chanNum ] = 
                    externalControl.volGain[ chanNum ];
                    
                // send message to tell which channel has volume changed
                txMsg.cmd  = MSGNEWVOL;
                txMsg.arg1 = chanNum;
                txMsg.arg2 = externalControl.volGain[ chanNum ];
                MBX_post( mbxProcess, &txMsg, 0 );
            }
            
            // See if user requested different coefficients.
            if( externalControl.firCoeff[ chanNum ] != 
                externalControlPrev.firCoeff[ chanNum ] ) {
            
                // new value entered
                externalControlPrev.firCoeff[ chanNum ] = 
                    externalControl.firCoeff[ chanNum ];
                    
                // send message to tell which channel has coeff changed
                txMsg.cmd  = MSGNEWCOEFF;
                txMsg.arg1 = chanNum;
                txMsg.arg2 = externalControl.firCoeff[ chanNum ];
                MBX_post( mbxProcess, &txMsg, 0 );
            }
        }
        
        // suspend self for 100 ticks, and then poll again
        TSK_sleep( 100 );
    }
}

