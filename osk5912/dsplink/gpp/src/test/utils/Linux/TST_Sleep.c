/** ============================================================================
 *  @file   TST_Sleep.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils\Linux
 *
 *  @desc   Implemetation of TST_Sleep function.
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
#include <unistd.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>

/*  ----------------------------------- Test framework                */
#include <TST_Sleep.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @macro  TIMER_RESOLUTION
 *
 *  @desc   Function Precondition.
 *  ============================================================================
 */
#define TIMER_RESOLUTION    1


/** ============================================================================
 *  @func   TST_Sleep
 *
 *  @desc   This function sleeps for the specified time.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
Void
TST_Sleep (Uint32 sleepTime)
{
    usleep (sleepTime * TIMER_RESOLUTION) ;
}

#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
