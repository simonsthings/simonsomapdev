/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef _CSL_INTC_H_
#define _CSL_INTC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <cslr.h>
#include <csl_error.h>
#include <csl_types.h>

/* Interrupt Event Counts */
#define _CSL_INTC_EVENTID__IVPDCNT          (16)
#define _CSL_INTC_EVENTID__IVPHCNT          (16)
#define _CSL_INTC_EVENTID__INTC20CNT        (16)
#define _CSL_INTC_EVENTID__INTC21CNT        (64)

/**
 * @brief   Count of the number of interrupt-events
 */
#define CSL_INTC_EVENTID_CNT        \
    (_CSL_INTC_EVENTID__IVPDCNT + _CSL_INTC_EVENTID__IVPHCNT + _CSL_INTC_EVENTID__INTC20CNT + _CSL_INTC_EVENTID__INTC21CNT)


/**
 * @brief   Indicates there is no associated event-handler
 */
#define CSL_INTC_EVTHANDLER_NONE        ((CSL_IntcEventHandlerRecord *) 0)


/**
 * @brief   Invalid handle
 */
#define CSL_INTC_BADHANDLE        (0)



/**
 * @brief   Event enable state
 */
typedef Uint8   CSL_IntcEventEnableState;


/**
 * @brief   Global Interrupt enable state
 */
typedef Uint8   CSL_IntcGlobalEnableState;


/**
 * @brief   Interrupt Vector IDs
 */
typedef enum {
    CSL_INTC_VECTID_DEFAULT =   0,
    CSL_INTC_VECTID_INVALID =   -1
} CSL_IntcVectId;

/**
 * @brief   Interrupt Event IDs
 */
