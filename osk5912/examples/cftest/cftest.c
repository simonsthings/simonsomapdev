/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== cftest.c ========
 */
#include <stdio.h>


unsigned char id[512];
unsigned char buf[512];
unsigned char str[64];


/* ------------------------------------------------------------------------ *
 *  CFlash User defined options                                             *
 * ------------------------------------------------------------------------ */

/* Define which data access size to use */
//#define MODE_16BIT
#define MODE_8BIT


/* ------------------------------------------------------------------------ *
 *  CFlash Registers                                                        *
 * ------------------------------------------------------------------------ */
#define CF_MEM_BASE     0x08000000
#define CF_ATTR_BASE    0x08000800
#define CF_IO_BASE      0x08001000
#define CF_DATA16       *( ( volatile unsigned short * )( CF_IO_BASE + 0x1f0 ) )
#define CF_DATA8        *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f0 ) )
#define CF_SECCNT       *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f2 ) )
#define CF_LBA7_0       *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f3 ) )
#define CF_LBA15_8      *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f4 ) )
#define CF_LBA23_16     *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f5 ) )
#define CF_LBA27_24     *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f6 ) )
#define CF_CMD          *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f7 ) )
#define CF_STATUS       *( ( volatile unsigned char * ) ( CF_IO_BASE + 0x1f7 ) )
#define CF_REG01        *( ( volatile unsigned short * )( CF_IO_BASE + 0x1f0 ) )
#define CF_REG23        *( ( volatile unsigned short * )( CF_IO_BASE + 0x1f2 ) )
#define CF_REG45        *( ( volatile unsigned short * )( CF_IO_BASE + 0x1f4 ) )
#define CF_REG67        *( ( volatile unsigned short * )( CF_IO_BASE + 0x1f6 ) )


#define GPIO_NUMBER       62
#define GPIO_GROUP        GPIO_NUMBER / 16
#define GPIO_PIN          GPIO_NUMBER % 16
unsigned int GPIO_BASE[ ] = { 0xFFFBE400, 0xFFFBEC00, 0xFFFBB400, 0xFFFBBC00 };
unsigned int INTR_BASE[ ] = { 0xFFFECB00, 0xFFFE0100, 0xFFFE0100, 0xFFFE0100 };
unsigned int INTR_MASK[ ] = { 0x00004000, 0x00000100, 0x00000200, 0x00010000 };
unsigned int INTR_IRQ[ ]  = { 0xFFFECB10, 0xFFFE0010, 0xFFFE0010, 0xFFFE0010 };
unsigned int INTR_ILR[ ]  = { 0xFFFECB54, 0xFFFE003C, 0xFFFE0040, 0xFFFE005C };
unsigned int INTR_CODE[ ] = { 14, 40, 41, 48 };
unsigned int gpio_base_addr;
unsigned int gpio_intr_base;
unsigned int gpio_intr_mask;
unsigned int gpio_intr_irq;
unsigned int gpio_intr_ilr;
unsigned int gpio_intr_code;


/* ------------------------------------------------------------------------ *
 *  Wait loop                                                               *
 * ------------------------------------------------------------------------ */
void wait( unsigned int cycles )
{
    while ( cycles-- > 0 );
}

/* ------------------------------------------------------------------------ *
 *  Issue CFlash command                                                    *
 * ------------------------------------------------------------------------ */
void CF_command( short cmd, unsigned long sec )
{
    #ifdef MODE_16BIT   // Write command as 16 bit values
        CF_REG23    = ( sec & 0xff ) << 8 | 0x01;
        CF_REG45    = ( sec & 0xffff00 ) >> 8;
        CF_REG67    = ( sec & 0xf000000 ) >> 24 | 0xe0 | ( cmd << 8 );
    #elif defined( MODE_8BIT )     // Write command as 8 bit values
        CF_SECCNT   = 0x01;
        CF_LBA7_0   = ( sec & 0xff );
        CF_LBA15_8  = ( sec & 0xff00 ) >> 8;
        CF_LBA23_16 = ( sec & 0xff0000 ) >> 16;
        CF_LBA27_24 = ( ( sec & 0xf000000 ) >> 24 ) | 0xe0;
        CF_CMD = cmd;
    #endif

    /* Wait while CF is busy */
    while ( ( CF_STATUS & 0x80 ) != 0 );
}

