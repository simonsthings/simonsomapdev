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
 */

#include <std.h>

// DSP/BIOS includes
#include <sio.h>
#include <sys.h>

// RF5 module includes
#include <scom.h>
#include <utl.h>

// application includes
#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */

#include "thrRxSplit.h"

// Buffers and the SIO object for the input device
#define NUMINPUTBUFFERS 2       // double buffering for SIO
static  Sample bufRx[ NUMINPUTBUFFERS ][ NUMCODECCHANS * FRAMELEN ];

/*
 *  Thread rxSplit object which encapsulates the state information 
 *  of the thread.
 */
ThrRxSplit thrRxSplit;

/*
 *  ======== thrRxSplitInit ========
 *
 */
Void thrRxSplitInit() 
{
    SIO_Attrs   attrs;
    SCOM_Handle scomReceive;

    attrs       = SIO_ATTRS;
    attrs.align = MEMALIGN;
    attrs.model = SIO_ISSUERECLAIM;

    /* create the codec input stream */
    thrRxSplit.inStream = SIO_create( "/dioCodec", SIO_INPUT, 
                         NUMCODECCHANS * FRAMELEN * sizeof( Sample ), &attrs );
    UTL_assert( thrRxSplit.inStream != NULL );

    /* create your receiving SCOM queue */
    scomReceive = SCOM_create( "scomRxSplit", NULL );
    UTL_assert( scomReceive != NULL );
}

/*
 *  ======== thrRxSplitStartup ========
 *
 */
Void thrRxSplitStartup() 
{
    Int rc, i;

    /* issue buffers to the input stream i.e. do the 'priming' */
    for (i = 0; i < NUMINPUTBUFFERS; i++) {
        rc = SIO_issue( thrRxSplit.inStream, bufRx[i], 
                        SIO_bufsize( thrRxSplit.inStream ), NULL );
        UTL_assert( rc >= 0 );
    }
}

/*
 *  ======== thrRxSplitRun ========
 *
 *  Main function of RxSplit task.
 */
Void thrRxSplitRun()
{
    SCOM_Handle scomReceive, scomSend;    

    // open the SCOM queues (your own for receiving and another for sending)
    scomReceive = SCOM_open( "scomRxSplit"     );
    scomSend    = SCOM_open( "scomToProcessFromRx" );

    UTL_assert( scomReceive != NULL );
    UTL_assert( scomSend    != NULL );

    while (TRUE) {
        ScomBufChannels *scombuf;
        Sample          *bufSrc;
        Int              i, chanNum;
        Int              codecChanNum = 0;

        // Reclaim full source buffer from the input stream
        SIO_reclaim( thrRxSplit.inStream, (Ptr *)&bufSrc, NULL );        

        // get the structure describing destination channels
        scombuf = (ScomBufChannels *)SCOM_getMsg( scomReceive, SYS_FOREVER );
        
        // split the input data: 
        // For stereo codec: LRLRLRLR -> LL..L,RR..R,LL..L,RR..R
        // For mono codec:   DDDD...D -> DD..D,DD..D,DD..D,DD..D
        for (chanNum = 0; chanNum < NUMCHANNELS; chanNum++) {
            Sample *bufDst = scombuf->bufChannel[ chanNum ];
            for (i = 0; i < FRAMELEN; i++) {
                bufDst[i] = bufSrc[ NUMCODECCHANS * i + codecChanNum ];
            }
            codecChanNum = (codecChanNum + 1) % NUMCODECCHANS;
        }

        // put the full buffer to the ToProc queue
        SCOM_putMsg( scomSend, scombuf );
        
        // Issue an empty buffer to the input stream
        SIO_issue( thrRxSplit.inStream, bufSrc, 
                        SIO_bufsize( thrRxSplit.inStream ), NULL );
    }
}

