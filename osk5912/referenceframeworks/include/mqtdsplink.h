/** ============================================================================
 *  @file   mqtdsplink.h
 *
 *  @path   $(DSPLINK)\dsp\inc
 *
 *  @desc   Defines the exported interface of the remote MQT for GPP-DSP
 *          communication.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#ifndef MQTDSPLINK_
#define MQTDSPLINK_


/*  ----------------------------------- DSP/BIOS Headers            */
#include <msgq.h>
#include <msgq_mqt.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <dsplink.h>


#ifdef __cplusplus
extern "C" {
#endif


/** ============================================================================
 *  @const  MQTDSPLINK_NAME
 *
 *  @desc   MQT name for the DSPLINK messaging component.
 *  ============================================================================
 */
#define MQTDSPLINK_NAME "MQTDSPLINK"

/** ============================================================================
 *  @const  DSPLINK_DSPMSGQ_NAME
 *
 *  @desc   Prefix to the names of all MSGQs created on the DSP for
 *          communication with the GPP.
 *
 *          Note: The message queues on the DSP used for inter-processor
 *                transfer through DSPLINK must be created with specific names
 *                expected by the DSPLINK MQT.
 *                The names must be of the following format:
 *                DSPLINK_DSP<PROCESSORID>MSGQ<MSGQID>
 *                <MSGQID> can have values from 00 to 'n-1' where 'n' is the
 *                maximum number of message queues on the processor.
 *                <PROCESSORID> corresponds to the processor id of the DSP
 *                (used on GPP side to reference each DSP).
 *  ============================================================================
 */
#define DSPLINK_DSPMSGQ_NAME "DSPLINK_DSP00MSGQ"

/** ============================================================================
 *  @const  DSPLINK_GPPMSGQ_NAME
 *
 *  @desc   Prefix to the names of all MSGQs created on the GPP for
 *          communication with the DSP.
 *
 *          Note: The message queues on the GPP used for inter-processor
 *                transfer through DSPLINK must be located by the DSP
 *                application with specific names expected by the DSPLINK MQT.
 *                The names must be of the following format:
 *                DSPLINK_GPPMSGQ<MSGQID>
 *                <MSGQID> can have values from 0 to 'n-1' where 'n' is the
 *                maximum number of message queues on the GPP.
 *  ============================================================================
 */
#define DSPLINK_GPPMSGQ_NAME "DSPLINK_GPPMSGQ"


/** ============================================================================
 *  @name   MQTDSPLINK_Params
 *
 *  @desc   Parameters required for initialization of the DSPLINK MQT.
 *
 *  @field  maxNumMsgq
 *              Maximum number of MSGQs that can be created on the remote
 *              processor.
 *  @field  maxMsgSize
 *              Maximum message size (in MADUs) supported by the MQT.
 *  @field  defaultMqaId
 *              The default MQA to be used by the remote MQT.
 *  ============================================================================
 */
typedef struct MQTDSPLINK_Params_tag {
    Uint16              maxNumMsgq ;
    Uint16              maxMsgSize ;
    Uint16              defaultMqaId ;
} MQTDSPLINK_Params ;


/** ============================================================================
 *  @name   MQTDSPLINK_FXNS
 *
 *  @desc   Declaration of the function table interface for the DSPLINK MQT.
 *  ============================================================================
 */
extern MSGQ_TransportFxns MQTDSPLINK_FXNS ;


/** ============================================================================
 *  @func   MQTDSPLINK_init
 *
 *  @desc   Performs global initialization of the remote MQT.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MQTDSPLINK_exit ()
 *  ============================================================================
 */
extern Void MQTDSPLINK_init () ;


/** ============================================================================
 *  @func   MQTDSPLINK_exit
 *
 *  @desc   Performs global finalization of the remote MQT.
 *
 *  @arg    None
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    MQTDSPLINK_init ()
 *  ============================================================================
 */
extern Void MQTDSPLINK_exit () ;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /* MQTDSPLINK_ */
