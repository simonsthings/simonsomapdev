/** @mainpage TIMER CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the TIMER module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given TIMER module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular TIMER Module. The CSL developer should use his discretion designing
 * new APIs or extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# CSL-001-DES, CSL 3.x Design Specification DocumentVersion 1.02
 *
 */

/** @file csl_timer.h
 *
 * @brief    Header file for functional layer of CSL
 *
 * Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 * Modification 1
 *    - modified on: 15/04/2004
 *    - reason: Created the sources
 *
 * @date 15th Apr, 2004
 * @author Amruth Tadas.
 *
 */
#ifndef _CSL_TIMER_H_
#define _CSL_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_timer.h>

/**
 * @brief Enumeration for Reload mode
 */
typedef enum {
    CSL_TIMER_LOADMODE_ONESHOT         = CSL_TIMER_CNTL_AR_ONESHOT, /**< One shot mode */
    CSL_TIMER_LOADMODE_RELOAD          = CSL_TIMER_CNTL_AR_RELOAD   /**< Auto Reload mode */
} CSL_TimerLoadMode;

/**
 * @brief Emulation setting
 */
typedef enum {
    CSL_TIMER_EMUMODE_STOP             = CSL_TIMER_CNTL_FREE_SUSPEND,   /**< Stop on emulation suspend */
    CSL_TIMER_EMUMODE_RUNFREE          = CSL_TIMER_CNTL_FREE_FREEMODE   /**< Unchanged on emulation suspend */
} CSL_TimerEmuMode;

/**
 * @brief External clock enable/disable settings
 */
typedef enum {
    CSL_TIMER_EXTCLOCK_DISABLE     = CSL_TIMER_CNTL_CLKEN_DISABLE,  /**< External clock disabled */
    CSL_TIMER_EXTCLOCK_ENABLE      = CSL_TIMER_CNTL_CLKEN_ENABLE    /**< External clock enabled */
} CSL_TimerExtClock;

/**
 * @brief   Prescale value
 */
typedef enum {
    CSL_TIMER_PRESCALE_CLKBY2               = CSL_TIMER_CNTL_PTV_CLKBY2,    /**< Prescale by 2 */
    CSL_TIMER_PRESCALE_CLKBY4               = CSL_TIMER_CNTL_PTV_CLKBY4,    /**< Prescale by 4 */
    CSL_TIMER_PRESCALE_CLKBY8               = CSL_TIMER_CNTL_PTV_CLKBY8,    /**< Prescale by 8 */
    CSL_TIMER_PRESCALE_CLKBY16              = CSL_TIMER_CNTL_PTV_CLKBY16,   /**< Prescale by 16 */
    CSL_TIMER_PRESCALE_CLKBY32              = CSL_TIMER_CNTL_PTV_CLKBY32,   /**< Prescale by 32 */
    CSL_TIMER_PRESCALE_CLKBY64              = CSL_TIMER_CNTL_PTV_CLKBY64,   /**< Prescale by 64 */
    CSL_TIMER_PRESCALE_CLKBY128             = CSL_TIMER_CNTL_PTV_CLKBY128,  /**< Prescale by 128 */
    CSL_TIMER_PRESCALE_CLKBY256             = CSL_TIMER_CNTL_PTV_CLKBY256   /**< Prescale by 256 */
} CSL_TimerPrescale;

/**
 * @brief   Enumeration of the control commands
 *
 * These are the control commands that could be used with
 * CSL_timerHwControl(..). Some of the commands expect an
 * argument as documented along-side the description of
 * the command.
 *
 */
typedef enum {
    CSL_TIMER_CMD_START,
        /**<
         * @brief   Start the timer
         * @param   None
         */
    CSL_TIMER_CMD_STOP,
        /**<
         * @brief   Stop the timer
         * @param   None
         */
    CSL_TIMER_CMD_SETCOUNT,
        /**<
         * @brief   Sets the load-value
         * @param   CSL_TimerCount
         */
    CSL_TIMER_CMD_REGRESET,
        /**<
         * @brief   Resets registers to power-on defaults
         * @param   None
         */
    CSL_TIMER_CMD_SETPRESCALE,
        /**<
         * @brief   Modify the prescale
         * @param   CSL_TimerPrescale
         */
    CSL_TIMER_CMD_MODEONESHOT,
        /**<
         * @brief   Change mode to One Shot
         * @param   None
         */
    CSL_TIMER_CMD_MODERELOAD,
        /**<
         * @brief   Change mode to Auto Reload
         * @param   None
         */
    CSL_TIMER_CMD_EMUSTOP,
        /**<
         * @brief   Configure to stop on encountering a emulator breakpoint
         * @param   None
         */
    CSL_TIMER_CMD_EMURUNFREE,
        /**<
         * @brief   Configure to run-free, even on encountering a emulator breakpoint
         * @param   None
         */
    CSL_TIMER_CMD_EXTCLKDISABLE,
        /**<
         * @brief   Disable use of external clock
         * @param   None
         */
    CSL_TIMER_CMD_EXTCLKENABLE
        /**<
         * @brief   Enable use of external clock
         * @param   None
         */
} CSL_TimerHwControlCmd;



