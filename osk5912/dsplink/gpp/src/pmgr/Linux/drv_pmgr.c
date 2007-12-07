/** ============================================================================
 *  @file   drv_pmgr.c
 *
 *  @path   $(DSPLINK)\gpp\src\pmgr\Linux
 *
 *  @desc   Implementation of linux module driver interface.
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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/io.h>

/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <safe.h>

#include <cfgdefs.h>
#if defined (MSGQ_COMPONENT)
#include <msgqdefs.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Profiling                   */
#include <profile.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- OSAL Headers                */
#include <osal.h>
#include <print.h>
#include <mem.h>
#include <mem_os.h>
#include <prcs.h>
#include <drv_pmgr.h>
#include <drv_api.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>
#include <buf.h>

/*  ----------------------------------- Processor Manager           */
#include <pmgr_proc.h>
#if defined (CHNL_COMPONENT)
#include <pmgr_chnl.h>
#endif /* if defined (CHNL_COMPONENT) */

#if defined (MSGQ_COMPONENT)
#include <pmgr_msgq.h>
#endif /* if defined (MSGQ_COMPONENT) */

/*  ----------------------------------- Link Driver                   */
#if defined (MSGQ_COMPONENT)
#include <ldrv_mqa.h>
#include <mqabuf.h>
#endif /* if defined (MSGQ_COMPONENT) */



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
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON GEN_SetReason (status, FID_C_OSAL_DRV_PMGR, __LINE__)

#if defined (CHNL_COMPONENT)
/** ============================================================================
 *  @macro  TO_USER_ADDR
 *
 *  @desc   Translate a kernel address to  user address for a given channel.
 *  ============================================================================
 */
#define TO_USER_ADDR(procId,chnlId,kernAddr) \
 (   mapInfo [procId][chnlId].userAddress \
   + ((Uint32) kernAddr - (Uint32) mapInfo [procId][chnlId].kernelAddress))

/** ============================================================================
 *  @macro  TO_KERN_ADDR
 *
 *  @desc   Translate a user address to  kernel address for a given channel.
 *  ============================================================================
 */
#define TO_KERN_ADDR(procId,chnlId,userAddr) \
 (   mapInfo [procId][chnlId].kernelAddress \
   + ((Uint32) userAddr - (Uint32) mapInfo [procId][chnlId].userAddress))


#if defined (MSGQ_COMPONENT)
/** ============================================================================
 *  @name   DRV_MsgqBufPoolEntry
 *
 *  @desc   This contains information about the user and kernel buffer mappings
 *          of the MSGQ BUF buffer Pool.
 *  ============================================================================
 */
STATIC DrvAddrMapEntry DRV_MsgqBufPoolEntry [MAX_ALLOCATORS] ;

/** ============================================================================
 *  @macro  MAP_MSGQ_BUFFER_TO_USER
 *
 *  @desc   Translate a kernel address to  user address for a given MSGQ buffer.
 *  ============================================================================
 */
#define MAP_MSGQ_BUFFER_TO_USER(kernAddr, mqaId) \
(  DRV_MsgqBufPoolEntry [mqaId].userAddress \
 + (((Uint32) (kernAddr)) - (Uint32)DRV_MsgqBufPoolEntry [mqaId].kernelAddress))

/** ============================================================================
 *  @macro  MAP_MSGQ_BUFFER_TO_KERN
 *
 *  @desc   Translate a user address to  kernel address for a given MSGQ buffer.
 *  ============================================================================
 */
#define MAP_MSGQ_BUFFER_TO_KERN(userAddr, mqaId) \
(   DRV_MsgqBufPoolEntry [mqaId].kernelAddress \
 + ((Uint32) userAddr - (Uint32)DRV_MsgqBufPoolEntry [mqaId].userAddress))
#endif /* if defined (MSGQ_COMPONENT) */


/** ============================================================================
 *  @name   DrvMapOperator
 *
 *  @desc   Type of translation or mapping to be done.
 *
 *  @field  DRV_KERN_TO_USER
 *              Kernel to user address translation or mapping.
 *  @field  DRV_USER_TO_KERN
 *              user to kernel address translation or mapping.
 *  ============================================================================
 */
typedef enum {
    DRV_KERN_TO_USER,
    DRV_USER_TO_KERN
} DrvMapOperator ;


/** ----------------------------------------------------------------------------
 *  @name   mapInfo
 *
 *  @desc   Address mapping information for each channel.
 *  ----------------------------------------------------------------------------
 */
STATIC DrvAddrMapEntry (*mapInfo) [MAX_CHANNELS] ;
#endif /* if defined (CHNL_COMPONENT) */


/** ----------------------------------------------------------------------------
 *  @name   major
 *
 *  @desc   Major number of driver.
 *  ----------------------------------------------------------------------------
 */
STATIC Int32 major = 230 ;

/** ----------------------------------------------------------------------------
 *  @name   DRV_IsInitialized
 *
 *  @desc   If the module has been initialized or not.
 *  ----------------------------------------------------------------------------
 */
STATIC Bool DRV_IsInitialized = FALSE ;


