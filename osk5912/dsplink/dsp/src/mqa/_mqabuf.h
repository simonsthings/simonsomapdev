/** ============================================================================
 *  @file   _mqabuf.h
 *
 *  @path   $(DSPLINK)\dsp\src\mqa
 *
 *  @desc   Internal declarations for the buffer MQA.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef _MQABUF_
#define _MQABUF_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <buf.h>
#include <msgq.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ----------------------------------------------------------------------------
 *  @name   MQABUF_Obj
 *
 *  @desc   Buffer object for the allocator, representing a single buffer pool.
 *
 *  @field  msgSize
 *              Size (in MADUs) of the messages in the buffer pool.
 *  @field  msgList
 *              List of messages in the buffer pool.
 *  ----------------------------------------------------------------------------
 */
typedef struct MQABUF_Obj_tag {
    Uint16      msgSize ;
    BUF_Handle  msgList ;
} MQABUF_Obj, *MQABUF_Handle ;

/** ----------------------------------------------------------------------------
 *  @name   MQABUF_State
 *
 *  @desc   Allocator state object.
 *
 *  @field  numBufPools
 *              Number of buffer pools configured in the MQA.
 *  @field  bufPools
 *              Array of buffer pools for various message sizes.
 *  ----------------------------------------------------------------------------
 */
typedef struct MQABUF_State_tag {
    Uint16        numBufPools ;
    MQABUF_Handle bufPools ;
} MQABUF_State ;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* _MQABUF_ */
