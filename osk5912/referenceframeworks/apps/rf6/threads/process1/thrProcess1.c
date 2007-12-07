/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== thrProcess1.c ========
 *
 *  Implementation of the Process1 thread. It performs the processing done on 
 *  the second data path: Link -> Processing -> Codec.
 *  
 *  This thread is a typical data processing thread. It consists of 
 *  NUMCHANNELS channels, each consisting of one cell. A cell is a 
 *  wrapper around an XDAIS algorithm that gives it a uniform data processing 
 *  interface. (since XDAIS algorithms have different processing functions with 
 *  different signatures). A channel is a collection of cells, which execute in 
 *  series. 
 */

#include <std.h>

// DSP/BIOS includes
#include <sem.h>

// RF module includes
#include <chan.h>
#include <icell.h>
#include <msgq.h>
#include <icc.h>
#include <icc_linear.h>
#include <utl.h>

// application includes
#include "appMsgqConfig.h"  // MSGQ configuration info
#include "appResources.h"
#include "appThreads.h"

// Thread includes
#include "thrProcess1.h"
#include "control/thrControl.h"

// Cell includes
#include "vol/cellVol.h"

/*
 *  Incoming data goes into bufInputs.
 */
static Sample bufInput[ NUMCHANNELS ][ FRAMELEN ];

/*
 *  Thread process1 object which encapsulates the state information 
 *  of the thread.
 */ 
ThrProcess1 thrProcess1;

// Local function prototypes
static Void setParamsAndStartChannels( Bool doChannelOpen );

/*
 *  ======== thrProcess1Init ========
 *
 *  Initialization of the thread's object structure, creation of
 *  MSGQ queues, and registration of cells (via setParamsAndStartChannels()
 *  function below).
 */
Void thrProcess1Init()
{
    Int i;
    MSGQ_Attrs msgqAttrs;
    
    // Set-up the threads's buffer pointers.
    for (i = 0; i < NUMCHANNELS; i++) {
        thrProcess1.bufInput[i]  = bufInput[i];
    }

    /* 
     *  Create the receiving message queue. Each reader creates its own
     *   message queue. There is one queue per task.           
     */    
    SEM_new(&thrProcess1.msgqSemObj, 0);

    msgqAttrs              = MSGQ_ATTRS;
    msgqAttrs.notifyHandle = (Ptr)&thrProcess1.msgqSemObj;
    msgqAttrs.pend         = (MSGQ_Pend)binarySemPend;
    msgqAttrs.post         = (MSGQ_Post)SEM_post;
    
    thrProcess1.msgQueue = MSGQ_create("msgqProcess1", &msgqAttrs);
    UTL_assert( thrProcess1.msgQueue != NULL );
        
    setParamsAndStartChannels( FALSE );
}

/*
 *  ======== thrProcess1Startup ========
 *
 *  Thread startup: post-initialization activation. Cells that were 
 *  registered in thrProcess1Init(), via setParamsAndStartChannels(),
 *  are now actually created when the channel is created.
 */
