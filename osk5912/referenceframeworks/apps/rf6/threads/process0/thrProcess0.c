/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrProcess0.c ========
 *
 *  Implementation of the Process0 thread. It performs the processing done on 
 *  the first data path: Codec -> Processing -> Link.
 * 
 *  This thread is a typical data processing thread. It consists of 
 *  NUMCHANNELS channels, each consisting of two cells. A cell is a 
 *  wrapper around an XDAIS algorithm that gives it a uniform data processing 
 *  interface. (since XDAIS algorithms have different processing functions with 
 *  different signatures). A channel is a collection of cells, which execute in 
 *  series. 
 */

#include <std.h>

// DSP/BIOS includes
#include <sem.h>
#include <msgq.h>

// RF module includes
#include <chan.h>
#include <icell.h>
#include <icc.h>
#include <icc_linear.h>
#include <utl.h>

// application includes
#include "appMsgqConfig.h"  // MSGQ configuration info
#include "appResources.h"
#include "appThreads.h"

// Thread includes
#include "thrProcess0.h"
#include "control/thrControl.h"

// Cell includes
#include "fir/cellFir.h"
#include "vol/cellVol.h"

/*
 *   Incoming data goes into bufInputs.
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
static Sample bufIntermediate[ FRAMELEN ];

/*
 *  Thread process0 object which encapsulates the state information 
 *  of the thread.
 */ 
ThrProcess0 thrProcess0;

/* 
 *  FIR filter coefficients: each channel runs a different instance of
 *  the FIR algorithm, and we initialize each instance with a different
 *  filter, i.e. with a different set of coefficients.
 */

// number of filter taps
#define NUMFIRTAPS  32

/* 
 *  The following filters have been designed with the compromise to let
 *  through a reasonable amount of audio content when used with sampling 
 *  rates from 8 to 48 kHz. This is due to the wide variety of default 
 *  sampling rates supported in the various ports of the Reference Frameworks.
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

// the list of all coefficient arrays
static Short * filterCoeffList[] = { 
    filterCoeffLowPass, filterCoeffHighPass, filterCoeffPassThrough
};


// Local function prototypes
static Void setParamsAndStartChannels( Bool doChannelOpen );

/*
 *  ======== thrProcess0Init ========
 *
 *  Initialization of the thread's object structure, creation of
 *  MSGQ queues, and registration of cells (via setParamsAndStartChannels()
 *  function below).
 */
Void thrProcess0Init()
{
    Int i;
    MSGQ_Attrs msgqAttrs;
    
    // Set-up the threads's buffer pointers. 
    for (i = 0; i < NUMCHANNELS; i++) {
        thrProcess0.bufInput[i]  = bufInput[i];
    }
    thrProcess0.bufIntermediate = bufIntermediate;

    /* 
     *  Create the receiving message queue. Each reader creates its own
     *  message queue. There is one queue per task.           
     */    
    SEM_new(&thrProcess0.msgqSemObj, 0);
    
    msgqAttrs              = MSGQ_ATTRS;
    msgqAttrs.notifyHandle = (Ptr)&thrProcess0.msgqSemObj;
    msgqAttrs.pend         = (MSGQ_Pend)binarySemPend;
    msgqAttrs.post         = (MSGQ_Post)SEM_post;
    
    thrProcess0.msgQueue = MSGQ_create("msgqProcess0", &msgqAttrs);
    UTL_assert( thrProcess0.msgQueue != NULL );
        
    setParamsAndStartChannels( FALSE );
}

/*
 *  ======== thrProcess0Startup ========
 *
 *  Thread startup: post-initialization activation. Cells that were 
 *  registered in thrProcess0Init(), via setParamsAndStartChannels(),
 *  are now actually created when the channel is created.
 */