/** ----------------------------------------------------------------------------
 *  @name   DRV_CallAPI
 *
 *  @desc   Function to invoke the APIs through ioctl.
 *
 *  @arg    cmd
 *              Command identifier.
 *          args
 *              Arguments to be passed for the command.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              Operation failed.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DRV_CallAPI (Uint32 cmd, CMD_Args * args) ;


/** ----------------------------------------------------------------------------
 *  @name   DRV_Open
 *
 *  @desc   Linux driver function to open the driver object.
 *
 *  @arg    inode
 *              inode pointer.
 *          filp
 *              File structure pointer.
 *
 *  @ret    0
 *              Success.
 *          non-zero
 *              Failure.
 *
 *  @enter  None.
 *
 *  @leave  None.
 *
 *  @see    None.

 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
int
DRV_Open    (struct inode * inode, struct file * filp) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_Release
 *
 *  @desc   Release function for Linux LINK driver.
 *
 *  @arg    filp
 *              file pointer.
 *  @arg    vma
 *              User virtual memory area structure pointer.
 *
 *  @ret    0
 *              Operation successfully completed.
 *          non-zero
 *              failure occured.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
int
DRV_Release (struct inode * inode, struct file * filp) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_Ioctl
 *
 *  @desc   ioctl function for of Linux LINK driver.
 *
 *  @arg    inode
 *              inode pointer.
 *  @arg    filp
 *              file pointer.
 *  @arg    cmd
 *              IOCTL command id.
 *  @arg    args
 *              arguments for the command.
 *
 *  @ret    0
 *              Operation successfully completed.
 *          non-zero
 *              failure occured.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
int
DRV_Ioctl (struct inode * inode, struct file * filp,
           unsigned int cmd, unsigned long args) ;


#if defined (CHNL_COMPONENT)
/** ----------------------------------------------------------------------------
 *  @func   DRV_Mmap
 *
 *  @desc   mmap function for LINK Linux driver.
 *
 *  @arg    filp
 *              file pointer.
 *  @arg    vma
 *              User virtual memory area structure pointer.
 *
 *  @ret    0
 *              Operation successfully completed.
 *          non-zero
 *              failure occured.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
int
DRV_Mmap (struct file *filp, struct vm_area_struct *vma) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_AddMapping
 *
 *  @desc   Adds user kernel address mapping information.
 *
 *  @arg    kernelAddress
 *              Kernel address value.
 *  @arg    userAddress
 *              User mode address value.
 *  @arg    physicalAddress
 *              Physical address value.
 *  @arg    length
 *              Length of mapping.
 *  @arg    oper
 *              Mapping type.
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
DRV_AddMapping (IN  Pvoid           kernelAddress,
                IN  Pvoid           userAddress,
                IN  Pvoid           physicalAddress,
                IN  Uint32          length,
                IN  DrvMapOperator  oper,
                IN  ProcessorId     procId,
                IN  ChannelId       chnlId) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_RemoveMapping
 *
 *  @desc   Removes the user kernel address mapping information for a
 *          particular channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    None
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
Void
DRV_RemoveMapping (IN  ProcessorId     procId,
                   IN  ChannelId       chnlId) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_GetMappingEntry
 *
 *  @desc   Gets the mapping entry for a particular channel.
 *
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    non-NULL
 *              Pointer to entry in the map table fo this particular channel.
 *          NULL
 *              If entry does not exist.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DrvAddrMapEntry *
DRV_GetMappingEntry (IN ProcessorId procId, IN ChannelId chnlId) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_Translate
 *
 *  @desc   Translate address from one domain to another.
 *
 *  @arg    toAddress
 *              Value of translated addresss.
 *  @arg    frmAddress
 *              Source address for translation.
 *  @arg    oper
 *              Translation type.
 *  @arg    procId
 *              Processor Identifier.
 *  @arg    chnlId
 *              Channel Identifier.
 *
 *  @ret    DSP_SOK
 *              Operation successfully completed.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DRV_Translate (IN OUT Pvoid *         toAddress,
               IN     Pvoid           frmAddress,
               IN     DrvMapOperator  oper,
               IN     ProcessorId     procId,
               IN     ChannelId       chnlId) ;
#endif /* if defined (CHNL_COMPONENT) */


/** ----------------------------------------------------------------------------
 *  @func   DRV_InitializeModule
 *
 *  @desc   Module initialization function for Linux driver.
 *
 *  @arg    None.
 *
 *  @ret    0
 *              Successful execution.
 *          non-zero
 *              Failure.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC int __init DRV_InitializeModule (void) ;


/** ----------------------------------------------------------------------------
 *  @func   DRV_FinalizeModule
 *
 *  @desc   Module finalization  function for Linux driver.
 *
 *  @arg    None.
 *
 *  @ret    None.
 *
 *  @enter  None
 *
 *  @leave  None
 *
 *  @see    None
 *  ----------------------------------------------------------------------------
 */
STATIC void  __exit DRV_FinalizeModule (void) ;


/** ----------------------------------------------------------------------------
 *  @name   DriverOps
 *
 *  @desc   Function to invoke the APIs through ioctl.
 *  ----------------------------------------------------------------------------
 */
STATIC struct file_operations driverOps = {
    open:    DRV_Open,
    release: DRV_Release,
    ioctl:   DRV_Ioctl,
#if defined (CHNL_COMPONENT)
    mmap:    DRV_Mmap,
#endif /* if defined (CHNL_COMPONENT) */
} ;


#if defined (CHNL_COMPONENT)
/** ----------------------------------------------------------------------------
 *  @name   memAllocAttrs
 *
 *  @desc   Allocation arguments for Map table allocation. These are filled up
 *          by alloc function and used by free function.
 *  ----------------------------------------------------------------------------
 */
STATIC MemAllocAttrs   DRV_MemAllocAttrs ;


