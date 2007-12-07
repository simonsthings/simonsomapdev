/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrTxJoin.c ========
 *
 *  Runtime procedure for thread TxJoin.
 *
 *  If our codec is a stereo codec, the output pipe, pipTx, 
 *  should contain mixed left and right output samples that
 *  the DMA would deliver to the codec.
 *
 *  Since we process channels individually, each channel produces a frame
 *  with only its own samples; that should now be joined in a 
 *  single output pipe frame (with size twice the size of each input frame)
 *
 *  pipTx0 (size=80)    channel #0
 *  LLLLLL...L -----+ 
 *                  +--> LRLRLRLRLR...LR pipTx (size=2x80)
 *  RRRRRR...R -----+ 
 *  pipTx1 (size=80)    channel #1
 *
 *  If we are using a board with a mono codec for development, such as 
 *  bare DSK5402 with its AD50 codec, however, pipTx has the
 *  same size as pipTx0, pipTx1 and we really just pick one of the
 *  two channels (the user determines which one) and copy to pipTx,
 *  and we discard the other one.
 *
 *  In general case, for separate frames sized FS of samples from N different
 *  channels, we would join them in one block in the following manner:
 *  (symbol "M" is N-1, introduced for picture clarity)
 *
 *  channel #1: pipTx0 (size=FS)  000000...0 -----+ 
 *  channel #2: pipTx1 (size=FS)  111111...1      +--> 0123...M0123...M 
 *                                ...             :    pipTx (size=NxFS)
 *  channel #M: pipTxN (size=FS)  MMMMMM...M -----+
 *
 *  Note: "size" here is given in samples, not words; actual pipe frame sizes
 *  in the BIOS config are defined in words. Size in words, where a "word"
 *  is as wide as an "Int", is therefore
 *  <size in words> = <size in samples> * sizeof( Sample ) / sizeof( Int ), or
 *  <size in words> = sizeInWords( <size in samples> )
 *  "Sample" is a typedef defined in appResources.h, and sizeInWords is a macro.
 */
#include <std.h>
#include <pip.h>

#include <utl.h>            /* debug/diagnostics utility functions */

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */
#include "thrTxJoin.h"      /* definition of thrTxJoin object */

/* 
 *  Initialization of the thread resources structure:
 *  Bind internally used pipe pointers to actual pipe objects defined in BIOS
 *  configuration.
 *  outputChannel is a variable defining which of the N channels will be 
 *  active, i.e. sent to the output (the other N-1 are discarded). The feature
 *  is used only if our platform has a mono codec, such as bare-bones DSK5402,
 *  where we simulate a stereo codec (or, in general, an N-channel codec) 
 *  and the variable is set by the control thread.
 */
ThrTxJoin thrTxJoin = {
    { &pipTx0, &pipTx1 },   /* pipIn[ NUMCHANNELS ] */
    &pipTx,                 /* pipOut */
    0                       /* outputChannel */
};

/*
 *  ========= thrTxJoinInit ========
 *  Initialization of data structures for the thread, called from 
 *  appThreads.c:thrInit() at init time.
 */
Void thrTxJoinInit( Void ) 
{
}

/*
 *  ========= thrTxJoinRun ========
 *  The "body" of the swiTxJoin thread.
 *
 *  Whenever a new frame is available in pipTx and there is one
 *  ready-to-read frame in both pipTx0 and pipTx1, this thread is posted.
 *
 *  That is so because left and right channels have to be synchronized.
 *  (If the channels were independent, we could output the active one
 *  as soon as it is ready, without waiting for the other channel.)
 */
Void thrTxJoinRun( Void ) {
    Int i, ch;
    Sample *srcBuf, *dstBuf;

    /* make sure the output pipe really has a free frame to write */
    UTL_assert( PIP_getWriterNumFrames( thrTxJoin.pipOut ) > 0 );

    /* 
     *  Record the period between two executions of this point in code.
     *  It should match 1000ms/<sampling rate> * FRAMELEN. We record the
     *  similar value for RxSplitRun in stsTime0 object.
     *  Out of curiosity we record the phase difference between the two
     *  as well, to see how much time passes between the execution of those
     *  two points. In the STS pannel, set stsTime1 unit to be milliseconds,
     *  and the stsTime2 (containing the phase) unit to be microseconds.
     */
    UTL_stsPeriod( stsTime1 );
    UTL_stsPhase ( stsTime0, stsTime1, stsTime2 ); /* [2] = abs([0] - [1]) */
    
    /* allocate an empty buffer from the output pipe */
    PIP_alloc( thrTxJoin.pipOut );
    dstBuf = (Sample *)PIP_getWriterAddr( thrTxJoin.pipOut );

    /* copy samples from input pipes to appropriate places in the output pipe */
    for (ch = 0; ch < NUMCHANNELS; ch++) {

        /* make sure the input (source) pipe really has a full frame */
        UTL_assert( PIP_getReaderNumFrames(thrTxJoin.pipIn[ ch ]) > 0 );
        
        /* get the full buffer from the input pipe for the channel */
        PIP_get( thrTxJoin.pipIn[ ch ] );
        srcBuf = (Sample *)PIP_getReaderAddr( thrTxJoin.pipIn[ ch ] );

        /* copy its samples to appropriate places in the destination frame */
        #ifdef APPMONOCODEC     /* simulation of stereo/N-ch. codec with mono */
        if (ch == thrTxJoin.outputChannel) {
            /* do the copy only if we're processing the active output channel */
            for (i = 0; i < FRAMELEN; i++) {
                /* AD50 and AD535 codecs must have LSB set to 0 */
                dstBuf[i] = srcBuf[i] & 0xfffe; 
            }
        }
        #else /* real stereo or N-channel codec */
        for (i = 0; i < FRAMELEN; i++) {
            dstBuf[ i * NUMCHANNELS + ch ] = srcBuf[i];
        }
        #endif
        
        /* and free the input pipe */
        PIP_free( thrTxJoin.pipIn[ ch ] );
    }

    /* record the amount of actual data stored in the output frame */
    #ifdef APPMONOCODEC
    PIP_setWriterSize( thrTxJoin.pipOut, sizeInWords( FRAMELEN ) );
    #else    
    PIP_setWriterSize( thrTxJoin.pipOut, sizeInWords( FRAMELEN ) * NUMCHANNELS);
    #endif

    /* and give the frame to the driver */
    PIP_put( thrTxJoin.pipOut );
}

/*
 *  ========= thrTxJoinSetOutputChannel ========
 *  Procedure that sets which of the NUMCHANNELS channels will be active,
 *  i.e. sent to the output. (ignored in a non-mono case)
 *
 *  This procedure is called from the likes of the Control thread.
 *  Instead of having the control thread directly write to
 *  thread TxJoin's variables, it calls this function which
 *  changes the state of this thread.
 */
Void thrTxJoinSetOutputChannel( Uns channelNumber )
{
    if (channelNumber < NUMCHANNELS) {
        thrTxJoin.outputChannel = channelNumber;
    }
}

