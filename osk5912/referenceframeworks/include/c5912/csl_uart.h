/*  =========================================================
*   Copyright (c) Texas Instruments Inc 2002, 2003, 2004
*
*   Use of this software is controlled by the terms and conditions found
*   in the license agreement under which this software has been supplied
*   provided
*   ==========================================================
*/
/** @mainpage UART Module
*
* @section Introduction
*
* @subsection xxx Purpose and Scope
* The purpose of this document is to identify a set of common CSL APIs for
* the UART Module across various devices. The CSL developer
* is expected to refer to this document while implementing APIs for these
* modules . Some of the listed APIs may not be applicable to a given UART
* While in other cases this list of APIs may not be
* sufficient to cover all the features of a particular UART Module.
* The CSL developer should use his/her discretion in
* designing new APIs or extending the existing ones to cover these.
*
*
*
* @subsection aaa Terms and Abbreviations
*   -# CSL  :  Chip Support Library
*   -# API  :  Application Programmer Interface
*   -# UART :  Universal Asynchronous Receiver Transmitter
*
* @subsection References
*    -# CSL 3.x Technical Requirements Specifications Version 0.5, dated
*       May 14th, 2003
*    -# The OMAP 1610 Technical Reference Manual (SWPU062B)
*
* @subsection Assumptions
*     The abbreviations CSL, UART have been used throughout this
*     document to refer to the OMAP 1610 UART Module.
*     'space' Refers to the empty 5/6/7/8 elements.
*     'character' Refers to the 5/6/7/8 bit long transfer elements.
*      This is not what is meant in conventional "C".
*/

/** @file csl_uart.h
*
*  @brief    Header file for functional layer of CSL
*
*  Description
*    - The different enumerations, structure definitions
*      and function declarations
*  @date 27th Jan, 2004
*  @author Ruchika Kharwar
*/

/**
@defgroup CSL_UART_API UART
*/
/**
@defgroup CSL_UART_DATASTRUCT  UART Data Structures
@ingroup CSL_UART_API
*/
/**
@defgroup CSL_UART_SYMBOL  Symbols
@ingroup CSL_UART_API
*/
/**
@defgroup CSL_UART_ENUM  Enumerated Data Types
@ingroup CSL_UART_API
*/

/**
@defgroup CSL_UART_FUNCTION  Functions
@ingroup CSL_UART_API
*/
#ifndef _CSL_UART_H_
#define _CSL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_uart.h>

/**************************************************************************\
* UART global macro declarations
\**************************************************************************/
/* Macros - events, used for
*  Interrupt Enable Symbols
*/
/** @defgroup CSL_UART_EVENT_SYMBOL Events
*   @ingroup CSL_UART_SYMBOL
*
*
* @{ */
/** Enable/Disable Receive Threshold Interrupt       */
#define  CSL_UART_EVENT_RHR           0x01
/** Enable/Disable TransmitThreshold Interrupt       */
#define  CSL_UART_EVENT_THR           0x02
/** Enable/Disable Line Status Interrupt             */
#define  CSL_UART_EVENT_LINESTS       0x04
/** Enable/Disable Modem Status Interrupt            */
#define  CSL_UART_EVENT_MODEMSTS      0x08
/** Enable/Disable Sleep Mode                                */
#define  CSL_UART_EVENT_SLEEP         0x10
/** Enable/Disable XOFF Interrupt                              */
#define  CSL_UART_EVENT_XOFF          0x20
/** Enable/Disable RTS Interrupt                                       */
#define  CSL_UART_EVENT_RTS           0x40
/** Enable/Disable CTS Interrupt                                       */
#define  CSL_UART_EVENT_CTS           0x80
/** Enable/Disable DSR Interrupt                                       */
#define  CSL_UART_EVENT_DSR           0x100
/** Enable/Disable RX/CTS/DSR Interrupt for Wakeup   */
#define  CSL_UART_EVENT_RCDWAKEUP     0x200
/**
@} */

/** @defgroup CSL_UART_TXTRIGGER_SYMBOL Tx Trigger Levels
*   @ingroup CSL_UART_SYMBOL
*
* @{ */
/** 8  spaces                 */
#define CSL_UART_TXTRIGLEVEL_8   8
/** 16 spaces                             */
#define CSL_UART_TXTRIGLEVEL_16  16
/** 32 spaces                             */
#define CSL_UART_TXTRIGLEVEL_32  32
/** 56 spaces                             */
#define CSL_UART_TXTRIGLEVEL_56  56
/**
@}*/

/** @defgroup CSL_UART_RXTRIGGER_SYMBOL Rx Trigger Levels
*   @ingroup CSL_UART_SYMBOL
*
* @{ */
/** 8  characters                      */
#define CSL_UART_RXTRIGLEVEL_8   8
/** 16 characters                      */
#define CSL_UART_RXTRIGLEVEL_16  16
/** 56 characters                      */
#define CSL_UART_RXTRIGLEVEL_56  56
/** 60 characters                      */
#define CSL_UART_RXTRIGLEVEL_60  60
/**
@}*/
/** @defgroup CSL_UART_WAKEUP_SYMBOL WakeUp Events
*   @ingroup CSL_UART_SYMBOL
*
* @{ */
/* Macros - UART WakeUp enable Flags
*  The application may also use these flags to allow specified source to enable wakeup
*/
/** CTS Activity can wake Up the system              */
#define CSL_UART_WAKEUPEVENT_CTS  CSL_FMK(UART_WER_CTS,1)
/** DSR Activity can wake Up the system              */
#define CSL_UART_WAKEUPEVENT_DSR  CSL_FMK(UART_WER_DSR,1)
/** RI  Activity can wake Up the system              */
#define CSL_UART_WAKEUPEVENT_RI   CSL_FMK(UART_WER_RI,1)
/** DCD Activity can wake Up the system              */
#define CSL_UART_WAKEUPEVENT_DCD  CSL_FMK(UART_WER_DCD,1)
/** RX/RXIR Activity can wake Up the system          */
#define CSL_UART_WAKEUPEVENT_RX   CSL_FMK(UART_WER_RXIR,1)
/** RHR Activity can wake Up the system              */
#define CSL_UART_WAKEUPEVENT_RHR  CSL_FMK(UART_WER_RHR,1)
/** Receive Line Status can wake Up the system       */
#define CSL_UART_WAKEUPEVENT_LS   CSL_FMK(UART_WER_RLS,1)

