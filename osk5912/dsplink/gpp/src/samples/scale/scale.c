/** ============================================================================
 *  @file   scale.c
 *
 *  @path   $(DSPLINK)\gpp\src\samples\scale
 *
 *  @desc   This is a scale application which sends a buffer across the DSP
 *          processor and recieves a buffer back using DSP/BIOS LINK. The DSP
 *          scales the returned buffer based on the control message sent by the
 *          GPP. The data buffer received is verified aginst the data sent to
 *          DSP. This application transfers only one buffer and no buffer(s) are
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
#include <platform.h>
#include <msgqdefs.h>

/*  ----------------------------------- Profiling                       */
#include <profile.h>

/*  ----------------------------------- DSP/BIOS LINK API               */
#include <proc.h>
#include <chnl.h>
#include <msgq.h>

/*  ----------------------------------- Application Header              */
#include <scale.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
 *  @const  NUM_ARGS
 *
 *  @desc   Number of arguments specified to the DSP application.
 *  ============================================================================
 */
#define NUM_ARGS   2

/** ============================================================================
 *  @const  MQABUFID
 *
 *  @desc   Id of the allocator
 *  ============================================================================
 */
#define MQABUFID   0

/** ============================================================================
 *  @const  LOCALMQT
 *
 *  @desc   Id of the local transport
 *  ============================================================================
 */
#define LOCALMQT   0

/** ============================================================================
 *  @const  REMOTEMQT
 *
 *  @desc   Id of the remote transport
 *  ============================================================================
 */
#define REMOTEMQT   1

/** ============================================================================
 *  @const  DSPMSGQID
 *
 *  @desc   Id of the message queue on the DSP
 *  ============================================================================
 */
#define DSPMSGQID   0

/*  ============================================================================
 *  @name   XFER_CHAR
 *
 *  @desc   The value used to initialize the output buffer and used for
 *          validation against the input buffer received.
 *  ============================================================================
 */
#define XFER_CHAR   0x1

/*  ============================================================================
 *  @name   SCALE_XferValue
 *
 *  @desc   The scaling factor for the returned buffer values.
 *  ============================================================================
 */
STATIC Uint8  SCALE_XferValue = 1 ;

/*  ============================================================================
 *  @name   SCALE_BufferSize
 *
 *  @desc   Size of buffer to be used for data transfer.
 *  ============================================================================
 */
STATIC Uint32  SCALE_BufferSize ;

/*  ============================================================================
 *  @name   SCALE_NumIterations
 *
 *  @desc   Size of buffer to be used for data transfer.
 *  ============================================================================
 */
STATIC Uint32  SCALE_NumIterations ;

/** ============================================================================
 *  @name   SCALE_InpBufs
 *
 *  @desc   Array of buffers used by input channel.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
STATIC Char8 * SCALE_InpBufs [1] ;

/** ============================================================================
 *  @name   SCALE_OutBufs
 *
 *  @desc   Array of buffers used by output channel.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
STATIC Char8 * SCALE_OutBufs [1] ;

/** ============================================================================
 *  @name   SCALE_InpIOReq
 *
 *  @desc   It gives information for adding or reclaiming an input request.
 *  ============================================================================
 */
STATIC ChannelIOInfo SCALE_InpIOReq  ;

/** ============================================================================
 *  @name   SCALE_OutIOReq
 *
 *  @desc   It gives information for adding or reclaiming an output request.
 *  ============================================================================
 */
STATIC ChannelIOInfo SCALE_OutIOReq ;

/** ============================================================================
 *  @const  SCALE_REQUEST_MSGID
 *
 *  @desc   Message id to denote a scaling change.
 *  ============================================================================
 */
#define SCALE_REQUEST_MSGID   1

/** ============================================================================
 *  @name   SCALE_ScaleMsg
 *
 *  @desc   Structure used to pass the scaling factor from the GPP to the DSP.
 *
 *  @field  msgHeader
 *              Required first field of a message.
 *  @field  scalingFactor
 *              Used to scale the output buffer values.
 *  ============================================================================
 */
typedef struct SCALE_ScaleMsg_tag {
    MsgqMsgHeader  msgHeader     ;
    Uint16         scalingFactor ;
} SCALE_ScaleMsg ;