/* ------------------------------------------------------------------------ *
 *  Initialize CFlash interrupts                                            *
 * ------------------------------------------------------------------------ */
void CF_initInterrupt( )
{
    gpio_base_addr = GPIO_BASE[GPIO_GROUP];
    gpio_intr_base = INTR_BASE[GPIO_GROUP];
    gpio_intr_mask = INTR_MASK[GPIO_GROUP];
    gpio_intr_irq  = INTR_IRQ[GPIO_GROUP];
    gpio_intr_ilr  = INTR_ILR[GPIO_GROUP];
    gpio_intr_code = INTR_CODE[GPIO_GROUP];

    /* Reset GPIO module */
    *( volatile unsigned short* )( gpio_base_addr + 0x10 ) = 2;

    /* Initialize GPIO to accept falling edge interupt signals */
    /* Set GPIO to input */
    *( volatile unsigned short* )( gpio_base_addr + 0x34 ) |= ( 1 << GPIO_PIN );

    if ( GPIO_PIN < 8 )
    {
        /* GPIO edge tigger select - falling edge tigger */
        *( volatile unsigned short* )( gpio_base_addr + 0x38 )
                                = ( unsigned short )( 1 << ( GPIO_PIN * 2 ) );
    }
    else if ( GPIO_PIN >= 8 )
    {
        /* GPIO edge tigger select - falling edge tigger */
        *( volatile unsigned short* )( gpio_base_addr + 0x3C )
                                            = 1 << ( ( GPIO_PIN - 8 ) * 2 );
    }

    /* GPIO interrupt enable */
    *( volatile unsigned short* )( gpio_base_addr + 0x1C ) = ( 1 << GPIO_PIN );

    /* GPIO set interrupt enable */
    *( volatile unsigned short* )( gpio_base_addr + 0xDC ) = ( 1 << GPIO_PIN );

    /* Deassert GPIO interrupt status */
    *( volatile unsigned short* )( gpio_base_addr + 0x18 ) |= ( 1 << GPIO_PIN );
    *( volatile unsigned short* )( gpio_base_addr + 0x20 ) |= ( 1 << GPIO_PIN );

    /* Clear Enabled IRQs */
    *( volatile unsigned short* )( gpio_base_addr + 0x1C ) |= ( 1 << GPIO_PIN );
    *( volatile unsigned short* )( gpio_base_addr + 0x24 ) |= ( 1 << GPIO_PIN );

    /* Clear IRQ1_GPIO interrupt on the MPU interrupt handler */
    *( volatile unsigned int* )( gpio_intr_base ) &= ~gpio_intr_mask;

    /* Unmask IRQ1_GPIO interrupt on the MPU interrupt handler */
    *( volatile unsigned int* )( gpio_intr_base + 4 ) &= ~gpio_intr_mask;

    /* Set Interrupt ILR - to level sensitive */
    *( volatile unsigned int* )( gpio_intr_ilr ) = 2;
}


/* ------------------------------------------------------------------------ *
 *  Check for CFlash interrupts                                             *
 * ------------------------------------------------------------------------ */
int CF_checkInterrupt( )
{
    /* Read the latest MPU interrupt source */
    unsigned int itr = *( volatile unsigned int* )( gpio_intr_base );

    /* Check for the GPIO interrupt */
    if ( ( itr & gpio_intr_mask ) == gpio_intr_mask )
        return 1;
    else
        return 0;
}

/* ------------------------------------------------------------------------ *
 *  Clear CFlash interrupts                                                 *
 * ------------------------------------------------------------------------ */
