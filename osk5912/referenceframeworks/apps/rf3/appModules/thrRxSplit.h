/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrRxSplit.h ========
 *
 *  Declaration of resources structure for thread RxSplit,
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
#ifndef THRRXSPLIT_
#define THRRXSPLIT_

#include <pip.h>

#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *  Here we define a structure that contains all the "private"
 *  thread information. This is not an algorithm processing
 *  thread so the structure doesn't have the usual form.
 *
 *  We will initialize these statically: pipIn will point to pipRx,
 *  pipOut[] will point to pipRx0, pipRx1, respectively. We use
 *  aliases for convenience.
 */
typedef struct ThrRxSplit {
    PIP_Handle pipIn;                  /* input pipe with joined channel data */
    PIP_Handle pipOut[ NUMCHANNELS ];  /* outpipes with individ. channel data */
} ThrRxSplit;

/*
 *  Declaration of the thread data object
 */
extern ThrRxSplit thrRxSplit;

/*
 *  Declaration of "public" thread functions 
 */
/* init function, called from thrInit() */
extern Void thrRxSplitInit( Void );

/* run function, called by the swiRxSplit SWI object */
extern Void thrRxSplitRun( Void );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* THRRXSPLIT_ */


