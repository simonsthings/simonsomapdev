/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appMsgqConfig.c ========
 *
 *  This file contains the static configuration of the MSGQ module.
 *  The configuration is a superset of the requirements for each 
 *  example.
 */

#include <std.h>
#include <msgq.h>
#include <mqabuf.h>
#include <mqtbiosque.h>
#include <platform.h>
#include <mqtdsplink.h>

#include "appMsgqConfig.h"
#include "appThreads.h"

static const Uint16 localMsgSizes[NUMLOCALMSGPOOLS] = 
    {sizeof(LocalDataMsg)};          /* Size of local data messages */

static const Uint16 remoteMsgSizes[NUMREMOTEMSGPOOLS] = 
    {sizeof(CtrlMsg),               /* Size of control messages */
     MQTDSPLINK_CTRLMSG_SIZE};      /* Required by dsplink transport */
          
static const Uint16 numLocalMsgs[NUMLOCALMSGPOOLS] = {NUMDATAMSGS};
static const Uint16 numRemoteMsgs[NUMREMOTEMSGPOOLS] = {NUMCTRLMSGS, 
                                                            NUMMQTCTRLMSGS};
                                            
static const MQABUF_Params localMqaBufParams = 
    {NUMLOCALMSGPOOLS,           /* Number of buffer pools       */
     (Uint16 *)localMsgSizes,    /* Msg sizes for each pool      */
     (Uint16 *)numLocalMsgs,     /* Number of msgs for each pool */
     0};                         /* Segment for DSP/BIOS objects */
static const MQABUF_Params remoteMqaBufParams = 
    {NUMREMOTEMSGPOOLS,          /* Number of buffer pools       */
     (Uint16 *)remoteMsgSizes,   /* Msg sizes for each pool      */
     (Uint16 *)numRemoteMsgs,    /* Number of msgs for each pool */
     0};                         /* Segment for DSP/BIOS objects */
              
static const MSGQ_AllocatorObj allocators[NUMALLOCATORS] = 
    {{"REMOTEMQABUF",                       /* Name of the allocator         */
     (MSGQ_AllocatorFxns *)&MQABUF_FXNS,    /* Allocator interface functions */
     (MQABUF_Params *)&remoteMqaBufParams,  /* Allocator configuration       */
     NULL,                                  /* Filled in by allocator        */
     REMOTEMQABUFID},                       /* Allocator Id                  */
     {"LOCALMQABUF",                        /* Name of the allocator         */
     (MSGQ_AllocatorFxns *)&MQABUF_FXNS,    /* Allocator interface functions */
     (MQABUF_Params *)&localMqaBufParams,   /* Allocator configuration       */
     NULL,                                  /* Filled in by allocator        */
     LOCALMQABUFID}                         /* Allocator Id                  */
    };  

/* 
 *  Remote transport configuration
 */
static const MQTDSPLINK_Params mqtDspLinkParams = 
    {MAXNUMREMOTEMSGQS,                     /* max remote MSGQs */
     MQTDSPLINK_CTRLMSG_SIZE,               /* max message size */
     REMOTEMQABUFID};                       /* default MQA ID   */
                  
/* 
 *  Transports static Configuration 
 */
static const MSGQ_TransportObj transport[NUMTRANSPORTS] = 
    {{"LOCALMQTBIOSQUE",                      /* Name of the transport       */
     (MSGQ_TransportFxns *)&MQTBIOSQUE_FXNS,  /* Transport interface fxns    */
     NULL,                                    /* Transport configuration     */
     NULL,                                    /* Filled in by transport      */
     LOCALMQTBIOSQUEID},                      /* Transport Id                */
     {"REMOTEMQT",                            /* Name of the transport       */
     (MSGQ_TransportFxns *)&MQTDSPLINK_FXNS,  /* Transport interface fxns    */
     (MQTDSPLINK_Params *)&mqtDspLinkParams,  /* Transport configuration     */
     NULL,                                    /* Filled in by transport      */
     REMOTEMQTID},                            /* Transport Id                */
     };                   

/* 
 *  Local Message Queue array
 */
static const MSGQ_Obj msgQueues[NUMMSGQUEUES];

/* 
 *  The Static configuration of the MSGQ module. MSGQ is a required
 *  global variable.
 */
static const MSGQ_Config msgqConfig = {
    (MSGQ_Obj *)msgQueues, 
    (MSGQ_AllocatorObj *)allocators, 
    (MSGQ_TransportObj *)transport,
    NUMMSGQUEUES, NUMALLOCATORS, NUMTRANSPORTS,
    LOCALMQTBIOSQUEID};  /* local transport */
MSGQ_Config *MSGQ = (MSGQ_Config *)&msgqConfig;