/** ----------------------------------------------------------------------------
 *  @name   DRV_Mmap
 *
 *  @desc   Mmap function implementation.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
int
DRV_Mmap (struct file * filp, struct vm_area_struct * vma)
{
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT ;

    if ((offset >= __pa (high_memory)) || (filp->f_flags & O_SYNC)) {
        vma->vm_flags |= VM_IO ;
    }

    vma->vm_page_prot = pgprot_noncached (vma->vm_page_prot) ;

    /* Don't swap them out */
    vma->vm_flags |= VM_RESERVED ;

    if (remap_page_range (vma->vm_start, offset,
                          vma->vm_end-vma->vm_start,
                          vma->vm_page_prot)) {
        return -EAGAIN ;
    }

    return 0 ;
}
#endif /* if defined (CHNL_COMPONENT) */


/** ----------------------------------------------------------------------------
 *  @func   DRV_InitializeModule
 *
 *  @desc   Module initialization  function for Linux driver.
 *  ----------------------------------------------------------------------------
 */
STATIC
int __init DRV_InitializeModule (void)
{
    int             result = 0       ;
    DSP_STATUS      status = DSP_SOK ;
#if defined (CHNL_COMPONENT)
    Uint32          tabSize          ;
#endif /* if defined (CHNL_COMPONENT) */

    status = OSAL_Initialize () ;

    TRC_0ENTER ("DRV_InitializeModule") ;

    if (DSP_SUCCEEDED (status)) {
        /*  --------------------------------------------------------------------
         *  To enable trace for a component and/or subcomponent, uncomment the
         *  corresponding statements below. (This is not a comprehensive list
         *  of available trace masks. See file signature.h)
         *  --------------------------------------------------------------------
         */

        /* TRC_ENABLE (ID_GEN_ALL)     ; */
        /* TRC_ENABLE (ID_GEN_COFF)    ; */
        /* TRC_ENABLE (ID_HAL_ALL)     ; */
        /* TRC_ENABLE (ID_HAL_IO)      ; */
        /* TRC_ENABLE (ID_HAL_HPI)     ; */
        /* TRC_ENABLE (ID_PMGR_ALL)    ; */
        /* TRC_ENABLE (ID_PMGR_PROC)   ; */
        /* TRC_ENABLE (ID_PMGR_CHNL)   ; */
        /* TRC_ENABLE (ID_PMGR_MSGQ)   ; */
        /* TRC_ENABLE (ID_LDRV_LMQT)   ; */
        /* TRC_ENABLE (ID_LDRV_RMQT)   ; */
        /* TRC_ENABLE (ID_LDRV_ALL)    ; */
        /* TRC_ENABLE (ID_GEN_BUF)     ; */
        /* TRC_ENABLE (ID_LDRV_DSP)    ; */
        /* TRC_ENABLE (ID_LDRV_DSP)    ; */
        /* TRC_ENABLE (ID_LDRV_CHNL)   ; */
        /* TRC_ENABLE (ID_LDRV_PROC)   ; */
        /* TRC_ENABLE (ID_LDRV_IO)     ; */
        /* TRC_ENABLE (ID_LDRV_SHM)    ; */
        /* TRC_ENABLE (ID_OSAL_ISR)    ; */
        /* TRC_ENABLE (ID_OSAL_DPC)    ; */
        /* TRC_ENABLE (ID_OSAL_MEM)    ; */
        /* TRC_ENABLE (ID_OSAL_DRIVER) ; */
        /* TRC_ENABLE (ID_OSAL_KFILE)  ; */

        /*  --------------------------------------------------------------------
         *  To set desired severity level for trace, uncomment the statement
         *  below and change the argument to the function below.
         *  --------------------------------------------------------------------
         */

        /* TRC_SET_SEVERITY (TRC_ENTER) ; */


        result = register_chrdev (major, "dsplink", &driverOps) ;
        if (result < 0) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
            TRC_1PRINT (TRC_LEVEL7,
                        "Linux API register_chrdev returned error: %d\n",
                        result) ;
        }
#if defined (CHNL_COMPONENT)
        else {
            tabSize = sizeof (DrvAddrMapEntry) * MAX_PROCESSORS * MAX_CHANNELS ;
            status = MEM_Calloc ((Void **) &mapInfo,
                                 tabSize,
                                 &DRV_MemAllocAttrs) ;

            if (DSP_FAILED (status)) {
                result = -1 ;
                SET_FAILURE_REASON ;
            }
        }
#endif /* if defined (CHNL_COMPONENT) */

        if (DSP_SUCCEEDED (status)) {
            DRV_IsInitialized = TRUE ;
        }
    }
    else {
        SET_FAILURE_REASON ;
        result = -1 ;
    }

    TRC_1LEAVE ("DRV_InitializeModule", result) ;

    return result ;
}


/** ----------------------------------------------------------------------------
 *  @name   DRV_FinalizeModule
 *
 *  @desc   Linux driver function to finalize the driver module.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
void __exit DRV_FinalizeModule (void)
{
    DSP_STATUS   status    = DSP_SOK ;

#if defined (CHNL_COMPONENT)
    MemFreeAttrs freeAttrs ;
#endif /* if defined (CHNL_COMPONENT) */

    TRC_0ENTER ("DRV_FinalizeModule") ;

#if defined (CHNL_COMPONENT)
    freeAttrs.physicalAddress = DRV_MemAllocAttrs.physicalAddress ;
    freeAttrs.size = sizeof (DrvAddrMapEntry) * MAX_PROCESSORS * MAX_CHANNELS ;

    status = MEM_Free (mapInfo, &freeAttrs) ;

    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }

    mapInfo = NULL ;