typedef enum {

    _CSL_INTC_EVENTID__IVPDSTART        =   0,

    CSL_INTC_EVENTID_RESET              =   _CSL_INTC_EVENTID__IVPDSTART + 0,   /**< Reset */
    CSL_INTC_EVENTID_NMI                =   _CSL_INTC_EVENTID__IVPDSTART + 1,   /**< Non maskable interrupt */
    CSL_INTC_EVENTID_EMUTEST            =   _CSL_INTC_EVENTID__IVPDSTART + 2,   /**< Emulator/test interrupt */
    CSL_INTC_EVENTID_L20INTHFIQ         =   _CSL_INTC_EVENTID__IVPDSTART + 3,   /**< Level 2.0 interrupt handler */
    CSL_INTC_EVENTID_TCABORT            =   _CSL_INTC_EVENTID__IVPDSTART + 4,   /**< TC_ABORT interrupt */
    CSL_INTC_EVENTID_MBX1               =   _CSL_INTC_EVENTID__IVPDSTART + 5,   /**< Mailbox 1 */
    CSL_INTC_EVENTID_L21INTHFIQ         =   _CSL_INTC_EVENTID__IVPDSTART + 6,   /**< Level 2.1 interrupt handler */
    CSL_INTC_EVENTID_IRQ2GPIO1          =   _CSL_INTC_EVENTID__IVPDSTART + 7,   /**< IRQ2 GPIO1 */
    CSL_INTC_EVENTID_TIMER3             =   _CSL_INTC_EVENTID__IVPDSTART + 8,   /**< DSP Timer3 */
    CSL_INTC_EVENTID_DMACH1             =   _CSL_INTC_EVENTID__IVPDSTART + 9,   /**< DMA Channel 1 */
    CSL_INTC_EVENTID_MPUIINT            =   _CSL_INTC_EVENTID__IVPDSTART + 10,  /**< MPUI */
    CSL_INTC_EVENTID_UART3              =   _CSL_INTC_EVENTID__IVPDSTART + 12,  /**< UART3 */
    CSL_INTC_EVENTID_WDTIM              =   _CSL_INTC_EVENTID__IVPDSTART + 13,  /**< Watchdog */
    CSL_INTC_EVENTID_DMACH4             =   _CSL_INTC_EVENTID__IVPDSTART + 14,  /**< DMA Channel 4 */
    CSL_INTC_EVENTID_DMACH5             =   _CSL_INTC_EVENTID__IVPDSTART + 15,  /**< DMA Channel 5 */

    _CSL_INTC_EVENTID__IVPDEND          =   _CSL_INTC_EVENTID__IVPDSTART + _CSL_INTC_EVENTID__IVPDCNT - 1,

    _CSL_INTC_EVENTID__IVPHSTART        =   _CSL_INTC_EVENTID__IVPDEND + 1,

    CSL_INTC_EVENTID_STIO               =   _CSL_INTC_EVENTID__IVPHSTART + 0,   /**< STIO */
    CSL_INTC_EVENTID_L21INTHIRQ         =   _CSL_INTC_EVENTID__IVPHSTART + 1,   /**< Level 2.1 IRQ */
    CSL_INTC_EVENTID_DMACH0             =   _CSL_INTC_EVENTID__IVPHSTART + 2,   /**< DMA Channel 0 */
    CSL_INTC_EVENTID_MBX2               =   _CSL_INTC_EVENTID__IVPHSTART + 3,   /**< Mailbox 2 */
    CSL_INTC_EVENTID_DMACH2             =   _CSL_INTC_EVENTID__IVPHSTART + 4,   /**< DMA Channel 2 */
    CSL_INTC_EVENTID_DMACH3             =   _CSL_INTC_EVENTID__IVPHSTART + 5,   /**< DMA Channel 3 */
    CSL_INTC_EVENTID_TIMER2             =   _CSL_INTC_EVENTID__IVPHSTART + 6,   /**< DSP Timer 2 */
    CSL_INTC_EVENTID_TIMER1             =   _CSL_INTC_EVENTID__IVPHSTART + 7,   /**< DSP Timer 1 */
    CSL_INTC_EVENTID_BERR               =   _CSL_INTC_EVENTID__IVPHSTART + 8,   /**< Bus Error */
    CSL_INTC_EVENTID_DLOG               =   _CSL_INTC_EVENTID__IVPHSTART + 9,   /**< Emulator interrupt DLOG */
    CSL_INTC_EVENTID_RTOS               =   _CSL_INTC_EVENTID__IVPHSTART + 10,  /**< Emulator interrupt RTOS */
    CSL_INTC_EVENTID_SINT28             =   _CSL_INTC_EVENTID__IVPHSTART + 11,  /**< Software Interrupt #28 */
    CSL_INTC_EVENTID_SINT29             =   _CSL_INTC_EVENTID__IVPHSTART + 12,  /**< Software Interrupt #29 */
    CSL_INTC_EVENTID_SINT30             =   _CSL_INTC_EVENTID__IVPHSTART + 13,  /**< Software Interrupt #30 */
    CSL_INTC_EVENTID_SINT31             =   _CSL_INTC_EVENTID__IVPHSTART + 14,  /**< Software Interrupt #31 */
    CSL_INTC_EVENTID_SINT32             =   _CSL_INTC_EVENTID__IVPHSTART + 15,  /**< Software Interrupt #32 */

    _CSL_INTC_EVENTID__IVPHEND          =   _CSL_INTC_EVENTID__IVPHSTART + _CSL_INTC_EVENTID__IVPHCNT - 1,

    _CSL_INTC_EVENTID__INTC20START      =   _CSL_INTC_EVENTID__IVPHEND + 1,

    CSL_INTC_EVENTID_MCBSP3_TX          =   _CSL_INTC_EVENTID__INTC20START + 0, /**< McBSP3 TX */
    CSL_INTC_EVENTID_MCBSP3_RX          =   _CSL_INTC_EVENTID__INTC20START + 1, /**< McBSP3 RX */
    CSL_INTC_EVENTID_MCBSP1_TX          =   _CSL_INTC_EVENTID__INTC20START + 2, /**< McBSP1 TX */
    CSL_INTC_EVENTID_MCBSP1_RX          =   _CSL_INTC_EVENTID__INTC20START + 3, /**< McBSP1 RX */
    CSL_INTC_EVENTID_UART2              =   _CSL_INTC_EVENTID__INTC20START + 4, /**< UART2 */
    CSL_INTC_EVENTID_UART1              =   _CSL_INTC_EVENTID__INTC20START + 5, /**< UART1 */
    CSL_INTC_EVENTID_MCSI1TX            =   _CSL_INTC_EVENTID__INTC20START + 6, /**< MCSI1 TX */
    CSL_INTC_EVENTID_MCSI1RX            =   _CSL_INTC_EVENTID__INTC20START + 7, /**< MCSI1 RX */
    CSL_INTC_EVENTID_MCSI2TX            =   _CSL_INTC_EVENTID__INTC20START + 8, /**< MCSI2 TX */
    CSL_INTC_EVENTID_MCSI2RX            =   _CSL_INTC_EVENTID__INTC20START + 9, /**< MCSI2 RX */
    CSL_INTC_EVENTID_MCSI1FERR          =   _CSL_INTC_EVENTID__INTC20START + 10,    /**< MCSI1 Frame Error */
    CSL_INTC_EVENTID_MCSI2FERR          =   _CSL_INTC_EVENTID__INTC20START + 11,    /**< MCSI2 Frame Error */
    CSL_INTC_EVENTID_IRQ2GPIO2          =   _CSL_INTC_EVENTID__INTC20START + 12,    /**< IRQ2_GPIO2 */
    CSL_INTC_EVENTID_IRQ2GPIO3          =   _CSL_INTC_EVENTID__INTC20START + 13,    /**< IRQ2_GPIO3 */
    CSL_INTC_EVENTID_IRQ2GPIO4          =   _CSL_INTC_EVENTID__INTC20START + 14,    /**< IRQ2_GPIO4 */
    CSL_INTC_EVENTID_I2C                =   _CSL_INTC_EVENTID__INTC20START + 15,    /**< I2C */

    _CSL_INTC_EVENTID__INTC20END        =   _CSL_INTC_EVENTID__INTC20START + _CSL_INTC_EVENTID__INTC20CNT - 1,

    _CSL_INTC_EVENTID__INTC21START      =   _CSL_INTC_EVENTID__INTC20END + 1,


    CSL_INTC_EVENTID_NFC                =   _CSL_INTC_EVENTID__INTC21START + 0, /**< NAND Flash */
    CSL_INTC_EVENTID_GPTIMER1           =   _CSL_INTC_EVENTID__INTC21START + 1, /**< GPTIMER1 */
    CSL_INTC_EVENTID_GPTIMER2           =   _CSL_INTC_EVENTID__INTC21START + 2, /**< GPTIMER2 */
    CSL_INTC_EVENTID_GPTIMER3           =   _CSL_INTC_EVENTID__INTC21START + 3, /**< GPTIMER3 */
    CSL_INTC_EVENTID_GPTIMER4           =   _CSL_INTC_EVENTID__INTC21START + 4, /**< GPTIMER4 */
    CSL_INTC_EVENTID_GPTIMER5           =   _CSL_INTC_EVENTID__INTC21START + 5, /**< GPTIMER5 */
    CSL_INTC_EVENTID_GPTIMER6           =   _CSL_INTC_EVENTID__INTC21START + 6, /**< GPTIMER6 */
    CSL_INTC_EVENTID_GPTIMER7           =   _CSL_INTC_EVENTID__INTC21START + 7, /**< GPTIMER7 */
    CSL_INTC_EVENTID_GPTIMER8           =   _CSL_INTC_EVENTID__INTC21START + 8, /**< GPTIMER8 */
    CSL_INTC_EVENTID_STIGIE             =   _CSL_INTC_EVENTID__INTC21START + 9, /**< STI global */
    CSL_INTC_EVENTID_MCBSP2TX           =   _CSL_INTC_EVENTID__INTC21START + 10,    /**< McBSP2 TX */
    CSL_INTC_EVENTID_MCBSP2RX           =   _CSL_INTC_EVENTID__INTC21START + 11,    /**< McBSP2 RX */
    CSL_INTC_EVENTID_MCSI1RSTINT        =   _CSL_INTC_EVENTID__INTC21START + 12,    /**< MCSI1_RST_INT */
    CSL_INTC_EVENTID_MCSI2RSTINT        =   _CSL_INTC_EVENTID__INTC21START + 13,    /**< MCSI2_RST_INT */
    CSL_INTC_EVENTID_MMCSDIO2           =   _CSL_INTC_EVENTID__INTC21START + 14,    /**< MMC/SDIO2 */
    CSL_INTC_EVENTID_SPI                =   _CSL_INTC_EVENTID__INTC21START + 15,    /**< SPI */
    CSL_INTC_EVENTID_SSTCH1FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 16,    /**< SST FIFO empty (channel 1) */
    CSL_INTC_EVENTID_SSTCH1FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 17,    /**< SST FIFO full (channel 1) */
    CSL_INTC_EVENTID_SSRCH1OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 18,    /**< SSR overrun (channel 1) */
    CSL_INTC_EVENTID_SSTCH2FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 19,    /**< SST FIFO empty (channel 2) */
    CSL_INTC_EVENTID_SSTCH2FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 20,    /**< SST FIFO full (channel 2) */
    CSL_INTC_EVENTID_SSRCH2OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 21,    /**< SSR overrun (channel 2) */
    CSL_INTC_EVENTID_SSTCH3FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 22,    /**< SST FIFO empty (channel 3) */
    CSL_INTC_EVENTID_SSTCH3FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 23,    /**< SST FIFO full (channel 3) */
    CSL_INTC_EVENTID_SSRCH3OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 24,    /**< SSR overrun (channel 3) */
    CSL_INTC_EVENTID_SSTCH4FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 25,    /**< SST FIFO empty (channel 4) */
    CSL_INTC_EVENTID_SSTCH4FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 26,    /**< SST FIFO full (channel 4) */
    CSL_INTC_EVENTID_SSRCH4OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 27,    /**< SSR overrun (channel 4) */
    CSL_INTC_EVENTID_SSTCH5FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 28,    /**< SST FIFO empty (channel 5) */
    CSL_INTC_EVENTID_SSTCH5FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 29,    /**< SST FIFO full (channel 5) */
    CSL_INTC_EVENTID_SSRCH5OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 30,    /**< SSR overrun (channel 5) */
    CSL_INTC_EVENTID_SSTCH6FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 31,    /**< SST FIFO empty (channel 6) */
    CSL_INTC_EVENTID_SSTCH6FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 32,    /**< SST FIFO full (channel 6) */
    CSL_INTC_EVENTID_SSRCH6OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 33,    /**< SSR overrun (channel 6) */
    CSL_INTC_EVENTID_SSTCH7FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 34,    /**< SST FIFO empty (channel 7) */
    CSL_INTC_EVENTID_SSTCH7FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 35,    /**< SST FIFO full (channel 7) */
    CSL_INTC_EVENTID_SSRCH7OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 36,    /**< SSR overrun (channel 7) */
    CSL_INTC_EVENTID_SSTCH0FIFOEMPTY    =   _CSL_INTC_EVENTID__INTC21START + 37,    /**< SST FIFO empty (channel 0) */
    CSL_INTC_EVENTID_SSTCH0FIFOFULL     =   _CSL_INTC_EVENTID__INTC21START + 38,    /**< SST FIFO full (channel 0) */
    CSL_INTC_EVENTID_SSRCH0OVERRUN      =   _CSL_INTC_EVENTID__INTC21START + 39,    /**< SSR overrun (channel 0) */
    CSL_INTC_EVENTID_GDDLCH0            =   _CSL_INTC_EVENTID__INTC21START + 40,    /**< GDD_LCH0 */
    CSL_INTC_EVENTID_GDDLCH1            =   _CSL_INTC_EVENTID__INTC21START + 41,    /**< GDD_LCH1 */
    CSL_INTC_EVENTID_GDDLCH2            =   _CSL_INTC_EVENTID__INTC21START + 42,    /**< GDD_LCH2 */
    CSL_INTC_EVENTID_GDDLCH3            =   _CSL_INTC_EVENTID__INTC21START + 43,    /**< GDD_LCH3 */
    CSL_INTC_EVENTID_GDDLCH4            =   _CSL_INTC_EVENTID__INTC21START + 44,    /**< GDD_LCH4 */
    CSL_INTC_EVENTID_GDDLCH5            =   _CSL_INTC_EVENTID__INTC21START + 45,    /**< GDD_LCH5 */
    CSL_INTC_EVENTID_GDDLCH6            =   _CSL_INTC_EVENTID__INTC21START + 46,    /**< GDD_LCH6 */
    CSL_INTC_EVENTID_GDDLCH7            =   _CSL_INTC_EVENTID__INTC21START + 47,    /**< GDD_LCH7 */

    _CSL_INTC_EVENTID__INTC21END        =   _CSL_INTC_EVENTID__INTC21START + _CSL_INTC_EVENTID__INTC21CNT - 1,

    CSL_INTC_EVENTID_INVALID            =   -1                                  /**< Invalid Event-ID */
} CSL_IntcEventId;




