/*  =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   provided
*   ==========================================================
*/
/** @mainpage IRDA Module
*
* @section Introduction
*
* @subsection xxx Purpose and Scope
* The purpose of this document is to identify a set of common CSL APIs for
* the IRDA Module across various devices. The CSL developer
* is expected to refer to this document while implementing APIs for these
* modules . Some of the listed APIs may not be applicable to a given IRDA
* While in other cases this list of APIs may not be
* sufficient to cover all the features of a particular IRDA Module.
* The CSL developer should use his/her discretion in
* designing new APIs or extending the existing ones to cover these.
*
*
*
* @subsection aaa Terms and Abbreviations
*   -# CSL  :  Chip Support Library
*   -# API  :  Application Programmer Interface
*   -# IRDA :  InfraRed Data Association
*
* @subsection References
*    -# CSL 3.x Technical Requirements Specifications Version 0.5, dated
*       May 14th, 2003
*    -# The OMAP 1610 Technical Reference Manual (SWPU062B)
*
* @subsection Assumptions
*     The abbreviations CSL, IRDA have been used throughout this
*     document to refer to the OMAP 1610 IRDA Module.
*/

/** @file csl_irda.h
*
*  @brief    Header file for functional layer of CSL
*
*  Description
*    - The different enumerations, structure definitions
*      and function declarations
*  @date 27th Jan, 2004
*  @author Ruchika Kharwar
*/

#ifndef _CSL_IRDA_H_
#define _CSL_IRDA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_irda.h>

/**************************************************************************\
* IRDA global macro declarations
\**************************************************************************/
/* Macros - events, used for
*  Interrupt Enable Symbols
*/
/**
@defgroup CSL_IRDA_API IRDA
*/
/**
@defgroup CSL_IRDA_DATASTRUCT  Data Structures
@ingroup CSL_IRDA_API
*/
/**
@defgroup CSL_IRDA_SYMBOL  Symbols
@ingroup CSL_IRDA_API
*/
/**
@defgroup CSL_IRDA_ENUM  Enumerated Data Types
@ingroup CSL_IRDA_API
*/

/**
@defgroup CSL_IRDA_FUNCTION  Functions
@ingroup CSL_IRDA_API
*/
/** @defgroup CSL_IRDA_EVENT_SYMBOL Events
*   @ingroup CSL_IRDA_SYMBOL
*
* @{ */
/** Enable/Receive EOT Interrupt  */
#define  CSL_IRDA_EVENT_EOT           0x80
/** Enable/Receive Receive Line Status Interrupt  */
#define  CSL_IRDA_EVENT_LINESTS       0x40
/** Enable/Receive Transmit Status Interrupt  */
#define  CSL_IRDA_EVENT_TXSTS         0x20
/** Enable/Receive Status FIFO trigger Level Interrupt  */
#define  CSL_IRDA_EVENT_STSFIFO_TRIG  0x10
/** Enable/Receive Receive Overrun Error Interrupt  */
#define  CSL_IRDA_EVENT_RXOVERRUN     0x08
/** Enable/Receive Last Byte of Frame in Rx FIFO interrupt     */
#define  CSL_IRDA_EVENT_LAST_RXBYTE   0x04
/** Enable/Receive THR interrupt     */
#define  CSL_IRDA_EVENT_THR           0x02
/** Enable/Receive RHR interrupt     */
#define  CSL_IRDA_EVENT_RHR           0x01
/**
@} */

/** @defgroup CSL_IRDA_TXTRIGGER_SYMBOL Tx Trigger Levels
*   @ingroup CSL_IRDA_SYMBOL
*
* @{ */
/** 8  spaces                 */
#define CSL_IRDA_TXTRIGLEVEL_8   8
/** 16 spaces                             */
#define CSL_IRDA_TXTRIGLEVEL_16  16
/** 32 spaces                             */
#define CSL_IRDA_TXTRIGLEVEL_32  32
/** 56 spaces                             */
#define CSL_IRDA_TXTRIGLEVEL_56  56
/**
@}*/

