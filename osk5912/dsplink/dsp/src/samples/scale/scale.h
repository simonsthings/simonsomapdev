/** ============================================================================
 *  @file   scale.h
 *
 *  @path   $(DSPLINK)\dsp\src\samples\scale
 *
 *  @desc   Header file for scale application.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef SCALE_
#define SCALE_

/*  ----------------------------------- DSP/BIOS Headers            */
#include <msgq.h>

#ifdef __cplusplus
extern "C" {
#endif

/** ============================================================================
 *  @const  SCALE_REQUEST_MSGID
 *
 *  @desc   Id to denote a scaling change.
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
    MSGQ_MsgHeader msgHeader;    
    Uint16         scalingFactor;
} SCALE_ScaleMsg;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SCALE_ */