/**
 * @brief   Enumeration of the queries
 *
 * These are the queries that could be used with CSL_timerGetHwStatus(..).
 * The queries return a value through the object pointed to by the pointer
 * that it takes as an argument. The argument supported by the query is
 * documented along-side the description of the query.
 */
typedef enum {
    CSL_TIMER_QUERY_COUNT,
    /**<
     * @brief   Retrieve the current count value
     * @param   (CSL_TimerCount *)
     */
    CSL_TIMER_QUERY_PRESCALE,
    /**<
     * @brief   Retrieve prescale value
     * @param   (CSL_TimerPrescale *)
     */
    CSL_TIMER_QUERY_LOADMODE,
    /**<
     * @brief   Retrieve current timer load mode
     * @param   (CSL_TimerLoadMode *)
     */
    CSL_TIMER_QUERY_EMUMODE,
    /**<
     * @brief   Retrieve the current emulation mode
     * @param   (CSL_TimerEmuMode *)
     */
    CSL_TIMER_QUERY_EXTCLOCK,
    /**<
     * @brief   Retrieve whether external clock is enabled or disabled
     * @param   (CSL_TimerExtClock *)
     */
    CSL_TIMER_QUERY_ISRUNNING
    /**<
     * @brief   Check whether timer is running
     * @param   (Bool *)
     */
} CSL_TimerHwStatusQuery;


/**<
 * @brief   Type abstraction for timer count
 */
typedef Uint32 CSL_TimerCount;


/**
 * @brief   Default values for the setup-parameters
 */
#define CSL_TIMER_HWSETUP_DEFAULTS {    \
        0xFFFFFFFF,                             \
        CSL_TIMER_PRESCALE_CLKBY2,              \
        CSL_TIMER_LOADMODE_ONESHOT,             \
    CSL_TIMER_EMUMODE_STOP,                 \
        CSL_TIMER_EXTCLOCK_DISABLE              \
}

/**
 * @brief   The setup-structure
 *
 * Used to configure the timer using CSL_timerHwSetup(..)
 */
typedef struct CSL_TimerHwSetup {
    CSL_TimerCount          loadVal;    /**< Load Value (reload count) */
    CSL_TimerPrescale       prescale;   /**< Prescale */
    CSL_TimerLoadMode       loadMode;   /**< Load Mode */
    CSL_TimerEmuMode        emuMode;    /**< Emulation Control */
    CSL_TimerExtClock       extClock;   /**< External Clock Enable/Disable */
} CSL_TimerHwSetup;

/**
 * @brief   The timer handle object
 *
 * This object is used refenced by the handle to identify the timer instance.
 */
typedef struct CSL_TimerObj {
    CSL_OpenMode            openMode;   /**< Mode in which the Timer was opened */
    CSL_Uid                 uid;        /**< The unique identifier of the Timer instance */
    CSL_Xio                 xio;        /**< Resource sharing identifier */
    CSL_TimerRegsOvly       regs;       /**< Pointer to the registers of the specific Timer */
    CSL_TimerNum            timerNum;   /**< Instance number of the timer */
} CSL_TimerObj;

/**
 * @brief   The timer handle
 *
 * This is returned by the CSL_timerOpen(..) API. The handle is used
 * to identify the instance of interest in all TIMER calls.
 */
typedef struct CSL_TimerObj *   CSL_TimerHandle;


