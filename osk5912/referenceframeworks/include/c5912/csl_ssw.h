
/** @mainpage Static Switch (DSP side)
*
* @section Introduction
*
* @subsection xxx Purpose and Scope
* The purpose of this document is to identify a set of common CSL APIs for the
* Static Switch module across various devices. The CSL developer
* is expected to refer to this document while designing APIs for these modules
* . Some of the listed APIs may not be applicable to a given Static Switch module.
* While in other cases this list of APIs may not be
* sufficient to cover all the features of a particular Staitc Switch Module.
* The CSL developer should use his discretion in designing
* new APIs or extending the existing ones to cover these.
*
*
* @subsection aaa Terms and Abbreviations
*   -# CSL:  Chip Support Library
*   -# API:  Application Programmer Interface
*   -# SSW:  Static Switch
*
* @subsection References
*    -# CSL-001-DES, CSL 3.x Design Specification DocumentVersion 1.02
*    -# SPRS231B, OMAP5912 Applications Processor Data Manual, December 2003 - Revised March 2004
*
* @subsection Assumptions
*     The abbreviations SSW, ssw and Ssw have been used throughout this
*     document to refer to the Static Switch module
*/


/** @file csl_ssw.h
 *
 *    @brief    Header file for functional layer of CSL
 *
 *  Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 *  Modification 1
 *    - Modified on: 14/04/2004
 *    - Reason: Starting from 5905 sources, created the initial file.
 *
 *  @date 14th April, 2004
 *    @author Prakash V. Gudnavar
 */
#ifndef _CSL_SSW_H_
#define _CSL_SSW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl_error.h>
#include <csl_types.h>
#include <cslr_ssw.h>

/**************************************************************************\
* SSW global macro declarations
\**************************************************************************/
#define CSL_SSW_PER_ANY           -1


/**************************************************************************\
* SSW global typedef declarations
\**************************************************************************/

/** @brief Enumeration for shared peripherals
 */
typedef enum {
    /** Configuration register for shared peripheral UART1 */
    CSL_SSW_SHAREDPER_UART1 = 0,
    /** Configuration register for shared peripheral UART2 */
    CSL_SSW_SHAREDPER_UART2 = 2,
    /** Configuration register for shared peripheral UART3 */
    CSL_SSW_SHAREDPER_UART3 = 4,
    /** Configuration register for shared peripheral MCBSP2 */
    CSL_SSW_SHAREDPER_MCBSP2 = 9,
    /** Configuration register for shared peripheral I2C1 */
    CSL_SSW_SHAREDPER_I2C = 0xA,
    /** Configuration register for shared peripheral SPI */
    CSL_SSW_SHAREDPER_SPI = 0xB,
    /** Configuration register for shared peripheral DMTIMER1 */
    CSL_SSW_SHAREDPER_DMTIMER1 = 0xC,
    /** Configuration register for shared peripheral DMTIMER2 */
    CSL_SSW_SHAREDPER_DMTIMER2 = 0xD,
    /** Configuration register for shared peripheral DMTIMER3 */
    CSL_SSW_SHAREDPER_DMTIMER3 = 0xE,
    /** Configuration register for shared peripheral DMTIMER4 */
    CSL_SSW_SHAREDPER_DMTIMER4 = 0xF,
    /** Configuration register for shared peripheral DMTIMER5 */
    CSL_SSW_SHAREDPER_DMTIMER5 = 0x10,
    /** Configuration register for shared peripheral DMTIMER6 */
    CSL_SSW_SHAREDPER_DMTIMER6 = 0x11,
    /** Configuration register for shared peripheral DMTIMER7 */
    CSL_SSW_SHAREDPER_DMTIMER7 = 0x13,
    /** Configuration register for shared peripheral DMTIMER8 */
    CSL_SSW_SHAREDPER_DMTIMER8 = 0x14,
    /** Configuration register for shared peripheral MMCSD2 */
    CSL_SSW_SHAREDPER_MMCSD2 = 0x16
} CSL_SswSharedPer;

/** @brief Enumeration for control commands passed to @a CSL_sswHwControl()
 *
 * This is the set of commands that are passed to the @a CSL_sswHwControl()
 * with an optional argument type-casted to @a void* . The arguments to be
 * passed with each enumeration if any are specified next to the enumeration */
typedef enum {
    /** Acquire a shared perpheral(argument type: @a CSL_SswSharedPer) */
    CSL_SSW_CMD_ACQUIRE      =                0,
    /** Release a shared perpheral(argument type: @a CSL_SswSharedPer) */
    CSL_SSW_CMD_RELEASE     =                 1
} CSL_SswHwControlCmd;

/** @brief Enumeration for queries passed to @a CSL_sswGetHwStatus()
 *
 * This is used to get the status of different operations or to get the
 * existing setup of SSW. The arguments to be passed with each
 * enumeration if any are specified next to the enumeration */
typedef enum {
    /** Get current h/w setup parameters (response type: @a CSL_SswHwSetup *) */
    CSL_SSW_QUERY_CURRENT_HWSETUP       =                     0
} CSL_SswHwStatusQuery;

/** @brief Structure that is used to setup the Static Switch
 *
 * This structure is used to setup or obtain the existing setup of
 * SSW using @a CSL_sswHwSetup() & @a CSL_sswGetHwStatus() functions
 * respectively. */
