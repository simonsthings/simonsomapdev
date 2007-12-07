/** ============================================================================
 *  @file   dsp.c
 *
 *  @path   $(DSPLINK)\gpp\src\ldrv\Linux\OMAP
 *
 *  @desc   Implementation of DSP sub-component.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Link               */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <bitops.h>
#include <intobject.h>
#include <safe.h>

#include <dspdefs.h>
#include <linkdefs.h>

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
#include <cfg.h>
#include <mem_os.h>
#include <mem.h>
#include <print.h>

/*  ----------------------------------- Generic Functions           */
#include <gen_utils.h>

/*  ----------------------------------- Link Driver                 */
#include <dsp.h>

/*  ----------------------------------- Hardware Abstraction Layer  */
#include <hal_object.h>
#include <hal_mpui.h>
#include <hal_clkrst.h>
#include <hal_io.h>
#include <hal_inth.h>
#include <hal_mailbox.h>
#include <hal_mmu.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_LDRV_DSP

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason.
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_C_DSP, __LINE__)

/** ============================================================================
 *  @const  DSP_UNRESET_VECTOR
 *
 *  @desc   DSP Unreset vector.
 *  ============================================================================
 */
#define DSP_UNRESET_VECTOR      0x00010000

/*  ============================================================================
 *  @const  ADDR_SELFLOOP_SARAM/ADDR_DARAM_END/ADDR_SELFLOOP_END
 *
 *  @desc   Start address and Byte codes for self loop.
 *  ============================================================================
 */
#define ADDR_SELFLOOP_SARAM     DSP_UNRESET_VECTOR
#define ADDR_SELFLOOP_END       ADDR_SELFLOOP_SARAM + 0x8

#define ADDR_DARAM_START        0x000000
#define ADDR_DARAM_END          0x00FFFF

#define ADDR_SARAM_START        0x010000
#define ADDR_SARAM_END          0x027FFF

/*  ============================================================================
 *  @const  OMAP_MAILBOX_INT1
 *
 *  @desc   Interrupt number corresponding to MAILBOX1 interrupt on OMAP.
 *          (Also defined in shm.c)
 *  ============================================================================
 */
#define OMAP_MAILBOX_INT1       10


/** ============================================================================
 *  @name   HalObjInfo
 *
 *  @desc   Object encapsulating hardware abstration layer.
 *
 *  @field  halObject
 *              HAL object.
 *  @field  isSetup
 *              Indicates whether the HAL object isinitialized.
 *  ============================================================================
 */
typedef struct HalObjInfo_tag {
    HalObject halObject ;
    Bool      isSetup   ;
} HalObjInfo ;


/** ============================================================================
 *  @const  SelfLoopInSARAM
 *
 *  @desc   Self loop in SRAM
 *          The array contains the byte code equivalent to "GOTO 0x010000".
 *          The instruction is followed by NOPs.
 *  ============================================================================
 */
STATIC Uint16 SelfLoopInSARAM [] = {
    0x016A,     /* GOTO 0x010000 */
    0x0000,
    0x2020,     /* NOP */
    0x2020      /* NOP */
} ;

/** ============================================================================
 *  @name   halObject
 *
 *  @desc   Array of HAL objects for each DSP.
 *  ============================================================================
 */
STATIC HalObjInfo halInfo [MAX_PROCESSORS] ;


/** ============================================================================
 *  @name   DSP_MmuSizeMap
 *
 *  @desc   Array containing the size in bytes corresponding to MMU SLST bits.
 *  ============================================================================
 */
STATIC Uint32 DSP_MmuSizeMap [] = {MMU_SIZE_SECTION,
                                   MMU_SIZE_LARGE,
                                   MMU_SIZE_SMALL,
                                   MMU_SIZE_TINY} ;

/** ============================================================================
 *  @name   DSP_InterfaceTable
 *
 *  @desc   The interface table exposed for the shared memory link driver.
 *  ============================================================================
 */
DspInterface OMAP5910_Interface = {
    &DSP_Setup,
    &DSP_Initialize,
    &DSP_Finalize,
    &DSP_Start,
    &DSP_Stop,
    &DSP_Idle,
    &DSP_EnableInterrupt,
    &DSP_DisableInterrupt,
    &DSP_Interrupt,
    &DSP_ClearInterrupt,
    &DSP_Read,
    &DSP_Write,
    &DSP_Control,
#if defined (DDSP_PROFILE)
    &DSP_Instrument,
#endif /* if defined (DDSP_PROFILE) */
#if defined (DDSP_DEBUG)
    &DSP_Debug,
#endif /* if defined (DDSP_DEBUG) */
} ;