/**
 * @brief   Enumeration of the control commands
 *
 * These are the control commands that could be used with
 * CSL_intcHwControl(..). Some of the commands expect an
 * argument as documented along-side the description of
 * the command.
 */
typedef enum {
    CSL_INTC_CMD_EVTENABLE,
        /**<
         * @brief   Enables the event
         * @param   None
         */
    CSL_INTC_CMD_EVTDISABLE,
        /**<
         * @brief   Disables the event
         * @param   None
         */
    CSL_INTC_CMD_EVTCLEAR,
        /**<
         * @brief   Clears the event (if pending)
         * @param   None
         */
    CSL_INTC_CMD_EVTSET,
        /**<
         * @brief   Sets the event (software triggering)
         * @param   None
         */
    CSL_INTC_CMD_SETPRIORITY,
        /**<
         * @brief   Modifies the interrupt priority
         * @param   CSL_IntcPriority
         */
    CSL_INTC_CMD_SETTYPE,
        /**<
        * @brief    Modify the interrupt type (route)
        * @param    CSL_IntcType
        */
    CSL_INTC_CMD_SETSENSE,
        /**<
         * @brief   Modify the interrupt sensitivity
         * @param   CSL_IntcSense
         */
    CSL_INTC_CMD_SETPACE
        /**<
         * @brief   Not Supported
         * @param   Not Applicable
         */
} CSL_IntcHwControlCmd;


