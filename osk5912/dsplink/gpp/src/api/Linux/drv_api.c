/** ============================================================================
 *  @file   drv_api.c
 *
 *  @path   $(DSPLINK)\gpp\src\api\Linux
 *
 *  @desc   User side driver wrapper that does proper ioctl calls for each
 *          API.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers         */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ----------------------------------- OSAL Headers                */
#include <print.h>
#include <drv_pmgr.h>
#include <drv_api.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Identifier for sub-component.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_OSAL_DRIVER

/** ============================================================================
 *  @macro  LINK_DRIVER_NAME
 *
 *  @desc   Name of the link driver.
 *  ============================================================================
 */
#define  LINK_DRIVER_NAME   "/dev/dsplink"

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON  \
            TRC_3PRINT (TRC_LEVEL7, \
                        "\nFailure: Status:[0x%x] File:[0x%x] Line:[%d]\n", \
                        status, FID_C_OSAL_DRV_API, __LINE__)


/** ============================================================================
 *  @name   DRV_Object_tag
 *
 *  @desc   OS specific definition of the driver object.
 *
 *  @field  signature
 *              Signature of the object.
 *  @field  driverHandle
 *              OS driver handle.
 *  @field  refCount
 *              Reference count for the driver object.
 *  ============================================================================
 */
struct DRV_Object_tag {
    Uint32   signature    ;
    Int32    driverHandle ;
    Uint32   refCount     ;
} ;


/** ============================================================================
 *  @name   DRV_Handle
 *
 *  @desc   Handle to Linux Driver object.
 *  ============================================================================
 */
EXTERN DRV_Object *  DRV_Handle ;


#if defined (CHNL_COMPONENT)
/** ============================================================================
 *  @name   mapTable
 *
 *  @desc   Pointer to a table that contain information about the user
 *          and kernel buffer mappings. This table physically exists on
 *          the kernel side.
 *  ============================================================================
 */
STATIC DrvAddrMapEntry (*mapTable) [MAX_CHANNELS] = NULL ;
#endif /* if defined (CHNL_COMPONENT) */


#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   DRV_MsgqBufPoolEntry
 *
 *  @desc   This contains information about the user and kernel buffer mappings
 *          of the BUF MQA Pool.
 *  ============================================================================
 */
