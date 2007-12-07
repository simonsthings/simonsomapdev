/** ============================================================================
 *  @file   drv_pmgr.h
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr\Linux
 *
 *  @desc   Linux module driver interface file.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (DRV_PMGR_H)
#define DRV_PMGR_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)
USES (errbase.h)
USES (drv_api.h)


/*  ============================================================================
 *  @const  BASE_CMD
 *
 *  @desc   Base command id to be used for linux ioctl's.
 *  ============================================================================
 */
#define BASE_CMD                           0x6B00

/*  ============================================================================
 *  @macro  CMD_PROC_XXXX
 *
 *  @desc   Command ids for PROC functions.
 *  ============================================================================
 */
#define PROC_BASE_CMD                      (BASE_CMD + 0x100)
#define CMD_PROC_SETUP                     (PROC_BASE_CMD + 1)
#define CMD_PROC_DESTROY                   (PROC_BASE_CMD + 2)
#define CMD_PROC_START                     (PROC_BASE_CMD + 3)
#define CMD_PROC_STOP                      (PROC_BASE_CMD + 4)
#define CMD_PROC_LOAD                      (PROC_BASE_CMD + 5)
#define CMD_PROC_LOADSECTION               (PROC_BASE_CMD + 6)
#define CMD_PROC_DETACH                    (PROC_BASE_CMD + 7)
#define CMD_PROC_ATTACH                    (PROC_BASE_CMD + 8)
#define CMD_PROC_GETSTATE                  (PROC_BASE_CMD + 9)
#define CMD_PROC_CONTROL                   (PROC_BASE_CMD + 10)

#if defined (DDSP_PROFILE)
#define CMD_PROC_INSTRUMENT                (PROC_BASE_CMD + 11)
#endif

#if defined (DDSP_DEBUG)
#define CMD_PROC_DEBUG                     (PROC_BASE_CMD + 12)
#endif


#if defined (CHNL_COMPONENT)
/*  ============================================================================
 *  @macro  CMD_CHNL_XXXX
 *
 *  @desc   Command ids for CHNL functions.
 *  ============================================================================
 */
#define CHNL_BASE_CMD                      (BASE_CMD + 0x200)
#define CMD_CHNL_CREATE                    (CHNL_BASE_CMD + 1)
#define CMD_CHNL_DELETE                    (CHNL_BASE_CMD + 2)
#define CMD_CHNL_ISSUE                     (CHNL_BASE_CMD + 3)
#define CMD_CHNL_RECLAIM                   (CHNL_BASE_CMD + 4)
#define CMD_CHNL_ALLOCATEBUFFER            (CHNL_BASE_CMD + 5)
#define CMD_CHNL_FREEBUFFER                (CHNL_BASE_CMD + 6)
#define CMD_CHNL_IDLE                      (CHNL_BASE_CMD + 7)
#define CMD_CHNL_FLUSH                     (CHNL_BASE_CMD + 8)
#define CMD_CHNL_CONTROL                   (CHNL_BASE_CMD + 9)


#if defined (DDSP_PROFILE)
#define CMD_CHNL_INSTRUMENT                (CHNL_BASE_CMD + 10)
#endif /* if defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
#define CMD_CHNL_DEBUG                     (CHNL_BASE_CMD + 11)
#endif /* if defined (DDSP_DEBUG) */
#endif /* if defined (CHNL_COMPONENT) */


/*  ============================================================================
 *  @macro  CMD_DRV_XXXX
 *
 *  @desc   Command ids for DRV specific calls.
 *  ============================================================================
 */
#define DRV_BASE_CMD                       (BASE_CMD + 0x300)
#define CMD_DRV_GETMAPTABLE_ADDRESS        (DRV_BASE_CMD + 1)

#if defined (MSGQ_COMPONENT)
#define CMD_DRV_ADDUSER_ADDRESS        (DRV_BASE_CMD + 2)
#endif /* if defined (MSGQ_COMPONENT) */


#if defined (MSGQ_COMPONENT)
/*  ============================================================================
 *  @macro  CMD_MSGQ_XXXX
 *
 *  @desc   Command ids for CHNL functions.
 *  ============================================================================
 */
