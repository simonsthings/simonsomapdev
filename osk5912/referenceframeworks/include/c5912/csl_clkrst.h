/** @mainpage CLKRST CSL (DSP side)
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for the CLKRST module across various devices. The CSL developer is expected to refer to this document while designing APIs for these modules. Some of the listed APIs may not be applicable to a given CLKRST module. While in other cases this list of APIs may not be sufficient to cover all the features of a particular CLKRST Module. The CSL developer should use his discretion in designing new APIs or extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:    Chip Support Library
 *   -# API:    Application Programmer Interface
 *   -# CLKRST: Clock Generator & System Reset
 *
 * @subsection References
 *    -# CSL-001-DES, CSL 3.x Design Specification Document Version 1.03
 *    -# SWPU062C, OMAP1610 Multimedia Processor Technical Reference Manual, November 2003, Chapter 2 Section 4
 *
 */

/** @file csl_clkrst.h
 *
 *  @brief    Header file for functional layer of CSL
 *
 *  Description
 *    - The different enumerations, structure definitions and function declarations
 *
 *  Modification 1
 *    - modified on: 14/04/2004
 *    - reason: Created the initial version
 *
 *  @date 29th March, 2004
 *  @author Sumant S. NaikKhanvte
 */

#ifndef _CSL_CLKRST_H_
#define _CSL_CLKRST_H_

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_clkrst.h>


/**************************************************************************\
* CLKRST global macro declarations
\**************************************************************************/

/**************************************************************************\
* CLKRST global typedef declarations
\**************************************************************************/


/** @brief Enumeration for CLKRST hardware control commands */
typedef enum {
        /** send the selected domains into reset : argument : @a Uint16 * */
        CSL_CLKRST_CMD_RESET_IN  =  1,
        /** bring the selected domains out of reset : argument : @a Uint16 * */
        CSL_CLKRST_CMD_RESET_OUT,
        /** enable the selected clocks. The argument is a bit vector formed by bitwise-ORing 1 or more members of @a CSL_ClkrstModule : argument : @a Uint16 * */
        CSL_CLKRST_CMD_CLK_ENABLE,
        /** disable the selected clocks. The argument is a bit vector formed by bitwise-ORing 1 or more members of @a CSL_ClkrstModule : argument : @a Uint16 * */
        CSL_CLKRST_CMD_CLK_DISABLE
} CSL_ClkrstHwControlCmd;


/** @brief Enumeration for CLKRST hardware status queries */
typedef enum {
        /** query for the status of CLKRST (source of reset & clock scheme) : argument : @a CSL_ClkrstStatus * */
        CSL_CLKRST_QUERY_STATUS  =  1,
        /** query the existing setting of the clock enable/disable. The argument returned is a bitwise-ORed bitmask of @a CSL_ClkrstModule : argument : @a Uint16 * */
        CSL_CLKRST_QUERY_CLKIDLEENTRY_SETTING
} CSL_ClkrstHwStatusQuery;


/** @brief Enumeration for all the idle-entry & enable/disable clocks of modules */
typedef enum {
        /** UART module */
        CSL_CLKRST_MODULE_UART  =  (1 << 0),
        /** Peripheral module */
        CSL_CLKRST_MODULE_PER   =  (1 << 1),
        /** Reference peripheral module */
        CSL_CLKRST_MODULE_XORP  =  (1 << 2),
        /** Watchdog timer module */
        CSL_CLKRST_MODULE_WDT   =  (1 << 3)
} CSL_ClkrstModule;


/** @brief Enumeration for CLKRST clocking schemes */
typedef enum {
        /** Fully synchronous clocking scheme */
        CSL_CLKRST_CLKSCHEME_FULLSYNC  =  CSL_CLKRST_DSP_SYSST_CLOCK_SELECT_FULL_SYNC,
        /** Synchronous scalable clocking scheme */
        CSL_CLKRST_CLKSCHEME_SYNCSCAL  =  CSL_CLKRST_DSP_SYSST_CLOCK_SELECT_SYNC_SCAL,
        /** bypass */
        CSL_CLKRST_CLKSCHEME_BYPASS    =  CSL_CLKRST_DSP_SYSST_CLOCK_SELECT_BYPASS,
        /** Mixed mode 3 clocking scheme */
        CSL_CLKRST_CLKSCHEME_MM3       =  CSL_CLKRST_DSP_SYSST_CLOCK_SELECT_MM3,
        /** Mixed mode 4 clocking scheme */
        CSL_CLKRST_CLKSCHEME_MM4       =  CSL_CLKRST_DSP_SYSST_CLOCK_SELECT_MM4
} CSL_ClkrstClkScheme;