/** ============================================================================
 *  @const  NUMMSGPOOLS
 *
 *  @desc   Number of BUF pools in the allocator
 *  ============================================================================
 */
#define NUMMSGPOOLS   2

/** ============================================================================
 *  @name   msgSizes
 *
 *  @desc   Message sizes managed by the allocator.
 *  ============================================================================
 */
STATIC Uint16 msgSizes[NUMMSGPOOLS] = {ALIGN (sizeof (SCALE_ScaleMsg)),
                                       RMQT_CTRLMSG_SIZE} ;

/** ============================================================================
 *  @const  NUMMSGINPOOL0
 *
 *  @desc   Number in first message pool.
 *  ============================================================================
 */
#define NUMMSGINPOOL0   1

/** ============================================================================
 *  @const  NUMMSGINPOOL1
 *
 *  @desc   Number in second message pool.
 *  ============================================================================
 */
#define NUMMSGINPOOL1   1

/** ============================================================================
 *  @name   msgCount
 *
 *  @desc   Number of messages in each pool managed by the allocator.
 *  ============================================================================
 */
STATIC Uint16 msgCount[NUMMSGPOOLS] = {NUMMSGINPOOL0, NUMMSGINPOOL1} ;

/** ============================================================================
 *  @name   mqaBufParams
 *
 *  @desc   Configuration parameters for the BUF-based allocator instance.
 *  ============================================================================
 */
STATIC MqaBufAttrs mqaBufAttrs = {NUMMSGPOOLS,    /* Number of pools     */
                                  msgSizes,       /* Array of sizes      */
                                  msgCount} ;     /* Array of msg counts */

/** ============================================================================
 *  @const  NUMLOCALMSGQUEUES
 *
 *  @desc   Number of local message queues
 *  ============================================================================
 */
#define NUMLOCALMSGQUEUES   0

/** ============================================================================
 *  @name   mqtLocalAttrs
 *
 *  @desc   Local transport attributes.
 *  ============================================================================
 */
STATIC LmqtAttrs mqtLocalAttrs = {NUMLOCALMSGQUEUES} ;

/** ============================================================================
 *  @const  NUMREMOTEMSGQUEUES
 *
 *  @desc   Number of remote message queues
 *  ============================================================================
 */
#define NUMREMOTEMSGQUEUES   1

/** ============================================================================
 *  @name   mqtRemoteAttrs
 *
 *  @desc   Local transport attributes.
 *  ============================================================================
 */
STATIC RmqtAttrs mqtRemoteAttrs = {NUMREMOTEMSGQUEUES,       /* maxNumMsgq    */
                                   RMQT_CTRLMSG_SIZE,        /* maxMsgSize    */
                                   MQABUFID} ;               /* defaultMqaId  */


#if defined (VERIFY_DATA)
/** ----------------------------------------------------------------------------
 *  @func   SCALE_VerifyData
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
 *  @see    SCALE_XferValue
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SCALE_VerifyData (IN Char8 * buf) ;
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
 *  ----------------------------------------------------------------------------
 */
EXTERN
int
atoi (char * str) ;