/**
 * @brief   Enumeration of the queries
 *
 * These are the queries that could be used with CSL_intcGetHwStatus(..).
 * The queries return a value through the object pointed to by the pointer
 * that it takes as an argument. The argument supported by the query is
 * documented along-side the description of the query.
 */
typedef enum {
    CSL_INTC_QUERY_PRIORITY,
        /**<
         * @brief   Retrieves the interrupt priority
         * @param   (CSL_IntcPriority *)
         */

    CSL_INTC_QUERY_TYPE,
        /**<
         * @brief   Returns the type (route)
         * @param   (CSL_IntcType *)
         */

    CSL_INTC_QUERY_SENSE,
        /**<
         * @brief   Retrieves the interrupt sensitivity
         * @param   (CSL_IntcSense *)
         */

    CSL_INTC_QUERY_ISEVENTPENDING
        /**<
         * @brief   Checks if event is pending
         * @param   (Bool *)
         */

} CSL_IntcHwStatusQuery;



/**
 * @brief   Interrupt Priority
 */
typedef enum {
    CSL_INTC_PRIORITY_DEFAULT   = 0  /**< Default priority */
} CSL_IntcPriority;

/**
 * @brief   Interrupt Type (Routing)
 */
typedef enum {
    CSL_INTC_TYPE_IRQ   = 0,    /**< IRQ -- normal interrupt request */
    CSL_INTC_TYPE_FIQ   = 1     /**< FIQ -- fast interrupt request */
} CSL_IntcType;

