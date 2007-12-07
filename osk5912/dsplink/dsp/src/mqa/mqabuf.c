/** ============================================================================
 *  @file   mqabuf.c
 *
 *  @path   $(DSPLINK)\dsp\src\mqa
 *
 *  @desc   Implementation of the buffer allocator. This module implements the
 *          allocator interface as required by the MSGQ model of DSP/BIOS(TM).
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <buf.h>
#include <mem.h>
#include <msgq.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <mqabuf.h>
#include <_mqabuf.h>
#include <dbc.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  MQABUF_MEM_SEGID
 *
 *  @desc   Default segment ID to be used for internal memory allocations.
 *  ============================================================================
 */
#define MQABUF_MEM_SEGID 0

/** ============================================================================
 *  @const  MQABUF_MEM_ALIGN
 *
 *  @desc   Default alignment to be used for internal memory allocations.
 *  ============================================================================
 */
#define MQABUF_MEM_ALIGN 0


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_open
 *
 *  @desc   Opens the buffer MQA and configures it according to the user
 *          attributes.
 *          This function is not reentrant.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *
 *  @ret    MSGQ_SUCCESS
 *              Component successfully opened.
 *          MSGQ_EMEMORY
 *              Failure during memory operation.
 *          MSGQ_EFAILURE
 *              General failure.
 *
 *  @enter  Handle to the MSGQ allocator object must be valid.
 *          Pointer to the attributes must be valid.
 *
 *  @leave  On success, the mqaInfo field in the MSGQ allocator object is set to
 *          a non-NULL value.
 *          On failure, the mqaInfo field in the MSGQ allocator object is set to
 *          NULL.
 *
 *  @see    MQABUF_Params, MQABUF_State, MQABUF_close ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQABUF_open (MSGQ_AllocatorHandle mqaHandle) ;


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_close
 *
 *  @desc   Closes the MQA, and cleans up its state object.
 *          This function is not reentrant.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *
 *  @ret    MSGQ_SUCCESS
 *              Component successfully closed.
 *          MSGQ_EMEMORY
 *              Failure during memory operation.
 *          MSGQ_EFAILURE
 *              General failure.
 *
 *  @enter  Handle to the MSGQ allocator object must be valid.
 *
 *  @leave  The mqtInfo field in the MSGQ transport object is set to NULL.
 *
 *  @see    MQABUF_State, MQABUF_open ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQABUF_close (MSGQ_AllocatorHandle mqaHandle) ;


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_alloc
 *
 *  @desc   Allocates a message buffer of the specified size.
 *          This function is reentrant.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *  @arg    size
 *              Pointer to the size (in MADUs) of the message to be allocated.
 *              On return, it stores the actual allocated size of the message,
 *              which is: On success: The same as the requested size
 *                        On failure: Zero
 *
 *  @ret    Valid message handle
 *              The message was successfully allocated
 *          NULL
 *              Failure in message allocation.
 *
 *  @enter  The MQA must be opened.
 *          Handle to the MSGQ allocator object must be valid.
 *          The pointer to the message size must be valid.
 *
 *  @leave  On success, size returned is greater than zero.
 *          On failure, size returned is zero.
 *
 *  @see    MQABUF_State, MQABUF_free ()
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_MsgHeader *
MQABUF_alloc (MSGQ_AllocatorHandle mqaHandle, Uint16 * size) ;


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_free
 *
 *  @desc   Frees a message buffer of the specified size.
 *          This function is reentrant.
 *
 *  @arg    mqaHandle
 *              Handle to the MSGQ allocator object.
 *  @arg    msg
 *              Address of the message to be freed.
 *  @arg    size
 *              Size (in MADUs) of the message to be freed.
 *
 *  @ret    None
 *
 *  @enter  The MQA must be opened.
 *          Handle to the MSGQ allocator object must be valid.
 *          The address of the message must be valid.
 *
 *  @leave  None
 *
 *  @see    MQABUF_State, MQABUF_alloc ()
 *  ----------------------------------------------------------------------------
 */
static
Void
MQABUF_free (MSGQ_AllocatorHandle mqaHandle,
             MSGQ_MsgHeader *     msg,
             Uint16               size) ;


/** ============================================================================
 *  @name   MQABUF_FXNS
 *
 *  @desc   Buffer allocator function pointer table.
 *  ============================================================================
 */
MSGQ_AllocatorFxns MQABUF_FXNS = {
    &MQABUF_open,
    &MQABUF_close,
    &MQABUF_alloc,
    &MQABUF_free
} ;


/** ============================================================================
 *  @name   MQABUF_PARAMS
 *
 *  @desc   Default parameters structure for the buffer MQA.
 *  ============================================================================
 */
