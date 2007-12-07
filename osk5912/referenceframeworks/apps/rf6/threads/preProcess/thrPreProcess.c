/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrPreProcess.c ========
 *
 *  Implementation of the PreProcess thread.  
 *  This thread is a typical data preprocessing thread. It gets data
 *  from either a DSP/BIOS Link channel or a codec driver and splits 
 *  the audio left and right data into 2 separate streams on a buffer-by- 
 *  buffer basis. By defining the input device and the MSGQ queues used 
 *  by this thread, the code can be reused for both data paths in RF6.
 */

#include <std.h>

// DSP/BIOS includes
#include <sem.h>
#include <sio.h>

// RF module includes
#include <msgq.h>
#include <utl.h>

// application includes
#include "appResources.h"   // application-wide common info 
#include "appThreads.h"     // thread-wide common info 

// Thread includes
#include "thrPreProcess.h"

/*
 *  ======== thrPreProcessInit ========
 *
 */
Void thrPreProcessInit( ThrPreProcess *thr ) 
{
    SIO_Attrs  sioAttrs;
    MSGQ_Attrs msgqAttrs;
        
    sioAttrs       = SIO_ATTRS;    
    sioAttrs.model = SIO_ISSUERECLAIM;
    sioAttrs.nbufs = thr->numPrimingBuffers;
    
    // create the input stream
    thr->inStream = SIO_create( thr->inStreamName, SIO_INPUT,
        thr->numChansFromDev * FRAMELEN * sizeof( Sample ), &sioAttrs );
    UTL_assert( thr->inStream != NULL );

    /* 
     *  Create the receiving message queue. Each reader creates its own
     *   message queue. There is one queue per task.           
     */    
    SEM_new(&thr->msgqSemObj, 0);

    msgqAttrs              = MSGQ_ATTRS;
    msgqAttrs.notifyHandle = (Ptr)&thr->msgqSemObj;
    msgqAttrs.pend         = (MSGQ_Pend)binarySemPend;
    msgqAttrs.post         = (MSGQ_Post)SEM_post;
    
    thr->msgQueue = MSGQ_create(thr->rxMsgqName, &msgqAttrs);
    UTL_assert( thr->msgQueue != NULL );
}

/*
 *  ======== thrPreProcessStartup ========
 *
 */
Void thrPreProcessStartup( ThrPreProcess *thr, Sample *bufRx[] )
{
    Int rc;
    Int i;

    // issue buffers to the input stream i.e. do the 'priming'
    for (i = 0; i < thr->numPrimingBuffers; i++) {
        rc = SIO_issue( thr->inStream, bufRx[i], 
                        SIO_bufsize( thr->inStream ), NULL );
        UTL_assert( rc == SYS_OK );
    }
}

/*
 *  ======== thrPreProcessRun ========
 *
 *  Main function of PreProcess task.
 */
Void thrPreProcessRun(Arg thrArg)
{
    MSGQ_Handle msgqSend;    
    MSGQ_Status status;
    
    ThrPreProcess *thr = (ThrPreProcess *)ArgToPtr( thrArg );
    
    /* 
     * Locate the Processing Task's message queue handle 
     * Note that we are using the blocking (synchronous) mode of the 
     * MSGQ_locate API
     */
    status = MSGQ_locate(thr->txMsgqName, &msgqSend, NULL);     
    UTL_assert (status == MSGQ_SUCCESS);
    
    while (TRUE) {
        LocalDataMsg    *msg;
        Sample          *bufDst, *bufSrc;
        Int              i;
        Uns              chanNum;
        Uns              deviceChanNum = 0;

        // Reclaim full source buffer from the input stream
        SIO_reclaim( thr->inStream, (Ptr *)&bufSrc, NULL );        

        /*
         * Get the structure describing destination channels. We are assuming 
         * that the type of the message received is always going to be correct.
         */
        msg = (LocalDataMsg *)MSGQ_get(thr->msgQueue, SYS_FOREVER);
        UTL_assert(msg != NULL);  //MSGQ internal error
        
        /* 
         * split the input data: 
         * For stereo codec: LRLRLRLR -> LL..L,RR..R,LL..L,RR..R
         * For mono codec:   DDDD...D -> DD..D,DD..D,DD..D,DD..D
         */
        for ( chanNum = 0; chanNum < NUMCHANNELS; chanNum++ ) {
            bufDst = msg->bufChannel[ chanNum ];
            for (i = 0; i < FRAMELEN; i++) {
                bufDst[i] = bufSrc[ thr->numChansFromDev * i + deviceChanNum ];
            }
            deviceChanNum = (deviceChanNum + 1) % thr->numChansFromDev;
        }

        // put the full buffer to the queue of processing task
        status = MSGQ_put(msgqSend, (MSGQ_Msg)msg, PREPROCESS_DATAMSGID, NULL);
        UTL_assert( status == MSGQ_SUCCESS );
        
        // Issue an empty buffer to the input stream
        SIO_issue( thr->inStream, bufSrc, SIO_bufsize( thr->inStream ), NULL );
    }
}