#endif /* if defined (CHNL_COMPONENT) */

    unregister_chrdev (major, "dsplink") ;

    DRV_IsInitialized = FALSE ;

    TRC_0LEAVE ("DRV_FinalizeModule") ;

    status = OSAL_Finalize () ;
    if (DSP_FAILED (status)) {
        SET_FAILURE_REASON ;
    }
}

/** ----------------------------------------------------------------------------
 *  @name   DRV_Open
 *
 *  @desc   Linux specific function to open the driver.
 *  ----------------------------------------------------------------------------
 */
int DRV_Open (struct inode * inode, struct file * filp)
{
    return 0 ;
}

/** ============================================================================
 *  @name   DRV_Release
 *
 *  @desc   Linux specific function to close the driver.
 *  ============================================================================
 */
STATIC
NORMAL_API
int
DRV_Release (struct inode * inode, struct file * filp)
{
    return 0 ;
}

/** ----------------------------------------------------------------------------
 *  @name   DRV_Ioctl
 *
 *  @desc   Function to invoke the APIs through ioctl.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
int DRV_Ioctl (struct inode * inode, struct file * filp,
               unsigned int cmd, unsigned long args)
{
    DSP_STATUS status   = DSP_SOK ;
    int        osStatus = -1      ;

    TRC_4ENTER ("DRV_Ioctl", inode, filp, cmd, args) ;

    status = DRV_CallAPI (cmd, (CMD_Args *) args) ;
    if (DSP_SUCCEEDED (status)) {
        osStatus = 0 ;
    }

    TRC_1LEAVE ("DRV_Ioctl", status) ;

    return osStatus ;
}


/** ============================================================================
 *  @name   module_init/module_exit
 *
 *  @desc   Macro calls that indicate initialization and finalization functions
 *          to the kernel.
 *  ============================================================================
 */
MODULE_LICENSE ("DSP/BIOS(TM) LINK") ;
module_init (DRV_InitializeModule) ;
module_exit (DRV_FinalizeModule) ;


