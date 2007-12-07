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
 *  This file shows an example of forwarding messages from the GPP
 *  to the appropriate thread using MSGQ. 
 */
#include <std.h> 

// Reference Framework includes
#include <utl.h>
#include <msgq.h>

// application includes
#include "appThreads.h"     // thread-wide common info 
#include "appMsgqConfig.h"  // MSGQ configuration info
#include "thrControl.h"

// Thread includes
#include "process0/thrProcess0.h"   // contains thread specific information
#include "process1/thrProcess1.h"   // contains thread specific information

/*
 *  Thread Control object which encapsulates the state information 
 *  of the thread.
 */ 
ThrControl thrControl;

/*
 *  ======== thrControlInit ========
 *
 */
Void thrControlInit()
{
    MSGQ_Attrs msgqAttrs;

    /* 
     *  Create the receiving message queue. Each reader creates its own
     *  message queue. There is one queue per task.           
     */ 
        
    SEM_new(&thrControl.msgqSemObj, 0);
    
    // Create the message queue to receive control messages from GPP.
    msgqAttrs              = MSGQ_ATTRS;
    msgqAttrs.notifyHandle = (Ptr)&thrControl.msgqSemObj;
    msgqAttrs.pend         = (MSGQ_Pend)binarySemPend;
    msgqAttrs.post         = (MSGQ_Post)SEM_post;
         
    thrControl.msgQueue = MSGQ_create("DSPLINK_DSP00MSGQ00", &msgqAttrs);
    UTL_assert( thrControl.msgQueue != NULL );
}

/*
 *  ======== thrControlStartup ========
 *
 */
Void thrControlStartup()
{    
    /*
     *  Nothing to do here.  Assuming the algorithms/thrProcessX
     *  have initial start-up states.       
     */     
}

/*
 *  ======== thrControlRun ========
 *
 *  Main function of Control task.
 */
Void thrControlRun()
{    
    CtrlMsg *msg;
    MSGQ_Status status;
    MSGQ_Handle thrProcess0Queue, thrProcess1Queue;
    MSGQ_Handle dstMsgQueue;
     
    /* 
     * Locate the Processing Tasks' message queue handles 
     * Note that we are using the blocking (synchronous) mode of the 
     * MSGQ_locate API
     */
    status = MSGQ_locate("msgqProcess0", &thrProcess0Queue, NULL);      
    UTL_assert(status == MSGQ_SUCCESS);
    status = MSGQ_locate("msgqProcess1", &thrProcess1Queue, NULL);      
    UTL_assert(status == MSGQ_SUCCESS);

    // Main loop    
    while ( TRUE ) {
    
        msg = (CtrlMsg *)MSGQ_get(thrControl.msgQueue, SYS_FOREVER );
        UTL_assert(msg != NULL);  //MSGQ internal error

        /* 
         *  Forward coefficient change messages to thrProcess0,
         *  and forward volume messages to thrProcess1. Note that
         *  we do not change the volume on thrProcess0. The user may
         *  modify this code to act differently if so desired. 
         */
        // Make sure command is valid
        UTL_assert( (msg->cmd == MSGNEWCOEFF) || (msg->cmd == MSGNEWVOL) );

        // select which process thread's message queue to put the msg on
        if (msg->cmd == MSGNEWCOEFF) {
            dstMsgQueue = thrProcess0Queue;
        }
        else if (msg->cmd == MSGNEWVOL) {
            dstMsgQueue = thrProcess1Queue;
        }
        else {
            UTL_logDebug("Dropped invalid message.");
            continue;
        }

        // Send the message           
        status = MSGQ_put(dstMsgQueue,     /* Destination message queue */
            (MSGQ_Msg)msg,                 /* The message        */
            CTRLMSGID,                     /* Message Id         */
            NULL);                         /* Reply message queue       */
        UTL_assert(status == MSGQ_SUCCESS);
    }
}

