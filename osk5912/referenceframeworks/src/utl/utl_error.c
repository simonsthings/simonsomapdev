/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_error.c ========
 *
 *  Implementation of the UTL_error() function
 */
#include <std.h>
#include <hwi.h>
#include <idl.h>
#include <log.h>

#include <utl.h>

/*
 *  UTL_assert support: for users who do not develop on C54 or who
 *  do not use Code Composer Studio, UTL_assert( <condition>) macro
 *  is defined to call UTL_error if the assertation fails.
 */

/*
 *  ======== UTL_error ========
 *  Place an error message (and up to 1 argument) into the BIOS system log,
 *  then halt the target
 */
Void UTL_error( String msg, Arg arg )
{
    HWI_disable();

    /* put message into system log and disable log (to preserve error) */
    LOG_error( msg, arg );
    LOG_disable( LOG_D_system );

    /* 
     *  If you place a breakpoint above you will instantly notice when
     *  an assertation fails.
     */

    /* 
     *  Loop for a long time, then quickly check if the user tries to
     *  halt the target (and halt it if so), then return to the loop
     */
    for (;;) {              /* loop forever */
        volatile int i;     /* preclude optimization */
        /* you may want to reduce this number if your target is very slow: */
        #define NUMBEROFITERATIONS 5000
        /* 
         *  If you are here, that means one of your assertations has failed; 
         *  examine the call stack and see where exactly that assertation is.
         */
        for (i = 0, IDL_run(); i < NUMBEROFITERATIONS; ++i) {
            ;
        }
    }
}