/**
 * @brief   Interrupt Sense
 */
typedef enum {
    CSL_INTC_SENSE_FALL_EDGE    =   0,  /**< Falling Edge */
    CSL_INTC_SENSE_ACTIVE_LOW   =   1   /**< Active Low */
} CSL_IntcSense;


/**
 * @brief   Event Handler pointer
 *
 * Event handlers ought to conform to this type
 */
typedef void (* CSL_IntcEventHandler)(void *);

/**
 * @brief   Event Handler Record
 *
 * Used to set-up the event-handler using CSL_intcPlugEventHandler(..)
 */
typedef struct CSL_IntcEventHandlerRecord {
    CSL_IntcEventHandler    handler;    /**< pointer to the event handler */
    void *                  arg;        /**< the argument to be passed to the
                                          handler when it is invoked */
} CSL_IntcEventHandlerRecord;


/**
 * @brief   The setup-structure
 *
 * Used to configure the interrupt controller for an event using
 * CSL_intcHwSetup(..)
 */
typedef struct CSL_IntcHwSetup {
    CSL_IntcPriority    priority;   /**< The interrupt priority */
    CSL_IntcType        type;       /**< The interrupt type (routing) */
    CSL_IntcSense       sense;      /**< The interrupt sensing */
} CSL_IntcHwSetup;

/**
 * @brief   Default values for the setup-parameters
 */
#define CSL_INTC_HWSETUP_DEFAULTS {                 \
    (CSL_IntcPriority) CSL_INTC_PRIORITY_DEFAULT,   \
    (CSL_IntcType)     CSL_INTC_TYPE_IRQ,           \
    (CSL_IntcSense)    CSL_INTC_SENSE_FALL_EDGE     \
}