/**
@}*/

/** @defgroup CSL_UART_LINESTS_SYMBOL Line Status
*   @ingroup CSL_UART_SYMBOL
*
* @{ */
/* Macros - UART Line Status/Errors
*/
/** Atleast one Parity/Framing/break error in the  Rx FIFO*/
#define CSL_UART_LINESTS_PFB_ERROR          CSL_FMK(UART_LSR_RX_FIFO_STS,1)
/** Tx Hold and Shift registers Empty */
#define CSL_UART_LINESTS_TXSHIFTEMPTY       CSL_FMK(UART_LSR_TX_SR_E,1)
/** Tx FIFO Empty */
#define CSL_UART_LINESTS_TXFIFOEMPTY        CSL_FMK(UART_LSR_TX_FIFO_E,1)
/** Break condition detected while the data is being read */
#define CSL_UART_LINESTS_BREAK_ERROR        CSL_FMK(UART_LSR_RX_BI,1)
/** Framing error in data being read from the Rx FIFO */
#define CSL_UART_LINESTS_RXFRAME_ERROR      CSL_FMK(UART_LSR_RX_FE,1)
/** Parity error in data being read from the Rx FIFO */
#define CSL_UART_LINESTS_PARITY_ERROR       CSL_FMK(UART_LSR_RX_PE,1)
/** Overrun error in data being read from the Rx FIFO */
#define CSL_UART_LINESTS_OVERRUN_ERROR      CSL_FMK(UART_LSR_RX_OE,1)
/** Framing error in data being read from the Rx FIFO */
#define CSL_UART_LINESTS_RXFIFO_NOTEMPTY    CSL_FMK(UART_LSR_RX_FIFO_E,1)
/**
@}*/
/** @defgroup CSL_UART_MODEMSTS_SYMBOL Modem Status
*   @ingroup CSL_UART_SYMBOL
*
* @{ */
/* Macros - Modem Status
*/
/** CTS input has changed state  */
#define CSL_UART_MODEMSTS_CTS               CSL_FMK(UART_MSR_CTS_STS,1)
/** DSR input has changed state  */
#define CSL_UART_MODEMSTS_DSR               CSL_FMK(UART_MSR_DSR_STS,1)
/** RI input has changed state from low to high  */
#define CSL_UART_MODEMSTS_RI                CSL_FMK(UART_MSR_RI_STS,1)
/** DCD input has changed state  */
#define CSL_UART_MODEMSTS_DCD               CSL_FMK(UART_MSR_DCD_STS,1)
/** CTS complement  */
#define CSL_UART_MODEMSTS_NCTS              CSL_FMK(UART_MSR_NCTS_STS,1)
/** DSR complement  */
#define CSL_UART_MODEMSTS_NDSR              CSL_FMK(UART_MSR_NDSR_STS,1)
/** RI complement  */
#define CSL_UART_MODEMSTS_NRI               CSL_FMK(UART_MSR_NRI_STS,1)
/** CD complement  */
#define CSL_UART_MODEMSTS_NCD               CSL_FMK(UART_MSR_NCD_STS,1)
/**
@}*/
/** @defgroup CSL_UART_SUPPSTS_SYMBOL Supplementary Status
*   @ingroup CSL_UART_SYMBOL
*
* @{ */
/* Macros - Supplementary Status/Errors
*/
/** Tx FIFO full  */
#define CSL_UART_SUPPSTS_TXFIFOFULL           CSL_FMK(UART_SSR_TX_FIFO_FULL,1)
/** Falling edge on Rx/CTS,DSR */
#define CSL_UART_SUPPSTS_RCD_WAKE_UP          CSL_FMK(UART_SSR_RX_CTS_DSR_WAKE_UP_STS,1)

/**
@}*/
/** @defgroup CSL_UART_RXFIFOTRIGGER_SYMBOL Rx FIFO Trigger Levels
*   @ingroup CSL_UART_SYMBOL
*
* @{ */
/* Macros - Rx FIFO Trigger Levels
*/

#define CSL_UART_RX_FIFO_TRIG_HALT_MIN     1
#define CSL_UART_RX_FIFO_TRIG_HALT_MAX     63
#define CSL_UART_RX_FIFO_TRIG_RESUME_MIN   1
#define CSL_UART_RX_FIFO_TRIG_RESUME_MAX   63

/**
@}*/
/**@defgroup CSL_UART_BAUDRATE_SYMBOL Baud Rate
*  @ingroup CSL_UART_SYMBOL
@{*/
/** @brief Baud Rate setting.
*/
#define CSL_UART_BAUD_0_3K      300
/**0.6 Kb/s */
#define CSL_UART_BAUD_0_6K      600
/**1.2Kb/s */
#define CSL_UART_BAUD_1_2K      1200
/** 2.4Kb/s*/
#define CSL_UART_BAUD_2_4K      2400
/** 4.8 Kb/s */
#define CSL_UART_BAUD_4_8K      4800
/** 9.6 Kb/s   */
#define CSL_UART_BAUD_9_6K      9600
/**14.4 Kb/s */
#define CSL_UART_BAUD_14_4K     14400
/**19.2 Kb/s */
#define CSL_UART_BAUD_19_2K     19200
/**28.8 Kb/s */
#define CSL_UART_BAUD_28_8K     28800
/** 38.4 Kb/s*/
#define CSL_UART_BAUD_38_4K     38400
/**57.6 Kb/s */
#define CSL_UART_BAUD_57_6K      57600
/**115.2 Kb/s */
#define CSL_UART_BAUD_115_2K     115200
/** 230.4 Kb/s */
#define CSL_UART_BAUD_230_4K     230400
/** 460.8 Kb/s */
#define CSL_UART_BAUD_460_8K     460800
/** 921.6  Kb/s */
#define CSL_UART_BAUD_921_6K     921600
/** 1.8342 Mb/s */
#define CSL_UART_BAUD_1_834M     1834200
/** 3.6864 Mb/s */
#define CSL_UART_BAUD_3_686M     3686400
/**
@}
*/
/**************************************************************************\
* UART global typedef declarations
\**************************************************************************/