/** @defgroup CSL_IRDA_RXTRIGGER_SYMBOL Rx Trigger Levels
*   @ingroup CSL_IRDA_SYMBOL
*
* @{ */
/** 8  characters                      */
#define CSL_IRDA_RXTRIGLEVEL_8   8
/** 16 characters                      */
#define CSL_IRDA_RXTRIGLEVEL_16  16
/** 56 characters                      */
#define CSL_IRDA_RXTRIGLEVEL_56  56
/** 60 characters                      */
#define CSL_IRDA_RXTRIGLEVEL_60  60
/**
@}*/
/** @defgroup CSL_IRDA_WAKEUP_SYMBOL WakeUp Events
*   @ingroup CSL_IRDA_SYMBOL
*
* @{ */
/* Macros - IRDA Wake Up Event Flags
*  The application may also use these flags to allow specified source to enable wakeup
*/
/** RX/RXIR Activity can wake Up the system          */
#define CSL_IRDA_WAKEUPEVENT_RX   0x10
/** RHR Activity can wake Up the system              */
#define CSL_IRDA_WAKEUPEVENT_RHR  0x20
/** Receive Line Status can wake Up the system       */
#define CSL_IRDA_WAKEUPEVENT_LS   0x40
/**
@}*/

/** @defgroup CSL_IRDA_LINESTS_SYMBOL Line Status/Errors
*   @ingroup CSL_IRDA_SYMBOL
*
* @{ */
/* Macros - IRDA Line Status/Errors
*/
/** Transmit holding Register Empty */
#define CSL_IRDA_LINESTS_THR_EMPTY                      0x80
/** Status FIFO Full */
#define CSL_IRDA_LINESTS_STSFIFO_FULL                   0x40
/** Rx FIFO contains the last byte of the frame  */
#define CSL_IRDA_LINESTS_LAST_RXBYTE                            0x20
/** Frame too long */
#define CSL_IRDA_LINESTS_FRAMETOOLONG                           0x10
/** Abort Pattern received  */
#define CSL_IRDA_LINESTS_RXABORT                                        0x08
/** CRC Error in the Frame at the top of the STATUS FIFO */
#define CSL_IRDA_LINESTS_CRC_ERROR                                      0x04
/** Status FIFO Empty */
#define CSL_IRDA_LINESTS_STSFIFO_EMPTY                      0x02
/**  No Data in the receive FIFO  */
#define CSL_IRDA_LINESTS_RXFIFO_EMPTY                               0x01
/**
@}*/

/**@defgroup CSL_IRDA_BAUD_SYMBOL Baud Rate
*   @ingroup CSL_IRDA_SYMBOL
@{*/
/** Macros - Symbols for Baud Rate setting
*/

/**2.4Kb/s */
#define  CSL_IRDA_BAUD_2_4K        2400
/**9.6 Kb/s */
#define  CSL_IRDA_BAUD_9_6K        9600
/**19.2Kb/s */
#define  CSL_IRDA_BAUD_19_2K       19200
/** 38.4Kb/s*/
#define  CSL_IRDA_BAUD_38_4K       38400
/** 57.6 Kb/s */
#define  CSL_IRDA_BAUD_57_6K       57600
/** 115.2 Kb/s   */
#define  CSL_IRDA_BAUD_115_2K      115200
/** 0.576 Mb/s */
#define  CSL_IRDA_BAUD_0_576M      576000
/**4 Mb/s */
#define  CSL_IRDA_BAUD_4M          4000000

/**
@}*/

/**************************************************************************\
* IRDA global typedef declarations
\**************************************************************************/

/**@defgroup CSL_IRDA_MODE_ENUM  Mode
*  @ingroup CSL_IRDA_ENUM
@{*/
/** @brief  Enums for the Mode of the IRDA module
*
* This is used for control/query commands to set/query the mode of the IRDA module.
*/
typedef enum {
    /** IRDA Slow Infrared Mode */
    CSL_IRDA_MODE_SIR =  CSL_IRDA_MDR1_MODE_SELECT_SIR,
    /** IRDA Medium Infrared Mode */
    CSL_IRDA_MODE_MIR =  CSL_IRDA_MDR1_MODE_SELECT_MIR,
    /** IRDA Fast Infrared Mode */
    CSL_IRDA_MODE_FIR =  CSL_IRDA_MDR1_MODE_SELECT_FIR,
    /** Default State */
    CSL_IRDA_MODE_NONE =   CSL_IRDA_MDR1_MODE_SELECT_DISABLE
}CSL_IrdaMode;
/**
@}*/

