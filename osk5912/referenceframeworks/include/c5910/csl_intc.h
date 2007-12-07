/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSL_INTC_H_
#define _CSL_INTC_H_

#include <csl_error.h>
#include <csl_types.h>

/**************************************************************************\
 * Interrupt Level limits.
\**************************************************************************/
// The DSP Interrupt limit specifiers ( 16 Interrupts )
#define CSL_INTC_IVPDINTSTART   (0) //DSP Interrupt list start.
#define CSL_INTC_IVPDINTEND     (15) // DSP Interrupt list end.

// The ARM Interrupt limit specifiers ( 16 Interrupts )
#define CSL_INTC_IVPHINTSTART  (16) //Host Interrupt list start.
#define CSL_INTC_IVPHINTEND    (31) //Host Interrupt list end.

// The Level 2 Interrupt Handler limit specifiers ( 16 Interrupts )
#define CSL_INTC_INTC2INTSTART  (32) //Level 2.0 Interrupt list start.
#define CSL_INTC_INTC2INTEND    (47) //Level 2.0 Interrupt list end.

#define CSL_INTC_EVT_CNT (48)

/**************************************************************************/

#define CSL_INTC_EVTHANDLER_NONE  ((CSL_IntcEventHandlerRecord *) 0)


/**************************************************************************\
* INTC global macro declarations
\**************************************************************************/
/* Start of INTC error codes */
#define CSL_EINTC_INVALID_HANDLE             (CSL_EINTC_FIRST - 1)

/* Handle passed in not valid */
#define CSL_EINTC_INVALID_EVENTID            (CSL_EINTC_FIRST - 2)

/* Event ID is not valid                                        */
#define CSL_EINTC_INVALID_VAL                (CSL_EINTC_FIRST - 3)

/* Invalid value passed to CSL                                  */
#define CSL_EINTC_BAD_INIT                   (CSL_EINTC_FIRST - 4)

/* Operation failed due to bad initialization of CSL's INTC     */
/* module and/or dispatcher                                     */
#define CSL_EINTC_UNSUPPORTED_CMD            (CSL_EINTC_FIRST - 5)

/* An unsupported H/W Control Command was passed                */
#define CSL_EINTC_UNSUPPORTED_QUERY          (CSL_EINTC_FIRST - 6)

#define CSL_INTC_EVTHANDLER_NONE                ((CSL_IntcEventHandlerRecord *) 0)


#define CSL_INTC_EVT_INVALID      (CSL_EINTC_FIRST - 7)
#define CSL_INTC_VECT_INVALID     (CSL_EINTC_FIRST -1)

#define CSL_INTC_CHIP_INT_REG     2
#define CSL_INTC_INT_ACTION       1
#define CSL_INTC_INT_MASK         0x7fffffcL
#define CSL_INTC_BADHANDLE        (0)
/**************************************************************************\
* INTC global typedef declarations
\**************************************************************************/
/* Generic enable/disable                        */
typedef enum {
    CSL_INTC_ACTION_ENABLE                =                  1,
    CSL_INTC_ACTION_DISABLE               =                  0
} CSL_IntcAction;

/* Debug or Normal Interrupt mode                */
typedef enum {
    CSL_INTC_INTERRUPTTYPE_INT                   =                  0,
    CSL_INTC_INTERRUPTTYPE_DBGINT                =                  2
} CSL_IntcInterruptType;

/* Interrupt Enable/Disable State */
typedef enum {
    CSL_INTC_EVENTENABLESTATE_ENABLE          =                  0,
    CSL_INTC_EVENTENABLESTATE_DISABLE         =                  1
} CSL_IntcEventEnableState;

/* Interrupt Enable/Disable State */
typedef enum {
    CSL_INTC_GLOBALENABLESTATE_GBLENABLE          =                  0,
    CSL_INTC_GLOBALENABLESTATE_GBLDISABLE         =                  1
} CSL_IntcGlobalEnableState;

