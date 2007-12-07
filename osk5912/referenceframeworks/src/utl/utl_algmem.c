/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== utl_algmem.c ========
 *
 *  Implementation of the UTL_showAlgmem() function.
 */
#include <std.h>
#include <log.h>

#include <ialg.h>

#include <algrf.h>
#include <utl.h>

/*
 *  ======== UTL_showAlgMem ==========
 *
 *  For the given XDAIS algorithm handle, shows all algorithm's memTab[] 
 *  allocation information on standard log output; if boolVerbose is nonzero,
 *  it also prints addresses
 */
Void UTL_showAlgMemFunc( Ptr algHandlePtr, String algName, Int boolVerbose )
{
    IALG_MemRec memTab[ ALGRF_MAXMEMRECS ];
    Int totalSizeInt = 0;
    Int totalSizeExt = 0;
    Int numRecs, i;
    IALG_Handle algHandle = (IALG_Handle)algHandlePtr;
    
    if (UTL_logDebugHandle == NULL) {
        return;
    }
    
    /* query the algorithm about its allocated memory segments */
    numRecs = algHandle->fxns->algFree( algHandle, memTab );
    memTab[0].base = algHandle;

    /* If pointer is NULL, treat the string as a null character */
    if (algName == NULL) {
        LOG_printf( UTL_logDebugHandle, "Alg  mem. alloc:" );
    }
    else {
        LOG_printf( UTL_logDebugHandle, "Alg %s mem. alloc:", algName );
    }
    for (i = 0; i < numRecs; ++i) {
        if (ALGRF_memSpace( memTab[i].space) == ALGRF->INTHEAP ) {
            LOG_printf( UTL_logDebugHandle, 
                "  memTab[%d]: size = %d (internal)", i, memTab[i].size );
            totalSizeInt += memTab[i].size;
        } else {
            LOG_printf( UTL_logDebugHandle, 
                "  memTab[%d]: size = %d (external)", i, memTab[i].size );
            totalSizeExt += memTab[i].size;
        }
        if (boolVerbose) {
            LOG_printf( UTL_logDebugHandle, 
                "    addr = 0x%x (segID = %d)", memTab[i].base,  memTab[i].space );
        }
    }
    LOG_printf( UTL_logDebugHandle, 
        "  total size: int. = %d, ext. = %d", totalSizeInt, totalSizeExt );
}


