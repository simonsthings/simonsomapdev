/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== appResources.c ========
 *
 *  Definition of application-wide global variables.
 */

#include <std.h>

// DSP/BIOS includes
#include <sem.h>

// RF module includes
#include <utl.h>

// App includes
#include "appResources.h"

/* 
 *  Definition of STS-based objects for use with UTL_sts*() functions.
 *
 *  UTL_Sts objects are an extension of standard STS objects, so that
 *  UTL_stsStart/UTL_stsStop, UTL_stsPeriod and UTL_stsPhase functions are 
 *  supported. They measure execution time, period between periodic executions
 *  of a point in code, and phase between two periodic points, by placing
 *  the appropriate result to the specified STS object.
 *
 *  Assuming the BIOS configuration has some STS objects defined (and
 *  the default configuration defines stsTime0, stsTime1, ..., stsTime9
 *  STS objects for convenience), the only requirement to use a UTL_sts*()
 *  function in a module is to include <utl.h>; the argument(s) to UTL_sts*()
 *  functions are names of STS objects from the BIOS config (for instance, 
 *  UTL_stsPeriod( stsTime4 ); or UTL_stsStart( mySts );
 *
 *  However, it is necessary to define the extended STS objects, i.e.
 *  UTL_Sts objects, somewhere in the application. It is most convenient
 *  to define all such STS objects in one file such as this one.
 *  We use UTL_stsDefine() macro for every STS object
 *  in the BIOS config that we intend to use with UTL_sts*() functions:
 *  UTL_stsDefine( stsTime4 );
 *  UTL_stsDefine( mySts ); etc.
 *
 *  UTL_stsDefine() is a macro, which expands to nothing if UTL_sts*
 *  functionality is disabled. Otherwise, the macro defines a variable 
 *  and provides initial values for its fields.
 */
UTL_stsDefine( stsTime0 );
UTL_stsDefine( stsTime1 );
UTL_stsDefine( stsTime2 );
UTL_stsDefine( stsTime3 );
UTL_stsDefine( stsTime4 );
UTL_stsDefine( stsTime5 );
UTL_stsDefine( stsTime6 );
UTL_stsDefine( stsTime7 );
UTL_stsDefine( stsTime8 );
UTL_stsDefine( stsTime9 );

/*
 *  ======== binarySemPend========
 *  Pend function used to emulates a binary semaphore in DSP/BIOS
 *  (DSP/BIOS does not have built-in support for binary semaphores)
 *  Note that this only works if there is only one reader/pender
 *  The MSGQ module requires that if a pend function exists, it 
 *  must be binary, so we provide this function as a workaround.
 */
Bool binarySemPend(SEM_Handle semHandle, Uns timeout)
{
    Bool retVal;
    
    retVal = SEM_pend(semHandle, timeout);
    SEM_reset(semHandle, 0); 
    return(retVal);    
}