/**
 * @brief   The interrupt handle object
 *
 * This object is used refenced by the handle to identify the event.
 */
typedef struct CSL_IntcObj {
    CSL_IntcEventId eventId;    /**< The event-id */
    CSL_IntcVectId  vectId;     /**< The vector-id */
    void *          reserved;   /**< Reserved for the future */
} CSL_IntcObj;


/**
 * @brief   The interrupt handle
 *
 * This is returned by the CSL_intcOpen(..) API. The handle is used
 * to identify the event of interest in all INTC calls.
 */
typedef struct CSL_IntcObj *    CSL_IntcHandle;


/**
 * @brief   Intialize INTC module
 *
 * This API performs any module-specific initialization. CSL_intcInit(..)
 * must be invoked before calling any other API in the INTC module.
 *
 * @b Example:
 * @verbatim
      CSL_sysInit( );
      if (CSL_intcInit( ) != CSL_SOK) {
        // module initialization failed! //
      }
  
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcInit (
        void
);






/**
 * @brief   Allocates an event for use
 *
 * The API would reserve an interrupt-event for use. It returns
 * a valid handle to the event only if the event is not currently
 * allocated. The user could release the event after use by calling
 * CSL_intcClose(..). The CSL-object ('intcObj') that the user
 * passes would be used to store information pertaining handle.
 *
 * @b Example:
 * @verbatim
        CSL_IntcObj     intcObj;
        CSL_IntcHandle  hIntc;
        CSL_Status      openStatus;
  
        hIntc = CSL_intcOpen(&intcObj, CSL_INTC_EVENTID_TIMER3,
                    CSL_INTC_VECTID_DEFAULT, NULL, &openStatus);
        if (openStatus != CSL_SOK) {
            // open failed //
        }
   @endverbatim
 *
 * @return  Handle identifying the event
 */

CSL_IntcHandle
    CSL_intcOpen (
        CSL_IntcObj *       intcObj,    /**< pointer to the CSL-object allocated by the user */
        CSL_IntcEventId     eventId,    /**< the event-id of the interrupt */
        CSL_IntcVectId      vectId,     /**< the interrupt-vector */
        CSL_IntcHwSetup *   setup,      /**< (optional) pointer to an optional setup-structure */
        CSL_Status *        status      /**< (optional) pointer to a variable that would receive the status */
);


/**
 * @brief   Releases an allocated event
 *
 * CSL_intcClose(..) must be called to release an event that has
 * been previously allocated with a call to CSL_intcOpen(..).
 *
 * @b Example:
 * @verbatim
        if (CSL_intcClose(hIntc) != CSL_SOK) {
            // close failed! //
        }
   @endverbatim
 *
 * @return  CSL_SOK on success; CSL_ESYS_FAIL on failure.
 */

CSL_Status
    CSL_intcClose (
        CSL_IntcHandle      hIntc   /**< Handle to an event; see CSL_intcOpen(..) */
);




/**
 * @brief   Sets up the interrupt controller for a particular event
 *
 * CSL_intcHwSetup(..) API is used to configure the interrupt controller
 * for the event identified by the handle. The user must instantiate
 * and initialize a setup-structure with appropriate configuration
 * parameters before passing it to the function.
 *
 * @b Example:
 * @verbatim
        CSL_IntcHwSetup setup = CSL_INTC_HWSETUP_DEFAULTS;
  
        setup.priority = CSL_INTC_PRIORITY_DEFAULT;
        setup.sense = CSL_INTC_SENSE_FALL_EDGE;
        setup.type = CSL_INTC_TYPE_IRQ;
  
        CSL_intcHwSetup(hIntc, &setup);
  
   @endverbatim
 *
 * @return
*/

CSL_Status
    CSL_intcHwSetup (
        CSL_IntcHandle      hIntc,  /**< Handle to an event; see CSL_intcOpen(..) */
        CSL_IntcHwSetup *   hwSetup /**< Pointer to a Setup-structure */
);



