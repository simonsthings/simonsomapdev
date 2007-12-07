/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_defaultLog.c ========
 *
 *  Defines handles for the four LOG objects (error/warning/messages/debug)
 *  and implements a function that lets the user set those.
 */
#include <std.h>
#include <log.h>

#include <utl.h>

LOG_Handle UTL_logErrorHandle   = NULL;
LOG_Handle UTL_logWarningHandle = NULL;
LOG_Handle UTL_logMessageHandle = NULL;
LOG_Handle UTL_logDebugHandle   = NULL;

/*
 *  ========= UTL_setLogs ==========
 *  Names the LOG objects to be used for errors/warnings/messages/debugging;
 *  they do not have to be all different.
 */
Void UTL_setLogs( LOG_Handle logErr, LOG_Handle logWarn, 
                         LOG_Handle logMsg, LOG_Handle logDbg )
{
    UTL_logErrorHandle   = logErr;
    UTL_logWarningHandle = logWarn;
    UTL_logMessageHandle = logMsg;
    UTL_logDebugHandle   = logDbg;
}