/**@defgroup CSL_IRDA_DMAMODE_ENUM  DMA Mode
*   @ingroup CSL_IRDA_ENUM
@{*/
/** @brief  Enums for the DMA Mode of the IRDA module
*
* This is used for control/query commands to set/query the DMA mode of the IRDA module.
*/
typedef enum {
    /** No DMA mode */
    CSL_IRDA_DMAMODE_NONE =  CSL_IRDA_SCR_DMA_MODE_2_00,
    /** DMA mode 1 */
    CSL_IRDA_DMAMODE_RXTX =  CSL_IRDA_SCR_DMA_MODE_2_01,
    /** DMA mode 2 */
    CSL_IRDA_DMAMODE_RX   =  CSL_IRDA_SCR_DMA_MODE_2_02,
    /** DMA mode 3 */
    CSL_IRDA_DMAMODE_TX   =  CSL_IRDA_SCR_DMA_MODE_2_03
}CSL_IrdaDmaMode;
/**
@}*/

/**@defgroup CSL_IRDA_STATUSFIFOTRIGGER_ENUM  Irda Status Trigger.
*   @ingroup CSL_IRDA_ENUM
@{*/
/** @brief  Enums for the Irda Status Trigger
*
* This is used for setting up the Irda Status Trigger Threshold.
*/
typedef enum {
    /** Status Threshold set to 1 */
        CSL_IRDA_STATUS_TRIGGER_1  = CSL_IRDA_MDR2_STS_FIFO_TRIG_1,
        /** Status Threshold set to 4 */
        CSL_IRDA_STATUS_TRIGGER_4  = CSL_IRDA_MDR2_STS_FIFO_TRIG_4,
        /** Status Threshold set to 7 */
        CSL_IRDA_STATUS_TRIGGER_7  = CSL_IRDA_MDR2_STS_FIFO_TRIG_7,
        /** Status Threshold set to 8 */
        CSL_IRDA_STATUS_TRIGGER_8  = CSL_IRDA_MDR2_STS_FIFO_TRIG_8
}CSL_IrdaStatusFifoTrigger;
/**
@}*/
/**@defgroup CSL_IRDA_FRAMEEND_ENUM  Frame End Mode
*   @ingroup CSL_IRDA_ENUM
@{*/
/** @brief  Enums used to specify the "End of Frame" mode
*
* This is used to set the Frame End Method.
*/
typedef enum {
    /* The Frame Length in the TXFLL, TXFLH used to mark EOT */
        CSL_IRDA_FRAME_ENDMODE_LENGTH  = 0,
        /* The End of transmission done when the ACREG */
        CSL_IRDA_FRAME_ENDMODE_EOT = 1
}CSL_IrdaFrameEndMode;
/**
@}*/
/**@defgroup CSL_IRDA_SIPMODE_ENUM  Sip Mode
*   @ingroup CSL_IRDA_ENUM
@{*/
/** @brief  Enums used to specify the "Sip Mode"
*
* This is used to set the Sip Generation.
*/
typedef enum {
    /** Sip Mode is manual */
        CSL_IRDA_SIPMODE_LENGTH  = CSL_IRDA_MDR1_SIP_MODE_MANUAL,
        /** Sip Mode is automatic */
        CSL_IRDA_SIPMODE_AUTOMATIC = CSL_IRDA_MDR1_SIP_MODE_AUTOMATIC
}CSL_IrdaSipMode;
/**
@}*/

/**@defgroup CSL_IRDA_CONTROLCMD_ENUM Enums for Control Commands
*  @ingroup CSL_IRDA_CONTROL_API
@{*/
/** @brief Enumeration for control commands passed to @a
CSL_irdaHwControl()
*
* This is the set of commands that are passed to the @a CSL_irdaHwControl()
* with an optional argument type-casted to @a void* .
* The arguments to be passed with each enumeration (if any) are specified
* next to the enumeration.
*/