static Uint16 MQABUF_MsgSize [3] = {16, 32, 64} ;
static Uint16 MQABUF_NumMsg  [3] = {10, 10, 10} ;
static MQABUF_Params MQABUF_PARAMS = {3,
                                       (Uint16 *) MQABUF_MsgSize,
                                       (Uint16 *) MQABUF_NumMsg,
                                       0
                                      } ;


/** ============================================================================
 *  Create named sections for all MQABUF functions to allow specific memory
 *  placement.
 *  ============================================================================
 */
#pragma CODE_SECTION (MQABUF_init,  ".text:init")
#pragma CODE_SECTION (MQABUF_exit,  ".text:exit")
#pragma CODE_SECTION (MQABUF_open,  ".text:MQABUF_open")
#pragma CODE_SECTION (MQABUF_close, ".text:MQABUF_close")
#pragma CODE_SECTION (MQABUF_alloc, ".text:MQABUF_alloc")
#pragma CODE_SECTION (MQABUF_free,  ".text:MQABUF_free")


/** ============================================================================
 *  @func   MQABUF_init
 *
 *  @desc   Performs global initialization of the buffer MQA.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
MQABUF_init ()
{
    /*  No initialization code is required */
}


/** ============================================================================
 *  @func   MQABUF_exit
 *
 *  @desc   Performs global finalization of the buffer MQA.
 *
 *  @modif  None
 *  ============================================================================
 */