/**@defgroup CSL_UART_AUTOBAUD_ENUM AutoBaud Speeds
*  @ingroup CSL_UART_ENUM
@{*/
/** @brief UART AutoBaud speeds
*
* This is used for query commands to query the  UART AutoBaud Mode
*/
typedef enum {
    /** 0Kb/s */
    CSL_UART_AUTOBAUDSPEED_NONE= 0  ,
    /** 115200 baud */
    CSL_UART_AUTOBAUDSPEED_1152= 1  ,
        /** 57600 baud */
        CSL_UART_AUTOBAUDSPEED_576= 2   ,
        /** 38400 baud */
        CSL_UART_AUTOBAUDSPEED_384= 3   ,
        /** 28800 baud */
        CSL_UART_AUTOBAUDSPEED_288= 4   ,
        /** 19200 baud */
        CSL_UART_AUTOBAUDSPEED_192= 5   ,
        /** 14400 baud */
        CSL_UART_AUTOBAUDSPEED_144= 6   ,
        /** 09600 baud */
        CSL_UART_AUTOBAUDSPEED_096= 7   ,
        /** 04800 baud */
        CSL_UART_AUTOBAUDSPEED_048= 8   ,
        /** 02400 baud */
        CSL_UART_AUTOBAUDSPEED_024= 9   ,
        /** 1200 baud */
        CSL_UART_AUTOBAUDSPEED_012= 10
}CSL_UartAutoBaudSpeed;
/**
@}
*/
/**@defgroup CSL_UART_AUTOBAUDCHARLEN_ENUM AutoBaud Character Length
*  @ingroup CSL_UART_ENUM
@{*/
/** @brief Enumeration for UART AutoBaud Character Length
*
* This is used for query commands to query the  UART AutoBaud Character Length
*/
typedef enum {
    /** Autobaud character length is 7 */
    CSL_UART_AUTOBITPERCHAR_7 = CSL_UART_UASR_BIT_BY_CHAR_7,
    /** Autobaud character length is 8 */
    CSL_UART_AUTOBITPERCHAR_8 = CSL_UART_UASR_BIT_BY_CHAR_8
}CSL_UartAutoBitperChar;
/**
@}*/
/**@defgroup CSL_UART_AUTOBAUDPARITY_ENUM AutoBaud parity
*  @ingroup CSL_UART_ENUM
@{*/
/** @brief Enumeration for UART AutoBaud parity
*
* This is used for query commands to query the UART AutoBaud parity
*/
typedef enum {
    /** No Autobaud parity detected */
    CSL_UART_AUTOPARITY_NONE = CSL_UART_UASR_PARITY_TYPE_NONE,
    /** Space detected as Autobaud parity */
    CSL_UART_AUTOPARITY_SPACE= CSL_UART_UASR_PARITY_TYPE_SPACE,
    /** Even Autobaud parity detected */
    CSL_UART_AUTOPARITY_EVEN = CSL_UART_UASR_PARITY_TYPE_EVEN,
    /** Odd Autobaud parity detected */
    CSL_UART_AUTOPARITY_ODD  = CSL_UART_UASR_PARITY_TYPE_ODD
}CSL_UartAutoParity;
/**
@}
*/
/**@defgroup CSL_UART_MODE_ENUM  Mode
*  @ingroup CSL_UART_ENUM
@{*/
/** @brief  Enums for the Mode of the UART module
*
* This is used for control/query commands to set/query the mode of the UART module.
*/
typedef enum {
    /** UART 16x mode */
    CSL_UART_MODE_16X         =   CSL_UART_MDR1_MODE_SELECT_16X,
    /** UART 16x Autobaud mode */
    CSL_UART_MODE_16XAUTOBAUD =   CSL_UART_MDR1_MODE_SELECT_16XAUTOBAUD,
    /** UART 13x mode */
    CSL_UART_MODE_13X         =   CSL_UART_MDR1_MODE_SELECT_13X,
    /** Default State */
    CSL_UART_MODE_DISABLE     =   CSL_UART_MDR1_MODE_SELECT_DISABLE
}CSL_UartMode;
/**
@}*/

/**@defgroup CSL_UART_DMAMODE_ENUM  DMA Mode of the UART module
*  @ingroup CSL_UART_ENUM
@{*/
/** @brief  Enums for the DMA Mode of the UART module
*
* This is used for control/query commands to set/query the DMA mode of the UART module.
*/
typedef enum {
    /** No DMA mode */
    CSL_UART_DMAMODE_NONE =  CSL_UART_SCR_DMA_MODE_2_00,
    /** DMA mode 1 */
    CSL_UART_DMAMODE_RXTX =  CSL_UART_SCR_DMA_MODE_2_01,
    /** DMA mode 2 */
    CSL_UART_DMAMODE_RX   =  CSL_UART_SCR_DMA_MODE_2_02,
    /** DMA mode 3 */
    CSL_UART_DMAMODE_TX   =  CSL_UART_SCR_DMA_MODE_2_03
}CSL_UartDmaMode;
/**
@}*/
/**@defgroup CSL_UART_CHARLEN_ENUM  UART character length
@ingroup CSL_UART_ENUM
@{*/
/** @brief  Enums for the UART character length
*
* This is used for control/query commands to set/query the UART character length.
*/
typedef enum {
    /** Character length of 5 */
    CSL_UART_CHARLEN_5    =  CSL_UART_LCR_CHAR_LENGTH_05,
    /** Character length of 6 */
    CSL_UART_CHARLEN_6    =  CSL_UART_LCR_CHAR_LENGTH_06,
    /** Character length of 7 */
    CSL_UART_CHARLEN_7    =  CSL_UART_LCR_CHAR_LENGTH_07,
    /** Character length of 8 */
    CSL_UART_CHARLEN_8    =  CSL_UART_LCR_CHAR_LENGTH_08
}CSL_UartCharLen;
/**
@}*/
/**@defgroup CSL_UART_FLOWCTRL_ENUM  Flow Control Type
@ingroup CSL_UART_ENUM

 @{*/
