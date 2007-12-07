/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrAudioproc.c ========
 *
 *  Static and dynamic initialization of thread Audioproc,
 *  and its runtime procedure
 */
#include <std.h>
#include <pip.h>

#include <utl.h>            /* debug/diagnostics utility functions */

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */
#include "fir.h"            /* interface for FIR algorithm */
#include "vol.h"            /* interface for VOL algorithm */
#include "thrAudioproc.h"   /* definition of thrAudioproc objects */

/*
 *  Static part of thread initialization
 */

/* 
 *  Initialization of the thread resources structure:
 *  NULL for algorithm handles, addresses of appropriate 
 *  pipe objects for input and output pipes,
 *  NULL for globally visible temporary pointers to pipe frames,
 *  addresses of intermediate buffers for this thread,
 *  and everything else thread-specific.
 */
ThrAudioproc thrAudioproc[ NUMCHANNELS ] = {
    { /* channel #0 */
        /* algorithm handle(s) (to be initialized in runtime) */
        NULL,                   /* algFIR */
        NULL,                   /* algVOL */
    
        /* input pipe(s) */
        &pipRx0,                /* pipIn */

        /* output pipe(s) */
        &pipTx0,                /* pipOut */

        /* buffer(s) */
        bufAudioproc,           /* bufInterm */
    
        /* everything else private for the thread */
        
    }, /* end channel # 0 */
    { /* channel #1 */
        /* algorithm handle(s) (to be initialized in runtime) */
        NULL,                   /* algFIR */
        NULL,                   /* algVOL */
    
        /* input pipe(s) */
        &pipRx1,                /* pipIn */

        /* output pipes */
        &pipTx1,                /* pipOut */

        /* buffer(s) */
        bufAudioproc,           /* buffer */
    
        /* everything else private for the thread */

    }, /* end channel # 1 */
};

/* 
 *  FIR filter coefficients: each channel runs a different instance of
 *  the FIR algorithm, and we initialize each instance with a different
 *  filter, i.e. with a different set of coefficients. In this example,
 *  channel #0 is a low-pass filter, and channel #1 is a hi-pass filter.
 */
static Sample filterCoefficients[ NUMCHANNELS ][ 32 ] = {
    /* 
     * The following filters have been designed with the compromise to let
     * through a reasonable amount of audio content when used with sampling 
     * rates from 8 to 48 kHz. This is due to the wide variety of default 
     * sampling rates supported in the various ports of RF5.
     */
     
    /* Low-pass, 32 taps, passband 0 to 500 Hz for sampling rate of 8kHz */
    {
        0x08FC, 0xF6DE, 0xF92A, 0xFA50, 0xFB17, 0xFBF0, 0xFD2A, 0xFECF,
        0x00EC, 0x036C, 0x0623, 0x08E1, 0x0B6E, 0x0D91, 0x0F1A, 0x0FE9,
        0x0FE9, 0x0F1A, 0x0D91, 0x0B6E, 0x08E1, 0x0623, 0x036C, 0x00EC,
        0xFECF, 0xFD2A, 0xFBF0, 0xFB17, 0xFA50, 0xF92A, 0xF6DE, 0x08FC
    },
    /* High-pass, 32 taps, passband 500 Hz to 4 kHz for sampling rate of 8kHz */
    {
        0x08B6, 0xFC32, 0xFC41, 0x09CF, 0x0467, 0x0B2E, 0x0099, 0x05FB, 
        0xF920, 0x014E, 0xF1B4, 0xFF3F, 0xE8F9, 0x02AB, 0xD626, 0x41EB, 
        0x41EB, 0xD626, 0x02AB, 0xE8F9, 0xFF3F, 0xF1B4, 0x014E, 0xF920,
        0x05FB, 0x0099, 0x0B2E, 0x0467, 0x09CF, 0xFC41, 0xFC32, 0x08B6
    },
};

/*
 *  Dynamic part of thread initialization
 */

/*
 *  ========= thrAudioprocInit ========
 *  Initialization of data structures for the thread(s), called from 
 *  appThreads.c:thrInit() at init time.
 *
 *  Here we create one instance of FIR algorithm per channel and
 *  one instance of VOL algorithm per channel. In a loop, we create
 *  parameters for algorithm instance for each channel by using the 
 *  default parameters and modifying fields that are different.
 *  (If the parameters are the same across channels, they can be
 *  created outside of the loop.)
 */
