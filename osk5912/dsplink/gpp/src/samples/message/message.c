/** ============================================================================
 *  @file   message.c
 *
 *  @path   $(DSPLINK)\gpp\src\samples\message
 *
 *  @desc   This is an application which sends messages to the DSP
 *          processor and receives them back using DSP/BIOS LINK.
 *          The message contents received are verified against the data
 *          sent to DSP.
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
#include <msgq.h>

/*  ----------------------------------- Application Header              */
#include <message.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/*  ============================================================================
 *  @const   NUM_ARGS
 *
 *  @desc   Number of arguments specified to the DSP application.
 *  ============================================================================
 */
#define NUM_ARGS 0

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

/** ============================================================================
 *  @const  GPPMSGQID
 *
 *  @desc   Id of the message queue on the GPP
 *  ============================================================================
 */
#define GPPMSGQID   0

/** ============================================================================
 *  @const  NUMMSGPOOLS
 *
 *  @desc   Number of BUF pools in the allocator
 *  ============================================================================
 */
#define NUMMSGPOOLS   3

/** ============================================================================
 *  @const  APP_BUFFER_SIZE
 *
 *  @desc   Messaging buffer used by the application.
 *          Note: This buffer is aligned according to the alignment expected
 *          by the platform.
 *  ============================================================================
 */
#define APP_BUFFER_SIZE  ALIGN (sizeof (MsgqMsgHeader))

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
#define NUMMSGINPOOL1   2

/** ============================================================================
 *  @const  NUMMSGINPOOL2
 *
 *  @desc   Number in third message pool.
 *  ============================================================================
 */
#define NUMMSGINPOOL2   4

/** ============================================================================
 *  @name   msgSizes
 *
 *  @desc   Message sizes managed by the allocator.
 *  ============================================================================
 */
STATIC Uint16 msgSizes[NUMMSGPOOLS] =  {APP_BUFFER_SIZE,
                                        RMQT_CTRLMSG_SIZE,
                                        sizeof (MsgqAsyncErrorMsg)};

/** ============================================================================
 *  @name   msgCount
 *
 *  @desc   Number of messages in each pool managed by the allocator.
 *  ============================================================================
 */
STATIC Uint16 msgCount[NUMMSGPOOLS] = {NUMMSGINPOOL0,
                                       NUMMSGINPOOL1,
                                       NUMMSGINPOOL2};

/** ============================================================================
 *  @name   mqaBufAttrs
 *
 *  @desc   Configuration parameters for the BUF-based allocator instance.
 *  ============================================================================
 */
STATIC MqaBufAttrs mqaBufAttrs = {NUMMSGPOOLS,    /* Number of pools     */
                                  msgSizes,       /* Array of sizes      */
                                  msgCount};      /* Array of msg counts */

/** ============================================================================
 *  @const  NUMLOCALMSGQUEUES
 *
 *  @desc   Number of local message queues
 *  ============================================================================
 */
#define NUMLOCALMSGQUEUES   1

/** ============================================================================
 *  @name   mqtLocalAttrs
 *
 *  @desc   Local transport attributes.
 *  ============================================================================
 */
STATIC LmqtAttrs mqtLocalAttrs = {NUMLOCALMSGQUEUES};

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
 *  @desc   Remote transport attributes.
 *  ============================================================================
 */
STATIC RmqtAttrs mqtRemoteAttrs = {NUMREMOTEMSGQUEUES,       /* maxNumMsgq    */
                                   RMQT_CTRLMSG_SIZE,        /* maxMsgSize    */
                                   MQABUFID};                /* defaultMqaId  */