#define MSGQ_BASE_CMD                      (BASE_CMD + 0x400)
#define CMD_MSGQ_ALLOCATOROPEN             (MSGQ_BASE_CMD + 1)
#define CMD_MSGQ_ALLOCATORCLOSE            (MSGQ_BASE_CMD + 2)
#define CMD_MSGQ_TRANSPORTOPEN             (MSGQ_BASE_CMD + 3)
#define CMD_MSGQ_TRANSPORTCLOSE            (MSGQ_BASE_CMD + 4)
#define CMD_MSGQ_CREATE                    (MSGQ_BASE_CMD + 5)
#define CMD_MSGQ_DELETE                    (MSGQ_BASE_CMD + 6)
#define CMD_MSGQ_LOCATE                    (MSGQ_BASE_CMD + 7)
#define CMD_MSGQ_RELEASE                   (MSGQ_BASE_CMD + 8)
#define CMD_MSGQ_ALLOC                     (MSGQ_BASE_CMD + 9)
#define CMD_MSGQ_FREE                      (MSGQ_BASE_CMD + 10)
#define CMD_MSGQ_PUT                       (MSGQ_BASE_CMD + 11)
#define CMD_MSGQ_GET                       (MSGQ_BASE_CMD + 12)
#define CMD_MSGQ_GETREPLYID                (MSGQ_BASE_CMD + 13)
#define CMD_MSGQ_SETERRORHANDLER           (MSGQ_BASE_CMD + 14)


#if defined (DDSP_PROFILE)
#define CMD_MSGQ_INSTRUMENT                (MSGQ_BASE_CMD + 15)
#endif /* if defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
#define CMD_MSGQ_DEBUG                     (MSGQ_BASE_CMD + 16)
#endif /* if defined (DDSP_DEBUG) */


#endif /* if defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @macro  DRV_INITIALIZE
 *
 *  @desc   OS dependent definition of initialization function.
 *  ============================================================================
 */
#define DRV_INITIALIZE(handle)         DRV_Initialize (handle, NULL)

/** ============================================================================
 *  @macro  DRV_FINALIZE
 *
 *  @desc   OS dependent definition of finalization function.
 *  ============================================================================
 */
#define DRV_FINALIZE(handle)           DRV_Finalize (handle, NULL)

/** ============================================================================
 *  @macro  DRV_INVOKE
 *
 *  @desc   OS dependent definition to call functions through ioctl's.
 *  ============================================================================
 */
#define DRV_INVOKE(handle,cmd,arg1)    DRV_Invoke (handle, cmd, arg1, NULL)


/** ============================================================================
 *  @name   CMD_Args
 *
 *  @desc   Union defining arguments to be passed to ioctl calls. For the
 *          explanation of individual field please see the corresponding APIs.

 *  @field  apiStatus
 *              Status returned by this API.
 *          apiArgs
 *              Union representing arguments for different APIs.
 *  ============================================================================
 */