/** @brief  Enums for the Flow Control
*
* This is used for control/query commands to set/query the UART Flow control parameters.
*/
typedef enum {
        /** Neither HW,nor S/W flow Control */
    CSL_UART_FLOWCONTROL_NONE = 0,
    /** Software Flow Control */
    CSL_UART_FLOWCONTROL_SW   = 1,
    /** Hardware Flow Control */
    CSL_UART_FLOWCONTROL_HW   = 2
}CSL_UartFlowControl;
/**
@}
*/
/**@defgroup CSL_UART_SWFLOWCTRL_ENUM  Software Flow Control
@ingroup CSL_UART_ENUM
@{*/
/** @brief  Enums for the Software Flow Control
*
* This is used for control/query commands to set/query the UART SW Flow control parameters.
*/
typedef enum {
    /** No Flow Control                           */
    CSL_UART_SWFLOW_NONE = 0,
    /** Receiver/Transmitter compares XON1,XOFF1                      */
    CSL_UART_SWFLOW_XONXOFF_1 = 2,
    /** Receiver/Transmitter compares XON2,XOFF2                      */
    CSL_UART_SWFLOW_XONXOFF_2 = 1,
    /** Receiver/Transmitter compares XON1,XON2,XOFF1,XOFF2           */
    CSL_UART_SWFLOW_XONXOFF_12 = 3
}CSL_UartSwFlow;
/**
@}*/
/**@defgroup CSL_UART_NUMSTOP_ENUM  Number of Stop bits
@ingroup CSL_UART_ENUM
@{*/
/** @brief  Enums for the Number of Stop bits
*
* This is used for control/query commands to set/query the Number of Stop bits.
*/
typedef enum {
    /** Number of Stop bits = 1                   */
    CSL_UART_NUMSTOP_1   ,
    /** Number of Stop bits = 1.5                 */
    CSL_UART_NUMSTOP_1_5 ,
    /** Number of Stop bits = 2                   */
    CSL_UART_NUMSTOP_2
}CSL_UartNumStop;
/**
@}*/
/**@defgroup CSL_UART_PARITY_ENUM  Parity Setting
@ingroup CSL_UART_ENUM
@{*/
/** @brief  Enums for the Parity Setting
*
* This is used for control/query commands to set/query the Parity Setting.
*/
typedef enum {
    /** No Parity                          */
    CSL_UART_PARITY_NONE    = 0,
    /** Odd Parity                         */
    CSL_UART_PARITY_ODD     = 1,
    /** Even Parity                        */
    CSL_UART_PARITY_EVEN    = 3,
    /** Forced 1 Parity                    */
    CSL_UART_PARITY_FORCED1 = 5,
    /** Forced 0 Parity                    */
    CSL_UART_PARITY_FORCED0 = 7
}CSL_UartParity;
/**
@}*/

/**@defgroup CSL_UART_INTRSOURCE_ENUM  Interrupt Source
@ingroup CSL_UART_ENUM
@{*/
/** @brief  Enums for the Interrupt Source
*
* This is used for query Interrupt Source.
*/
typedef enum {
    /** No Interrupt Occured             */
    CSL_UART_INTRSOURCE_NONE       = CSL_UART_IIR_IT_PENDING_NONE,
    /** Receiver Line Status Interrupt   */
    CSL_UART_INTRSOURCE_LINESTS    = CSL_UART_IIR_IT_TYPE_RX_LINE_STATUS << 1,
    /** Rx Timeout Interrupt             */
    CSL_UART_INTRSOURCE_RXTIMEOUT  = CSL_UART_IIR_IT_TYPE_RX_TIMEOUT << 1,
    /** Rx Interrupt                     */
    CSL_UART_INTRSOURCE_RHR        = CSL_UART_IIR_IT_TYPE_RHR << 1,
    /** Tx Interrupt                         */
    CSL_UART_INTRSOURCE_THR        = CSL_UART_IIR_IT_TYPE_THR << 1 ,
    /** Modem Status Interrupt           */
    CSL_UART_INTRSOURCE_MODEMSTS   = CSL_UART_IIR_IT_TYPE_MSR << 1,
    /** XOFF/SPecial Character Interrupt */
    CSL_UART_INTRSOURCE_XOFFSPECIAL = CSL_UART_IIR_IT_TYPE_XOFF_SPCHAR << 1,
    /** RTS/CTS/DSR pin change state(active low to high)  */
    CSL_UART_INTRSOURCE_RCD              = CSL_UART_IIR_IT_TYPE_RCD << 1
}CSL_UartIntrSource;
/**
@}*/
/**@defgroup CSL_UART_CONTROLCMD_ENUM  Control Commands
*  @ingroup CSL_UART_CONTROL_API
@{*/
/** @brief Enumeration for control commands passed to @a
CSL_UartHwControl()
*
* This is the set of commands that are passed to the @a CSL_UartHwControl()
* with an optional argument type-casted to @a void* .
* The arguments to be passed with each enumeration (if any) are specified
* next to the enumeration.
*/

