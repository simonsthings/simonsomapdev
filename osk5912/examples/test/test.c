/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== test.c ========
 */
#include <stdio.h>
#include <osk5912.h>
#include <osk5912_uart.h>
#include <osk5912_flash.h>
#include <osk5912_i2c.h>
#include <osk5912_aic23.h>
#include <osk5912_power.h>
#include <osk5912_enet.h>

/* ------------------------------------------------------------------------ *
 *  Internal Testing Variables                                              *
 * ------------------------------------------------------------------------ */
Uint8 src[64], dst[70];
Uint32 buffer32[256];
Uint16 buffer8[256];

/* Codec configuration settings */
OSK5912_AIC23_Config config =
{
    0x0017,  /* 0 OSK5912_AIC23_LEFTINVOL  Left line input channel volume */
    0x0017,  /* 1 OSK5912_AIC23_RIGHTINVOL Right line input channel volume */
    0x00d8,  /* 2 OSK5912_AIC23_LEFTHPVOL  Left channel headphone volume */
    0x00d8,  /* 3 OSK5912_AIC23_RIGHTHPVOL Right channel headphone volume */
    0x0011,  /* 4 OSK5912_AIC23_ANAPATH    Analog audio path control */
    0x0000,  /* 5 OSK5912_AIC23_DIGPATH    Digital audio path control */
    0x0000,  /* 6 OSK5912_AIC23_POWERDOWN  Power down control */
    0x0043,  /* 7 OSK5912_AIC23_DIGIF      Digital audio interface format */
    0x0081,  /* 8 OSK5912_AIC23_SAMPLERATE Sample rate control */
    0x0001   /* 9 OSK5912_AIC23_DIGACT     Digital interface activation */
};

/* Pre-generated sine wave data, 16-bit signed samples */
Int16 sinetable[] =
{
    0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
    0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
    0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
    0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
    0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
    0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
};

/* ------------------------------------------------------------------------ *
 *  Testing Function                                                        *
 * ------------------------------------------------------------------------ */
void TEST_execute( Int16 ( *funchandle )( ), char *testname, Int16 ledmask,
                    Int16 insertdelay )
{
    Int16 status;

    /* Display test ID */
    printf( "%02d  Testing %s...\n", ledmask, testname );

    /* Call test function */
    status = funchandle( );

    /* Check for test fail */
    if ( status > 0 )
    {
        /* Print error message */
        printf( "     FAIL... error code %d... quitting\n", status, testname );

        /* Software Breakpoint to Code Composer */
        asm( " .long 0xE1200070" );
    }
    else
    {
        /* Print error message */
        printf( "    PASS\n", testname );
    }
}

/* ------------------------------------------------------------------------ *
 *  Test LEDS                                                               *
 * ------------------------------------------------------------------------ */
Int16 TEST_leds( )
{
    Uint16 i;
    Uint16 loop = 50;
    Uint16 wait = 100;

    /* Initialize LEDs D2 & D3 to be off */
    OSK5912_LED_init( );

    /* Toggle LEDs */
    OSK5912_LED_toggle( 0 );
    for ( i = 0 ; i < loop ; i++ )
    {
        OSK5912_LED_toggle( 0 );
        OSK5912_LED_toggle( 1 );
        OSK5912_waitmsec( wait );
    }

    /* Leaving LEDs on */
    OSK5912_LED_on( 0 );
    OSK5912_LED_on( 1 );

    return 0;
}

/* ------------------------------------------------------------------------ *
 *  Memory functions                                                        *
 * ------------------------------------------------------------------------ */
Int16 MEM_fill( Uint32 start, Uint32 len, Uint32 val )
{
    Uint32 i, end;

    /* Calculate end of range */
    end = start + len;

    /* Fill a range with a value */
    for ( i = start; i < end; i += 4 )
         *( Uint32* )i = val;

    /* Verify the data */
    for ( i = start; i < end; i += 4 )
        if ( *( Uint32* )i != val )
            return 1;

    return 0;
}

Int16 MEM_addr( Uint32 start, Uint32 len )
{
    Uint32 i, end;

    /* Calculate end of range */
    end = start + len;
    /* Fill the range with its address */
    for ( i = start; i < end; i += 4 )
         *( Uint32* )i  = i;

    /* Verify the data */
    for ( i = start; i < end; i += 4 )
        if ( *( Uint32* )i != i )
            return 1;

    return 0;
}