/**
 * @brief   Perform a control-operation
 *
 * This API is used to invoke any of the supported control-operations
 * supported by the module. Note: Refer to the control-command documentation
 * for details on the parameter (if any) that a specific command accepts.
 *
 * @b Example:
 * @verbatim
        CSL_intcHwControl(hIntc, CSL_INTC_CMD_EVTSET, NULL);
   @endverbatim
 *
 * @return  CSL_SOK on success;
 *          CSL_ESYS_FAIL on failure;
 *          CSL_ESYS_BADHANDLE if the handle is invalid;
 *          CSL_ESYS_NOTSUPPORTED if not supported;
 *          CSL_ESYS_INVCMD on an invalid command.
*/

CSL_Status
    CSL_intcHwControl (
        CSL_IntcHandle          hIntc,      /**< Handle to an event; see CSL_intcOpen(..) */
        CSL_IntcHwControlCmd    command,    /**< Command identifier */
        void *                  commandArg  /**< Optional command parameter */
);




/**
 * @brief   Queries the peripheral for status
 *
 * The CSL_intcGetHwStatus(..) API could be used to retrieve status
 * or configuration information from the peripheral. The user must
 * allocate an object that would hold the retrieved information
 * and pass a pointer to it to the function. The type of the object
 * is specific to the query-command.
 *
 * @b Example:
 * @verbatim
        Bool    evtPending = FALSE;
        while (evtPending == FALSE) {
            CSL_intcGetHwStatus(hIntc, CSL_INTC_QUERY_ISEVENTPENDING, &evtPending);
        }
  
   @endverbatim
 *
 * @return  CSL_SOK on success;
 *          CSL_ESYS_FAIL on failure;
 *          CSL_ESYS_BADHANDLE if the handle is invalid;
 *          CSL_ESYS_NOTSUPPORTED if not supported;
 *          CSL_ESYS_INVQUERY on an invalid query.
*/
CSL_Status
    CSL_intcGetHwStatus (
        CSL_IntcHandle          hIntc,      /**< Handle to an event; see CSL_intcOpen(..) */
        CSL_IntcHwStatusQuery   query,      /**< Query identifier */
        void *                  response    /**< Pointer to an object that would contain the retrieved information */
);





/**
 * @brief   Enable an event
 *
 * The API enables the specified event. If the user wishes to restore
 * the enable-state of the event at a later point of time, they may
 * store the current state using the parameter, which could be used
 * with CSL_intcEventRestore(..).
 * Note: The function directly works on the event and hence it is
 * not necessary to "open" the event to invoke the API.
 *
 * @b Example:
 * @verbatim
        CSL_intcEventEnable(CSL_INTC_EVENTID_UART1, NULL);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcEventEnable (
        CSL_IntcEventId             eventId,    /**< Event-ID of interest */
        CSL_IntcEventEnableState *  prevState   /**< (Optional) Pointer to object that would store current state */
);


/**
 * @brief   Disable an event
 *
 * The API disables the specified event. If the user wishes to restore
 * the enable-state of the event at a later point of time, they may
 * store the current state using the parameter, which could be used
 * with CSL_intcEventRestore(..).
 * Note: The function directly works on the event and hence it is
 * not necessary to "open" the event to invoke the API.
 *
 * @b Example:
 * @verbatim
        CSL_IntcEventEnableState    oldState;
        CSL_intcEventDisable(CSL_INTC_EVENTID_I2C, &oldState);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcEventDisable (
        CSL_IntcEventId             eventId,    /**< Event-ID of interest */
        CSL_IntcEventEnableState *  prevState   /**< (Optional) Pointer to object that would store current state */
);


/**
 * @brief   Restore the event to a previous enabled/disabled state
 *
 * The API restores the specified event to a previous enable-state
 * as recorded by the event-enable state passed as an argument.
 * Note: The function directly works on the event and hence it is
 * not necessary to "open" the event to invoke the API.
 *
 * @b Example:
 * @verbatim
        CSL_intcEventRestore(CSL_INTC_EVENTID_I2C, oldState);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

CSL_Status
    CSL_intcEventRestore (
        CSL_IntcEventId             eventId,    /**< Event-ID of interest */
        CSL_IntcEventEnableState    prevState   /**< Object that contains information about previous state */
);



/**
 * @brief   Globally enable interrupts
 *
 * The API enables the global interrupt by manipulating the processor's
 * global interrupt enable/disable flag. If the user wishes to restore
 * the enable-state at a later point, they may store the current state
 * using the parameter, which could be used with CSL_intcGlobalRestore(..).
 *
 * @b Example:
 * @verbatim
        CSL_intcGlobalEnable(NULL);
   @endverbatim
 *
 * @return  CSL_SOK on success
 */

