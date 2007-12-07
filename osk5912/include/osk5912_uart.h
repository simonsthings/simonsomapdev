/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_uart.h ========
 *  UART interface on the OSK5912
 */

#ifndef OSK5912_UART_
#define OSK5912_UART_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ *
 *  UART Parameter Definitions                                              *
 * ------------------------------------------------------------------------ */
#define OSK5912_UART_CONFIGREGS     4
#define OSK5912_UART_NUM_UARTS      3

/* ------------------------------------------------------------------------ *
 *  UART DEVICE ID                                                          *
 * ------------------------------------------------------------------------ */
typedef enum UART_ID
{
    OSK5912_UART1 = 0,
    OSK5912_UART2,
    OSK5912_UART3
} UART_ID;

/* ------------------------------------------------------------------------ *
 *  UART BAUD RATE Divisors ( DLL & DLH )                                   *
 * ------------------------------------------------------------------------ */
/* MODR1_MODE_SELECT = UART_16X_MODE */
#define UART_BAUD_RATE_300                  10000
#define UART_BAUD_RATE_600                  5000
#define UART_BAUD_RATE_1200                 2500
#define UART_BAUD_RATE_2400                 1250
#define UART_BAUD_RATE_4800                 625
#define UART_BAUD_RATE_9600                 313
#define UART_BAUD_RATE_14400                208
#define UART_BAUD_RATE_19200                156
#define UART_BAUD_RATE_28800                104
#define UART_BAUD_RATE_38400                78
#define UART_BAUD_RATE_57600                52
#define UART_BAUD_RATE_115200               26
#define UART_BAUD_RATE_230400               13
/* MODR1_MODE_SELECT = UART_13X_MODE */
#define UART_BAUD_RATE_460800               8
#define UART_BAUD_RATE_921600               4
#define UART_BAUD_RATE_1834200              2
#define UART_BAUD_RATE_3686400              1

/* ------------------------------------------------------------------------ *
 *  UART DATA STRUCTURES                                                    *
 * ------------------------------------------------------------------------ */

/* UART handle */
typedef Int16 OSK5912_UART_Handle;

/* Config structure for the OSK5912 UART */
typedef struct
{
    Uint8 reg_ier;
    Uint8 reg_fcr;
    Uint8 reg_lcr;
    Uint8 reg_mcr;
} OSK5912_UART_Config;

/* ------------------------------------------------------------------------ *
 *  PROTOTYPES                                                              *
 * ------------------------------------------------------------------------ */

/* Initialize UART and return handle */
OSK5912_UART_Handle
OSK5912_UART_open( Int16 devid, Int16 baudrate, OSK5912_UART_Config *Config );

/* Set a UART register */
void OSK5912_UART_rset( OSK5912_UART_Handle hUart, Int16 regnum, Uint8 regval );

/* Get the value of a UART register */
Uint8 OSK5912_UART_rget( OSK5912_UART_Handle hUart, Int16 regnum );

/* Check if the UART is ready to receive data */
Uint8 OSK5912_UART_rcvReady( OSK5912_UART_Handle hUart );

/* Get one character of data from the UART */
Uint8 OSK5912_UART_getChar( OSK5912_UART_Handle hUart );

/* Check if the UART is ready to send data */
Uint8 OSK5912_UART_xmtReady( OSK5912_UART_Handle hUart );

/* Send one character of data to the UART */
void OSK5912_UART_putChar( OSK5912_UART_Handle hUart, Uint8 data );

#ifdef __cplusplus
}
#endif

#endif