typedef enum {
/**@brief  Set the Interrupt Enable for IRDA Events
This command does the following :- \n
1.      Programmation of IER Register \n
@param (CSL_BitMask16*)
@return CSL_SOK

*/
    CSL_IRDA_CMD_SETEVENTS  ,
/**@brief  Soft Resets the module
This command does the following :- \n
1.      Programmation of SOFTRESET in SYSC \n

@param None
@return CSL_SOK
*/
    CSL_IRDA_CMD_RESET          ,
/**@brief Sets events qualified to do wake up.
This command does the following :- \n
1.      Programmation of WER Register \n

@param (CSL_BitMask16*)
@return CSL_SOK
*/
    CSL_IRDA_CMD_SETWAKEUPEVENTS,                       /* OR'd Events by user to enableWakeUp        */
/**@brief Read specified characters from Rx FIFO.
A pointer to the object of type @a CSL_IrdaXfer needs to be passed as the third
argument to @a CSL_irdaHwControl() with this command.
The 'xfernum' field acts as output parameter reporting the actual number of bytes read.
This command does the following :- \n
1.      Programmation of LSR Register \n
2.      Programmation of RHR Register
@param (CSL_IrdaXfer*)
@return CSL_SOK
*/
    CSL_IRDA_CMD_READ       ,
/**@brief  Write specified num of character into TxFIFO.
A pointer to the object of type @a CSL_IrdaXfer needs to be passed as the third
argument to @a CSL_irdaHwControl() with this command.
The 'xfernum' field acts as output parameter reporting the actual number of bytes written.
This command does the following :- \n
1.      Programmation of THR Register \n
2.      Programmation of SSR Register
@param (CSL_IrdaXfer*)
@return CSL_SOK
*/
    CSL_IRDA_CMD_WRITE       ,

/**@brief Internal OCP clock gating strategy is set to "Clock is running"
This command does the following :- \n
1.      Programmation of AUTOIDLE field of SYSC Register
@param None
@return CSL_SOK
*/
    CSL_IRDA_CMD_SYSAUTOIDLE_CLKRUN,
/**@brief Apply Auto OCP clock gating strategy.
This command does the following :- \n
1.      Programmation of AUTOIDLE field of SYSC Register
@param None
@return CSL_SOK
*/
    CSL_IRDA_CMD_SYSAUTOIDLE_AUTO_OCP ,
/**@brief System Wake Up Feature control enabled
This command does the following :- \n
1.      Programmation of ENAWAKEUP field of SYSC Register
@param None
@return CSL_SOK
*/
    CSL_IRDA_CMD_SYSWAKE_ENABLE   ,
/**@brief System Wake Up Feature control disabled
This command does the following :- \n
1.      Programmation of ENAWAKEUP field of SYSC Register
@param None
@return CSL_SOK
*/
    CSL_IRDA_CMD_SYSWAKE_DISABLE ,
/**@brief  Set power control management to "Smart Idle"
This command does the following :- \n
1.      Programmation of IDLEMODE  field of SYSC Register
@param None
@return CSL_SOK
*/
    CSL_IRDA_CMD_SYSIDLEMODE_SMART ,
/**@brief  Set power control management to "Force Idle"
This command does the following :- \n
1.      Programmation of IDLEMODE  field of SYSC Register
@param None
@return CSL_SOK
*/
      CSL_IRDA_CMD_SYSIDLEMODE_FORCE ,
/**@brief  Set power control management to "None"
This command does the following :- \n
1.      Programmation of IDLEMODE  field of SYSC Register
@param None
@return CSL_SOK
*/
      CSL_IRDA_CMD_SYSIDLEMODE_NONE  ,
/**@brief Enable Sleep Mode
This command does the following :- \n
1.      Programmation of IR_SLEEP  field of MDR1 Register
@param None
@return CSL_SOK
*/
      CSL_IRDA_CMD_SLEEP_ENABLE,
/**@brief Disable Sleep Mode
This command does the following :- \n
1.      Programmation of IR_SLEEP  field of MDR1 Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_SLEEP_DISABLE,
/**@brief TXIR pin output forced high to configure the IRDA transceiver
This command does the following :- \n
1.      Programmation of SET_TXIR field of MDR1 Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_TXIR_SET,
/**@brief Store and Controlled TX start
This command does the following :- \n
1.      Programmation of SCT field of MDR1 Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_SCT_ENABLE,
/**@brief Sets the Frame End Mode to "Frame-length Method "
This command does the following :- \n
1.      Programmation of FRAME_END_MODE field of MDR1 Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_FRAME_ENDMODE_LENGTH,
/**@brief Sets the Frame End Mode to "Set EOT Method "
This command does the following :- \n
1.      Programmation of FRAME_END_MODE field of MDR1 Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_FRAME_ENDMODE_EOT,
/**@brief Sets the transmit Frame Length
This command does the following :- \n
1.      Programmation of TXFLL Register \n
2.      Programmation of TXFLH Register
@param (Uint16*)
@return CSL_SOK
*/

          CSL_IRDA_CMD_TXFRAME_LEN,
/**@brief Reads the RESUME Register to clear the underrun error
This command does the following :- \n
1.      Programmation of Resume Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_RESUME,
/**@brief Sets the BOF length
This command does the following :- \n
1.      Programmation of EBLR Register
@param None
@return CSL_SOK
*/

      CSL_IRDA_CMD_BOFLENGTH,
/**@brief Sets the BOF character to 0xFF
This command does the following :- \n
1.      Programmation of BLR_XBOF_TYPE in BLR Register
@param (Uint16 *)
@return CSL_SOK
*/

          CSL_IRDA_CMD_BOF_FF,
/**@brief Sets the BOF character to 0xC0
This command does the following :- \n
1.      Programmation of BLR_XBOF_TYPE in BLR Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_BOF_CO,
/**@brief Sets the EOT bit
This should be used in case the "Set EOT Method" method is used for Frame End mode.
This command does the following :- \n
1.      Programmation of EOT_EN in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_EOT_SET,
/**@brief This should be used to ABORT the current transmitted frame.
This command does the following :- \n
1.      Programmation of ABORT_EN in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_ABORT,
/**@brief This should be used in case the "Stored and Controlled" type of \
transmission is used. When this command is issued, transmission commences.
This command does the following :- \n
1.      Programmation of SCTX_EN in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_TX_START,
/**@brief This should be used in MIR/FIR Modes only. A SIP is sent at the end of the current trasnmission
when this command is issued. This command should be used when the SIP generation mode chosen is set to
Manual.
This command does the following :- \n
1.      Programmation of SEND_SIP in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_SIP_SEND,
/**@brief Long stop bits cannot be transmitted, and TX underrun is enabled.
This command does the following :- \n
1.      Programmation of DIS_TX_UNDERRUN in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_TX_UNDERRUN_ENABLE,
/**@brief Long stop bits can be transmitted, and TX underrun is disabled.
This command does the following :- \n
1.      Programmation of DIS_TX_UNDERRUN in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_TX_UNDERRUN_DISABLE,
/**@brief Normal operation state of RXIR set.
This command does the following :- \n
1.      Programmation of DIS_TX_RX in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_RX_IR_ENABLE,
/**@brief RXIR input disabled.
This command does the following :- \n
1.      Programmation of DIS_TX_RX in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_RX_IR_DISABLE,
/**@brief SD pin set high.
This command does the following :- \n
1.      Programmation of SD_MOD in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_SDMODE_HIGH,
/**@brief SD pin set low.
This command does the following :- \n
1.      Programmation of SD_MOD in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_SDMODE_LOW,
/**@brief SIR pulse width selected to 3/16 of baud-rate pulse width.
This command does the following :- \n
1.      Programmation of PULSE_TYPE in ACREG Register
@param None
@return CSL_SOK
*/
          CSL_IRDA_CMD_PULSETYPE_316,
/**@brief SIR pulse width selected to 1.6us.
This command does the following :- \n
1.      Programmation of PULSE_TYPE in ACREG Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_PULSETYPE_16,
/**@brief Status FIFO is reset.
This command does the following :- \n
1.      Programmation of STS_FIFO_RESET in BLR Register
@param None
@return CSL_SOK
*/

          CSL_IRDA_CMD_STSFIFO_RESET
}CSL_IrdaHwControlCmd;
/** @}*/

