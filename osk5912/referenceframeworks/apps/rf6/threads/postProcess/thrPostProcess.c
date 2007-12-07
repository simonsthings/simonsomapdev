/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrPostProcess.c ========
 *
 *  Implementation of the PostProcess thread.  
 *  This thread is a typical data postprocessing thread. It joins 
 *  the audio data into a single stream with alternating left-and-right
 *  samples a buffer-by-buffer basis, then sends the data to either a 
 *  DSP/BIOS Link channel or a codec driver. By defining the output device  
 *  and the MSGQ queues used by this thread, the code can be reused for
 *  both data paths in RF6.
 */

#include <std.h>
#include <string.h>

// DSP/BIOS includes
#include <sem.h>
#include <sio.h>

// RF module includes
#include <msgq.h>
#include <utl.h>

// application includes
#include "appMsgqConfig.h"  // MSGQ configuration info
#include "appResources.h"   // application-wide common info
#include "appThreads.h"     // thread-wide common info 

// Thread includes
#include "thrPostProcess.h"

/*
 *  ======== thrPostProcessInit ========
 *
 */
Void thrPostProcessInit( ThrPostProcess *thr )
{
    SIO_Attrs  sioAttrs;
    MSGQ_Attrs msgqAttrs;

    sioAttrs       = SIO_ATTRS;
    sioAttrs.model = SIO_ISSUERECLAIM;
    sioAttrs.nbufs = thr->numPrimingBuffers;
 
    // create the output stream 
    thr->outStream = SIO_create( thr->outStreamName, SIO_OUTPUT,
        thr->numChansToDev * FRAMELEN * sizeof( Sample ), &sioAttrs );
    UTL_assert( thr->outStream != NULL );

    /* 
     *  Create the receiving message queue. Each reader creates its own
     *  message queue. There is one queue per task.     
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
 *  ======== thrPostProcessStartup ========
 *
 */
Void thrPostProcessStartup( ThrPostProcess *thr, Sample *bufTx[] )
{
    Int rc;
    Int i;
    
    // issue buffers to the output stream i.e. do the 'priming'
    for (i = 0; i < thr->numPrimingBuffers; i++) {
        rc = SIO_issue( thr->outStream, bufTx[i], 
                        SIO_bufsize( thr->outStream ), NULL ); 
        UTL_assert( rc == SYS_OK );
    }
}

/*
 *  ======== thrPostProcessRun ========
 *
 *  Main function of PostProcess task.
 */
Void thrPostProcessRun(Arg thrArg)
{
    Int i;
    MSGQ_Handle  msgqSend;    
    MSGQ_Status  status;
    LocalDataMsg *msg;
    
    ThrPostProcess *thr = (ThrPostProcess *)ArgToPtr( thrArg );

    /* 
     * Locate the Processing Task's message queue handle 
     * Note that we are using the blocking (synchronous) mode of the 
     * MSGQ_locate API
     */
    status = MSGQ_locate(thr->txMsgqName, &msgqSend, NULL);     
    UTL_assert (status == MSGQ_SUCCESS);

    // Fill in the priming message
    msg = (LocalDataMsg *)MSGQ_alloc(LOCALMQABUFID, sizeof(LocalDataMsg));
    UTL_assert( msg != NULL );
    for (i = 0; i < NUMCHANNELS; i++) {
        msg->bufChannel[i] = thr->bufInput[i];
    }

    /*
     *  Prime the MSGQ of the preceding task in the processing chain.
     *  The convention here is for the task appearing later in the data flow
     *  to prime the MSGQ of the task(s) preceding it.
     */ 
    status = MSGQ_put(msgqSend, (MSGQ_Msg)msg, POSTPROCESS_DATAMSGID, NULL);
    UTL_assert( status == MSGQ_SUCCESS );
            
    while (TRUE) {
        Sample   *bufDst, *bufSrc;        
        Int      i;
        Int      deviceChanNum;         

        // Reclaim empty destination buffer from the output link stream
        SIO_reclaim( thr->outStream, (Ptr *)&bufDst, NULL );         

        /*
         * Get the structure describing source channels. We are assuming that
         * the type of the message received is always going to be correct.
         */
        msg = (LocalDataMsg *)MSGQ_get(thr->msgQueue, SYS_FOREVER);
        UTL_assert(msg != NULL);  //MSGQ internal error
        
        // join the input data.
        for ( deviceChanNum = 0; deviceChanNum < thr->numChansToDev; 
              deviceChanNum++ ) {

            /*
             *  Select the appropriate processed buffer:
             *  Examples:
             *  Case 1 device channels < NUMCHANNELS
             *     you processed stereo data and sending to a mono-codec,
             *     only send the first channel of the stereo data. 
             *
             *  Case 2 device channels > NUMCHANNELS
             *     you processed mono data and sending to a stereo-codec,
             *     replicated the mono data into both stereo channels. 
             *  
             *  Case 3 device channels = NUMCHANNELS
             *     the stereo processed data goes into the stereo channels.
             */
            bufSrc = msg->bufChannel[ deviceChanNum % NUMCHANNELS  ];
 
            // interleave the data.
            for (i = 0; i < FRAMELEN; i++) {
                bufDst[ thr->numChansToDev * i + deviceChanNum ] = bufSrc[i];
            }
        }

        // send the message describing consumed input buffers to Process
        status = MSGQ_put( msgqSend, (MSGQ_Msg)msg, 
                           POSTPROCESS_DATAMSGID, NULL );
        UTL_assert( status == MSGQ_SUCCESS );
        
        // issue full destination buffer to the output link stream
        SIO_issue( thr->outStream, bufDst, SIO_bufsize( thr->outStream ), 
                   NULL );
    }
}

