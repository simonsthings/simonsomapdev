#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/ioctl.h>

/*--------------------------------------------- Linux specific include */
//#include <linux/soundcard.h> blaaaaaaa

/*  ----------------------------------- DSP/BIOS Link                   */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <platform.h>
#include <msgqdefs.h>
/*----------------------------------- DSP/BIOS Link API                 */
#include <proc.h>
#include <chnl.h>
#include <msgq.h>
#include <pool.h>

/*  ----------------------------------- Profiling                       */
#include <profile.h>

#define DDSP_DEBUG 


/* ----------------------------- DSPLINK 1.30 Related PreProcessor Definitions             */

/** ============================================================================
 *  @const  ID_PROCESSOR
 *
 *  @desc   The processor id of the processor being used.
 *  ============================================================================
 */
#define ID_PROCESSOR       0


/** ============================================================================
 *  @const  CHNL_ID_OUTPUT
 *
 *  @desc   ID of channel used to send data to DSP.
 *  ============================================================================
 */
#define CHNL_ID_OUTPUT     0


/** ============================================================================
 *  @const  CHNL_ID_INPUT
 *
 *  @desc   ID of channel used to receive data from DSP.
 *  ============================================================================
 */
#define CHNL_ID_INPUT      1
/*  ============================================================================
 *  @const  NUM_ARGS
 *
 *  @desc   Number of arguments specified to the DSP application.
 *  ============================================================================
 */
#define NUM_ARGS             2

/** ============================================================================
 *  @name   SAMPLE_POOL_ID
 *
 *  @desc   ID of the POOL used for the application.
 *  ============================================================================
 */
#define POOL_ID       0

/** ============================================================================
 *  @name   NUM_BUF_SIZES
 *
 *  @desc   Number of buffer pools to be configured for the applicatio.
 *  ============================================================================
 */
#define NUMBUFFERPOOLS        1

/** ============================================================================
 *  @const  NUM_BUF_POOL0
 *
 *  @desc   Number of buffers in the buffer pool.
 *  ============================================================================
 */
#define NUMBUFS        4

/** ============================================================================
 *  @const  NUM_BUF_POOL1
 *
 *  @desc   Number of buffers in second buffer pool.
 *  ============================================================================
 */
//#define NUM_BUF_POOL1        2

/** ============================================================================
 *  @const  NUM_BUF_POOL2
 *
 *  @desc   Number of buffers in third buffer pool.
 *  ============================================================================
 */
//#define NUM_BUF_POOL2        1

/** ============================================================================
 *  @name   NUM_BUF_POOL3
 *
 *  @desc   Number of buffers in fourth buffer pool.
 *  ============================================================================
 */
//#define NUM_BUF_POOL3        2

/** ============================================================================
 *  @name   SAMPLE_CTRLMSG_SIZE
 *
 *  @desc   Control message size.
 *  ============================================================================
 */
//#if defined (ZCPY_LINK)
//#define SAMPLE_CTRLMSG_SIZE ZCPYMQT_CTRLMSG_SIZE
//#endif /* if defined (ZCPY_LINK) */

/** ============================================================================
 *  @const  MsgqDsp1
 *
 *  @desc   Name of the first MSGQ on the DSP.
 *  ============================================================================
 */
//STATIC Char8 SampleDspMsgqName [DSP_MAX_STRLEN] = "DSPMSGQ1" ;

/** ============================================================================
 *  @name   SampleDspMsgq
 *
 *  @desc   DSP's MSGQ Object.
 *  ============================================================================
 */
//STATIC MsgqQueue SampleDspMsgq = (Uint32) MSGQ_INVALIDMSGQ ;


/* ------------------------------------------------- Wavplayer Application PreProcessor Definitions           */

/** ============================================================================
 *  @const  CTRL_MSGID
 *
 *  @desc   Message id to denote a message transfer.
 *  ============================================================================
 */
//#define CTRL_MSGID            1

/** ============================================================================
 *  @const  APP_MSG_SIZE
 *
 *  @desc   Messaging buffer used by the application.
 *          Note: This buffer is aligned according to the alignment expected
 *          by the platform.
 *  ============================================================================
 */