STATIC DrvAddrMapEntry DRV_MsgqBufPoolEntry [MAX_ALLOCATORS] ;
#endif /* if defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @name   DRV_Initialize
 *
 *  @desc   Initialization function for the driver.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DRV_Initialize (OUT DRV_Object ** drvObj, OPT IN OUT Pvoid arg)
{
    DSP_STATUS status       = DSP_SOK ;
    Int32      osStatus               ;

#if defined (CHNL_COMPONENT)
    Void *     physicalAddr = NULL    ;
    size_t     length                 ;
    CMD_Args   cmdArgs                ;
#endif
#if defined (MSGQ_COMPONENT)
    Uint32 i ;
#endif /* if defined (MSGQ_COMPONENT) */

    TRC_2ENTER ("DRV_Initialize", drvObj, arg) ;

    DBC_Require (drvObj != NULL) ;

    if (drvObj == NULL) {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }
    else if (*drvObj != NULL) {
        (*drvObj)->refCount++ ;
    }
    else {
        *drvObj = (DRV_Object *) malloc (sizeof (DRV_Object)) ;

        if (*drvObj != NULL) {
            (*drvObj)->driverHandle = open (LINK_DRIVER_NAME, O_SYNC | O_RDWR) ;

            if ((*drvObj)->driverHandle < 0) {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
            else {
                osStatus =
                    fcntl ((*drvObj)->driverHandle, F_SETFD, FD_CLOEXEC) ;
                if (osStatus != 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }

#if defined (CHNL_COMPONENT)
            if (DSP_SUCCEEDED (status)) {
                osStatus = ioctl ((*drvObj)->driverHandle,
                                  CMD_DRV_GETMAPTABLE_ADDRESS,
                                  &cmdArgs) ;

                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
                else {
                    physicalAddr = cmdArgs.apiArgs.drvPhyAddrArgs.phyAddr ;
                    length =    sizeof (DrvAddrMapEntry)
                              * MAX_CHANNELS
                              * MAX_PROCESSORS ;

                    mapTable = mmap (NULL,
                                     length,
                                     PROT_READ | PROT_WRITE,
                                     MAP_SHARED,
                                     (*drvObj)->driverHandle,
                                     (off_t) physicalAddr) ;

                    if (mapTable == (Void *)-1) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }
                }
                if (DSP_FAILED (status)) {
                    close ((*drvObj)->driverHandle) ;
                }
            }
#endif /* if defined (CHNL_COMPONENT) */

#if defined (MSGQ_COMPONENT)
            /*  ----------------------------------------------------------------
             *  For all allocators, set the valid bit of DRV_MsgqBufPoolEntry
             *  structure to FALSE.
             *  ----------------------------------------------------------------
             */
            for (i = 0 ; i < MAX_ALLOCATORS ; i++) {
                DRV_MsgqBufPoolEntry [i].valid = FALSE ;
            }
#endif /* if defined (MSGQ_COMPONENT) */

            if (DSP_SUCCEEDED (status)) {
                (*drvObj)->signature = SIGN_DRV ;
                (*drvObj)->refCount = 1 ;
            }
            else {
                (*drvObj)->signature = SIGN_NULL ;
                free (*drvObj) ;
                *drvObj = NULL ;
            }
        }
        else {
            status = DSP_EMEMORY ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("DRV_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @name   DRV_Finalize
 *
 *  @desc   Finalize function for the driver.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DRV_Finalize (IN DRV_Object * drvObj, OPT IN OUT Pvoid arg)
{
    DSP_STATUS status = DSP_SOK ;
    int        osStatus         ;
#if defined (CHNL_COMPONENT)
    Uint32     length           ;
#endif /* if defined (CHNL_COMPONENT) */

    TRC_2ENTER ("DRV_Finalize", drvObj, arg) ;

    DBC_Require (drvObj != NULL) ;
    DBC_Require (IS_OBJECT_VALID (drvObj, SIGN_DRV)) ;

    if (IS_OBJECT_VALID (drvObj, SIGN_DRV)) {
        drvObj->refCount-- ;
        if (drvObj->refCount == 0) {
#if defined (CHNL_COMPONENT)
            length =  sizeof (DrvAddrMapEntry) * MAX_CHANNELS * MAX_PROCESSORS ;

            osStatus = munmap (mapTable, length) ;
            if (osStatus == -1) {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
#endif /* if defined (CHNL_COMPONENT) */

            osStatus = close (drvObj->driverHandle) ;

            drvObj->signature = SIGN_NULL ;
            free (drvObj) ;
            drvObj = NULL ;

            if ((osStatus < 0) && DSP_SUCCEEDED (status)) {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
            else if (DSP_SUCCEEDED (status)) {
                status = DSP_SFINALIZED ;
            }
        }
    }
    else {
        status = DSP_EPOINTER ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("DRV_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @name   DRV_Invoke
 *
 *  @desc   Invokes the lower layer function, which is OS dependent.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DRV_Invoke (IN         DRV_Object * drvObj,
            IN         Uint32       cmdId,
            OPT IN OUT Pvoid        arg1,
            OPT IN OUT Pvoid        arg2)
{
    DSP_STATUS  status    = DSP_SOK ;
    DSP_STATUS  tmpStatus = DSP_SOK ;
    CMD_Args *  args      = arg1    ;
    int         osStatus            ;

#if defined (CHNL_COMPONENT)
    ProcessorId procId              ;
    ChannelId   chnlId              ;
#endif /* if defined (CHNL_COMPONENT) */


    TRC_4ENTER ("DRV_Invoke", drvObj, cmdId, arg1, arg2) ;

#if defined (MSGQ_COMPONENT)
    DBC_Require (   ((drvObj != NULL) && (IS_OBJECT_VALID (drvObj, SIGN_DRV)))
                 || ((drvObj == NULL) && (cmdId == CMD_MSGQ_ALLOCATOROPEN))
                 || ((drvObj == NULL) && (cmdId == CMD_PROC_ATTACH))) ;
#else /* if defined (MSGQ_COMPONENT) */
    DBC_Require (   ((drvObj != NULL) && (IS_OBJECT_VALID (drvObj, SIGN_DRV)))
                 || ((drvObj == NULL) && (cmdId == CMD_PROC_ATTACH))) ;
#endif /* if defined (MSGQ_COMPONENT) */

    if (   IS_OBJECT_VALID (drvObj, SIGN_DRV)
#if defined (MSGQ_COMPONENT)
        || (cmdId == CMD_MSGQ_ALLOCATOROPEN)
#endif /* if defined (MSGQ_COMPONENT) */
        || (cmdId == CMD_PROC_ATTACH)) {
        DBC_Assert (args != NULL) ;

        switch (cmdId) {

#if defined (MSGQ_COMPONENT)
        case CMD_MSGQ_PUT:
            {
                args->apiArgs.msgqPutArgs.mqaId =
                             args->apiArgs.msgqPutArgs.msg->mqaId ;
                osStatus = ioctl (drvObj->driverHandle,
                                  cmdId,
                                  args) ;
                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            break ;

        case CMD_MSGQ_GET:
            {
                osStatus = ioctl (drvObj->driverHandle, cmdId, args) ;

                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            break ;
#endif /* if defined (MSGQ_COMPONENT) */

#if defined (CHNL_COMPONENT)
        /*  Channel Issue & Reclaim is handled on top of switch statement
         *  to optimize the data transfer path.
         */
        case CMD_CHNL_ISSUE:
        case CMD_CHNL_RECLAIM:
            {
                osStatus = ioctl (drvObj->driverHandle, cmdId, args) ;

                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            break ;

        case CMD_CHNL_ALLOCATEBUFFER:
            {
                Char8  *   userAddrStart = NULL ;
                Char8  **  bufArray      = NULL ;
                Uint32     i                    ;
                Uint32     numBufs              ;
                Uint32     size                 ;
                off_t      phyAddr              ;

                procId   = args->apiArgs.chnlAllocateBufferArgs.procId   ;
                chnlId   = args->apiArgs.chnlAllocateBufferArgs.chnlId   ;
                size     = args->apiArgs.chnlAllocateBufferArgs.size     ;
                numBufs  = args->apiArgs.chnlAllocateBufferArgs.numBufs  ;
                bufArray = args->apiArgs.chnlAllocateBufferArgs.bufArray ;

                osStatus = ioctl (drvObj->driverHandle,
                                  CMD_CHNL_ALLOCATEBUFFER,
                                  args) ;

                if ((osStatus != 0) || (DSP_FAILED (args->apiStatus))) {
                    if (osStatus != 0) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    /*
                     * This physical address is returned by the previous
                     * ioctl with cmd id CMD_CHNL_ALLOCATEBUFFER
                     */
                    phyAddr = args->apiArgs.chnlAllocateBufferArgs.phyAddr ;

                    userAddrStart = mmap (NULL,
                                          numBufs*size,
                                          PROT_READ|PROT_WRITE,
                                          MAP_SHARED,
                                          drvObj->driverHandle,
                                          phyAddr) ;

                    if (userAddrStart == (Void *)-1) {
                        status = DSP_EMEMORY ;
                        SET_FAILURE_REASON ;
                    }
                    else {
                        for (i = 0 ; i < numBufs ; i++) {
                            bufArray [i] = userAddrStart + (i * size) ;
                        }

                        /*
                         * Put user address in the entry of the mapping
                         * table. Kernel driver code uses it for
                         * translation between user and kernel addresses on
                         * issue and reclaims.
                         */
                        mapTable [procId][chnlId].userAddress
                                                    = userAddrStart ;
                    }
                }
            }
            break ;

        case CMD_CHNL_FREEBUFFER:
            {
                Char8 **   bufArray = NULL  ;
                Uint32     numBufs          ;

                procId = args->apiArgs.chnlFreeBufferArgs.procId ;
                chnlId = args->apiArgs.chnlFreeBufferArgs.chnlId ;
                numBufs = args->apiArgs.chnlFreeBufferArgs.numBufs ;
                bufArray = args->apiArgs.chnlFreeBufferArgs.bufArray ;

                osStatus = munmap (bufArray[0],
                                   mapTable[procId][chnlId].length) ;
                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
                else {
                    osStatus = ioctl (drvObj->driverHandle,
                                      CMD_CHNL_FREEBUFFER,
                                      args) ;
                    if (osStatus < 0) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }
                }
            }
            break ;
#endif /* if defined (CHNL_COMPONENT) */

        case CMD_PROC_ATTACH:
            {
                status = DRV_Initialize (&DRV_Handle, NULL) ;
                if (DSP_SUCCEEDED (status)) {
                    osStatus
                       = ioctl (DRV_Handle->driverHandle, cmdId, args) ;
                    if (osStatus < 0) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }

                /*
                 *  This is the case in which thread of a
                 *  already attached process tries to attach.
                 */
                if (   (DSP_SUCCEEDED (status))
                    && (drvObj != NULL)
                    &&  (args->apiStatus == DSP_SALREADYATTACHED)) {

                    args->apiStatus = DSP_EALREADYCONNECTED ;
                    DRV_Finalize (DRV_Handle, NULL) ;
                }
            }
            break ;

        case CMD_PROC_DETACH:
            {
                if (drvObj == NULL) {
                    status = DSP_EACCESSDENIED ;
                    SET_FAILURE_REASON ;
                }

                if (DSP_SUCCEEDED (status)) {
                    osStatus = ioctl (drvObj->driverHandle, cmdId, args) ;
                    if (osStatus < 0) {
                        status = DSP_EFAIL ;
                        SET_FAILURE_REASON ;
                    }

                    tmpStatus = DRV_Finalize (DRV_Handle, NULL) ;
                    if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                        status = tmpStatus ;
                        SET_FAILURE_REASON ;
                    }
                    if (tmpStatus == DSP_SFINALIZED) {
                        DRV_Handle = NULL ;
                        drvObj = NULL ;
                    }
                }
            }
            break ;

#if defined (MSGQ_COMPONENT)
        case CMD_MSGQ_ALLOCATOROPEN:
            {
                Uint32      size          ;
                Uint32      userAddrStart ;
                off_t       phyAddr       ;
                CMD_Args    cmdArgs       ;
                AllocatorId mqaId         ;

                status = DRV_Initialize (&DRV_Handle, NULL) ;

                if (DSP_SUCCEEDED (status)) {
                    mqaId = args->apiArgs.msgqAllocatorOpenArgs.mqaId ;
                    if (DRV_MsgqBufPoolEntry [mqaId].valid == FALSE) {
                        osStatus = ioctl (DRV_Handle->driverHandle,
                                          cmdId,
                                          args) ;
                        if ((osStatus != 0) || (DSP_FAILED (args->apiStatus))) {
                            if (osStatus != 0) {
                                status = DSP_EFAIL ;
                                SET_FAILURE_REASON ;
                            }
                        }
                        else {
                            /*
                             *  This physical address and size are returned by
                             *  the previous ioctl with cmd id
                             *  CMD_MSGQ_ALLOCATOROPEN.
                             */
                            phyAddr =
                                   args->apiArgs.msgqAllocatorOpenArgs.phyAddr ;
                            size    =
                                   args->apiArgs.msgqAllocatorOpenArgs.size    ;

                            userAddrStart =(Uint32) mmap (NULL,
                                                       size,
                                                       PROT_READ|PROT_WRITE,
                                                       MAP_SHARED,
                                                       DRV_Handle->driverHandle,
                                                       phyAddr) ;

                            if (userAddrStart == (Uint32) -1) {
                                status = DSP_EMEMORY ;
                                SET_FAILURE_REASON ;
                            }
                            else {
                                DRV_MsgqBufPoolEntry [mqaId].userAddress =
                                                    (Void *) userAddrStart ;
                                DRV_MsgqBufPoolEntry [mqaId].length = size ;
                                DRV_MsgqBufPoolEntry [mqaId].valid  = TRUE ;

                                cmdArgs.apiArgs.drvUsrAddrArgs.mqaId = mqaId ;
                                cmdArgs.apiArgs.drvUsrAddrArgs.usrAddr
                                    = (Void *) userAddrStart ;
                                /*  This ioctl call is made to tell the kernel
                                 *  side about the user address returned by mmap
                                 *  call.
                                 */
                                osStatus = ioctl (DRV_Handle->driverHandle,
                                                  CMD_DRV_ADDUSER_ADDRESS,
                                                  &cmdArgs) ;
                                if (osStatus < 0) {
                                    status = DSP_EFAIL ;
                                    SET_FAILURE_REASON ;
                                }
                            }
                        }
                    }
                    else {
                        /* Allocator is already opened */
                        status = DSP_EACCESSDENIED ;
                        SET_FAILURE_REASON ;
                    }

                    if (DSP_FAILED (status)) {
                        tmpStatus = DRV_Finalize (DRV_Handle, NULL) ;
                        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                            status = tmpStatus ;
                            SET_FAILURE_REASON ;
                        }
                        if (tmpStatus == DSP_SFINALIZED) {
                            DRV_Handle = NULL ;
                            drvObj = NULL ;
                        }
                    }
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
            break ;

        case CMD_MSGQ_ALLOCATORCLOSE:
            {
                AllocatorId mqaId ;

                if (drvObj == NULL) {
                    status = DSP_EACCESSDENIED ;
                    SET_FAILURE_REASON ;
                }

                if (DSP_SUCCEEDED (status)) {
                    mqaId = args->apiArgs.msgqAllocatorCloseArgs.mqaId ;
                    if (DRV_MsgqBufPoolEntry [mqaId].valid == TRUE) {
                        osStatus = munmap (
                                       DRV_MsgqBufPoolEntry [mqaId].userAddress,
                                       DRV_MsgqBufPoolEntry [mqaId].length) ;
                        if (osStatus < 0) {
                            status = DSP_EFAIL ;
                            SET_FAILURE_REASON ;
                        }
                        else {
                            DRV_MsgqBufPoolEntry [mqaId].valid = FALSE ;
                            osStatus = ioctl (drvObj->driverHandle,
                                              cmdId,
                                              args) ;
                            if (osStatus < 0) {
                                status = DSP_EFAIL ;
                                SET_FAILURE_REASON ;
                            }
                        }
                        tmpStatus = DRV_Finalize (DRV_Handle, NULL) ;
                        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
                            status = tmpStatus ;
                            SET_FAILURE_REASON ;
                        }
                        if (tmpStatus == DSP_SFINALIZED) {
                            DRV_Handle = NULL ;
                            drvObj = NULL ;
                        }
                    }
                    else {
                        /* Allocator is not opened */
                        status = DSP_EINVALIDARG ;
                        SET_FAILURE_REASON ;
                    }
                }
            }
            break ;

        case CMD_MSGQ_FREE:
            {
                args->apiArgs.msgqFreeArgs.mqaId =
                             args->apiArgs.msgqFreeArgs.msg->mqaId ;
                osStatus = ioctl (drvObj->driverHandle,
                                  cmdId,
                                  args) ;
                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            break ;

        case CMD_MSGQ_GETREPLYID:
            {
                args->apiArgs.msgqGetReplyIdArgs.mqaId =
                             args->apiArgs.msgqGetReplyIdArgs.msg->mqaId ;
                osStatus = ioctl (drvObj->driverHandle,
                                  cmdId,
                                  args) ;
                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            break ;

#endif /* if defined (MSGQ_COMPONENT) */

        default:
            {
                osStatus = ioctl (drvObj->driverHandle, cmdId, args) ;

                if (osStatus < 0) {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            break ;
        }

        /* Return API's status if ioctl is successful */
        if (DSP_SUCCEEDED (status)) {
            status = args->apiStatus ;
        }
    }
    else {
        status = DSP_EATTACHED ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("DRV_Invoke", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