Int16 MEM_addrInv( Uint32 start, Uint32 len )
{
    Uint32 i, end;

    /* Calculate end of range */
    end = start + len;

    /* Fill the range with its address */
    for ( i = start; i < end; i += 4 )
         *( Uint32 * )i = ~i;

    /* Verify the data */
    for ( i = start; i < end; i += 4 )
        if ( *( Uint32 * )i != ( ~i ) )
            return 1;

    return 0;
}

Int16 MEM_walking( Uint32 add )
{
    Int16 i;
    Uint32 mask, *pdata;

    pdata = ( Uint32 * )add;

    /* Walking ones and zeros */
    mask = 1;
    for ( i = 0; i < 32; i++ )
    {
        /* Test one in bit position i */
        *pdata = mask;
        if ( *pdata != mask )
            return 1;

        /* Test zero in bit position i */
        *pdata = ~mask;
        if ( *pdata != ( ~mask ) )
            return 1;

        mask = mask << 1;
    }
    return 0;
}

Int16 MEM_test( Uint32 start, Uint32 len, Int16 patterntype )
{
    Int16 status = 0;

    if ( !patterntype )
    {
        /* Run the fill tests */
        status |= MEM_fill( start, len, 0x00000000 );
        status |= MEM_fill( start, len, 0x55555555 );
        status |= MEM_fill( start, len, 0xAAAAAAAA );
        status |= MEM_fill( start, len, 0xFFFFFFFF );
    }
    else
    {
        /* Run the address tests */
        status |= MEM_addr( start, len );
        status |= MEM_addrInv( start, len );
    }
    return status;
}

/* ------------------------------------------------------------------------ *
 *  Memory test                                                             *
 *      TEST_intMem( )                                                      *
 *      TEST_extMem( )                                                      *
 * ------------------------------------------------------------------------ */
Int16 TEST_intMem( )
{
    /* Check internal memory ( byte 0x20000000 to byte 0x2003E800 ) */
    if ( MEM_test( 0x20014000, 0x2A800, 0 ) )
        return 1;   // Failed Test
    if ( MEM_test( 0x20014000, 0x2A800, 1 ) )
        return 2;   // Failed Test
    if ( MEM_walking( 0x20014000 ) )
        return 3;   // Failed Test

    /* Passed */
    return 0;
}

Int16 TEST_extMem( )
{
    Uint32 length = 0x2000000;

    /* Check external memory ( byte 0x10000000 to byte 0x12000000 ) */
    if ( MEM_test( 0x10000000, length, 0 ) )
        return 1;   // Failed Test
    if ( MEM_test( 0x10000000, length, 1 ) )
        return 2;   // Failed Test
    if ( MEM_walking( 0x10000000 ) )
        return 3;   // Failed Test

    /* Passed */
    return 0;
}


/* ------------------------------------------------------------------------ *
 *  add_flash_loop( )                                                       *
 *      Adds a branch to self opcode to Flash.                              *
 * ------------------------------------------------------------------------ */
add_flash_loop( )
{
    Uint32 opcode = 0xEAFFFFFE;

    /* Add branch to self opcode in place of Interrupt Vector Table */
    OSK5912_FLASH_init( );
    OSK5912_FLASH_erase( OSK5912_FLASH_BASE, 0x20000 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+0, 4 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+4, 4 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+8, 4 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+12, 4 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+16, 4 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+20, 4 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+24, 4 );
    OSK5912_FLASH_write( (Uint32)&opcode, OSK5912_FLASH_BASE+28, 4 );
}

/* ------------------------------------------------------------------------ *
 *  TEST_flashMemory( )                                                     *
 *      Flash is divided into 128Kb pages                                   *
 *      To test each page, that page must be erased, before testing         *
 *      Only the first 128Kb page will be fully tested; the remaining pages *
 *      will have the inital portion of Flash tested.                       *
 * ------------------------------------------------------------------------ */
