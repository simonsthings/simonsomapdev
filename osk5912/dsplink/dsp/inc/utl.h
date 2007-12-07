/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/* 
 *  ======== utl.h ========
 *
 *  Utility module for various debugging operations
 *
 *  The module provides several classes of features, each of which can be
 *  enabled or disabled individually, unless a *level* of debugging is
 *  defined (see below). Each debugging feature is implemented via a macro
 *  (or set of macros) which expands to code if the feature (class) is 
 *  turned on, otherwise it expands to nothing.
 *
 *  Each feature can be turned on or off with its specific flag, by defining
 *  it to be 0 or 1 (individual features are described at the place of their
 *  definitions in this file). Classes are described in detail further in
 *  this file. 
 * 
 *  Instead of defining individual class, the user can also define level
 *  of debugging, which automatically enables certain classes and disables
 *  others (unless they are already enabled or disabled by the user); a level
 *  enables all the classes the previous level enables and one other.
 *  (Also, a combination can be used: level + individual classes explicitly
 *  turned on or off). 
 *
 *  Here is the list of classes, their levels, flag names, and brief 
 *  descriptions:
 *
 *  - error messages                (level: 10, flag: UTL_LOGERROR): 
 *    printing user's error messages to a LOG object via UTL_logError();
 *  - warning messages              (level: 20, flag: UTL_LOGWARNING): 
 *    printing user's warning messages to a LOG object via UTL_logWarning();
 *  - general messages              (level: 30, flag: UTL_LOGMESSAGE):
 *    printing user's general messages to a LOG object via UTL_logMessage();
 *  - debug messages                (level: 40, flag: UTL_LOGDEBUG):
 *    printing user's debug messages to a LOG object via UTL_logDebug();
 *  - assertions                    (level: 50, flag: UTL_ASSERT):
 *    halting execution if the condition in assertion in UTL_assert() fails
 *  - time statistics               (level: 60, flag: UTL_STS):
 *    storing various real-time parameters into STS objects using UTL_sts*()
 *  - algorithm memory usage        (level: 70, flag: UTL_ALGMEM)
 *    reporting heap usage with UTL_showAlgMem(), UTL_showHeapUsage()
 *
 *  Defining UTL_DBGLEVEL automatically defines above flags to be 0 or 1,
 *  unless a flag for the class is already defined (to be 0 or 1).
 *  The levels are:
 *
 *  level  0: all debugging features disabled
 *  level 10: UTL_LOGERROR only is defined
 *  level 20: all in lower levels, plus UTL_LOGWARNING is defined
 *  level 30: all in lower levels, plus UTL_LOGMESSAGE is defined
 *  level 40: all in lower levels, plus UTL_LOGDEBUG   is defined
 *  level 50: all in lower levels, plus UTL_ASSERT     is defined
 *  level 60: all in lower levels, plus UTL_STS        is defined
 *  level 70: all in lower levels, plus UTL_ALGMEM     is defined
 *
 *  Example: compiling with -DUTL_DBGLEVEL=30 will enable classes
 *  error/warning/general messages (meaning macros for those features
 *  will be turned into actual code) and disable all others (meaning
 *  their macros will expand to nothing).
 *  Example 2: compiling with -DUTL_DBGLEVEL=20 and -DUTL_ASSERT=1
 *  will enable error/warning messages and assertions
 *  Example 3: compiling with-DUTL_DBGLEVEL=70 -DUTL_STS=0 
 *  will enable all classes except STS.
 *
 *  Note that usually levels 0-30 would be used in deployment mode, 
 *  and levels 40 and up in development mode).
 *
 *  Error/warning/message/debug logs can use same LOG objects or 
 *  different ones; assert and algmem use the LOG object used by
 *  UTL_logDebug. The choice of logs is made by
 *  
 *  UTL_setLogs( <LOG for errors>,   <LOG for warnings>, 
 *               <LOG for messages>, <LOG for debugging> );
 *
 *  This macro also expands to nothing if none of the classes that use
 *  log objects is enabled.
 */
#ifndef UTL_
#define UTL_

#include <log.h>
#include <sts.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 
 *  If UTL_DBGLEVEL is defined, we define which debugging classes will be
 *  enabled or disabled by defining them here
 */
