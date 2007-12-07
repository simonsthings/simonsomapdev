/** ============================================================================
 *  @file   swiMessage.h
 *
 *  @path   $(DSPLINK)\dsp\src\samples\message
 *
 *  @desc   This is simple SWI based application that uses MSGQ interface to
 *          implement control from GPP. It receives a messages from the GPP,
 *          verifies its content and sends it back to the GPP.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

#ifndef SWIMESSAGE_
#define SWIMESSAGE_

/*  ----------------------------------- DSP/BIOS Headers            */
#include <msgq.h>

#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  SWI_DSP_MSGQNAME
 *
 *  @desc   Name of the message queue on the DSP.
 *  ============================================================================
 */
#define SWI_DSP_MSGQNAME                    "DSPLINK_DSP00MSGQ00"

/** ============================================================================
 *  @const  SWI_GPP_MSGQNAME
 *
 *  @desc   Name of the message queue on the GPP.
 *  ============================================================================
 */
#define SWI_GPP_MSGQNAME                    "DSPLINK_GPPMSGQ00"

/** ============================================================================
 *  @name   SWIMESSAGE_TransferInfo
 *
 *  @desc   Structure used to keep various information needed by various phases
 *          of the application.
 *
 *  @field  sequenceNumber
 *              Expected id from the GPP.
 *  @field  msgqHandle
 *              Handle to created message queue.
 *  @field  locatedMsgqHandle
 *              Handle to located message queue. 
 *  @field  swi
 *              SWI structure.
 *  ============================================================================
 */
typedef struct SWIMESSAGE_TransferInfo_tag {
    Uns         sequenceNumber;
    MSGQ_Handle msgqHandle;
    MSGQ_Handle locatedMsgqHandle;
    SWI_Handle swi;
} SWIMESSAGE_TransferInfo;


/** ============================================================================
 *  @func   SWIMESSAGE_create
 *
 *  @desc   Create phase function of SWIMESSAGE application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Uns SWIMESSAGE_create (SWIMESSAGE_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   SWIMESSAGE_delete
 *
 *  @desc   Delete phase function of SWIMESSAGE application.
 *
 *  @arg    transferInfo
 *              Information for transfer.
 *
 *  @ret    SYS_OK
 *              Successful operation.
 *          SYS_EBADIO
 *              Failure occured while doing IO.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ============================================================================
 */
Uns SWIMESSAGE_delete (SWIMESSAGE_TransferInfo * transferInfo);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SWIMESSAGE_ */