Void thrProcess0Startup()
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
        firParams.coeffPtr  = filterCoeffLowPass; // and override fields
        firParams.filterLen = NUMFIRTAPS;
        firParams.frameLen  = FRAMELEN;

        volParams           = IVOL_PARAMS;
        volParams.frameSize = FRAMELEN;        

        /* 
         *  register the cells: define what will be input buffers and
         *  what will be output buffers for each cell.
         */
        if (doChannelOpen == FALSE) {
            
            // Setup a default cell used to initialize the actual cells
            ICELL_Obj   defaultCell = ICELL_DEFAULT;

            /*
             *  cell 0 - FIR: create an input linear ICC buffer that points to
             *  bufInput[ <channel number> ], and an output linear ICC buffer
             *  that points to the intermediate buffer, bufIntermediate;
             *  the cell will always look for data in those buffers.
             */
            cell = &thrProcess0.cellList[ (chanNum * THRPROCESS0_NUMCELLS) +
                THRPROCESS0_CELLFIR ];
            *cell                = defaultCell;
            cell->name           = "FIR";
            cell->cellFxns       = &FIR_CELLFXNS;            
            cell->algFxns        = (IALG_Fxns *)&FIR_IFIR;
            cell->algParams      = (IALG_Params *)&firParams;
            cell->scrBucketIndex = THRPROCESSSCRBUCKET;

            inputIcc  = (ICC_Handle)ICC_linearCreate( 
                                   thrProcess0.bufInput[ chanNum ], 
                                   FRAMELEN * sizeof( Sample ) );
            UTL_assert( inputIcc != NULL);

            outputIcc = (ICC_Handle)ICC_linearCreate( 
                                                thrProcess0.bufIntermediate,
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
            cell = &thrProcess0.cellList[ (chanNum * THRPROCESS0_NUMCELLS) + 
                THRPROCESS0_CELLVOL ];
            *cell                = defaultCell;            
            cell->name           = "VOL";
            cell->cellFxns       = &VOL_CELLFXNS;            
            cell->algFxns        = (IALG_Fxns *)&VOL_IVOL;
            cell->algParams      = (IALG_Params *)&volParams;
            cell->scrBucketIndex = THRPROCESSSCRBUCKET;

            inputIcc  = outputIcc;

            outputIcc = (ICC_Handle)ICC_linearCreate(
                                thrProcess0.bufOutput[ chanNum ], 
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
            thrProcess0.cellList[ (chanNum * THRPROCESS0_NUMCELLS) + 
                THRPROCESS0_CELLFIR ].algParams = (IALG_Params *)&firParams;
            thrProcess0.cellList[ (chanNum * THRPROCESS0_NUMCELLS) + 
                THRPROCESS0_CELLVOL ].algParams = (IALG_Params *)&volParams;

            UTL_logDebug1("Channel Number: %d", chanNum);

            // open the channel: this causes the algorithms to be created
            rc = CHAN_open( &thrProcess0.chanList[ chanNum ], 
                &thrProcess0.cellList[ chanNum * THRPROCESS0_NUMCELLS ], 
                THRPROCESS0_NUMCELLS,
                NULL );
        }
        UTL_assert( rc == TRUE );
    }
}

/*
 *  ======== processMsg ========
 *
 *  Processes messages for the task.
 *  Called from the thread's run() function.
 */
static Void processMsg(CtrlMsg *rxMsg)
{
    Int chanNum;
    ICELL_Handle hCell;

    /* 
     *  The format we expect for a message, i.e. {cmd, arg1, arg2} is 
     *  1. volume change message: { MSGNEWVOL,   channel #, volume value },
     *  2. filter change message: { MSGNEWCOEFF, channel #, filter type}
     *     where filter type is 0: low pass, 1: high pass, 2: passthrough.
     */
    chanNum = rxMsg->arg1;
        
    // Process only messages with valid channel number
    if ( (chanNum >= 0) && (chanNum < NUMCHANNELS) ) {

        switch (rxMsg->cmd) {        
            case MSGNEWVOL:
                {
                    IVOL_Status volStatus;
                
                    // get handle for channel chanNum's VOL cell 
                    hCell = &thrProcess0.cellList[THRPROCESS0_CELLVOL +
                        chanNum * THRPROCESS0_NUMCELLS];
                    UTL_assert( hCell != NULL );

                    /* 
                     *  get current contents of the control structure 
                     *  to avoid overriding non-modified ones 
                     */
                    VOL_cellControl( hCell, IVOL_GETSTATUS, 
                        (IALG_Status *)&volStatus );
                    volStatus.gainPercentage = rxMsg->arg2; 
                    VOL_cellControl( hCell, IVOL_SETSTATUS, 
                        (IALG_Status *)&volStatus );
                                     
                    UTL_logDebug2( "Changing volume on channel %d to %d", 
                        chanNum, volStatus.gainPercentage);
                }
                break;

            case MSGNEWCOEFF:
                {
                    IFIR_Status firStatus;
                    Short *coeffPtr;
                    Int coeffIndex;

                    // pick the right set of coefficients -- use arg2 as index 
                    coeffIndex = rxMsg->arg2;
                    UTL_assert( coeffIndex  <= 2 );
                    coeffPtr = filterCoeffList[ coeffIndex ];

                    // get handle for channel chanNum's FIR cell
                    hCell = &thrProcess0.cellList[THRPROCESS0_CELLFIR +  
                        chanNum * THRPROCESS0_NUMCELLS];
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
                    UTL_logDebug2( "Changing coeffs on channel %d to %d", 
                        chanNum, coeffIndex);
                }
                break;

            default:
                break;
        }
    }
}