#ifdef UTL_DBGLEVEL 

    #if UTL_DBGLEVEL >= 10
        #ifndef     UTL_LOGERROR
            #define UTL_LOGERROR   1
        #endif
    #endif

    #if UTL_DBGLEVEL >= 20
        #ifndef     UTL_LOGWARNING
            #define UTL_LOGWARNING 1
        #endif
    #endif

    #if UTL_DBGLEVEL >= 30
        #ifndef     UTL_LOGMESSAGE
            #define UTL_LOGMESSAGE 1
        #endif
    #endif

    #if UTL_DBGLEVEL >= 40
        #ifndef     UTL_LOGDEBUG
            #define UTL_LOGDEBUG   1
        #endif
    #endif

    #if UTL_DBGLEVEL >= 50
        #ifndef     UTL_ASSERT
            #define UTL_ASSERT     1
        #endif
    #endif

    #if UTL_DBGLEVEL >= 60
        #ifndef     UTL_STS
            #define UTL_STS        1
        #endif
    #endif

    #if UTL_DBGLEVEL >= 70
        #ifndef     UTL_ALGMEM
            #define UTL_ALGMEM     1
        #endif
    #endif

#endif  /* UTL_DBGLEVEL */

/* flags which are not defined, are defined here to be 0 */
#ifndef     UTL_LOGERROR
    #define UTL_LOGERROR   0
#endif
#ifndef     UTL_LOGWARNING
    #define UTL_LOGWARNING 0
#endif
#ifndef     UTL_LOGMESSAGE
    #define UTL_LOGMESSAGE 0
#endif
#ifndef     UTL_LOGDEBUG
    #define UTL_LOGDEBUG   0
#endif
#ifndef     UTL_ASSERT
    #define UTL_ASSERT     0
#endif
#ifndef     UTL_STS     
    #define UTL_STS        0
#endif
#ifndef     UTL_ALGMEM  
    #define UTL_ALGMEM     0
#endif



/*
 *  Setting up LOG objects for classes:
 *  We have LOG objects for errors, warnings, messages, 
 *  and debug/assert/algmem. They are all set with UTL_setLogs() call,
 *  defined to be a macro that expands to nothing if none of the classes
 *  are enabled.
 */
#if UTL_LOGERROR == 1 || UTL_LOGWARNING == 1 || UTL_LOGMESSAGE == 1 || \
    UTL_LOGDEBUG == 1 || UTL_ASSERT == 1 || UTL_STS == 1 || UTL_ALGMEM == 1

    extern LOG_Handle UTL_logErrorHandle;
    extern LOG_Handle UTL_logWarningHandle;
    extern LOG_Handle UTL_logMessageHandle;
    extern LOG_Handle UTL_logDebugHandle;

    /*
     *  ========= UTL_setLogs ==========
     *  Names the LOG objects to be used for errors/warnings/messages/debugging;
     *  they do not have to be all different.
     */
    extern Void UTL_setLogs( LOG_Handle logErr, LOG_Handle logWarn, 
                             LOG_Handle logMsg, LOG_Handle logDbg );

#else   /* none of the classes enabled, so the macro is empty */
    #define UTL_setLogs( logErr, logWarn, logMsg, logDbg )
#endif



/*
 *  UTL_log{Error,Warning,Message,Debug}() macros
 *
 *  These four classes of debugging/diagnostics functions perform, if enabled,
 *  a LOG_printf of the given parameters to a default LOG object 
 *  for that class (as determined by UTL_setLogs(), see above)
 *
 *  Syntax: 
 *  UTL_log{Error,Warning,Message,Debug}[0,1,2]( <format>[, <arg1>[, <arg2>]] )
 *
 *  The suffix in the function name (nothing, 0, 1, or 2) determines how many
 *  parameters the formatted output has (none, none, one, or two, respectively).
 *  
 *  Example: UTL_logMessage1( "Current framesize: %d", fs );
 *  If UTL_LOGMESSAGE is defined to be 1 (UTL_DBGLEVEL >= 10), it will
 *  expand to LOG_printf( UTL_logErrorHandle, "Current framesize: %d", fs );
 *  where UTL_logErrorHandle contains the address of the LOG objects for
 *  errors as determined by the first parameter in a call to UTL_setLogs().
 */
#if UTL_LOGERROR == 1
    #define UTL_logError( format )  \
            LOG_printf( UTL_logErrorHandle, (format) )
    #define UTL_logError0( format )  \
            LOG_printf( UTL_logErrorHandle, (format) )
    #define UTL_logError1( format, arg1 )  \
            LOG_printf( UTL_logErrorHandle, (format), (arg1) )
    #define UTL_logError2( format, arg1, arg2 )  \
            LOG_printf( UTL_logErrorHandle, (format), (arg1), (arg2) )
#else   /* UTL_LOGERROR == 0 */
    #define UTL_logError( format )
    #define UTL_logError0( format )
    #define UTL_logError1( format, arg1 )
    #define UTL_logError2( format, arg1, arg2 )