void CF_clearInterrupt( )
{
    /* Deassert GPIO interrupt status */
    *( volatile unsigned short* )( gpio_base_addr + 0x18 ) |= ( 1 << GPIO_PIN );
    *( volatile unsigned short* )( gpio_base_addr + 0x20 ) |= ( 1 << GPIO_PIN );

    /* Clear Enabled IRQs */
    *( volatile unsigned short* )( gpio_base_addr + 0x1C ) |= ( 1 << GPIO_PIN );
    *( volatile unsigned short* )( gpio_base_addr + 0x24 ) |= ( 1 << GPIO_PIN );

    /* Clear the MPU interrupt handler for GPIO interrupts */
    *( volatile unsigned int* )( gpio_intr_base ) &= ~gpio_intr_mask;
    *( volatile unsigned int* )( gpio_intr_base + 0x18 ) = 1;

    wait( 100 );
}

/* ------------------------------------------------------------------------ *
 *  Cflash initialization                                                   *
 * ------------------------------------------------------------------------ */
void CF_init( )
{
    OSK5912_init( 192 );

    /* ARM_IDLECT2 - Enabling Clocks - same as bsl */
    *( volatile unsigned short * )0xFFFECE08 = 0x0ACF;
                                    // 0x0800 - EN_CKOUT_ARM
                                    // 0x0200 - ?? this bit is reserved
                                    // 0x0080 - EN_TIMCK
                                    // 0x0040 - EN_APICK
                                    // 0x0008 - EN_LCDCK
                                    // 0x0004 - EN_PERCK
                                    // 0x0002 - EN_XORPCK
                                    // 0x0001 - EN_WDTCK

    /* ARM_IDLECT3 - enabling Traffic Controller Clocks */
    *( volatile unsigned short * )0xFFFECE24 = 0x0015;
                                    // 0x0010 - EN_TC2_CK
                                    // 0x0004 - EN_TC1_CK
                                    // 0x0001 - EN_OCPI_CK

    /* Set up timing for CS2 */
    *( volatile  unsigned int * )0xFFFECC18 = 0
            | ( 0 << 20 )           // Bus Width: 16-bit
            | ( 4 << 16 )           // 4 Read mode: Asynchronous Read
            | ( 10 << 12 )          // Page/WE wait states
            | ( 1 << 8 )            // 1 Write wait states
            | ( 9 << 4 )            // 7-9 Read wait states
            | ( 3 << 0 )            // 1 Ref. Clk = Clk/6   ( 3 )
            ;

    /* ADvance EMIFS Config for CS2 */
    *( volatile  unsigned int * )0xFFFECC58 = 0x44;

    /* Dynamic wait state enable for CS2 */
    *( volatile unsigned int* )0xFFFECC40 |= 0x1;

    /* Enable CF on CS1 */
    *( ( volatile unsigned short * )0xfffe2802 ) = 0xb;

    /* Toggle CF reset - OFF */
    *( ( volatile unsigned short * )0xfffe2804 ) = 0x1;
    wait( 100000 );

    /* Toggle CF reset - ON */
    *( ( volatile unsigned short * )0xfffe2804 ) = 0x0;
    wait( 100000 );

    /* Access attribute memory to put card in I/O mode w/task file at 0x1f0 */
    *( ( volatile unsigned char * ) CF_ATTR_BASE + 0x200 ) = 0x42;

    /* Initialize CF interrupts */
    CF_initInterrupt( );
    wait( 200000 );
}

/* ------------------------------------------------------------------------ *
 *  CFlash id check                                                         *
 * ------------------------------------------------------------------------ */
int CF_identify( )
{
    int i, status;
    unsigned short *pdata;
    unsigned long size;

    status = 0;

    /* Issue read identifier command */
    CF_command( 0xec, 0x00000000 );

    /* Read ID string */
    memset( id, 0, 16 );
    pdata = ( unsigned short * )id;
    for ( i = 0; i < 256; i++ ) {
        *pdata++ = CF_DATA16;
    }

    /* Check for valid identifier */
    printf( "CF Identifier:   0x%x\n", ( id[0] << 8 ) + id[1] );
    if ( !( ( id[0] == 0x8a ) && ( id[1] == 0x84 ) ) )
    {
        printf( "Invalid CF identifier found\n" );
        return 1;
    }

    /* Print model number */
    for ( i = 0; i < 40; i+=2 )
    {
        str[i] = id[i + 55];
        str[i + 1] = id[i + 54];
    }
    str[40] = 0;
    printf( "Model number:    [%s]\n", str );

    /* Print serial number */
    for ( i = 0; i < 20; i+=2 )
    {
        str[i] = id[i + 21];
        str[i + 1] = id[i + 20];
    }
    str[20] = 0;
    printf( "Serial number:   [%s]\n", str );

    /* Print capacity */
    size = ( ( id[123] << 24 ) + ( id[122] << 16 ) + ( id[121] << 8 ) + ( id[120] ) ) * 512;
    printf( "Capacity:        %d bytes\n", size );
    printf( "\n" );

    return status;
}