/* DBG/NORMAL interrupt Enable/Disable           */
typedef enum {
    CSL_INTC_INTERRUPTOPTIONS_INTMOD_EN             = (CSL_INTC_EVENTENABLESTATE_ENABLE | CSL_INTC_INTERRUPTTYPE_INT),
    CSL_INTC_INTERRUPTOPTIONS_INTMOD_DIS            = (CSL_INTC_EVENTENABLESTATE_DISABLE | CSL_INTC_INTERRUPTTYPE_INT),
    CSL_INTC_INTERRUPTOPTIONS_INTMOD_DBGEN          = (CSL_INTC_EVENTENABLESTATE_ENABLE | CSL_INTC_INTERRUPTTYPE_DBGINT),
    CSL_INTC_INTERRUPTOPTIONS_INTMOD_DBGDIS         = (CSL_INTC_EVENTENABLESTATE_DISABLE | CSL_INTC_INTERRUPTTYPE_DBGINT)
} CSL_IntcInterruptOptions;

/* typedef for the default Vector Id. */
typedef enum {
    CSL_INTC_VECTID_DEFAULT          =                  0
} CSL_IntcVectId;

/*      The listing of Interrupt source IDs. This table lists all the interrupts
        available on OMAP 5910. */
typedef enum {
    CSL_INTC_EVENTID_RESET                 =                  0,
    CSL_INTC_EVENTID_NMI                   =                  1,
    CSL_INTC_EVENTID_EMTEST                =                  2,
    CSL_INTC_EVENTID_L2INTHFIQ             =                  3,
    CSL_INTC_EVENTID_TCABORT               =                  4,
    CSL_INTC_EVENTID_MBX1                  =                  5,
    CSL_INTC_EVENTID_HSABMBX               =                  6,
    CSL_INTC_EVENTID_GPIO                  =                  7,
    CSL_INTC_EVENTID_TIMER3                =                  8,
    CSL_INTC_EVENTID_DMACH1                =                  9,
    CSL_INTC_EVENTID_MPUIINT               =                 10,
    CSL_INTC_EVENTID_ILLEGAL               =                 11,
    CSL_INTC_EVENTID_UART                  =                 12,
    CSL_INTC_EVENTID_WDTIM                 =                 13,
    CSL_INTC_EVENTID_DMACH4                =                 14,
    CSL_INTC_EVENTID_DMACH5                =                 15,
    CSL_INTC_EVENTID_EMIF                  =                 16,
    CSL_INTC_EVENTID_LOCALBUS              =                 17,
    CSL_INTC_EVENTID_DMACH0                =                 18,
    CSL_INTC_EVENTID_MBX2                  =                 19,
    CSL_INTC_EVENTID_DMACH2                =                 20,
    CSL_INTC_EVENTID_DMACH3                =                 21,
    CSL_INTC_EVENTID_TIMER2                =                 22,
    CSL_INTC_EVENTID_TIMER1                =                 23,
    CSL_INTC_EVENTID_BERR                  =                 24,
    CSL_INTC_EVENTID_DLOG                  =                 25,
    CSL_INTC_EVENTID_RTOS                  =                 26,
    CSL_INTC_EVENTID_SINT28                =                 27,
    CSL_INTC_EVENTID_SINT29                =                 28,
    CSL_INTC_EVENTID_SINT30                =                 29,
    CSL_INTC_EVENTID_SINT31                =                 30,
    CSL_INTC_EVENTID_SINT32                =                 31,
    CSL_INTC_EVENTID_MCBSP3TX              =                 32,
    CSL_INTC_EVENTID_MCBSP3RX              =                 33,
    CSL_INTC_EVENTID_MCBSP1TX              =                 34,
    CSL_INTC_EVENTID_MCBSP1RX              =                 35,
    CSL_INTC_EVENTID_UART2                 =                 36,
    CSL_INTC_EVENTID_UART1                 =                 37,
    CSL_INTC_EVENTID_MCSI1TX               =                 38,
    CSL_INTC_EVENTID_MCSI1RX               =                 39,
    CSL_INTC_EVENTID_MCSI2TX               =                 40,
    CSL_INTC_EVENTID_MCSI2RX               =                 41,
    CSL_INTC_EVENTID_MCSI1                 =                 42,
    CSL_INTC_EVENTID_MCSI2                 =                 43,
    CSL_INTC_EVENTID_EXTPER3               =                 44,
    CSL_INTC_EVENTID_EXTPER4               =                 45,
    CSL_INTC_EVENTID_EXTPER5               =                 46,
    CSL_INTC_EVENTID_EXTPER6               =                 47
} CSL_IntcEventId;