#endif  /* UTL_LOGERROR */

#if UTL_LOGWARNING == 1
    #define UTL_logWarning( format )  \
            LOG_printf( UTL_logWarningHandle, (format) )
    #define UTL_logWarning0( format )  \
            LOG_printf( UTL_logWarningHandle, (format) )
    #define UTL_logWarning1( format, arg1 )  \
            LOG_printf( UTL_logWarningHandle, (format), (arg1) )
    #define UTL_logWarning2( format, arg1, arg2 )  \
            LOG_printf( UTL_logWarningHandle, (format), (arg1), (arg2) )
#else   /* UTL_LOGWARNING == 0 */
    #define UTL_logWarning( format )
    #define UTL_logWarning0( format )
    #define UTL_logWarning1( format, arg1 )
    #define UTL_logWarning2( format, arg1, arg2 )
#endif  /* UTL_LOGWARNING */

#if UTL_LOGMESSAGE == 1
    #define UTL_logMessage( format )  \
            LOG_printf( UTL_logMessageHandle, (format) )
    #define UTL_logMessage0( format )  \
            LOG_printf( UTL_logMessageHandle, (format) )
    #define UTL_logMessage1( format, arg1 )  \
            LOG_printf( UTL_logMessageHandle, (format), (arg1) )
    #define UTL_logMessage2( format, arg1, arg2 )  \
            LOG_printf( UTL_logMessageHandle, (format), (arg1), (arg2) )
#else   /* UTL_LOGMESSAGE == 0 */
    #define UTL_logMessage( format )
    #define UTL_logMessage0( format )
    #define UTL_logMessage1( format, arg1 )
    #define UTL_logMessage2( format, arg1, arg2 )
#endif  /* UTL_MESSAGE */

#if UTL_LOGDEBUG == 1
    #define UTL_logDebug( format )  \
            LOG_printf( UTL_logDebugHandle, (format) )
    #define UTL_logDebug0( format )  \
            LOG_printf( UTL_logDebugHandle, (format) )
    #define UTL_logDebug1( format, arg1 )  \
            LOG_printf( UTL_logDebugHandle, (format), (arg1) )
    #define UTL_logDebug2( format, arg1, arg2 )  \
            LOG_printf( UTL_logDebugHandle, (format), (arg1), (arg2) )
#else   /* UTL_LOGDEBUG == 0 */
    #define UTL_logDebug( format )
    #define UTL_logDebug0( format )
    #define UTL_logDebug1( format, arg1 )
    #define UTL_logDebug2( format, arg1, arg2 )
#endif  /* UTL_LOGDEBUG */

        
/*
 *  UTL_ALGMEM class comprises two related funtions:
 *  UTL_showAlgMem() which displays algorithm instance's memory usage, and
 *  UTL_showHeapUsage() which reports a particular heap's usage 
 */
#if UTL_ALGMEM == 1
    /* 
     *  ======== UTL_showAlgMem ==========
     *
     *  Reporting XDAIS algorithm memory usage
     *
     *  This macro provides size and memory type information for each allocated
     *  memory segment of the given algorithm (by calling the function below),
     *  as well as the total size broken down by memory type.
     *  If the third argument is set to 1, it will also show the actual
     *  memory address of the segment. That can be accomplished by defining
     *  flag UTL_ALGMEMVERBOSE to 1.
     */
    #define UTL_showAlgMem( alg )       \
        UTL_showAlgMemFunc( (Ptr)(alg), UTL_stringify( alg ), \
                            UTL_ALGMEMVERBOSE )
    /* Variant of UTL_showAlgMem() where name is specified directly. */
    #define UTL_showAlgMemName( alg, algName )       \
        UTL_showAlgMemFunc( (Ptr)(alg), algName, UTL_ALGMEMVERBOSE )
    #ifndef     UTL_ALGMEMVERBOSE
        #define UTL_ALGMEMVERBOSE 0   /* by default, don't display addresses */
    #endif
    
    /*
     *  ======== UTL_showAlgMemFunc ==========
     *
     *  Declaration of the actual function implementing the above macro.
     */
    extern Void UTL_showAlgMemFunc( Ptr algHandle, String algName, 
                                    Int boolVerbose );

    /* 
     *  ======== UTL_showHeapUsage ==========
     *
     *  A small utility function that shows the size and usage of the
     *  given segment. Similarly to a function above, it is a macro that
     *  expands the argument for printing and calls the actual function.
     */
    #define UTL_showHeapUsage( SEGID ) /* name such as IDATA */     \
        UTL_showHeapUsageFunc( SEGID, UTL_stringify( SEGID ) )
        
    /*
     *  ======== UTL_showHeapUsageFunc ==========
     *
     *  Declaration of the actual function implementing the above macro.
     */
    extern Void UTL_showHeapUsageFunc( Int segid, String segname );
    