/** ============================================================================
 *  @func   SCALE_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @modif  SCALE_InpBufs , SCALE_OutBufs
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SCALE_Create (IN Char8 * dspExecutable,
              IN Char8 * strBufferSize,
              IN Char8 * strNumIterations)
{
    DSP_STATUS    status         = DSP_SOK ;
    Uint32        numArgs        = 0       ;
    Uint8 *       ptr8           = NULL    ;
    Uint16 *      ptr16          = NULL    ;
    ChannelAttrs  chnlAttrInput            ;
    ChannelAttrs  chnlAttrOutput           ;
    Uint16        i                        ;
    Char8       * args [NUM_ARGS]          ;

    SCALE_0Print ("Entered SCALE_Create ()\n") ;

    /*
     *  Create and initialize the proc object.
     */
    status = PROC_Setup () ;

    /*
     *  Open the allocator.
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_AllocatorOpen (MQABUFID, &mqaBufAttrs) ;
        if (DSP_FAILED (status)) {
            SCALE_1Print ("MSGQ_AllocatorOpen () failed. Status = [0x%x]\n",
                            status) ;
        }
    }
    else {
        SCALE_1Print ("PROC_Setup () failed. Status = [0x%x]\n", status) ;
    }

    /*
     *  Open the local transport.
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_TransportOpen (LOCALMQT, &mqtLocalAttrs) ;
        if (DSP_FAILED (status)) {
            SCALE_1Print ("MSGQ_TransportOpen () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Attach the Dsp with which the transfers have to be done.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (ID_PROCESSOR, NULL) ;
        if (DSP_FAILED (status)) {
            SCALE_1Print ("PROC_Attach () failed. Status = [0x%x]\n",
                            status) ;
        }
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
            SCALE_1Print ("PROC_Load () failed. Status = [0x%x]\n", status) ;
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
            SCALE_1Print ("CHNL_Create () failed. Status = [0x%x]\n",
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
            SCALE_1Print ("CHNL_Create () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Allocate buffer(s) for data transfer to DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (ID_PROCESSOR,
                                      CHNL_ID_OUTPUT,
                                      SCALE_OutBufs,
                                      SCALE_BufferSize ,
                                      1) ;
        if (DSP_FAILED (status)) {
            SCALE_1Print ("CHNL_AllocateBuffer () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Allocate buffer(s) for data transfer from DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = CHNL_AllocateBuffer (ID_PROCESSOR,
                                      CHNL_ID_INPUT,
                                      SCALE_InpBufs,
                                      SCALE_BufferSize ,
                                      1) ;
        if (DSP_FAILED (status)) {
            SCALE_1Print ("CHNL_AllocateBuffer () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Initialize the buffer with valid data.
     */
    if (DSP_SUCCEEDED (status)) {
        ptr8  = (Uint8 *) (SCALE_OutBufs [0]) ;
        ptr16 = (Uint16 *) (SCALE_OutBufs [0]) ;
        for (i = 0 ;
             DSP_SUCCEEDED (status) && (i < SCALE_BufferSize / DSP_MAUSIZE) ;
             i++) {
            if (DSP_MAUSIZE == 1) {
                *ptr8 = XFER_CHAR ;
                ptr8++ ;
            }
            else if (DSP_MAUSIZE == 2) {
                *ptr16 = XFER_CHAR ;
                ptr16++ ;
            }
        }
    }

    /*
     *  Start execution on DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (ID_PROCESSOR) ;
        if (DSP_FAILED (status)) {
            SCALE_1Print ("PROC_Start () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Open the remote transport.
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_TransportOpen (REMOTEMQT, &mqtRemoteAttrs) ;
        if (DSP_FAILED (status)) {
            SCALE_1Print ("MSGQ_TransportOpen () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    SCALE_0Print ("Leaving SCALE_Create ()\n") ;

    return status ;
}


/** ============================================================================
 *  @func   SCALE_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
SCALE_Execute (IN Uint32 numIterations)
{
    DSP_STATUS       status = DSP_ENOTFOUND ;
    MsgqLocateAttrs  locateAttrs            ;
    Uint32           i                      ;
    SCALE_ScaleMsg * msg                    ;


    SCALE_0Print ("Entered SCALE_Execute ()\n") ;

    /*
     *  Locate the DSP's message queue
     */
    locateAttrs.timeout = WAIT_FOREVER ;
    while (status == DSP_ENOTFOUND) {
        status = MSGQ_Locate (ID_PROCESSOR, DSPMSGQID, &locateAttrs) ;
        if (status == DSP_ENOTFOUND) {
            SCALE_Sleep (1000) ;
        }
        else if (DSP_FAILED (status)) {
            SCALE_1Print ("MSGQ_Locate () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Fill the IO Request structure
     *  It gives Information for adding or reclaiming an input request.
     */
    if (DSP_SUCCEEDED (status)) {
        SCALE_OutIOReq.buffer = SCALE_OutBufs [0] ;
        SCALE_OutIOReq.size   = SCALE_BufferSize   ;

        SCALE_InpIOReq.buffer  = SCALE_InpBufs [0] ;
        SCALE_InpIOReq.size    = SCALE_BufferSize   ;
    }

    for (i = 1 ; (i <= SCALE_NumIterations) && (DSP_SUCCEEDED (status)) ; i++) {
        /*
         *  Send data to DSP.
         *  Issue 'filled' buffer to the channel.
         */
        status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_OUTPUT, &SCALE_OutIOReq) ;

        /*
         *  Reclaim 'empty' buffer from the channel
         */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Reclaim (ID_PROCESSOR,
                                   CHNL_ID_OUTPUT,
                                   WAIT_FOREVER,
                                   &SCALE_OutIOReq) ;
            if (DSP_FAILED (status)) {
                SCALE_1Print ("CHNL_Reclaim () failed. Status = [0x%x]\n",
                                status) ;
            }
        }
        else {
            SCALE_1Print ("CHNL_Issue () failed. Status = [0x%x]\n", status) ;
        }

        /*
         *  Receive data from DSP
         *  Issue 'empty' buffer to the channel.
         */
        if (DSP_SUCCEEDED (status)) {
            status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_INPUT, &SCALE_InpIOReq) ;
            if (DSP_FAILED (status)) {
                SCALE_1Print ("CHNL_Issue () failed. Status = [0x%x]\n",
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
                                   &SCALE_InpIOReq) ;
            if (DSP_FAILED (status)) {
                SCALE_1Print ("CHNL_Reclaim () failed. Status = [0x%x]\n",
                                status) ;
            }
        }

#if defined (VERIFY_DATA)
        /*
         *  Verify correctness of data received.
         */
        if (DSP_SUCCEEDED (status)) {
            status = SCALE_VerifyData (SCALE_InpBufs [0]) ;
        }
#endif
        if (DSP_SUCCEEDED (status) && (i % 100) == 0) {
            SCALE_1Print ("Transferred %d buffers\n", i) ;
        }

        if ((i % 100) == 0) {
            /* Change the scaling factor */
            SCALE_XferValue++ ;
            status = MSGQ_Alloc (MQABUFID,
                                 ALIGN (sizeof (SCALE_ScaleMsg)),
                                 (MsgqMsg *) &msg) ;
            if (DSP_SUCCEEDED (status)) {
                msg->scalingFactor = SCALE_XferValue ;
                /* Send the allocated message */
                status = MSGQ_Put (ID_PROCESSOR,
                                   DSPMSGQID,
                                   (MsgqMsg) msg,
                                   SCALE_REQUEST_MSGID,
                                   MSGQ_INVALID_ID) ;
                if (DSP_SUCCEEDED (status)) {
                    SCALE_1Print ("Changed the scale factor to: %d\n",
                                  SCALE_XferValue) ;
                }
                else {
                    SCALE_1Print ("MSGQ_Put () failed. Status = [0x%x]\n",
                                    status) ;
                }
            }
            else {
                SCALE_1Print ("MSGQ_Alloc () failed. Status = [0x%x]\n",
                                status) ;
            }
        }
    }

    /*
     *  Release the remote message queue
     */
    status = MSGQ_Release (ID_PROCESSOR, DSPMSGQID) ;
    if (DSP_FAILED (status)) {
        SCALE_1Print ("MSGQ_Release () failed. Status = [0x%x]\n", status) ;
    }

    SCALE_0Print ("Leaving SCALE_Execute ()\n") ;

    return status ;
}


