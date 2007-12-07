/** ============================================================================
 *  @file   OsalDriver.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\kernel\Linux
 *
 *  @desc   Implementation of Osal Linux module driver interface.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */

/*  ------------------------------------- OS Specific Headers         */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

/*  ------------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <errbase.h>

/*  ------------------------------------- Trace & Debug               */
#include <signature.h>
#include <safe.h>
#include <dbc.h>
#include <trc.h>

/*  ------------------------------------- OSAL Headers                */
#include <prcs.h>
#include <print.h>
#include <TST_Framework.h>
#include <TST_PrintFuncs.h>
#include <OsalDriver.h>
#include <OSAL_DpcCreate.h>
#include <OSAL_DpcDelete.h>
#include <OSAL_DpcFinalize.h>
#include <OSAL_DpcInitialize.h>
#include <OSAL_DpcSchedule.h>
#include <OSAL_IsrCreate.h>
#include <OSAL_IsrDelete.h>
#include <OSAL_IsrInitialize.h>
#include <OSAL_IsrFinalize.h>
#include <OSAL_IsrInstall.h>
#include <OSAL_IsrUninstall.h>
#include <OSAL_SyncInitialize.h>
#include <OSAL_SyncFinalize.h>
#include <OSAL_SyncOpenEvent.h>
#include <OSAL_SyncSetEvent.h>
#include <OSAL_SyncResetEvent.h>
#include <OSAL_SyncWaitOnEvent.h>
#include <OSAL_SyncCloseEvent.h>
#include <OSAL_SyncCreateCS.h>
#include <OSAL_SyncDeleteCS.h>
#include <OSAL_SyncEnterCS.h>
#include <OSAL_SyncLeaveCS.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Identifier for sub-component.
 *  ============================================================================
 */
#define  DRIVER_NAME_STR    "test_driver"

/** ============================================================================
 *  @macro  major
 *
 *  @desc   Major number of driver.
 *  ============================================================================
 */
static int major = 210 ;


/** ============================================================================
 *  @func  OsalDriverOpen
 *
 *  @desc  This function gets called when the user opens the driver.
 *  ============================================================================
 */
int
OsalDriverOpen (struct inode * inode, struct file * filp) ;


/** ============================================================================
 *  @func  OsalDriverRelease
 *
 *  @desc  This function gets called when the user closes the driver.
 *  ============================================================================
 */
int
OsalDriverRelease (struct inode * inode, struct file * filp) ;


/** ============================================================================
 *  @func  OsalDriverIoctl
 *
 *  @desc  This function gets called when the user cals the ioctl command after
 *         opening the driver.
 *  ============================================================================
 */
int
OsalDriverIoctl (struct inode * inode,
                 struct file * filp,
                 unsigned int cmd,
                 unsigned long args) ;


/** ============================================================================
 *  @func  CallAPI
 *
 *  @desc  This function maps the appropriate osal function to the various ioctl
 *         commands.
 *  ============================================================================
 */
DSP_STATUS
CallAPI (Uint32 cmd, TST_Args * args, DSP_STATUS * retVal) ;


/** ============================================================================
 *  @func  testDriverFileOps
 *
 *  @desc  This function maps the appropriate osal function to the various ioctl
 *         commands.
 *  ============================================================================
 */
static struct file_operations testDriverFileOps = {
    open    :  OsalDriverOpen  ,
    ioctl   :  OsalDriverIoctl ,
    release :  OsalDriverRelease
} ;


/** ============================================================================
 *  @func  initOsalDriverModule
 *
 *  @desc  This function gets invoked when the driver is inserted into the
 *         kernel.
 *  ============================================================================
 */
int initOsalDriverModule (void)
{
    int result ;
    DSP_STATUS status ;

    status = MEM_Initialize () ;
    if (DSP_SUCCEEDED (status)) {
        result = register_chrdev (major, DRIVER_NAME_STR, &testDriverFileOps) ;
        if (result < 0) {
            status = DSP_EFAIL ;
            TST_PrnError ("initOsalDriverModule failed"
                          " Status = [0x%x]", status) ;
            TST_PrnInfo ("Linux API register_chrdev returned error code"
                          " :  %x", result) ;
        }
    }
    else {
        result = -1 ;
    }

    return result ;
}


/** ============================================================================
 *  @func  finalizeOsalDriverModule
 *
 *  @desc  This function gets invoked when the driver is unloaded from the
 *         kernel.
 *  ============================================================================
 */
void
finalizeOsalDriverModule (void)
{
    unregister_chrdev (major, DRIVER_NAME_STR) ;
}


/** ============================================================================
 *  @func  OsalDriverOpen
 *
 *  @desc  This function gets called when the user opens the driver.
 *  ============================================================================
 */
int
OsalDriverOpen (struct inode * inode, struct file * filp)
{
    MOD_INC_USE_COUNT ;
    return 0 ;
}


/** ============================================================================
 *  @func  OsalDriverRelease
 *
 *  @desc  This function gets called when the user closes the driver.
 *  ============================================================================
 */
