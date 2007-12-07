/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== msgq.h ========
 *  This is the public include file for the MSGQ module.
 */
#ifndef MSGQ_
#define MSGQ_

#ifdef __cplusplus
extern "C" {
#endif

/* Make sure that std.h is included before msgq.h */ 
#ifndef STD_
#error std.h must be included before msgq.h
#endif

/* Needed since the following typedefs may not be in std.h yet. */ 
#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

/* Handle the 6X ISA */
#if defined(_6x_)
/* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */
typedef unsigned       Uint32;
typedef unsigned short Uint16;
typedef unsigned char  Uint8;

/* Signed integer definitions (32bit, 16bit, 8bit) follow... */
typedef int            Int32;
typedef short int      Int16;
typedef char           Int8;

/* Handle the 54 and 55 ISAs */
#elif defined(_54_) || defined(_55_)
/* Unsigned integer definitions (32bit and 16bit) follow... */
typedef unsigned long  Uint32;
typedef unsigned       Uint16;

/* Signed integer definitions (32bit and 16bit) follow... */
typedef long int       Int32;
typedef int            Int16;

/* Other ISAs not supported */
#else
#error MSGQ not supported for this ISA.
#endif  /* defined(_6x_) */

#endif  /* _TI_STD_TYPES */

/* Return type for many of the MSGQ APIs. */ 
typedef Int16   MSGQ_Status;

/* Error type for transport asynchronous errors */ 
typedef Int16   MSGQ_MqtError;  

/* Message ids ranges */
#define MSGQ_INTERNALIDSSTART  0xFE00  /* Start of internal MSGQ msg id range*/
#define MSGQ_ASYNCLOCATEMSGID  0xFE00  /* Asynchronous locate message id     */
#define MSGQ_ASYNCERRORMSGID   0xFE01  /* Asynchronous error message id      */
#define MSGQ_INTERNALIDSEND    0xFEFF  /* End of internal MSGQ msg id range  */
#define MSGQ_MQTMSGIDSSTART    0xFF00  /* Start of transport msg id range    */
#define MSGQ_MQTMSGIDSEND      0xFFFE  /* End of transport msg id range      */
#define MSGQ_INVALIDMSGQID     0xFFFF  /* Used to denote no message id value.*/

/* MSGQ_Status return codes */ 
#define MSGQ_SUCCESS                     0  /* Successful!               */
#define MSGQ_EFAILURE      (MSGQ_Status)-1  /* Generic failure           */
#define MSGQ_ENOTFOUND     (MSGQ_Status)-2  /* Resource not found        */
#define MSGQ_EMEMORY       (MSGQ_Status)-3  /* Memory allocation failure */
#define MSGQ_ETIMEOUT      (MSGQ_Status)-4  /* Timout occurred           */
#define MSGQ_ENOTIMPL      (MSGQ_Status)-5  /* Not implemented function  */

/* Error codes */ 
#define MSGQ_MQTERROREXIT  (MSGQ_MqtError)-1
#define MSGQ_MQTFAILEDPUT  (MSGQ_MqtError)-2

/* A handle is always a pointer to an object. */
typedef struct MSGQ_Obj          *MSGQ_Handle;
typedef struct MSGQ_AllocatorObj *MSGQ_AllocatorHandle;
typedef struct MSGQ_TransportObj *MSGQ_TransportHandle;

/* Typedefs for the post and pend functions. */
typedef Bool (*MSGQ_Pend)(Ptr notifyHandle, Uns timeout);
typedef Void (*MSGQ_Post)(Ptr notifyHandle);

/* The message queue attributes.  Used when creating a message queue. */
typedef struct MSGQ_Attrs {
    Ptr        notifyHandle;                           
    MSGQ_Pend  pend;
    MSGQ_Post  post;
} MSGQ_Attrs;

/* The locate attributes.  Used when locating a message queue. */
typedef struct MSGQ_LocateAttrs {    
    Uns         timeout;
    MSGQ_Handle replyHandle;
    Uint16      mqaId;
    Arg         arg;
} MSGQ_LocateAttrs;

/*
 *  The following is the message element.  This structure must be the first
 *  field of all messages passed on a message queue.
 */
typedef struct MSGQ_MsgHeader { 
    Uint32 reserved[2];         /* Transport specific                       */
    Uint16 mqtId;               /* Id of the MQT that send the message      */
    Uint16 mqaId;               /* Id of the allocator that alloc. the msg  */
    Uint16 size;                /* Size of the allocated msg                */
    Uint16 dstId;               /* Destinaton message queue id              */
    Uint16 srcId;               /* Source message queue id                  */
    Uint16 msgId;               /* User specified message id                */
} MSGQ_MsgHeader, *MSGQ_Msg;

/* Asynchronous locate message */
typedef struct MSGQ_AsyncLocateMsg {
    MSGQ_MsgHeader  header;
    MSGQ_Handle     msgqHandle;
    Arg             arg;
} MSGQ_AsyncLocateMsg;

/* Asynchronous error message */
typedef struct MSGQ_AsyncErrorMsg {
    MSGQ_MsgHeader  header;
    MSGQ_MqtError   errorType;
    Arg             arg1;
    Arg             arg2;
} MSGQ_AsyncErrorMsg;

/* Typedefs for the transport interface functions. */
typedef MSGQ_Status (*MSGQ_MqtOpen)(MSGQ_TransportHandle  mqtHandle,
                                    Uint16                numLocalMsgQueues,
                                    MSGQ_Handle           localMsgQueues);
typedef MSGQ_Status (*MSGQ_MqtClose)(MSGQ_TransportHandle  mqtHandle);
typedef MSGQ_Status (*MSGQ_MqtCreate)(MSGQ_TransportHandle  mqtHandle,
                                      Ptr                  *mqtRepository,
                                      MSGQ_Attrs           *attrs);
typedef MSGQ_Status (*MSGQ_MqtLocate)(MSGQ_TransportHandle  mqtHandle,
                                      String                queueName,
                                      MSGQ_Handle          *msgqHandle,
                                      MSGQ_LocateAttrs     *locateAttrs);
typedef MSGQ_Status (*MSGQ_MqtDelete)(MSGQ_Handle msgqHandle);
typedef MSGQ_Status (*MSGQ_MqtRelease)(MSGQ_Handle msgqHandle);
typedef MSGQ_Msg    (*MSGQ_MqtGet)(MSGQ_Handle msgqHandle, Uns timeout);
typedef MSGQ_Status (*MSGQ_MqtPut)(MSGQ_Handle msgqHandle, MSGQ_Msg msg);
typedef MSGQ_Handle (*MSGQ_MqtGetReplyHandle)(MSGQ_TransportHandle  mqtHandle,
                                              MSGQ_Msg              msg);
typedef MSGQ_Msg    (*MSGQ_MqtGetById)(MSGQ_Handle msgqHandle,
                                       Uint16      msgIds[],
                                       Uint16      numIds,
                                       Uns         timeout);

/* The transport interface function definition. */
typedef struct MSGQ_TransportFxns { 
    MSGQ_MqtOpen           mqtOpen;
    MSGQ_MqtClose          mqtClose;
    MSGQ_MqtCreate         mqtCreate;
    MSGQ_MqtLocate         mqtLocate;
    MSGQ_MqtDelete         mqtDelete;
    MSGQ_MqtRelease        mqtRelease;
    MSGQ_MqtGet            mqtGet;
    MSGQ_MqtPut            mqtPut;
    MSGQ_MqtGetReplyHandle mqtGetReplyHandle;   
    MSGQ_MqtGetById        mqtGetById;   
} MSGQ_TransportFxns;

/* Typedefs for the allocator interface functions */
typedef MSGQ_Status (*MSGQ_MqaOpen)(MSGQ_AllocatorHandle  mqaHandle);
typedef MSGQ_Status (*MSGQ_MqaClose)(MSGQ_AllocatorHandle mqaHandle);
typedef MSGQ_Msg    (*MSGQ_MqaAlloc)(MSGQ_AllocatorHandle mqaHandle, 
                                     Uint16 *size);
typedef Void        (*MSGQ_MqaFree)(MSGQ_AllocatorHandle mqaHandle,
                                    MSGQ_Msg             msg,
                                    Uint16               size);

/* The allocator interface function definition. */
typedef struct MSGQ_AllocatorFxns {
    MSGQ_MqaOpen        mqaOpen;
    MSGQ_MqaClose       mqaClose;
    MSGQ_MqaAlloc       mqaAlloc;
    MSGQ_MqaFree        mqaFree;
} MSGQ_AllocatorFxns;

/* The transport object */
typedef struct MSGQ_TransportObj {
    String              mqtName;    /* Name of the transport         */
    MSGQ_TransportFxns *mqtFxns;    /* Transport interface functions */
    Ptr                 mqtParams;  /* Transport setup parameters    */
    Ptr                 mqtInfo;    /* Transport state information   */
    Uint16              mqtId;      /* Index into transport table    */
} MSGQ_TransportObj;

/* The allocator object */
typedef struct MSGQ_AllocatorObj {
    String              mqaName;    /* Name of the allocator         */
    MSGQ_AllocatorFxns *mqaFxns;    /* Allocator interface functions */
    Ptr                 mqaParams;  /* Allocator setup parameters    */
    Ptr                 mqaInfo;    /* Allocator state information   */
    Uint16              mqaId;      /* Index into allocator table    */
} MSGQ_AllocatorObj;

/* The following is the message queue object. */
typedef struct MSGQ_Obj {
    String               msgqName;
    MSGQ_TransportHandle mqtHandle;
    Ptr                  mqtRepository;
    MSGQ_MqtGet          mqtGet;
    MSGQ_MqtPut          mqtPut;
    Ptr                  notifyHandle;
    MSGQ_Pend            pend;
    MSGQ_Post            post;
    Uint16               msgqId;
    Uint16               status;
} MSGQ_Obj;

/* Configuration structure */
typedef struct MSGQ_Config {
    MSGQ_Obj            *msgQueues;        /* Array of local message queues  */
    MSGQ_AllocatorObj   *allocators;       /* Array of allocators            */
    MSGQ_TransportObj   *transports;       /* Array of transports            */
    Uint16               numMsgQueues;     /* Number of local message queues */
    Uint16               numAllocators;    /* Number of allocators           */
    Uint16               numTransports;    /* Number of transports           */    
    Uint16               localTransportId; /* Id of the local transport      */
} MSGQ_Config;

/* Must be supplied by application */
extern MSGQ_Config *MSGQ;

/* Default creation and location attributes */
extern const MSGQ_Attrs       MSGQ_ATTRS;
extern const MSGQ_LocateAttrs MSGQ_LOCATEATTRS;

/* Inlines used to extract data from a message */
static inline Uint16 MSGQ_getMsgId(MSGQ_Msg msg) { return(msg->msgId); }
static inline Uint16 MSGQ_getMsgSize(MSGQ_Msg msg) { return(msg->size); }

/* Inline used get a message */
static inline MSGQ_Msg MSGQ_get(MSGQ_Handle msgqHandle, Uns timeout)
{
    /* assert(msgqHandle != NULL); */

    /* Simply call the transport associated with this message queue */
    return (msgqHandle->mqtGet(msgqHandle, timeout));
}

/* Function prototypes */
extern Void        MSGQ_init(Void);
extern Void        MSGQ_exit(Void);
extern MSGQ_Handle MSGQ_create(String queueName, MSGQ_Attrs  *attrs);
extern MSGQ_Status MSGQ_locate(String             queueName, 
                               MSGQ_Handle       *msgqHandle, 
                               MSGQ_LocateAttrs  *locateAttrs);
extern MSGQ_Status MSGQ_delete(MSGQ_Handle msgqHandle);
extern MSGQ_Status MSGQ_release(MSGQ_Handle msgqHandle);
extern MSGQ_Msg    MSGQ_alloc(Uint16 mqaId, Uint16 size);
extern Void        MSGQ_free(MSGQ_Msg msg);
extern MSGQ_Status MSGQ_put(MSGQ_Handle msgqHandle, 
                            MSGQ_Msg    msg,
                            Uint16      msgId, 
                            MSGQ_Handle srcHandle);
extern MSGQ_Msg    MSGQ_get(MSGQ_Handle msgqHandle, Uns timeout);
extern MSGQ_Handle MSGQ_getReplyHandle(MSGQ_Msg msg);
extern Void        MSGQ_setErrorHandler(MSGQ_Handle errorHandler,
                                        Uint16      mqaId);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MSGQ_ */

