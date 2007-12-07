/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_uart.c ========
 *  UART module for the OSK5912
 */

#include <osk5912.h>
#include <osk5912_uart.h>

/* ------------------------------------------------------------------------ *
 *  UART lookup table                                                       *
 *      Setup an address lookup table                                       *
 * ------------------------------------------------------------------------ */
static Uint32 osk5912_uart_lookup_table[OSK5912_UART_NUM_UARTS] = {
    ( Uint32 )UART1_BASE_ADDR,
    ( Uint32 )UART2_BASE_ADDR,
    ( Uint32 )UART3_BASE_ADDR
};

/* ------------------------------------------------------------------------ *
 *  OSK5912_UART_rset( hUart, regnum, regval )                              *
 *      Set a UART register                                                 *
 * ------------------------------------------------------------------------ */
void OSK5912_UART_rset( OSK5912_UART_Handle hUart, Int16 regnum, Uint8 regval )
{
    Uint8* pdata;
    Int16 lcr;

    /* Determine if the register access needs any other setup to access */
    if ( regnum & UART_MODE_LCR_80 )
    {
        lcr = OSK5912_UART_rget( hUart, UART_LCR );
        OSK5912_UART_rset( hUart, UART_LCR, lcr | 0x80 );
    }
    if ( regnum & UART_MODE_LCR_BF )
    {
        lcr = OSK5912_UART_rget( hUart, UART_LCR );
        OSK5912_UART_rset( hUart, UART_LCR, 0xBF );
    }

    /* Set address of registers to write */
    pdata = ( Uint8* )( osk5912_uart_lookup_table[hUart] + ( regnum & 0xFF ) );

    /* Write value to address */
    *pdata = regval & 0xff;

    /* Return LCR reg if necessary */
    if ( regnum & 0x3000 )
        OSK5912_UART_rset( hUart, UART_LCR, lcr );
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_UART_rget( hUart, regnum )                                      *
 *      Get the value of a UART register                                    *
 * ------------------------------------------------------------------------ */
Uint8 OSK5912_UART_rget( OSK5912_UART_Handle hUart, Int16 regnum )
{
    Uint8* pdata;
    Uint8 regval;
    Int16 lcr;

    /* Determine if the register access need any other setup to access */
    if ( regnum & UART_MODE_LCR_80 )
    {
        lcr = OSK5912_UART_rget( hUart, UART_LCR );
        OSK5912_UART_rset( hUart, UART_LCR, lcr | 0x80 );
    }
    if ( regnum & UART_MODE_LCR_BF )
    {
        lcr = OSK5912_UART_rget( hUart, UART_LCR );
        OSK5912_UART_rset( hUart, UART_LCR, 0xBF );
    }

    /* Set address of registers to read */
    pdata = ( Uint8* )( osk5912_uart_lookup_table[hUart] + ( regnum & 0xFF ) );

    /* Read value */
    regval = *pdata;

    /* Return LCR reg if necessary */
    if ( regnum & ( UART_MODE_LCR_BF | UART_MODE_LCR_80 ) )
        OSK5912_UART_rset( hUart, UART_LCR, lcr );

    /* Return registers value */
    return regval;
}

/* ------------------------------------------------------------------------ *
 *  resetUart( hUart )                                                      *
 *      Resets the specific UART                                            *
 *      Sets IER = 0                                                        *
 *      Sets FCR = 0                                                        *
 *      Holds UART in reset mode                                            *
 * ------------------------------------------------------------------------ */
void resetUart( OSK5912_UART_Handle hUart )
{
    /* Reset Sequence */
    OSK5912_UART_rset( hUart, UART_EFR, 0x10 );
    OSK5912_UART_rset( hUart, UART_IER, 0 );
    OSK5912_UART_rset( hUart, UART_MCR, 0 );
    OSK5912_UART_rset( hUart, UART_EFR, 0 );
    OSK5912_UART_rset( hUart, UART_LCR, 0 );
    OSK5912_UART_rset( hUart, UART_MDR1, UART_MDR1_MODE_SELECT_DISABLE );

    /* Assert Soft Reset */
    OSK5912_UART_rset( hUart, UART_SYSC, UART_SYSC_SOFTRESET );

    /* Check for Soft Reset to be done */
    while ( ! ( OSK5912_UART_rget( hUart, UART_SYSS ) & UART_SYSS_RESETDONE ) );
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_UART_open( devid, baudrate, config )                            *
 *      Initialize UART and return handle                                   *
 * ------------------------------------------------------------------------ */
OSK5912_UART_Handle
OSK5912_UART_open( Int16 devid, Int16 baudrate, OSK5912_UART_Config *config )
{
    /* Temp registers variables */
    Int16 efr;

    /* Assign handle */
    OSK5912_UART_Handle hUart = devid;

    /* ULPD - request UART 1 & 2 & 3 48Mhz clocks */
    WriteField( ULPD_SOFT_REQ, 7, 0x0E00 );

    /* Wait for the ULPD to setup the clocks  */
    OSK5912_waitusec( 100 );

    /* Reset the UART from any previous state */
    resetUart( hUart );

    /* Set IER, FCR, LCR, MCR regs */
    efr = OSK5912_UART_rget( hUart, UART_EFR );
    OSK5912_UART_rset( hUart, UART_EFR, efr | 0x10 );
    OSK5912_UART_rset( hUart, UART_IER, config->reg_ier );
    OSK5912_UART_rset( hUart, UART_FCR, config->reg_fcr );
    OSK5912_UART_rset( hUart, UART_LCR, config->reg_lcr );
    OSK5912_UART_rset( hUart, UART_MCR, config->reg_mcr );
    OSK5912_UART_rset( hUart, UART_EFR, efr );

    /* Disable MDR1 */
    OSK5912_UART_rset( hUart, UART_MDR1, UART_MDR1_MODE_SELECT_DISABLE );

    /* Set Baudrate */
    OSK5912_UART_rset( hUart, UART_DLL, baudrate & 0xff );
    OSK5912_UART_rset( hUart, UART_DLH, ( baudrate >> 8 ) & 0xff );

    /* Set MDR1 - enable UART */
    if ( baudrate <= UART_BAUD_RATE_460800 )
        OSK5912_UART_rset( hUart, UART_MDR1, UART_MDR1_MODE_SELECT_13X_MODE );
    else
        OSK5912_UART_rset( hUart, UART_MDR1, UART_MDR1_MODE_SELECT_16X_MODE );

    /* Return Handle */
    return hUart;
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_UART_rcvReady( hUart )                                          *
 *      Returns:                                                            *
 *          0 for not ready to recv                                         *
 *          1 for ready to recv                                             *
 * ------------------------------------------------------------------------ */
Uint8 OSK5912_UART_rcvReady( OSK5912_UART_Handle hUart )
{
    /* Check if there is any data in RX FIFO */
    return OSK5912_UART_rget( hUart, UART_LSR ) & UART_LSR_RX_FIFO_E;
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_UART_getChar( hUart )                                           *
 *      Get one character of data from the UART                             *
 * ------------------------------------------------------------------------ */
Uint8 OSK5912_UART_getChar( OSK5912_UART_Handle hUart )
{
    /* Returns the first character in the RX_FIFO */
    return OSK5912_UART_rget( hUart, UART_RHR );
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_UART_xmtReady( hUart )                                          *
 *      Returns:                                                            *
 *          0 for not ready to send                                         *
 *          1 for ready to send                                             *
 * ------------------------------------------------------------------------ */
Uint8 OSK5912_UART_xmtReady( OSK5912_UART_Handle hUart )
{
    /* Check if ready to send any data in TX FIFO */
    return OSK5912_UART_rget( hUart, UART_LSR ) & UART_LSR_TX_FIFO_E;
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_UART_putChar( hUart, data )                                     *
 *      Send one character of data to the UART                              *
 * ------------------------------------------------------------------------ */
void OSK5912_UART_putChar( OSK5912_UART_Handle hUart, Uint8 data )
{
    /* Transfer a new byte */
    OSK5912_UART_rset( hUart, UART_THR, data );
}
