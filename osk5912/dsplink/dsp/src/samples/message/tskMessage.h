/** ============================================================================
 *  @file   tskMessage.h
 *
 *  @path   $(DSPLINK)\dsp\src\samples\message
 *
 *  @desc   This is simple TSK based application that uses MSGQ interface to
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

#ifndef TSKMESSAGE_
#define TSKMESSAGE_

/*  ----------------------------------- DSP/BIOS Headers            */
#include <msgq.h>
#include <sem.h>

#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  TSK_DSP_MSGQNAME
 *
 *  @desc   Name of the message queue on the DSP.
 *  ============================================================================
 */
#define TSK_DSP_MSGQNAME                    "DSPLINK_DSP00MSGQ00"

 /** ============================================================================
 *  @const  TSK_GPP_MSGQNAME
 *
 *  @desc   Name of the message queue on the GPP.
 *  ============================================================================
 */
#define TSK_GPP_MSGQNAME                    "DSPLINK_GPPMSGQ00"

/** ============================================================================
 *  @name   TSKMESSAGE_TransferInfo
 *
 *  @desc   Structure used to keep various information needed by various phases
 *          of the application.
 *
 *  @field  sequenceNumber
 *              Expected id from the GPP.
 *  @field  msgqHandle
 *              Handle of created message queue.
 *  @field  notifySemObj
 *              Semaphore used for message notification.
 *  @field  locatedMsgqHandle
 *              Handle to located message queue. 
 *  ============================================================================
 */
typedef struct TSKMESSAGE_TransferInfo_tag {
    Uns         sequenceNumber;
    MSGQ_Handle msgqHandle;
    SEM_Obj     notifySemObj;
    MSGQ_Handle locatedMsgqHandle;
} TSKMESSAGE_TransferInfo;


/** ============================================================================
 *  @func   TSKMESSAGE_create
 *
 *  @desc   Create phase function of TSKMESSAGE application.
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
Uns TSKMESSAGE_create (TSKMESSAGE_TransferInfo ** transferInfo);

/** ============================================================================
 *  @func   TSKMESSAGE_execute
 *
 *  @desc   Excecute phase function of TSKMESSAGE application.
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
Uns TSKMESSAGE_execute (TSKMESSAGE_TransferInfo * transferInfo);

/** ============================================================================
 *  @func   TSKMESSAGE_delete
 *
 *  @desc   Delete phase function of TSKMESSAGE application.
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
Uns TSKMESSAGE_delete (TSKMESSAGE_TransferInfo * transferInfo);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* TSKMESSAGE_ */