Int16 TEST_flashMemory( )
{
    Uint32 i, k, limit;
    Uint32 *p32;

    Uint32 current_page = 0;
    Uint32 total_pages = 0;
    Uint32 current_page_addr = OSK5912_FLASH_BASE;
    Uint32 current_addr = 0;
    Uint32 block_addr;

    /* Initialize Flash */
    OSK5912_FLASH_init( );

    /* Determine the total number of pages of Flash to test */
    total_pages = OSK5912_FLASH_getTotalPages( 2 );
    printf( "     Total pages: %d\n", total_pages );

/* ------------------------------------------------------------------------ *
 *  Erase Flash                                                             *
 * ------------------------------------------------------------------------ */
    printf( "     Erasing Flash ( please wait )\n" );
    OSK5912_FLASH_erase( OSK5912_FLASH_BASE, total_pages
                                            * OSK5912_FLASH_PAGESIZE );

/* ------------------------------------------------------------------------ *
 *  Program Flash                                                           *
 * ------------------------------------------------------------------------ */
    printf( "     Programming Flash\n" );

    /* Start Programming at beginning of Flash */
    current_page_addr = OSK5912_FLASH_BASE;
    for ( current_page = 0 ; current_page < total_pages ; current_page++ )
    {
        /* Point current address to start of the flash page */
        current_addr = current_page_addr;

        /* Test all the first page of each Flash chip */
        /* Test 1Kb of all other pages */
        if ( ( current_page == 0 ) || ( current_page == total_pages >> 1 ) )
            limit = 128;    // Test 128KB of first page
        else
            limit = 1;      // Test 1KB of each other page

        /* Point block address to start of the flash page */
        block_addr = current_page_addr;

        for ( i = 0 ; i < limit ; i++, block_addr += 1024 )
        {
            /* Create the pattern to write to Flash */
            p32 = buffer32;
            for ( k = 0 ; k < 256 ; k++, current_addr += 4 )
                *p32++ = current_addr;

            /* Write the pattern to Flash */
            OSK5912_FLASH_write( ( Uint32 )buffer32, block_addr, 1024 );
        }

        /* Increment block address to next page of flash */
        current_page_addr += OSK5912_FLASH_PAGESIZE;
    }

/* ------------------------------------------------------------------------ *
 *  Verify Flash                                                            *
 * ------------------------------------------------------------------------ */
    printf( "     Verifying Flash\n" );

    /* Start Verifying at beginning of Flash */
    current_page_addr = OSK5912_FLASH_BASE;
    for ( current_page = 0 ; current_page < total_pages ; current_page++ )
    {
        /* Point current address to start of the flash page */
        current_addr = current_page_addr;

        /* Test all the first page of each Flash chip */
        /* Test 1Kb of all other pages */
        if ( ( current_page == 0 ) || ( current_page == total_pages >> 1 ) )
            limit = 128;    // Test 128KB of first page
        else
            limit = 1;      // Test 1KB of each other page

        /* Point block address to start of the flash page */
        block_addr = current_page_addr;

        for ( i = 0 ; i < limit ; i++, block_addr += 1024 )
        {
            /* Read Flash contents */
            OSK5912_FLASH_read( block_addr, ( Uint32 )buffer32, 1024 );

            /* Verify the pattern is in Flash */
            p32 = buffer32;
            for ( k = 0 ; k < 256 ; k++, current_addr += 4 )
                if ( *p32++ != current_addr )
                    return current_page + 1;     // Test failed
        }

        /* Increment block address to next page of flash */
        current_page_addr += OSK5912_FLASH_PAGESIZE;
    }

    add_flash_loop();

    /* Test Passed */
    return 0;
}

/* ------------------------------------------------------------------------ *
 *  TEST_uart1( )                                                           *
 *      set up a loopback on uart1 and test                                 *
 * ------------------------------------------------------------------------ */
