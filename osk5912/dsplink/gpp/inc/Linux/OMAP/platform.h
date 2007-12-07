/** ============================================================================
 *  @file   platform.h
 *
 *  @path   $(DSPLINK)\gpp\inc\Linux\OMAP
 *
 *  @desc   Defines platform specific attributes for the sample application.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (PLATFORM_H)
#define PLATFORM_H


USES (gpptypes.h)
USES (errbase.h)


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  DSP_MAUSIZE
 *
 *  @desc   Size of the DSP MAU (in bytes).
 *  ============================================================================
 */
#define DSP_MAUSIZE         2

/** ============================================================================
 *  @const  DSPLINK_BUF_ALIGN
 *
 *  @desc   Alignment of message buffers allocated for transfer.
 *  ============================================================================
 */
#define DSPLINK_BUF_ALIGN   2

/** ============================================================================
 *  @const  RMQT_CTRLMSG_SIZE
 *
 *  @desc   This constant defines the size (in bytes) of control messages used
 *          within the remote MQT.
 *  ============================================================================
 */
#define RMQT_CTRLMSG_SIZE   64

/** ============================================================================
 *  @const  ALIGN
 *
 *  @desc   Macro to align a number.
 *  ============================================================================
 */
#define ALIGN(x) (Uint16)(  (Uint16)(  (  (x)                 \
                                        + DSPLINK_BUF_ALIGN   \
                                        - 1)                  \
                                     / DSPLINK_BUF_ALIGN)     \
                          * DSPLINK_BUF_ALIGN)


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif /* if !defined (PLATFORM_H) */