#if defined (VERIFY_DATA)
/** ----------------------------------------------------------------------------
 *  @func   MESSAGE_VerifyData
 *
 *  @desc   This function verifies the data-integrity of given message.
 *
 *  @arg    msg
 *              This is the pointer of the message whose contents are to be
 *              validated.
 *  @arg    sequenceNumber
 *              Expected value of the msgqId.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Contents of the message is unexpected.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
MESSAGE_VerifyData (IN MsgqMsg msg, IN Uint16 sequenceNumber) ;
#endif /* if defined (VERIFY_DATA) */


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
 *  @func   MESSAGE_Create
 *
 *  @desc   This function allocates and initializes resources used by
 *          this application.
 *
 *  @modif  MESSAGE_InpBufs , MESSAGE_OutBufs
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MESSAGE_Create (IN Char8 * dspExecutable)
{
    DSP_STATUS      status         = DSP_SOK  ;
    Uint32          numArgs        = NUM_ARGS ;
    MsgqLocateAttrs locateAttrs               ;

    MESSAGE_0Print ("Entered MESSAGE_Create ()\n") ;

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
            MESSAGE_1Print ("MSGQ_AllocatorOpen () failed. Status = [0x%x]\n",
                            status) ;
        }
    }
    else {
        MESSAGE_1Print ("PROC_Setup () failed. Status = [0x%x]\n", status) ;
    }

    /*
     *  Open the local transport.
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_TransportOpen (LOCALMQT, &mqtLocalAttrs) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("MSGQ_TransportOpen () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Create the GPP's message queue
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_Create (GPPMSGQID, NULL) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("MSGQ_Create () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Set the message queue that will receive any async. errors
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_SetErrorHandler (GPPMSGQID, MQABUFID) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("MSGQ_SetErrorHandler () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Attach the Dsp with which the transfers have to be done.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Attach (ID_PROCESSOR, NULL) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("PROC_Attach () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Load the executable on the DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Load (ID_PROCESSOR, dspExecutable, numArgs, NULL) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("PROC_Load () failed. Status = [0x%x]\n", status) ;
        }
    }

     /*
      *  Start execution on DSP.
      */
    if (DSP_SUCCEEDED (status)) {
        status = PROC_Start (ID_PROCESSOR) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("PROC_Start () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    /*
     *  Open the remote transport.
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_TransportOpen (REMOTEMQT, &mqtRemoteAttrs) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("MSGQ_TransportOpen () failed. Status = [0x%x]\n",
                            status) ;
        }
     }

    /*
     *  Locate the DSP's message queue
     */
    if (DSP_SUCCEEDED (status)) {
        locateAttrs.timeout = WAIT_FOREVER ;
        status = DSP_ENOTFOUND ;
        while (status == DSP_ENOTFOUND) {
            status = MSGQ_Locate (ID_PROCESSOR, DSPMSGQID, &locateAttrs) ;
            if (status == DSP_ENOTFOUND) {
                MESSAGE_Sleep (100000) ;
            }
            else if (DSP_FAILED (status)) {
                MESSAGE_1Print ("MSGQ_Locate () failed. Status = [0x%x]\n",
                                status) ;
            }
        }
    }

    MESSAGE_0Print ("Leaving MESSAGE_Create ()\n") ;

    return status ;
}


/** ============================================================================
 *  @func   MESSAGE_Execute
 *
 *  @desc   This function implements the execute phase for this application.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MESSAGE_Execute (IN Uint32 numIterations)
{
    DSP_STATUS  status         = DSP_SOK ;
    Uint16      sequenceNumber = 0       ;
    Uint32      i                        ;
    MsgqMsg     msg                      ;

    MESSAGE_0Print ("Entered MESSAGE_Execute ()\n") ;

    /*
     *  Allocate the message.
     */
    if (DSP_SUCCEEDED (status)) {
        status = MSGQ_Alloc (MQABUFID, APP_BUFFER_SIZE, &msg) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("MSGQ_Alloc () failed. Status = [0x%x]\n",
                            status) ;
        }
    }

    for (i = 1; (i <= numIterations) && (DSP_SUCCEEDED (status)); i++) {
        /*
         *  Send the message
         */
        status = MSGQ_Put (ID_PROCESSOR,
                           DSPMSGQID,
                           msg,
                           sequenceNumber,
                           MSGQ_INVALID_ID) ;

        /*
         *  Receive the message back.
         */
        if (DSP_SUCCEEDED (status)) {
            status = MSGQ_Get (GPPMSGQID, WAIT_FOREVER, &msg) ;
            if (DSP_FAILED (status)) {
                MESSAGE_1Print ("MSGQ_Get () failed. Status = [0x%x]\n",
                                status) ;
            }
        }
        else {
            MESSAGE_1Print ("MSGQ_Put () failed. Status = [0x%x]\n", status) ;
        }

#if defined (VERIFY_DATA)
        /*
         *  Verify correctness of data received.
         */
        if (DSP_SUCCEEDED (status)) {
            status = MESSAGE_VerifyData (msg, sequenceNumber) ;
        }
#endif
        sequenceNumber++ ;
        if (DSP_SUCCEEDED (status) && ((i % 1000) == 0)) {
            MESSAGE_1Print ("Transferred %d messages\n", i) ;
        }
    }

    /*
     *  Free the message if present.
     */
    if (msg != NULL) {
        status = MSGQ_Free (msg) ;
        if (DSP_FAILED (status)) {
            MESSAGE_1Print ("MSGQ_Free () failed. Status = [0x%x]\n", status) ;
        }
    }

    MESSAGE_0Print ("Leaving MESSAGE_Execute ()\n") ;

    return status ;
}