/** @brief Enumeration for input clock sources */
typedef enum {
        /** DSPTIM_CK supplied by input reference clock */
        CSL_CLKRST_CLKSRC_TIM_IPREFCK   =  CSL_FMKT(CLKRST_DSP_CKCTL_TIMXO, IP_REF_CK),
        /** DSPTIM_CK supplied by CKGEN2 clock */
        CSL_CLKRST_CLKSRC_TIM_CKGEN2    =  CSL_FMKT(CLKRST_DSP_CKCTL_TIMXO, CK_GEN2)
} CSL_ClkrstClkSrc;


/** @brief Enumeration for MPU's idle status */
typedef enum {
        /** MPU megacell is in global-idle state */
        CSL_CLKRST_MPUSTATUS_IDLE    =  CSL_CLKRST_DSP_SYSST_IDLE_ARM_IDLE,
        /** MPU megacell is active */
        CSL_CLKRST_MPUSTATUS_ACTIVE  =  CSL_CLKRST_DSP_SYSST_IDLE_ARM_ACTIVE
} CSL_ClkrstMpuStatus;


/** @brief Enumeration for the different reset domains controlled through software */
typedef enum {
        /** peripherals under software reset control */
        CSL_CLKRST_RESETTYPE_PER    =  CSL_FMKT(CLKRST_DSP_RSTCT2_PER_EN, ENABLED),
        /** peripherals under software & watch dog reset control */
        CSL_CLKRST_RESETTYPE_WDPER  =  CSL_FMKT(CLKRST_DSP_RSTCT2_WD_PER_EN, ENABLED)
} CSL_ClkrstResetType;


/** @brief Enumeration for the different sources for reset */
typedef enum {
        /** Power-on reset */
        CSL_CLKRST_RESETSRC_POR    =  CSL_FMKT(CLKRST_DSP_SYSST_POR, YES),
        /** external reset */
        CSL_CLKRST_RESETSRC_EXT    =  CSL_FMKT(CLKRST_DSP_SYSST_EXT_RST, YES),
        /** MPU watchdog timer reset */
        CSL_CLKRST_RESETSRC_MPUWD  =  CSL_FMKT(CLKRST_DSP_SYSST_ARM_WDRST, YES),
        /** global software reset */
        CSL_CLKRST_RESETSRC_GLOB   =  CSL_FMKT(CLKRST_DSP_SYSST_GLOB_SWRST, YES),
        /** DSP watchdog timer reset */
        CSL_CLKRST_RESETSRC_DSPWD  =  CSL_FMKT(CLKRST_DSP_SYSST_DSP_WDRST, YES)
} CSL_ClkrstResetSrc;


/** @brief this object contains the reference to the instance of CLKRST opened using the @a CSL_clkrstOpen()
 *
 * The pointer to this is passed to all CLKRST CSL APIs. */
typedef struct CSL_ClkrstObj {
    /** This is the mode in which the CSL instance is opened */
    CSL_OpenMode        openMode;
    /** This is a unique identifier to the instance of CLKRST being referred to by this object */
    CSL_Uid             uid;
    /** This is the variable that contains the current state of a resource being shared by current instance of CLKRST with other peripherals */
    CSL_Xio             xio;
    /** This is a pointer to the registers of the instance of CLKRST referred to by this object */
    CSL_ClkrstRegsOvly  regs;
    /** This is the instance of CLKRST being referred to by this object */
    Int16               perNum;
} CSL_ClkrstObj;

/** @brief this is a pointer to @a CSL_ClkrstObj & is passed as the first parameter to all CLKRST CSL APIs */
typedef struct CSL_ClkrstObj *CSL_ClkrstHandle;


/** @brief part of @a CSL_ClkrstHwSetup used to configure clock dividers
 *
 * This structure is used to configure the clock dividers for the different clocks. The permissible values are 0-3. The clock is divided by 2^(value) */
typedef struct CSL_ClkrstClkDiv {
        /** divider value for peripheral clock */
        Uint8  perDiv;
} CSL_ClkrstClkDiv;


/** @brief main structure that is used to setup the CLKRST 
 *
 * This structure is used to setup or obtain the existing setup of CLKRST using @a CSL_clkrstHwSetup() & @a CSL_clkrstGetHwSetup() functions respectively. If a particular member pointer is null, then these functions do not setup or get the setup of the corresponding part of CLKRST respectively */
typedef struct CSL_ClkrstHwSetup {
        /** This pointer is used to configure the sources for Timer clock pins */
        CSL_ClkrstClkSrc  *clkSourcePtr;
        /** This pointer is used to configure the idle entry modes of different clock pins. This takes a bitmask made by bitwise-ORing @a CSL_ClkrstModule except @a CSL_CLKRST_MODULE_UART */
        Uint16            *clkIdleEntryPtr;
        /** pointer to the structure that hold information about clock dividers needed */
        CSL_ClkrstClkDiv  *clkDivPtr;
} CSL_ClkrstHwSetup;