//#define APP_MSG_SIZE  ALIGN (sizeof (CtrlMsg), DSPLINK_BUF_ALIGN)
//#define APP_MSG_SIZE  sizeof (CtrlMsg)

/* 1024 * 7 is the size used due to accomodate a sampling rate of upto 88KHz   */ 
/* Minimum possible multiple is 1024*1                                         */
//#define BUFFERSIZE             7168 (1024*7)
//#define NUMOUTBUFS             1
/*------------------------------------- NOT NEEDED-----------------------------------------------------*/

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
 *  @desc   Number of iterations of data transfer.
 *          A value of 0 in LOOP_NumIterations implies infinite iterations.
 *  ============================================================================
 */
STATIC Uint32  LOOP_NumIterations ;

/** ============================================================================
 *  @name   LOOP_Buffers
 *
 *  @desc   Array of buffers used by the loop application.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
STATIC Char8 * LOOP_Buffers [1] ;

/** ============================================================================
 *  @name   LOOP_IOReq
 *
 *  @desc   It gives information for adding or reclaiming a request.
 *  ============================================================================
 */
STATIC ChannelIOInfo LOOP_IOReq  ;

/*==============================================================================
*/
/*  ============================================================================
 *  @name   XFER_CHAR
 *
 *  @desc   The value used to initialize the output buffer and used for
 *          validation against the input buffer recieved.
 *  ============================================================================
 */
#define XFER_CHAR   (Char8) 0xE7

/** ============================================================================
 *  @func   LOOP_Atoi
 *
 *  @desc   This function converts a string into an integer value.
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
 *  @see    None
 *  ============================================================================
 */
#define LOOP_Atoi atoi


/** ============================================================================
 *  @func   LOOP_0Print
 *
 *  @desc   Print a message without any arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\loop_os.c
 *
 *  @arg    str
 *              String message to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_0Print (Char8 * str) ;


/** ============================================================================
 *  @func   LOOP_1Print
 *
 *  @desc   Print a message with one arguments.
 *          This is a OS specific function and is implemented in file:
 *              <GPPOS>\loop_os.c
 *
 *  @arg    str
 *              String message to be printed.
 *  @arg    arg
 *              Argument to be printed.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_1Print (Char8 * str, Uint32 arg) ;

/*==============================================the functions start==================================================*/



/*int prog_main()
{
    LOOP_Main ("mainprog", 7168, 1) ;

}*/


int main(int argc, char argv[])
{
    int argc;
    char *argv[4];
    Char8 * dspExecutable    = NULL ;
    Char8 * strBufferSize    = NULL ;
    Char8 * strNumIterations = NULL ;


    
    argc = 4;
    argv[0] = "mainprog";
    //argv[1] = "wav_rec_rf6_dsp.out";
    //argv[2] = "ISIP.wav";
    //argv[3] = "1"; 

    printf("\n Hello waverec World\n");
    
    if ( argc < 2 ) {
       printf ("Usage : %s <absolute path of DSP executable> "
           "<Buffer Size> \n"
           "For infinite transfers, use value of 0 for <number of transfers>\n",
           argv [0]) ;
    }
   
   else {
        dspExecutable    = argv [1] ;
        strBufferSize    = argv [2] ;
        //strNumIterations = argv [3] ;
       // LOOP_Main (dspExecutable, strBufferSize, strNumIterations) ;

	//Correct:
        LOOP_Main ("mainprog", (7168)) ;

	//Wrong:
       // LOOP_Main (dspExecutable,strNumIterations, strBufferSize ) ;
    }

    return 0 ;
}
    
/** ============================================================================
 *  @func   LOOP_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @arg    dspExecutable
 *              DSP executable name.
 *  @arg    bufferSize
 *              String representation of buffer size to be used
 *              for data transfer.
 *  @arg    strNumIterations
 *              Number of iterations a data buffer is transferred between
 *              GPP and DSP in string format.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Resource allocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LOOP_Delete
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LOOP_Create("mainprog",(7168));
/*LOOP_Create (IN Char8 * dspExecutable,
             IN Char8 * strBufferSize,
             IN Char8 * strNumIterations) ;*/



