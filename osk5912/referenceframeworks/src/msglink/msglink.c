/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== msglink.c ========
 *  Messaging on top of Link's channels.
 */

/* separate section placement of MSGLINK functions */
#pragma CODE_SECTION( MSGLINK_init,     ".text:init"         )
#pragma CODE_SECTION( MSGLINK_exit,     ".text:exit"         )
#pragma CODE_SECTION( MSGLINK_create,   ".text:create"       )
#pragma CODE_SECTION( MSGLINK_delete,   ".text:delete"       )
#pragma CODE_SECTION( MSGLINK_send,     ".text:MSGLINK_send" )
#pragma CODE_SECTION( MSGLINK_recv,     ".text:MSGLINK_recv" )


#include <std.h>

/* BIOS Header files */
#include <mem.h>
#include <sio.h>

/* Reference Framework modules */
#include <msglink.h>
#include <utl.h>

/* Hard-code these for now.  Make sure they match up with the GPP side */
#define INPUTDEVNAME  "/dio_dsplink14"
#define OUTPUTDEVNAME "/dio_dsplink15"

/*
 *  Structure that stores the state information for the channels
 *  that are being used for messaging.
 *  Note the actual messages buffers are in this structure.  Currently
 *  doing this for simplicity.
 */
typedef struct ChanObj {
    Bool            inUse;
    SIO_Handle      hStream;
    Uns             primedBufs;
    MdUns           bufSize;
    Ptr             bufs[MSGLINK_NUMBUFS];
} ChanObj, *ChanHandle;

/*
 *  Currently only supporting 1 input and 1 output message channel 
 *  for simplicity.  
 */
static ChanObj chanInput;
static ChanObj chanOutput;


/*
 *  ======== MSGLINK_init ========
 *  Initialize the input and output message link as not in use.
 *
 */
Void MSGLINK_init( Void )
{
    chanInput.inUse  = FALSE;
    chanOutput.inUse = FALSE;
}

/*
 *  ======== MSGLINK_exit ========
 *  Currently does nothing.
 */
Void MSGLINK_exit( Void )
{
}

/*
 *  ======== MSGLINK_create ========
 *  Creates a message link on top of a SIO channel.
 *  This function should only be called once for each
 *  mode.
 */
Int MSGLINK_create( MSGLINK_Mode msglinkMode, MdUns msgSize, Uns timeout )
{
    SIO_Attrs  attrs;
    ChanHandle chan;
    Char      *devName;
    Int        success = MSGLINK_SUCCESS;
    Int        mode;
    Int        i;

    /* Validate parameter */
    UTL_assert( (msglinkMode == MSGLINK_INPUT) || 
                (msglinkMode == MSGLINK_OUTPUT) );

    /*
     *  Set-up local variables accordingly depending whether it is
     *  input or output message link.
     */
    if (msglinkMode == MSGLINK_INPUT) {
        chan    = &chanInput;
        mode    = SIO_INPUT;
        devName = INPUTDEVNAME;
    }
    else {
        chan    = &chanOutput;
        mode    = SIO_OUTPUT;
        devName = OUTPUTDEVNAME;
    }

    /* Make sure this channel is not already created. */
    UTL_assert( chan->inUse == FALSE );

    /* Attributes for the stream creation */
    attrs         = SIO_ATTRS;
    attrs.nbufs   = MSGLINK_NUMBUFS;
    attrs.segid   = 0;
    attrs.align   = 0;
    attrs.flush   = FALSE; // Allow pending messages to be sent.
    attrs.model   = SIO_ISSUERECLAIM;
    attrs.timeout = timeout;

    /* Creating stream */
    chan->hStream = SIO_create( devName, mode, msgSize, &attrs );
    if ( chan->hStream == NULL ) {
        return( MSGLINK_FAILURE );
    }

    /* 
     *  Initialize the rest of the state information and allocate
     *  the messages.
     */
    chan->inUse      = TRUE;
    chan->primedBufs = 0;
    chan->bufSize    = msgSize;
    for ( i = 0; i < MSGLINK_NUMBUFS; i++ ) {
        /*
         *  Use the default BIOS segment id.  
         *  No alignment needed 
         */
        chan->bufs[i] = MEM_alloc( 0, chan->bufSize, 0 );
        if ( chan->bufs[i] == MEM_ILLEGAL ) {
            success = MSGLINK_FAILURE;
        }
    }

    if ( success == MSGLINK_FAILURE ) {
        /* Delete the message link */
        MSGLINK_delete( msglinkMode );
        return( MSGLINK_FAILURE );
    }

    /*
     *  On the input stream, we need to issue the empty buffers.
     *  This allows the device to filling them in when the data
     *  starts being received.
     *
     *  On the output stream, there is no priming.  You cannot 
     *  prime messages.  There is logic in the MSGLINK_send()
     *  that handles the ability to have multiple buffering.
     */
    if ( msglinkMode == MSGLINK_INPUT ) {

        /* Issue all the buffers for this stream. */
        for ( i = 0; i < MSGLINK_NUMBUFS; i++ ) {

            if ( SIO_issue( chan->hStream, chan->bufs[i], 
                            msgSize, NULL ) != SYS_OK ) {
                /* Something is wrong.*/
                UTL_logError1("MSGLINK_create(): SIO_issue() failed. i=%d", i);

                /* Delete the message link */
                MSGLINK_delete( msglinkMode );

                return( MSGLINK_FAILURE );
            }
        }
    }

    return( MSGLINK_SUCCESS );
}

