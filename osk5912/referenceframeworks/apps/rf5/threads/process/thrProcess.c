/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrProcess.c ========
 *
 *  Implementation of the Process thread.  
 *  This thread is a typical data processing thread. It consists of NUMCHANNELS
 *  RF5 channels, each consisting of two RF5 cells. An RF5 cell is a wrapper
 *  around an XDAIS algorithm that gives it a uniform data processing interface.
 *  (since XDAIS algorithms have different processing functions with different
 *  signatures). An RF5 channel is a collection of cells, which execute in 
 *  series. 
 */

#include <std.h>

// DSP/BIOS includes
#include <mbx.h>
#include <sys.h>

// RF5 module includes
#include <chan.h>
#include <icell.h>
#include <scom.h>
#include <icc.h>
#include <icc_linear.h>
#include <utl.h>

// application includes
#include "appResources.h"
#include "appThreads.h"

// Thread include
#include "thrProcess.h"

// Cell includes
#include "fir/cellFir.h"
#include "vol/cellVol.h"


/*
 *   Incoming data goes into bufInputs.
 *   Outgoing data goes into bufOutputs.
 *   bufIntermediate is used in the intermediate processing.
 *
 *   The intermediate buffer is shared across all channels.
 *   In this appl., we need only one because there are only 2 cells
 *   per channel and there is only 1 output buffer for the first
 *   cell of each channel.  If there were more cells per channel
 *   or if there were more inputs/outputs per cell, additional 
 *   intermediate buffers are needed as appropriate.
 */
static Sample bufInput[ NUMCHANNELS ][ FRAMELEN ];
static Sample bufOutput[ NUMCHANNELS ][ FRAMELEN ];
static Sample bufIntermediate[ FRAMELEN ];

/*
 *  Thread process object which encapsulates the state information 
 *  of the thread.
 */ 
ThrProcess thrProcess;

/* 
 * Handle to mailbox for control messages 
 */
MBX_Handle mbxProcess;

/* 
 *  FIR filter coefficients: each channel runs a different instance of
 *  the FIR algorithm, and we initialize each instance with a different
 *  filter, i.e. with a different set of coefficients.
 */

// number of filter taps
#define NUMFIRTAPS  32

/* 
 * The following filters have been designed with the compromise to let
 * through a reasonable amount of audio content when used with sampling 
 * rates from 8 to 48 kHz. This is due to the wide variety of default 
 * sampling rates supported in the various ports of RF5.
 */
static Short filterCoeffLowPass[ NUMFIRTAPS ] = {
    // Low-pass, 32 taps, passband 0 to 500 Hz for sampling rate of 8kHz
    0x08FC, 0xF6DE, 0xF92A, 0xFA50, 0xFB17, 0xFBF0, 0xFD2A, 0xFECF,
    0x00EC, 0x036C, 0x0623, 0x08E1, 0x0B6E, 0x0D91, 0x0F1A, 0x0FE9,
    0x0FE9, 0x0F1A, 0x0D91, 0x0B6E, 0x08E1, 0x0623, 0x036C, 0x00EC,
    0xFECF, 0xFD2A, 0xFBF0, 0xFB17, 0xFA50, 0xF92A, 0xF6DE, 0x08FC
};
static Short filterCoeffHighPass[ NUMFIRTAPS ] = {
    // High-pass, 32 taps, passband 500 Hz to 4 kHz for sampling rate of 8kHz
    0x08B6, 0xFC32, 0xFC41, 0x09CF, 0x0467, 0x0B2E, 0x0099, 0x05FB, 
    0xF920, 0x014E, 0xF1B4, 0xFF3F, 0xE8F9, 0x02AB, 0xD626, 0x41EB, 
    0x41EB, 0xD626, 0x02AB, 0xE8F9, 0xFF3F, 0xF1B4, 0x014E, 0xF920,
    0x05FB, 0x0099, 0x0B2E, 0x0467, 0x09CF, 0xFC41, 0xFC32, 0x08B6
};
static Short filterCoeffPassThrough[ NUMFIRTAPS ] = {
    // Pass-through, 32 taps
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7FFF
};

