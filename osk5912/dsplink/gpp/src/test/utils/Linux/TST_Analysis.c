/** ============================================================================
 *  @file   TST_Analysis.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils\Linux
 *
 *  @desc   Implemetation of Analysis Functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers           */
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_Analysis.h>
#include <TST_PrintFuncs.h>
#include <TST_FileOperation.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   TST_ZeroTime
 *
 *  @desc   This variable stores the value of zero time.
 *  ============================================================================
 */
STATIC time_t initialTime = 0 ;


/** ============================================================================
 *  @func   TST_ZeroTime
 *
 *  @desc   This function resets the system timer.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TST_ZeroTime ()
{
    time (&initialTime) ;
}


/** ============================================================================
 *  @func   TST_GetTime
 *
 *  @desc   This function gives the time elapsed since last reset.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Uint32
TST_GetTime ()
{
    Uint32 clock_value ;
    time_t currentTime ;

    time (&currentTime) ;
    clock_value = (Uint32) (currentTime - initialTime) ;

    return clock_value ;
}


/** ============================================================================
 *  @func   TST_DoAnalysis
 *
 *  @desc   This function calculates the throughput of the test under
 *          consideration.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_DoAnalysis (IN Uint32 startTime, IN Uint32 stopTime,
                IN Uint32 iterations, IN Uint32 bufSize)
{
    DSP_STATUS status       = DSP_SOK ;
    Uint32     ticksElapsed           ;
    Real64     throughput             ;

    ticksElapsed = stopTime - startTime ;

    if (ticksElapsed != 0) {
        TST_PrnInfo ("Size       = 0x%x Bytes", bufSize) ;

        throughput = ((Real64)(bufSize) * (Real64)(iterations) * TICS_PER_SEC)
                      / ((Real64)ticksElapsed) ;
        TST_PrnInfo ("Throughput = %.2f bytes/Sec", throughput) ;

        throughput = ((Real64)(iterations) * TICS_PER_SEC)
                      / ((Real64)ticksElapsed) ;
        TST_PrnInfo ("             %.2f buffers/Sec", throughput) ;
    }
    else {
        TST_PrnInfo ("Negligible data transfer time.") ;
    }

    return status ;
}


/** ============================================================================
 *  @func   TST_GetFileSize.
 *
 *  @desc   This function which gives the size of the file  in bytes.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_GetFileSize (IN Char8 * fileName, OUT Uint32 * size)
{
    DSP_STATUS status   = DSP_SOK ;
    int        osStatus           ;
    struct     stat buf           ;

    TST_PrnDebug ("Entered TST_GetFileSize ()") ;

    DBC_Require (fileName != NULL) ;
    DBC_Require (size != NULL) ;

    if ((fileName == NULL) || (size == NULL)) {
        status = DSP_EINVALIDARG ;
    }

    if (DSP_SUCCEEDED (status)) {
        osStatus = stat (fileName, &buf);
        *size = (Uint32) buf.st_size ;
    }

    TST_PrnDebug ("Leaving TST_GetFileSize () ") ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
