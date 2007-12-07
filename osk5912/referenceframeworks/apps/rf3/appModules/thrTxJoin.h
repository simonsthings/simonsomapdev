/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrTxJoin.h ========
 *
 *  Declaration of resources structure for thread TxJoin,
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
#ifndef THRTXJOIN_
#define THRTXJOIN_

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
 */
typedef struct ThrTxJoin {
    PIP_Obj *pipIn[ NUMCHANNELS ];
    PIP_Obj *pipOut;
    Int      outputChannel;     /* channel number: 0, 1, ... */
} ThrTxJoin;

/*
 *  Declaration of the thread data object
 */
extern ThrTxJoin thrTxJoin;

/*
 *  Declaration of "public" thread functions 
 */
/* init function, called from thrInit() */
extern Void thrTxJoinInit( Void );

/* run function, called by the swiTxJoin SWI object */
extern Void thrTxJoinRun( Void );

/* active channel # change function, called by the control thread */
extern Void thrTxJoinSetOutputChannel( Uns channelNumber );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* THRTXJOIN_ */