/* the list of all coefficient arrays */
static Short * filterCoeffList[] = { 
    filterCoeffLowPass, filterCoeffHighPass, filterCoeffPassThrough
};


// Local function prototypes
static Void setParamsAndStartChannels( Bool doChannelOpen );

/*
 *  ======== thrProcessInit ========
 *
 *  Initialization of the thread's object structure, creation of
 *  SCOM queues, and registration of cells (via setParamsAndStartChannels()
 *  function below).
 */
Void thrProcessInit()
{
    Int i;
    SCOM_Handle scomReceiveFromRx;
    SCOM_Handle scomReceiveFromTx;

    /* Set-up the threads's buffer pointers. */
    for (i = 0; i < NUMCHANNELS; i++) {
        thrProcess.bufInput[i]  = bufInput[i];
        thrProcess.bufOutput[i] = bufOutput[i];
    }
    thrProcess.bufIntermediate = bufIntermediate;

    /* create named SCOM queues for receiving messages from other tasks */
    scomReceiveFromRx = SCOM_create( "scomToProcessFromRx", NULL );
    scomReceiveFromTx = SCOM_create( "scomToProcessFromTx", NULL );
    UTL_assert( scomReceiveFromRx != NULL );
    UTL_assert( scomReceiveFromTx != NULL );    
    
    /* create mailbox for control messages */
    mbxProcess = MBX_create(sizeof(CtrlMsg), THRPROCESS_MBXLENGTH, NULL);  
    UTL_assert( mbxProcess != NULL );

    setParamsAndStartChannels( FALSE );
}

/*
 *  ======== thrProcessStartup ========
 *
 *  Thread startup: post-initialization activation. Cells that were 
 *  registered in thrProcessInit(), via setParamsAndStartChannels(),
 *  are now actually created when the channel is created.
 */
Void thrProcessStartup()
{
    setParamsAndStartChannels( TRUE );
}


/*
 *  ======== setParamsAndStartChannels ========
 *
 *  This function creates channels/cells from the list. It's called twice:
 *  once from init (with FALSE passed as the argument), and once from 
 *  startup (with TRUE). Both times it has to create cell parameters
 *  and keep it on the stack -- but the first time it registers the cells,
 *  whereas the second time it opens the channel, which causes the XDAIS
 *  algorithms in the cells to be created.
 */