typedef enum {
/**@brief  Soft Resets the module
1.      Programmation of SOFTRESET field in SYSC
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_RESET = 0              ,
/**@brief Sets events qualified to do wake up.
1.      Programmation of WER Register
@param (CSL_BitMask16*)
@return CSL_SOK
*/
    CSL_UART_CMD_SETWAKEUPEVENTS = 1,
/**@brief Read specified characters from Rx FIFO.
The "actualnumbytes" act as output parameters reporting the Actual number of bytes read and error.
1.      Access of RHR Register \n
2.      Access of LSR Register
@param (CSL_UartXfer*)
@return CSL_SOK
*/
    CSL_UART_CMD_READ = 2          ,
/**@brief  Write specified num of character into TxFIFO.
1.      Access of THR Register \n
The "actualnumbytes" field acts as output parameters reporting the Actual number of bytes written.
@param (CSL_UartXfer*)
@return CSL_SOK
*/
    CSL_UART_CMD_WRITE  = 3       ,
/**@brief Set the XOFF2 character
1.      Programmation of XOFF2
@param (Uint8*)
@return CSL_SOK
*/
    CSL_UART_CMD_SET_SPECIALCHAR = 4  ,
/**@brief When a falling edge event occured on RX,CTS,DSR then SSR[1] is set
The only way to clear this is by issuing this command. This command does :- \n
1. Praogrammation of the RX_CTS_DSR_WAKE_UP_ENABLE field of SCR (Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_RCDEVENT_CLEAR = 5,

/**@brief  Enables the Corresponding Interrupts

This command does the following :- \n
1.      Programmation of IER Register \n
2.      Programmation of DSR_IT field in SCR \n
3.      Programmation of RX_CTS_DSR_WAKE_UP in SCR \n

@param (CSL_BitMask16*)
@return CSL_SOK
*/
    CSL_UART_CMD_EVENT_DISABLE = 0x6|0  ,
/**@brief  Disables the Corresponding Interrupts
This command does the following :- \n
1.      Programmation of IER Register \n
2.      Programmation of DSR_IT field in SCR \n
3.      Programmation of RX_CTS_DSR_WAKE_UP in SCR \n

@param (CSL_BitMask16*)
@return CSL_SOK
*/
    CSL_UART_CMD_EVENT_ENABLE  = 0x6|1  ,
/**@brief Disable "XON ANY" Function
1.  Programmation of XON_EN in MCR(Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_XONANY_DISABLE = 0x8 | CSL_UART_MCR_XON_EN_DISABLE,

/**@brief Enable "XON ANY" Function
1.  Programmation of XON_EN in MCR(Set to 1)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_XONANY_ENABLE = 0x8 | CSL_UART_MCR_XON_EN_ENABLE,
/**@brief Disables the Tx Empty Mode of txinterrupt generation
1.  Programmation of TX_EMPTY_CTL_IT field in the SCR(Set to 0).
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_TXEMPTYIT_DISABLE = 0xA | CSL_UART_SCR_TX_EMPTY_CTL_IT_DISABLE,
/**@brief Enables the Tx Empty Mode of Tx interrupt generation
1.  Programmation of TX_EMPTY_CTL_IT field in the SCR(Set to 1).
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_TXEMPTYIT_ENABLE = 0xA | CSL_UART_SCR_TX_EMPTY_CTL_IT_ENABLE,
 /**@brief Disables the RTS line i.e pulls it high
1.      Programmation of RTS field in MCR(Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_RTS_DISABLE = 0xC | CSL_UART_MCR_RTS_DISABLE,

/**@brief Enables the RTS line i.e pulls it low
1.  Programmation of RTS field in MCR(Set to 1)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_RTS_ENABLE = 0xC | CSL_UART_MCR_RTS_ENABLE,
/**@brief  Disables the DTR line i.e pulls it high
1.  Programmation of DTR field in the MCR(Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_DTR_DISABLE = 0xE | CSL_UART_MCR_DTR_DISABLE  ,
/**@brief Enables the DTR line i.e pulls it low
1.  Programmation of RTS field in MCR(Set to 1)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_DTR_ENABLE = 0xE | CSL_UART_MCR_DTR_ENABLE  ,

/**@brief  Pull the RI line high
1.      Programmation of RI_STS_CH field in the MCR(Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_RI_HIGH = 0x10 | CSL_UART_MCR_RI_STS_CH_HIGH,
/**@brief Pull the RI line low
1.  Programmation of RI_STS_CH field in the MCR(Set to 1)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_RI_LOW = 0x10 | CSL_UART_MCR_RI_STS_CH_LOW,
/**@brief Pull the DCD line high
1.  Programmation of  CD_STS_CH field in MCR(Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_DCD_HIGH = 0x12 | CSL_UART_MCR_CD_STS_CH_HIGH  ,
/**@brief Pull the DCD line low
1.  Programmation of  CD_STS_CH field in MCR(Set to 1)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_DCD_LOW = 0x12 | CSL_UART_MCR_CD_STS_CH_LOW  ,
/**@brief Internal OCP clock gating strategy is set to "Clock is running"
1.      Programmation of AUTOIDLE  in SYSC(Set to CLKRUN mode).
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_SYSAUTOIDLE_CLKRUN = 0x14 | CSL_UART_SYSC_AUTOIDLE_CLKRUN  ,
/**@brief Apply Auto OCP clock gating strategy.
1.      Programmation of AUTOIDLE  in SYSC(Set to AUTO_OCP mode).
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_SYSAUTOIDLE_AUTO_OCP = 0x14 | CSL_UART_SYSC_AUTOIDLE_AUTO_OCP ,
/**@brief System Wake Up Feature control disabled
1.      Programmation of  ENAWAKEUP in SYSC(Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_SYSWAKE_DISABLE = 0x16 | CSL_UART_SYSC_ENAWAKEUP_DISABLE  ,
/**@brief System Wake Up Feature control enabled
1.      Programmation of  ENAWAKEUP in SYSC(Set to 1)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_SYSWAKE_ENABLE = 0x16 | CSL_UART_SYSC_ENAWAKEUP_ENABLE  ,
/**@brief Disable special character detect
1.  Programmation of  SPECIAL_CHAR_DETECT in EFR(Set to 0)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_SPECIALCHARDETECT_DISABLE = 0x18 | CSL_UART_EFR_SPECIAL_CHAR_DETECT_DISABLE   ,

/**@brief Enable special character detect
1.  Programmation of  SPECIAL_CHAR_DETECT in EFR(Set to 1)
@param None
@return CSL_SOK
*/
      CSL_UART_CMD_SPECIALCHARDETECT_ENABLE = 0x18 | CSL_UART_EFR_SPECIAL_CHAR_DETECT_ENABLE ,
/**@brief Disables the Break Condition
1.  Programmation of BREAK_EN field in  LCR register(Set to 0).
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_BREAK_DISABLE = 0x1A | CSL_UART_LCR_BREAK_EN_DISABLE,

/**@brief Enable the break Condition
1.  Programmation of BREAK_EN field in the LCR register(Set to 1).
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_BREAK_ENABLE = 0x1A | CSL_UART_LCR_BREAK_EN_ENABLE ,
/**@brief  Set power control management to "Force Idle"
1.      Programmation of  IDLEMODE in SYSC(Set to FORCE)
@param None
@return CSL_SOK
*/
      CSL_UART_CMD_SYSIDLEMODE_FORCE = 0x1C | CSL_UART_SYSC_IDLEMODE_FORCE  ,
/**@brief  Set power control management to "None"
1.      Programmation of  IDLEMODE in SYSC(Set to NONE)
@param None
@return CSL_SOK
*/
      CSL_UART_CMD_SYSIDLEMODE_NONE = 0x1C | CSL_UART_SYSC_IDLEMODE_NONE  ,

/**@brief  Set power control management to "Smart Idle"
1.      Programmation of  IDLEMODE in SYSC(Set to SMART)
@param None
@return CSL_SOK
*/
    CSL_UART_CMD_SYSIDLEMODE_SMART = 0x1C | CSL_UART_SYSC_IDLEMODE_SMART
}CSL_UartHwControlCmd;
/** @}*/