/**@defgroup CSL_IRDA_QUERYCMD_ENUM Enums for Query Commands
*  @ingroup CSL_IRDA_QUERY_API
* @{ */

/** @brief Enumeration for queries passed to @a CSL_irdaGetHwStatus()
*
* This is used to get the status of different operations of IRDA.
* The arguments to be passed with each
* enumeration if any are specified next to the enumeration */

typedef enum {
/**@brief  Queries the interrupts events enabled in the IRDA Module
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_IRDA_QUERY_EVENTS,
/**@brief  Queries the wakeup events enabled in the IRDA Module
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_IRDA_QUERY_WAKEUP_EVENTS,
/**@brief Queries whether an IRTX_UNDERRUN occured.
@param (Bool*)
@return CSL_SOK
*/
        CSL_IRDA_QUERY_IRTX_UNDERRUN,
/**@brief Queries Receiver Line Status
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_IRDA_QUERY_LINESTATUS,
/**@brief Version number of the module
@param (Uint16*)
@return CSL_SOK
*/
        CSL_IRDA_QUERY_VERSION,
/**@brief Interrupt Source read from the Interrupt Identification register
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_IRDA_QUERY_INTRSOURCE,
/**@brief Query to find out if the  System Reset is complete or ongoing
@param (Bool*) \n
      TRUE Reset is done \n
      FALSE Reset is ongoing \n
@return CSL_SOK
*/
        CSL_IRDA_QUERY_RESETDONE,
/**@brief Query to find out if the Tx FIFO is full
@param (Bool*) \n
      TRUE Tx FIFO is full \n
      FALSE Tx FIFO is not full \n
@return CSL_SOK
*/
        CSL_IRDA_QUERY_TXFIFO_FULL,
/**@brief Query the last frame length and associated status
@param (CSL_IrdaFrameParam*) \n
@return CSL_SOK
*/
        CSL_IRDA_QUERY_RXFRAMEPARAM
}CSL_IrdaHwStatusQuery;
/**
@}*/


