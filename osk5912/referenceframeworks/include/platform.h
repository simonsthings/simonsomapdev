/** ============================================================================
 *  @file   dsplink.h
 *
 *  @path   $(DSPLINK)\dsp\src\inc\OMAP
 *
 *  @desc   Platform-specific include file.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef _DSPLINK_
#define _DSPLINK_

#ifdef __cplusplus
extern "C" {
#endif

/*  ============================================================================
 *  @name  DARAM
 *
 *  @desc  External declaration for memory segment.
 *  ============================================================================
 */
extern Int DARAM ;

/** ============================================================================
 *  @const  DSPLINK_SEGID
 *
 *  @desc   Value of segment id parameter to alloc/create calls.
 *  ============================================================================
 */
#define DSPLINK_SEGID   DARAM

/** ============================================================================
 *  @const  DSPLINK_ALIGN
 *
 *  @desc   Value of Align parameter to alloc/create calls.
 *  ============================================================================
 */
#define DSPLINK_ALIGN   1

/** ============================================================================
 *  @const  DSP_MAUSIZE
 *
 *  @desc   Size of the DSP MAU (in bytes).
 *  ============================================================================
 */
#define DSP_MAUSIZE     2

#if defined (_MSGQ_COMPONENT)
/** ============================================================================
 *  @const  MQTDSPLINK_CTRLMSG_SIZE
 *
 *  @desc   Size (in MADUs) of the control messages used by the DSPLINK MQT.
 *  ============================================================================
 */
#define MQTDSPLINK_CTRLMSG_SIZE 32
#endif /* defined (_MSGQ_COMPONENT) */

/*  ============================================================================
 *  @const  ALIGN
 *
 *  @desc   Macro to align a number.
 *  ============================================================================
 */
#define ALIGN(x) (Uint16)(  (Uint16)(  (  (x)             \
                                        + DSPLINK_ALIGN   \
                                        - 1)              \
                                     / DSPLINK_ALIGN)     \
                          * DSPLINK_ALIGN)


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* _DSPLINK_ */