/** ----------------------------------------------------------------------------
 *  @name   DRV_CallAPI
 *
 *  @desc   Function to invoke the APIs through ioctl.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DRV_CallAPI (Uint32 cmd, CMD_Args * args)
{
    DSP_STATUS status    = DSP_SOK ; /* status of driver's ioctl    */
    DSP_STATUS retStatus = DSP_SOK ; /* status of the PMGR function */

    TRC_2ENTER ("DRV_CallAPI", cmd, args) ;

    args->apiStatus = DSP_SOK ;

    switch (cmd) {

#if defined (MSGQ_COMPONENT)
    case CMD_MSGQ_PUT:
        {
            AllocatorId mqaId ;

            mqaId = args->apiArgs.msgqPutArgs.mqaId ;
            if (DRV_MsgqBufPoolEntry [mqaId].valid == TRUE) {
                args->apiArgs.msgqPutArgs.msg =
                     MAP_MSGQ_BUFFER_TO_KERN (args->apiArgs.msgqPutArgs.msg,
                                              mqaId) ;
                retStatus = PMGR_MSGQ_Put(
                                 args->apiArgs.msgqPutArgs.procId,
                                 args->apiArgs.msgqPutArgs.destMsgqId,
                                 args->apiArgs.msgqPutArgs.msg,
                                 args->apiArgs.msgqPutArgs.msgId,
                                 args->apiArgs.msgqPutArgs.srcMsgqId) ;

            }
            else {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_MSGQ_GET:
        {
            AllocatorId mqaId ;

            retStatus = PMGR_MSGQ_Get (
                            args->apiArgs.msgqGetArgs.msgqId,
                            args->apiArgs.msgqGetArgs.timeout,
                            args->apiArgs.msgqGetArgs.msg) ;

            if (DSP_SUCCEEDED (retStatus)) {
                mqaId = (*(args->apiArgs.msgqGetArgs.msg))->mqaId ;
                if (DRV_MsgqBufPoolEntry [mqaId].valid == TRUE) {
                    *(args->apiArgs.msgqGetArgs.msg) =
                     MAP_MSGQ_BUFFER_TO_USER (
                        *(args->apiArgs.msgqGetArgs.msg), mqaId) ;
                }
                else {
                    status = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            args->apiStatus = retStatus ;
        }
        break ;
#endif /* if defined (MSGQ_COMPONENT) */

#if defined (CHNL_COMPONENT)
    case CMD_CHNL_ISSUE:
        {
            ProcessorId procId = args->apiArgs.chnlIssueArgs.procId ;
            ChannelId chnlId = args->apiArgs.chnlIssueArgs.chnlId  ;
            Uint8 * kernAddress = NULL ;
            Uint8 * userAddress = NULL ;

            userAddress = args->apiArgs.chnlIssueArgs.ioReq->buffer ;

            /* Translate User address to kernel address */
            status = DRV_Translate (
                           (Void **) &kernAddress,
                           args->apiArgs.chnlIssueArgs.ioReq->buffer,
                           DRV_USER_TO_KERN,
                           procId,
                           chnlId) ;

            if (DSP_SUCCEEDED (status)){
                args->apiArgs.chnlIssueArgs.ioReq->buffer = kernAddress ;

                retStatus = PMGR_CHNL_Issue (
                                args->apiArgs.chnlIssueArgs.procId,
                                args->apiArgs.chnlIssueArgs.chnlId,
                                args->apiArgs.chnlIssueArgs.ioReq) ;

                /* Put back the user address again  to ioReq->buffer */
                args->apiArgs.chnlIssueArgs.ioReq->buffer = userAddress ;
                args->apiStatus = retStatus ;
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        break ;

    case CMD_CHNL_RECLAIM:
        {
            ProcessorId procId = args->apiArgs.chnlReclaimArgs.procId ;
            ChannelId chnlId = args->apiArgs.chnlReclaimArgs.chnlId  ;
            Uint8 * userAddress  = NULL ;

            if (DSP_SUCCEEDED (status)){
                retStatus = PMGR_CHNL_Reclaim (
                                args->apiArgs.chnlReclaimArgs.procId,
                                args->apiArgs.chnlReclaimArgs.chnlId,
                                args->apiArgs.chnlReclaimArgs.timeout,
                                args->apiArgs.chnlReclaimArgs.ioReq) ;

                if (DSP_FAILED (retStatus)) {
                    args->apiStatus = retStatus ;
                }
                else {
                    /* Translate back kernel address to user address */
                    status = DRV_Translate (
                                (Void **) &userAddress,
                                args->apiArgs.chnlReclaimArgs.ioReq->buffer,
                                DRV_KERN_TO_USER,
                                procId,
                                chnlId) ;
                    if (DSP_FAILED (status)) {
                        SET_FAILURE_REASON ;
                    }
                    else {
                        args->apiArgs.chnlReclaimArgs.ioReq->buffer
                                                        = userAddress ;
                        args->apiStatus = retStatus ;
                    }
                }
            }
            else {
                SET_FAILURE_REASON ;
            }
        }
        break ;
#endif /* if defined (CHNL_COMPONENT) */

    case CMD_PROC_SETUP:
        {
#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
            Uint32 i ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */

#if defined (CHNL_COMPONENT)
            Uint32 j ;
            /*  ----------------------------------------------------------------
             *  For all channels set the valid bit of mapInfo structure to FALSE
             *  ----------------------------------------------------------------
             */
            for (i = 0 ; i < MAX_PROCESSORS ; i++) {
                for (j = 0 ; j < MAX_CHANNELS ; j++) {
                    mapInfo [i][j].valid = FALSE ;
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

            retStatus = PMGR_PROC_Setup () ;
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_PROC_DESTROY:
        {
#if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT)
            Uint32 i ;
#endif /* if defined (CHNL_COMPONENT) || defined (MSGQ_COMPONENT) */

#if defined (CHNL_COMPONENT)
            Uint32 j ;

            /*  ----------------------------------------------------------------
             *  For all channels where freebuffer was not called, set the valid
             *  bit of mapInfo structure to FALSE.
             *  ----------------------------------------------------------------
             */
            for (i = 0 ; i < MAX_PROCESSORS ; i++) {
                for (j = 0 ; j < MAX_CHANNELS ; j++) {
                    DBC_Assert (mapInfo [i][j].valid == FALSE) ;
                    mapInfo [i][j].valid = FALSE ;
                }
            }
#endif /* if defined (CHNL_COMPONENT) */

#if defined (MSGQ_COMPONENT)
            /*  ----------------------------------------------------------------
             *  For all allocators that were not closed, set the valid bit of
             *  DRV_MsgqBufPoolEntry structure to FALSE.
             *  ----------------------------------------------------------------
             */
            for (i = 0 ; i < MAX_ALLOCATORS ; i++) {
                DBC_Assert (DRV_MsgqBufPoolEntry [i].valid == FALSE) ;
                DRV_MsgqBufPoolEntry [i].valid = FALSE ;
            }
#endif /* if defined (MSGQ_COMPONENT) */

            retStatus = PMGR_PROC_Destroy () ;
            args->apiStatus = retStatus ;
#if defined (DDSP_DEBUG)
            MEM_Debug () ;
#endif /* if defined (DDSP_DEBUG) */
        }

        break ;

    case CMD_PROC_START:
        retStatus = PMGR_PROC_Start (args->apiArgs.procStartArgs.procId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_PROC_STOP:
        retStatus = PMGR_PROC_Stop (args->apiArgs.procStopArgs.procId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_PROC_LOAD:
        retStatus = PMGR_PROC_Load (args->apiArgs.procLoadArgs.procId,
                                    args->apiArgs.procLoadArgs.imagePath,
                                    args->apiArgs.procLoadArgs.argc,
                                    args->apiArgs.procLoadArgs.argv) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_PROC_LOADSECTION:
        retStatus = PMGR_PROC_LoadSection (
                args->apiArgs.procLoadSectionArgs.procId,
                args->apiArgs.procLoadSectionArgs.imagePath,
                args->apiArgs.procLoadSectionArgs.sectID) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_PROC_ATTACH:
        retStatus = PMGR_PROC_Attach (args->apiArgs.procAttachArgs.procId,
                                      args->apiArgs.procAttachArgs.attr) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_PROC_DETACH:
        retStatus = PMGR_PROC_Detach (args->apiArgs.procDetachArgs.procId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_PROC_CONTROL:
        retStatus = PMGR_PROC_Control (
                     args->apiArgs.procControlArgs.procId,
                     args->apiArgs.procControlArgs.cmd,
                     args->apiArgs.procControlArgs.arg) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_PROC_GETSTATE:
        retStatus = PMGR_PROC_GetState (
                        args->apiArgs.procGetStateArgs.procId,
                        args->apiArgs.procGetStateArgs.procState) ;
        args->apiStatus = retStatus ;
        break ;

#if defined (DDSP_PROFILE)
    case CMD_PROC_INSTRUMENT:
        retStatus = PMGR_PROC_Instrument (
                      args->apiArgs.procInstrumentArgs.procId,
                      args->apiArgs.procInstrumentArgs.procStats) ;
        args->apiStatus = retStatus ;
        break ;
#endif /* if defined (DDSP_PROFILE) */

#if defined (DDSP_DEBUG)
    case CMD_PROC_DEBUG:
        PMGR_PROC_Debug (args->apiArgs.procDebugArgs.procId) ;
        break ;
#endif /* if defined (DDSP_DEBUG) */

#if defined (CHNL_COMPONENT)
    case CMD_CHNL_CREATE:
        retStatus = PMGR_CHNL_Create (
                        args->apiArgs.chnlCreateArgs.procId,
                        args->apiArgs.chnlCreateArgs.chnlId,
                        args->apiArgs.chnlCreateArgs.attrs) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_CHNL_DELETE:
        retStatus = PMGR_CHNL_Delete (args->apiArgs.chnlDeleteArgs.procId,
                                      args->apiArgs.chnlDeleteArgs.chnlId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_CHNL_ALLOCATEBUFFER:
        {
            Char8 *           memChunk      = NULL ;
            DrvAddrMapEntry * mapEntry      = NULL ;
            MemAllocAttrs     memAllocAttrs        ;
            Uint32            length               ;
            ProcessorId       procId               ;
            ChannelId         chnlId               ;
            MemFreeAttrs      memFreeAttrs         ;

            procId = args->apiArgs.chnlAllocateBufferArgs.procId ;
            chnlId = args->apiArgs.chnlAllocateBufferArgs.chnlId ;

            mapEntry = DRV_GetMappingEntry (procId, chnlId) ;

            length =   args->apiArgs.chnlAllocateBufferArgs.size
                     * args->apiArgs.chnlAllocateBufferArgs.numBufs ;

            if (mapEntry->valid == TRUE) {
                /* Memory has already been allocated for this channel */
                retStatus = DSP_EFAIL ;
            }
            else {
                retStatus = MEM_Alloc ((Void **) &memChunk,
                                       length,
                                       &memAllocAttrs) ;
            }

            if (DSP_SUCCEEDED (retStatus)) {
                args->apiArgs.chnlAllocateBufferArgs.phyAddr
                                = (Uint32) memAllocAttrs.physicalAddress ;

                /* Add the information about kernel address, length and
                 * physical address. User address information will be
                 * filled by the DRV_API subcomponent for this entry
                 * in the Mapping table
                 */
                status = DRV_AddMapping (memChunk,
                                         0,
                                         memAllocAttrs.physicalAddress,
                                         length,
                                         DRV_KERN_TO_USER,
                                         procId,
                                         chnlId) ;

                if (DSP_FAILED (status)) {
                    /* Free Buffer here */
                    memFreeAttrs.physicalAddress
                                        = memAllocAttrs.physicalAddress ;
                    memFreeAttrs.size = length ;
                    MEM_Free (memChunk, &memFreeAttrs) ;
                    retStatus = DSP_EFAIL ;
                    SET_FAILURE_REASON ;
                }
            }
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_CHNL_FREEBUFFER:
        {
            DrvAddrMapEntry *   mapEntry     = NULL ;
            ProcessorId         procId              ;
            ChannelId           chnlId              ;
            MemFreeAttrs        memFreeAttrs        ;

            procId = args->apiArgs.chnlFreeBufferArgs.procId ;
            chnlId = args->apiArgs.chnlFreeBufferArgs.chnlId ;

            mapEntry = DRV_GetMappingEntry (procId, chnlId) ;

            if (mapEntry->valid == TRUE) {
                memFreeAttrs.physicalAddress = mapEntry->physicalAddress ;
                memFreeAttrs.size = mapEntry->length ;

                DRV_RemoveMapping (procId, chnlId) ;

                retStatus = MEM_Free (mapEntry->kernelAddress,
                                      &memFreeAttrs) ;
            }
            else {
                /* No allocation has been done. */
                retStatus = DSP_EINVALIDARG ;
            }

            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_CHNL_IDLE:
        retStatus = PMGR_CHNL_Idle (
                                args->apiArgs.chnlIdleArgs.procId,
                                args->apiArgs.chnlIdleArgs.chnlId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_CHNL_FLUSH:
        retStatus = PMGR_CHNL_Flush (
                                args->apiArgs.chnlFlushArgs.procId,
                                args->apiArgs.chnlFlushArgs.chnlId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_CHNL_CONTROL:
        retStatus = PMGR_CHNL_Control (
                        args->apiArgs.chnlControlArgs.procId,
                        args->apiArgs.chnlControlArgs.chnlId,
                        args->apiArgs.chnlControlArgs.cmd,
                        args->apiArgs.chnlControlArgs.arg) ;
        args->apiStatus = retStatus ;
        break ;

#if defined (DDSP_PROFILE)
    case CMD_CHNL_INSTRUMENT:
        retStatus = PMGR_CHNL_Instrument (
                       args->apiArgs.chnlInstrumentArgs.procId,
                       args->apiArgs.chnlInstrumentArgs.chnlId,
                       args->apiArgs.chnlInstrumentArgs.chnlStats) ;
        args->apiStatus = retStatus ;
        break ;
#endif /* if defined (DDSP_PROFILE) */

#if defined (DDSP_DEBUG)
    case CMD_CHNL_DEBUG:
        PMGR_CHNL_Debug (args->apiArgs.chnlDebugArgs.procId,
                         args->apiArgs.chnlDebugArgs.chnlId) ;
        break ;
#endif /* if defined (DDSP_DEBUG) */

    case CMD_DRV_GETMAPTABLE_ADDRESS:
        args->apiArgs.drvPhyAddrArgs.phyAddr
                                = (Void *) DRV_MemAllocAttrs.physicalAddress ;
        break ;
#endif /* if defined (CHNL_COMPONENT) */

#if defined (MSGQ_COMPONENT)
    case CMD_MSGQ_ALLOCATOROPEN:
        {
            MqaBufState *   mqaState = NULL ;
            AllocatorId     mqaId           ;

            mqaId = args->apiArgs.msgqAllocatorOpenArgs.mqaId ;
            retStatus = PMGR_MSGQ_AllocatorOpen (
                            mqaId,
                            args->apiArgs.msgqAllocatorOpenArgs.mqaAttrs,
                            (Pvoid *)&mqaState) ;
            if (DSP_SUCCEEDED (retStatus)) {
                DRV_MsgqBufPoolEntry [mqaId].kernelAddress = (Void *)
                                                        (mqaState->virtAddr) ;
                DRV_MsgqBufPoolEntry [mqaId].physicalAddress
                                 = (Void *) (mqaState->phyAddr) ;
                DRV_MsgqBufPoolEntry [mqaId].length = mqaState->size ;
                DRV_MsgqBufPoolEntry [mqaId].valid = TRUE ;

                args->apiArgs.msgqAllocatorOpenArgs.phyAddr
                                = mqaState->phyAddr ;
                args->apiArgs.msgqAllocatorOpenArgs.size  = mqaState->size  ;
            }
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_MSGQ_ALLOCATORCLOSE:
        {
            AllocatorId mqaId ;

            mqaId = args->apiArgs.msgqAllocatorOpenArgs.mqaId ;
            retStatus = PMGR_MSGQ_AllocatorClose (mqaId) ;
            if (DSP_SUCCEEDED (retStatus)) {
                DRV_MsgqBufPoolEntry [mqaId].valid = FALSE ;
            }
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_MSGQ_TRANSPORTOPEN:
        retStatus = PMGR_MSGQ_TransportOpen (
                       args->apiArgs.msgqTransportOpenArgs.mqtId,
                       args->apiArgs.msgqTransportOpenArgs.mqtAttrs) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_MSGQ_TRANSPORTCLOSE:
        retStatus = PMGR_MSGQ_TransportClose (
                       args->apiArgs.msgqTransportCloseArgs.mqtId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_MSGQ_CREATE:
        retStatus = PMGR_MSGQ_Create (
                       args->apiArgs.msgqCreateArgs.msgqId,
                       args->apiArgs.msgqCreateArgs.msgqAttrs) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_MSGQ_DELETE:
        retStatus = PMGR_MSGQ_Delete (
                       args->apiArgs.msgqDeleteArgs.msgqId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_MSGQ_LOCATE:
        retStatus = PMGR_MSGQ_Locate (
                       args->apiArgs.msgqLocateArgs.procId,
                       args->apiArgs.msgqLocateArgs.msgqId,
                       args->apiArgs.msgqLocateArgs.attrs) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_MSGQ_RELEASE:
        retStatus = PMGR_MSGQ_Release (
                       args->apiArgs.msgqReleaseArgs.procId,
                       args->apiArgs.msgqReleaseArgs.msgqId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_MSGQ_ALLOC:
        {
            AllocatorId mqaId ;

            mqaId = args->apiArgs.msgqAllocArgs.mqaId ;
            if (DRV_MsgqBufPoolEntry [mqaId].valid == TRUE) {
                retStatus = PMGR_MSGQ_Alloc (mqaId,
                                args->apiArgs.msgqAllocArgs.size,
                                args->apiArgs.msgqAllocArgs.msg) ;
                if (DSP_SUCCEEDED (retStatus)) {
                    *(args->apiArgs.msgqAllocArgs.msg) =
                     MAP_MSGQ_BUFFER_TO_USER (
                        *(args->apiArgs.msgqAllocArgs.msg), mqaId) ;
                }
            }
            else {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_MSGQ_FREE:
        {
            AllocatorId mqaId ;

            mqaId = args->apiArgs.msgqFreeArgs.mqaId ;
            if (DRV_MsgqBufPoolEntry [mqaId].valid == TRUE) {
                args->apiArgs.msgqFreeArgs.msg =
                     MAP_MSGQ_BUFFER_TO_KERN (args->apiArgs.msgqFreeArgs.msg,
                                              mqaId) ;
                retStatus = PMGR_MSGQ_Free (args->apiArgs.msgqFreeArgs.msg) ;
            }
            else {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_MSGQ_GETREPLYID:
        {
            AllocatorId mqaId ;

            mqaId = args->apiArgs.msgqGetReplyIdArgs.mqaId ;
            if (DRV_MsgqBufPoolEntry [mqaId].valid == TRUE) {
                args->apiArgs.msgqGetReplyIdArgs.msg =
                     MAP_MSGQ_BUFFER_TO_KERN (
                        args->apiArgs.msgqGetReplyIdArgs.msg,
                        mqaId) ;
                retStatus = PMGR_MSGQ_GetReplyId (
                               args->apiArgs.msgqGetReplyIdArgs.msg,
                               args->apiArgs.msgqGetReplyIdArgs.procId,
                               args->apiArgs.msgqGetReplyIdArgs.msgqId) ;

            }
            else {
                status = DSP_EFAIL ;
                SET_FAILURE_REASON ;
            }
            args->apiStatus = retStatus ;
        }
        break ;

    case CMD_MSGQ_SETERRORHANDLER:
        retStatus = PMGR_MSGQ_SetErrorHandler (
                       args->apiArgs.msgqSetErrorHandlerArgs.msgqId,
                       args->apiArgs.msgqSetErrorHandlerArgs.mqaId) ;
        args->apiStatus = retStatus ;
        break ;

    case CMD_DRV_ADDUSER_ADDRESS:
        {
            AllocatorId mqaId ;

            mqaId = args->apiArgs.drvUsrAddrArgs.mqaId ;
            DRV_MsgqBufPoolEntry [mqaId].userAddress =
                                args->apiArgs.drvUsrAddrArgs.usrAddr ;
        }
        break ;

#if defined (DDSP_PROFILE)
    case CMD_MSGQ_INSTRUMENT:
        retStatus = PMGR_MSGQ_Instrument (
                       args->apiArgs.msgqInstrumentArgs.procId,
                       args->apiArgs.msgqInstrumentArgs.msgqId,
                       args->apiArgs.msgqInstrumentArgs.retVal) ;
        args->apiStatus = retStatus ;
        break ;
#endif /* if defined (DDSP_PROFILE) */

#if defined (DDSP_DEBUG)
    case CMD_MSGQ_DEBUG:
        PMGR_MSGQ_Debug (args->apiArgs.msgqDebugArgs.procId,
                         args->apiArgs.msgqDebugArgs.msgqId) ;
        args->apiStatus = retStatus ;
        break ;
#endif /* if defined (DDSP_DEBUG) */

#endif /* if defined (MSGQ_COMPONENT) */
    default:
        TRC_1PRINT (TRC_LEVEL7,
                    "Incorrect command id specified [0x%x]\n",
                    cmd) ;
        status = DSP_EFAIL ;
        break ;
    }

    TRC_1LEAVE ("DRV_CallAPI", status) ;

    return status ;
}


#if defined (CHNL_COMPONENT)
/** ----------------------------------------------------------------------------
 *  @func   DRV_AddMapping
 *
 *  @desc   Adds user kernel address mapping information.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
DSP_STATUS
DRV_AddMapping (IN  Pvoid           kernelAddress,
                IN  Pvoid           userAddress,
                IN  Pvoid           physicalAddress,
                IN  Uint32          length,
                IN  DrvMapOperator  oper,
                IN  ProcessorId     procId,
                IN  ChannelId       chnlId)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("DRV_AddMapping", kernelAddress, userAddress, physicalAddress) ;
    TRC_4ENTER ("DRV_AddMapping", length, oper, procId, chnlId) ;

    DBC_Require (DRV_IsInitialized == TRUE) ;

    if (oper == DRV_KERN_TO_USER) {
        mapInfo [procId][chnlId].kernelAddress = kernelAddress ;
        mapInfo [procId][chnlId].userAddress = userAddress ;
        mapInfo [procId][chnlId].physicalAddress = physicalAddress ;
        mapInfo [procId][chnlId].length = length ;
        mapInfo [procId][chnlId].valid = TRUE ;
    }
    else {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("DRV_AddMapping", status) ;

    return status ;
}


/** ----------------------------------------------------------------------------
 *  @func   DRV_RemoveMapping
 *
 *  @desc   Removes the user kernel address mapping information for a
 *          particular channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
Void
DRV_RemoveMapping (IN  ProcessorId     procId,
                   IN  ChannelId       chnlId)

{
    TRC_2ENTER ("DRV_RemoveMapping", procId, chnlId) ;

    DBC_Require (DRV_IsInitialized == TRUE) ;

    mapInfo [procId][chnlId].valid = FALSE ;

    TRC_0LEAVE ("DRV_RemoveMapping") ;
}


/** ----------------------------------------------------------------------------
 *  @func   DRV_GetMappingEntry
 *
 *  @desc   Gets the mapping entry for a particular channel.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DrvAddrMapEntry *
DRV_GetMappingEntry (IN ProcessorId procId, IN ChannelId chnlId)
{
    TRC_2ENTER ("DRV_GetMappingEntry", procId, chnlId) ;

    DBC_Require (DRV_IsInitialized == TRUE) ;

    TRC_1LEAVE ("DRV_GetMappingEntry", &(mapInfo [procId][chnlId])) ;

    return (&(mapInfo [procId][chnlId])) ;
}


/** ----------------------------------------------------------------------------
 *  @func   DRV_Translate
 *
 *  @desc   Translate address from one domain to another.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DRV_Translate (IN OUT Pvoid *         toAddress,
               IN     Pvoid           frmAddress,
               IN     DrvMapOperator  oper,
               IN     ProcessorId     procId,
               IN     ChannelId       chnlId)
{
    DSP_STATUS status  = DSP_SOK ;

    TRC_5ENTER ("DRV_Translate", frmAddress, toAddress, oper, procId, chnlId) ;

    DBC_Require (DRV_IsInitialized == TRUE) ;
    DBC_Require (toAddress != NULL) ;

    if (toAddress == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (mapInfo [procId][chnlId].valid == FALSE) {
        status = DSP_EMEMORY ;
        SET_FAILURE_REASON ;
    }
    else {
        if (oper == DRV_KERN_TO_USER) {
            DBC_Assert (frmAddress >= mapInfo [procId][chnlId].kernelAddress) ;
            *toAddress = TO_USER_ADDR (procId,
                                       chnlId,
                                       frmAddress) ;
        }
        else if (oper == DRV_USER_TO_KERN) {
            DBC_Assert (frmAddress >= mapInfo [procId][chnlId].userAddress) ;
            *toAddress = TO_KERN_ADDR (procId,
                                       chnlId,
                                       frmAddress) ;
        }
        else {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("DRV_Translate", status) ;

    return status ;
}
#endif /* if defined (CHNL_COMPONENT) */


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
