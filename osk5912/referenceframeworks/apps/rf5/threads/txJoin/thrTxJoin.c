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
 */

#include <std.h>
#include <string.h>

// DSP/BIOS includes
#include <sio.h>
#include <sys.h>

// RF5 module includes
#include <scom.h>
#include <utl.h>

// application includes
#include "appResources.h"   /* application-wide common info */
#include "appThreads.h"     /* thread-wide common info */

#include "thrTxJoin.h"

// Buffers for input device (2 for double buffering)
#define NUMOUTPUTBUFFERS 2       // double buffering for SIO
static  Sample bufTx[ NUMOUTPUTBUFFERS ][ NUMCODECCHANS * FRAMELEN ];

/*  Structure to allow changing of active channels to the codec */ 
Int activeChannels[NUMCODECCHANS];

/*
 *  Thread TxJoin object which encapsulates the state information 
 *  of the thread.
 */ 
ThrTxJoin thrTxJoin;


/*
 *  ======== thrTxJoinInit ========
 *
 */
Void thrTxJoinInit()
{
    SIO_Attrs   attrs;
    SCOM_Handle scomReceive;
    Int         codecChanNum;
    
    attrs       = SIO_ATTRS;
    attrs.align = MEMALIGN;
    attrs.model = SIO_ISSUERECLAIM;

    /* create the codec output stream */
    thrTxJoin.outStream = SIO_create( "/dioCodec", SIO_OUTPUT, 
                      NUMCODECCHANS * FRAMELEN * sizeof( Sample ), &attrs );
    UTL_assert( thrTxJoin.outStream != NULL );
    
    /* create your receiving SCOM queue */
    scomReceive = SCOM_create( "scomTxJoin", NULL );
    UTL_assert( scomReceive != NULL );

    /* Initialize which channels go to the codec */
    for (codecChanNum = 0; codecChanNum < NUMCODECCHANS; codecChanNum++) {
        activeChannels[codecChanNum] = codecChanNum;
    }     
}

/*
 *  ======== thrTxJoinStartup ========
 *
 */
Void thrTxJoinStartup()
{
    Int rc, i;
    
    // issue silent buffers (0) to the output stream
    memset( bufTx, 0, sizeof( bufTx ) );
    
    for (i = 0; i < NUMOUTPUTBUFFERS; i++) {
        rc = SIO_issue( thrTxJoin.outStream, bufTx[i], 
                        SIO_bufsize( thrTxJoin.outStream ), NULL ); 
        UTL_assert( rc >= 0 );
    }
}

/*
 *  ======== thrTxJoinRun ========
 *
 *  Main function of TxJoin task.
 */
Void thrTxJoinRun()
{
    SCOM_Handle scomReceive, scomSend;

    // open the SCOM queues (your own for receiving and another for sending)
    scomReceive = SCOM_open( "scomTxJoin"      );
    scomSend    = SCOM_open( "scomToProcessFromTx" );

    UTL_assert( scomReceive != NULL );
    UTL_assert( scomSend    != NULL );

    while (TRUE) {
        ScomBufChannels *scombuf;
        Sample          *bufDst, *bufSrc;        
        Int              i, codecChanNum;
        
        // Reclaim empty destination buffer from the output stream
        SIO_reclaim( thrTxJoin.outStream, (Ptr *)&bufDst, NULL );         

        // get the structure describing source channels
        scombuf = (ScomBufChannels *)SCOM_getMsg( scomReceive, SYS_FOREVER ); 

        // join the input data based on the active channel(s).
        for (codecChanNum = 0; codecChanNum < NUMCODECCHANS; codecChanNum++) {
            bufSrc = scombuf->bufChannel[ activeChannels[codecChanNum] ];
            for (i = 0; i < FRAMELEN; i++) {
                bufDst[ NUMCODECCHANS * i + codecChanNum ] = bufSrc[i];
            }
        }

        // send back descriptors of now used up source channels to Process
        SCOM_putMsg( scomSend, scombuf );
        
        // issue full destination buffer to the output stream
        SIO_issue( thrTxJoin.outStream, bufDst, 
                        SIO_bufsize( thrTxJoin.outStream ), NULL );
    }
}