CSL_Status
    CSL_intcGlobalEnable (
        CSL_IntcGlobalEnableState * prevState   /**< (Optional) Pointer to object that would store current state */
);





/**
 * @brief   Globally disable interrupts
 *
 * The API disables the global interrupt by manipulating the processor's
 * global interrupt enable/disable flag. If the user wishes to restore
 * the enable-state at a later point, they may store the current state
 * using the parameter, which could be used with CSL_intcGlobalRestore(..).
 *
 * @b Example:
 * @verbatim
        CSL_IntcGlobalEnableState   gieState;
  
        CSL_intcGlobalDisable(&gieState);
        // critical-section code //
        CSL_intcGlobalRestore(gieState);
   @endverbatim
 *
 * @return  CSL_SOK on success
 */

CSL_Status
    CSL_intcGlobalDisable (
        CSL_IntcGlobalEnableState * prevState   /**< (Optional) Pointer to object that would store current state */
);





/**
 * @brief   Restore global interrupt enable/disable to a previous state
 *
 * The API restores the global interrupt enable/disable state to a previous
 * state as recorded by the global-event-enable state passed as an argument.
 *
 * @b Example:
 * @verbatim
        CSL_IntcGlobalEnableState   gieState;
  
        CSL_intcGlobalDisable(&gieState);
        // critical-section code //
        CSL_intcGlobalRestore(gieState);
   @endverbatim
 *
 * @return  CSL_SOK on success
*/

void
    CSL_intcGlobalRestore (
        CSL_IntcGlobalEnableState       prevState   /**< Object containing information about previous state */
);



/**
 * @brief   Hooks up and sets up the CSL interrupt dispatchers
 *
 * The user should call CSL_intcDispatcherInit(..) if they
 * wish to make use of the dispatchers built into the CSL INTC
 * module.
 *
 * Note: This API must be called before using
 * CSL_intcPlugEventHandler(..).
 *
 * @b Example:
 * @verbatim
      CSL_sysInit( );
      if (CSL_intcInit( ) != CSL_SOK) {
        // module initialization failed! //
      }
      if (CSL_intcDispatcherInit( ) != CSL_SOK) {
        // CSL dispatcher setting up failed! //
      }
   @endverbatim
 *
 * @return CSL_SOK on success; CSL_ESYS_FAIL on failure.
*/

CSL_Status
    CSL_intcDispatcherInit (
        void
);




/**
 * @brief   Associate an event-handler with an event
 *
 * CSL_intcPlugEventHandler(..) ties an event-handler to an event; so
 * that the occurence of the event, would result in the event-handler
 * being invoked.
 *
 * @b Example:
 * @verbatim
        CSL_IntcEventHandlerRecord  evtHandlerRecord;
  
        evtHandlerRecord.handler = myIsr;
        evtHandlerRecord.arg     = (void *)hTimer;
        CSL_intcPlugEventHandler(hIntc, &evtHandlerRecord);
   @endverbatim
 *
 * @return  Returns the address of the previous handler
*/

CSL_IntcEventHandler
    CSL_intcPlugEventHandler (
        CSL_IntcHandle                  hIntc,
                /**< Handle identying the interrupt-event; see CSL_intcOpen(..) */
        CSL_IntcEventHandlerRecord *    eventHandlerRecord
                /**< Provides the details of the event-handler */
);


/**
 * @brief   Hook up an Interrupt Service Routine
 *
 * The CSL_intcHookIsr(..) hooks up the Interrupt Service Routine
 * to the specified interrupt vector. Note: Unlike CSL_intcPlugEventHandler(..)
 * where the handler is called via the CSL internal dispatcher, in
 * this case the ISR is directly called by the CPU on an interrupt.
 *
 * @b Example:
 * @verbatim
        CSL_intcHookIsr(CSL_INTC_EVENTID_MBX1, (void *)mbxIsr);
   @endverbatim
 */
CSL_Status  CSL_intcHookIsr (
    CSL_IntcEventId       evtId,    /**< Interrupt to hook-up; can be one of the 32 CPU interrupts */
    Uint32                isrAddr   /**< Pointer to the ISR */
);

#ifdef __cplusplus
}
#endif

#endif