/* ------------------------------------------------------------------------ *
 *  CFlash test                                                             *
 * ------------------------------------------------------------------------ */
int CF_test( )
{
    int n, i;
    unsigned short *pdata, data, compare, status;

    CF_clearInterrupt( );

    /* Fill sectors with data */
    for ( n = 1; n < 2; n++ )
    {
        /* Check initial interrupt state */
        if ( CF_checkInterrupt( ) == 1 )
        {
            CF_clearInterrupt( );
            printf( "*** Interrupt asserted before operation started\n" );
            status = 1;
        }

        /* Set transfer parameters */
        CF_command( 0x30, n );

        /* Transfer data */
        for ( i = 0; i < 512; i += 2 )
        {
            CF_DATA16 = i + ( n * 512 );
        }

        /* Wait for interrupt to propagate from GPIO to interrupt handler */
        wait( 300000 );

        /* Check that interrupt was asserted */
        if ( CF_checkInterrupt( ) == 1 )
             CF_clearInterrupt( );
        else
        {
            printf( "*** Interrupt NOT detected\n" );
            status = 1;
        }
    }

    /* Verify data */
    status = 0;
    for ( n = 1; n < 2; n++ )
    {
        /* Check initial interrupt state */
        if ( CF_checkInterrupt( ) == 1 )
        {
            CF_clearInterrupt( );
            printf( "*** Interrupt asserted before operation started\n" );
            status = 1;
        }

        /* Set transfer parameters */
        printf( "Testing sector %d\n", n );
        CF_command( 0x20, n );

        /* Check that interrupt was asserted */
        if ( CF_checkInterrupt( ) == 1 )
             CF_clearInterrupt( );
        else
        {
            printf( "*** Interrupt NOT detected\n" );
            //status = 1;
        }

        /* Read and verify data */
        pdata = ( unsigned short * )buf;
        for ( i = 0; i < 512; i += 2 )
        {
            /* Read data */
            data = CF_DATA16;
            *pdata++ = data;
            compare = i + ( n * 512 );

            /* Print data */
            if ( ( i & 0xf ) == 0 )
                printf( "%04x:  ", compare );
            printf( "%04x ", data );
            if ( ( i & 0xe ) == 0xe )
                printf( "\n" );

            /* Compare data */
            if ( data != compare )
                status = 1;
        }
        printf( "\n" );
    }
    return status;
}

/* ------------------------------------------------------------------------ *
 *  CFlash test                                                             *
 * ------------------------------------------------------------------------ */
void main( )
{
    int status = 0;

    /* Initialize the CF interface */
    CF_init( );

    /* Read CF card identification string */
    if ( CF_identify( ) != 0 )
        status |= 1;

    /* Write & verify a test pattern on the Compact Flash
     * NOTE: this test is destructive to any preformatted Compact Flash Card
     * since it overwrites the first sector. */
    if ( CF_test( ) != 0 )
        status |= 2;

    /* Print test results */
    if ( status == 0 )
    {
        printf( "\n*** ALL Tests Passed ***\n" );
    }
    else
    {
        if ( status & 1 )
            printf( "ERROR: CF identify failed\n" );
        if ( status & 2 )
            printf( "ERROR: CF read/write test failed\n" );
    }

    /* Software breakpoint returns to Code Composer */
    asm( " .long 0xE1200070" );
    while( 1 );
}