typedef struct CMD_Args_tag {
    DSP_STATUS apiStatus ;
    union {
        struct {
        } procSetupArgs ;

        struct {
        } procDestroyArgs ;

        struct {
            ProcessorId     procId ;
        } procStartArgs ;

        struct {
            ProcessorId     procId ;
        } procStopArgs ;

        struct {
            ProcessorId     procId ;
            ProcAttr *      attr   ;
        } procAttachArgs ;

        struct {
            ProcessorId    procId ;
        } procDetachArgs ;

        struct {
            ProcessorId    procId    ;
            Char8 *        imagePath ;
            Uint32         argc      ;
            Char8 **       argv      ;
        } procLoadArgs ;

        struct {
            ProcessorId     procId    ;
            ProcState *     procState ;
        } procGetStateArgs ;

        struct {
            ProcessorId     procId ;
            Int32           cmd    ;
            Pvoid           arg    ;
        } procControlArgs ;

        struct {
            ProcessorId     procId    ;
            Char8 *         imagePath ;
            Uint32          sectID    ;
        } procLoadSectionArgs ;

#if defined (DDSP_PROFILE)
        struct {
            ProcessorId       procId    ;
            ProcInstrument *  procStats ;
        } procInstrumentArgs ;
#endif

#if defined (DDSP_DEBUG)
        struct {
            ProcessorId       procId ;
        } procDebugArgs ;
#endif

#if defined (CHNL_COMPONENT)
        struct {
            ProcessorId     procId ;
            ChannelId       chnlId ;
            ChannelAttrs *  attrs  ;
        } chnlCreateArgs ;

        struct {
            ProcessorId     procId ;
            ChannelId       chnlId ;
        } chnlDeleteArgs ;

        struct {
            ProcessorId     procId ;
            ChannelId       chnlId ;
            ChannelIOInfo * ioReq  ;
        } chnlIssueArgs ;

        struct {
            ProcessorId     procId  ;
            ChannelId       chnlId  ;
            Uint32          timeout ;
            ChannelIOInfo * ioReq   ;
        } chnlReclaimArgs ;

        struct {
            ProcessorId     procId   ;
            ChannelId       chnlId   ;
            Char8 **        bufArray ;
            Uint32          size     ;
            Uint32          numBufs  ;
            Uint32          phyAddr  ;
        } chnlAllocateBufferArgs ;

        struct {
            ProcessorId     procId   ;
            ChannelId       chnlId   ;
            Char8 **        bufArray ;
            Uint32          numBufs  ;
        } chnlFreeBufferArgs ;

        struct {
            ProcessorId     procId ;
            ChannelId       chnlId ;
        } chnlFlushArgs ;

        struct {
            ProcessorId     procId ;
            ChannelId       chnlId ;
        } chnlIdleArgs ;

        struct {
            ProcessorId     procId ;
            ChannelId       chnlId ;
            Int32           cmd    ;
            Pvoid           arg    ;
        } chnlControlArgs ;

#if defined (DDSP_PROFILE)
        struct {
            ProcessorId       procId    ;
            ChannelId         chnlId    ;
            ChnlInstrument *  chnlStats ;
        } chnlInstrumentArgs ;
#endif

#if defined (DDSP_DEBUG)
        struct {
            ProcessorId       procId ;
            ChannelId         chnlId ;
        } chnlDebugArgs ;
#endif
#endif /* if defined (CHNL_COMPONENT) */

        struct {
            Void * phyAddr ;
        } drvPhyAddrArgs ;

#if defined (MSGQ_COMPONENT)
        struct {
            AllocatorId mqaId    ;
            Pvoid       mqaAttrs ;
            Uint32      phyAddr  ;
            Uint32      size     ;
        } msgqAllocatorOpenArgs ;

        struct {
            AllocatorId mqaId ;
        } msgqAllocatorCloseArgs ;

        struct {
            TransportId mqtId    ;
            Pvoid       mqtAttrs ;
        } msgqTransportOpenArgs ;

        struct {
            TransportId mqtId ;
        } msgqTransportCloseArgs ;

        struct {
            MsgQueueId  msgqId ;
            MsgqAttrs * msgqAttrs ;
        } msgqCreateArgs ;

        struct {
            MsgQueueId msgqId ;
        } msgqDeleteArgs ;

        struct {
            ProcessorId       procId ;
            MsgQueueId        msgqId ;
            MsgqLocateAttrs * attrs  ;
        } msgqLocateArgs ;

        struct {
            ProcessorId procId ;
            MsgQueueId  msgqId ;
        } msgqReleaseArgs ;

        struct {
            AllocatorId mqaId ;
            Uint16      size  ;
            MsgqMsg *   msg   ;
        } msgqAllocArgs ;

        struct {
            MsgqMsg     msg   ;
            AllocatorId mqaId ;
        } msgqFreeArgs ;

        struct {
            ProcessorId procId     ;
            MsgQueueId  destMsgqId ;
            MsgqMsg     msg        ;
            Uint16      msgId      ;
            MsgQueueId  srcMsgqId  ;
            AllocatorId mqaId ;
        } msgqPutArgs ;

        struct {
            MsgQueueId  msgqId  ;
            Uint32      timeout ;
            MsgqMsg  *  msg     ;
        } msgqGetArgs ;

        struct {
            MsgqMsg       msg    ;
            ProcessorId * procId ;
            MsgQueueId *  msgqId ;
            AllocatorId   mqaId ;
        } msgqGetReplyIdArgs ;

        struct {
            MsgQueueId msgqId ;
            Uint16     mqaId  ;
        } msgqSetErrorHandlerArgs ;

        struct {
            Void *        usrAddr ;
            AllocatorId   mqaId   ;
        } drvUsrAddrArgs ;

#if defined (DDSP_PROFILE)
        struct {
            ProcessorId      procId ;
            MsgQueueId       msgqId ;
            MsgqInstrument * retVal ;
        } msgqInstrumentArgs ;
#endif /* if defined (DDSP_PROFILE) */

#if defined (DDSP_DEBUG)
        struct {
            ProcessorId procId ;
            MsgQueueId  msgqId ;
        } msgqDebugArgs ;
#endif /* if defined (DDSP_DEBUG) */
#endif /* if defined (MSGQ_COMPONENT) */

    } apiArgs ;
} CMD_Args ;

/** ============================================================================
 *  @name   DrvAddrMapEntry
 *
 *  @desc   Structure for the entry of user-kernel address mapping table.
 *
 *  @field  userAddress
 *              User side address of the buffer.
 *  @field  kernelAddress
 *              kernel side address of the buffer.
 *  @field  physicalAddress
 *              Physical address of the buffer.
 *  @field  length
 *              Total length of the mapped memory chunk.
 *  @field  valid
 *              Tells if the entry is valid or not.
 *  ============================================================================
 */
typedef struct DrvAddrMapEntry_tag {
    Void * userAddress     ;
    Void * kernelAddress   ;
    Void * physicalAddress ;
    Uint32 length          ;
    Bool   valid           ;
} DrvAddrMapEntry ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !define (DRV_PMGR_H) */
