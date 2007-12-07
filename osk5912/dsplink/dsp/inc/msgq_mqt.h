/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== msgq_mqt.h ========
 *  This is the public include file for the MSGQ module intended to
 *  be used by transports.
 */
#ifndef MSGQ_MQT_
#define MSGQ_MQT_

#include <msgq.h>
#include <fxn.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Internal constants used by both MSGQ and the MQTs. */
#define MSGQ_INVALIDMQTID   (Uint16)-1
#define MSGQ_INVALIDMQAID   (Uint16)-1

/* The different status values for a MSGQ_Obj */ 
#define MSGQ_EMPTY    0      /* Messaging queue not being used    */
#define MSGQ_RESERVED 1      /* Messaging queue being initialized */
#define MSGQ_INUSE    2      /* Messaging queue being used        */

/* 
 *  Use the following stub function definitions if an allocator 
 *  or transport function is not implemented.
 */
extern MSGQ_Status MSGQ_notImplStatus(Void);
extern Ptr         MSGQ_notImplPtr(Void);
extern Void        MSGQ_notImplVoid(Void);

#define MSGQ_MQTOPENNOTIMPL    (MSGQ_MqtOpen)MSGQ_notImplStatus
#define MSGQ_MQTCLOSENOTIMPL   (MSGQ_MqtClose)MSGQ_notImplStatus
#define MSGQ_MQTCREATENOTIMPL  (MSGQ_MqtCreate)MSGQ_notImplStatus
#define MSGQ_MQTLOCATENOTIMPL  (MSGQ_MqtLocate)MSGQ_notImplStatus
#define MSGQ_MQTDELETENOTIMPL  (MSGQ_MqtDelete)MSGQ_notImplStatus
#define MSGQ_MQTRELEASENOTIMPL (MSGQ_MqtRelease)MSGQ_notImplStatus
#define MSGQ_MQTPUTNOTIMPL     (MSGQ_MqtPut)MSGQ_notImplStatus
#define MSGQ_MQTGETNOTIMPL     (MSGQ_MqtGet)MSGQ_notImplPtr
#define MSGQ_MQTGETREPLYHANDLENOTIMPL (MSGQ_MqtGetReplyHandle)MSGQ_notImplPtr
#define MSGQ_MQTGETBYIDNOTIMPL (MSGQ_MqtGetById)MSGQ_notImplPtr

#define MSGQ_MQAOPENNOTIMPL    (MSGQ_MqaOpen)MSGQ_notImplStatus
#define MSGQ_MQACLOSENOTIMPL   (MSGQ_MqaClose)MSGQ_notImplStatus
#define MSGQ_MQAALLOCNOTIMPL   (MSGQ_MqaAlloc)MSGQ_notImplPtr
#define MSGQ_MQAFREENOTIMPL    (MSGQ_MqaFree)FXN_F_nop

/* Function prototypes */
extern Void        MSGQ_sendErrorMsg(Uint16 errorType, Arg arg1, Arg arg2);
extern MSGQ_Status MSGQ_locateLocal(String queueName, MSGQ_Handle *msgqHandle);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MSGQ_MQT_ */

