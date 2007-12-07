/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrRxSplit.c ========
 *
 *  Runtime procedure for thread RxSplit.
 *
 *  Thread RxSplit would be used in applications where channels are dependent
 *  i.e. come together -- stereo or surround sound, for example. In stereo
 *  mode, stereo codec delivers left and right channel as two successive 
 *  words, and unless we can program the DMA to transfer left and right
 *  samples to different pipes, they are all crammed in one pipe. This thread
 *  splits left and right samples in two pipes for stereo data, or, in general, 
 *  splits mixed channel data in separate pipes, one per channel.
 *
 *  This thread does not exist in an application where samples for 
 *  different channels arrive independently.
 *
 *  Since we want to process channel by channel, we split the samples in
 *  pipRx into only left samples and only right samples, that go to pipes
 *  pipRx0 and pipRx1, respectively:
 *
 *  pipRx (size=2x80)  .-----> LLLLLL...L  pipRx0 (size=80) channel #0
 *  LRLRLRLRLR...LR ---+
 *                     `-----> RRRRRR...R  pipRx1 (size=80) channel #1
 *
 *  In general case, for samples for N channels joined together in a 
 *  single pipe, we separate them the following way (FS is frame size;
 *  ("M" is N-1, introduced for picture clarity):
 * 
 *  pipRx (size=NxFS)  .-----> 000000...0  pipRx0 (size=FS) channel #0
 *  0123...M0123..M ---+-----> 111111...1  pipRx1 (size=FS) channel #1
 *                     +-----> 222222...2  pipRx2 (size=FS) channel #2
 *                     :                                    ...
 *                     `-----> MMMMMM...M  pipRxN (size=FS) channel #M
 *
 *  If we are using a board with a mono codec for development, such as 
 *  bare DSK5402 with its AD50 codec, however, pipRx has the same size as 
 *  pipRx0, pipRx1,... and the data is simply duplicated from pipRx to
 *  all pipRx{0,1,2,...}.
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
#include "thrRxSplit.h"     /* definition of thrRxSplit object */

/*
 *  Static thread initialization
 */

/* 
 *  Initialization of the thread resources structure:
 *  Bind internally used pipe pointers to actual pipe objects defined in BIOS
 *  configuration 
 */
ThrRxSplit thrRxSplit = {
    &pipRx,                 /* pipIn */
    { &pipRx0, &pipRx1 }    /* pipOut[ NUMCHANNELS ] */
};

/*
 *  ========= thrRxSplitInit ========
 *  Initialization of data structures for the thread, called from 
 *  appThreads.c:thrInit() at init time.
 */
Void thrRxSplitInit( Void ) 
{
}

/*
 *  ========= thrRxSplitRun ========
 *  The "body" of the swiRxSplit thread.
 *
 *  Whenever a new frame is ready in pipRx and there is at least one
 *  free frame in both pipRx0 and pipRx1, this thread is posted.
 */
Void thrRxSplitRun( Void ) 
{
    Int i, ch;
    Sample *srcBuf, *dstBuf;

    /* make sure the input pipe really has a new frame to read */
    UTL_assert( PIP_getReaderNumFrames( thrRxSplit.pipIn ) > 0 );

    /* 
     *  Record the period between two executions of this point in code.
     *  By looking at stsTime0 in the STS panel we can see if our driver timing 
     *  is right: if we select "stsTime0" unit in STS panel to be "milliseconds"
     *  then the value should be 1000/<sampling rate>*FRAMELEN. For instance,
     *  for 44100 Hz and 80 samples in a frame, the result is 1.81ms.
     */
    UTL_stsPeriod( stsTime0 );
        
    /* get the full buffer from the input pipe */
    PIP_get( thrRxSplit.pipIn );
    srcBuf = (Sample *)PIP_getReaderAddr( thrRxSplit.pipIn );
    
    /* copy appropriate samples from the input pipe to output pipes */
    for (ch = 0; ch < NUMCHANNELS; ch++) {

        /* make sure the destination pipe has a free frame */
        UTL_assert( PIP_getWriterNumFrames( thrRxSplit.pipOut[ ch ] ) > 0 );
        
        /* allocate an empty buffer */
        PIP_alloc( thrRxSplit.pipOut[ ch ] );
        dstBuf = (Sample *)PIP_getWriterAddr( thrRxSplit.pipOut[ ch ] );

        /* copy all source samples for the channel to its destination pipe */
        for (i = 0; i < FRAMELEN; i++) {    /* FRAMELEN is size in samples */
            #ifdef APPMONOCODEC  /* simulation of stereo/N-ch. codec w/ mono */
            dstBuf[i] = srcBuf[i];
            #else
            dstBuf[i] = srcBuf[ i * NUMCHANNELS + ch ]; /* real stereo/N-ch. */
            #endif          
        }
        
        /* finally, do a "put" operation for the output pipe */
        PIP_setWriterSize( thrRxSplit.pipOut[ ch ], sizeInWords( FRAMELEN ) );
        PIP_put( thrRxSplit.pipOut[ ch ] );
    }

    /* Free the input buffer */
    PIP_free( thrRxSplit.pipIn );
}

