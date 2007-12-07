/** ============================================================================
 *  @file   OsalDriver.h
 *
 *  @path   $(DSPLINK)\gpp\src\test\kernel\Linux
 *
 *  @desc   Defines Osal Linux driver header file.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


#if !defined (OSALDRIVER_H)
#define OSALDRIVER_H


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


USES (gpptypes.h)
USES (errbase.h)


/** ============================================================================
 *  @const  BASE_CMD
 *
 *  @desc   Base command id to be used for Linux ioctl's.
 *  ============================================================================
 */
#define BASE_OSAL_IOCTL             0x6d00


/** ============================================================================
 *  @macro  DPC_XXX_ID
 *
 *  @desc   Command ids for DPC Test cases.
 *  ============================================================================
 */
#define    DPC_Initialize_ID          (BASE_OSAL_IOCTL + 1)
#define    DPC_Finalize_ID            (BASE_OSAL_IOCTL + 2)
#define    DPC_Create_ID              (BASE_OSAL_IOCTL + 3)
#define    DPC_Delete_ID              (BASE_OSAL_IOCTL + 4)
#define    DPC_Schedule_ID            (BASE_OSAL_IOCTL + 5)


/** ============================================================================
 *  @macro  ISR_XXX_ID
 *
 *  @desc   Command ids for ISR Test cases.
 *  ============================================================================
 */
#define    ISR_Initialize_ID          (BASE_OSAL_IOCTL + 6)
#define    ISR_Finalize_ID            (BASE_OSAL_IOCTL + 7)
#define    ISR_Create_ID              (BASE_OSAL_IOCTL + 8)
#define    ISR_Delete_ID              (BASE_OSAL_IOCTL + 9)
#define    ISR_Install_ID             (BASE_OSAL_IOCTL + 10)
#define    ISR_Uninstall_ID           (BASE_OSAL_IOCTL + 11)


/** ============================================================================
 *  @macro  SYNC_XXX_ID
 *
 *  @desc   Command ids for SYNC Test cases.
 *  ============================================================================
 */
#define    SYNC_Initialize_ID         (BASE_OSAL_IOCTL + 12)
#define    SYNC_Finalize_ID           (BASE_OSAL_IOCTL + 13)
#define    SYNC_OpenEvent_ID          (BASE_OSAL_IOCTL + 14)
#define    SYNC_CloseEvent_ID         (BASE_OSAL_IOCTL + 15)
#define    SYNC_ResetEvent_ID         (BASE_OSAL_IOCTL + 16)
#define    SYNC_SetEvent_ID           (BASE_OSAL_IOCTL + 17)
#define    SYNC_WaitOnEvent_ID        (BASE_OSAL_IOCTL + 18)
#define    SYNC_CreateCS_ID           (BASE_OSAL_IOCTL + 19)
#define    SYNC_DeleteCS_ID           (BASE_OSAL_IOCTL + 20)
#define    SYNC_EnterCS_ID            (BASE_OSAL_IOCTL + 21)
#define    SYNC_LeaveCS_ID            (BASE_OSAL_IOCTL + 22)



/** ============================================================================
 *  @name   TST_Args
 *
 *  @desc   Struct defining arguments to be passed to ioctl calls.
 *  ============================================================================
 */
typedef struct TST_Args_tag {
    Uint32 argc;
    Char8 ** argv;
    DSP_STATUS apiStatus ;
} TST_Args ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */


#endif /* !define (OSALDRIVER_H) */
