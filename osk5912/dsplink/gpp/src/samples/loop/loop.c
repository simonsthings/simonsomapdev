/** ============================================================================
 *  @file   loop.c
 *
 *  @path   $(DSPLINK)\gpp\src\samples\loop
 *
 *  @desc   This is a loop application which sends a buffer across the DSP
 *          processor and recieves a buffer back using DSP/BIOS LINK.
 *          The data buffer received is verified aginst the data sent to DSP.
 *          This application transfers only one buffer and no buffer(s) are
 *          queued.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link                   */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif

/*  ----------------------------------- Profiling                       */
#include <profile.h>

/*  ----------------------------------- DSP/BIOS LINK API               */
#include <proc.h>
#include <chnl.h>

/*  ----------------------------------- Application Header              */
#include <loop.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
 *  @name   NUM_ARGS
 *
 *  @desc   Number of arguments specified to the DSP application.
 *  ============================================================================
 */
#define NUM_ARGS 2

/*  ============================================================================
 *  @name   XFER_CHAR
 *
 *  @desc   The value used to initialize the output buffer and used for
 *          validation against the input buffer recieved.
 *  ============================================================================
 */
#define XFER_CHAR   0xE7

/*  ============================================================================
 *  @name   LOOP_BufferSize
 *
 *  @desc   Size of buffer to be used for data transfer.
 *  ============================================================================
 */
STATIC Uint32  LOOP_BufferSize ;

/*  ============================================================================
 *  @name   LOOP_NumIterations
 *
 *  @desc   Size of buffer to be used for data transfer.
 *  ============================================================================
 */
STATIC Uint32  LOOP_NumIterations ;

/** ============================================================================
 *  @name   LOOP_InpBufs
 *
 *  @desc   Array of buffers used by input channel.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
STATIC Char8 * LOOP_InpBufs [1] ;

/** ============================================================================
 *  @name   LOOP_OutBufs
 *
 *  @desc   Array of buffers used by output channel.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
STATIC Char8 * LOOP_OutBufs [1] ;

/** ============================================================================
 *  @name   LOOP_InpIOReq
 *
 *  @desc   It gives information for adding or reclaiming an input request.
 *  ============================================================================
 */
STATIC ChannelIOInfo LOOP_InpIOReq  ;

/** ============================================================================
 *  @name   LOOP_OutIOReq
 *
 *  @desc   It gives information for adding or reclaiming an output request.
 *  ============================================================================
 */
STATIC ChannelIOInfo LOOP_OutIOReq ;


#if defined (VERIFY_DATA)
/** ----------------------------------------------------------------------------
 *  @func   LOOP_VerifyData
 *
 *  @desc   This function verifies the data-integrity of given buffer.
 *
 *  @arg    buf
 *              This is the pointer of the buffer whose contents are to be
 *              validated.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Contents of the input buffer and the output buffer are
 *              different.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    XFER_CHAR
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LOOP_VerifyData (IN Char8 * buf) ;
#endif /*  defined (VERIFY_DATA) */


/** ----------------------------------------------------------------------------
 *  @func   atoi
 *
 *  @desc   Extern function declaration to avoid the compiler warning.
 *
 *  @arg    str
 *              String representation of the number.
 *
 *  @ret    <valid integer>
 *              If the 'initial' part of the string represents a valid integer
 *          0
 *              If the string passed does not represent an integer or is NULL.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    XFER_CHAR
 *  ----------------------------------------------------------------------------
 */
EXTERN
int
atoi (char * str) ;