/* Enum for defining the various commands passed */
typedef enum {
    CSL_INTC_CMD_EVTENABLE          =                  0,
    CSL_INTC_CMD_EVTDISABLE,
    CSL_INTC_CMD_EVTCLEAR,
    CSL_INTC_CMD_EVTSET,
    CSL_INTC_CMD_SETPRIORITY,
    CSL_INTC_CMD_SETTYPE,
    CSL_INTC_CMD_SETSENSE,
    CSL_INTC_CMD_SETPACE
} CSL_IntcHwControlCmd;

/* Enum for defining the various queries passed  */
typedef enum {
    CSL_INTC_QUERY_VECTID            =                  0,
    CSL_INTC_QUERY_ISEVENTHANDLED,
    CSL_INTC_QUERY_ISEVENTPENDING,
    CSL_INTC_QUERY_PRIORITY,
    CSL_INTC_QUERY_TYPE,
    CSL_INTC_QUERY_SENSE,
    CSL_INTC_QUERY_PACE,
    CSL_INTC_QUERY_EVTSTATUS
} CSL_IntcHwStatusQuery;

/* Default priority level                        */
typedef enum {
    CSL_INTC_PRIORITY_DEFAULT      =                  0
} CSL_IntcPriority;

/* Interrupt routing                             */
typedef enum {
    CSL_INTC_TYPE_IRQ                   =                  0,
    CSL_INTC_TYPE_FIQ                   =                  1
} CSL_IntcType;

/* Interrupt trigger                             */
typedef enum {
    CSL_INTC_SENSE_FALL_EDGE             =                  0,
    CSL_INTC_SENSE_ACTIVE_LOW            =                  1,
    CSL_INTC_SENSE_RAISE_EDGE,
    CSL_INTC_SENSE_ACTIVE_HIGH
} CSL_IntcSense;

typedef Uint32 CSL_IntcEventState;

// Event Handler type.
typedef void    (*CSL_IntcEventHandler)(void*);


typedef void (*CSL_IntcIsrPtr) ( void );

typedef struct CSL_IntcEventHandlerRecord {
    CSL_IntcEventHandler    handler;
    void                    *arg;
} CSL_IntcEventHandlerRecord;

/* IRQ setup structure                           */
typedef struct CSL_IntcHwSetup {
    CSL_IntcPriority        priority;
    CSL_IntcType            type;
    CSL_IntcSense           sense;
} CSL_IntcHwSetup;

/* CSL_IntcObj structure definition.             */
typedef struct CSL_IntcObj {
    CSL_IntcEventId         eventId;
    CSL_IntcVectId          vectId;
    void                    *reserved;
} CSL_IntcObj;

typedef struct CSL_IntcObj *CSL_IntcHandle;


/* Hw Setup Defaults for the INTC Module */

#define CSL_INTC_HWSETUP_DEFAULTS{                  \
(CSL_IntcPriority) CSL_INTC_PRIORITY_DEFAULT,       \
(CSL_IntcType)     CSL_INTC_TYPE_FIQ,               \
(CSL_IntcSense)    CSL_INTC_SENSE_FALL_EDGE         \
}

/**************************************************************************\
* INTC global function declarations
\**************************************************************************/

/*
 * ======================================================
 *   @func  CSL_intcInit
 *
 *   @desc  This function initialises the INTC module at all levels.
 *
 *   @arg       void
 *
 *   @ret  CSL_Status
 *                              returns the status CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */

CSL_Status  CSL_intcInit( void);

/*
 * ======================================================
 *   @func  CSL_intcDispatcherInit
 *
 *   @desc  Set up top-level IRQ and FIQ dispatchers.
 *
 *   @arg   void
 *
 *   @ret   CSL_Status
 *                              returns the status CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */

CSL_Status  CSL_intcDispatcherInit( void);

/*
 * ======================================================
 *   @func  CSL_intcOpen
 *
 *   @desc  This function opens an instance of the event specified & returns a handle to the same
 *          if and only if the event instance is NOT already in use.
 *
 *   @arg  myIntcObj
 *              INTC Handle object.
 *   @arg  eventId
 *              Event Id of the interrupt.
 *   @arg  vectId
 *              Vector Id of the interrupt.
 *   @arg  mySetup
 *              INTC Setup structure.
 *   @arg  st
 *              Return status
 *
 *   @ret  Handle
 *                              INTC Handle.
 *
 * ======================================================
 */

CSL_IntcHandle  CSL_intcOpen(
    CSL_IntcObj               *myIntcObj,
    CSL_IntcEventId           eventId,
    CSL_IntcVectId            vectId,
    CSL_IntcHwSetup           *mySetup,
    CSL_Status                *st
);

/*
 * ======================================================
 *   @func  CSL_intcHookIsr
 *
 *   @desc  This function is used to hook up the address passed
 *          to the appropriate location in the vector table for
 *          passed interrupt number.
 *
 *   @arg  evtId
 *              interrupt event id
 *   @arg  isrAddr
 *              address of the function to hook up as ISR
 *
 *   @ret  CSL_Status
 *              returns the status CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
*/

CSL_Status  CSL_intcHookIsr(
    Uint32                evtId,
    Uint32                isrAddr
);


/*
 * ======================================================
 *   @func  CSL_intcHwSetup
 *
 *   @desc  Configures an interrupt using setup-structure
 *
 *   @arg  hIntc
 *              Handle corresponding to the event.
 *   @arg  params
 *              Pointer to the IRQ-setup structure.
 *
 *   @ret  CSL_Status
 *                              returns the status CSL_SOK or CSL_ESYS_FAIL.
 * ======================================================
 */

CSL_Status  CSL_intcHwSetup(
    CSL_IntcHandle            hIntc,
    CSL_IntcHwSetup           *params
);

/*
 * ======================================================
 *   @func  CSL_intcHwControl
 *
 *   @desc  This function will perform specified control operation on the
 *          interrupt whose handle is passed as argument to this function.
 *
 *   @arg  hIntc
 *              INTC Handle
 *   @arg  mycmd
 *              Command such as CSL_INTC_CMD_EVTSET or CSL_INTC_CMD_EVTCLEAR etc.
 *   @arg  myControlArg
 *              Control Argument
 *
 *   @ret  CSL_Status
 *
 * ======================================================
 */

CSL_Status  CSL_intcHwControl(
    CSL_IntcHandle            hIntc,
    CSL_IntcHwControlCmd      mycmd,
    void                      *myControlArg
);

/*
 * ======================================================
 *   @func  CSL_intcClose
 *
 *   @desc  This API will perform specified control operation on the
 *          interrupt whose handle is passed as argument to this function.
 *
 *   @arg       intcHandle
 *              INTC Handle
 *
 *   @ret       CSL_Status
 *                              returns the status viz. CSL_SOK or CSL_ESYS_FAIL.
 * ======================================================
 */

CSL_Status  CSL_intcClose(
    CSL_IntcHandle            intcHandle
);

/*
 * ======================================================
 *   @func   CSL_intcGetHwStatus
 *
 *   @desc   The query is processed in a non-destructive manner as far as
 *           the interrupt setup is concerned. The concrete type of the void*
 *           argument passed should correspond to one of the supported queries
 *           as enumerated in the CSL_intcHwStatusQuery data type.
 *
 *   @arg  hIntc
 *              INTC Handle
 *   @arg  myQuery
 *              Query to be processed.
 *   @arg  answer
 *              This parameter is filled with the old status.
 *
 *   @ret  CSL_Status
 *                              returns the status viz. CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */

CSL_Status  CSL_intcGetHwStatus(
    CSL_IntcHandle            hIntc,
    CSL_IntcHwStatusQuery     myQuery,
    void                      *answer
);

/*
 * ======================================================
 *   @func  CSL_intcEventEnable
 *
 *   @desc  Enable a specific interrupt
 *
 *   @arg   evt_id
 *              Event ID of the interrupt
 *   @arg   oldState
 *              returns the old state in this parameter.
 *
 *   @ret   CSL_Status
 *                              returns the status viz. CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */

CSL_Status  CSL_intcEventEnable(
    CSL_IntcEventId           evt_id,
    CSL_IntcEventEnableState        *oldState
);

/*
 * ======================================================
 *   @func  CSL_intcEventDisable
 *
 *   @desc  Disable a specific interrupt
 *
 *   @arg   evt_id
 *              Event ID of the interrupt
 *   @arg   oldState
 *              returns the old state in this parameter.
 *
 *   @ret   CSL_Status
 *                              returns the status viz. CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */

CSL_Status  CSL_intcEventDisable(
    CSL_IntcEventId           evt_id,
   CSL_IntcEventEnableState                *prevState
);

/*
 * ======================================================
 *   @func  CSL_intcEventRestore
 *
 *   @desc  Restore a specific interrupt
 *
 *   @arg  evt_id
 *              Event ID of the interrupt
 *   @arg  ieState
 *              The interrupt state to restore to
 *
 *   @ret  CSL_Status
 *                              returns the status viz. CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */
CSL_Status  CSL_intcEventRestore(
    CSL_IntcEventId           evt_id,
    Uint32                ieState
);

/*
 * ======================================================
 *   @func  CSL_intcGlobalEnable
 *
 *   @desc  Enable IRQ/FIQ exceptions
 *
 *   @arg  oldState
 *             returns the old state in this parameter.
 *
 *   @ret  CSL_Status
 *                              returns the status viz. CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */

CSL_Status  CSL_intcGlobalEnable(
    CSL_IntcGlobalEnableState        *prevState
);

/*
 * ======================================================
 *   @func  CSL_intcGlobalDisable
 *
 *   @desc  Disables both the IRQ and FIQ exception generation.
 *
 *   @arg   oldState
 *             returns the old state in this parameter.
 *
 *   @ret  CSL_Status
 *                              returns the status viz. CSL_SOK or CSL_ESYS_FAIL.
 *
 * ======================================================
 */

CSL_Status  CSL_intcGlobalDisable(
    CSL_IntcGlobalEnableState               *prevState
);

/*
 * ======================================================
 *   @func  CSL_intcGlobalRestore
 *
 *   @desc  Restore IRQ/FIQ exceptions
 *
 *   @arg   gie
 *              Global Interrrupt Enable Mask.
 *
 *   @ret   void
 *
 * ======================================================
 */

void  CSL_intcGlobalRestore(
        CSL_IntcGlobalEnableState       gie
);

/*
 * ======================================================
 *   @func  CSL_intcPlugEventHandler
 *
 *   @desc   This function is used to bind an ISR with the corresponding
 *           interrupt. The "CSL_IntcEventHandlerRecord" data structure
 *           is filled in this function.
 *
 *   @arg  hIntc
 *              INTC Handle
 *   @arg  eventHandlerRecord
 *                              The eventhandler record data struture that has two parameters
 *              viz. the ISR address & a void * argument reserved for future use.
 *
 *   @ret  CSL_IntcEventHandler
 *              Returns the reference to the previous eventhandler.
 *
 * ======================================================
 */

CSL_IntcEventHandler
        CSL_intcPlugEventHandler (
                CSL_IntcHandle                  hIntc,
                CSL_IntcEventHandlerRecord *    eventHandlerRecord
);

#endif