/**
*   @addtogroup CSL_IRDA_DATASTRUCT
* @{ */
/** @brief Structure used to query the Received Frame parameters.
*/
typedef struct CSL_IrdaFrameParam{
    /** Status of the FIFO Line Status */
        CSL_BitMask16  fifoLineStatus;
        /** Length of Frame at top of the FIFO */
        Uint16            frameLength;
}CSL_IrdaFrameParam;
/**
@} */
/**
*   @addtogroup CSL_IRDA_DATASTRUCT
* @{ */
/** @brief CSL_IrdaXfer is used when sending/receiving data.
*/
typedef struct CSL_IrdaXfer{
    /** Ptr to data transmit/receive buffer */
    Uint8  *buff;
    /** Number of bytes to be transferred*/
    Uint16  num;
    /** Number of bytes actually  transferred*/
    Uint16  xfernum;
}CSL_IrdaXfer;
/**
@} */
/** @addtogroup CSL_IRDA_SETUP_API
*
* @{ */
/** @brief CSL_IrdaHwCtrlSetup is a substructure of the CSL_IrdaHwSetup.
*
* that has the fields required for configuring for the IRDA Module.
*/
typedef struct CSL_IrdaHwCtrlSetup{
        /** Module Clock Frequency(Hz)                             */
        Uint32          moduleClock;
        /** Standard baudrate setting                              */
        Uint32    baudrate;
        /** Either 13x/16x/Autobaud mode                           */
    CSL_IrdaMode    mode;
    /* One of 4 possible DMA modes                                 */
        CSL_IrdaDmaMode dmaMode;
        /** Status FIFO trigger threshold              */
    CSL_IrdaStatusFifoTrigger    stsfifoTrig;
        /** A maximum 6 bit Receive  Trigger Level                 */
        Uint16                  rxTrigLevel;
        /** A maximum 6 bit Transmit Trigger Level                 */
    Uint16          txTrigLevel;
    /** OR'd combination of interrupts to be enabled     */
        CSL_BitMask16   interruptEnable;
        /** FIFO Enable/Disable                                          \n
            TRUE  - Enable FIFO  \n
            FALSE - Disable FIFO  \n
         */
        Bool            fifoenable;
        }CSL_IrdaHwCtrlSetup;

/** @brief CSL_IrdaTransferControl has all the fields required to configure IRDA \n
*   Transfer Parameters.
*/

typedef struct CSL_IrdaTransferControl {
    /** Sip Mode generation type - Auto/Manual */
    CSL_IrdaSipMode      sipMode;
    /** Frame End Mode */
    CSL_IrdaFrameEndMode frameEndMode;
    /** Received maximum frame length */
    Uint16                       rxMaxFramelen;
    /** Enable/disable Store Control \n
        TRUE :- Enable \n
        FALSE :- Disable
    */
        Bool                     storeControl;
}CSL_IrdaTransferControl;
/** @brief CSL_IrdaAdrressControl has all the fields required to configure IRDA \n
*   Addressing Parameters.
*/
typedef struct CSL_IrdaAddressControl {
    /** Address 1, this is used only if addr1Enable is true */
    Uint16    addr1;
    /** Address 2, this is used only if addr2Enable is true */
    Uint16    addr2;
    /** Address 1 Enable/Disable */
    Bool      addr1Enable;
    /** Address 2 Enable/Disable */
    Bool      addr2Enable;
}CSL_IrdaAddressControl;
/** @brief CSL_IrdaHwSetup has all the fields required to configure IRDA
*
* This structure has the substructures required to configure IRDA at
* Power-Up/Reset.
*
*/

