/** ============================================================================
 *  @file   mqabuf.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv
 *
 *  @desc   Implements the BUF MQA interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

#include <msgqdefs.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                     */
#include <profile.h>

/*  ----------------------------------- OSAL Header                   */
#include <mem.h>
#include <mem_os.h>
#include <print.h>

/*  ----------------------------------- Generic Function              */
#include <gen_utils.h>
#include <buf.h>

/*  ----------------------------------- Link Driver                   */
#include <ldrv_mqa.h>
#include <mqabuf.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent ID.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_MQABUF


/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_LDRV_MQABUF, __LINE__)


/** ============================================================================
 *  @name   MQABUF_IsInitialized
 *
 *  @desc   Flag to keep track of initialization of this subcomponent.
 *  ============================================================================
 */
STATIC Bool MQABUF_IsInitialized = FALSE ;


/** ============================================================================
 *  @name   MQABUF_Interface
 *
 *  @desc   The MQA APIs are exposed to MSGQ through this function pointer
 *          table.
 *  ============================================================================
 */
MqaInterface MQABUF_Interface = {
    &MQABUF_Initialize,
    &MQABUF_Finalize,
    &MQABUF_Open,
    &MQABUF_Close,
    &MQABUF_Alloc,
    &MQABUF_Free
} ;


