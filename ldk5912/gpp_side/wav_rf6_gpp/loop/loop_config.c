/** ============================================================================
 *  @file   loop_config.c
 *
 *  @path   $(DSPLINK)/dsp/src/samples/loop/
 *
 *  @desc   Source file containing configuration for the LOOP sample.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- DSP/BIOS Headers            */
#include <std.h>
#include <sys.h>
#include <pool.h>

/*  ----------------------------------- DSP/BIOS LINK Headers       */
#include <linkcfg.h>
#include <dsplink.h>
#include <failure.h>
#include <platform.h>

#if !defined (DM642_PCI)
#include <shm_ips.h>
#endif /* if defined (DM642_PCI) */

#if defined (CHNL_ZCPY_LINK)
#include <sma_pool.h>
#endif /* if defined (CHNL_ZCPY_LINK) */

#if defined (CHNL_PCPY_LINK)
#include <buf_pool.h>
#endif /* if defined (CHNL_PCPY_LINK) */

/*  ----------------------------------- Sample Headers              */
#include <loop_config.h>


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @const  NUM_POOLS
 *
 *  @desc   Number of pools used in this application.
 *  ============================================================================
 */
#define NUM_POOLS       1

#if defined (CHNL_PCPY_LINK)
/** ============================================================================
 *  @name   dummyPoolFxns
 *
 *  @desc   Dummy pool function table defined so that the actual pool with
 *          correct buffer sizes can be opened in the application.
 *  ============================================================================
 */
POOL_Fxns dummyPoolFxns =
{
    (POOL_Open)  SYS_zero,  // have open return 0 so POOL_init will not fail
    (POOL_Close) FXN_F_nop, // have close do nothing
    (POOL_Alloc) SYS_one,   // have alloc return non-zero
    (POOL_Free)  FXN_F_nop  // have free do nothing
} ;

/** ============================================================================
 *  @name   LOOP_Pools
 *
 *  @desc   Array of pools.
 *  ============================================================================
 */
POOL_Obj LOOP_Pools [NUM_POOLS] =
{
    {
        &FXN_F_nop,                  /* Init Function                    */
        &dummyPoolFxns,              /* Pool interface functions         */
        NULL,                        /* Pool params                      */
        NULL                         /* Pool object: Set within pool     */
    }
} ;

#endif /* if defined (CHNL_PCPY_LINK) */


#if defined (CHNL_ZCPY_LINK)

#define SAMPLEPOOL_open(poolId, params)
#define SAMPLEPOOL_close(poolId)

/** ============================================================================
 *  @name   smaPoolParams
 *
 *  @desc   Parameters for the SMA pool.
 *  ============================================================================
 */
SMAPOOL_Params smaPoolParams =
{
    0, /* Pool ID */
    TRUE
} ;

/** ============================================================================
 *  @name   LOOP_Pools
 *
 *  @desc   Array of pools.
 *  ============================================================================
 */
POOL_Obj LOOP_Pools [NUM_POOLS] =
{
    {
        &SMAPOOL_init,               /* Init Function                    */
        (POOL_Fxns *) &SMAPOOL_FXNS, /* Pool interface functions         */
        &smaPoolParams,              /* Pool params                      */
        NULL                         /* Pool object: Set within pool     */
    }
} ;

#endif /* if defined (CHNL_ZCPY_LINK) */


/** ============================================================================
 *  @name   POOL_config
 *
 *  @desc   POOL configuration information.
 *          POOL_config is a required global variable.
 *  ============================================================================
 */
POOL_Config POOL_config = {LOOP_Pools, NUM_POOLS} ;


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
