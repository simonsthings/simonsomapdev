/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSL_TIMER_H_
#define _CSL_TIMER_H_

#include <csl_types.h>
#include <csl_error.h>
#include <cslr_timer.h>

/**************************************************************************\
* TIMER global macro declarations
\**************************************************************************/


/**************************************************************************\
* TIMER global typedef declarations
\**************************************************************************/
/* Number of TIMER Peripherals. */
typedef enum {
    CSL_TIMER_ANY                  =                 -1,
    CSL_TIMER_1                    =                  0,
    CSL_TIMER_2                    =                  1,
    CSL_TIMER_3                    =                  2
} CSL_TimerNum;

/* Reload mode. */
typedef enum {
    CSL_TIMER_LOADMODE_ONESHOT         = CSL_TIMER_CNTL_AR_ONESHOT,
    CSL_TIMER_LOADMODE_RELOAD          = CSL_TIMER_CNTL_AR_RELOAD
} CSL_TimerLoadMode;

/* Emulation setting. */
typedef enum {
    CSL_TIMER_EMUMODE_STOP             = CSL_TIMER_CNTL_FREE_SUSPEND,
    CSL_TIMER_EMUMODE_RUNFREE          = CSL_TIMER_CNTL_FREE_FREEMODE
} CSL_TimerEmuMode;

/* External clock enable/disable settings. */
typedef enum {
    CSL_TIMER_EXTCLOCK_DISABLE     = CSL_TIMER_CNTL_CLKEN_DISABLE,
    CSL_TIMER_EXTCLOCK_ENABLE      = CSL_TIMER_CNTL_CLKEN_ENABLE
} CSL_TimerExtClock;

/* Prescale value. */
typedef enum {
    CSL_TIMER_PRESCALE_CLKBY2               = CSL_TIMER_CNTL_PTV_CLKBY2,
    CSL_TIMER_PRESCALE_CLKBY4               = CSL_TIMER_CNTL_PTV_CLKBY4,
    CSL_TIMER_PRESCALE_CLKBY8               = CSL_TIMER_CNTL_PTV_CLKBY8,
    CSL_TIMER_PRESCALE_CLKBY16              = CSL_TIMER_CNTL_PTV_CLKBY16,
    CSL_TIMER_PRESCALE_CLKBY32              = CSL_TIMER_CNTL_PTV_CLKBY32,
    CSL_TIMER_PRESCALE_CLKBY64              = CSL_TIMER_CNTL_PTV_CLKBY64,
    CSL_TIMER_PRESCALE_CLKBY128             = CSL_TIMER_CNTL_PTV_CLKBY128,
    CSL_TIMER_PRESCALE_CLKBY256             = CSL_TIMER_CNTL_PTV_CLKBY256
} CSL_TimerPrescale;

/* Enum's used to "Control" the timer. */
typedef enum {
    CSL_TIMER_CMD_START, 
    CSL_TIMER_CMD_STOP, 
    CSL_TIMER_CMD_SETCOUNT, 
    /* set Register values to Power-on default. */
    CSL_TIMER_CMD_REGRESET,
    CSL_TIMER_CMD_SETPRESCALE,
    CSL_TIMER_CMD_MODEONESHOT,
    CSL_TIMER_CMD_MODERELOAD,
    CSL_TIMER_CMD_EMUSTOP,
    CSL_TIMER_CMD_EMURUNFREE,
    CSL_TIMER_CMD_EXTCLKDISABLE,
    CSL_TIMER_CMD_EXTCLKENABLE
} CSL_TimerHwControlCmd;

/* Enum's used to "Query" the Timer h/w. */
typedef enum {
    CSL_TIMER_QUERY_COUNT,
    CSL_TIMER_QUERY_PRESCALE,
    CSL_TIMER_QUERY_LOADMODE,
    CSL_TIMER_QUERY_EMUMODE,
    CSL_TIMER_QUERY_EXTCLOCK,
    CSL_TIMER_QUERY_ISRUNNING
} CSL_TimerHwStatusQuery;


typedef Uint32 CSL_TimerCount;



#define CSL_TIMER_HWSETUP_DEFAULTS {    \
        0xFFFFFFFF,                     \
        CSL_TIMER_PRESCALE_CLKBY2,              \
        CSL_TIMER_LOADMODE_ONESHOT,             \
        CSL_TIMER_EMUMODE_STOP,         \
        CSL_TIMER_EXTCLOCK_DISABLE      \
}

/* Structure used to setup timer device */
typedef struct CSL_TimerHwSetup {
    CSL_TimerCount          loadVal;
    CSL_TimerPrescale       prescale;
    CSL_TimerLoadMode       loadMode;
    CSL_TimerEmuMode        emuMode;
    CSL_TimerExtClock       extClock;
} CSL_TimerHwSetup;

/* TIMER types */
typedef struct CSL_TimerObj {
    CSL_OpenMode            openMode;   /* Always the first member of <PERI>Obj */
    CSL_Uid                 uid;        /* Always the second member of <PERI>Obj */
    CSL_Xio                 xio;        /* Always the third member of <PERI>Obj */
    CSL_TimerRegsOvly       regs;
    CSL_TimerNum            timerNum;
} CSL_TimerObj;