Void thrAudioprocInit( Void ) 
{
    /* declaration of filter, volume parameter structures */
    FIR_Params firParams;
    VOL_Params volParams;
    Int i;

    for (i = 0; i < NUMCHANNELS; i++) {  
        /* 
         *  Set the parameters structure to the default, i.e. 
         *  the one used in i<alg>.c, and modify fields that are different.
         */
        firParams = FIR_PARAMS;         /* default parameters */
        firParams.coeffPtr   =              /* filter coefficients */
            (Short *)filterCoefficients[i];
        firParams.filterLen  =              /* filter size */
            sizeof( filterCoefficients[i] ) / sizeof( Sample );
        firParams.frameLen   = FRAMELEN;    /* frame size in samples */
    
        /* create algorithm instance for channel #i */
        thrAudioproc[i].algFIR = FIR_create( &FIR_IFIR, &firParams );

        /* 
         *  Confirm that the instantiation was successful. If it failed,
         *  most likely the heap is not big enough. To find out the needed
         *  value (rather than to guess), in appThreads.c you can do
         *  ALGRF_setup( EXTERNALHEAP, EXTERNALHEAP ); i.e. force all
         *  allocation in external memory, run the initialization functions,
         *  and examine the reports from UTL_showAlgMem() below.
         */
        UTL_assert( thrAudioproc[i].algFIR != NULL );
        
        /* and show algorithm memory usage */
        UTL_showAlgMem( thrAudioproc[i].algFIR );

        /* do the same for the VOLume algorithm: create parameters structure */
        volParams = VOL_PARAMS;             /* default parameters */
        volParams.frameSize  = FRAMELEN;    /* size in samples */
        volParams.gainPercentage = 100;     /* default gain */
    
        /* create instance, confirm creation success, show memory usage */
        thrAudioproc[i].algVOL = VOL_create( &VOL_IVOL, &volParams );
        UTL_assert( thrAudioproc[i].algVOL != NULL );
        UTL_showAlgMem( thrAudioproc[i].algVOL );
    }
}    

/*
 *  Runtime thread code, invoked by the appropriate SWI object
 *  every time the object is posted
 */

/*
 *  ========= thrAudioprocRun ========
 *  The "body" of the swiAudioproc0, swiAudioproc1,... threads.
 *
 *  The single argument of this function is the channel number:
 *  0, 1, 2 etc. up to NUM_CHANNELS - 1. All the SWI objects
 *  that invoke this function pass the channel number as the
 *  argument. 
 *
 *  Based on the channel number, the thread -- the procedure --
 *  decides which thread resource object to access.
 */
Void thrAudioprocRun( Arg aChan )
{
    Sample *src, *dst;
    Int     size;       /* in samples */
    Int     chan;

    /*
     *  cast 'Arg' types to 'Int'. This is required on 55x large data model
     *  since Arg is not the same size as Int and Ptr in that model.
     *  On all other devices (54x, 55x small, 6x) ArgToInt is a simple cast
     */
    chan = ArgToInt( aChan );

    /*
     *  Check that the preconditions are met, that is the in-pipe 
     *  has a ready-to-consume buffer of data and the out-pipe 
     *  has a free buffer, in other words that this thread has
     *  not been posted in error.
     */
    UTL_assert( PIP_getReaderNumFrames( thrAudioproc[chan].pipIn  ) > 0 );
    UTL_assert( PIP_getWriterNumFrames( thrAudioproc[chan].pipOut ) > 0 );

    /* get the full buffer from the input pipe */
    PIP_get( thrAudioproc[chan].pipIn );
    src = (Sample *)PIP_getReaderAddr( thrAudioproc[chan].pipIn );
    /* get the size in samples (the function below returns it in words) */
    size = sizeInSamples( PIP_getReaderSize( thrAudioproc[chan].pipIn ) );

    /* get the empty buffer from the out-pipe */
    PIP_alloc( thrAudioproc[chan].pipOut );
    dst = (Sample *)PIP_getWriterAddr( thrAudioproc[chan].pipOut );

    /* apply filter and store result in intermediate buffer */
    FIR_apply( thrAudioproc[chan].algFIR, 
               src, thrAudioproc[chan].bufInterm );

    /* amplify the signal in the interm. buffer and store result in dst */
    VOL_apply( thrAudioproc[chan].algVOL, 
               thrAudioproc[chan].bufInterm, dst );
    
    /* Record the amount of actual data being sent */
    PIP_setWriterSize( thrAudioproc[chan].pipOut, sizeInWords( size ) );

    /* Free the receive buffer, put the transmit buffer */
    PIP_free( thrAudioproc[chan].pipIn  );
    PIP_put ( thrAudioproc[chan].pipOut );
}

/*
 *  ========= thrAudioprocSetOutputVolume ========
 *  Procedure that changes output volume for an Audioproc thread
 *
 *  This procedure is called from the likes of the Control thread.
 *  Instead of having the control thread directly write to
 *  thread Audioproc's variables, it calls this function which
 *  changes the state of the VOL algorithm instance for the
 *  given channel.
 */
Void thrAudioprocSetOutputVolume( Int chan, Int volume )
{
    /* VOL algorithm control structure */
    IVOL_Status status;

    UTL_assert( chan < NUMCHANNELS );   /* sanity checking */
    
    /* 
     *  Apply volume gain information to the appropriate alg. instance.
     *  Retrieve the current parameters, change the fields that need
     *  to be changed, and apply the new parameters. Volume gain is
     *  a percentage, a number from 0 to 200, 100 being the normal
     *  volume (100%).
     */
    VOL_control( thrAudioproc[ chan ].algVOL, IVOL_GETSTATUS, &status );
    status.gainPercentage = volume;
    VOL_control( thrAudioproc[ chan ].algVOL, IVOL_SETSTATUS, &status );
}