/**@defgroup CSL_UART_QUERYCMD_ENUM Query Commands
*  @ingroup CSL_UART_QUERY_API
* @{ */

/** @brief Enumeration for queries passed to @a CSL_UartGetHwStatus()
*
* This is used to get the status of different operations or to get the
* existing setup of UART. The arguments to be passed with each
* enumeration if any are specified next to the enumeration */

typedef enum {
/**@brief  Queries the events enabled in the UART Module
           OR'd events activated in the UART module
IER Accessed
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_UART_QUERY_EVENTS,
/**@brief  Queries the wakeup events enabled in the UART Module
OR'd events activated in the UART module \n
WER Accessed
@param (CSL_BitMask16*)
@return CSL_SOK
*/
    CSL_UART_QUERY_WAKEUP_EVENTS,
/**@brief Queries Receiver Line Status
LSR Accessed
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_UART_QUERY_LINESTATUS,
/**@brief Queries Modem Status
MSR Accessed
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_UART_QUERY_MODEMSTATUS,
/**@brief Queries Supplementary Status
SSR Accessed
@param (CSL_BitMask16*)
@return CSL_SOK
*/
        CSL_UART_QUERY_SUPPSTATUS,
/**@brief Queries the Autobaud parameters in the AUTOBAUD mode
UASR Accessed
@param (CSL_UartAutoBaudSpeed*)
@return CSL_SOK
*/
        CSL_UART_QUERY_AUTOBAUD,
/**@brief Version number of the module
MVR Accessed
@param (Uint16*)
@return CSL_SOK
*/
        CSL_UART_QUERY_VERSION,
/**@brief Interrupt Source read from the Interrupt Identification register
IIR Accessed
@param (CSL_UartIntrSource*)
@return CSL_SOK
*/
        CSL_UART_QUERY_INTRSOURCE,
/**@brief Query to find out if the  System Reset is complete or ongoing
SYSS Accessed
@param (Bool*) \n
      TRUE Reset is done \n
      FALSE Reset is ongoing \n
@return CSL_SOK
*/
        CSL_UART_QUERY_RESET
}CSL_UartHwStatusQuery;
/**
@}*/

/**
*   @addtogroup CSL_UART_DATASTRUCT
* @{ */
/** @brief CSL_UartXfer is used when sending/receiving data.
*/

typedef struct CSL_UartXfer{
    /** Ptr to data transfer/receive buffer */
    Uint8* buff;
    /** Number of bytes(5/6/7/8 chars) to be transferred*/
    Uint16  num ;
    /** Number of bytes(5/6/7/8 chars) actually  transferred*/
    Uint16  xfernum ;
    /** OR'd combination of errors      i.e Parity and/or Break and/or Frame Error   */
    CSL_BitMask16    error  ;
}CSL_UartXfer;
/**
@} */
/**
*   @addtogroup CSL_UART_DATASTRUCT
* @{ */
/** @brief CSL_UartAutoBaud is used when querying the Autobaud parameters .
*/
typedef struct CSL_UartAutoBaud{
        /** Uart Autobaud speed */
    CSL_UartAutoBaudSpeed    speed;
    /** Uart Autobaud character length */
    CSL_UartAutoBitperChar   bits;
    /** Uart Autobaud parity */
    CSL_UartAutoParity       parity;
}CSL_UartAutoBaud;


/**
@} */
/** @addtogroup CSL_UART_DATASTRUCT
*
* @{ */
/** @brief CSL_UartHwCtrlSetup is a substructure of the CSL_UartHwSetup.
*
* that has the fields required for configuring for the UART Module.
*/
typedef struct CSL_UartHwCtrlSetup{
        /** Module Clock Frequency(Hz)                                     */
        Uint32          moduleClock;
        /** Standard baudrate setting                              */
        Uint32    baudrate;
        /** Either 13x/16x/Autobaud mode                           */
    CSL_UartMode    mode;
    /* One of 4 possible DMA modes                                 */
        CSL_UartDmaMode dmaMode;
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
        /** Loop Back Enable/Disbable   \n
                TRUE  - Enable Loopback  \n
            FALSE - Disable Loopback  \n                                  */
    Bool            loopenable;
}CSL_UartHwCtrlSetup;