/** ----------------------------------------------------------------------------
 *  @func   DSP_Run
 *
 *  @desc   Start execution by putting the branch instruction.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    startAddr
 *              Address from where execution starts.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *
 *  @enter  dspId must be valid.
 *          DSP must have been setup before this call.
 *
 *  @leave  None.
 *
 *  @see    DSP_Start
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_Run (IN ProcessorId dspId, IN Uint32 startAddr) ;


/** ----------------------------------------------------------------------------
 *  @func   DSP_MmuInit
 *
 *  @desc   Initialize the MMU table entries for the DSP.
 *
 *  @arg    dspId
 *              Processor Id.
 *  @arg    numEntries
 *              Number of MMU Entries.
 *  @arg    mmuTable
 *              Array of MMU Entries.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *
 *  @enter  dspId must be valid.
 *          DSP must have been setup before this call.
 *          mmuTable must be a valid pointer.
 *
 *  @leave  None.
 *
 *  @see    DSP_Initialize
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_MmuInit (IN ProcessorId     dspId,
             IN Uint32          numEntries,
             IN DspMmuEntry *   mmuTable) ;


/** ----------------------------------------------------------------------------
 *  @func   DSP_MapAddressInGpp
 *
 *  @desc   Map DSP addresses to GPP space space if required. This function
 *          iterates through the MMU entries to check which entries need to
 *          be mapped.
 *
 *  @arg    dspObj
 *              DSP object containing mapping information.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  dspObj must be a valid pointer.
 *
 *  @leave  None.
 *
 *  @see    DSP_UnmapAddressFromGpp
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_MapAddressInGpp (IN OUT DspObject *  dspObj) ;


/*  ----------------------------------------------------------------------------
 *  @func   DSP_UnmapAddressFromGpp
 *
 *  @desc   Unmap GPP addresses corresponding to DSP address mapped earlier
 *          using a call to DSP_MapAddressInGpp ()
 *
 *  @arg    dspObj
 *              The DSP object.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_EFAIL
 *              General failure.
 *
 *  @enter  dspObj must be a valid pointer.
 *
 *  @leave  None.
 *
 *  @see    DSP_MapAddressInGpp
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_UnmapAddressFromGpp (IN OUT DspObject * dspObj) ;


/** ----------------------------------------------------------------------------
 *  @func   DSP_GetAddressMappedInGpp
 *
 *  @desc   Get mapped address in GPP address space corresponding to DSP
 *          address.
 *
 *  @arg    dspObj
 *              DSP object containing mapping information.
 *  @arg    dspAddr
 *              DSP Address to be mapped.
 *  @arg    dspObj
 *              Location to hold the mapped address in GPP space.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *          DSP_ERANGE
 *              Address does not fall in the range of mapped GPP addresses.
 *
 *  @enter  dspObj must be a valid pointer.
 *          gppAddr must be a valid pointer.
 *
 *  @leave  None.
 *
 *  @see    DSP_UnmapAddressFromGpp
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_GetAddressMappedInGpp (IN  DspObject * dspObj,
                           IN  Uint32      dspAddr,
                           OUT Uint32 *    gppAddr) ;


/** ----------------------------------------------------------------------------
 *  @func   DSP_WriteSelfLoop
 *
 *  @desc   Writes self loop in SARAM area.
 *
 *  @arg    dspId
 *              Processor Id.
 *
 *  @ret    DSP_SOK
 *              Operation completed successfully.
 *
 *  @enter  dspId must be valid.
 *          DSP must have been setup before this call.
 *
 *  @leave  None.
 *
 *  @see    DSP_Stop.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_WriteSelfLoop (IN ProcessorId dspId) ;


/** ============================================================================
 *  @func   DSP_Setup
 *
 *  @desc   Sets up components to make DSP reachable from GPP.
 *          Changes include setting up ARM Port Interface.
 *          Causes no state transition on DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Setup (IN ProcessorId dspId, IN DspObject *  dspObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("DSP_Setup", dspId, dspObj) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (dspObj != NULL) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        /*  ----------------------------------------------------------------
         *  Initialize the HAL object.
         *  ----------------------------------------------------------------
         */
        HAL_Initialize (dspId, &(halInfo [dspId].halObject)) ;
        halInfo [dspId].isSetup = TRUE ;

        /*  ----------------------------------------------------------------
         *  Enable the clocks on ARM side
         *  ----------------------------------------------------------------
         */
        HAL_ClockEnable (&(halInfo [dspId].halObject), CLOCK_MPUI) ;

        /*  ----------------------------------------------------------------
         *  Enable clocks on DSP side
         *  ----------------------------------------------------------------
         */
        HAL_ClockEnable (&(halInfo [dspId].halObject), CLOCK_DSP) ;
        HAL_ClockEnable (&(halInfo [dspId].halObject), CLOCK_DSP_PER) ;

        /*  ----------------------------------------------------------------
         *  Reset DSP
         *  ----------------------------------------------------------------
         */
        HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Reset) ;

        /*  ----------------------------------------------------------------
         *  Disable programming of Priority, EMIF configuration registers
         *  ----------------------------------------------------------------
         */
        HAL_ControlDspInterface (&(halInfo [dspId].halObject),
                                 RSTCT_Reset) ;

        /*  ----------------------------------------------------------------
         *  Allow access of all SARAM banks through API
         *  ----------------------------------------------------------------
         */
        HAL_MpuiSetSize (&(halInfo [dspId].halObject), 0xFFFF) ;

        /*  ----------------------------------------------------------------
         *  Setup ARM Port Interface
         *  ----------------------------------------------------------------
         */
        HAL_MpuiSetup (&(halInfo [dspId].halObject),
                       MPUI_FREQ_HIGH,
                       MPUI_TIMEOUT_EN,
                       MPUI_ERR_EN,
                       MPUI_ACCESS_FACTOR_4,
                       MPUI_TIMEOUT_MAX,
                       MPUI_ENDIANISM_NO_CONVERT ,
                       MPUI_ACCESS_PRIORITY_ARM_DMA_HSAB) ;

        /*  ----------------------------------------------------------------
         *  Tell DSP to boot from SARAM
         *  ----------------------------------------------------------------
         */
        HAL_MpuiBootConfig (&(halInfo [dspId].halObject),
                           MPUI_DSP_BOOT_INTERNAL) ;

        /*  ----------------------------------------------------------------
         *  Enable programming of Priority, EMIF configuration registers
         *  ----------------------------------------------------------------
         */
        HAL_ControlDspInterface (&(halInfo [dspId].halObject),
                                 RSTCT_Release) ;

        /*  ----------------------------------------------------------------
         *  Write the self loop so that when we release the DSP, it
         *  remains in a valid state.
         *  ----------------------------------------------------------------
         */
        status = DSP_WriteSelfLoop (dspId) ;
        if (DSP_FAILED (status)) {
            TRC_0PRINT (TRC_LEVEL7, "Couldn't write self loop !\n") ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("DSP_Setup", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Initialize
 *
 *  @desc   Resets the DSP and initializes the components required by DSP.
 *          Puts the DSP in RESET state.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Initialize (IN ProcessorId  dspId, IN DspObject *  dspObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("DSP_Initialize", dspId, dspObj) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Disable the DSP Watchdog timer
         *  --------------------------------------------------------------------
         */
        HAL_ClockDisable (&(halInfo [dspId].halObject), CLOCK_DSP_WDT) ;

        /*  --------------------------------------------------------------------
         *  Reset DSP
         *  --------------------------------------------------------------------
         */
        HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Reset) ;

        /*  --------------------------------------------------------------------
         *  Map DSP physical address to GPP virtual address.
         *  --------------------------------------------------------------------
         */
        status = DSP_MapAddressInGpp (dspObj) ;

        /*  --------------------------------------------------------------------
         *  Write a 'self loop' on DSP
         *  --------------------------------------------------------------------
         */
        if (DSP_SUCCEEDED (status)) {
            status = DSP_WriteSelfLoop (dspId) ;
            if (DSP_FAILED (status)) {
                TRC_0PRINT (TRC_LEVEL7, "Couldn't write self loop !\n") ;
                SET_FAILURE_REASON ;
            }
        }
        else {
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("DSP_Initialize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Finalize
 *
 *  @desc   Finalizes the DSP sub-component. Either of following can be
 *          implemeted here based on application requirement:
 *          1. Reset the DSP as it it no longer required by application.
 *          2. Call DSP_Idle () to let the DSP run in 'self loop'.
 *          3. Let the DSP run free without modifying its execution state.
 *
 *          Option 1 is implemented here.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Finalize (IN ProcessorId dspId, IN DspObject *  dspObj)
{
    DSP_STATUS status    = DSP_SOK ;
    DSP_STATUS tmpStatus = DSP_SOK ;

    TRC_2ENTER ("DSP_Finalize", dspId, dspObj) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Option 1 : Reset the DSP and disable DSP clock.
         *             (Uncomment the code below)
         *  --------------------------------------------------------------------
         */
        status = DSP_Stop (dspId, dspObj) ;
        if (DSP_SUCCEEDED (status)) {
            HAL_ClockDisable (&(halInfo [dspId].halObject), CLOCK_DSP) ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        /*  --------------------------------------------------------------------
         *  Option 2 : Call DSP_Idle.
         *             (Uncomment the code below)
         *  --------------------------------------------------------------------
         */
        /* status = DSP_Idle (dspId, dspObj) ; */

        /*  --------------------------------------------------------------------
         *  Option 3 : Don't disturb execution state of DSP.
         *             (Comment the code for both options above)
         *  --------------------------------------------------------------------
         */

        /*  --------------------------------------------------------------------
         *  Unmap the GPP virtual address
         *  --------------------------------------------------------------------
         */
        tmpStatus = DSP_UnmapAddressFromGpp (dspObj) ;
        if (DSP_FAILED (tmpStatus) && DSP_SUCCEEDED (status)) {
            status = tmpStatus ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("DSP_Finalize", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Start
 *
 *  @desc   Causes DSP to start execution from the given DSP address.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Start (IN ProcessorId dspId, IN DspObject *  dspObj, IN Uint32 dspAddr)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("DSP_Start", dspId, dspObj, dspAddr) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        status = DSP_MmuInit (dspId,
                              dspObj->numMmuEntries,
                              dspObj->mmuTable) ;

        if (DSP_SUCCEEDED (status)) {
            status = DSP_Run (dspId, dspAddr) ;
        }
        else {
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            TRC_0PRINT (TRC_LEVEL1, "DSP started !\n") ;
        }
        else {
            SET_FAILURE_REASON ;
            TRC_0PRINT (TRC_LEVEL7, "DSP couldn't be started !\n") ;
        }
    }

    TRC_1LEAVE ("DSP_Start", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Stop
 *
 *  @desc   Stops execution on DSP.
 *          DSP transitions to STOPPED state after successful completion.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Stop (IN ProcessorId dspId, IN DspObject *  dspObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("DSP_Stop", dspId, dspObj) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Reset the DSP and the DSP interface.
         *  --------------------------------------------------------------------
         */
        HAL_ControlDsp          (&(halInfo [dspId].halObject), RSTCT_Reset) ;
        HAL_ControlDspInterface (&(halInfo [dspId].halObject), RSTCT_Reset) ;

        /*  --------------------------------------------------------------------
         *  To prevent failure on loading DSP executable multiple times,
         *  reset API size register to 0xFFFF if it was set to 0x0000 earlier.
         *  --------------------------------------------------------------------
         */
        if (HAL_MpuiGetSize (&(halInfo [dspId].halObject)) == 0) {
            HAL_MpuiSetSize (&(halInfo [dspId].halObject), 0xFFFF) ;
        }

        /*  --------------------------------------------------------------------
         *  Release the DSP interface.
         *  --------------------------------------------------------------------
         */
        HAL_ControlDspInterface (&(halInfo [dspId].halObject), RSTCT_Release) ;

        /*  --------------------------------------------------------------------
         *  Write a 'self loop' on DSP
         *  --------------------------------------------------------------------
         */
        status = DSP_WriteSelfLoop (dspId) ;

        if (DSP_SUCCEEDED (status)) {
            TRC_0PRINT (TRC_LEVEL1, "DSP stopped !\n") ;
        }
        else {
            SET_FAILURE_REASON ;
            TRC_0PRINT (TRC_LEVEL7, "DSP couldn't be stopped !\n") ;
        }
    }

    TRC_1LEAVE ("DSP_Stop", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Idle
 *
 *  @desc   Puts DSP in self loop.
 *          This function provides hook for power handling. Instead of writing
 *          the 'self loop' in DSP reset vector, power handling can be written
 *          as part of 'idle code'.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Idle (IN ProcessorId dspId, IN DspObject * dspObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("DSP_Idle", dspId, dspObj) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Write self loop to the DSP
         *  --------------------------------------------------------------------
         */
        HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Reset) ;

        status = DSP_WriteSelfLoop (dspId) ;
        if (DSP_SUCCEEDED (status)) {
            HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Release) ;
        }
        else {
            TRC_0PRINT (TRC_LEVEL7, "Couldn't write idle code !\n") ;
            SET_FAILURE_REASON ;
        }
    }

    TRC_1LEAVE ("DSP_Idle", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_EnableInterrupt
 *
 *  @desc   Enables the specified interrupt for communication with DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_EnableInterrupt (IN ProcessorId         dspId,
                     IN DspObject *         dspObj,
                     IN InterruptObject *   intInfo)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("DSP_EnableInterrupt", dspId, dspObj, intInfo) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        if (intInfo->intId == OMAP_MAILBOX_INT1) {
            HAL_MailboxIntEnable (&(halInfo [dspId].halObject),
                                  LOW_LEVEL_SENSITIVE) ;
        }
    }

    TRC_1LEAVE ("DSP_EnableInterrupt", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_DisableInterrupt
 *
 *  @desc   Disables the specified interrupt for communication with DSP.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_DisableInterrupt (IN ProcessorId        dspId,
                      IN DspObject *        dspObj,
                      IN InterruptObject *  intInfo)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_3ENTER ("DSP_DisableInterrupt", dspId, dspObj, intInfo) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        if (intInfo->intId == OMAP_MAILBOX_INT1) {
            HAL_MailboxIntDisable (&(halInfo [dspId].halObject)) ;
        }
    }

    TRC_1LEAVE ("DSP_DisableInterrupt", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Interrupt
 *
 *  @desc   Interrupt the DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Interrupt (IN ProcessorId       dspId,
               IN DspObject *       dspObj,
               IN InterruptObject * intInfo,
               IN Pvoid             arg)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_4ENTER ("DSP_Interrupt", dspId, dspObj, intInfo, arg) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        if (intInfo->intId == OMAP_MAILBOX_INT1) {
            HAL_MailboxInterruptDSP (&(halInfo [dspId].halObject),
                                     *(Uint16 *) arg) ;
#if defined (DDSP_PROFILE)
            dspObj->dspStats->intsGppToDsp += 1 ;
#endif /* defined (DDSP_PROFILE) */
        }
    }

    TRC_1LEAVE ("DSP_Interrupt", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_ClearInterrupt
 *
 *  @desc   Clears the DSP interrupt.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_ClearInterrupt (IN  ProcessorId         dspId,
                    IN  DspObject *         dspObj,
                    IN  InterruptObject *   intInfo,
                    OUT Pvoid               retVal)
{
    DSP_STATUS status = DSP_SOK ;

    /*  ------------------------------------------------------------------------
     *  Uncomment the statement below, if prints are allowed in interrupt
     *  context.
     *  ------------------------------------------------------------------------
     */
    /* TRC_4ENTER ("DSP_ClearInterrupt", dspId, dspObj, intInfo, retVal) ; */

    /* Initialize the return value */
    if (retVal == NULL) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else {
        *(Uint16 *) retVal = 0 ;

        if ((IS_VALID_PROCID (dspId) == FALSE) || (dspObj == NULL)) {
            status = DSP_EINVALIDARG ;
            SET_FAILURE_REASON ;
        }
        else if (halInfo [dspId].isSetup == FALSE) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }
        else if (intInfo->intId == OMAP_MAILBOX_INT1) {
            *(Uint16 *) retVal = HAL_MailboxIntClear (
                                    &(halInfo [dspId].halObject)) ;
#if defined (DDSP_PROFILE)
            dspObj->dspStats->intsDspToGpp += 1 ;
#endif /* defined (DDSP_PROFILE) */
        }
    }

    /*  ------------------------------------------------------------------------
     *  Uncomment the statement below, if prints are allowed in interrupt
     *  context.
     *  ------------------------------------------------------------------------
     */
   /* TRC_1LEAVE ("DSP_ClearInterrupt", status) ; */

    return status ;
}


/** ============================================================================
 *  @func   DSP_Read
 *
 *  @desc   Read data from DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Read (IN  ProcessorId  dspId,
          IN  DspObject *  dspObj,
          IN  Uint32       dspAddr,
          IN  Endianism    endianInfo,
          OUT Uint32 *     numBytes,
          OUT Uint8 *      buffer)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_6ENTER ("DSP_Read",
                dspId,
                dspObj,
                dspAddr,
                endianInfo,
                numBytes,
                buffer) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (numBytes != NULL) ;
    DBC_Require (buffer != NULL) ;

    if (   (IS_VALID_PROCID (dspId) == FALSE)
        || (dspObj == NULL)
        || (numBytes == NULL)
        || (buffer == NULL)
        || (   (endianInfo != Endianism_Big)
            && (endianInfo != Endianism_Little)
            && (endianInfo != Endianism_Default))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        HAL_ReadDspData (&(halInfo [dspId].halObject),
                         endianInfo,
                         buffer,
                         dspAddr,
                         *numBytes) ;
#if defined (DDSP_PROFILE)
        dspObj->dspStats->dataDspToGpp += *numBytes ;
#endif /* defined (DDSP_PROFILE) */
    }

    TRC_1LEAVE ("DSP_Read", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Write
 *
 *  @desc   Write data to DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Write (IN ProcessorId dspId,
           IN DspObject * dspObj,
           IN Uint32      dspAddr,
           IN Endianism   endianInfo,
           IN Uint32      numBytes,
           IN Uint8 *     buffer)
{
    DSP_STATUS  status  = DSP_SOK         ;
    Uint32      gppAddr = ADDRMAP_INVALID ;

    TRC_6ENTER ("DSP_Write",
                dspId,
                dspObj,
                dspAddr,
                endianInfo,
                numBytes,
                buffer) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;
    DBC_Require (dspObj != NULL) ;
    DBC_Require (buffer != NULL) ;

    if (   (IS_VALID_PROCID (dspId) == FALSE)
        || (dspObj == NULL)
        || (buffer == NULL)
        || (   (endianInfo != Endianism_Big)
            && (endianInfo != Endianism_Little)
            && (endianInfo != Endianism_Default))) {
        status = DSP_EINVALIDARG ;
        SET_FAILURE_REASON ;
    }
    else if (halInfo [dspId].isSetup == FALSE) {
        status = DSP_EFAIL ;
        SET_FAILURE_REASON ;
    }
    else {
        /*  --------------------------------------------------------------------
         *  Check if 'dspAddr' lies in 'Self Loop Area' in SARAM
         *  --------------------------------------------------------------------
         */
        if ((dspAddr + numBytes) > DSP_UNRESET_VECTOR &&
             dspAddr             < ADDR_SELFLOOP_END ) {
            status = DSP_EFAIL ;
            SET_FAILURE_REASON ;
        }

        if (DSP_SUCCEEDED (status)) {
            if (IS_RANGE_VALID (dspAddr,
                                ADDR_DARAM_START,
                                ADDR_DARAM_END)) {
                /*  ------------------------------------------------------------
                 *  Write to DARAM
                 *  ------------------------------------------------------------
                 */
                HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Release) ;
                HAL_WriteDspData (&(halInfo [dspId].halObject),
                                  endianInfo,
                                  buffer,
                                  dspAddr,
                                  numBytes) ;
                HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Reset) ;
            }
            else if (IS_RANGE_VALID (dspAddr,
                                     ADDR_SARAM_START,
                                     ADDR_SARAM_END)) {
                /*  ------------------------------------------------------------
                 *  Write to SARAM
                 *  ------------------------------------------------------------
                 */
                HAL_WriteDspData (&(halInfo [dspId].halObject),
                                  endianInfo,
                                  buffer,
                                  dspAddr,
                                  numBytes) ;
            }
            else {
                /*  ------------------------------------------------------------
                 *  Write to External Memory
                 *  ------------------------------------------------------------
                 */
                status = DSP_GetAddressMappedInGpp (dspObj, dspAddr, &gppAddr) ;
                if (DSP_SUCCEEDED (status)) {
                    HAL_WriteExtDspData (&(halInfo [dspId].halObject),
                                         endianInfo,
                                         buffer,
                                         gppAddr,
                                         numBytes) ;
                }
                else {
                    SET_FAILURE_REASON ;
                }
            }
        }

#if defined (DDSP_PROFILE)
        if (DSP_SUCCEEDED (status)) {
            dspObj->dspStats->dataGppToDsp += numBytes ;
        }
#endif /* defined (DDSP_PROFILE) */
    }

    TRC_1LEAVE ("DSP_Write", status) ;

    return status ;
}


/** ============================================================================
 *  @func   DSP_Control
 *
 *  @desc   Hook for performing device dependent control operation.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Control (IN  ProcessorId dspId,
             IN  DspObject * dspObj,
             IN  Int32       cmd,
             OPT Pvoid       arg)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_4ENTER ("DSP_Control", dspId, dspObj, cmd, arg) ;

    status = DSP_ENOTIMPL ;

    TRC_1LEAVE ("DSP_Control", status) ;

    return status ;
}


#if defined (DDSP_PROFILE)
/** ============================================================================
 *  @func   DSP_Instrument
 *
 *  @desc   Gets the instrumentation information related to the specified
 *          DSP object.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
DSP_STATUS
DSP_Instrument (IN DspObject * dspObj, OUT DspStats * retVal)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_2ENTER ("DSP_Instrument", dspObj, retVal) ;

    *retVal = *dspObj->dspStats ;

    TRC_1LEAVE ("DSP_Instrument", status) ;

    return status ;
}

#endif /* if defined (DDSP_PROFILE) */


#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @func   DSP_Debug
 *
 *  @desc   Prints out debug information of the specified DSP object.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
DSP_Debug (IN DspObject * dspObj)
{
    DSP_STATUS status = DSP_SOK ;

    TRC_1ENTER ("DSP_Debug", dspObj) ;

    DBC_Require (dspObj != NULL) ;

    if (dspObj == NULL) {
        status = DSP_EINVALIDARG ;
        TRC_0PRINT (TRC_LEVEL7, "Invalid Argument\n") ;
        SET_FAILURE_REASON ;
    }
    else {
        TRC_0PRINT (TRC_LEVEL4, "DspObj-->") ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    dspName         = %s\n",   dspObj->dspName) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    dspArch         = 0x%x\n", dspObj->dspArch) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    execName        = %s\n",   dspObj->execName) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    loaderInterface = 0x%x\n", dspObj->loaderInterface) ;
#if defined (CHNL_COMPONENT)
        TRC_1PRINT (TRC_LEVEL4,
                    "    linkTable       = %d\n",   dspObj->linkTable) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    numLinks        = %d\n",   dspObj->numLinks) ;
#endif /* if defined (CHNL_COMPONENT) */
        TRC_1PRINT (TRC_LEVEL4,
                    "    autoStart       = %d\n",   dspObj->autoStart) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    resetVector     = 0x%x\n",  dspObj->resetVector) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    wordSize        = %d\n",   dspObj->wordSize) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    endian          = %d\n",   dspObj->endian) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    mmuFlag         = %d\n",   dspObj->mmuFlag) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    mmuTable        = %d\n",   dspObj->mmuTable) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    numMmuEntries   = %d\n",   dspObj->numMmuEntries) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    interface       = 0x%x\n", dspObj->interface) ;
        TRC_1PRINT (TRC_LEVEL4,
                    "    addrMapInGpp    = %d\n",   dspObj->addrMapInGpp) ;
    }

    TRC_1LEAVE ("DSP_Debug", status) ;
}
#endif /* if defined (DDSP_DEBUG) */


