/** ============================================================================
 *  @file   hal_mailbox.c
 *
 *  @path   $(DSPLINK)\gpp\src\hal\OMAP
 *
 *  @desc   Hardware Abstraction Layer for OMAP.
 *          Defines necessary functions for Mailbox Interrupt Handling.
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
#include <errbase.h>
#include <safe.h>

/*  ----------------------------------- Trace & Debug               */
#include <signature.h>
#include <dbc.h>
#include <trc.h>

/*  ----------------------------------- Profiling                   */
/*  ----------------------------------- OSAL Headers                */
#include <print.h>

/*  ----------------------------------- Hardware Abstraction Layer  */
#include <hal_object.h>
#include <hal_inth.h>
#include <hal_mailbox.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif


/** ============================================================================
 *  @macro  COMPONENT_ID
 *
 *  @desc   Component and Subcomponent Identifier.
 *  ============================================================================
 */
#define  COMPONENT_ID       ID_HAL_MAILBOX

/** ============================================================================
 *  @macro  SET_FAILURE_REASON
 *
 *  @desc   Sets failure reason
 *  ============================================================================
 */
#define SET_FAILURE_REASON   GEN_SetReason (status, FID_HAL_MAILBOX, __LINE__)

#if defined (DDSP_DEBUG)
/** ============================================================================
 *  @const  MAX_POLL_COUNT
 *
 *  @desc   For debug mode, indicates the maximum count to wait for interrupt
 *          to be cleared before timing out.
 *  ============================================================================
 */
#define MAX_POLL_COUNT  100000000
#endif /* defined (DDSP_DEBUG) */


/** ============================================================================
 *  @func   HAL_MailboxIntEnable
 *
 *  @desc   Enables the mailbox interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MailboxIntEnable (IN HalObject * halObject,
                      IN Uint32      type)
{
    volatile Uint32 * levelReg ;

    TRC_2ENTER ("HAL_MailboxIntEnable", halObject, type) ;

    DBC_Require (halObject != NULL) ;

    /* Configure the interrupt by writing into ILRx register. */
    levelReg = (Uint32 *) (   (halObject->baseINTH)
                            +  0x1C
                            + (4 * (DSP_MAILBOX1_INT))) ;

    REG32 (levelReg) = (  (INTH_IRQ         << INTH_FIQNIRQ_POSBIT)
                        | (DSP_MAILBOX1_INT << INTH_PRIORITY_POSBIT)
                        | (type             << INTH_SENSITIVE_EDGE_POSBIT)) ;

    /* Clear the ITR register */
    *(Uint32 *) (halObject->baseINTH +
                  INTH_IT_REG_OFFSET +
                  INTH_MASK_IT_REG_OFFSET) &= ~(1 << DSP_MAILBOX1_INT) ;

    TRC_0LEAVE ("HAL_MailboxIntEnable") ;
}


/** ============================================================================
 *  @func   HAL_MailboxIntDisable
 *
 *  @desc   Disables the mailbox interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Void
HAL_MailboxIntDisable (IN HalObject * halObject)
{
    TRC_1ENTER ("HAL_MailboxIntDisable", halObject) ;

    DBC_Require (halObject != NULL) ;

    REG32 (halObject->baseINTH +
           INTH_IT_REG_OFFSET +
           INTH_MASK_IT_REG_OFFSET) |= (1 << DSP_MAILBOX1_INT) ;

    TRC_0LEAVE ("HAL_MailboxIntDisable") ;
}


/** ============================================================================
 *  @func   HAL_MailboxIntClear
 *
 *  @desc   Clears the mailbox interrupt.
 *
 *  @modif  None
 *  ============================================================================
 */
NORMAL_API
Uint16
HAL_MailboxIntClear (IN HalObject * halObject)
{
    Uint16   retVal ;

    /* Read DSP to ARM Mailbox 1 registers */
    retVal = (Uint16)
             (REG16 (halObject->baseMAILBOX + MB_DSP2ARM1B_REG_OFFSET)) ;

    return retVal ;
}


/** ============================================================================
 *  @func   HAL_MailboxInterruptDSP
 *
 *  @desc   Interrupts the DSP.
 *
 *  @modif  None.
 *  ============================================================================
 */
NORMAL_API
Bool
HAL_MailboxInterruptDSP (IN HalObject * halObject, Uint16 intrVal)
{
    Uint16          temp   = 0xF00  ;
    Bool            retVal = TRUE   ;
#if defined (DDSP_DEBUG)
    volatile Uint32 i      = 0      ;
#endif /* if (DDSP_DEBUG) */

    TRC_2ENTER ("HAL_MailboxInterruptDSP", halObject, intrVal) ;

    DBC_Require (halObject != NULL) ;

    /* Check that DSP has cleared previous interrupt */
    while (((  REG16 (halObject->baseMAILBOX + MB_ARM2DSP1_FLAG_REG_OFFSET)
             & MB_ARM2DSP_FLAG) != 0x0000) && (retVal == TRUE)) {
#if defined (DDSP_DEBUG)
       i++ ;
       if (i == MAX_POLL_COUNT) {
           retVal = FALSE ;
       }
#endif /* if (DDSP_DEBUG) */
    }

    if (retVal == TRUE) {
        REG16 (halObject->baseMAILBOX + MB_ARM2DSP1_REG_OFFSET)  = temp ;
        REG16 (halObject->baseMAILBOX + MB_ARM2DSP1B_REG_OFFSET) = intrVal ;
    }

    TRC_0LEAVE ("HAL_MailboxInterruptDSP") ;

    return retVal ;
}


#if defined (__cplusplus)
}
#endif