static Void setParamsAndStartChannels( Bool doChannelOpen ) 
{
    IFIR_Params firParams;
    IVOL_Params volParams;    
    Int         chanNum;
    ICELL_Obj  *cell;
    Bool        rc;    
    ICC_Handle  inputIcc;
    ICC_Handle  outputIcc;
    

    for (chanNum = 0; chanNum < NUMCHANNELS; chanNum++) {

        // prepare parameters for FIR and VOL
        firParams           = IFIR_PARAMS;        // copy the defaults
        firParams.coeffPtr  = filterCoeffLowPass; // and override different fields
        firParams.filterLen = NUMFIRTAPS;
        firParams.frameLen  = FRAMELEN;

        volParams           = IVOL_PARAMS;
        volParams.frameSize = FRAMELEN;        

        /* 
         *  register the cells: define what will be input buffers and
         *  what will be output buffers for each cell.
         */
        if (doChannelOpen == FALSE) {
            
            /* Setup a default cell used to initialize the actual cells */
            ICELL_Obj   defaultCell = ICELL_DEFAULT;

            /*
             *  cell 0 - FIR: create an input linear ICC buffer that points to
             *  bufInput[ <channel number> ], and an output linear ICC buffer
             *  that points to the intermediate buffer, bufIntermediate;
             *  the cell will always look for data in those buffers.
             */
            cell = &thrProcess.cellList[ (chanNum * NUMCELLS) + CELLFIR ];
            *cell                = defaultCell;
            cell->name           = "FIR";
            cell->cellFxns       = &FIR_CELLFXNS;            
            cell->algFxns        = (IALG_Fxns *)&FIR_IFIR;
            cell->algParams      = (IALG_Params *)&firParams;
            cell->scrBucketIndex = THRPROCESSSCRBUCKET;

            inputIcc  = (ICC_Handle)ICC_linearCreate( 
                                   thrProcess.bufInput[ chanNum ], 
                                   FRAMELEN * sizeof( Sample ) );
            UTL_assert( inputIcc != NULL);

            outputIcc = (ICC_Handle)ICC_linearCreate( 
                                                thrProcess.bufIntermediate,
                                                FRAMELEN * sizeof( Sample ) );
            UTL_assert( outputIcc != NULL);

            // Only one input and one output ICC are needed.
            rc = CHAN_regCell( cell, &inputIcc, 1, &outputIcc, 1 );
            
            /*
             *  cell 1 - VOL: create an input linear ICC buffer that points to
             *  the intermediate buffer, bufIntermediate, and an output linear 
             *  ICC buffer that points to bufOutput[ <channel number> ].
             *  the cell will always look for data in those buffers.
             */
            cell = &thrProcess.cellList[ (chanNum * NUMCELLS) + CELLVOL ];
            *cell                = defaultCell;            
            cell->name           = "VOL";
            cell->cellFxns       = &VOL_CELLFXNS;            
            cell->algFxns        = (IALG_Fxns *)&VOL_IVOL;
            cell->algParams      = (IALG_Params *)&volParams;
            cell->scrBucketIndex = THRPROCESSSCRBUCKET;

            inputIcc  = outputIcc;

            outputIcc = (ICC_Handle)ICC_linearCreate(
                                thrProcess.bufOutput[ chanNum ], 
                                FRAMELEN * sizeof( Sample ) );
            UTL_assert( outputIcc != NULL);

            // Only one input and one output ICC are needed.
            rc = CHAN_regCell( cell, &inputIcc, 1, &outputIcc, 1 );
        }
        else {

            /*
             *  Since firParams and volParams are local variables, they need to
             *  be set-up again for proper creation of the algorithms.
             */ 
            thrProcess.cellList[ (chanNum * NUMCELLS) + CELLFIR ].algParams = 
                (IALG_Params *)&firParams;
            thrProcess.cellList[ (chanNum * NUMCELLS) + CELLVOL ].algParams = 
                (IALG_Params *)&volParams;

            UTL_logDebug1("Channel Number: %d", chanNum);

            // open the channel: this causes the algorithms to be created
            rc = CHAN_open( &thrProcess.chanList[ chanNum ], 
                            &thrProcess.cellList[ chanNum * NUMCELLS ], 
                            NUMCELLS,
                            NULL );
        }
        UTL_assert( rc == TRUE );
    }
}


/*
 *  ======== checkMsg ========
 *
 *  Checks if there are any messages for the process and acts on them.
 *  Called from the thread's run() function.
 */
static Void checkMsg()
{
    CtrlMsg rxMsg;

    while( MBX_pend( mbxProcess, &rxMsg, 0) ) {
        Int chanNum;
        ICELL_Handle hCell;

        /* 
         *  The format we expect for a message, i.e. {cmd, arg1, arg2} is 
         *  1. volume change message: { MSGNEWVOL,   channel #, volume value },
         *  2. filter change message: { MSGNEWCOEFF, channel #, filter type}
         *     where filter type is 0: low pass, 1: high pass, 2: passthrough.
         */
        chanNum = rxMsg.arg1;
        UTL_assert( (chanNum >= 0) && (chanNum < NUMCHANNELS) );

        switch (rxMsg.cmd) {        
            case MSGNEWVOL:
                {
                    IVOL_Status volStatus;

                    /* get handle for channel chanNum's VOL cell */
                    hCell = &thrProcess.cellList[CELLVOL + chanNum * NUMCELLS];
                    UTL_assert( hCell != NULL );

                    /* 
                     *  get current contents of the control structure 
                     *  to avoid overriding non-modified ones 
                     */
                    VOL_cellControl( hCell, IVOL_GETSTATUS, 
                                     (IALG_Status *)&volStatus );
                    volStatus.gainPercentage = rxMsg.arg2; 
                    VOL_cellControl( hCell, IVOL_SETSTATUS, 
                                     (IALG_Status *)&volStatus );
                }
                break;

            case MSGNEWCOEFF:
                {
                    IFIR_Status firStatus;
                    Short *coeffPtr;

                    /* pick the right set of coefficients, use arg2 as index */
                    UTL_assert( rxMsg.arg2 <= 2 );
                    coeffPtr = filterCoeffList[ rxMsg.arg2 ];

                    /* get handle for channel chanNum's FIR cell */
                    hCell = &thrProcess.cellList[CELLFIR + chanNum * NUMCELLS];
                    UTL_assert( hCell != NULL );

                    /* 
                     *  get current contents of the control structure 
                     *  to avoid overriding non-modified ones 
                     */
                    FIR_cellControl( hCell, IFIR_GETSTATUS, 
                                     (IALG_Status *)&firStatus );
                    firStatus.coeffPtr  = coeffPtr; 
                    FIR_cellControl( hCell, IFIR_SETSTATUS, 
                                     (IALG_Status *)&firStatus );
                }
                break;

            default:
                break;
        }
    }
}