/** ============================================================================
 *  @func   LOOP_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @modif  LOOP_InpBufs , LOOP_OutBufs
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LOOP_Create (IN Char8 * dspExecutable,
             IN Char8 * strBufferSize,
             IN Char8 * strNumIterations)
{
    DSP_STATUS    status         = DSP_SOK ;
    Char8 *       temp           = NULL    ;
    Uint32        numArgs        = 0       ;
    ChannelAttrs  chnlAttrInput            ;
    ChannelAttrs  chnlAttrOutput           ;
    Uint16        i                        ;
    Char8 *       args [NUM_ARGS]          ;

    LOOP_0Print ("Entered LOOP_Create ()\n") ;

    /*
     *  Create and initialize the proc object.
     */
    status = PROC_Setup () ;

    /*
     *  Attach the Dsp with which the transfers have to be done.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (ID_PROCESSOR, NULL) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("PROC_Attach failed. Status = [0x%x]\n", status) ;
        }
    }
    else {
        LOOP_1Print ("PROC_Setup failed. Status =  [0x%x]\n", status) ;
    }

    /*
     *  Load the executable on the DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        numArgs  = NUM_ARGS         ;
        args [0] = strBufferSize    ;
        args [1] = strNumIterations ;

        status = PROC_Load (ID_PROCESSOR, dspExecutable, numArgs, args) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("PROC_Load failed. Status = [0x%x]\n", status) ;
        }
    }

    /*
     *  Create a channel to DSP
     */
    if (DSP_SUCCEEDED (status)) {
        chnlAttrOutput.mode      = ChannelMode_Output     ;
        chnlAttrOutput.endianism = Endianism_Default      ;
        chnlAttrOutput.size      = ChannelDataSize_16bits ;

        status = CHNL_Create (ID_PROCESSOR, CHNL_ID_OUTPUT, &chnlAttrOutput) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("CHNL_Create failed (output). Status = [0x%x]\n",
                         status) ;
        }
    }

    /*
     *  Create a channel from DSP
     */
    if (DSP_SUCCEEDED (status)) {
        chnlAttrInput.mode      = ChannelMode_Input      ;
        chnlAttrInput.endianism = Endianism_Default      ;
        chnlAttrInput.size      = ChannelDataSize_16bits ;

        status = CHNL_Create (ID_PROCESSOR, CHNL_ID_INPUT, &chnlAttrInput) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("CHNL_Create failed (input). Status = [0x%x]\n",
                         status) ;
        }
    }

    /*
     *  Allocate buffer(s) for data transfer to DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (ID_PROCESSOR,
                                      CHNL_ID_OUTPUT,
                                      LOOP_OutBufs,
                                      LOOP_BufferSize ,
                                      1) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("CHNL_AllocateBuffer failed (output)."
                         " Status = [0x%x]\n",
                         status) ;
        }
    }

    /*
     *  Allocate buffer(s) for data transfer from DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (ID_PROCESSOR,
                                      CHNL_ID_INPUT,
                                      LOOP_InpBufs,
                                      LOOP_BufferSize ,
                                      1) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("CHNL_AllocateBuffer failed (input)."
                         " Status = [0x%x]\n",
                         status) ;
        }
    }

    /*
     *  Initialize the buffer with valid data.
     */
    if (DSP_SUCCEEDED (status)) {
        temp = LOOP_OutBufs [0] ;

        for (i = 0 ; i < LOOP_BufferSize ; i++) {
            *temp++ = XFER_CHAR ;
        }
    }

    LOOP_0Print ("Leaving LOOP_Create ()\n") ;

    return status ;
}


/** ============================================================================
 *  @func   LOOP_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LOOP_Execute (IN Uint32 numIterations)
{
    DSP_STATUS status = DSP_SOK ;
    Uint32     i                ;

    LOOP_0Print ("Entered LOOP_Execute ()\n") ;

    /*
     *  Start execution on DSP.
     */
    status = PROC_Start (ID_PROCESSOR) ;

    /*
     *  Fill the IO Request structure
     *  It gives Information for adding or reclaiming an input request.
     */
    if (DSP_SUCCEEDED (status)) {
        LOOP_OutIOReq.buffer = LOOP_OutBufs [0] ;
        LOOP_OutIOReq.size   = LOOP_BufferSize   ;

        LOOP_InpIOReq.buffer  = LOOP_InpBufs [0] ;
        LOOP_InpIOReq.size    = LOOP_BufferSize   ;
    }
    else {
        LOOP_1Print ("PROC_Start failed. Status = [0x%x]\n", status) ;
    }

    for (i = 1 ; (i <= LOOP_NumIterations) && (DSP_SUCCEEDED (status)) ; i++) {
        /*
         *  Send data to DSP.
         *  Issue 'filled' buffer to the channel.
         */
        status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_OUTPUT, &LOOP_OutIOReq) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("CHNL_Issue failed (output). Status = [0x%x]\n",
                          status) ;
        }

        /*
         *  Reclaim 'empty' buffer from the channel
         */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (ID_PROCESSOR,
                                   CHNL_ID_OUTPUT,
                                   WAIT_FOREVER,
                                   &LOOP_OutIOReq) ;
            if (DSP_FAILED (status)) {
                LOOP_1Print ("CHNL_Reclaim failed (output). Status = [0x%x]\n",
                             status) ;
            }
        }

        /*
         *  Receive data from DSP
         *  Issue 'empty' buffer to the channel.
         */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_INPUT, &LOOP_InpIOReq) ;
            if (DSP_FAILED (status)) {
                LOOP_1Print ("CHNL_Issue failed (input). Status = [0x%x]\n",
                             status) ;
            }
        }

        /*
         *  Reclaim 'filled' buffer from the channel
         */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (ID_PROCESSOR,
                                   CHNL_ID_INPUT,
                                   WAIT_FOREVER,
                                   &LOOP_InpIOReq) ;
            if (DSP_FAILED (status)) {
                LOOP_1Print ("CHNL_Reclaim failed (input). Status = [0x%x]\n",
                             status) ;
            }
        }

#if defined (VERIFY_DATA)
        /*
         *  Verify correctness of data received.
         */
        if (DSP_SUCCEEDED (status)) {
            status = LOOP_VerifyData (LOOP_InpBufs [0]) ;
            if (DSP_FAILED (status)) {
                LOOP_0Print ("Data integrity failed\n") ;
            }
        }