/** ============================================================================
 *  @func   LOOP_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @arg    numIterations
 *              Number of iterations a data buffer is transferred between
 *              GPP and DSP in string format.
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Loop execution failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LOOP_Delete , LOOP_Create
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LOOP_Execute (Void) ;
//LOOP_Execute (IN Uint32 numIterations) ;


/** ============================================================================
 *  @func   LOOP_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          LOOP_Create ().
 *          During cleanup, the allocated resources are being freed
 *          unconditionally. Actual applications may require stricter check
 *          against return values for robustness.
 *
 *  @arg    None
 *
 *  @ret    DSP_SOK
 *              Operation Successfully completed.
 *          DSP_EFAIL
 *              Resource deallocation failed.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LOOP_Create
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_Delete (Void) ;


/** ============================================================================
 *  @func   LOOP_Main
 *
 *  @desc   The OS independent driver function for the loop sample application.
 *
 *  @arg    dspExecutable
 *              Name of the DSP executable file.
 *  @arg    strBufferSize
 *              Buffer size to be used for data-transfer in string format.
 *  @arg    strNumIterations
 *              Number of iterations a data buffer is transferred between
 *              GPP and DSP in string format.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    LOOP_Create, LOOP_Execute, LOOP_Delete
 *  ============================================================================
 */

 /************************ Do we require this LOOP_Main again ???************************************/

NORMAL_API
Void
LOOP_Main ("mainprog", (7168)) ;
/*LOOP_Main (IN Char8 * dspExecutable,
           IN Char8 * strBuffersize,
           IN Char8 * strNumIterations) ; */




/*===============================================================================
*/
/** ============================================================================
 *  @func   LOOP_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @modif  LOOP_Buffers
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
LOOP_Create (IN Char8 * dspExecutable,
             IN Char8 * strBufferSize)
            // IN Char8 * strNumIterations)
{
    DSP_STATUS    status                    = DSP_SOK   ;
    Char8 *       temp                      = NULL      ;
    Uint32        numArgs                   = 0         ;
    Uint32        numBufs [NUMBUFFERPOOLS]  = {NUMBUFS} ;

    ChannelAttrs  chnlAttrInput            ;
    ChannelAttrs  chnlAttrOutput           ;
    Uint16        i                        ;
    Char8 *       args [NUM_ARGS]          ;
    Uint32        size [NUMBUFFERPOOLS]    ;
#if defined (ZCPY_LINK)
    SmaPoolAttrs  poolAttrs                ;
#endif /* if defined (ZCPY_LINK) */
#if defined (PCPY_LINK)
    BufPoolAttrs  poolAttrs                ;