/** The open call sets up the data structures for the particular instance of
 *  TIMER device. The device can be re-opened anytime after it has been normally
 *  closed if so required. TIMER Hardware setup will be performed at the end of
 *  the open call only if the HwSetup Pointer supplied was non- NULL. The handle
 *  returned by this call is input as an essential argument for rest of the APIs
 *  described for this module.
 *
 *  <b> Usage Constraints: </b>
 *  The CSL system as well as TIMER must be successfully initialized
 *  via @a CSL_sysInit() and @a CSL_timerInit() before calling this
 *  function. Memory for the @a CSL_timerObj must be allocated outside
 *  this call. This object must be retained while usage of this peripheral.
 *
 *  @b Example:
 *  @verbatim

          CSL_TimerObj     timerObj;
          CSL_TimerHwSetup timerSetup;
          CSL_Status       status;
                ...
          hTimer = CSL_timerOpen(&timerObj,
                          CSL_TIMER_1,
                          CSL_EXCLUSIVE,
                          &timerSetup,
                          &status);
   @endverbatim
 *
 * @return returns a handle @a CSL_TimerHandle to the requested instance of
 * TIMER if the call is successful, otherwise, a @a NULL is returned.
 *
 */
 CSL_TimerHandle  CSL_timerOpen(
    CSL_TimerObj              *hTimerObj,
    CSL_TimerNum              timerNum,
    CSL_OpenMode              openMode,
    CSL_TimerHwSetup          *hwSetup,
    CSL_Status                *status
);

/**  The Init call initializes the TIMER .
 *
 * @return returns the status of the operation (see @a CSL_Status)
 */
 CSL_Status  CSL_timerInit(
    void
);

/**  The Close call releases the resource and appropriate shared pins.
 *
 * <b> Usage Constraints: </b>
 * Both @a CSL_timerInit() and @a CSL_timerOpen() must be called successfully
 * in that order before @a CSL_timerClose() can be called.
 *
 * @b Example:
 * @verbatim


   CSL_TimerHandle hTimer;
   ...
   CSL_timerClose(hTimer);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */

 CSL_Status  CSL_timerClose(
    CSL_TimerHandle           hTimer
);

/** This function initializes the device registers with the appropriate values
 *  provided through the HwSetup Data structure. This function needs to be called
 *  only if the HwSetup Structure was not previously passed through the Open call.
 *  After the Setup is completed, the serial device is ready for data transfer.
 *  For information passed through the HwSetup Data structure refer
 *  @a CSL_TimerHwSetup.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_timerInit() and @a CSL_timerOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_TimerHandle hTimer;
     CSL_TimerHwSetup hwSetup = CSL_TIMER_HWSETUP_DEFAULTS;
     CSL_timerHwSetup(hTimer, &hwSetup);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */

 CSL_Status  CSL_timerHwSetup(
    CSL_TimerHandle           hTimer,
    CSL_TimerHwSetup          *setup
);

/** Control operations for the TIMER.  For a particular control operation, the
 *  pointer to the corresponding data type needs to be passed as argument HwControl
 *  function Call. All the arguments (Structure elements included) passed to  the
 *  HwControl function are inputs. For the list of commands supported and argument
 *  type that can be @a void* casted & passed with a particular command refer to
 *  @a CSL_TimerHwControlCmd.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_timerInit() and @a CSL_timerOpen() must be called successfully
 *  in that order before @a CSL_timerHwControl() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_TimerHwControlCmd
 *
 * @b Example:
 * @verbatim
       CSL_TimerHandle hTimer;
       CSL_Status status;
       ...
       status = CSL_timerHwControl(hTimer, CSL_TIMER_CMD_START, NULL);
   @endverbatim
 *
 *  @return returns the status of the operation (see @a CSL_Status)
 *
 */

CSL_Status  CSL_timerHwControl(
    CSL_TimerHandle           hTimer,
    CSL_TimerHwControlCmd cmd,
    void                      *cmdArg
);


/** This function is used to read the current device configuration, status flags
 *  and the value present associated registers. Following table details the various
 *  status queries supported and the associated data structure to record the response.
 *  User should allocate memory for the said data type and pass its pointer as an
 *  unadorned void* argument to the status query call. For details about the various
 *  status queries supported and the associated data structure to record the response,
 *  refer to @a CSL_TimerHwStatusQuery
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_timerInit() and @a CSL_timerOpen() must be called successfully
 *  in that order before @a CSL_timerGetHwStatus() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_TimerHwStatusQuery
 *
 * @b Example:
 * @verbatim
          CSL_TimerHandle hTimer;
          CSL_Status status;
          Uint16  *response;
           ...
          status = CSL_timerGetHwStatus(hTimer, CSL_TIMER_QUERY_PRESCALE, &cmdArg);
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */

CSL_Status  CSL_timerGetHwStatus(
    CSL_TimerHandle           hTimer,
    CSL_TimerHwStatusQuery    myQuery,
    void                      *response
);

#ifdef __cplusplus
}
#endif


#endif