/** ============================================================================
 *  @func   MESSAGE_Delete
 *
 *  @desc   This function releases resources allocated earlier by call to
 *          MESSAGE_Create ().
 *          During cleanup, the allocated resources are being freed
 *          unconditionally. Actual applications may require stricter check
 *          against return values for robustness.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
MESSAGE_Delete ()
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;

    MESSAGE_0Print ("Entered MESSAGE_Delete ()\n") ;

    /*
     *  Release the remote message queue
     */
    status = MSGQ_Release (ID_PROCESSOR, DSPMSGQID) ;
    if (DSP_FAILED (status)) {
        MESSAGE_1Print ("MSGQ_Release () failed. Status = [0x%x]\n", status) ;
    }

    /*
     *  Close the remote transport
     */
    tmpStatus = MSGQ_TransportClose (REMOTEMQT) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("MSGQ_TransportClose () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Stop execution on DSP.
     */
    tmpStatus = PROC_Stop (ID_PROCESSOR) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("PROC_Stop () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Detach from the processor
     */
    tmpStatus = PROC_Detach  (ID_PROCESSOR) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("PROC_Detach () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     * Reset the error handler before deleting the MSGQ that receives
     * the error messages.
     */
    tmpStatus = MSGQ_SetErrorHandler (MSGQ_INVALID_ID, MSGQ_INVALID_ID) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("MSGQ_SetErrorHandler () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Delete the local message queue
     */
    tmpStatus = MSGQ_Delete (GPPMSGQID) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("MSGQ_Delete () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Close the local transport
     */
    tmpStatus = MSGQ_TransportClose (LOCALMQT) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("MSGQ_TransportClose () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Close the allocator
     */
    tmpStatus = MSGQ_AllocatorClose (MQABUFID) ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("MSGQ_AllocatorClose () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    /*
     *  Destroy the PROC object.
     */
    tmpStatus = PROC_Destroy () ;
    if (DSP_SUCCEEDED (status) && DSP_FAILED (tmpStatus)) {
        MESSAGE_1Print ("PROC_Destroy () failed. Status = [0x%x]\n",
                        tmpStatus) ;
    }

    MESSAGE_0Print ("Leaving MESSAGE_Delete ()\n") ;
}


/** ============================================================================
 *  @func   MESSAGE_Main
 *
 *  @desc   Entry point for the application
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
MESSAGE_Main (IN Char8 * dspExecutable,
              IN Char8 * strNumIterations)
{
    DSP_STATUS status        = DSP_SOK ;
    Uint32     numIterations = 0       ;

    MESSAGE_0Print ("========== Sample Application : MESSAGE ==========\n") ;

    if ((dspExecutable != NULL) && (strNumIterations != NULL)) {
        numIterations = atoi (strNumIterations) ;
        if (numIterations == 0) {
            status = DSP_ERANGE ;
        }

        /*
         *  Specify the dsp executable file name for message creation phase.
         */
        if (DSP_SUCCEEDED (status)) {
            status = MESSAGE_Create (dspExecutable) ;
        }

        /*
         *  Execute the message execute phase.
         */
        if (DSP_SUCCEEDED (status)) {
            status = MESSAGE_Execute (numIterations) ;
        }

        /*
         *  Perform cleanup operation.
         */
        MESSAGE_Delete () ;
    }
    else {
        status = DSP_EINVALIDARG ;
        MESSAGE_0Print ("ERROR! Invalid arguments specified for  "
                     "message application\n") ;
    }

    MESSAGE_0Print ("====================================================\n") ;
}


#if defined (VERIFY_DATA)
/** ----------------------------------------------------------------------------
 *  @func   MESSAGE_VerifyData
 *
 *  @desc   This function verifies the data-integrity of given buffer.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
MESSAGE_VerifyData (IN MsgqMsg msg, IN Uint16 sequenceNumber)
{
    DSP_STATUS status = DSP_SOK ;
    Uint16     msgId ;

    /*
     *  Verify the message
     */
    msgId = MSGQ_GetMsgId(msg);
    if ((DSP_SUCCEEDED (status)) &&  (msgId != sequenceNumber)) {
        status = DSP_EFAIL;
        MESSAGE_0Print ("ERROR! Data integrity check failed\n") ;
    }

    return status ;
}
#endif /* if defined (VERIFY_DATA) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