Void
MQABUF_exit ()
{
    /*  No finalization code is required */
}


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_open
 *
 *  @desc   Opens the buffer MQA and configures it according to the user
 *          attributes.
 *          This function is not reentrant.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQABUF_open (MSGQ_AllocatorHandle mqaHandle)
{
    MSGQ_Status     status    = MSGQ_SUCCESS ;
    MQABUF_Params * mqaParams = mqaHandle->mqaParams;
    BUF_Attrs       bufAttrs  = BUF_ATTRS ;
    Uint16          count     = 0 ;
    MQABUF_State *  mqaState ;
    MQABUF_Handle   bufPools ;
    Uint16          numBufPools ;
    Uint16          i ;
    Uint16          j ;
    Uint16 *        msgSize ;
    Uint16 *        numMsg ;

    DBC_require (mqaHandle != NULL) ;

    /*  If no attributes are specified, use the default ones. */
    if (mqaParams == NULL) {
        mqaParams = (MQABUF_Params *) &MQABUF_PARAMS ;
    }

    mqaState = MEM_alloc (MQABUF_MEM_SEGID,
                          sizeof (MQABUF_State),
                          MQABUF_MEM_ALIGN) ;
    if (mqaState != MEM_ILLEGAL) {
        numBufPools = mqaParams->numBufPools ;
        /*  Check if user has specified more than one buffer pool of same sized
         *  messages. If so, combine such pools into a single one with the
         *  total number of messages
         */
        msgSize = MEM_alloc  (MQABUF_MEM_SEGID,
                              (numBufPools * sizeof (Uint16)),
                              MQABUF_MEM_ALIGN) ;
        numMsg  = MEM_calloc (MQABUF_MEM_SEGID,
                              (numBufPools * sizeof (Uint16)),
                              MQABUF_MEM_ALIGN) ;
        if ((msgSize != NULL) && (numMsg != NULL)) {
            for (i = 0 ; i < numBufPools ; i++) {
                if (numMsg [i] != (Uint16) -1) {
                    msgSize [count] = mqaParams->msgSize [i] ;
                    numMsg [count]  = mqaParams->numMsg  [i] ;
                    for (j = i + 1 ; j < numBufPools ; j++) {
                        if (msgSize [count] == mqaParams->msgSize [j]) {
                            numMsg [count] += mqaParams->numMsg [j] ;
                            numMsg [j] = (Uint16) -1 ;
                        }
                    }
                    count++ ;
                }
            }

            bufPools = MEM_alloc (MQABUF_MEM_SEGID,
                                  (count * sizeof (MQABUF_Obj)),
                                  MQABUF_MEM_ALIGN) ;
            if (bufPools != MEM_ILLEGAL) {
                mqaState->numBufPools = count ;
                mqaState->bufPools    = bufPools ;
                bufAttrs.segid = mqaParams->segId ;

                for (i = 0 ; ((i < count) && (status == MSGQ_SUCCESS)) ; i++) {
                    bufPools [i].msgSize = msgSize [i] ;
                    bufPools [i].msgList = BUF_create (
                                      numMsg  [i],/* Number of buffers */
                                      msgSize [i],/* Size of each buffer */
                                      1,          /* Alignment of each buffer */
                                      &bufAttrs) ;/* Attributes */
                    if (bufPools [i].msgList == NULL) {
                        status = MSGQ_EFAILURE ;
                    }
                }
            }
            else {
                status = MSGQ_EMEMORY ;
            }
        }
        else {
            status = MSGQ_EMEMORY ;
        }

        /* Free temporary memory */
        if (msgSize != MEM_ILLEGAL) {
            MEM_free (MQABUF_MEM_SEGID,
                      msgSize,
                      (numBufPools * sizeof (Uint16))) ;
        }
        if (numMsg != MEM_ILLEGAL) {
            MEM_free (MQABUF_MEM_SEGID,
                      numMsg,
                      (numBufPools * sizeof (Uint16))) ;
        }
    }
    else {
        status = MSGQ_EMEMORY ;
    }

    mqaHandle->mqaInfo = mqaState ;

    if (status != MSGQ_SUCCESS) {
        /*  Cleanup any allocated resources on failure */
        if (mqaState != MEM_ILLEGAL) {
            MQABUF_close (mqaHandle) ;
        }
    }

    DBC_ensure (    ((status == MSGQ_SUCCESS) && (mqaHandle->mqaInfo != NULL))
                ||  (status != MSGQ_SUCCESS)) ;
    DBC_ensure (    ((status != MSGQ_SUCCESS) && (mqaHandle->mqaInfo == NULL))
                ||  (status == MSGQ_SUCCESS)) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_close
 *
 *  @desc   Closes the MQA, and cleans up its state object.
 *          This function is not reentrant.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_Status
MQABUF_close (MSGQ_AllocatorHandle mqaHandle)
{
    MSGQ_Status     status      = MSGQ_SUCCESS ;
    Bool            freeStatus  = TRUE ;
    MQABUF_State *  mqaState ;
    MQABUF_Handle   bufPools ;
    Uint16          numBufPools ;
    Uint16          i ;

    DBC_require (mqaHandle != NULL) ;

    mqaState = (MQABUF_State *) mqaHandle->mqaInfo ;
    DBC_assert (mqaState != NULL) ;

    bufPools    = mqaState->bufPools ;
    numBufPools = mqaState->numBufPools ;
    if (bufPools != MEM_ILLEGAL) {
        for (i = 0 ;
             ((i < numBufPools) && (bufPools [i].msgList != NULL)) ;
             i++) {
            freeStatus = BUF_delete (bufPools [i].msgList) && freeStatus ;
        }
        freeStatus =    MEM_free (MQABUF_MEM_SEGID,
                                  bufPools,
                                  (numBufPools * sizeof (MQABUF_Obj)))
                     && freeStatus ;
    }
    freeStatus =    MEM_free (MQABUF_MEM_SEGID,
                              mqaState,
                              sizeof (MQABUF_State))
                 && freeStatus ;

    if (freeStatus == FALSE) {
        status = MSGQ_EMEMORY ;
    }

    mqaHandle->mqaInfo = NULL ;

    DBC_ensure (mqaHandle->mqaInfo == NULL) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_alloc
 *
 *  @desc   Allocates a message buffer of the specified size.
 *          This function is reentrant.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
MSGQ_MsgHeader *
MQABUF_alloc (MSGQ_AllocatorHandle mqaHandle, Uint16 * size)
{
    MSGQ_MsgHeader * msg      = NULL ;
    MQABUF_State *   mqaState ;
    MQABUF_Handle    bufPools ;
    Uint16           i ;

    DBC_require (mqaHandle != NULL) ;
    DBC_require (size != NULL) ;

    mqaState = (MQABUF_State *) mqaHandle->mqaInfo ;
    DBC_assert (mqaState != NULL) ;

    bufPools = mqaState->bufPools ;
    for (i = 0 ; i < mqaState->numBufPools ; i++) {
        if (bufPools [i].msgSize == *size) {
            msg = (MSGQ_MsgHeader *) BUF_alloc (bufPools [i].msgList) ;
            break ;
        }
    }

    if (msg == NULL) {
        *size = 0 ;
    }

    DBC_ensure (    ((msg != NULL) && (*size > 0))
                ||  ((msg == NULL) && (*size == 0))) ;

    return msg ;
}


/** ----------------------------------------------------------------------------
 *  @func   MQABUF_free
 *
 *  @desc   Frees a message buffer of the specified size.
 *          This function is reentrant.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
static
Void
MQABUF_free (MSGQ_AllocatorHandle mqaHandle, MSGQ_MsgHeader * msg, Uint16 size)
{
    MQABUF_State * mqaState ;
    MQABUF_Handle  bufPools ;
    Uint16         i ;

    DBC_require (mqaHandle != NULL) ;
    DBC_require (msg != NULL) ;

    mqaState = (MQABUF_State *) mqaHandle->mqaInfo ;
    DBC_assert (mqaState != NULL) ;

    bufPools = mqaState->bufPools ;
    for (i = 0 ; i < mqaState->numBufPools ; i++) {
        if (bufPools [i].msgSize == size) {
            BUF_free (bufPools [i].msgList, (Ptr) msg) ;
            break ;
        }
    }
}


#ifdef __cplusplus
}
#endif /* extern "C" */