/*
 *  ======== thrProcessRun ========
 *
 *  Main function of the Process thread.
 */
Void thrProcessRun()
{
    Int i, chanNum;
    Bool rc;    

    /* open SCOM queues for sending messages to RxSplit and TxJoin tasks */
    SCOM_Handle scomReceiveFromRx = SCOM_open( "scomToProcessFromRx");
    SCOM_Handle scomReceiveFromTx = SCOM_open( "scomToProcessFromTx");
    SCOM_Handle scomSendToRx      = SCOM_open( "scomRxSplit" );
    SCOM_Handle scomSendToTx      = SCOM_open( "scomTxJoin"  );

    UTL_assert( scomReceiveFromRx != NULL );
    UTL_assert( scomReceiveFromTx != NULL );
    UTL_assert( scomSendToRx      != NULL );
    UTL_assert( scomSendToTx      != NULL );

    // Fill in the SCOM messages
    for (i = 0; i < NUMCHANNELS; i++) {
        thrProcess.scomMsgRx.bufChannel[i] = thrProcess.bufInput[i];
        thrProcess.scomMsgTx.bufChannel[i] = thrProcess.bufOutput[i];
    }

    // put the Rx message on the SCOM queue for RxSplit thread
    SCOM_putMsg( scomSendToRx, &(thrProcess.scomMsgRx) );

    // put the Tx message on the queue to yourself, i.e. from TxJoin thread
    SCOM_putMsg( scomReceiveFromTx, &(thrProcess.scomMsgTx) );

    // Main loop
    while (TRUE) {
        ScomBufChannels *scomMsgRx, *scomMsgTx;

        // check for control (MBX) messages (not to be confused with SCOM msgs)
        checkMsg();

        // get the message describing full input buffers from Rx
        scomMsgRx = (ScomBufChannels *)SCOM_getMsg( scomReceiveFromRx, 
            SYS_FOREVER );

        // get the message describing empty output buffers from Tx
        scomMsgTx = (ScomBufChannels *)SCOM_getMsg( scomReceiveFromTx, 
            SYS_FOREVER );

        // record the time period between two frames of data in stsTime0
        UTL_stsPeriod( stsTime0 );

        // process the data
        for( chanNum = 0; chanNum < NUMCHANNELS; chanNum++ ) {

            CHAN_Handle chanHandle = &thrProcess.chanList[ chanNum ];

            // Set the input ICC buffer for FIR cell for each channel
            ICC_setBuf(chanHandle->cellSet[CELLFIR].inputIcc[0],
                       scomMsgRx->bufChannel[chanNum], 
                       FRAMELEN * sizeof( Sample ) );

            // Set the output ICC buffer for VOL cell for each channel
            ICC_setBuf(chanHandle->cellSet[CELLVOL].outputIcc[0],
                       scomMsgTx->bufChannel[chanNum], 
                       FRAMELEN * sizeof( Sample ) );

            // execute the channel 
            UTL_stsStart( stsTime1 );  // start the stopwatch
            rc = CHAN_execute( chanHandle, NULL );
            UTL_assert( rc == TRUE );
            UTL_stsStop( stsTime1 );   // elapsed time goes to this STS 
        }
        
        // send the message describing full output buffers to Tx
        SCOM_putMsg( scomSendToTx, scomMsgTx );
        
        // send the message describing consumed input buffers to Rx
        SCOM_putMsg( scomSendToRx, scomMsgRx );
    }
}

