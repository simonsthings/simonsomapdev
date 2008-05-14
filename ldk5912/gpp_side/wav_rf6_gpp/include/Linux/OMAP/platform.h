/** ============================================================================
 *  @file   platform.h
 *
 *  @path   $(DSPLINK)/gpp/inc/Linux/OMAP/
 *
 *  @desc   Defines platform specific attributes for the sample application.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
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

#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @const  PCPYMQT_CTRLMSG_SIZE
 *
 *  @desc   This constant defines the size (in bytes) of control messages used
 *          within the PCPY MQT.
 *  ============================================================================
 */
#define PCPYMQT_CTRLMSG_SIZE   128

/** ============================================================================
 *  @const  ZCPYMQT_CTRLMSG_SIZE
 *
 *  @desc   This constant defines the size (in bytes) of control messages used
 *          within the ZCPY MQT.
 *  ============================================================================
 */
#define ZCPYMQT_CTRLMSG_SIZE   128
#endif /* if defined (MSGQ_COMPONENT) */

/** ============================================================================
 *  @const  ALIGN
 *
 *  @desc   Macro to align a number.
 *          x: The number to be aligned
 *          y: The value that the number should be aligned to.
 *  ============================================================================
 */
#define ALIGN(x, y) (Uint32)((Uint32)((x + y - 1) / y) * y)

/** ============================================================================
 *  @const  RTC_LOAD_VALUE
 *
 *  @desc   Gives the value to be loaded into the RTC register.
 *  ============================================================================
 */
#define RTC_LOAD_VALUE 0xFFFFFFFF

/** ============================================================================
 *  @const  RTC_COUNTER_TYPE
 *
 *  @desc   Indicates the type of RTC counter (Up/Down counter)
 *          0 --> Up-counter
 *          1 --> Down-counter
 *  ============================================================================
 */
#define RTC_COUNTER_TYPE 1

/** ============================================================================
 *  @macro  RTC_REG_VALUE
 *
 *  @desc   Gives the value of a 32-bit register.
 *  ============================================================================
 */
#define RTC_REG_VALUE(x) *((volatile Uint32 *) (x))

/** ============================================================================
 *  @const  MAX_LDRV_CHANNELS
 *
 *  @desc   Maximum number of channels used by the link driver.
 *  ============================================================================
 */
#define MAX_LDRV_CHANNELS    MAX_CHANNELS


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */

#endif /* if !defined (PLATFORM_H) */