#else                           /* UTL_ALGMEM */
    #define UTL_showAlgMem( alg )
    #define UTL_showAlgMemName( alg, algName )
    #define UTL_showHeapUsage( SEGID ) 
#endif                          /* UTL_ALGMEM */


/*  
 *  ======== UTL_sts* ==========
 *
 *  There are several utility functions for time measurement: measuring
 *  periods, execution times, and phases between periodic events.
 *
 *  Arguments to UTL_sts* functions are names of STS objects as defined
 *  in the DSP/BIOS configuration file (not the addresses). The only 
 *  prerequisite for using an  STS object with any UTL_sts function 
 *  is to use the following macro in any of the .c modules of the application: 
 *  UTL_stsDefine( <sts> );
 *  where <sts> is the name of the STS object as defined in the configuration 
 *  (e.g. "stsTime0" or "stsTransfer").
 *  This macro allocates a space for an internal structure used by UTL_sts* 
 *  functions and initializes that object.
 *
 *  Supported functions (macros) are the following: 
 * 
 *  UTL_stsStart( <sts> ), UTL_stsStop( <sts> ) -- start/stop sequence
 *  for measuring execution time. UTL_stsStart() also returns current time,
 *  UTL_stsStop() returns the measured difference. NOTE: for accurate
 *  results, it is recommended that you DISABLE INTERRUPTS before calling
 *  UTL_stsStart() and to enable them after returning from UTL_stsStop().
 * 
 *  UTL_stsPeriod( <sts> ) measures the difference in time between two
 *  calls to this procedure (for the same STS object), applies it to 
 *  the <sts> object and returns the difference.
 *
 *  UTL_stsPhase( <stsSrc1>, <stsSrc2>, <stsDst> ) -- examines the
 *  absolute difference in timestamp values for STS objects <stsSrc1> and 
 *  <stsSrc2>, stores it in <stsDst> and returns that value. Objects
 *  <stsSrc1> and <stsSrc2> must have been used in a UTL_stsPeriod()
 *  function; for instance, if data transmit functions for two channels
 *  call UTL_stsPeriod( stsCh0 ) and UTL_stsPeriod( stsCh0 ) respectively,
 *  UTL_stsPhase( stsCh0, stsCh0, stsPh01 ) will return the phase 
 *  difference between the two channels and update stsPh01 accordingly.
 */
