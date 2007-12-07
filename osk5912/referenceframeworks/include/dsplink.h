/** ============================================================================
 *  @file   dsplink.h
 *
 *  @path   $(DSPLINK)\dsp\inc
 *
 *  @desc   Defines constants, data types and structures used by
 *          DSP/BIOS(TM) LINK.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef DSPLINK_
#define DSPLINK_


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  DSPLINK_DRIVER_NAME
 *
 *  @desc   DSPLINK driver name
 *  ============================================================================
 */
#define DSPLINK_DRIVER_NAME   "/dsplink"

/** ============================================================================
 *  @const  MAX_DRIVER_NAME_LEN
 *
 *  @desc   Maximum length of the driver name.
 *  ============================================================================
 */
#define MAX_DRIVER_NAME_LEN    16

/** ============================================================================
 *  @const  MAX_CHANNELS
 *
 *  @desc   Maximum channels supported by DSP/BIOS(TM) LINK within the complete
 *          system.
 *  ============================================================================
 */
#define MAX_CHANNELS    16

#if defined (_MSGQ_COMPONENT)
/** ============================================================================
 *  @const  ID_MSGCHNL_FM_GPP
 *
 *  @desc   ID of the messaging channel from the GPP.
 *  ============================================================================
 */
#define ID_MSGCHNL_FM_GPP MAX_CHANNELS

/** ============================================================================
 *  @const  ID_MSGCHNL_TO_GPP
 *
 *  @desc   ID of the messaging channel to the GPP.
 *  ============================================================================
 */
#define ID_MSGCHNL_TO_GPP (MAX_CHANNELS + 1)
#endif /* if defined (_MSGQ_COMPONENT) */

/** ============================================================================
 *  @const  BASE_SHM_CHANNELS
 *
 *  @desc   Base channel id for the SHM link driver.
 *  ============================================================================
 */
#define BASE_SHM_CHANNELS             0

/** ============================================================================
 *  @const  NUM_SHM_CHANNELS
 *
 *  @desc   Number of channels supported by the SHM link driver.
 *  ============================================================================
 */
#define NUM_SHM_CHANNELS              16

/** ============================================================================
 *  @const  BASE_HPI_CHANNELS
 *
 *  @desc   Base channel id for the hpi link driver.
 *  ============================================================================
 */
#define BASE_HPI_CHANNELS             0

/** ============================================================================
 *  @const  NUM_HPI_CHANNELS
 *
 *  @desc   Number of channels supported by the HPI link driver.
 *  ============================================================================
 */
#define NUM_HPI_CHANNELS              16


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* DSPLINK_ */