/*
 *  ======== MSGLINK_delete ========
 *  Free the resources created in MSGLINK_create().
 *  This function should only be called once for each
 *  mode.
 */
Int MSGLINK_delete( MSGLINK_Mode msglinkMode )
{
    Int i;
    ChanHandle chan;

    /*  
     *  Set-up local variable accordingly depending whether it is 
     *  input or output message link.
     */
    if ( msglinkMode == MSGLINK_INPUT ) {
        chan = &chanInput;
    }
    else {
        chan = &chanOutput;
    }

    /* Make sure this channel was already created. */
    UTL_assert( chan->inUse == TRUE );

    /* Free the buffers allocated in the MSGLINK_create */
    for ( i = 0; i < MSGLINK_NUMBUFS; i++ ) {
        if ( chan->bufs[i] != MEM_ILLEGAL ) {
            MEM_free( 0, chan->bufs[i], chan->bufSize );
        }
    }

    /* 
     *  Delete the stream. The pending messages will be sent since
     *  the SIO_create's attrs had the flush field set to FALSE.
     */
    if ( SIO_delete( chan->hStream ) != SYS_OK ) {
        return( MSGLINK_FAILURE );
    }

    /* Reset the state of this channel. */
    chan->inUse = FALSE;

    return( MSGLINK_SUCCESS );
}

/*
 *  ======== MSGLINK_send ========
 *  Sends a message across the output channel.
 *  This function is not thread-safe.  Only one thread should be calling
 *  this at once.
 *
 *  NOTE: The caller must deal with endianism issues if necessary.
 */
Int MSGLINK_send( Ptr msg )
{
    Int status;
    Int nmadus;
    Ptr msgBuf;

    /* Make sure the channel has been created. */
    UTL_assert( chanOutput.inUse == TRUE );

    /* Validate parameter. */
    UTL_assert( msg != NULL );

    /* Use up all the un-issued buffers first */
    if ( chanOutput.primedBufs < MSGLINK_NUMBUFS) {

        /* Grab the next un-used buffer */
        msgBuf = chanOutput.bufs[chanOutput.primedBufs];
        chanOutput.primedBufs++;
    }
    else {
        /* Reclaim an empty buffer from output stream */
        nmadus = SIO_reclaim( chanOutput.hStream, (Ptr *) &msgBuf, NULL );
        if ( nmadus < 0 ) {
            return( MSGLINK_FAILURE );
        }
    }

    /* Copy the message into the stream's buffer */
    memcpy( msgBuf, msg, chanOutput.bufSize );

    /* Issue a full buffer to the output stream. */
    status = SIO_issue( chanOutput.hStream, msgBuf, chanOutput.bufSize, NULL );
    if ( status != SYS_OK ) {
        return( MSGLINK_FAILURE );
    }

    return( MSGLINK_SUCCESS );
}


/*
 *  ======== MSGLINK_recv ======== 
 *  Receive a message from the input channel
 *  This function is not thread-safe.  Only one thread should be calling
 *  this at once.
 *
 *  NOTE: The caller must deal with endianism issues if necessary.
 */
Int MSGLINK_recv( Ptr msg )
{
    Int status;
    Int nmadus;
    Ptr msgBuf;

    /* Make sure the channel has been created. */
    UTL_assert( chanInput.inUse == TRUE );

    /* Validate parameter. */
    UTL_assert( msg != NULL );

    /*
     *  Reclaim full buffer from input stream.  
     *  Note the input stream was primed in the MSGLINK_create().
     */
    nmadus = SIO_reclaim( chanInput.hStream, (Ptr *) &msgBuf, NULL );
    if ( nmadus < 0 ) {
        return( MSGLINK_FAILURE );
    }

    /* Copy the stream's buffer into the caller's buffer. */
    memcpy( msg, msgBuf, chanInput.bufSize );

    /* Return the empty buffer to the input stream */
    status = SIO_issue( chanInput.hStream, msgBuf, chanInput.bufSize, NULL );

    if ( status != SYS_OK ) {
        return( MSGLINK_FAILURE );
    }

    return( MSGLINK_SUCCESS );
}

