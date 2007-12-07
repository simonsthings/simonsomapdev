/** ============================================================================
 *  @file   Benchmarking.c
 *
 *  @path   $(PROJROOT)\src\samples\benchmarking
 *
 *  @desc   This is a Benchmarking application which sends a buffer across the
 *          DSP processor and measures the performance of the transfer. In this
 *          application the transfer is done using only one buffer and no
 *          queueing of buffers is done.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  This software is copyright to Texas Instruments Incorporated.
 *  Its usage is governed by the terms and conditions stipulated in
 *  the license agreement under which this software has been supplied.
 *
 *  The recipient of this software implicitly accepts the terms of
 *  the license.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers           */
#include <stdio.h>
#include <stdlib.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>

/*  ----------------------------------- Processor Manager             */
#include <proc.h>
#include <chnl.h>

/*  ----------------------------------- Application Header            */
#include <benchmarking.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   dspExecutable
 *
 *  @desc   Complete path of DSP executable.
 *  ============================================================================
 */
Char8 * dspExecutable = NULL ;

/** ============================================================================
 *  @name   outputBufArray
 *
 *  @desc   Array of buffers used by output channel.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
Char8 * outputBufArray [1] ;

/** ============================================================================
 *  @name   ioReqOutput
 *
 *  @desc   It gives information for adding or reclaiming an output request.
 *  ============================================================================
 */
ChannelIOInfo ioReqOutput ;

/** ============================================================================
 *  @name   noOfArguments
 *
 *  @desc   Number of arguments to be passed to the DSP side.
 *  ============================================================================
 */
Uint32 noOfArguments ;

/** ============================================================================
 *  @name   arrayOfArguments
 *
 *  @desc   Array of arguments to be passed to the DSP side.
 *  ============================================================================
 */
Char8 * arrayOfArguments [MAX_ARGUMENTS] ;

/** ============================================================================
 *  @name   bufferSize
 *
 *  @desc   The size of buffer transferred between GPP & DSP.
 *  ============================================================================
 */
Uint32 bufferSize ;

/** ============================================================================
 *  @name   numTransfers
 *
 *  @desc   Number of times the data transfer is done.
 *  ============================================================================
 */
Uint32 numTransfers ;


/** ============================================================================
 *  @func   Benchmarking_Create
 *
 *  @desc   This function allocates nesessary resources.
 *
 *  @modif  outputBufArray
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
Benchmarking_Create ()
{
    DSP_STATUS    status         = DSP_SOK ;
    ChannelAttrs  chnlAttrOutput           ;

    printf ("Entered create  phase\n") ;
    /*  Create and initialize the proc object.
     */
    status = PROC_Setup () ;

    /*  Attach the Dsp with which the tranfers have to be done.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (ID_PROCESSOR, NULL) ;
    }

    /*  Create a channel to DSP
     */
    if (DSP_SUCCEEDED (status)) {
        chnlAttrOutput.mode      = ChannelMode_Output     ;
        chnlAttrOutput.endianism = Endianism_Default      ;
        chnlAttrOutput.size      = ChannelDataSize_16bits ;

        status = CHNL_Create (ID_PROCESSOR, CHNL_ID_OUTPUT, &chnlAttrOutput) ;
    }

    /*  Allocate buffer(s) for data transfer to DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (ID_PROCESSOR,
                                      CHNL_ID_OUTPUT,
                                      outputBufArray,
                                      bufferSize ,
                                      1) ;
    }

    if (DSP_SUCCEEDED (status)) {
        /*  Load the executable on the DSP.
         */
        status = PROC_Load (ID_PROCESSOR,
                            dspExecutable,
                            noOfArguments,
                            arrayOfArguments) ;
    }


    printf ("Leaving create  phase\n") ;

    return status ;
}


/** ============================================================================
 *  @func   Benchmarking_Execute
 *
 *  @desc   This function executes the Benchmarking application.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
Benchmarking_Execute ()
{
    DSP_STATUS status = DSP_SOK ;
    Uint32     i                ;

    printf ("Entered execute phase\n") ;

    /*  Start execution on DSP.
     */
    status = PROC_Start (ID_PROCESSOR) ;

    /*  Fill the IO Request structure
     *  It gives Information for adding or reclaiming an output request.
     */
    ioReqOutput.buffer = outputBufArray [0] ;
    ioReqOutput.size   = bufferSize   ;

    for (i = 1 ; (i <= numTransfers) && (DSP_SUCCEEDED (status)) ; i++) {
        /*  Send data to DSP.
         *  Issue 'filled' buffer to the channel.
         */
        status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_OUTPUT, &ioReqOutput) ;

        /*  Reclaim 'empty' buffer from the channel
         */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (ID_PROCESSOR,
                                   CHNL_ID_OUTPUT,
                                   WAIT_FOREVER,
                                   &ioReqOutput) ;
        }
        if ((i % 1000) == 0) {
            printf ("Transferred %ld buffers.\n", i) ;
        }
    }

    /*  Stop execution on DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Stop (ID_PROCESSOR) ;
    }

    printf ("Leaving execute phase\n") ;

    return status ;
}


/** ============================================================================
 *  @func   Benchmarking_Delete
 *
 *  @desc   This function deallocates resources allocated earlier by call to
 *          Benchmarking_Create (). During cleanup, the allocated resources are
 *          being freed unconditionally. Actual applications may require
 *          stricter check against return values for robustness.
 *
 *  @modif  outputBufArray
 *  ============================================================================
 */
NORMAL_API
Void
Benchmarking_Delete ()
{
    printf ("Entered delete  phase\n") ;

    /*  Free the buffer(s) allocated for channel to DSP
     */
    CHNL_FreeBuffer (ID_PROCESSOR,
                     CHNL_ID_OUTPUT,
                     outputBufArray,
                     1) ;

    /*  Delete the output channel
     */
    CHNL_Delete  (ID_PROCESSOR, CHNL_ID_OUTPUT) ;

    /*  Detach from the processor
     */
    PROC_Detach  (ID_PROCESSOR) ;

    /*  Destroy the PROC object.
     */
    PROC_Destroy () ;

    printf ("Leaving delete  phase\n") ;
}


/** ============================================================================
 *  @func   main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  dspExecutable
 *  ============================================================================
 */
int
main (int argc, char ** argv)
{
    DSP_STATUS status = DSP_SOK ;

    printf ("========= Sample Application : Benchmarking ==========\n") ;

    if (argc != 4) {
        printf ("Usage : "
                "benchmarkinggpp <absolute path of DSP executable> \n"
                "<Buffer Size> <Number of transfers>\n") ;
        status = DSP_EFAIL ;
    }
    else {

        /*  Read name of DSP executable from command-line
         */
        dspExecutable        = argv [1]        ;
        noOfArguments        = 2               ;
        arrayOfArguments [0] = argv [2]        ;
        arrayOfArguments [1] = argv [3]        ;
        bufferSize           = atoi (argv [2]) ;
        numTransfers         = atoi (argv [3]) ;

        /*  Allocate necessary resources for the application
         */
        status = Benchmarking_Create () ;

        /*  Do the data transfer
         */
        if (DSP_SUCCEEDED (status)) {
            status = Benchmarking_Execute () ;
        }

        /*  Cleanup. Deallocate used resources.
         */
        Benchmarking_Delete () ;
    }

    printf ("==============================================\n") ;

    return 0 ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