typedef struct CSL_IrdaHwSetup {
        /** Configuration parameters at power Up/Reset
     */
    CSL_IrdaHwCtrlSetup* hwCtrlSetup;
    /** Configuration parameters for IRDA transfer control parameters
     */
    CSL_IrdaTransferControl* hwTransferSetup;
    /** Configuration parameters for IRDA Addressing Information
     */
    CSL_IrdaAddressControl* hwAddressSetup;
}CSL_IrdaHwSetup;
/**
@}*/

#define CSL_IRDA_TRIG_RX_DMA_TRIG  32
#define CSL_IRDA_TRIG_TX_DMA_TRIG  32

/**
 * @defgroup CSL_IRDA_DEFAULT_SYMBOL IRDA CSL Defaults
 * @ingroup CSL_IRDA_SYMBOL
 *
 * @{ */

/** Default IRDA module clock */
#define CSL_IRDA_MODULE_CLOCK_DEFAULT 48000000

/** Default max frame length for RCV */
#define CSL_IRDA_RXFRMLENGTH_DEFAULT  32

/** Defaults for 'hwCtrlSetup' in HwSetup */
#define CSL_IRDA_HWCTRL_SETUP_DEFAULTS {  \
       (Uint32)CSL_IRDA_MODULE_CLOCK_DEFAULT, \
       (Uint32)CSL_IRDA_BAUD_9_6K, \
       (CSL_IrdaMode)CSL_IRDA_MODE_NONE,\
       (CSL_IrdaDmaMode)CSL_IRDA_DMAMODE_NONE,\
       (CSL_IrdaStatusFifoTrigger)CSL_IRDA_STATUS_TRIGGER_4, \
       (Uint16)CSL_IRDA_TRIG_RX_DMA_TRIG,\
           (Uint16)CSL_IRDA_TRIG_TX_DMA_TRIG,\
           (CSL_BitMask16)(CSL_IRDA_EVENT_RHR|CSL_IRDA_EVENT_THR),\
           (Bool)TRUE \
        }

/** Defaults for 'hwTransferSetup' in HwSetup */
#define CSL_IRDA_TRANSFERCTRL_SETUP_DEFAULTS {  \
           (CSL_IrdaSipMode)CSL_IRDA_SIPMODE_AUTOMATIC, \
           (CSL_IrdaFrameEndMode)CSL_IRDA_FRAME_ENDMODE_LENGTH,  \
           (Uint16)CSL_IRDA_RXFRMLENGTH_DEFAULT, \
           (Bool)TRUE \
           }

/** Defaults for 'hwAddressSetup' in HwSetup */
#define  CSL_IRDA_ADDRESS_SETUP_DEFAULTS{  \
           (Uint16)0x0, \
           (Uint16)0x0, \
           (Bool)FALSE, \
           (Bool)FALSE  \
           }

/** Defaults CSL_IrdaHwSetup */
#define CSL_IRDA_HWSETUP_DEFAULTS { \
            CSL_IRDA_HWCTRL_SETUP_DEFAULTS, \
            CSL_IRDA_TRANSFERCTRL_SETUP_DEFAULTS, \
            CSL_IRDA_ADDRESS_SETUP_DEFAULTS \
        }
/**
@} */

/**
@addtogroup CSL_IRDA_OPEN_API
@{
*/
/** @brief This object contains the reference to the instance of IRDA
* opened using the @a CSL_irdaOpen()
*
* The pointer to this is passed to all IRDA CSL APIs.
* This structure has the fields required to configure IRDA for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function.
*/
typedef struct CSL_IrdaObj {
    /** This is the mode in which the CSL instance is opened */
    CSL_OpenMode            openMode;
    /** This is a unique identifier to the instance of IRDA being
     * referred to by this object */
    CSL_Uid                 uid;
    /** This is the variable that contains the current state of a
     * resource being shared by current instance of IRDA with
     * other peripherals */
    CSL_Xio                 xio;
    /** This is a pointer to the registers of the instance of IRDA
     * referred to by this object */
    CSL_IrdaRegsOvly       regs;
    /** This is the instance of IRDA being referred to by this object */
    CSL_IrdaNum            irdaNum;
}CSL_IrdaObj;
/**
@}
*/

