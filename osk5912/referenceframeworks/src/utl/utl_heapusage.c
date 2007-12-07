/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_heapUsage.c ========
 *
 *  Implementation of the UTL_showHeapUsage() function
 */
#include <std.h>
#include <log.h>
#include <mem.h>

#include <utl.h>

/*
 *  ======== UTL_showHeapUsageFunc ==========
 *
 *  The function that prints heap usage information.
 */
Void UTL_showHeapUsageFunc( Int segid, String segname ) 
{
    MEM_Stat statbuf, *sb = &statbuf;
    
    if (UTL_logDebugHandle == NULL) {
        return;
    }
    if (MEM_stat( segid, sb ) == TRUE) {
        LOG_printf( UTL_logDebugHandle, "Heap %s: size = %d", 
                    segname, sb->size );
        LOG_printf( UTL_logDebugHandle, "     used: %d (%d%%)", 
                    sb->used, (Uns)((LgUns)sb->used * 100/sb->size) );
    }
}