/** ============================================================================
 *  @func   SCALE_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          SCALE_Create ().
 *          During cleanup, the allocated resources are being freed
 *          unconditionally. Actual applications may require stricter check
 *          against return values for robustness.
 *
 *  @modif  SCALE_InpBufs , SCALE_OutBufs
 *  ============================================================================
 */
NORMAL_API
Void
SCALE_Delete ()
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;

    SCALE_0Print ("Entered SCALE_Delete ()\n") ;

    /*
     *  Close the remote transport
     */
    status = MSGQ_TransportClose (REMOTEMQT) ;
    if (DSP_FAILED (status)) {
        SCALE_1Print ("MSGQ_TransportClose () failed. Status = [0x%x]\n",
                        status) ;
    }

    /*
     *  Stop execution on DSP.
     */
    tmpStatus = PROC_Stop (ID_PROCESSOR) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("PROC_Stop () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Free the buffer(s) allocated for channel from DSP
     */
    tmpStatus = CHNL_FreeBuffer (ID_PROCESSOR, CHNL_ID_INPUT, SCALE_InpBufs, 1) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("CHNL_FreeBuffer () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Free the buffer(s) allocated for channel to DSP
     */
    tmpStatus = CHNL_FreeBuffer (ID_PROCESSOR,
                                 CHNL_ID_OUTPUT,
                                 SCALE_OutBufs,
                                 1) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("CHNL_FreeBuffer () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Delete the input channel
     */
    tmpStatus = CHNL_Delete  (ID_PROCESSOR, CHNL_ID_INPUT) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("CHNL_Delete () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Delete the output channel
     */
    tmpStatus = CHNL_Delete  (ID_PROCESSOR, CHNL_ID_OUTPUT) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("CHNL_Delete () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Detach from the processor
     */
    tmpStatus = PROC_Detach (ID_PROCESSOR) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("PROC_Detach () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Close the local transport
     */
    tmpStatus = MSGQ_TransportClose (LOCALMQT) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("MSGQ_TransportClose () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Close the allocator
     */
    tmpStatus = MSGQ_AllocatorClose (MQABUFID) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("MSGQ_AllocatorClose () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Destroy the PROC object.
     */
    tmpStatus = PROC_Destroy () ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        SCALE_1Print ("PROC_Destroy () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    SCALE_0Print ("Leaving SCALE_Delete ()\n") ;
}


/** ============================================================================
 *  @func   SCALE_Main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
SCALE_Main (IN Char8 * dspExecutable,
            IN Char8 * strBufferSize,
            IN Char8 * strNumIterations)
{
    DSP_STATUS status = DSP_SOK ;

    SCALE_0Print ("=============== Sample Application : SCALE ==========\n") ;

    if (   (dspExecutable != NULL)
        && (strBufferSize != NULL)
        && (strNumIterations != NULL)) {

        /*
         *  Validate the buffer size and number of iterations specified.
         */
        SCALE_BufferSize = ALIGN (atoi (strBufferSize)) ;
        if (SCALE_BufferSize == 0) {
            status = DSP_ESIZE ;
        }

        SCALE_NumIterations = atoi (strNumIterations) ;
        if (SCALE_NumIterations == 0) {
            status = DSP_ERANGE ;
        }

        /*
         *  Specify the dsp executable file name and the buffer size for
         *  loop creation phase.
         */
        if (DSP_SUCCEEDED (status)) {
            status = SCALE_Create (dspExecutable,
                                  strBufferSize,
                                  strNumIterations) ;
        }

        /*
         *  Execute the data transfer loop.
         */
        if (DSP_SUCCEEDED (status)) {
            status = SCALE_Execute (SCALE_NumIterations) ;
        }

        /*
         *  Perform cleanup operation.
         */
        SCALE_Delete () ;
    }
    else {
        status = DSP_EINVALIDARG ;
        SCALE_0Print ("ERROR! Invalid arguments specified for "
                     "scale application\n") ;
    }

    SCALE_0Print ("====================================================\n") ;
}


#if defined (VERIFY_DATA)
/** ----------------------------------------------------------------------------
 *  @func   SCALE_VerifyData
 *
 *  @desc   This function verifies the data-integrity of given buffer.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
SCALE_VerifyData (IN Char8 * buf)
{
    DSP_STATUS status = DSP_SOK ;
    Uint16 *   ptr16  = NULL    ;
    Int16      i                ;

    /*
     *  Verify the data
     */
    ptr16 = (Uint16 *) (buf) ;
    for (i = 0 ;
         DSP_SUCCEEDED (status) && (i < SCALE_BufferSize / DSP_MAUSIZE) ;
         i++) {
        if (DSP_MAUSIZE == 1) {
            if (*buf != (XFER_CHAR * SCALE_XferValue)) {
                SCALE_0Print ("ERROR! Data integrity check failed\n") ;
                status = DSP_EFAIL ;
            }
            buf++ ;
        }
        else if (DSP_MAUSIZE == 2) {
            if (*ptr16 != (XFER_CHAR * SCALE_XferValue)) {
                SCALE_0Print ("ERROR! Data integrity check failed\n") ;
                status = DSP_EFAIL ;
            }
            ptr16++ ;
        }
    }

    return status ;
}
#endif


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