Void thrProcess1Startup()
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
    IVOL_Params volParams;    
    Int         chanNum;
    ICELL_Obj  *cell;
    Bool        rc;    
    ICC_Handle  inputIcc;
    ICC_Handle  outputIcc;
    

    for (chanNum = 0; chanNum < NUMCHANNELS; chanNum++) {

        // prepare parameters for VOL
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
             *  cell 0 - VOL: create an input linear ICC buffer that points to
             *  the intermediate buffer, bufIntermediate, and an output linear 
             *  ICC buffer that points to bufOutput[ <channel number> ].
             *  the cell will always look for data in those buffers.
             */
            cell = &thrProcess1.cellList[ (chanNum * THRPROCESS1_NUMCELLS) + 
                THRPROCESS1_CELLVOL ];
            *cell                = defaultCell;            
            cell->name           = "VOL";
            cell->cellFxns       = &VOL_CELLFXNS;            
            cell->algFxns        = (IALG_Fxns *)&VOL_IVOL;
            cell->algParams      = (IALG_Params *)&volParams;
            cell->scrBucketIndex = THRPROCESSSCRBUCKET;

            inputIcc  = (ICC_Handle)ICC_linearCreate( 
                                   thrProcess1.bufInput[ chanNum ], 
                                   FRAMELEN * sizeof( Sample ) );
            UTL_assert(inputIcc != NULL);

            outputIcc = (ICC_Handle)ICC_linearCreate(
                                thrProcess1.bufOutput[ chanNum ], 
                                FRAMELEN * sizeof( Sample ) );
            UTL_assert(outputIcc != NULL);

            // Only one input and one output ICC are needed.
            rc = CHAN_regCell( cell, &inputIcc, 1, &outputIcc, 1 );
        }
        else {

            /*
             *  Since volParams is a local variable, it needs to
             *  be set-up again for proper creation of the algorithms.
             */ 
            thrProcess1.cellList[ (chanNum * THRPROCESS1_NUMCELLS) + 
                THRPROCESS1_CELLVOL ].algParams = (IALG_Params *)&volParams;

            UTL_logDebug1("Channel Number: %d", chanNum);

            // open the channel: this causes the algorithms to be created
            rc = CHAN_open( &thrProcess1.chanList[ chanNum ], 
                &thrProcess1.cellList[ chanNum * THRPROCESS1_NUMCELLS ], 
                THRPROCESS1_NUMCELLS,
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
     *  The format we expect for a message, i.e. {cmd, arg1, arg2} is: 
     *  volume change message: { MSGNEWVOL, channel #, volume value },
     */
    chanNum = rxMsg->arg1;
        
    // Process only messages with valid channel number
    if ( (chanNum >= 0) && (chanNum < NUMCHANNELS) ) {
        switch (rxMsg->cmd) {        
            case MSGNEWVOL:
                {
                    IVOL_Status volStatus;
                    
                    // get handle for channel chanNum's VOL cell 
                    hCell = &thrProcess1.cellList[THRPROCESS1_CELLVOL +  
                        chanNum * THRPROCESS1_NUMCELLS];
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
                                     
                    UTL_logDebug2("Changing volume on channel %d to %d", 
                        chanNum, volStatus.gainPercentage);
                }
                break;

            default:
                break;
        }
    }
}

/*
 *  ======== thrProcess1Run ========
 *
 *  Main function of the Process1 thread.
 */
Void thrProcess1Run()
{
    Int i;
    Int chanNum;
    Bool rc; 
    Uns msgId;   
    MSGQ_Msg msg;
    LocalDataMsg *msgPreProcess, *msgPostProcess;
    MSGQ_Status status;
    MSGQ_Handle msgqToPreProcess, msgqToPostProcess; 

    /* 
     * Locate the message queues of pre and post processing tasks 
     * Note that we are using the blocking (synchronous) mode of the 
     * MSGQ_locate API
     */
    status = MSGQ_locate("msgqPreProcessLink", &msgqToPreProcess, NULL);        
    UTL_assert( status == MSGQ_SUCCESS );
    status = MSGQ_locate("msgqPostProcessCodec", &msgqToPostProcess, NULL);     
    UTL_assert( status == MSGQ_SUCCESS );

    // Fill in the priming message
    msgPreProcess = (LocalDataMsg *)MSGQ_alloc(LOCALMQABUFID,
        sizeof(LocalDataMsg));
    UTL_assert( msgPreProcess != NULL );
    for (i = 0; i < NUMCHANNELS; i++) {
        msgPreProcess->bufChannel[i] = thrProcess1.bufInput[i];
    }

    /*
     *  Prime the MSGQ of the preceding task in the processing chain.
     *  The convention here is for the task appearing later in the data flow
     *     to prime the MSGQ of the task(s) preceding it.
     */ 
    status = MSGQ_put(msgqToPreProcess, (MSGQ_Msg)msgPreProcess,
        PROCESS1_DATAMSGID, NULL);
    UTL_assert( status == MSGQ_SUCCESS );
    
    /* Reset the message pointers */    
    msgPreProcess = NULL;
    msgPostProcess = NULL;
     
    // Main loop
    while (TRUE) {
        msg = MSGQ_get(thrProcess1.msgQueue, SYS_FOREVER);
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

            // record the time period between two frames of data in stsTime2
            UTL_stsPeriod( stsTime2 );

            // process the data
            for( chanNum = 0; chanNum < NUMCHANNELS; chanNum++ ) {

                CHAN_Handle chanHandle = &thrProcess1.chanList[ chanNum ];

                /*
                 *  Set up the input and output ICC buffers for each channel
                 */
                // Set the input ICC buffer
                ICC_setBuf(chanHandle->cellSet[THRPROCESS1_CELLVOL].inputIcc[0],
                    msgPreProcess->bufChannel[chanNum], 
                    FRAMELEN * sizeof( Sample ) );

                // Set the output ICC buffer
                ICC_setBuf(chanHandle->cellSet[THRPROCESS1_CELLVOL].outputIcc[0],
                    msgPostProcess->bufChannel[chanNum], 
                    FRAMELEN * sizeof( Sample ) );

                // execute the channel 
                UTL_stsStart( stsTime3 );  // start the stopwatch
                rc = CHAN_execute( chanHandle, NULL );
                UTL_assert( rc == TRUE );
                UTL_stsStop( stsTime3 );   // elapsed time goes to this STS 
            }
            
            // send the message describing full output buffers to PostProcess
            status = MSGQ_put(msgqToPostProcess, (MSGQ_Msg)msgPostProcess,
                PROCESS1_DATAMSGID, NULL);
            UTL_assert( status == MSGQ_SUCCESS );
            
            // send the message describing consumed input buffers to PreProcess
            status = MSGQ_put(msgqToPreProcess, (MSGQ_Msg)msgPreProcess, 
                PROCESS1_DATAMSGID, NULL);
            UTL_assert( status == MSGQ_SUCCESS );
            
            //Reset the pointers so we are ready for next frame
            msgPostProcess = NULL;
            msgPreProcess = NULL;  
        }                  
    }
}