/*
 *  ======== thrProcess0Run ========
 *
 *  Main function of the Process0 thread.
 */
Void thrProcess0Run()
{
    Int i;
    Int chanNum;
    Bool rc;    
    Uns msgId;
    MSGQ_Msg msg;
    LocalDataMsg * msgPreProcess, * msgPostProcess;
    MSGQ_Status status;
    MSGQ_Handle msgqToPreProcess, msgqToPostProcess; 
    
    /* 
     * Locate the message queues of pre and post processing tasks 
     * Note that we are using the blocking (synchronous) mode of the 
     * MSGQ_locate API
     */
    status = MSGQ_locate("msgqPreProcessCodec", &msgqToPreProcess, NULL);       
    UTL_assert( status == MSGQ_SUCCESS );
    status = MSGQ_locate("msgqPostProcessLink", &msgqToPostProcess, NULL);      
    UTL_assert( status == MSGQ_SUCCESS );
        
    // Fill in the priming message
    msgPreProcess = (LocalDataMsg *)MSGQ_alloc(LOCALMQABUFID, 
        sizeof(LocalDataMsg));
    UTL_assert( msgPreProcess != NULL );
    for (i = 0; i < NUMCHANNELS; i++) {
        msgPreProcess->bufChannel[i] = thrProcess0.bufInput[i];
    }

    /*
     *  Prime the MSGQ of the preceding task in the processing chain.
     *  The convention here is for the task appearing later in the data flow
     *  to prime the MSGQ of the task(s) preceding it.
     */ 
    status = MSGQ_put(msgqToPreProcess, (MSGQ_Msg)msgPreProcess,
        PROCESS0_DATAMSGID, NULL);
    UTL_assert( status == MSGQ_SUCCESS );
    
    /* Reset the message pointers */
    msgPreProcess = NULL;
    msgPostProcess = NULL;   
     
    // Main loop
    while (TRUE) {
        msg = MSGQ_get(thrProcess0.msgQueue, SYS_FOREVER);
        UTL_assert(msg != NULL);  //MSGQ internal error
        msgId = MSGQ_getMsgId(msg);
        
        switch (msgId) {
            case CTRLMSGID:
                // process control messages
                processMsg((CtrlMsg *)msg);
                MSGQ_free(msg);
                break;
            case PREPROCESS_DATAMSGID:
                msgPreProcess = (LocalDataMsg *)msg;
                break;
            case POSTPROCESS_DATAMSGID:
                msgPostProcess = (LocalDataMsg *)msg;
                break;
            default:
                UTL_assert( msgId < NUMMSGIDS );  //Should never happen
                break;
        }

        if ((msgPostProcess != NULL) && (msgPreProcess != NULL)) {

            // record the time period between two frames of data in stsTime0
            UTL_stsPeriod( stsTime0 );

            // process the data
            for( chanNum = 0; chanNum < NUMCHANNELS; chanNum++ ) {

                CHAN_Handle chanHandle = &thrProcess0.chanList[ chanNum ];

                /*
                 *  Set up the input and output ICC buffers for each channel
                 *  The intermediate buffers never change as the same ones
                 *      are re-used for each frame of data
                 */
                // Set the input ICC buffer
                ICC_setBuf(chanHandle->cellSet[THRPROCESS0_CELLFIR].inputIcc[0],
                    msgPreProcess->bufChannel[chanNum], 
                    FRAMELEN * sizeof( Sample ) );

                // Set the output ICC buffer
                ICC_setBuf(chanHandle->cellSet[THRPROCESS0_CELLVOL].outputIcc[0],
                    msgPostProcess->bufChannel[chanNum], 
                    FRAMELEN * sizeof( Sample ) );

                // execute the channel 
                UTL_stsStart( stsTime1 );  // start the stopwatch
                rc = CHAN_execute( chanHandle, NULL );
                UTL_assert( rc == TRUE );
                UTL_stsStop( stsTime1 );   // elapsed time goes to this STS 
            }
            
            // send the message describing full output buffers to PostProcess
            status = MSGQ_put(msgqToPostProcess, (MSGQ_Msg)msgPostProcess,
                PROCESS0_DATAMSGID, NULL);
            UTL_assert( status == MSGQ_SUCCESS );
            
            // send the message describing consumed input buffers to PreProcess
            status = MSGQ_put(msgqToPreProcess, (MSGQ_Msg)msgPreProcess, 
                PROCESS0_DATAMSGID, NULL);
            UTL_assert( status == MSGQ_SUCCESS );
            
            //Reset the pointers so we are ready for next frame
            msgPostProcess = NULL;
            msgPreProcess = NULL;
        }
    }
}