#if UTL_STS == 1

    typedef struct _UTLSTS_Obj {
        STS_Obj *sts;      /* pointer to STS object defined in config file */
        LgUns    lastTime; /* last timestamp for this object */
        LgUns    count;    /* number of times Period invoked (used for Phase) */
    } UTLSTS_Obj;

    #define UTL_stsDefine( stsName )        \
        extern STS_Obj stsName;         \
        UTLSTS_Obj utl##stsName = { &(stsName), 0, 0 }

    #define UTL_stsStart( stsName ) do {        \
        extern UTLSTS_Obj utl##stsName;         \
        UTL_stsStartFunc( &(utl##stsName) );    \
        } while (0)

    #define UTL_stsStop( stsName ) do {         \
        extern UTLSTS_Obj utl##stsName;         \
        UTL_stsStopFunc( &(utl##stsName) );     \
        } while (0)

    #define UTL_stsPeriod( stsName ) do {       \
        extern UTLSTS_Obj utl##stsName;         \
        UTL_stsPeriodFunc( &(utl##stsName) );   \
        } while (0)

    #define UTL_stsPhase( stsSrc1, stsSrc2, stsDst ) do {                     \
        extern UTLSTS_Obj utl##stsSrc1, utl##stsSrc2, utl##stsDst ;           \
        UTL_stsPhaseFunc( &(utl##stsSrc1), &(utl##stsSrc2), &(utl##stsDst) ); \
        } while (0)

    #define UTL_stsReset( stsName ) do {        \
        extern UTLSTS_Obj utl##stsName;         \
        UTL_stsResetFunc( &(utl##stsName) );    \
        } while (0)

    extern LgUns UTL_stsStartFunc ( UTLSTS_Obj *utlsts );
    extern LgUns UTL_stsStopFunc  ( UTLSTS_Obj *utlsts );
    extern LgUns UTL_stsPeriodFunc( UTLSTS_Obj *utlsts );
    extern LgUns UTL_stsPhaseFunc ( UTLSTS_Obj *utlstsSrc1, 
                                    UTLSTS_Obj *utlstsSrc2, 
                                    UTLSTS_Obj *utlstsDst );
    extern Void  UTL_stsResetFunc ( UTLSTS_Obj *utlsts );

#else   /* UTL_STS */

    #define UTL_stsDefine( stsName ) extern Int noUTLStsFunctionality
    #define UTL_stsStart( stsName )
    #define UTL_stsStop( stsName )
    #define UTL_stsPeriod( stsName )
    #define UTL_stsPhase( stsSrc1, stsSrc2, stsDst )
    #define UTL_stsReset( stsName )

#endif  /* UTL_STS */



/*
 *  ======== UTL_assert ========
 *  Assert macro for exceptions
 * 
 *  If an assertion fails, we interrupt the execution flow and preserve
 *  the state of the system.
 *  If the application is being debugged with Code Composer Studio, we place 
 *  a word in program memory that has the effect of a breakpoint -- it halts 
 *  the execution and CCS shows the line of code where the failed assertion 
 *  occured. The special word that causes the breakpoint effect is different 
 *  for every DSP family.
 *  Otherwise, if CCS is not used, we disable interrupts and run into an 
 *  infinite loop, waiting for the user to halt the target manually.
 *  To do so, define UTL_ASSERTCCS macro flag externally to be 0.
 */
#if UTL_ASSERT == 1  /* first check if UTL_ASSERT() is enabled at all */

    /* 
     *  First we define the special word for each platform: 
     *  On 55x we add a NOP to guard against small possibility that 
     *  long instruction pipeline causes debugger to stop at next C src line
     *  instead of currently executing C src statement. We also ensure this 
     *  works on both 55x mnemonic (default) and algebraic 55x assembler.
     *  Note that asm statement is a single execution context to maintain
     *  pre-processor consistency.
     */
    #if   defined( _54_ ) 
        #define UTL_STOPWORD asm( "  ESTOP" )
    #elif defined( _55_ )
        #define UTL_STOPWORD asm(" .if (.MNEMONIC)\n ESTOP_1\n .else\n ESTOP_1()\n .endif\n NOP")
    #elif defined( _6x_ ) 
        #define UTL_STOPWORD asm( "    NOP\n    .word 0x10000000" )
    #else
        #define UTL_ASSERTCCS 0  /* unknown platform => no CCS breakpoint */
    #endif

    #if !defined( UTL_ASSERTCCS ) 
        #define UTL_ASSERTCCS 0
    #endif

    /* check if it's OK with the user to use CCS breakpoints for asserts() */
    #if UTL_ASSERTCCS == 0
        /* it is; define the UTL_assert macro */
        #define UTL_assert( expr )      \
            for (; !( expr ); )         \
                UTL_STOPWORD

    #else /* UTL_ASSERTCCS == 0 */  /* no Code Composer Studio here */
        #define UTL_assert( expr ) do {                                        \
            if ( !(expr) ) {  /* use UTL_error() to halt, prototyped below */  \
                UTL_error( "Assertion Violation: " __FILE__ ", line "          \
                    UTL_stringify(__LINE__) " (" UTL_stringify(expr) ")", 0 ); \
            }                                                                  \
        } while (0)
    #endif  /* UTL_ASSERTCCS */

    /*
     *  UTL_assertFunc() is meant for testing expressions with side effects,
     *  i.e. mostly for checking function calls. If enabled, behaves exactly
     *  the same as UTL_assert. If disabled, it is not expanded to nothing
     *  but to actual expression. E.g.,
     *  UTL_assertFunc( someFunc() == TRUE ); 
     *  If asserts are disabled, expands to
     *  (someFunc() == TRUE);
     */
    #define UTL_assertFunc UTL_assert
    
#else                       /* UTL_ASSERT */
    #define UTL_assert( expr )
    #define UTL_assertFunc( expr ) (expr)
#endif                      /* UTL_ASSERT */

/*
 *  Function prototype for UTL_error: place an error message (and up to 1 
 *  argument) into the BIOS system log, then halt the target. Can be used
 *  outside of UTL_assert() if someone needs it.
 */
extern Void UTL_error( String msg, Arg arg );

/*
 *  ======== UTL_stringify ========
 *  A utility macro that "stringifies" the macro argument x
 */
#define UTL_stringify(x)   _UTL_stringify(x)
#define _UTL_stringify(x)  #x


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* UTL_ */