Int16 TEST_uart1( )
{
    Int16 i;
    OSK5912_UART_Handle hUart;
    OSK5912_UART_Config uartcfg;

    uartcfg.reg_ier = 0x00;  // IER - No interrupts enabled
    uartcfg.reg_fcr = 0x57;  // FCR - FIFO Mode, 16 character trigger level;
    uartcfg.reg_lcr = 0x03;  // LCR - 8 bits, no parity, 1 stop
    uartcfg.reg_mcr = 0x00;  // MCR

/* ------------------------------------------------------------------------ *
 *  Multiplex Pins for UART1 Test                                           *
 * ------------------------------------------------------------------------ */
    /* Configure OMAP5912 - disables previously defined pin muxes */
    PMUX_COMP_MODE_CTRL_0 = 0;

    /* Set pins USB.DP & USB.DM to be used by the USB */
    PMUX_USB_TRANSCEIVER_CTRL = 0x0000;

    /* Set pins UART1.TX & UART1.RX to be used by UART1 */
    WriteField( PMUX_FUNC_MUX_CTRL_9, 0, MASK18 );
    WriteField( PMUX_FUNC_MUX_CTRL_9, 1, MASK21 );

    /* Configure OMAP5910 - enables pin muxes */
    PMUX_COMP_MODE_CTRL_0 = 0x0000EAEF;

    /* A short delay is required to allow the pin muxing to complete */
    OSK5912_waitmsec( 20 );



    /* Open UART1 for testing at a 115200 baudrate */
    hUart = OSK5912_UART_open( OSK5912_UART1, UART_BAUD_RATE_115200, &uartcfg );

    /* Loop through 256 bytes */
    for ( i = 0 ; i < 256 ; i++ )
    {
        while ( ! OSK5912_UART_xmtReady( hUart ) );
        OSK5912_UART_putChar( hUart, i & 0xff );

        while ( ! OSK5912_UART_rcvReady( hUart ) );
        buffer8[i] = OSK5912_UART_getChar( hUart );
    }

    /* Verify data */
    for ( i = 0 ; i < 256 ; i++ )
        if ( buffer8[i] != ( i & 0xff ) )
            return i + 1;   // Failed

    return 0;   // Passed
}

/* ------------------------------------------------------------------------ *
 *  TEST_aic23( )                                                           *
 *      Test aic23 with a sine wave                                         *
 * ------------------------------------------------------------------------ */
Int16 TEST_aic23( )
{
    OSK5912_AIC23_CodecHandle hCodec;
    Int16 msec, sample, data;

    /* Length of sine wave table */
    Int16 sine_table_size = 48;

    /* Initialize I2C */
    OSK5912_I2C_init( );

    /* Start the codec */
    hCodec = OSK5912_AIC23_openCodec( 0, &config );

    if ( 1 )
    {
        /* Generate a 1KHz sine wave for 2.5 seconds */
        for ( msec = 0 ; msec < 2500 ; msec++ )
        {
            for ( sample = 0 ; sample < sine_table_size ; sample++ )
            {
                /* Send a sample to the left channel */
                while ( !OSK5912_AIC23_write16( hCodec, sinetable[sample] ) );

                /* Send a sample to the right channel */
                while ( !OSK5912_AIC23_write16( hCodec, sinetable[sample] ) );
            }
        }
    }
    else
    {
        /* Audio loopback */
        while( 1 )
        {
            while ( !OSK5912_AIC23_read16( hCodec, &data ) );
            while ( !OSK5912_AIC23_write16( hCodec, data ) );
        }
    }

    /* Close the codec */
    OSK5912_AIC23_closeCodec( hCodec );

    return 0;
}

/* ------------------------------------------------------------------------ *
 *  TEST_usbLoop( )                                                         *
 *      Test USB signals by muxing the USB.DP & USB.DM pins with UART 1     *
 * ------------------------------------------------------------------------ */