typedef struct CSL_TimerObj *CSL_TimerHandle;

/**************************************************************************\
* TIMER global function declarations
\**************************************************************************/

/*
 * ======================================================
 *   @func   csl_timerOpen
 *   @desc
 *         Function opens the TIMER Obj, for the specified TIMER NUMBER.
 *
 *
 *   @arg  hTimerObj
 *              Handle to the TIMER Obj
 *   @arg  timerNum
 *              Tells the TIMER to be open, CSL_TIMER_ANY symbolizes for ANY available TIMER.
 *   @arg  openMode
 *              Pin Mode: EXL or SHARED
 *   @arg  hwSetup
 *              H/W setup parameters, if NULL, h/w setup is ignored.
 *   @arg  status
 *              Return Status.
 *   @ret  Handle
 *
 *          Valid Timer handle will be returned if status value is equal to CSL_SOK.
 *          if status returned is
 *          CSL_ESYS_OVFL: No Timer Available.
 *          CSL_ESYS_INUSE: Requested Timer is in Use.
 *
 *
 *   @eg
 *     hTimer = CSL_timerOpen(hTimer, CSL_TIMER_ANY, CSL_EXCLUSIVE, hwSetup, &status);
 *
 *
 * ======================================================
*/

 CSL_TimerHandle  CSL_timerOpen(
    CSL_TimerObj              *hTimerObj,
    CSL_TimerNum              timerNum,
    CSL_OpenMode              openMode,
    CSL_TimerHwSetup          *hwSetup,
    CSL_Status                *status
);

/*
 * ======================================================
 *   @func   csl_timerInit
 *   @desc
 *         Function initializes the TIMER services.
 *         All TIMER services will be active after this call.
 *
 *
 *   @ret  CSL_Status
 *
 *          CSL_SOK.
 *
 *
 *   @eg
 *     csl_timerInit(); // All TIMER services are active after this call.
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_timerInit(
    void
);

/*
 * ======================================================
 *   @func   csl_timerClose
 *   @desc
 *         Function closes the TIMER specified by its Handle.
 *
 *
 *   @arg  hTimer
 *              The handle to the DSP Timer instance.
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful, desired operation is done.
 *          CSL_ESYS_BADHANDLE : Handle pass is INVALID.
 *
 *
 *   @eg
 *     status = CSL_timerClose(hTimer);
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_timerClose(
    CSL_TimerHandle           hTimer
);

/*
 * ======================================================
 *   @func   csl_timerHwSetup
 *   @desc
 *         Function sets the specified TIMER H/W.
 *
 *
 *   @arg  hTimer
 *              The handle to the DSP Timer instance.
 *   @arg  setup
 *              The Setup structure used to setup timer device has the following members
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful, desired operation is done.
 *          CSL_ESYS_INVPARAMS : Parameters passed is NULL.
 *
 *
 *   @eg
 *     status = CSL_timerHwSetup(hTimer, &setup);
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_timerHwSetup(
    CSL_TimerHandle           hTimer,
    CSL_TimerHwSetup          *setup
);

/*
 * ======================================================
 *   @func   csl_timerHwControl
 *   @desc
 *         Function controls the TIMER peripheral specified by the Hanldle.
 *         Operation done is based on the "cmd".
 *
 *
 *   @arg  hTimer
 *              The handle to the DSP Timer instance.
 *   @arg  cmd
 *              Control Command which needs to performed.
 *   @arg  cmdArg
 *              Argument related to the COMMAND.
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful, desired operation is done.
 *          CSL_ESYS_INVCMD : Control Command is not supported.
 *
 *
 *   @eg
 *     status = CSL_timerHwControl(hTimer, CSL_TIMER_CMD_START, NULL);
 *     This will START the TIMER represented by the HANDLE - hTimer.
 *
 * ======================================================
*/

 CSL_Status  CSL_timerHwControl(
    CSL_TimerHandle           hTimer,
    CSL_TimerHwControlCmd cmd,
    void                      *cmdArg
);

/*
 * ======================================================
 *   @func   csl_timerGetHwStatus
 *   @desc
 *         Function query's the H/W - TIMER specified by the Handle, command is
 *         is specified by the CSL_timerHwStatusQuery.
 *
 *
 *   @arg  hTimer
 *              Handle to Timer Object, if its NULL than CSL_ESYS_BADHANDLE
 *   @arg  myQuery
 *              user's Query
 *   @arg  response
 *              Slot for saving response to query placed.
 *   @ret  CSL_Status
 *
 *          CSL_SOK : Successful Retreive, (* cmdArg) will have the desired value.
 *          CSL_ESYS_INVQUERY : Query Command is not supported.
 *
 *
 *   @eg
 *     status = CSL_timerGetHwStatus(hTimer, CSL_TIMER_QUERY_PRESCALE, &cmdArg);
 *
 *
 * ======================================================
*/

 CSL_Status  CSL_timerGetHwStatus(
    CSL_TimerHandle           hTimer,
    CSL_TimerHwStatusQuery    myQuery,
    void                      *response
);


#endif