/** ============================================================================
 *  @func   MQABUF_Initialize
 *
 *  @desc   Performs global initialization of the buffer MQA.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
MQABUF_Initialize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("MQABUF_Initialize") ;

    DBC_Require (MQABUF_IsInitialized == FALSE) ;

    if (MQABUF_IsInitialized == FALSE) {
        status = BUF_Initialize () ;

        if (DSP_SUCCEEDED (status)) {
            MQABUF_IsInitialized = TRUE ;
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_0LEAVE ("MQABUF_Initialize") ;
}


/** ============================================================================
 *  @func   MQABUF_Finalize
 *
 *  @desc   Performs global finalization of the buffer MQA.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
MQABUF_Finalize ()
{
    DSP_STATUS status = DSP_SOK ;

    TRC_0ENTER ("MQABUF_Finalize") ;

    DBC_Require (MQABUF_IsInitialized == TRUE) ;

    if (MQABUF_IsInitialized == TRUE) {
        status = BUF_Finalize () ;

        if (DSP_FAILED (status)) {
            SET_FAILURE_REASON ;
        }
    }

    MQABUF_IsInitialized = FALSE ;

    TRC_0LEAVE ("MQABUF_Finalize") ;
}


/** ============================================================================
 *  @func   MQABUF_Open
 *
 *  @desc   Opens the buffer MQA and configures it according to the user
 *          attributes. This function is called once during
 *          LDRV_MSGQ_AllocatorOpen (). It creates an instance of the
 *          MqaBufState object, initializes it, and returns the handle to the
 *          caller. This handle to the MQA state object is passed to the
 *          allocator during its functions for allocating and freeing the
 *          messages. The steps involved in initialization of the MQA are:
 *
 *              1.Calculate the total size of buffer required for the MQA.
 *                This is equivalent to:
 *                The sum of (For each pool: (numMsgs * msgSize))
 *
 *              2.Allocate a contiguous buffer of calculated size.
 *
 *              3.Create and initialize each buffer pool through calls to
 *                BUF_Create (), passing the kernel address of the contiguous
 *                buffer reserved for each pool through step (2).
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Open (IN  LdrvMsgqAllocatorHandle mqaHandle, IN  Pvoid mqaAttrs)
{
    DSP_STATUS      status      = DSP_SOK ;
    DSP_STATUS      tmpStatus   = DSP_SOK ;
    MqaBufState  *  mqaState    = NULL    ;
    MqaBufObj *     bufPools    = NULL    ;
    Uint16          i           = 0       ;
    Uint16          j           = 0       ;
    Uint16          count       = 0       ;
    Uint16          numBufPools = 0       ;
    MqaBufAttrs *   bufAttrs    = NULL    ;
    Uint32          size        = 0       ;
    Uint32          startAddr   = 0       ;
    Uint16 *        msgSize     = NULL    ;
    Uint16 *        numMsg      = NULL    ;
    MemAllocAttrs   allocArg              ;

    TRC_2ENTER ("MQABUF_Open", mqaHandle, mqaAttrs) ;

    DBC_Require (MQABUF_IsInitialized == TRUE) ;
    DBC_Require (mqaHandle != NULL) ;
    DBC_Require (mqaAttrs != NULL) ;

    if ((mqaHandle == NULL) || (mqaAttrs == NULL) ) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        bufAttrs = (MqaBufAttrs *) mqaAttrs ;

        /*  Check if the size field contains the correct size of the structure
         *  to be passed
         */
        if ((bufAttrs->msgSize != NULL) && (bufAttrs->numMsg != NULL)) {
            /* Allocate memory for the state object */
            status = MEM_Calloc ((Void **) &mqaState,
                                 (sizeof (MqaBufState)),
                                 MEM_DEFAULT) ;
            if (DSP_FAILED (status)) {
                SET_FAILURE_REASON ;
            }
            else {
                mqaHandle->mqaInfo = mqaState ;
            }
        }
        else {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        /*  Check if user has specified more than one buffer pool of same sized
         *  messages. If so, combine such pools into a single one with the
         *  total number of messages.
         */
        numBufPools = bufAttrs->numBufPools ;
        status = MEM_Alloc ((Void **)&msgSize,
                            (numBufPools * sizeof (Uint16)),
                            MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            status = MEM_Calloc ((Void **)&numMsg,
                                (numBufPools * sizeof (Uint16)),
                                MEM_DEFAULT) ;
            if (DSP_SUCCEEDED (status)) {
                for (i = 0, count = 0 ; i < numBufPools ; i++) {
                    if (numMsg [i] != (Uint16) -1) {
                        msgSize [count] = bufAttrs->msgSize [i] ;
                        numMsg [count]  = bufAttrs->numMsg  [i] ;
                        for (j = i + 1 ; j < numBufPools ; j++) {
                            if (msgSize [count] == bufAttrs->msgSize [j]) {
                                numMsg [count] += bufAttrs->numMsg [j] ;
                                numMsg [j] = (Uint16) -1 ;
                            }
                        }
                        count++ ;
                    }
                }
            }
            else {
                SET_FAILURE_REASON ;
                FREE_PTR (msgSize) ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        /* Allocate memory for the buffer pool objects */
        numBufPools = count ;
        mqaState->numBufPools = numBufPools ;
        status = MEM_Calloc ((Void **) &bufPools,
                            (numBufPools * sizeof (MqaBufObj)),
                            MEM_DEFAULT) ;
        if (DSP_SUCCEEDED (status)) {
            mqaState->bufPools = bufPools ;
            /*  Calculate the size of the continuous memory chunk which
             *  has to be allocated
             */
            for (i = 0 ; i < numBufPools ; i++) {
                size += (numMsg [i] * msgSize [i]) ;
            }
            mqaState->size =  size ;
            status = MEM_Alloc ((Void **) &(mqaState->virtAddr),
                                size,
                                &allocArg) ;
            if (DSP_SUCCEEDED (status)) {
                startAddr = mqaState->virtAddr ;
                /* Save the physical adderess in the state object */
                mqaState->phyAddr = (Uint32) allocArg.physicalAddress ;
                for (i = 0 ;
                     ((i < numBufPools) && (DSP_SUCCEEDED (status))) ;
                     i++) {
                    /*  Create BUF pools for the arguments specified by
                     *  the user
                     */
                    bufPools [i].msgSize = msgSize [i] ;
                    status = BUF_Create (numMsg  [i],
                                         msgSize [i],
                                         &(bufPools [i].msgList),
                                         startAddr) ;
                    startAddr +=   (numMsg  [i])
                                 * (msgSize [i]) ;
                }
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
        if (DSP_FAILED (status)) {
            MQABUF_Close (mqaHandle) ;
        }

        tmpStatus = FREE_PTR (msgSize) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }

        tmpStatus = FREE_PTR (numMsg) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MQABUF_Open", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MQABUF_Close
 *
 *  @desc   This function closes the MQA, and cleans up its state object.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Close (IN  LdrvMsgqAllocatorHandle mqaHandle)
{
    DSP_STATUS      status      = DSP_SOK ;
    DSP_STATUS      tmpStatus   = DSP_SOK ;
    MqaBufState *   mqaState    = NULL    ;
    MqaBufObj *     bufPools    = NULL    ;
    Uint16          numBufPools = 0       ;
    Uint16          i           = 0       ;
    MemFreeAttrs    freeArg               ;

    TRC_1ENTER ("MQABUF_Close", mqaHandle) ;

    DBC_Require (MQABUF_IsInitialized == TRUE) ;
    DBC_Require (mqaHandle != NULL) ;

    if (mqaHandle == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqaState = (MqaBufState *) (mqaHandle->mqaInfo) ;
        if (mqaState == NULL) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    if (DSP_SUCCEEDED (status)) {
        bufPools = mqaState->bufPools ;
        numBufPools = mqaState->numBufPools ;

        if (bufPools != NULL) {
            /* Delete the BUF pools */
            for (i = 0 ; ((i < numBufPools) && (bufPools [i].msgList != NULL)) ;
                 i++) {
                tmpStatus = BUF_Delete (bufPools [i].msgList) ;
                if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                    status = tmpStatus ;
                    SET_FAILURE_REASON ;
                }
            }
            /* Free memory for the buffer pool objects */
            tmpStatus = FREE_PTR (bufPools) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }

        if ((Pvoid) (mqaState->virtAddr) != NULL) {
            /*  Free memory for the continuous memory chunk which was created
             *  for creating BUF pools
             */
            freeArg.physicalAddress = (Uint32 *) (mqaState->phyAddr) ;
            freeArg.size            = mqaState->size ;
            tmpStatus = MEM_Free ((Pvoid)(mqaState->virtAddr), &freeArg) ;
            if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
                status = tmpStatus ;
                SET_FAILURE_REASON ;
            }
        }

        /* Free memory for the state object  */
        tmpStatus = FREE_PTR (mqaState) ;
        if ((DSP_SUCCEEDED (status)) && (DSP_FAILED (tmpStatus))) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
        mqaHandle->mqaInfo = NULL ;
    }

    TRC_1LEAVE ("MQABUF_Close", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MQABUF_Alloc
 *
 *  @desc   This function allocates a message buffer of the specified size.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Alloc (IN     LdrvMsgqAllocatorHandle mqaHandle,
              IN OUT Uint16 *                size,
              OUT    MsgqMsg *               addr)
{
    DSP_STATUS       status      = DSP_SOK ;
    MqaBufState *    mqaState    = NULL    ;
    MqaBufObj *      bufPools    = NULL    ;
    Bool             found       = FALSE   ;
    Uint16           i           = 0       ;

    TRC_3ENTER ("MQABUF_Alloc", mqaHandle, size, addr) ;

    DBC_Require (MQABUF_IsInitialized == TRUE) ;
    DBC_Require (mqaHandle != NULL) ;
    DBC_Require (size != NULL) ;
    DBC_Require (addr != NULL) ;

    if ((mqaHandle == NULL) || (size == NULL) || (addr == NULL)) {
        status = DSP_EINVALIDARG ;
        if (addr != NULL) {
            *addr = NULL ;
        }
        SET_FAILURE_REASON ;
    }
    else {
        *addr = NULL ;
        mqaState = (MqaBufState *) (mqaHandle->mqaInfo) ;
        DBC_Assert (mqaState != NULL) ;
        if (mqaState != NULL) {
            bufPools = mqaState->bufPools ;

            for (i = 0 ; ((i < mqaState->numBufPools) && (found != TRUE)) ;
                 i++) {
                if (bufPools [i].msgSize == *size) {
                    found = TRUE ;
                    status = BUF_Alloc (bufPools [i].msgList, (Pvoid *) addr) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
            }
            /* Invalid size requested by user */
            if (found == FALSE) {
                status = DSP_EINVALIDARG ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    DBC_Ensure (   ((addr == NULL) && DSP_FAILED (status))
                || (   (addr != NULL)
                     && (*addr == NULL)
                     && DSP_FAILED (status))
                || (DSP_SUCCEEDED (status))) ;

    DBC_Ensure ((   (addr != NULL)
                    && (*addr != NULL)
                    && DSP_SUCCEEDED (status))
                 || (DSP_FAILED (status))) ;

    TRC_1LEAVE ("MQABUF_Alloc", status) ;

    return status ;
}


/** ============================================================================
 *  @func   MQABUF_Free
 *
 *  @desc   This function frees a message buffer of the specified size.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
MQABUF_Free (IN  LdrvMsgqAllocatorHandle mqaHandle,
             IN  MsgqMsg                 addr,
             IN  Uint16                  size)
{
    DSP_STATUS       status      = DSP_SOK ;
    MqaBufState *    mqaState    = NULL    ;
    MqaBufObj *      bufPools    = NULL    ;
    Bool             found       = FALSE   ;
    Uint16           i           = 0       ;

    TRC_3ENTER ("MQABUF_Free", mqaHandle, addr, size) ;

    DBC_Require (MQABUF_IsInitialized == TRUE) ;
    DBC_Require (mqaHandle != NULL) ;
    DBC_Require (addr != NULL) ;

    if ((mqaHandle == NULL) || (addr == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        mqaState = (MqaBufState *) (mqaHandle->mqaInfo) ;
        DBC_Assert (mqaState != NULL) ;
        if (mqaState != NULL) {
            bufPools = mqaState->bufPools ;

            for (i = 0 ; ((i < mqaState->numBufPools) && (found != TRUE)) ;
                 i++) {
                if (bufPools [i].msgSize == size) {
                    found = TRUE ;
                    status = BUF_Free (bufPools [i].msgList, addr) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                }
            }
            /* Invalid size requested by user */
            if (found == FALSE) {
                status = DSP_EINVALIDARG ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("MQABUF_Free", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