typedef struct CSL_ClkrstStatus {
        /** the clock scheme used for the OMAP Gigacell */
        CSL_ClkrstClkScheme  clkScheme;
        /** status of the reset source for OMAP Gigacell; the returned value will be a member of @CSL_ClkrstResetSrc */
        Uint8                status;
        /** idle mode status of the MPU */
        CSL_ClkrstMpuStatus  mpuStatus;
} CSL_ClkrstStatus;


/** The default value for @a CSL_ClkrstHwSetup structure */
#define CSL_CLKRST_HWSETUP_DEFAULTS { \
           NULL,                      \
           NULL,                      \
           NULL                       \
        }

/** The default value for @a CSL_ClkrstClkDiv structure */
#define CSL_CLKRST_CLKDIV_DEFAULTS { \
           0,                        \
           0,                        \
           0                         \
        }


/** @brief initializes the CLKRST
 *
 * This function is idempotent in that calling it many times is same as calling it once. This function initializes the CLKRST CSL data structures.
 * 
 * <b> Usage Constraints: </b>
 * CSL system initialization must be successfully completed by invoking @a CSL_sysInit() before calling this function. This function should be called before using any of the CSL APIs
 * 
 * @b Example:
 * @verbatim


   ...
   CSL_sysInit();
   if (CSL_SOK != CSL_clkrstInit()) {
       return;
   }
   @endverbatim
 * 
 * @return returns the status of the operation
 * 
 */
CSL_Status  CSL_clkrstInit(
    void
);


/** @brief opens if possible the instance of CLKRST requested
 * 
 * Reserves the specified clkrst for use. The deviced can be re-opened anytime after it has been normally closed if so required. The handle returned by this call is input as an essential argument for rest of the APIs described for this module.
 * 
 * <b> Usage Constraints: </b>
 * The CSL system as well as CLKRST must be succesfully initialized via @a CSL_sysInit() and @a CSL_clkrstInit() before calling this function. Memory for the @a CSL_ClkrstObj must be allocated outside this call.
 * 
 * @b Example:
 * @verbatim


   CSL_ClkrstObj     clkrstObj;
   CSL_ClkrstHwSetup clkrstSetup;
   CSL_Status        status;
   ...
   hClkrst = CSL_clkrstOpen(&clkrstobj,
                            CSL_CLKRST_0,
                            CSL_EXCLUSIVE,
                            &clkrstSetup,
                            &status);
   @endverbatim
 * 
 * @return returns a handle @a CSL_ClkrstHandle to the requested instance of CLKRST if the call is successful, otherwise, a @a NULL is returned
 * 
 */
CSL_ClkrstHandle CSL_clkrstOpen(
    /** pointer to the object that holds reference to the instance of CLKRST requested after the call to @a CSL_clkrstOpen() */
    CSL_ClkrstObj                            *hClkrstObj,
    /** instance of CLKRST to which a handle is requested */
    CSL_ClkrstNum                            clkrstNum,
    /** specifies if CLKRST should be opened with excusive or share access to the associate pins */
    CSL_OpenMode                            openMode,
    /** if a valid structure is passed (not @a NULL), then the @a CSL_clkrstHwSetup() is called with this parameter */
    CSL_ClkrstHwSetup                        *hwSetup,
    /** This returns the status (success/errors) of the call */
    CSL_Status                              *status
);



/** @brief closes the instance of CLKRST requested
 *
 * Unreserves the clkrst identified by the handle.
 *
 * <b> Usage Constraints: </b>
 * Both @a CSL_clkrstInit() and @a CSL_clkrstOpen() must be called successfully in that order before @a CSL_clkrstClose() can be called.
 *
 * @b Example:
 * @verbatim


   CSL_ClkrstHandle hClkrst;
   ...
   CSL_clkrstClose(hClkrst);
   @endverbatim
 * 
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_clkrstClose(
    /** pointer to the object that holds reference to the required instance of CLKRST */
    CSL_ClkrstHandle                         hClkrst
);



/** Configures the CLKRST using the values passed in through the setup structure. The setup structure consists of pointers to substructures. If these pointers are set to @a NULL, then the setup corresponding to that sub-structure is not done.
 * 
 * <b> Usage Constraints: </b>
 * Both @a CSL_clkrstInit() and @a CSL_clkrstOpen() must be called successfully in that order before this function can be called. The main setup structure consists of pointers to sub-structures. The user has to allocate space for & fill in the main setup structure & it's sub-structures appropriately before calling this function. If some part of CLKRST is not to be configured, then the corresponding setup sub-structure pointers must be set to @a NULL
 * 
 * @b Example:
 * @verbatim


     CSL_ClkrstHandle hClkrst;
     CSL_ClkrstHwSetup hwSetup = CSL_CLKRST_HWSETUP_DEFAULTS;
     Uint16 clkIdleEntry = (CSL_CLKRST_MODULE_PER |
                            CSL_CLKRST_MODULE_WDT |
                            CSL_CLKRST_MODULE_XORP);
     ...
     hwSetup.clkIdleEntryPtr = &clkTdleEntry;
     CSL_clkrstHwSetup(hClkrst, &hwSetup);
   @endverbatim
 * 
 * @return returns the status of the setup operation
 * 
 */