Int16 TEST_usbLoop( )
{
    Int16 i;
    OSK5912_UART_Handle hUart;
    OSK5912_UART_Config uartcfg;

    OSK5912_POWER_rset( 0x10, 0x55 );

    uartcfg.reg_ier = 0x00;  // IER - No interrupts enabled
    uartcfg.reg_fcr = 0x57;  // FCR - FIFO Mode, 16 character trigger level;
    uartcfg.reg_lcr = 0x03;  // LCR - 8 bits, no parity, 1 stop
    uartcfg.reg_mcr = 0x00;  // MCR

/* ------------------------------------------------------------------------ *
 *  Multiplex Pins for USB Test                                             *
 * ------------------------------------------------------------------------ */
    /* Configure OMAP5910 - disables previously defined pin muxes */
    PMUX_COMP_MODE_CTRL_0 = 0;

    /* Multiplex USB.DP & USB.DM pins to be UART1.TX & UART1.RX */
    PMUX_USB_TRANSCEIVER_CTRL = 0x0050;

    /* Disable the original UART1.RX & UART1.TX pins */
    WriteField( PMUX_FUNC_MUX_CTRL_9, 7, MASK18 );
    WriteField( PMUX_FUNC_MUX_CTRL_9, 0, MASK21 );

    /* Configure OMAP5910 - enables pin muxes */
    PMUX_COMP_MODE_CTRL_0 = 0x0000EAEF;

    /* A short delay is required to allow the pin muxing to complete */
    OSK5912_waitmsec( 20 );

    /* Open UART1 for testing at a 115200 baudrate */
    hUart = OSK5912_UART_open( OSK5912_UART1, UART_BAUD_RATE_115200, &uartcfg );

    /* Loop through 256 bytes */
    for ( i = 0 ; i < 256 ; i++ )
    {
        while ( ! OSK5912_UART_xmtReady( hUart ) );
        OSK5912_UART_putChar( hUart, i & 0xff );

        while ( ! OSK5912_UART_rcvReady( hUart ) );
        buffer8[i] = OSK5912_UART_getChar( hUart );
    }

    /* Verify data */
    for ( i = 0 ; i < 256 ; i++ )
        if ( buffer8[i] != ( i & 0xff ) )
            return i + 1;   // FAIL

    /* PASS */
    return 0;
}


/* ------------------------------------------------------------------------ *
 *  TEST_enet( )                                                            *
 *      Test Ethernet                                                       *
 * ------------------------------------------------------------------------ */
Int16 TEST_enet( )
{
    Uint16 i, intstatus, len, *pdata, pnum;
    volatile Uint16 status;
    Int16 returnval = 0;

    /* Check link */
    printf( "    Checking link\n" );
    OSK5912_waitusec( 200000 );
    if ( ( OSK5912_ENET_rget( SMC91C96_EPH_STATUS ) & 0x4000 ) != 0 )
    {
        printf( "    Link detected!\n" );
    }
    else
    {
        printf( "    Link NOT detected!\n" );
        return 1;
    }

    /* Clear MMU state */
    OSK5912_ENET_rset( SMC91C96_MMU_COMMAND, 0x40 );

    /* Enable auto release transmit mode */
    OSK5912_ENET_rset( SMC91C96_CONTROL, 0x0b10 );

    /* Enable receive, promiscuous mode, all-multicast */
    OSK5912_ENET_rset( SMC91C96_RCR, 0x0106 );

    /* Enable transmit and receive interrupts */
    OSK5912_ENET_rset( SMC91C96_INTERRUPT, 0x0000 ); // 0x0300

    /* Create source buffer */
    for ( i = 0; i < 64; i++ )
        src[i] = 32 + i;

    /* Clear destination buffer */
    for ( i = 0; i < 70; i++ )
        dst[i] = 0xff;

    /* Transmit data, issue allocate memory for TX */
    OSK5912_ENET_rset( SMC91C96_MMU_COMMAND, 0x20 );

    /* Wait for alloc to complete */
    while ( ( OSK5912_ENET_rget( SMC91C96_INTERRUPT ) & 0x08 ) == 0 );

    /* Set pnum */
    pnum = OSK5912_ENET_rget( SMC91C96_PNR ) & 0x3f;
    OSK5912_ENET_rset( SMC91C96_PNR, pnum << 8 );

    /* Set pointer to 0, write, auto-increment */
    OSK5912_ENET_rset( SMC91C96_POINTER, 0x4000 );

    /* Generate packet head */
    OSK5912_ENET_rset( SMC91C96_DATAL, 0x0000 );
    OSK5912_ENET_rset( SMC91C96_DATAL, 64 + 6 ); // 64 + packet frame

    /* Copy packet data */
    pdata = ( Uint16 * )&src;
    for ( i = 0; i < 32; i++ )
         OSK5912_ENET_rset( SMC91C96_DATAL, *pdata++ );

    /* Generate packet tail */
    OSK5912_ENET_rset( SMC91C96_DATAL, 0 );

    /* Enable transmission ( padded, full duplex ) */
    OSK5912_ENET_rset( SMC91C96_TCR, 0x8881 );

    /* Enqueue packet number to start transmission */
    OSK5912_ENET_rset( SMC91C96_MMU_COMMAND, 0xc0 );

    /* Wait for receive interrupt */
    do
    {
        intstatus = OSK5912_ENET_rget( SMC91C96_INTERRUPT );
    } while ( ( intstatus & 0x0001 ) == 0 );

    /* Configure data pointer for auto-inc reads  */
    OSK5912_ENET_rset( SMC91C96_POINTER, 0xe000 );

    /* Get status word */
    status = OSK5912_ENET_rget( SMC91C96_DATAL );

    /* Get packet length minus SMC91C96 packet frame and CRC */
    len = ( OSK5912_ENET_rget( SMC91C96_DATAL ) & 0x7ff ) - 6 - 4;

    /* Check data length */
    if ( len == 64 )
    {
        printf( "    %d bytes received\n", len );

        /* Read data */
        pdata = ( Uint16 * )&dst;
        for ( i = 0; i < 32; i++ )
             *pdata++ = OSK5912_ENET_rget( SMC91C96_DATAL );

        /* Check data */
        for ( i = 0; i < 64; i++ )
            if ( src[i] != dst[i] )
            {
                printf( "    Receive data does not match transmit data\n" );
                returnval = 1;
            }

        /* Remove RX packet */
        OSK5912_ENET_rset( SMC91C96_MMU_COMMAND, 0x80 );
    }
    else
    {
        printf( "    Wrong number of bytes received ( %d )\n", len );
        returnval = 1;
    }

    return returnval;
}