/** @brief CSL_UartHwSetupModem is a substructure of the CSL_UartHwSetup.
*
* It has the fields required for the Modem Setup
*/
typedef struct CSL_UartHwSetupModem{
        /** Length of each transmitted character 5/6/7/8 bits*/
        CSL_UartCharLen  length;
        /** Number of Stop bits                               */
        CSL_UartNumStop  stopBits;
        /** Type of Parity                                    */
        CSL_UartParity   parity;
}CSL_UartHwSetupModem;

/** @brief CSL_UartHwSetupFlowCtrl is a substructure of the CSL_UartHwSetup.
*
* It has the fields required for the configuration of Modem Flow Control
*/
typedef struct CSL_UartHwSetupFlowCtrl{
        /** Software Rx Control Parameter   */
        CSL_UartSwFlow     swRxctrl;
        /** Software Tx Control Parameter   */
    CSL_UartSwFlow     swTxctrl;
    /** Standard Flow Control Types                                        */
        CSL_UartFlowControl flowType;
        /** Rx Threshold to stop reception CSL_UartReg */
        Uint16       rxFifoTriggerHalt;
        /** Rx Threshold to  resume reception  */
    Uint16       rxFifoTriggerResume;
    /** AUTO RTS Enable            \n
            TRUE - Enable Auto RTS \n
            FALSE - Disable Auto RTS */
        Bool  autorts;
        /** AUTO CTS Enable            \n
                TRUE - Enable Auto CTS \n
            FALSE - Disable Auto CTS */
    Bool  autocts;
    /** xon1 character (relevant if flowType is SW)      */
    Uint8       xon1;
    /** xon2 character (relevant if flowType is SW)      */
        Uint8       xon2;
        /** xoff1 character (relevant if flowType is SW)     */
        Uint8       xoff1;
        /** xoff2 character (relevant if flowType is SW)     */
        Uint8       xoff2;
}CSL_UartHwSetupFlowCtrl;
/**
@}
*/
/* Default Setup Structure */
#define CSL_UART_HWMODEM_SETUP_DEFAULTS {  \
           (CSL_UartCharLen)CSL_UART_CHARLEN_8, \
           (CSL_UartNumStop)CSL_UART_NUMSTOP_2,  \
           (CSL_UartParity)CSL_UART_PARITY_EVEN  \
           }

#define CSL_UART_TRIG_RX_DMA_TRIG  32
#define CSL_UART_TRIG_TX_DMA_TRIG  32
#define CSL_UART_HWCTRL_SETUP_DEFAULTS {  \
       (Uint32)48000000,\
       (Uint32)CSL_UART_BAUD_9_6K, \
           (CSL_UartMode)CSL_UART_MODE_16X,\
           (CSL_UartDmaMode)CSL_UART_DMAMODE_NONE,\
           (Uint16)CSL_UART_TRIG_RX_DMA_TRIG,\
           (Uint16)CSL_UART_TRIG_TX_DMA_TRIG,\
           (CSL_BitMask16)(CSL_UART_EVENT_RHR|CSL_UART_EVENT_THR),\
           (Bool)TRUE, \
           (Bool)FALSE \
        }


#define  CSL_UART_HWFLOWCTRL_SETUP_DEFAULTS{  \
           /* Rx SW Flow Control */(CSL_UartSwFlow)CSL_UART_SWFLOW_NONE, \
           /* Tx SW Flow Control */(CSL_UartSwFlow)CSL_UART_SWFLOW_NONE, \
           /* Flow Control Type */(CSL_UartFlowControl)CSL_UART_FLOWCONTROL_HW, \
           /* Rx Fifo Trigger Halt */(Uint16)56, \
           /* Rx Fifo Trigger Resume */(Uint16)16, \
           /* Auto RTS Enable  */(Bool)TRUE, \
           /* Auto CTS Enable */(Bool)TRUE,  \
           /* XON1 Character  */(Uint8)0x0, \
           /* XON2 Character  */(Uint8)0x0, \
           /* XOFF1 Character  */(Uint8)0x0, \
           /* XOFF2 Character  */(Uint8)0x0 \
           }

/** @addtogroup CSL_UART_DATASTRUCT
*
* @{ */
/** @brief CSL_UartHwSetup has all the fields required to configure UART
*
* This structure has the substructures required to configure UART at
* Power-Up/Reset.
*
*/

typedef struct CSL_UartHwSetup {
        /** Configuration parameters at power Up/Reset
     */
    CSL_UartHwCtrlSetup* hwCtrlSetup;
    /** Configuration parameters for Modem Setup
     */
    CSL_UartHwSetupModem* modemSetup;
    /** Configuration parameters for Flow Control
     */
    CSL_UartHwSetupFlowCtrl* flowCtrlSetup;
}CSL_UartHwSetup;
/**
@}*/


/**
@addtogroup CSL_UART_DATASTRUCT
@{
*/
/** @brief This object contains the reference to the instance of UART
* opened using the @a CSL_UartOpen()
*
* The pointer to this is passed to all UART CSL APIs.
* This structure has the fields required to configure UART for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function.
*/
typedef struct CSL_UartObj {
    /** This is the mode in which the CSL instance is opened */
    CSL_OpenMode            openMode;
    /** This is a unique identifier to the instance of UART being
     * referred to by this object */
    CSL_Uid                 uid;
    /** This is the variable that contains the current state of a
     * resource being shared by current instance of UART with
     * other peripherals */
    CSL_Xio                 xio;
    /** This is a pointer to the registers of the instance of UART
     * referred to by this object */
    CSL_UartRegsOvly       regs;
    /** This is the instance of UART being referred to by this object */
    CSL_UartNum            uartNum;
}CSL_UartObj;
/**
@}
*/

/** @brief this is a pointer to @a CSL_UartObj & is passed as the first
* parameter to all UART CSL APIs */
typedef struct CSL_UartObj *CSL_UartHandle;


