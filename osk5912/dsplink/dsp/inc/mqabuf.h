/** ============================================================================
 *  @file   mqabuf.h
 *
 *  @path   $(DSPLINK)\dsp\inc
 *
 *  @desc   Defines the exported interface of the buffer MQA.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef MQABUF_
#define MQABUF_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <msgq.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @name   MQABUF_Attrs
 *
 *  @desc   Attributes required for initialization of the buffer MQA.
 *
 *  @field  numBufPools
 *              Number of buffer pools to be configured in the MQA.
 *  @field  msgSize
 *              Array of sizes of the messages in the buffer pools.
 *              The sizes are in MADUs.
 *              This array is of size numBufPools.
 *  @field  numMsg
 *              Array of number of messages in all buffer pools.
 *              This array is of size numBufPools.
 *  @field  segId
 *              Segment ID for allocation of the buffers.
 *  ============================================================================
 */
typedef struct MQABUF_Params_tag {
    Uint16     numBufPools ;
    Uint16 *   msgSize ;
    Uint16 *   numMsg ;
    Uint16     segId ;
} MQABUF_Params ;


/** ============================================================================
 *  @name   MQABUF_FXNS
 *
 *  @desc   Declaration of the function table interface for the buffer MQA.
 *  ============================================================================
 */
extern MSGQ_AllocatorFxns MQABUF_FXNS ;


/** ============================================================================
 *  @func   MQABUF_init
 *
 *  @desc   Performs global initialization of the buffer MQA.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MQABUF_exit ()
 *  ============================================================================
 */
extern Void MQABUF_init () ;


/** ============================================================================
 *  @func   MQABUF_exit
 *
 *  @desc   Performs global finalization of the buffer MQA.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MQABUF_init ()
 *  ============================================================================
 */
extern Void MQABUF_exit () ;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* MQABUF_ */