int
OsalDriverRelease (struct inode * inode, struct file * filp)
{
    MOD_DEC_USE_COUNT ;
    return 0 ;
}


/** ============================================================================
 *  @func  OsalDriverIoctl
 *
 *  @desc  This function gets called when the user cals the ioctl command after
 *         opening the driver.
 *  ============================================================================
 */
int OsalDriverIoctl (struct inode  * inode,
                     struct file   * filp,
                     unsigned int    cmd,
                     unsigned long   args)
{
    TST_Args   apiArgs  ;
    int        osStatus ;
    DSP_STATUS status   ;

    /* copy user sent arguments to kernel space */
    osStatus = copy_from_user (&apiArgs, (TST_Args *)args, sizeof(TST_Args)) ;

    if (osStatus >= 0) {
        status = CallAPI (cmd, &apiArgs, &(((TST_Args *)args)->apiStatus)) ;
    }
    return 0 ;
}


/** ============================================================================
 *  @func  CallAPI
 *
 *  @desc  This function maps the appropriate osal function to the various ioctl
 *         commands.
 *  ============================================================================
 */
DSP_STATUS CallAPI (Uint32 cmd, TST_Args * args, DSP_STATUS * retVal)
{
    DSP_STATUS status = DSP_SOK ;

    switch (cmd) {
        case DPC_Initialize_ID :
            *retVal = OSAL_DpcInitialize (args->argc, args->argv) ;
            break ;

        case DPC_Finalize_ID :
            *retVal = OSAL_DpcFinalize (args->argc, args->argv) ;
            break ;

        case DPC_Create_ID :
            *retVal = OSAL_DpcCreate (args->argc, args->argv) ;
            break ;

        case DPC_Delete_ID :
            *retVal = OSAL_DpcDelete (args->argc, args->argv) ;
            break ;

        case DPC_Schedule_ID :
            *retVal = OSAL_DpcSchedule (args->argc, args->argv) ;
            break ;

        case ISR_Initialize_ID :
            *retVal = OSAL_IsrInitialize (args->argc, args->argv) ;
            break ;

        case ISR_Finalize_ID :
            *retVal = OSAL_IsrFinalize (args->argc, args->argv) ;
            break ;

        case ISR_Create_ID :
            *retVal = OSAL_IsrCreate (args->argc, args->argv) ;
            break ;

        case ISR_Delete_ID :
            *retVal = OSAL_IsrDelete (args->argc, args->argv) ;
            break ;

        case ISR_Install_ID :
            *retVal = OSAL_IsrInstall (args->argc, args->argv) ;
            break ;

        case ISR_Uninstall_ID :
            *retVal = OSAL_IsrUninstall (args->argc, args->argv) ;
            break ;

        case SYNC_Initialize_ID :
            *retVal = OSAL_SyncInitialize (args->argc, args->argv) ;
            break ;

        case SYNC_Finalize_ID :
            *retVal = OSAL_SyncFinalize (args->argc, args->argv) ;
            break ;

        case SYNC_OpenEvent_ID :
            *retVal = OSAL_SyncOpenEvent (args->argc, args->argv) ;
            break ;

        case SYNC_CloseEvent_ID :
            *retVal = OSAL_SyncCloseEvent (args->argc, args->argv) ;
            break ;

        case SYNC_ResetEvent_ID :
            *retVal = OSAL_SyncResetEvent (args->argc, args->argv) ;
            break ;

        case SYNC_SetEvent_ID :
            *retVal = OSAL_SyncSetEvent (args->argc, args->argv) ;
            break ;

        case SYNC_WaitOnEvent_ID :
            *retVal = OSAL_SyncWaitOnEvent (args->argc, args->argv) ;
            break ;

        case SYNC_CreateCS_ID :
            *retVal = OSAL_SyncCreateCS (args->argc, args->argv) ;
            break ;

        case SYNC_DeleteCS_ID :
            *retVal = OSAL_SyncDeleteCS (args->argc, args->argv) ;
            break ;

        case SYNC_EnterCS_ID :
            *retVal = OSAL_SyncEnterCS (args->argc, args->argv) ;
            break ;

        case SYNC_LeaveCS_ID :
            *retVal = OSAL_SyncLeaveCS (args->argc, args->argv) ;
            break ;

        default :
            status = DSP_EFAIL ;
            TST_PrnError ("CallAPI failed Status = [0x%x]", status) ;
            TST_PrnInfo ("Unknown API request cmd = [0x%x]", cmd) ;
            break ;
    }
    return status ;
}


/** ============================================================================
 *  @func  module_init
 *
 *  @desc  This function calls initOsalDriverModule when the Osal Driver is
 *         inserted into the kernel.
 *  ============================================================================
 */
module_init (initOsalDriverModule) ;


/** ============================================================================
 *  @func  module_exit
 *
 *  @desc  This function calls finalizeOsalDriverModule when the Osal Driver is
 *         unloaded from the kernel.
 *  ============================================================================
 */
module_exit (finalizeOsalDriverModule) ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