/**
@defgroup CSL_UART_OPEN_API UART Open API
@ingroup CSL_UART_FUNCTION
@brief Opens if possible the instance of UART requested
@{*/
CSL_UartHandle  CSL_uartOpen
(
        /** Pointer to the handle object - to be allocated by the user
     */
        CSL_UartObj              *hUartObj,
        /** Specifies the instance of UART to be opened */
        CSL_UartNum              UartNum,
        /** Specifies if UART should be opened with exclusive OR
         * share access to the associate pins */
        CSL_OpenMode              openMode,
        /** If a valid structure is passed (not @a NULL), then
     * the @a CSL_UartHwSetup() is called with this parameter */
        CSL_UartHwSetup          *hwSetup,
        /** Pointer to the variable that holds the holds the status of the open
         call */
        CSL_Status                *status
);
/**
@}
*/
/**
@defgroup CSL_UART_CLOSE_API UART Close API
@ingroup CSL_UART_FUNCTION
@brief Closes the instance of UART requested
@{*/
CSL_Status  CSL_uartClose
(
    /** Pointer to the object that holds reference to the
     * instance of UART requested after the call */
        CSL_UartHandle           hUart
);
/**
@}*/
/**
@defgroup CSL_UART_SETUP_API UART Setup API
@ingroup CSL_UART_FUNCTION
@brief Programs the UART with the setup configuration as specified in the arguments
@{*/
CSL_Status  CSL_uartHwSetup
(
        /** Pointer to the object that holds reference to the
     * instance of UART requested after the call */
        CSL_UartHandle           hUart,
        /** Pointer to setup structure which contains the
     * information to program UART to a useful state */
        CSL_UartHwSetup*         setup
);
/**
@}*/
/**
@defgroup CSL_UART_GETSETUP_API UART Get Setup API
@ingroup CSL_UART_FUNCTION
@brief Reads the setup from the peripheral registers.
@{*/
CSL_Status  CSL_uartGetHwSetup
(
        /** Pointer to the object that holds reference to the
     * instance of UART requested after the call */
        CSL_UartHandle           hUart,

        /** Placeholder to return the status; @a CSL_UartHwSetup* */
        CSL_UartHwSetup*         mySetup
);
/**
@}
*/
/**
@defgroup CSL_UART_CONTROL_API UART Control API
@ingroup CSL_UART_FUNCTION
@brief Controls the different operations that can be performed by UART
@{*/

CSL_Status  CSL_uartHwControl
(
        /** Pointer to the object that holds reference to the
     * instance of UART requested after the call */
        CSL_UartHandle           hUart,
        /** The command to this API which indicates the action to be taken */
        CSL_UartHwControlCmd     cmd,
        /** Optional argument @a void* casted */
        void                     *cmdArg
);
/**
@}*/
/**
@defgroup CSL_UART_INIT_API UART Init API
@ingroup CSL_UART_FUNCTION
@brief Initializes the UART
@{*/
CSL_Status  CSL_uartInit
(
void
);
/**
@}*/
/**
@defgroup CSL_UART_QUERY_API UART Query API
@ingroup CSL_UART_FUNCTION
@brief Returns the status of the requested operation/parameter of UART
@{*/
CSL_Status  CSL_uartGetHwStatus
(
        /** Pointer to the object that holds reference to the
     * instance of UART requested after the call */
        CSL_UartHandle           hUart,
        /** The query to this API which indicates the status/setup
     * to be returned */
        CSL_UartHwStatusQuery    myQuery,
        /** Placeholder to return the status; @a void* casted */
        void                     *response
);
/**
@}*/

/**
@defgroup EXAMPLE Example Usage in an ISR
@ingroup CSL_UART_API
@{

In order to clarify on the usage of some of the CSL APIs, a skeleton is presented below \n

Example: Interrupt driven Low level Driver\n
Main routine\n
Void main(){\n
        CSL_UartHwSetup hwSetup = CSL_UART_HWSETUP_DEFAULTS;\n
        CSL_UartObj              UARTObj,\n
        CSL_UartNum              uartNum,\n
        CSL_OpenMode             openMode,\n
        CSL_Status               stat;\n
// Initialize system before making any module calls specific calls \n
If (CSL_sysInit() == CSL_SOK){\n
           CSL_uartInit();\n
}\n
// Open the Module with default configuration for setup \n
// Interrupts enabled include CSL_UART_EVENT_RHR,CSL_UART_EVENT_RHR,CSL_UART_EVENT_LINESTS,\n
CSL_UART_EVENT_MODEMSTS
hUart = CSL_uartOpen(&UARTObj,CSL_UART_0,CSL_EXCLUSIVE,&hwSetup,&stat);\n
if (hUart == NULL)\n
   exit;\n
} // End of Main \n
Interrupt Service Routine\n
---------------------------\n
Isr_enter{\n
CSL_UartIntrSource intrSource;\n
Uin16 status;\n
CSL_BitMask16 suppStatus;\n
        CSL_uartGetHwStatus(hUart, CSL_UART_QUERY_EVENTS,&intrSource);\n
      Switch (intrSource) {\n
  Case CSL_UART_INTRSOURCE_NONE: \n
   // Check if it is the wakeup interrupt\n
   CSL_uartGetHwStatus(hUart,CSL_UART_QUERY_SUPPSTATUS ,&status);\n
   If (status & CSL_UART_SUPPSTATUS_RCD_WAKE_UP)\n
      WakeupFxn(); // Call appropriate fxn for the wakeup Event\n

        Case CSL_UART_INTRSOURCE_LINESTS: \n
        // Read LineStatus Register \n
  CSL_uartGetHwStatus(hUart,CSL_UART_QUERY_LINESTATUS,&suppStatus);\n

  Case CSL_UART_INTRSOURCE_RHR :\n

        }\n

} // ISR Exit \n

@}
*/

#ifdef __cplusplus
}
#endif

#endif