#endif /* if defined (PCPY_LINK) */

    LOOP_0Print ("Entered LOOP_Create ()\n") ;

    /*
     *  Create and initialize the proc object.
     */

    LOOP_BufferSize = (7168);
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
     *  Open the pool.
     */
    if (DSP_SUCCEEDED (status)) {
        size [0] = LOOP_BufferSize ;
        poolAttrs.bufSizes      = (Uint32 *) &size ;
        poolAttrs.numBuffers    = (Uint32 *) &numBufs ;
        poolAttrs.numBufPools   = NUMBUFFERPOOLS ;
#if defined (ZCPY_LINK)
        poolAttrs.exactMatchReq = TRUE ;
#endif /* if defined (ZCPY_LINK) */
        status = POOL_Open (POOL_ID, &poolAttrs) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("POOL_Open () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Load the executable on the DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        numArgs  = NUM_ARGS         ;
        args [0] = strBufferSize    ;
        //args [1] = strNumIterations ;

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
        printf("\n\nPROC_Start() success!!!!!!!!!!!!\n\n");

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
        printf("\n\nCHNL_Create() success!!!!!!!!!!!!\n\n");
        status = CHNL_AllocateBuffer (ID_PROCESSOR,
                                      CHNL_ID_OUTPUT,
                                      LOOP_Buffers,
                                      LOOP_BufferSize ,
                                      1) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("CHNL_AllocateBuffer failed (output)."
                         " Status = [0x%x]\n",
                         status) ;
        }
    }

    /*
     *  Initialize the buffer with valid data.
     */
    if (DSP_SUCCEEDED (status)) {
        temp = LOOP_Buffers [0] ;

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
LOOP_Execute (Void)
{
    DSP_STATUS status = DSP_SOK ;
    //Uint32     i      ;

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
        LOOP_IOReq.buffer = LOOP_Buffers [0] ;
        LOOP_IOReq.size   = LOOP_BufferSize   ;
    }
    else {
        LOOP_1Print ("PROC_Start failed. Status = [0x%x]\n", status) ;
    }

   /* for (i = 1 ;
         (   (LOOP_NumIterations == 0) || (i <= LOOP_NumIterations))
          && (DSP_SUCCEEDED (status)) ;
         i++) { */
        /*
         *  Send data to DSP.
         *  Issue 'filled' buffer to the channel.
         */
      /*  status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_OUTPUT, &LOOP_IOReq) ;
        if (DSP_FAILED (status)) {
            LOOP_1Print ("CHNL_Issue failed (output). Status = [0x%x]\n",
                          status) ;
        } */

        /*
         *  Reclaim 'empty' buffer from the channel
         */
        /*if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (ID_PROCESSOR,
                                   CHNL_ID_OUTPUT,
                                   WAIT_FOREVER,
                                   &LOOP_IOReq) ;
            if (DSP_FAILED (status)) {
                LOOP_1Print ("CHNL_Reclaim failed (output). Status = [0x%x]\n",
                             status) ;
            }
        } */

        /*
         *  Receive data from DSP
         *  Issue 'empty' buffer to the channel.
         */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_INPUT, &LOOP_IOReq) ;
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
                                   &LOOP_IOReq) ;
            if (DSP_FAILED (status)) {
                LOOP_1Print ("CHNL_Reclaim failed (input). Status = [0x%x]\n",
                             status) ;
            }
        }

    // }  /* for loop ends */

    /*
     *  Stop execution on DSP.
     */
    status = PROC_Stop (ID_PROCESSOR) ;

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
 *  @modif  LOOP_Buffers
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_Delete (Void)
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;

    LOOP_0Print ("Entered LOOP_Delete ()\n") ;

    /*
     *  Free the buffer(s) allocated for channel to DSP
     */
    tmpStatus = CHNL_FreeBuffer (ID_PROCESSOR, CHNL_ID_OUTPUT, LOOP_Buffers, 1) ;
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
     *  Close the pool
     */
    tmpStatus = POOL_Close (POOL_ID) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        LOOP_1Print ("POOL_Close () failed. Status = [0x%x]\n",
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
        LOOP_BufferSize = ALIGN (LOOP_Atoi (strBufferSize), DSPLINK_BUF_ALIGN) ;
        if (LOOP_BufferSize == 0) {
            status = DSP_ESIZE ;
        }

        LOOP_NumIterations = LOOP_Atoi (strNumIterations) ;

        /*
         *  Specify the dsp executable file name and the buffer size for
         *  loop creation phase.
         */
        status = LOOP_Create (dspExecutable, strBufferSize, strNumIterations) ;

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
    
/*===========================================================================================================
*/


/** ============================================================================
 *  @func   atoi
 *
 *  @desc   Extern declaration for function that converts a string into an
 *          integer value.
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
 *  @see    None
 *  ============================================================================
 */
extern int atoi (const char * str) ;




/** ============================================================================
 *  @func   LOOP_0Print
 *
 *  @desc   Print a message without any arguments.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_0Print (Char8 * str)
{
    printf (str) ;
    fflush (stdout) ;
}


/** ============================================================================
 *  @func   LOOP_1Print
 *
 *  @desc   Print a message with one arguments.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
LOOP_1Print (Char8 * str, Uint32 arg)
{
    printf (str, arg) ;
    fflush (stdout) ;
}

