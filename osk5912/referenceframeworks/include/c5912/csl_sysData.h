/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef _CSL_SYSDATA_H_
#define _CSL_SYSDATA_H_

#include <csl.h>
#include <csl_types.h>

#include <csl_resId.h>

#include <csl_intc.h>
#include <cslr_chip.h>


typedef struct CSL_SysDataObj {

    CSL_AltRouteHandler             altRouteHandler;    /* alternate base address router */
    CSL_Phy2VirtHandler             p2vHandler;         /* physical to virtual address mapping routine */
    CSL_Virt2PhyHandler             v2pHandler;         /* virtual to physical address mapping routine */
    Uint32                      versionId;          /* CSL s/w version ID */
    Uint32                      chipId;             /* CSL chip ID */
    CSL_ChipRegsOvly                chipRegs;           /* CSL chip Overlay structure, to be initialized
                                                         * in CSL_chipInit() */
    CSL_Xio                         xioInUse;           /* Shared-pin allocation bit-mask */

    CSL_BitMask32                   resInUse[(CSL_CHIP_NUM_RESOURCES + 31) / 32]; /* in-use allocMask */
    CSL_BitMask32                   initDone[(CSL_CHIP_NUM_MODULES   + 31) / 32]; /* init-done flags */

/* Include one data pointer for each CSL module
 * that needs additional context memory here under. The
 * actual memory is claimed during CSL Initialization of
 * that particular CSL module.
 */

     CSL_BitMask32 *                intcAllocMask;
     CSL_IntcEventHandlerRecord *       intcEventHandlerRecord;

} CSL_SysDataObj;

typedef CSL_SysDataObj *    CSL_SysDataHandle;
extern  CSL_SysDataHandle   CSL_sysDataHandle;


#endif /* _CSL_SYSDATA_H_ */