/*  ----------------------------------------------------------------------------
 *  @func   DSP_Run
 *
 *  @desc   Start DSP execution from given address.
 *          This function writes a 'GOTO' instruction with specified address
 *          at the unreset vector and releases the DSP.
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_Run (IN ProcessorId dspId, IN Uint32 startAddr)
{
    DSP_STATUS status         = DSP_SOK ;
    Uint16     startAddrHi    = 0x6A00  ;
    Uint16     startAddrLo    = 0x0000  ;
    Uint16     startAddrTemp1 = 0x0000  ;
    Uint16     startAddrTemp2 = 0x0000  ;
    Uint16     noOp           = 0x2020  ;

    TRC_2ENTER ("DSP_Run", dspId, startAddr) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    /*  ------------------------------------------------------------------------
     *  Reset the DSP
     *  ------------------------------------------------------------------------
     */
    HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Reset) ;

    if (startAddr != DSP_UNRESET_VECTOR) {
        /*  --------------------------------------------------------------------
         *  Write the 'GOTO' instruction at DSP unreset vector.
         *  --------------------------------------------------------------------
         */
        startAddrHi = (Uint16) (startAddrHi + (startAddr >> 16)) ;
        startAddrLo = (Uint16) startAddr ;

        startAddrTemp1 = (Uint16) ((startAddrHi >> 8) & 0x00ff) ;
        startAddrTemp1 = (Uint16) (  startAddrTemp1
                                   + ((startAddrHi << 8) & 0xff00)) ;
        startAddrHi     = startAddrTemp1 ;

        startAddrTemp1 = (Uint16) ((startAddrLo >> 8) & 0x00ff) ;
        startAddrTemp2 = (Uint16) ((startAddrLo << 8) & 0xff00) ;
        startAddrLo    = (Uint16) (startAddrTemp1 + startAddrTemp2) ;

        HAL_WriteDspData (&(halInfo [dspId].halObject),
                          Endianism_Default,
                          (Uint8 *) &startAddrHi,
                          DSP_UNRESET_VECTOR,
                          sizeof (startAddrHi)) ;

        HAL_WriteDspData (&(halInfo [dspId].halObject),
                          Endianism_Default,
                          (Uint8 *) &startAddrLo,
                          (DSP_UNRESET_VECTOR + 2),
                          sizeof (startAddrLo)) ;

        HAL_WriteDspData (&(halInfo [dspId].halObject),
                          Endianism_Default,
                          (Uint8 *) &noOp,
                          (DSP_UNRESET_VECTOR + 4),
                          sizeof (noOp)) ;
    }

    /*  ------------------------------------------------------------------------
     *  Release the DSP
     *  ------------------------------------------------------------------------
     */
    HAL_ControlDsp (&(halInfo [dspId].halObject), RSTCT_Release) ;

    TRC_1LEAVE ("DSP_Run", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   DSP_MmuInit
 *
 *  @desc   Initialize the DSP MMU.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_MmuInit (IN ProcessorId     dspId,
             IN Uint32          numEntries,
             IN DspMmuEntry *   mmuTable)
{
    DSP_STATUS   status           = DSP_SOK ;
    CONST Uint8  mmuLockBaseValue = 0       ;
    Uint32       i                          ;

    TRC_3ENTER ("DSP_MmuInit", dspId, numEntries, mmuTable) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    HAL_ControlDspMmu (&(halInfo [dspId].halObject), MMU_Disable)    ;
    HAL_ControlDspMmu (&(halInfo [dspId].halObject), MMU_Reset)      ;
    HAL_ControlDsp    (&(halInfo [dspId].halObject), RSTCT_Reset)    ;
    HAL_ControlDspMmu (&(halInfo [dspId].halObject), MMU_Release)    ;
    HAL_ControlDspMmu (&(halInfo [dspId].halObject), MMU_DisableWtl) ;

    for (i = 0 ; i < numEntries ; i++) {
        HAL_DspMmuTlbEntry (&(halInfo [dspId].halObject),
                            mmuTable [i].physicalAddress,
                            mmuTable [i].virtualAddress,
                            (MMU_SLST) (mmuTable [i].size),
                            (MMU_AP) (mmuTable [i].access),
                            (Char8) (mmuLockBaseValue),
                            mmuTable [i].entry,
                            (MMU_PRESERVED) (mmuTable [i].preserve)) ;
    }

    HAL_ControlDspMmu (&(halInfo [dspId].halObject), MMU_Enable) ;
    HAL_ControlDsp    (&(halInfo [dspId].halObject), RSTCT_Release) ;

    TRC_1LEAVE ("DSP_MmuInit", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   DSP_MapAddressInGpp
 *
 *  @desc   Initialize the MMU table entries for the DSP.
 *
 *  @modif  dspObj
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_MapAddressInGpp (IN OUT DspObject * dspObj)
{
    DSP_STATUS status = DSP_SOK ;

    MemMapInfo mapInfo ;
    Uint32     i       ;

    TRC_1ENTER ("DSP_MapAddressInGpp", dspObj) ;

    DBC_Require (dspObj != NULL) ;

    for (i = 0 ; (i < dspObj->numMmuEntries) && DSP_SUCCEEDED (status) ; i++) {
        if (dspObj->mmuTable [i].mapInGpp == TRUE) {
            mapInfo.src  = dspObj->mmuTable [i].physicalAddress ;
            mapInfo.size = DSP_MmuSizeMap [dspObj->mmuTable [i].size] ;

            status = MEM_Map (&mapInfo) ;
            if (DSP_SUCCEEDED (status)) {
                dspObj->addrMapInGpp [i] = mapInfo.dst ;
            }
            else {
                SET_FAILURE_REASON ;
                DSP_UnmapAddressFromGpp (dspObj) ;
            }
        }
        else {
            dspObj->addrMapInGpp [i] = ADDRMAP_INVALID ;
        }
    }

    TRC_1LEAVE ("DSP_MapAddressInGpp", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   DSP_UnmapAddressFromGpp
 *
 *  @desc   Initialize the MMU table entries for the DSP.
 *
 *  @modif  dspObj
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_UnmapAddressFromGpp (IN OUT DspObject * dspObj)
{
    DSP_STATUS status = DSP_SOK ;

    MemUnmapInfo unmapInfo ;
    Uint32     i ;

    TRC_1ENTER ("DSP_UnmapAddressFromGpp", dspObj) ;

    DBC_Require (dspObj != NULL) ;

    for (i = 0 ; i < dspObj->numMmuEntries ; i++) {
        if (dspObj->addrMapInGpp [i] != ADDRMAP_INVALID) {
            unmapInfo.addr = dspObj->addrMapInGpp [i] ;
            if (unmapInfo.addr != 0) {
                status = MEM_Unmap (&unmapInfo) ;
                if (DSP_FAILED (status)) {
                    SET_FAILURE_REASON ;
                }
            }
        }
    }

    TRC_1LEAVE ("DSP_UnmapAddressFromGpp", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   DSP_GetAddressMappedInGpp
 *
 *  @desc   Get the GPP address corresponding to the specified address in DSP
 *          space.
 *
 *  @modif  None
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_GetAddressMappedInGpp (IN  DspObject * dspObj,
                           IN  Uint32      dspAddr,
                           OUT Uint32 *    gppAddr)
{
    DSP_STATUS    status   = DSP_SOK ;
    Bool          found    = FALSE   ;
    DspMmuEntry * mmuEntry = NULL    ;

    Uint32        i ;

    TRC_3ENTER ("DSP_GetAddressMappedInGpp", dspObj, dspAddr, gppAddr) ;

    DBC_Require (dspObj != NULL) ;
    DBC_Require (gppAddr != NULL) ;

    for (i = 0 ; (found == FALSE) && (i < dspObj->numMmuEntries) ; i++) {
        if (dspObj->addrMapInGpp [i] != ADDRMAP_INVALID) {
            mmuEntry = &(dspObj->mmuTable [i]) ;
            if (IS_RANGE_VALID (dspAddr,
                                mmuEntry->virtualAddress,
                                (  mmuEntry->virtualAddress
                                 + DSP_MmuSizeMap [mmuEntry->size]))) {
                found = TRUE ;
                *gppAddr =   dspAddr
                           - mmuEntry->virtualAddress
                           + dspObj->addrMapInGpp [i] ;
            }
        }
    }

    if (found == FALSE) {
        status = DSP_ERANGE ;
        SET_FAILURE_REASON ;
    }

    TRC_1LEAVE ("DSP_GetAddressMappedInGpp", status) ;

    return status ;
}


/*  ----------------------------------------------------------------------------
 *  @func   DSP_WriteSelfLoop
 *
 *  @desc   Writes self loop in SARAM area (ADDR_SELFLOOP_SARAM).
 *
 *  @modif  None.
 *  ----------------------------------------------------------------------------
 */
STATIC
NORMAL_API
DSP_STATUS
DSP_WriteSelfLoop (IN ProcessorId dspId)
{
    DSP_STATUS  status  = DSP_SOK ;

    TRC_1ENTER ("DSP_WriteSelfLoop", dspId) ;

    DBC_Require (IS_VALID_PROCID (dspId)) ;
    DBC_Require (halInfo [dspId].isSetup == TRUE) ;

    HAL_WriteDspData (&(halInfo [dspId].halObject),
                      Endianism_Default,
                      (Uint8 *) SelfLoopInSARAM,
                      ADDR_SELFLOOP_SARAM,
                      sizeof (SelfLoopInSARAM)) ;

    TRC_1LEAVE ("DSP_WriteSelfLoop", status) ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