/* ------------------------------------------------------------------------ *
 *  TEST_enetEEPROM( )                                                      *
 *      Test Ethernet EEPROM                                                *
 * ------------------------------------------------------------------------ */
Int16 TEST_enetEEPROM( )
{
    Uint16 save, newval;
    volatile Int16 status;

    status = 0;

    /* Read the contents at address 0 */
    save = OSK5912_ENET_readEEPROM( 0 );

    /* Write address 0 with the inverse of its original contents */
    OSK5912_ENET_writeEEPROM( 0, ~save );

    /* Clear any data in the GENERAL register to ensure test validity */
    OSK5912_ENET_rset( SMC91C96_GENERAL, save );

    /* Read the value at address 0 back */
    newval = OSK5912_ENET_readEEPROM( 0 );

    /* Check result */
    if ( newval != ( ~save & 0xffff ) )
        status = 1;

    /* Restore address 0 to its initial value */
    OSK5912_ENET_writeEEPROM( 0, save );

    /* Status is not returned earlier so original contents will be written */
    /* back to EEPROM even if the test fails */
    return status;
}

/* ------------------------------------------------------------------------ *
 *  Run general test                                                        *
 * ------------------------------------------------------------------------ */
void main( )
{
    /* Initialize the board support library, must be called first */
    OSK5912_init( 192 );

    /* Run the tests sequentially */
    TEST_execute( TEST_leds,        "LEDs",                     1, 0 );
    TEST_execute( TEST_intMem,      "SRAM",                     2, 0 );
  //TEST_execute( TEST_enet,        "Ethernet Loopback",        3, 0 );
    TEST_execute( TEST_enetEEPROM,  "Ethernet EEPROM",          4, 0 );
  //TEST_execute( TEST_uart1,       "Uart 1 Loopback",          5, 0 );
  //TEST_execute( TEST_usbLoop,     "Usb Loopback",             6, 0 );
    TEST_execute( TEST_extMem,      "SDRAM",                    7, 0 );

    /* NOTE: Running the Flash test will erase the entire contents at the
     * beginning of FLASH.  If there is any image already loaded into Flash
     * that image will be erased.  The only way to insert the image is to
     * use an emulator and program that area of Flash */
  //TEST_execute( TEST_flashMemory, "Flash",                    8, 0 );
    TEST_execute( TEST_aic23,       "AIC23 Tone" ,              9, 0 );

    printf( "\n***ALL Tests Passed***\n" );

    /* Software Breakpoint to Code Composer */
    asm( " .long 0xE1200070" );
    while( 1 );
}
