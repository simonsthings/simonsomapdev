/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appMain.c ========
 */

/* Preprocessor includes, definitions */
#include <std.h>

#include <pip.h>    

#include <utl.h>            /* debug/diagnostics utility functions */

#include "appIO.h"          /* application IO initialization, priming */

#include "firapp.h"         /* algorithm's application interface(s) */

#include "appMain.h"        /* application function declarations */
#include "appBiosObjects.h"     /* BIOS Application-wide constants */


/* Rx and Tx flags set on next IDL loop spin when respective condition met */
volatile Bool inputReadyFlag =  FALSE;   /* Set when Rx buffer is full */
volatile Bool outputReadyFlag = FALSE;   /* Set when Tx buf ready for data */

/*  
 *  Macro, which expands to nothing if UTL_sts* functionality is disabled via
 *  reduced UTL_DBGLEVEL setting. Otherwise, it defines the UTLSTS object.
 *  All UTL fxns may be included/excluded by increasing/decreasing UTL_DBGLEVEL
 */
UTL_stsDefine( audioProcessExecTime );

/* Declaration of XDAIS v-tables. These are the algorithm(s) entry pts */
extern IFIR_Fxns    FIR_IFIR;      /* FIR algorithm */

/* Handles to the XDAIS algorithm(s). Provides reentrant access to the fxns */
FIR_Handle          firHandle0; 

/* local function prototypes */
static Void audioProcess(PIP_Handle in, PIP_Handle out);


/*
 *  ======== main ========
 */
Void main()
{
    /* Prepare buffer pointer array for Static XDAIS Algorithm instantiation */
    Char *firChanBufs[] = {firChanBufId00, firChanBufId01, firChanBufId02Scr};
        
    /* name LOG objects to be used for error/warning/general/debug messages */
    UTL_setLogs( &trace, &trace, &trace, &trace );    
        
    /* greet the user */
    UTL_logDebug( "RF Level 1 started" );

    /* initialize I/O */
    appIOInit();
    
    /* Initialize the XDAIS algorithm(s) */
    FIR_init();
    
    /* Statically create an instance of the XDAIS algo(s) ie no heaps etc */
    firHandle0 = FIR_new( &FIR_IFIR, FIR_chanParamPtrs[0], 
            firChanBufs, (sizeof(firChanBufs) / sizeof(firChanBufs[0])) );
    
    /* Confirm the instantiation of XDAIS algorithm(s) was successful */
    UTL_assert( firHandle0 != NULL );
                                    
    /* prime I/O */
    appIOPrime();

    /* Fall into BIOS idle loop */
    return;                 
}


/*
 *  ======== audioProcess ========
 * 
 *  Heart of the processing. 
 *  Note that slight differences in processing for various h/w codecs, 
 *  and stereo .v. mono are not accounted for here since user should replace it
 *  with app-specific code. 
 *  Examples of minor variations include :-
 *  (i) ADxxx codecs require low-bit masked off, else its treated as ctrl word
 *  (ii) mono codecs are typically 16 bits, while stereo codec drivers may pack
 *  16-bits Left, 16-bits Right together. Some algorithms expect left, right
 *  packed, others require separated streams.
 *  This fxn simply filters the data, masks LSB, independent of such variations
 */
static Void audioProcess(PIP_Handle in, PIP_Handle out)
{
    Uns *src, *dst;
    Uns size, i;
    
    /*
     *  Check that preconditions are met, i.e. the in-pipe has a 
     *  ready-to-consume buffer of data and the out-pipe has a free buffer.
     *  In other words that this thread has not been posted in error.
     */
    UTL_assert( PIP_getReaderNumFrames(in ) > 0 );
    UTL_assert( PIP_getWriterNumFrames(out) > 0 );

    /* 
     * Get the full buffer from the receive PIP.
     * Get the size in samples (since PIP_getReaderSize returns it in words) 
     */
    PIP_get(in);
    src = (Uns *)PIP_getReaderAddr(in);
    size = PIP_getReaderSize(in) * sizeof(short);

    /* get the empty buffer from the out-pipe */
    PIP_alloc(out);
    dst = (Uns *)PIP_getWriterAddr(out);

    /* Execute the XDAIS Algorithm(s) */         
    FIR_apply(firHandle0, (Short *)src, (Short *)dst);       
    
    /*
     * Mask off the low bit for compatibility with those codecs
     * that interpret a low bit of '1' as a command flag.
     */
    for (i = 0; i < size; i++) {
        dst[i] = dst[i] & 0xfffe;
    }
     
    /* 
     *  Record the amount of actual data being sent
     *  Then put/output the full transmit buffer.
     */
    PIP_setWriterSize(out, size);
    PIP_put(out);
    
    /* Free up the input/receive buffer */
    PIP_free(in);    

} /* end of audioProcess() */


/*
 *  ======== setFlags ========
 */
Void setFlags(Arg flagSetting)
{   
    switch (ArgToInt(flagSetting)) {
    case 0x1:
        inputReadyFlag = TRUE;      /* input buffer ready to be processed */
        break;
    case 0x2:        
        outputReadyFlag = TRUE;     /* output buffer has been processed */
        break;
    default:
        UTL_assert( FALSE );        /* Error. Should never be a 'bad' flag */
    } /* end switch */

} /* end of setFlags() */


/*
 *  ======== checkFlags ========
 */
Void checkFlags()
{   
    if (inputReadyFlag && outputReadyFlag) {

        /* Begin timing of processing from 'Ready' to 'Completion' */           
        UTL_stsStart( audioProcessExecTime );
                
        inputReadyFlag =  FALSE;                /* Reset flags */
        outputReadyFlag = FALSE;
                
        audioProcess(&pipRx, &pipTx);   /* Process Data */

        /* Mark end of timing interval */   
        UTL_stsStop( audioProcessExecTime );
    }

} /* end of checkFlags() */