/** @brief this is a pointer to @a CSL_IrdaObj & is passed as the first
* parameter to all IRDA CSL APIs */
typedef struct CSL_IrdaObj *CSL_IrdaHandle;

/**
@defgroup CSL_IRDA_OPEN_API IRDA Open API
@ingroup CSL_IRDA_FUNCTION
@brief Opens if possible the instance of IRDA requested
@{*/
CSL_IrdaHandle  CSL_irdaOpen
(
        /** Pointer to the handle object - to be allocated by the user
     */
        CSL_IrdaObj              *hIrdaObj,
        /** Specifies the instance of IRDA to be opened */
        CSL_IrdaNum              IrdaNum,
        /** Specifies if IRDA should be opened with exclusive OR
         * share access to the associate pins */
        CSL_OpenMode              openMode,
        /** If a valid structure is passed (not @a NULL), then
     * @a CSL_irdaHwSetup() function is called with this parameter */
        CSL_IrdaHwSetup          *hwSetup,
        /** Pointer to the variable that holds the holds the status of the open
         call */
        CSL_Status                *status
);
/**
@}
*/
/**
@defgroup CSL_IRDA_CLOSE_API IRDA Close API
@ingroup CSL_IRDA_FUNCTION
@brief Closes the instance of IRDA requested
@{*/
CSL_Status  CSL_irdaClose
(
    /** Pointer to the object that holds reference to the
     * instance of IRDA requested after the call */
        CSL_IrdaHandle           hIrda
);
/**
@}*/
/**
@defgroup CSL_IRDA_SETUP_API IRDA Setup API
@ingroup CSL_IRDA_FUNCTION
@brief Programs the IRDA with the setup configuration as specified in the arguments
@{*/
CSL_Status  CSL_irdaHwSetup
(
        /** Pointer to the object that holds reference to the
     * instance of IRDA requested after the call */
        CSL_IrdaHandle           hIrda,
        /** Pointer to setup structure which contains the
     * information to program IRDA to a useful state */
        CSL_IrdaHwSetup*         setup
);
/**
@}*/
/**
@defgroup CSL_IRDA_CONTROL_API IRDA Control API
@ingroup CSL_IRDA_FUNCTION
@brief Controls the different operations that can be performed by IRDA
@{*/

CSL_Status  CSL_irdaHwControl
(
        /** Pointer to the object that holds reference to the
     * instance of IRDA requested after the call */
        CSL_IrdaHandle           hIrda,
        /** The command to this API which indicates the action to be taken */
        CSL_IrdaHwControlCmd     cmd,
        /** Optional argument @a void* casted */
        void                     *cmdArg
);
/**
@}*/
/**
@defgroup CSL_IRDA_INIT_API IRDA Init API
@ingroup CSL_IRDA_FUNCTION
@brief Initializes the IRDA
@{*/
CSL_Status  CSL_irdaInit
(
void
);
/**
@}*/
/**
@defgroup CSL_IRDA_QUERY_API IRDA Query API
@ingroup CSL_IRDA_FUNCTION
@brief Returns the status of the requested operation/parameter of IRDA
@{*/
CSL_Status  CSL_irdaGetHwStatus
(
        /** Pointer to the object that holds reference to the
     * instance of IRDA requested after the call */
        CSL_IrdaHandle           hIrda,
        /** The query to this API which indicates the status/setup
     * to be returned */
        CSL_IrdaHwStatusQuery    myQuery,
        /** Placeholder to return the status; @a void* casted */
        void                     *response
);
/**
@}*/

/**
@defgroup CSL_IRDA_GETHWSETUP_API IRDA Query HwSetup API
@ingroup CSL_IRDA_FUNCTION
@brief Returns the status of current IRDA HwSetup
@{*/
CSL_Status CSL_irdaGetHwSetup
(
    /** IRDA handle returned by successful 'open' */
    CSL_IrdaHandle hIrda,
    /** Pointer to @a CSL_IrdaHwSetup structure to read the setup parameters*/
    void *setup
);

/**
@}*/

#ifdef __cplusplus
}
#endif


#endif