#endif
        if (DSP_SUCCEEDED (status) && (i % 1000) == 0) {
            LOOP_1Print ("Transferred %d buffers\n", i) ;
        }
    }

    /*
     *  Stop execution on DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Stop (ID_PROCESSOR) ;
    }

    LOOP_0Print ("Leaving LOOP_Execute ()\n") ;

    return status ;
}


/** ============================================================================
 *  @func   LOOP_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          LOOP_Create ().
 *          During cleanup, the allocated resources are being freed
 *          unconditionally. Actual applications may require stricter check
 *          against return values for robustness.
 *
 *  @modif  LOOP_InpBufs , LOOP_OutBufs
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_Delete ()
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;

    LOOP_0Print ("Entered LOOP_Delete ()\n") ;

    /*
     *  Free the buffer(s) allocated for channel from DSP
     */
    status = CHNL_FreeBuffer (ID_PROCESSOR, CHNL_ID_INPUT, LOOP_InpBufs, 1) ;
    if (DSP_FAILED (status)) {
        LOOP_1Print ("CHNL_FreeBuffer () failed  (input). Status = [0x%x]\n",
                      status) ;
    }

    /*
     *  Free the buffer(s) allocated for channel to DSP
     */
    tmpStatus = CHNL_FreeBuffer (ID_PROCESSOR, CHNL_ID_OUTPUT, LOOP_OutBufs, 1) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        LOOP_1Print ("CHNL_FreeBuffer () failed (output). Status = [0x%x]\n",
                     tmpStatus) ;
    }

    /*
     *  Delete both input and output channels
     */
    tmpStatus = CHNL_Delete  (ID_PROCESSOR, CHNL_ID_INPUT) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        LOOP_1Print ("CHNL_Delete () failed (input). Status = [0x%x]\n",
                     tmpStatus) ;
    }
    tmpStatus = CHNL_Delete  (ID_PROCESSOR, CHNL_ID_OUTPUT) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        LOOP_1Print ("CHNL_Delete () failed (output). Status = [0x%x]\n",
                     tmpStatus) ;
    }

    /*
     *  Detach from the processor
     */
    tmpStatus = PROC_Detach  (ID_PROCESSOR) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        LOOP_1Print ("PROC_Detach () failed. Status = [0x%x]\n", tmpStatus) ;
    }

    /*
     *  Destroy the PROC object.
     */
    tmpStatus = PROC_Destroy () ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        LOOP_1Print ("PROC_Destroy () failed. Status = [0x%x]\n", tmpStatus) ;
    }

    LOOP_0Print ("Leaving LOOP_Delete ()\n") ;
}


/** ============================================================================
 *  @func   LOOP_Main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_Main (IN Char8 * dspExecutable,
           IN Char8 * strBufferSize,
           IN Char8 * strNumIterations)
{
    DSP_STATUS status = DSP_SOK ;

    LOOP_0Print ("=============== Sample Application : LOOP ==========\n") ;

    if (   (dspExecutable != NULL)
        && (strBufferSize != NULL)
        && (strNumIterations != NULL)) {

        /*
         *  Validate the buffer size and number of iterations specified.
         */
        LOOP_BufferSize = atoi (strBufferSize) ;
        if (LOOP_BufferSize == 0) {
            status = DSP_ESIZE ;
        }

        LOOP_NumIterations = atoi (strNumIterations) ;
        if (LOOP_NumIterations == 0) {
            status = DSP_ERANGE ;
        }

        /*
         *  Specify the dsp executable file name and the buffer size for
         *  loop creation phase.
         */
        if (DSP_SUCCEEDED (status)) {
            status = LOOP_Create (dspExecutable,
                                  strBufferSize,
                                  strNumIterations) ;
        }

        /*
         *  Execute the data transfer loop.
         */
        if (DSP_SUCCEEDED (status)) {
            status = LOOP_Execute (LOOP_NumIterations) ;
        }

        /*
         *  Perform cleanup operation.
         */
        LOOP_Delete () ;
    }
    else {
        status = DSP_EINVALIDARG ;
        LOOP_0Print ("ERROR! Invalid arguments specified for while executing "
                     "loop application\n") ;
    }

    LOOP_0Print ("====================================================\n") ;
}


#if defined (VERIFY_DATA)
/** ----------------------------------------------------------------------------
 *  @func   LOOP_VerifyData
 *
 *  @desc   This function verifies the data-integrity of given buffer.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
LOOP_VerifyData (IN Char8 * buf)
{
    DSP_STATUS status = DSP_SOK ;
    Int16      i                ;

    /*
     *  Verify the data.
     */
    for (i = 0 ; (i < LOOP_BufferSize) && (DSP_SUCCEEDED (status)) ; i++) {
        if (*buf++ != XFER_CHAR) {
            status = DSP_EFAIL ;
        }
    }

    return status ;
}
#endif


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