typedef struct CSL_SswHwSetup {
    /** Ownership of UART1: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  uart1Owned;
    /** Ownership of UART2: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  uart2Owned;
    /** Ownership of UART3: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  uart3Owned;
    /** Ownership of MCBSP2: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  mcbsp2Owned;
    /** Ownership of I2C: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  i2cOwned;
    /** Ownership of SPI: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  spiOwned;
    /** Ownership of DMTIMER1: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer1Owned;
    /** Ownership of DMTIMER2: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer2Owned;
    /** Ownership of DMTIMER3: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer3Owned;
    /** Ownership of DMTIMER4: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer4Owned;
    /** Ownership of DMTIMER5: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer5Owned;
    /** Ownership of DMTIMER6: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer6Owned;
    /** Ownership of DMTIMER7: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer7Owned;
    /** Ownership of DMTIMER8: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  dmtimer8Owned;
    /** Ownership of MMCSD2: 1 ==> DSP owns the peripheral, 0 ==> DSP doesn't own the peripheral */
    Bool  mmcsd2Owned;
} CSL_SswHwSetup ;


/** @brief This object contains the reference to the instance of SSW
 * opened using the @a CSL_sswOpen()
 *
 * The pointer to this, (typedefed as handle), is passed to all SSW CSL APIs. */
typedef struct CSL_SswObj {
    /** This is the mode in which the CSL instance is opened */
    CSL_OpenMode                        openMode;
    /** This is a unique identifier to the instance of SSW being
     * referred to by this object */
    CSL_Uid                                     uid;
    /** This is the variable that contains the current state of a
     * resource being shared by current instance of SSW with
     * other peripherals */
    CSL_Xio                                     xio;
    /** This is a pointer to the registers of the instance of SSW
     * referred to by this object */
    CSL_SswRegsOvly             regs;
    /** This is the instance of SSW being referred to by this object */
    CSL_SswNum                          sswNum;
} CSL_SswObj;

typedef struct CSL_SswObj *CSL_SswHandle;

/**************************************************************************\
* SSW global function declarations
\**************************************************************************/


 CSL_SswHandle  CSL_sswOpen(
    CSL_SswObj                *sswObj, /** Pointer to the SSW Handle Object - to be allocated by the user */
    CSL_SswNum                sswNum,
    CSL_OpenMode              openMode,
    CSL_SswHwSetup            *hwSetup,
    CSL_Status                *status
);

/*
 * ======================================================
 *   @func   CSL_sswClose
 *   @desc   Close (Invalidate) a SSW module (passed as handle).
 *                       The SSW module cannot be accessed any more.
 *
 *   @arg  hSsw
 *              Handle to SSW module
 *   @ret  CSL_Status
 *
 * ======================================================
*/

 CSL_Status  CSL_sswClose(
    CSL_SswHandle             hSsw
);

/*
 * ======================================================
 *   @func   CSL_sswHwSetup
 *   @desc
 *         Configures a SSW module using the different config structures.
 *
 *   @arg  hSsw
 *              Handle to the SSW module
 *   @arg  setup
 *              h/w setup structure for the SSW module
 *   @ret  CSL_Status
 *      The function will return an error if cannot complete the request
 *
 * ======================================================
*/

 CSL_Status  CSL_sswHwSetup(
    CSL_SswHandle             hSsw,
    CSL_SswHwSetup            *setup
);

/*
 * ======================================================
 *   @func   CSL_sswHwControl
 *   @desc   Controls the parameters of SSW module (passed as handle)
 *           There are various parameters such as
 *                 1) TODO
 *                 2) TODO
 *          .........
 *           Usage Constraints:
 *           SSW should have been opened before with a valid handle
 *
 *          The are various Operations that can be done here are:
 *           1) TODO
 *           2) TODO
 *           3) TODO
 *           4) TODO
 *          .........
 *
 *           The function will return an error if cannot complete the request
 *
 *   @arg  hSsw
 *              Handle to the SSW module
 *   @arg  cmd
 *              The operation to be performed.
 *   @arg  cmdArg
 *              This is a void pointer, and will be cast to the required type.
 *   @ret  CSL_Status
 *
 * ======================================================
*/

 CSL_Status  CSL_sswHwControl(
    CSL_SswHandle           hSsw,
    CSL_SswHwControlCmd cmd,
    void                *cmdArg
);

/*
 * ======================================================
 *   @func   CSL_sswInit
 *   @desc
 *         This is SSW module's init function. Currently it does nothing.
 *         Needs to be called before using SSW module.
 *
 *   @ret  void
 *      Nothing.
 *
 * ======================================================
*/

 CSL_Status  CSL_sswInit(
    void
);

/*
 * ======================================================
 *   @func   CSL_sswGetHwStatus
 *   @desc
 *         Traces the various parameters of SSW module(passed as handle)
 *
 *           Usage Constraints:
 *           SSW module should have been opened before with a valid handle
 *
 *           The are various Operations that can be done here are:
 *           1) TODO
 *           2) TODO
 *          .......
 *
 *   @arg  hSsw
 *              Handle to the SSW module
 *   @arg  query
 *              The Values to be traced back
 *   @arg  response
 *              This is a void pointer, and will be cast to the required type.
 *   @ret  CSL_Status
 *           The function will return an error if cannot complete the request
 *
 * ======================================================
*/

 CSL_Status  CSL_sswGetHwStatus(
    CSL_SswHandle                       hSsw,
    CSL_SswHwStatusQuery        query,
    void*                                       response
);

#ifdef __cplusplus
}
#endif

#endif