CSL_Status  CSL_clkrstHwSetup(
    /** pointer to the object that holds reference to the required instance of CLKRST */
    CSL_ClkrstHandle                         hClkrst,
    /** pointer to setup structure which contains the information to program CLKRST to a useful state */
    CSL_ClkrstHwSetup                        *setup
);


/** Gets the configuration of the CLKRST into the setup structure. The setup structure consists of pointers to substructures. If these pointers are set to @a NULL, then the setup corresponding to that sub-structure is not fetched.
 * 
 * <b> Usage Constraints: </b>
 * Both @a CSL_clkrstInit() and @a CSL_clkrstOpen() must be called successfully in that order before this function can be called. The main setup structure consists of pointers to sub-structures. The user has to allocate space for & fill in the main setup structure & it's sub-structures appropriately before calling this function. Those parts of CLKRST whose information is not required should have their correspondinf setup sub-structures set to NULL
 * 
 * @b Example:
 * @verbatim


     CSL_ClkrstHandle hClkrst;
     CSL_ClkrstHwSetup hwSetup = CSL_CLKRST_HWSETUP_DEFAULTS;
     Uint16 clkIdleEntry;
     ...
     hwSetup.clkIdleEntryPtr = &clkTdleEntry;
     CSL_clkrstGetHwSetup(hClkrst, &hwSetup);
   @endverbatim
 * 
 * @return returns the status of the setup operation
 * 
 */
CSL_Status  CSL_clkrstGetHwSetup(
    /** pointer to the object that holds reference to the required instance of CLKRST  */
    CSL_ClkrstHandle                         hClkrst,
    /** pointer to setup structure which contains the information to program CLKRST to a useful state */
    CSL_ClkrstHwSetup                        *setup
);


/** Takes a command with an optional argument & implements it. This function is used to carry out the different operations performed by clkrst.
 * 
 * <b> Usage Constraints: </b>
 * Both @a CSL_clkrstInit() and @a CSL_clkrstOpen() must be called successfully in that order before @a CSL_clkrstHwControl() can be called. For the argument type that can be @a void* casted & passed with a particular command refer to @a CSL_clkrstHwControlCmd
 * 
 * @b Example:
 * @verbatim


      CSL_clkrstHandle hclkrst;
      CSL_Status status;
      Uint16 clkEn = (CSL_CLKRST_MODULE_WDT |
                      CSL_CLKRST_MODULE_PER |
                      CSL_CLKRST_MODULE_XORP);
      ...
      status = CSL_clkrstHwControl(hclkrst,
                                   CSL_CLKRST_CMD_CLK_ENABLE,
                                   &clkEn);
   @endverbatim
 * 
 * @return returns the status of the operation
 * 
 */
CSL_Status  CSL_clkrstHwControl(
    /** pointer to the object that holds reference to the required instance of CLKRST */
    CSL_ClkrstHandle                         hClkrst,
    /** the command to this API which indicates the action to be taken */
    CSL_ClkrstHwControlCmd                   cmd,
    /** an optional argument @a void* casted */
    void                                    *arg
);


/** Gets the status of the different operations or the current setup of CLKRST. The status is returned through @a CSL_ClkrstHwSetup. The obtaining of status
 * is the reverse operation of @a CSL_clkrstHwSetup() function.
 * 
 * <b> Usage Constraints: </b>
 * Both @a CSL_clkrstInit() and @a CSL_clkrstOpen() must be called successfully in that order before @a CSL_clkrstGetHwStatus() can be called. For the argument type that can be @a void* casted & passed with a particular command refer to @a CSL_ClkrstHwStatusQuery
 * 
 * @b Example:
 * @verbatim

 
    CSL_ClkrstHandle hClkrst;
    CSL_Status status;
    Uint16 response;
    ...
    status = CSL_clkrstGetHwStatus(hClkrst,
                                   CSL_CLKRST_QUERY_CLK_SETTING,
                                   &response);
   @endverbatim
 * 
 * @return returns the status of the operation
 * 
 */
CSL_Status  CSL_clkrstGetHwStatus(
    /** pointer to the object that holds reference to the required instance of CLKRST */
    CSL_ClkrstHandle                         hClkrst,
    /** the query to this API which indicates the status/setup to be returned */
    CSL_ClkrstHwStatusQuery                  query,
    /** placeholder to return the status; @a void* casted */
    void                                    *response
);



#endif
